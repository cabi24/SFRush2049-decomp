"""Shared global declarations for the game image: generate, check, audit names.

    python3 -m tools.conveyor.pipeline.shared_decls generate
    python3 -m tools.conveyor.pipeline.shared_decls check
    python3 -m tools.conveyor.pipeline.shared_decls names

Frontier plan, workstream E step 1 ("scalars first").  Every matched game
source (`src/blob/*.c`, `src/blob/groups/*/*.c`) carries its own copy of the
generated prelude plus hand overrides.  This tool reads all of them and
proposes ONE declaration per global address.  It edits nothing that exists:

  generate -> include/game_globals.h                       (proposal header)
              cloud/work/frontier/type_model/decl_conflicts.md
  check    -> per-source compatibility with the header that is on disk
              (totals on stdout, detail in type_model/decl_check.json)
  names    -> cloud/work/frontier/type_model/names_audit.md

Precedence when choosing a type (strongest first):

  1. retail access evidence: the load/store mnemonics retail code uses at the
     address (`type_model/refs.json`, produced by `scan_refs.py`) and the
     record arrays of `type_model/bases.json`;
  2. agreement among matched sources that USE the symbol with that type
     (a prelude declaration a source never references is not evidence);
  3. the generated default (the spelling the shared prelude carries).

A matched source proves its declaration is compatible with that function's
code, not that it is the true type.  `volatile` declarations are shaping
quirks: they are listed separately and never promoted into the header.

Standard library only, Python 3.9.  Output is deterministic (no timestamps),
so a rerun after the source set changes gives a reviewable diff.
"""
import argparse
import bisect
import collections
import json
import re
import struct
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[3]
TYPE_MODEL = REPO / "cloud/work/frontier/type_model"
HEADER = REPO / "include/game_globals.h"
CONFLICTS_MD = TYPE_MODEL / "decl_conflicts.md"
NAMES_MD = TYPE_MODEL / "names_audit.md"
CHECK_JSON = TYPE_MODEL / "decl_check.json"

IMAGE_BASE = 0x80086A50
DATA_START = 0x8010FD7C
RODATA_START = 0x80123870
IMAGE_END = 0x801249F0          # IMAGE_BASE + 647,072; refreshed from the image when present

# --------------------------------------------------------------------------
# C type model (only what extern declarations of globals need)
# --------------------------------------------------------------------------

# name -> (kind, width, sign)
BUILTIN = {
    "s8": ("int", 1, "s"), "u8": ("int", 1, "u"),
    "s16": ("int", 2, "s"), "u16": ("int", 2, "u"),
    "s32": ("int", 4, "s"), "u32": ("int", 4, "u"),
    "s64": ("int", 8, "s"), "u64": ("int", 8, "u"),
    "f32": ("float", 4, None), "f64": ("float", 8, None),
    # distinct C types with the representation of one of the above
    "char": ("int", 1, "u"),          # IDO: plain char is unsigned
    "long": ("int", 4, "s"), "unsigned long": ("int", 4, "u"),
    "void": ("void", 0, None),
}
_PRIMITIVE_WORDS = {"char", "short", "int", "long", "float", "double", "void",
                    "unsigned", "signed"}
_QUALIFIERS = {"const", "volatile"}
_STORAGE = {"extern", "static", "register"}
# multiset of primitive words -> canonical base (true typedef identities)
_PRIMITIVE_CANON = {
    ("char",): "char", ("char", "signed"): "s8", ("char", "unsigned"): "u8",
    ("short",): "s16", ("int", "short"): "s16", ("short", "signed"): "s16",
    ("short", "unsigned"): "u16", ("int", "short", "unsigned"): "u16",
    ("int",): "s32", ("signed",): "s32", ("int", "signed"): "s32",
    ("unsigned",): "u32", ("int", "unsigned"): "u32",
    ("long",): "long", ("int", "long"): "long", ("long", "signed"): "long",
    ("long", "unsigned"): "unsigned long",
    ("long", "long"): "s64", ("long", "long", "unsigned"): "u64",
    ("float",): "f32", ("double",): "f64", ("void",): "void",
}

TypeInfo = collections.namedtuple("TypeInfo", "base quals ptr dims fn")
TypeInfo.__doc__ = """base: canonical base type name; quals: frozenset of
const/volatile; ptr: pointer depth; dims: tuple of array bounds (None =
unsized, int, or the literal text of a macro bound); fn: True for a function
pointer or any declarator this parser does not model."""


def strip_comments(text):
    text = re.sub(r"/\*.*?\*/", " ", text, flags=re.S)
    return re.sub(r"//[^\n]*", " ", text)


def split_top(text, sep=","):
    """Split on `sep` outside (), [] and {}."""
    parts, depth, cur = [], 0, []
    for ch in text:
        if ch in "([{":
            depth += 1
        elif ch in ")]}":
            depth -= 1
        if ch == sep and depth == 0:
            parts.append("".join(cur))
            cur = []
        else:
            cur.append(ch)
    parts.append("".join(cur))
    return parts


_TOKEN = re.compile(r"\{.*\}|[A-Za-z_]\w*|0[xX][0-9A-Fa-f]+|\d+|\S", re.S)


def _split_specifiers(tokens, aliases):
    """Return (base, quals, rest_tokens) for a declaration's token list."""
    quals, prim, base, i = set(), [], None, 0
    while i < len(tokens):
        tok = tokens[i]
        if tok in _STORAGE:
            i += 1
        elif tok in _QUALIFIERS:
            quals.add(tok)
            i += 1
        elif tok in _PRIMITIVE_WORDS and base is None:
            prim.append(tok)
            i += 1
        elif tok in ("struct", "union", "enum") and base is None and not prim:
            i += 1
            tag = ""
            if i < len(tokens) and re.match(r"[A-Za-z_]", tokens[i]):
                tag = tokens[i]
                i += 1
            if i < len(tokens) and tokens[i].startswith("{"):
                i += 1
                base = "%s %s{...}" % (tok, tag) if tag else "%s {...}" % tok
            else:
                base = "%s %s" % (tok, tag)
        elif (base is None and not prim and re.match(r"[A-Za-z_]", tok)
              and i + 1 < len(tokens) and tokens[i + 1] not in (",", ";", "=", "[", ")")):
            base = tok
            i += 1
        else:
            break
    if prim:
        base = _PRIMITIVE_CANON.get(tuple(sorted(prim)), " ".join(prim))
    seen = set()
    while base in aliases and base not in seen:      # typedef s32 M2C_UNK;
        seen.add(base)
        base = aliases[base]
    return base, quals, tokens[i:]


def _parse_dim(text):
    text = text.strip()
    if not text:
        return None
    try:
        return int(text, 0)
    except ValueError:
        return text


def parse_declarator(text):
    """`*D_80123456[4]` -> (name, ptr, dims, fn, quals).  quals are those that
    bind to the object when it is a pointer (`char *const p`)."""
    text = text.split("=", 1)[0].strip()
    quals = set()
    if "(" in text:
        m = re.search(r"\(\s*\*+\s*([A-Za-z_]\w*)", text)
        name = m.group(1) if m else None
        if name is None:
            m = re.search(r"[A-Za-z_]\w*", text)
            name = m.group(0) if m else None
        return name, 0, (), True, quals
    m = re.match(r"^((?:\*|\s|const\b|volatile\b)*)([A-Za-z_]\w*)\s*((?:\[[^\]]*\]\s*)*)$", text)
    if not m:
        return None, 0, (), True, quals
    ptr = m.group(1).count("*")
    if ptr:
        # qualifiers after the last star qualify the object itself
        quals.update(re.findall(r"const|volatile", m.group(1).rsplit("*", 1)[1]))
    dims = tuple(_parse_dim(d) for d in re.findall(r"\[([^\]]*)\]", m.group(3)))
    return m.group(2), ptr, dims, False, quals


def parse_declaration(stmt, aliases=None):
    """Parse one declaration statement (without the trailing `;`).

    Returns a list of (name, TypeInfo).  Function prototypes are skipped."""
    aliases = aliases or {}
    tokens = _TOKEN.findall(stmt)
    base, quals, rest = _split_specifiers(tokens, aliases)
    if base is None or not rest:
        return []
    out = []
    for part in split_top(" ".join(rest)):
        part = part.strip()
        if not part:
            continue
        name, ptr, dims, fn, dq = parse_declarator(part)
        if name is None:
            continue
        if fn and not re.search(r"\(\s*\*", part):
            continue                                  # function prototype
        object_quals = set(dq) if ptr else set(quals) | set(dq)
        pointee = base
        if ptr and quals:
            pointee = " ".join(sorted(quals) + [base])    # `const char *p`
        out.append((name, TypeInfo(pointee, frozenset(object_quals), ptr, dims, fn)))
    return out


def spell(ti, name="$"):
    if ti.fn:
        return "%s (*%s)()" % (ti.base, name)
    quals = "".join(q + " " for q in sorted(ti.quals))
    dims = "".join("[%s]" % ("" if d is None else d) for d in ti.dims)
    return "%s%s %s%s%s" % (quals, ti.base, "*" * ti.ptr, name, dims)


def bare(ti):
    """The type without object qualifiers."""
    return ti._replace(quals=frozenset())


def is_struct_typed(ti):
    """True when the declaration needs a typedef this header does not own."""
    if ti.fn:
        return True
    base = ti.base
    for q in ("const ", "volatile "):
        base = base.replace(q, "")
    return base not in BUILTIN


def element(ti):
    """(kind, width, sign) of the innermost object an access touches."""
    if ti.fn or ti.ptr:
        return ("ptr", 4, None)
    base = ti.base.replace("const ", "").replace("volatile ", "")
    return BUILTIN.get(base, ("struct", None, None))


CATEGORIES = ("identical", "qualifier", "bound", "repr", "signedness", "shape")
SEVERITY = {c: i for i, c in enumerate(CATEGORIES)}


def compare(a, b):
    """Compatibility class of two declarations of the same object.

    identical  - same C type
    qualifier  - differ only by const/volatile
    bound      - differ only in that one array's first bound is unsized
                 (legal to combine in one translation unit)
    repr       - distinct C types with one representation (char/u8, long/s32)
    signedness - integer types of one width with different signedness
    shape      - anything else: width, float/int, pointer/array/struct shape
    """
    if a == b:
        return "identical"
    if a.fn or b.fn:
        return "shape"
    if bare(a) == bare(b):
        return "qualifier"
    worst = "qualifier" if a.quals != b.quals else "identical"

    def bump(cat):
        return cat if SEVERITY[cat] > SEVERITY[worst] else worst

    if a.ptr != b.ptr or len(a.dims) != len(b.dims):
        return "shape"
    if a.dims != b.dims:
        if a.dims[1:] != b.dims[1:] or not (a.dims[0] is None or b.dims[0] is None):
            return "shape"
        worst = bump("bound")
    if a.base != b.base:
        if a.ptr:
            return "shape"
        ka, kb = element(a._replace(dims=())), element(b._replace(dims=()))
        if ka[0] != "int" or kb[0] != "int" or ka[1] != kb[1]:
            return "shape"
        worst = bump("repr" if ka[2] == kb[2] else "signedness")
    return worst


# --------------------------------------------------------------------------
# Retail access evidence
# --------------------------------------------------------------------------

_WIDTH_CLASS = {"lb": "b", "lbu": "b", "sb": "b", "lh": "h", "lhu": "h", "sh": "h",
                "lw": "w", "sw": "w", "lwc1": "f", "swc1": "f",
                "ldc1": "d", "sdc1": "d", "lwl": "x", "lwr": "x", "swl": "x", "swr": "x"}
_CLASS_WIDTH = {"b": 1, "h": 2, "w": 4, "f": 4, "d": 8}


class Evidence(object):
    """What retail code does at one absolute address."""

    def __init__(self):
        self.mn = collections.Counter()          # every load/store mnemonic
        self.indexed = 0                         # indexed accesses/formations, any scale
        self.strides = collections.Counter()     # recovered index scale (>= 2 bytes)
        self.formed = 0                          # lui+addiu address formations
        self.via_base = collections.Counter()    # reached as base+off, base != addr
        self.by_func = collections.defaultdict(collections.Counter)

    def add(self, ref):
        mn = ref["mn"]
        if ref.get("kind") == "form":
            self.formed += 1
            self.by_func[ref.get("f", "?")]["form"] += 1
            self._index(ref)
            return
        self.mn[mn] += 1
        self.by_func[ref.get("f", "?")][mn] += 1
        self._index(ref)
        base = ref.get("base")
        if base is not None and base != ref["addr"]:
            self.via_base[base] += 1

    def _index(self, ref):
        if ref.get("indexed"):
            self.indexed += 1
            # the scan reports 1 (or 0) when it saw an index but no scaling
            if ref.get("stride", 0) >= 2:
                self.strides[ref["stride"]] += 1

    @property
    def accesses(self):
        return sum(self.mn.values())

    def classes(self):
        return {_WIDTH_CLASS.get(m, "x") for m in self.mn}

    def sign(self, cls):
        """'s', 'u', 'mixed' or None (no loads) for width class b or h."""
        signed, unsigned = {"b": ("lb", "lbu"), "h": ("lh", "lhu")}[cls]
        s, u = self.mn.get(signed, 0), self.mn.get(unsigned, 0)
        if s and u:
            return "mixed"
        return "s" if s else "u" if u else None

    def stride(self):
        """Most common recovered index scale, or None (not indexed, or the
        scale was not recovered)."""
        return self.strides.most_common(1)[0][0] if self.strides else None

    def width_type(self):
        """Width-correct integer type when the signedness is not settled:
        signed unless unsigned loads are the majority (the datasyms rule)."""
        classes = self.classes()
        if len(classes) != 1 or next(iter(classes)) not in ("b", "h"):
            return None
        cls = next(iter(classes))
        signed, unsigned = {"b": ("lb", "lbu"), "h": ("lh", "lhu")}[cls]
        names = {"b": ("s8", "u8"), "h": ("s16", "u16")}[cls]
        return names[1] if self.mn.get(unsigned, 0) > self.mn.get(signed, 0) else names[0]

    def settled_type(self):
        """(base, confidence) when the accesses settle a scalar element type,
        else (None, reason)."""
        classes = self.classes()
        if not classes:
            return None, "no load/store at this address"
        if len(classes) > 1:
            return None, "mixed access widths (%s)" % self.summary()
        cls = next(iter(classes))
        if cls == "f":
            return "f32", "high"
        if cls == "d":
            return "f64", "high"
        if cls == "w":
            return "s32", "width"            # signedness/pointer-ness not observable
        if cls in ("b", "h"):
            sign = self.sign(cls)
            names = {"b": ("s8", "u8"), "h": ("s16", "u16")}[cls]
            loads = sum(n for m, n in self.mn.items() if m.startswith("l"))
            if sign == "s":
                return names[0], "high" if loads >= 2 else "medium"
            if sign == "u":
                return names[1], "high" if loads >= 2 else "medium"
            if sign == "mixed":
                return None, "both signed and unsigned loads (%s)" % self.summary()
            return None, "stores only, signedness not observable (%s)" % self.summary()
        return None, "unaligned access (%s)" % self.summary()

    def summary(self):
        parts = ["%s:%d" % (m, n) for m, n in sorted(self.mn.items())]
        if self.indexed:
            parts.append("idx[" + (",".join("0x%X" % s for s in sorted(self.strides)) or "?") + "]")
        if self.formed:
            parts.append("form:%d" % self.formed)
        return " ".join(parts) if parts else "none"


def build_evidence(refs):
    table = collections.defaultdict(Evidence)
    for ref in refs:
        table[ref["addr"]].add(ref)
    return table


def decl_vs_evidence(ti, ev):
    """How a declaration sits with the retail accesses at its address.

    ok / no-evidence / signedness / width / shape, plus `n/a` for struct types.
    """
    if is_struct_typed(ti):
        return "n/a"
    if ev is None or not ev.accesses:
        return "no-evidence"
    kind, width, sign = element(ti)
    classes = ev.classes()
    mine = {"int": {1: "b", 2: "h", 4: "w", 8: "d"}, "ptr": {4: "w"},
            "float": {4: "f", 8: "d"}}.get(kind, {}).get(width)
    if mine not in classes:
        return "width"
    if mine in ("b", "h") and sign is not None:
        seen = ev.sign(mine)
        if seen in ("s", "u") and seen != sign:
            return "signedness"
    if ev.indexed and not ti.dims:
        return "shape"                      # retail indexes it; a scalar cannot
    return "ok"


# --------------------------------------------------------------------------
# Choosing one declaration per address
# --------------------------------------------------------------------------

class Cand(object):
    """One distinct declared type (object qualifiers kept) at an address."""

    def __init__(self, ti):
        self.ti = ti
        self.files = []        # every source declaring it
        self.users = []        # ... that also reference the symbol
        self.default = False   # the generated prelude spelling

    @property
    def volatile(self):
        return "volatile" in self.ti.quals

    def rank(self):
        return (len(self.users), 0 if self.default else 1, len(self.files), spell(self.ti))


Choice = collections.namedtuple(
    "Choice", "ti bucket basis status confidence reason dissent")
Choice.__doc__ = """ti: chosen TypeInfo (None in the struct bucket);
bucket: emit | struct; basis: sources | default | evidence;
status: clean | superseded | resolved | open | struct;
dissent: [(Cand, category, verdict)] for declarations that are used or
hand-written and are not interchangeable with the choice."""


def _merge_bound(best, pool):
    """Pick the array bound for `best` from the candidates that only differ
    from it in the first bound."""
    if not best.dims:
        return best, None
    sized = set()
    for cand in pool:
        ti = bare(cand.ti)
        if ti._replace(dims=()) == bare(best)._replace(dims=()) and len(ti.dims) == len(best.dims) \
                and ti.dims[1:] == best.dims[1:] and ti.dims[0] is not None:
            sized.add(ti.dims[0])
    if len(sized) == 1:
        return best._replace(dims=(next(iter(sized)),) + best.dims[1:]), None
    if len(sized) > 1:
        return best._replace(dims=(None,) + best.dims[1:]), \
            "declared bounds disagree (%s); left unsized" % ", ".join(str(s) for s in sorted(sized, key=str))
    return best, None


def choose(cands, ev=None, record_array=None):
    """Apply the precedence rules to the candidate declarations of one address.

    cands: list of Cand; ev: Evidence or None; record_array: a bases.json
    entity when the address lies in element 0 of a record array."""
    live = [c for c in cands if not c.volatile]
    used = [c for c in cands if c.users]
    if record_array is not None or any(is_struct_typed(c.ti) for c in live):
        reason = "struct-typed by at least one source"
        if record_array is not None:
            reason = "record array: stride 0x%X at 0x%08X" % (record_array["stride"], record_array["base"])
        return Choice(None, "struct", "sources", "struct", "open", reason,
                      [(c, "shape", "n/a") for c in used])
    verdict = {id(c): decl_vs_evidence(c.ti, ev) for c in cands}

    def fits(cand):
        return verdict[id(cand)] in ("ok", "no-evidence")

    def same(cand, ti):
        return compare(bare(cand.ti), ti) in ("identical", "qualifier", "bound")

    ok = [c for c in live if fits(c)]
    notes, basis, confidence, bound_note = [], "sources", "high", None
    if ok:
        top = max(ok, key=Cand.rank)
        best = bare(top.ti)
        if "const" in top.ti.quals:
            notes.append("const dropped")
        if not top.users:
            basis = "default" if top.default else "sources"
        best, bound_note = _merge_bound(best, ok)
        if bound_note:
            notes.append(bound_note)
    else:
        donors = [c for c in cands if c.volatile and fits(c)]
        pool = sorted(live or cands, key=Cand.rank, reverse=True)
        scalars = [c for c in pool if verdict[id(c)] == "shape" and not c.ti.dims]
        settled, conf = ev.settled_type() if ev is not None else (None, "no evidence")
        stride = ev.stride() if ev is not None else None
        if donors:
            best = bare(max(donors, key=Cand.rank).ti)
            notes.append("only volatile-declaring sources fit retail; the qualifier is not promoted")
        elif scalars or (settled is not None and ev.indexed):
            elem = bare(scalars[0].ti) if scalars else TypeInfo(settled, frozenset(), 0, (), False)
            width = element(elem)[1]
            if stride and stride > width:
                return Choice(None, "struct", "evidence", "struct", "open",
                              "retail indexes it with stride 0x%X, wider than the %d-byte element: "
                              "record or 2-D array" % (stride, width),
                              [(c, "shape", verdict[id(c)]) for c in used])
            best = elem._replace(dims=(None,))
            basis, confidence = "evidence", "high" if stride == width else "medium"
            notes.append("retail indexes it (%s); no source declares an array"
                         % ("stride 0x%X" % stride if stride else "index scale not recovered"))
        elif settled is not None:
            best = TypeInfo(settled, frozenset(), 0, (), False)
            basis, confidence = "evidence", "medium" if conf == "width" else conf
            notes.append("no declaration fits the retail accesses")
        else:
            width_only = ev.width_type() if ev is not None else None
            if width_only is not None:
                best = TypeInfo(width_only, frozenset(), 0, (None,) if ev.indexed else (), False)
                return Choice(best, "emit", "evidence", "open", "low",
                              "retail settles the width but not the signedness: %s" % conf,
                              [(c, compare(bare(c.ti), best), verdict[id(c)]) for c in used])
            best = bare(pool[0].ti)
            return Choice(best, "emit", "sources", "open", "low",
                          "no declaration fits the retail accesses and they do not settle a type: %s" % conf,
                          [(c, compare(bare(c.ti), best), verdict[id(c)]) for c in used if not same(c, best)])
    dissent, differing = [], False
    for cand in cands:
        if same(cand, best):
            continue
        differing = differing or not cand.volatile
        if cand.users:
            dissent.append((cand, compare(bare(cand.ti), best), verdict[id(cand)]))
    real = [d for d in dissent if not d[0].volatile]
    if bound_note and not real:
        rivals = [(c, "shape", verdict[id(c)]) for c in ok
                  if c.users and c.ti.dims and c.ti.dims[0] is not None and same(c, best)]
        if len({c.ti.dims for c, _cat, _v in rivals}) > 1:
            return Choice(best, "emit", "sources", "open", "low", "; ".join(notes), rivals)
    if not real:
        status = "resolved" if basis == "evidence" else ("superseded" if differing else "clean")
        return Choice(best, "emit", basis, status, confidence, "; ".join(notes), dissent)
    if all(v in ("signedness", "width", "shape") for _c, _cat, v in real):
        # every used declaration that differs from the proposal is contradicted by retail
        loads = sum(n for m, n in ev.mn.items() if m.startswith("l"))
        conf = "high" if (loads >= 2 or ev.indexed) and confidence == "high" else "medium"
        return Choice(best, "emit", "evidence", "resolved", conf, "; ".join(notes), dissent)
    # retail cannot tell the rivals apart
    total = sum(len(c.users) for c in live)
    agree = sum(len(c.users) for c in live if same(c, best))
    conf = "medium" if total and agree * 3 >= total * 2 else "low"
    notes.append("retail fits more than one declared type; the proposal is the type most using sources "
                 "declare (%d of %d)" % (agree, total))
    return Choice(best, "emit", "sources", "open", conf, "; ".join(notes), dissent)


# --------------------------------------------------------------------------
# Reading the source tree
# --------------------------------------------------------------------------

_DNAME = re.compile(r"\bD_([0-9A-Fa-f]{8})\b")
_EXTERN = re.compile(r"\bextern\b([^;{}]*);")
_EXTERN_AGG = re.compile(r"\bextern\s+(?:const\s+|volatile\s+)*(?:struct|union)\b[^;{}]*\{")
_DEFINITION = re.compile(
    r"^(?:static[ \t]+)?(?:(?:const|volatile|unsigned|signed|struct|union)[ \t]+)*[A-Za-z_]\w*[ \t*]+"
    r"[^;(){}=\n]*\bD_[0-9A-Fa-f]{8}\b[^;(){}\n]*;", re.M)
_ALIAS = re.compile(r"\btypedef\s+((?:(?:const|volatile|unsigned|signed|char|short|int|long|float|double)\s+)+|"
                    r"(?:s8|u8|s16|u16|s32|u32|s64|u64|f32|f64)\s+)([A-Za-z_]\w*)\s*;")


def scalar_aliases(text):
    """`typedef s32 M2C_UNK;` -> {"M2C_UNK": "s32"} (builtin targets only)."""
    out = {}
    for m in _ALIAS.finditer(text):
        words = m.group(1).split()
        quals = [w for w in words if w in _QUALIFIERS]
        rest = [w for w in words if w not in _QUALIFIERS]
        if quals:
            continue
        base = rest[0] if len(rest) == 1 and rest[0] in BUILTIN else _PRIMITIVE_CANON.get(tuple(sorted(rest)))
        name = m.group(2)
        if base and name not in BUILTIN and name != base:
            out[name] = base
    return out


def declaration_statements(text):
    """Yield the text of every statement that can declare a global."""
    for m in _EXTERN.finditer(text):
        yield "extern" + m.group(1)
    for m in _EXTERN_AGG.finditer(text):
        i, depth = m.end(), 1
        while i < len(text) and depth:
            depth += (text[i] == "{") - (text[i] == "}")
            i += 1
        j = text.find(";", i)
        if j > 0:
            yield text[m.start():j]
    for m in _DEFINITION.finditer(text):
        stmt = m.group(0)[:-1]
        if not stmt.lstrip().startswith(("typedef", "return", "extern")):
            yield stmt


def resolve_name(name, symbols):
    m = _DNAME.fullmatch(name)
    if m:
        return int(m.group(1), 16)
    return symbols.get(name)


def strip_disabled(text):
    """Drop `#if 0 ... #endif` blocks (not nested)."""
    return re.sub(r"^[ \t]*#[ \t]*if[ \t]+0\b.*?^[ \t]*#[ \t]*endif[^\n]*", " ", text, flags=re.S | re.M)


def function_spans(text):
    """[(name, start, end)] for every top-level function definition body."""
    spans, depth, start, name = [], 0, 0, None
    last_top = 0
    for m in re.finditer(r"[{};]", text):
        ch = m.group(0)
        if ch == ";":
            if depth == 0:
                last_top = m.end()
        elif ch == "{":
            if depth == 0:
                head = text[last_top:m.start()]
                hm = re.search(r"([A-Za-z_]\w*)\s*\((?:[^()]|\([^()]*\))*\)\s*$", head)
                name = hm.group(1) if hm and "=" not in head else None
                start = m.start()
            depth += 1
        else:
            depth = max(0, depth - 1)
            if depth == 0:
                if name:
                    spans.append((name, start, m.end()))
                last_top = m.end()
                name = None
    return spans


def blank_functions(text, names):
    """Replace the bodies of the named functions with spaces."""
    if not names:
        return text
    out, pos = [], 0
    for name, start, end in function_spans(text):
        if name in names:
            out.append(text[pos:start])
            out.append("{}")
            pos = end
    out.append(text[pos:])
    return "".join(out)


def parse_source(text, symbols=None, extra_aliases=None, standins=()):
    """Declarations of globals in one source.

    Returns (decls, used) where decls is a list of (addr, name, TypeInfo) and
    used is the set of addresses the source references outside declarations.
    References inside `standins` (group context functions: compiled as callers
    but not themselves proven) do not count as use."""
    symbols = symbols or {}
    text = strip_disabled(strip_comments(text))
    aliases = dict(extra_aliases or {})
    aliases.update(scalar_aliases(text))
    decls, seen = [], set()
    decl_tokens = collections.Counter()
    for stmt in declaration_statements(text):
        found = False
        for name, ti in parse_declaration(stmt, aliases):
            addr = resolve_name(name, symbols)
            if addr is None:
                continue
            found = True
            if (addr, name, ti) not in seen:
                seen.add((addr, name, ti))
                decls.append((addr, name, ti))
        if found:
            decl_tokens.update(re.findall(r"[A-Za-z_]\w*", stmt))
    body = blank_functions(text, set(standins))
    removed = collections.Counter()
    if standins:
        removed = collections.Counter(re.findall(r"[A-Za-z_]\w*", text))
        removed.subtract(collections.Counter(re.findall(r"[A-Za-z_]\w*", body)))
    all_tokens = collections.Counter(re.findall(r"[A-Za-z_]\w*", text))
    used = set()
    for _addr, name, _ti in decls:
        if all_tokens[name] - removed[name] > decl_tokens[name]:
            used.add(resolve_name(name, symbols))
    return decls, used


def standin_functions(path):
    """Functions in a group source that are context only (stand-in callers)."""
    if path.parent.name == "blob":
        return set()
    try:
        doc = json.loads((path.parent / "group.json").read_text())
    except (OSError, ValueError):
        return set()
    members = set(doc.get("members", []))
    return {n for n in doc.get("context", []) if n not in members}


def source_files(repo=REPO):
    blob = repo / "src/blob"
    return sorted(blob.glob("*.c")) + sorted(blob.glob("groups/*/*.c"))


def source_functions(path):
    """Layout target ids whose retail code a source file accounts for."""
    if path.parent.name == "blob":
        return [path.stem]
    meta = path.parent / "group.json"
    try:
        doc = json.loads(meta.read_text())
    except (OSError, ValueError):
        return []
    return list(doc.get("members", []))


def linker_symbols(repo=REPO):
    """name -> address from the generated blob linker script (read-only)."""
    out = {}
    try:
        text = (repo / "src/blob/blob.ld").read_text()
    except OSError:
        return out
    for m in re.finditer(r"PROVIDE\(\s*(\w+)\s*=\s*0x([0-9A-Fa-f]+)\s*\)", text):
        out[m.group(1)] = int(m.group(2), 16)
    return out


def region(addr, image_end=IMAGE_END):
    if addr < IMAGE_BASE:
        return "static"
    if addr < DATA_START:
        return "code"
    if addr < RODATA_START:
        return "data"
    if addr < image_end:
        return "rodata"
    return "bss"


class Model(object):
    """Everything `generate`, `check` and `names` share."""

    def __init__(self, repo=REPO, type_model=None):
        self.repo = repo
        self.type_model = type_model or (repo / "cloud/work/frontier/type_model")
        self.symbols = linker_symbols(repo)
        self.files = source_files(repo)
        self.image_end = IMAGE_END
        image = repo / "build/game_code.bin"
        if image.exists():
            self.image_end = IMAGE_BASE + image.stat().st_size
        self.per_file = {}          # rel -> (decls, used)
        self.cands = collections.defaultdict(dict)       # addr -> {TypeInfo: Cand}
        self.names = collections.defaultdict(collections.Counter)   # addr -> name -> users
        self.evidence = {}
        self.entities = []
        self.calls = None

    def rel(self, path):
        return str(path.relative_to(self.repo))

    def load_sources(self):
        data_symbols = {n: a for n, a in self.symbols.items()}
        present = []
        for path in self.files:
            try:
                text = path.read_text(errors="replace")
            except OSError:
                continue          # the tree is live: a source can vanish between glob and read
            present.append(path)
            decls, used = parse_source(text, data_symbols, standins=standin_functions(path))
            rel = self.rel(path)
            self.per_file[rel] = (decls, used)
            for addr, name, ti in decls:
                cand = self.cands[addr].setdefault(ti, Cand(ti))
                if rel not in cand.files:
                    cand.files.append(rel)
                    if addr in used:
                        cand.users.append(rel)
                self.names[addr][name] += 1 + (1000 if addr in used else 0)
        self.files = present
        threshold = default_threshold(len(self.files))
        for table in self.cands.values():
            common = [c for c in table.values() if len(c.files) >= threshold]
            if common:
                max(common, key=lambda c: len(c.files)).default = True
        return self

    def load_evidence(self):
        refs_path = self.type_model / "refs.json"
        if refs_path.exists():
            self.evidence = build_evidence(json.loads(refs_path.read_text()))
        bases_path = self.type_model / "bases.json"
        if bases_path.exists():
            self.entities = sorted(json.loads(bases_path.read_text()), key=lambda e: e["base"])
        return self

    def entity_at(self, addr):
        """(entity, element index, offset) for the tightest bases.json entity
        containing addr, or None."""
        best = None
        for ent in self.entities:
            end = ent.get("end") or ent["base"] + ent.get("maxoff", 0) + 1
            if ent["kind"] == "struct":
                end = max(end, ent["base"] + ent.get("maxoff", 0) + 1)
            elif ent.get("count") in (None, ASSUMED_COUNT) and ent.get("stride"):
                end = ent["base"] + ent["stride"]      # element count is a guess: trust element 0 only
            if ent["base"] <= addr < end:
                if best is None or ent["base"] >= best["base"]:
                    best = ent
        if best is None:
            return None
        off = addr - best["base"]
        stride = best.get("stride") or 0
        if best["kind"] == "array" and stride:
            return best, off // stride, off % stride
        return best, 0, off

    def record_array(self, addr):
        hit = self.entity_at(addr)
        if hit and hit[0]["kind"] == "array" and (hit[0].get("stride") or 0) >= 0xC and hit[1] == 0:
            return hit[0]
        return None

    def decide(self, addr):
        cands = sorted(self.cands[addr].values(), key=Cand.rank, reverse=True)
        return choose(cands, self.evidence.get(addr), self.record_array(addr))

    def primary_name(self, addr):
        names = self.names[addr]
        return sorted(names, key=lambda n: (-names[n], not n.startswith("D_"), n))[0]

    def own_accesses(self, rel, addr):
        ev = self.evidence.get(addr)
        if ev is None:
            return ""
        total = collections.Counter()
        for func in source_functions(self.repo / rel):
            total.update(ev.by_func.get(func, {}))
        return " ".join("%s:%d" % kv for kv in sorted(total.items()))


ASSUMED_COUNT = 64      # cluster_bases.py's placeholder when no end pointer evidences the count


def default_threshold(nfiles):
    """A spelling carried by at least this many sources is the generated
    prelude's (source_dup.py used 300 of 622)."""
    return max(3, int(nfiles * 0.4))


# --------------------------------------------------------------------------
# generate
# --------------------------------------------------------------------------

def _files_text(files, limit=5):
    names = [f.replace("src/blob/groups/", "groups/").replace("src/blob/", "") for f in files]
    if len(names) > limit:
        return ", ".join(names[:limit]) + " +%d more" % (len(names) - limit)
    return ", ".join(names)


def _agree(cands, ti):
    users = total = 0
    for cand in cands:
        total += len(cand.users)
        if compare(bare(cand.ti), ti) in ("identical", "qualifier", "bound"):
            users += len(cand.users)
    return users, total


def render_header(model, decisions):
    by_region = collections.defaultdict(list)
    todo = []
    for addr in sorted(decisions):
        choice = decisions[addr]
        if choice.bucket == "struct":
            todo.append(addr)
        else:
            by_region[region(addr, model.image_end)].append(addr)
    emitted = sum(len(v) for v in by_region.values())
    out = []
    w = out.append
    w("/* GENERATED by tools/conveyor/pipeline/shared_decls.py - do not hand-edit; rerun `generate`.")
    w(" *")
    w(" * PROPOSAL (frontier plan, workstream E step 1). No source includes this file yet.")
    w(" * One declaration per global address, for scalars and simple arrays.")
    w(" * Precedence: retail access evidence > matched sources that use the symbol > generated default.")
    w(" * %d sources read; %d addresses declared here; %d struct-typed addresses left as TODO at the end."
      % (len(model.files), emitted, len(todo)))
    w(" *")
    w(" * Trailing comment: ADDRESS | use A/U decl N | retail accesses | notes")
    w(" *   use A/U  matched sources that reference the symbol: A declare this type, U reference it at all")
    w(" *   decl N   sources that carry any declaration of it (mostly the unused generated prelude)")
    w(" *   retail   load/store mnemonics at this address over the whole image (type_model/refs.json);")
    w(" *            idx[S] = indexed with stride S, form = address formed (lui+addiu), none = never accessed directly")
    w(" *   OPEN / RESOLVED entries are argued in cloud/work/frontier/type_model/decl_conflicts.md")
    w(" */")
    w("#ifndef GAME_GLOBALS_H")
    w("#define GAME_GLOBALS_H")
    w("")
    w('#include "types.h"')
    titles = [("static", "Static segment (boot/library data referenced by game code)"),
              ("code", "Addresses inside game code (suspect: data symbols should not point here)"),
              ("data", "Game image .data (0x%08X..0x%08X) - per translation unit" % (DATA_START, RODATA_START)),
              ("rodata", "Game image .rodata (0x%08X..0x%08X) - function-owned literals and jump tables;\n"
                         " * these externs stand in for literals and disappear once functions own their rodata"
                         % (RODATA_START, model.image_end)),
              ("bss", "Storage after the image (BSS)")]
    for key, title in titles:
        addrs = by_region.get(key)
        if not addrs:
            continue
        w("")
        w("/* ---- %s: %d ---- */" % (title, len(addrs)))
        for addr in addrs:
            choice = decisions[addr]
            cands = list(model.cands[addr].values())
            users, total = _agree(cands, choice.ti)
            ev = model.evidence.get(addr)
            notes = []
            if choice.status == "open":
                notes.append("OPEN")
            elif choice.status == "resolved":
                notes.append("RESOLVED by retail")
            hit = model.entity_at(addr)
            if hit:
                ent, idx, off = hit
                if ent["kind"] == "array":
                    notes.append("in array 0x%08X[%d]+0x%X (stride 0x%X)" % (ent["base"], idx, off, ent["stride"]))
                elif off:
                    notes.append("field of 0x%08X+0x%X" % (ent["base"], off))
                else:
                    notes.append("aggregate base (%d offsets)" % ent.get("noffs", 0))
            name = model.primary_name(addr)
            aliases = sorted(n for n in model.names[addr] if n != name)
            if aliases:
                notes.append("alias " + ",".join(aliases))
            if any(c.volatile for c in cands):
                notes.append("volatile quirk in %d source(s)" % sum(len(c.files) for c in cands if c.volatile))
            decl = "extern %s;" % spell(choice.ti, name)
            comment = "/* %08X | use %d/%d decl %d | %s%s */" % (
                addr, users, total, len({f for c in cands for f in c.files}),
                ev.summary() if ev is not None else "none",
                " | " + "; ".join(notes) if notes else "")
            w("%-34s %s" % (decl, comment))
    w("")
    w("/* ---- TODO(struct): %d addresses that need a shared struct typedef (plan E steps 2-3) ----" % len(todo))
    w(" * Not declared here. Each line: address | candidate declarations (sources using/declaring) | retail.")
    w(" * `sdk:` marks a type implied by the libultra call the address is passed to (see names_audit.md).")
    w(" */")
    w("#if 0")
    for addr in todo:
        cands = sorted(model.cands[addr].values(), key=Cand.rank, reverse=True)
        ev = model.evidence.get(addr)
        parts = []
        for cand in cands:
            tag = " (generated default)" if cand.default else ""
            parts.append("%s x%d/%d%s" % (spell(cand.ti, model.primary_name(addr)),
                                         len(cand.users), len(cand.files), tag))
        sdk = model.sdk_type(addr)
        line = "TODO(struct) %08X | %s | %s" % (addr, " ; ".join(parts), ev.summary() if ev is not None else "none")
        if not decisions[addr].reason.startswith("struct-typed"):
            line += " | " + decisions[addr].reason
        if sdk:
            line += " | sdk: " + sdk
        w(line)
    w("#endif")
    w("")
    w("#endif /* GAME_GLOBALS_H */")
    return "\n".join(out) + "\n"


def _conflict_rows(model, addr, choice):
    """Markdown bullet lines describing every declared type at addr."""
    rows = []
    for cand in sorted(model.cands[addr].values(), key=Cand.rank, reverse=True):
        if choice.ti is not None:
            cat = compare(bare(cand.ti), choice.ti)
        else:
            cat = "struct"
        verdict = decl_vs_evidence(cand.ti, model.evidence.get(addr))
        tag = "generated default, " if cand.default else ""
        files = cand.users or cand.files
        own = ""
        if not cand.default or len(cand.users) <= 6:
            owns = [(f, model.own_accesses(f, addr)) for f in cand.users[:6]]
            owns = ["%s `%s`" % (f.replace("src/blob/groups/", "groups/").replace("src/blob/", ""), o)
                    for f, o in owns if o]
            if owns:
                own = "; own retail accesses: " + ", ".join(owns)
        rows.append("  - `%s` - %s%d using / %d declaring; vs proposal: %s; vs retail: %s; %s%s"
                    % (spell(cand.ti, "D"), tag, len(cand.users), len(cand.files), cat, verdict,
                       _files_text(files), own))
    return rows


def render_conflicts(model, decisions):
    resolved, opened, structs, superseded, volatile, retail_only = [], [], [], [], [], []
    for addr in sorted(decisions):
        choice = decisions[addr]
        cands = list(model.cands[addr].values())
        if any(c.volatile for c in cands):
            volatile.append(addr)
        if choice.bucket == "struct":
            if len({bare(c.ti) for c in cands if c.users and not c.volatile}) >= 2:
                structs.append(addr)
            continue
        if choice.status == "open":
            opened.append(addr)
        elif choice.status == "resolved":
            (resolved if choice.dissent else retail_only).append(addr)
        elif choice.status == "superseded":
            superseded.append(addr)
    out = []
    w = out.append
    w("# Declaration conflicts in matched game sources")
    w("")
    w("GENERATED by `python3 -m tools.conveyor.pipeline.shared_decls generate`; rerun instead of editing.")
    w("Companion of `include/game_globals.h` (proposal; nothing includes it yet).")
    w("")
    w("An address is listed when declarations that are hand-written or actually used disagree with each")
    w("other, or when a declaration disagrees with the load/store widths retail code uses at that address")
    w("(`refs.json`: a linear scan, no control flow, so absence of an access proves nothing).")
    w("\"using\" = the source references the symbol outside its declaration; \"own retail accesses\" = what")
    w("the retail code of that source's own function(s) does at the address.")
    w("")
    w("| class | addresses |")
    w("|---|---:|")
    w("| sources read | %d |" % len(model.files))
    w("| addresses declared by any source | %d |" % len(decisions))
    w("| emitted in the header (scalar or simple array) | %d |" % sum(1 for c in decisions.values() if c.bucket == "emit"))
    w("| left as struct TODO | %d |" % sum(1 for c in decisions.values() if c.bucket == "struct"))
    w("| 1. conflicts resolved by retail evidence | %d |" % len(resolved))
    w("| 2. conflicts left open (scalar/array) | %d |" % len(opened))
    w("| ... of which two or more using sources disagree | %d |" % sum(1 for a in opened if decisions[a].dissent))
    w("| ... of which only the width is settled (stores only, or unaligned), no using source disagrees | %d |"
      % sum(1 for a in opened if not decisions[a].dissent))
    w("| 3. struct-typed addresses with rival declarations (open, plan E steps 2-3) | %d |" % len(structs))
    w("| 4. unused declarations contradicted by retail (header follows retail) | %d |" % len(retail_only))
    w("| 5. `volatile` overrides (never promoted) | %d |" % len(volatile))
    w("| 6. unused generated default superseded by a used declaration | %d |" % len(superseded))
    w("")

    def section(title, intro, addrs):
        w("## %s" % title)
        w("")
        w(intro)
        w("")
        if not addrs:
            w("None.")
            w("")
        for addr in addrs:
            choice = decisions[addr]
            ev = model.evidence.get(addr)
            name = model.primary_name(addr)
            head = "### `%s` (0x%08X, %s)" % (name, addr, region(addr, model.image_end))
            w(head)
            w("")
            if choice.ti is not None:
                w("- proposal: `%s` - basis %s, confidence %s" % (spell(choice.ti, name), choice.basis, choice.confidence))
            else:
                sdk = model.sdk_type(addr)
                w("- proposal: %s" % ("%s - from retail call/idiom evidence, confidence high" % sdk if sdk
                                      else "none (needs a shared struct typedef)"))
            w("- retail: `%s`" % (ev.summary() if ev is not None else "none"))
            if choice.reason:
                w("- reasoning: %s" % choice.reason)
            w("- declarations:")
            for row in _conflict_rows(model, addr, choice):
                w(row)
            w("")

    section("1. Resolved by retail evidence",
            "Every rival declaration below is contradicted by the widths, signedness or indexing retail code uses; "
            "the proposal is the one that is not. Sources on the losing side still match because their own function "
            "only needs the narrower view (see their own accesses); they need a cast or the shared type when they move "
            "into one unit, and must then be re-proved.", resolved)
    section("2. Open: retail cannot tell the rivals apart",
            "Retail accesses fit more than one of the declared types (32-bit signedness, pointer versus integer, "
            "mixed signed/unsigned loads, array bounds). The proposal is the majority among using sources and is "
            "NOT settled; a maintainer decides.", opened)
    section("3. Struct-typed addresses with rival declarations",
            "Deferred to plan E steps 2-3 (one shared typedef per record). Listed so the rivals are visible; "
            "an SDK type is proposed where the address is passed to a libultra call. Only addresses where "
            "two or more USED declarations differ are listed; the header's TODO block has all of them.", structs)
    w("## 4. Unused declarations contradicted by retail")
    w("")
    w("No source that uses these symbols disagrees with the header. The declarations that do disagree are "
      "carried but never referenced (generated prelude text), and they contradict the retail accesses, so the "
      "header takes the type from retail. \"medium\" = a single load, or an index whose scale the scan did not recover.")
    w("")
    w("| address | header | contradicted declaration | retail | confidence |")
    w("|---|---|---|---|---|")
    for addr in retail_only:
        choice = decisions[addr]
        ev = model.evidence.get(addr)
        other = [c for c in sorted(model.cands[addr].values(), key=Cand.rank, reverse=True)
                 if compare(bare(c.ti), choice.ti) not in ("identical", "qualifier", "bound")]
        w("| %08X | `%s` | %s | `%s` | %s |" % (
            addr, spell(choice.ti, model.primary_name(addr)),
            ", ".join("`%s` x%d" % (spell(c.ti, "D"), len(c.files)) for c in other) or "-",
            ev.summary() if ev is not None else "none", choice.confidence))
    w("")
    w("## 5. `volatile` overrides")
    w("")
    w("A `volatile` declaration forces a reload and is a shaping device for one function. It is never "
      "promoted; the header carries the unqualified type and these sources keep a local quirk.")
    w("")
    w("| address | volatile declaration | sources | own retail accesses | header |")
    w("|---|---|---|---|---|")
    for addr in volatile:
        choice = decisions[addr]
        for cand in model.cands[addr].values():
            if not cand.volatile:
                continue
            own = "; ".join(filter(None, (model.own_accesses(f, addr) for f in cand.files[:4])))
            head = "`%s`" % spell(choice.ti, "D") if choice.ti is not None else "struct TODO"
            if choice.ti is not None:
                head += " (%s)" % compare(bare(cand.ti), choice.ti)
            w("| %08X | `%s` | %s | %s | %s |" % (addr, spell(cand.ti, "D"), _files_text(cand.files, 3), own or "-", head))
    w("")
    w("## 6. Unused generated defaults superseded")
    w("")
    w("The generated prelude spelling differs from the chosen type, but no source that uses the symbol "
      "declares the prelude spelling. Not a conflict between matches; listed for completeness.")
    w("")
    w("| address | header | generated default | using sources of the header type |")
    w("|---|---|---|---:|")
    for addr in superseded:
        choice = decisions[addr]
        default = [c for c in model.cands[addr].values() if c.default]
        users, _total = _agree(model.cands[addr].values(), choice.ti)
        w("| %08X | `%s` | %s | %d |" % (addr, spell(choice.ti, "D"),
                                         "`%s`" % spell(default[0].ti, "D") if default else "-", users))
    w("")
    counts = dict(resolved=len(resolved), open=len(opened),
                  open_with_rival_users=sum(1 for a in opened if decisions[a].dissent), struct_rivals=len(structs),
                  retail_only=len(retail_only), volatile=len(volatile), superseded=len(superseded))
    return "\n".join(out) + "\n", counts


def cmd_generate(args):
    model = Model().load_sources().load_evidence()
    model.load_calls()
    decisions = {addr: model.decide(addr) for addr in model.cands}
    header = render_header(model, decisions)
    conflicts, counts = render_conflicts(model, decisions)
    Path(args.header).write_text(header)
    Path(args.conflicts).write_text(conflicts)
    emitted = sum(1 for c in decisions.values() if c.bucket == "emit")
    print("sources read            %d" % len(model.files))
    print("addresses declared      %d" % len(decisions))
    print("  emitted in header     %d" % emitted)
    print("  struct TODO           %d" % (len(decisions) - emitted))
    for key in ("resolved", "open", "open_with_rival_users", "struct_rivals", "retail_only", "volatile", "superseded"):
        print("  %-21s %d" % (key, counts[key]))
    print("wrote %s" % args.header)
    print("wrote %s" % args.conflicts)
    return 0


# --------------------------------------------------------------------------
# check
# --------------------------------------------------------------------------

def parse_header(text):
    """(declared, todo): addr -> (name, TypeInfo) from the extern lines, and
    the set of addresses in the TODO(struct) block."""
    todo = {int(m.group(1), 16) for m in re.finditer(r"^TODO\(struct\) ([0-9A-Fa-f]{8})", text, re.M)}
    declared = {}
    for line in text.splitlines():
        m = re.match(r"\s*(extern\b[^;]*);\s*/\*\s*([0-9A-Fa-f]{8})\b", line)
        if not m:
            continue
        for name, ti in parse_declaration(m.group(1)):
            declared[int(m.group(2), 16)] = (name, ti)
    return declared, todo


CHECK_CLASSES = CATEGORIES + ("struct_todo", "absent")


def check_source(decls, used, declared, todo):
    """Classify each declaration of one source against the header.

    Returns [(addr, name, spelling, class, used?)]."""
    rows = []
    for addr, name, ti in decls:
        if addr in declared:
            cls = compare(ti, declared[addr][1])
        elif addr in todo:
            cls = "struct_todo"
        else:
            cls = "absent"
        rows.append((addr, name, spell(ti, name), cls, addr in used))
    return rows


def worst_class(rows, used_only=False):
    order = {c: i for i, c in enumerate(CHECK_CLASSES)}
    classes = [r[3] for r in rows if r[4] or not used_only]
    return max(classes, key=order.__getitem__) if classes else "identical"


def cmd_check(args):
    header_path = Path(args.header)
    if not header_path.exists():
        print("missing %s: run `generate` first" % header_path, file=sys.stderr)
        return 2
    declared, todo = parse_header(header_path.read_text())
    model = Model().load_sources()
    all_decl = collections.Counter()
    used_decl = collections.Counter()
    file_all = collections.Counter()
    file_used = collections.Counter()
    detail = {}
    for rel in sorted(model.per_file):
        decls, used = model.per_file[rel]
        rows = check_source(decls, used, declared, todo)
        for row in rows:
            all_decl[row[3]] += 1
            if row[4]:
                used_decl[row[3]] += 1
        file_all[worst_class(rows)] += 1
        file_used[worst_class(rows, used_only=True)] += 1
        detail[rel] = {
            "worst_all": worst_class(rows), "worst_used": worst_class(rows, used_only=True),
            "declarations": len(rows), "used": sum(1 for r in rows if r[4]),
            "not_identical_used": [{"addr": "%08X" % r[0], "decl": r[2], "class": r[3],
                                    "header": spell(declared[r[0]][1], declared[r[0]][0]) if r[0] in declared else None}
                                   for r in rows if r[4] and r[3] != "identical"],
        }
    legend = {
        "identical": "same C type as the header",
        "qualifier": "differs only by const/volatile",
        "bound": "differs only by an unsized array bound (legal together)",
        "repr": "distinct C type, same representation (char/u8, long/s32)",
        "signedness": "same width, different signedness",
        "shape": "different width or shape (scalar/array/pointer/struct)",
        "struct_todo": "address is in the header's struct TODO block",
        "absent": "address not in the header",
    }
    print("header: %s (%d declarations, %d struct TODO)" % (header_path, len(declared), len(todo)))
    print("sources: %d" % len(model.per_file))
    print("")
    print("%-12s %12s %12s %12s %12s   %s" % ("class", "decls(all)", "decls(used)", "files(all)", "files(used)", ""))
    for cls in CHECK_CLASSES:
        print("%-12s %12d %12d %12d %12d   %s" % (cls, all_decl[cls], used_decl[cls], file_all[cls], file_used[cls], legend[cls]))
    print("%-12s %12d %12d %12d %12d" % ("total", sum(all_decl.values()), sum(used_decl.values()),
                                        sum(file_all.values()), sum(file_used.values())))
    print("")
    print("files(...) counts each source once, by its worst declaration; `used` restricts to declarations the")
    print("source references. A source could `#include` the header today (dropping its own prelude) iff its")
    print("files(used) class is identical, qualifier or bound.")
    ready = sum(file_used[c] for c in ("identical", "qualifier", "bound"))
    print("sources ready on used declarations: %d of %d" % (ready, len(model.per_file)))
    if args.json:
        Path(args.json).write_text(json.dumps({
            "header": str(header_path), "sources": len(model.per_file),
            "totals": {"declarations_all": dict(all_decl), "declarations_used": dict(used_decl),
                       "files_all": dict(file_all), "files_used": dict(file_used)},
            "files": detail}, indent=1, sort_keys=True) + "\n")
        print("wrote %s" % args.json)
    return 0


# --------------------------------------------------------------------------
# Call-argument scan (types implied by libultra calls) and names audit
# --------------------------------------------------------------------------

# callee -> {argument index: type the argument points to}
SDK_ARGS = {
    "osCreateMesgQueue": {0: "OSMesgQueue", 1: "OSMesg[]"},
    "osRecvMesg": {0: "OSMesgQueue"}, "osSendMesg": {0: "OSMesgQueue"}, "osJamMesg": {0: "OSMesgQueue"},
    "osSetEventMesg": {1: "OSMesgQueue"},
    "osCreateThread": {0: "OSThread"}, "osStartThread": {0: "OSThread"},
    "osDestroyThread": {0: "OSThread"}, "osSetThreadPri": {0: "OSThread"},
    "osSetTimer": {0: "OSTimer"},
    "osContStartReadData": {0: "OSMesgQueue"}, "osContStartQuery": {0: "OSMesgQueue"},
    "osContGetReadData": {0: "OSContPad[]"}, "osContGetQuery": {0: "OSContStatus[]"},
    "osPfsInitPak": {0: "OSMesgQueue", 1: "OSPfs"},
    "osPfsAllocate": {0: "OSPfs"}, "osPfsDeleteFile": {0: "OSPfs"}, "osPfsFindFile": {0: "OSPfs"},
    "osPfsReadWriteFile": {0: "OSPfs"}, "osPfsGetFileStat": {0: "OSPfs"}, "osPfsFreeBlocks": {0: "OSPfs"},
    "osPfsChecker": {0: "OSPfs"}, "osMotorInit": {0: "OSMesgQueue", 1: "OSPfs"},
    "osPiStartDma": {0: "OSIoMesg"},
    "guMtxIdent": {0: "Mtx"}, "guOrtho": {0: "Mtx"}, "guPerspective": {0: "Mtx"}, "guLookAt": {0: "Mtx"},
    "guMtxF2L": {0: "f32[4][4]", 1: "Mtx"}, "guMtxL2F": {0: "f32[4][4]", 1: "Mtx"},
    "guMtxIdentF": {0: "f32[4][4]"}, "guOrthoF": {0: "f32[4][4]"},
    "guPerspectiveF": {0: "f32[4][4]"}, "guLookAtF": {0: "f32[4][4]"},
}
_CALLER_SAVED = (1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 24, 25, 31)


def _sx(value):
    return value - 0x10000 if value & 0x8000 else value


def scan_calls(words, vaddr, callee_name):
    """Linear scan of one function.

    words: list of big-endian instruction words; callee_name: addr -> name.
    Returns (calls, bumps): calls is a list of (pc, callee, {arg: value}) with
    value ("abs", addr) | ("ptr", addr) | ("arg", n); bumps counts, per
    address, `p = *A; ...; *A = p + 8` sequences (display-list style cursor).
    """
    state = {4 + i: ("arg", i) for i in range(4)}
    calls, bumps = [], collections.Counter()

    def step(word):
        op, rs, rt, rd = word >> 26, (word >> 21) & 31, (word >> 16) & 31, (word >> 11) & 31
        imm, fn = word & 0xFFFF, word & 63

        def put(reg, value=None):
            if reg == 0:
                return
            if value is None:
                state.pop(reg, None)
            else:
                state[reg] = value

        src = state.get(rs)
        if op == 0x0F:
            put(rt, ("hi", imm << 16))
        elif op in (9, 0xD):
            if src and src[0] == "hi":
                put(rt, ("abs", (src[1] + (_sx(imm) if op == 9 else imm)) & 0xFFFFFFFF))
            elif src and src[0] == "abs" and op == 9:
                put(rt, ("abs", (src[1] + _sx(imm)) & 0xFFFFFFFF))
            elif src and src[0] == "ptr" and op == 9 and imm == 8:
                put(rt, ("bump", src[1]))
            elif op == 9 and imm == 0 and src:
                put(rt, src)
            else:
                put(rt)
        elif op == 0x23:                                   # lw
            if src and src[0] in ("hi", "abs"):
                put(rt, ("ptr", (src[1] + _sx(imm)) & 0xFFFFFFFF))
            else:
                put(rt)
        elif op == 0x2B:                                   # sw
            val = state.get(rt)
            if val and val[0] == "bump" and src and src[0] in ("hi", "abs") \
                    and ((src[1] + _sx(imm)) & 0xFFFFFFFF) == val[1]:
                bumps[val[1]] += 1
        elif op in (0x20, 0x21, 0x22, 0x24, 0x25, 0x26):
            put(rt)
        elif op in (8, 0xA, 0xB, 0xC, 0xE):
            put(rt)
        elif op == 0:
            if fn in (0x21, 0x25) and rt == 0:             # move rd, rs
                put(rd, src)
            elif fn in (0x21, 0x25) and rs == 0:
                put(rd, state.get(rt))
            elif fn in (8, 9, 0x18, 0x19, 0x1A, 0x1B, 0x11, 0x13, 0x0C, 0x0D):
                pass
            else:
                put(rd)
        elif op == 0x11 and rs in (0, 2):
            put(rt)

    i = 0
    while i < len(words):
        word = words[i]
        op, fn = word >> 26, word & 63
        is_jal = op == 3
        is_jalr = op == 0 and fn == 9
        if is_jal or is_jalr:
            pc = vaddr + 4 * i
            if i + 1 < len(words):
                step(words[i + 1])                         # delay slot
            if is_jal:
                target = ((pc + 4) & 0xF0000000) | ((word & 0x03FFFFFF) << 2)
                args = {n: state[4 + n] for n in range(4)
                        if state.get(4 + n) and state[4 + n][0] in ("abs", "ptr", "arg")}
                calls.append((pc, callee_name.get(target, "func_%08X" % target), args))
            for reg in _CALLER_SAVED:
                state.pop(reg, None)
            i += 2
            continue
        step(word)
        i += 1
    return calls, bumps


def infer_sdk_types(function_calls, sdk_args=None, rounds=3):
    """function_calls: {function: [(pc, callee, args)]}.

    Returns (typed, wrappers): typed maps ("abs"|"ptr", addr) to a Counter of
    (type, callee); wrappers maps game functions that forward their own
    argument to a typed parameter: {function: {arg: type}}."""
    signatures = {k: dict(v) for k, v in (sdk_args or SDK_ARGS).items()}
    sdk_names = set(signatures)
    wrappers = {}
    for _ in range(rounds):
        changed = False
        for func, calls in function_calls.items():
            for _pc, callee, args in calls:
                sig = signatures.get(callee)
                if not sig:
                    continue
                for n, value in args.items():
                    if value[0] == "arg" and n in sig:
                        mine = wrappers.setdefault(func, {})
                        if value[1] not in mine:
                            mine[value[1]] = sig[n]
                            changed = True
        for func, sig in wrappers.items():
            if func not in sdk_names and signatures.get(func) != sig:
                signatures[func] = dict(sig)
                changed = True
        if not changed:
            break
    typed = collections.defaultdict(collections.Counter)
    for func, calls in function_calls.items():
        for _pc, callee, args in calls:
            sig = signatures.get(callee)
            if not sig:
                continue
            for n, value in args.items():
                if n in sig and value[0] in ("abs", "ptr"):
                    typed[value][(sig[n], callee)] += 1
    return typed, wrappers


def critical_sections(calls):
    """Queues a function takes with osRecvMesg and gives back with
    osJamMesg/osSendMesg (the mutex idiom).  Returns the set of queue values."""
    held, out = set(), set()
    for _pc, callee, args in calls:
        queue = args.get(0)
        if queue is None:
            continue
        if callee == "osRecvMesg":
            held.add(queue)
        elif callee in ("osJamMesg", "osSendMesg") and queue in held:
            out.add(queue)
    return out


def _load_calls(model):
    image_path = model.repo / "build/game_code.bin"
    layout_path = model.repo / "build/blob_layout.json"
    model.calls, model.bumps, model.funcs = {}, collections.Counter(), {}
    model.bump_funcs = collections.defaultdict(set)
    model.typed, model.wrappers = {}, {}
    if not (image_path.exists() and layout_path.exists()):
        return model
    image = image_path.read_bytes()
    layout = json.loads(layout_path.read_text())
    callee_name = {addr: name for name, addr in model.symbols.items()
                   if not name.startswith("D_")}
    funcs = []
    for reg in layout["regions"]:
        for entry in reg["entries"]:
            if entry["kind"] == "function":
                funcs.append((entry["vaddr"], entry["size"], entry["target_id"]))
                callee_name[entry["vaddr"]] = entry["target_id"]
    for vaddr, size, name in sorted(funcs):
        off = vaddr - IMAGE_BASE
        words = list(struct.unpack(">%dI" % (size // 4), image[off:off + size - size % 4]))
        calls, bumps = scan_calls(words, vaddr, callee_name)
        model.calls[name] = calls
        model.funcs[name] = (vaddr, size, words)
        for addr, n in bumps.items():
            model.bumps[addr] += n
            model.bump_funcs[addr].add(name)
    model.typed, model.wrappers = infer_sdk_types(model.calls)
    return model


def _sdk_type(model, addr):
    """Type implied for the object AT addr by libultra call arguments."""
    if not getattr(model, "typed", None):
        return ""
    direct = model.typed.get(("abs", addr))
    via = model.typed.get(("ptr", addr))
    parts = []
    if direct:
        types = collections.Counter()
        for (typ, _callee), n in direct.items():
            types[typ] += n
        parts.append(" / ".join("%s (%d call sites)" % kv for kv in types.most_common()))
    if via:
        types = collections.Counter()
        for (typ, _callee), n in via.items():
            types[typ] += n
        parts.append(" / ".join("%s * (%d call sites)" % kv for kv in types.most_common()))
    bumps = model.bumps.get(addr, 0)
    if bumps >= 5 and len(model.bump_funcs[addr]) >= 3:
        parts.append("Gfx * (display-list cursor idiom, %d sites in %d functions)"
                     % (bumps, len(model.bump_funcs[addr])))
    return "; ".join(parts)


Model.load_calls = _load_calls
Model.sdk_type = _sdk_type


def hand_names(repo=REPO):
    """Hand names for addresses: [(addr, name, source, comment, is_func)]."""
    rows = []
    path = repo / "symbol_addrs.us.txt"
    if path.exists():
        for line in path.read_text(errors="replace").splitlines():
            m = re.match(r"\s*(//\s*dup-addr\([^)]*\):\s*)?(\w+)\s*=\s*0x([0-9A-Fa-f]{8})\s*;\s*(?://\s*(.*))?$", line)
            if not m or (line.lstrip().startswith("//") and not m.group(1)):
                continue
            comment = m.group(4) or ""
            rows.append((int(m.group(3), 16), m.group(2),
                         "symbol_addrs.us.txt" + (" (dup-addr)" if m.group(1) else ""),
                         comment, "type:func" in comment))
    try:
        from . import disasm
        for addr, name in sorted(disasm.GAME_SYMBOLS.items()):
            if not name.startswith("D_"):
                rows.append((addr, name, "GAME_SYMBOLS", "", False))
    except Exception:                                      # pragma: no cover - optional layer
        pass
    return rows


_QUEUE_WORDS = re.compile(r"mesg|msg|queue|_mq|mq_|lock|mutex|sema", re.I)
_TYPE_CLAIM = re.compile(r"\((s8|u8|s16|u16|s32|u32|f32|f64)(\[\d*\])?\)")
_ARRAY_WORDS = re.compile(r"\barray\b|\btable\b|\bbuffer\b|\bstruct(ure)?\b", re.I)
_NAME_ARRAY_WORDS = re.compile(r"Array|Table|Buffer|Struct|List$|Data[AB]?$")
_LOCK_WORDS = re.compile(r"sync|lock|guard|critical|mutex", re.I)
# libultra calls no gameplay routine ported from the arcade has a reason to make
_SETUP_CALLS = re.compile(r"^(osCreate|osPfs|osMotor|osCont|osSetEventMesg|osSetTimer)")


def audit_globals(names, evidence, typed, bumps, bump_funcs, entity_at, holders):
    """Rule-based contradictions between a global's hand name/comment and code.

    Returns a list of dicts: addr, names, rule, evidence, proposal."""
    by_addr = collections.defaultdict(list)
    for addr, name, source, comment, is_func in names:
        if not is_func and not name.startswith(("D_", "func_")):
            by_addr[addr].append((name, source, comment))
    findings = []

    def add(addr, rule, ev_text, proposal, contradicted=True):
        findings.append(dict(addr=addr, names=by_addr.get(addr, []), rule=rule, evidence=ev_text,
                             proposal=proposal, contradicted=contradicted))

    for key in sorted(typed, key=lambda k: (k[1], k[0])):
        how, addr = key
        types = collections.Counter()
        callees = collections.Counter()
        for (typ, callee), n in typed[key].items():
            types[typ] += n
            callees[callee] += n
        typ = types.most_common(1)[0][0]
        if how == "ptr":
            typ += " *"
        ev_text = "passed %s to %s" % ("by value (loaded pointer)" if how == "ptr" else "by address",
                                       ", ".join("%s x%d" % kv for kv in callees.most_common()))
        labels = by_addr.get(addr, [])
        is_queue = typ.startswith("OSMesgQueue")
        mutex = holders.get(key)
        if is_queue:
            stem = "gLockQueue" if mutex else "gMesgQueue"
            proposal = "%s%s_%08X : %s" % (stem, "Ptr" if how == "ptr" else "", addr, typ)
            if mutex:
                ev_text += "; taken with osRecvMesg and released with osJamMesg/osSendMesg in %d function(s)" % len(mutex)
        else:
            proposal = "D_%08X : %s" % (addr, typ)
        bad = [n for n, _s, c in labels if is_queue and not _QUEUE_WORDS.search(n)]
        add(addr, "sdk-arg", ev_text, proposal, contradicted=bool(bad) if labels else False)
    for addr in sorted(by_addr):
        ev = evidence.get(addr)
        for name, source, comment in by_addr[addr]:
            claim = _TYPE_CLAIM.search(comment)
            if claim and ev is not None and ev.accesses:
                ti = TypeInfo(claim.group(1), frozenset(), 0, (None,) if claim.group(2) else (), False)
                verdict = decl_vs_evidence(ti._replace(dims=(None,)) if ev.stride() else ti, ev)
                if verdict in ("width", "signedness"):
                    settled, _conf = ev.settled_type()
                    add(addr, "type-claim", "comment claims `%s%s`; retail accesses `%s` (%s)"
                        % (claim.group(1), claim.group(2) or "", ev.summary(), verdict),
                        "retype to %s" % (settled or "the observed widths"))
            hit = entity_at(addr)
            if hit and hit[0]["kind"] == "array" and (hit[0].get("stride") or 0) >= 0xC and (hit[1] or hit[2]):
                ent, idx, off = hit
                findings.append(dict(addr=addr, names=[(name, source, comment)], rule="inside-record",
                                     evidence="element %d, +0x%X" % (idx, off), proposal="field at +0x%X" % off,
                                     contradicted=True, base=ent["base"], stride=ent["stride"],
                                     count=ent.get("count"), index=idx, offset=off, name=name))
            if ev is not None and ev.accesses >= 3 and not ev.indexed and ev.formed * 5 <= ev.accesses \
                    and len(ev.classes()) == 1 and not (hit and hit[0]["kind"] == "array") \
                    and ("abs", addr) not in typed:
                if _NAME_ARRAY_WORDS.search(name) or (_ARRAY_WORDS.search(comment) and not claim):
                    settled, _conf = ev.settled_type()
                    add(addr, "scalar-not-aggregate", "`%s` (\"%s\") implies an aggregate; retail does `%s` "
                        "at this address and never indexes it" % (name, comment[:60], ev.summary()),
                        "a scalar name; type %s" % (settled or "by width"))
    for addr in sorted(bumps):
        if bumps[addr] >= 5 and len(bump_funcs[addr]) >= 3:
            labels = by_addr.get(addr, [])
            add(addr, "cursor", "`p = *A; *A = p + 8` in %d functions (%d sites): an 8-byte-record write cursor "
                "(the Gfx display-list idiom)" % (len(bump_funcs[addr]), bumps[addr]),
                "gGfxHead_%08X : Gfx *" % addr, contradicted=bool(labels) and not any(
                    re.search(r"gfx|dl|display|glist", n, re.I) for n, _s, _c in labels))
    return findings


def audit_name_collisions(names, symbols):
    """Names bound to more than one address, and addresses with several names."""
    by_name = collections.defaultdict(set)
    by_addr = collections.defaultdict(set)
    for addr, name, source, _comment, _is_func in names:
        by_name[name].add((addr, source.split(" ")[0]))
        by_addr[addr].add(name)
    for name, addr in symbols.items():
        if name in by_name:
            by_name[name].add((addr, "blob.ld"))
    multi_addr = {n: sorted(v) for n, v in by_name.items() if len({a for a, _s in v}) > 1}
    multi_name = {a: sorted(v) for a, v in by_addr.items() if len(v) > 1}
    return multi_addr, multi_name


def audit_functions(names, calls, funcs, wrappers):
    """Rule-based contradictions for named game functions."""
    label = collections.defaultdict(list)
    for addr, name, source, comment, is_func in names:
        if is_func:
            label[addr].append((name, source, comment))
    by_addr = {v[0]: (name, v[1], v[2]) for name, v in funcs.items()}
    findings = []
    for addr in sorted(by_addr):
        target, size, words = by_addr[addr]
        labels = label.get(addr, [])
        named = [(n, s, c) for n, s, c in labels if not n.startswith("func_")]
        if not target.startswith("func_") and not any(n == target for n, _s, _c in named):
            named.append((target, "blob_layout", ""))
        if not named:
            continue
        fcalls = calls.get(target, [])
        sections = critical_sections(fcalls)
        callees = [c for _pc, c, _a in fcalls]
        os_calls = sorted({c for c in callees if c in SDK_ARGS})
        others = [c for c in callees if c not in ("osRecvMesg", "osJamMesg", "osSendMesg")]
        seen_names = set()
        for name, source, comment in named:
            if name in seen_names:
                continue
            seen_names.add(name)
            arcade = "arcade" in comment.lower()
            setup = [c for c in os_calls if _SETUP_CALLS.match(c)]
            if words[:2] == [0x03E00008, 0x00000000] and size <= 8:
                findings.append(dict(addr=addr, name=name, source=source, rule="empty-stub",
                                     evidence="body is `jr ra; nop` (%d bytes); comment: %s" % (size, comment[:70] or "-"),
                                     proposal="stub_%08X (or delete: likely the remains of an inlined static)" % addr))
            elif sections and len(callees) <= 4 and size <= 0xA0:
                queue = sorted(sections)[0]
                findings.append(dict(addr=addr, name=name, source=source, rule="lock-wrapper",
                                     queue=queue, wrapped=tuple(others), size=size, comment=comment,
                                     evidence="", proposal=""))
            elif arcade and setup:
                findings.append(dict(addr=addr, name=name, source=source, rule="arcade-name-with-setup-calls",
                                     evidence="%d bytes; claims an arcade origin (\"%s\") but calls %s"
                                     % (size, comment[:70], ", ".join(setup)),
                                     proposal="func_%08X until re-anchored from code" % addr))
            elif arcade and sections:
                findings.append(dict(addr=addr, name=name, source=source, rule="arcade-name-with-locks",
                                     evidence="%d bytes; %d critical section(s)" % (size, len(sections)), proposal=""))
    families = collections.defaultdict(set)
    for f in findings:
        if f["rule"] == "lock-wrapper":
            families[(f["queue"], f["wrapped"])].add(f["name"])
    for f in findings:
        if f["rule"] != "lock-wrapper":
            continue
        queue, wrapped = f["queue"], f["wrapped"]
        siblings = sorted(families[(queue, wrapped)] - {f["name"]})
        inner = ", ".join(wrapped) or "inline code only"
        f["contradicted"] = not _LOCK_WORDS.search(f["name"] + " " + f["comment"]) or len(siblings) >= 2
        f["evidence"] = "%d bytes: osRecvMesg(0x%08X); %s; osJamMesg/osSendMesg(0x%08X)" % (
            f["size"], queue[1], inner, queue[1])
        if siblings:
            f["evidence"] += ". Same lock and same callee as: " + ", ".join("`%s`" % n for n in siblings)
        f["proposal"] = "locked_%s_%08X" % (wrapped[0] if wrapped else "inline", f["addr"])
    return findings


def render_names(model, global_findings, function_findings, multi_addr, multi_name):
    out = []
    w = out.append
    records = [f for f in global_findings if f["rule"] == "inside-record"]
    plain = [f for f in global_findings if f["rule"] != "inside-record"]
    contradicted = [f for f in plain if f["contradicted"]]
    wrappers = [f for f in function_findings if f["rule"] == "lock-wrapper"]
    setup = [f for f in function_findings if f["rule"] == "arcade-name-with-setup-calls"]
    stubs = [f for f in function_findings if f["rule"] == "empty-stub"]
    locks = [f for f in function_findings if f["rule"] == "arcade-name-with-locks"]
    w("# Names audit: hand names contradicted by code")
    w("")
    w("GENERATED by `python3 -m tools.conveyor.pipeline.shared_decls names`; rerun instead of editing.")
    w("Nothing was renamed. Names come from `symbol_addrs.us.txt` (and its `dup-addr` lines), `GAME_SYMBOLS`")
    w("and the layout target ids. Evidence is retail code only: load/store widths (`refs.json`), record arrays")
    w("(`bases.json`) and a linear scan of every game function for arguments passed to libultra routines.")
    w("")
    w("Limits. The scan has no control flow and loses register state at calls, so a missing finding proves")
    w("nothing. The libultra routine names in the static segment are themselves labels; the queue findings")
    w("rest on `osCreateMesgQueue`/`osRecvMesg`/`osSendMesg`/`osJamMesg` being what they are called. Proposed")
    w("names are deliberately neutral (role plus address): the code shows what a thing is, not what it is for.")
    w("")
    w("| rule | findings | contradicting a hand name |")
    w("|---|---:|---:|")
    for rule in ("sdk-arg", "cursor", "type-claim", "scalar-not-aggregate"):
        rows = [f for f in plain if f["rule"] == rule]
        w("| globals: %s | %d | %d |" % (rule, len(rows), sum(1 for f in rows if f["contradicted"])))
    w("| globals: inside-record (name for a field of a record array) | %d | %d |" % (len(records), len(records)))
    w("| functions: lock-wrapper | %d | %d |" % (len(wrappers), sum(1 for f in wrappers if f["contradicted"])))
    w("| functions: arcade-name-with-setup-calls | %d | %d |" % (len(setup), len(setup)))
    w("| functions: empty-stub | %d | %d |" % (len(stubs), len(stubs)))
    w("| functions: arcade-named, body takes N64 locks (note only) | %d | 0 |" % len(locks))
    w("| names bound to more than one address | %d | %d |" % (len(multi_addr), len(multi_addr)))
    w("")
    w("## 1. Globals whose name or stated type is contradicted")
    w("")
    w("| address | current name(s) | rule | code evidence | proposed replacement |")
    w("|---|---|---|---|---|")
    for f in sorted(contradicted, key=lambda f: (f["rule"] not in ("sdk-arg", "cursor"), f["rule"], f["addr"])):
        names = ", ".join("`%s`" % n for n in sorted({n for n, _s, _c in f["names"]})) or "-"
        w("| %08X | %s | %s | %s | %s |" % (f["addr"], names, f["rule"], f["evidence"].replace("|", "/"), f["proposal"]))
    w("")
    w("Rules: **sdk-arg** the address (or the pointer stored there) is an argument of a libultra call whose")
    w("parameter type is known; **cursor** `p = *A; *A = p + 8`, the display-list write idiom; **type-claim** the")
    w("comment in `symbol_addrs.us.txt` states a C type whose width retail contradicts (a single access is weak")
    w("on its own, but these names were transcribed from arcade offsets, which REPORT.md section 3 shows do not carry over);")
    w("**scalar-not-aggregate** the name or comment says array/struct/table/buffer but retail loads and stores")
    w("one scalar there and never indexes it.")
    w("")
    w("## 2. Hand names for fields of record arrays")
    w("")
    w("Each name below is bound, as if it were a standalone global, to an address that retail reaches as")
    w("`base + index*stride + offset`. For element 0 the name is at best a field name; for a later element it")
    w("cannot be right as a global (it would name one element's copy of a field). Element counts of 64 in")
    w("`bases.json` are placeholders, so only element 0 of those arrays is considered.")
    w("Proposal for all of them: retire the global name and carry it, if a code anchor supports it, as the")
    w("field name at that offset of the record type (plan E steps 2-3).")
    w("")
    w("| record array | stride | count | names (element, offset) |")
    w("|---|---|---|---|")
    by_base = collections.defaultdict(list)
    for f in records:
        by_base[(f["base"], f["stride"], f["count"])].append(f)
    for (base, stride, count), rows in sorted(by_base.items()):
        cells = ", ".join("`%s` [%d]+0x%X" % (f["name"], f["index"], f["offset"])
                          for f in sorted(rows, key=lambda f: (f["addr"], f["name"])))
        w("| %08X | 0x%X | %s | %s |" % (base, stride, "?" if count in (None, ASSUMED_COUNT) else count, cells))
    w("")
    w("## 3. Functions")
    w("")
    w("### 3.1 Critical-section wrappers")
    w("")
    w("At most four calls and 160 bytes: take a queue with `osRecvMesg`, call something, give it back with")
    w("`osJamMesg`/`osSendMesg`. The honest name is \"locked <callee>\". \"contradicted\" = the name and comment say")
    w("nothing about locking, or three or more functions with unrelated names wrap the same callee under the")
    w("same lock. Callee names in the evidence are current labels and are not themselves verified.")
    w("")
    w("| address | current name | contradicted | code evidence | proposed replacement |")
    w("|---|---|---|---|---|")
    for f in sorted(wrappers, key=lambda f: (f["queue"], f["wrapped"], f["addr"], f["name"])):
        w("| %08X | `%s` | %s | %s | %s |" % (f["addr"], f["name"], "yes" if f["contradicted"] else "no",
                                            f["evidence"].replace("|", "/"), f["proposal"]))
    w("")
    w("### 3.2 Arcade-named functions that create queues or touch controller/Pak services")
    w("")
    w("| address | current name | code evidence | proposed replacement |")
    w("|---|---|---|---|")
    for f in sorted(setup, key=lambda f: (f["addr"], f["name"])):
        w("| %08X | `%s` | %s | %s |" % (f["addr"], f["name"], f["evidence"].replace("|", "/"), f["proposal"]))
    w("")
    if stubs:
        w("### 3.3 Named empty stubs")
        w("")
        w("| address | current name | code evidence | proposed replacement |")
        w("|---|---|---|---|")
        for f in sorted(stubs, key=lambda f: (f["addr"], f["name"])):
            w("| %08X | `%s` | %s | %s |" % (f["addr"], f["name"], f["evidence"].replace("|", "/"), f["proposal"]))
        w("")
    w("### 3.4 Note: arcade-named functions whose body takes N64 locks")
    w("")
    w("Not a contradiction by itself (a port can add locking), but the arcade source cannot be transcribed")
    w("for these and the pairing should be re-anchored from code before the name is relied on: "
      + ", ".join("`%s` (%08X)" % (f["name"], f["addr"]) for f in sorted(locks, key=lambda f: (f["addr"], f["name"])))
      + ".")
    w("")
    w("### 3.5 Recorded elsewhere, not derived by these rules")
    w("")
    w("- `entity_name_copy` is `bsearch` (frontier plan section 2). Recognising library routines by shape is not automated here.")
    w("- `gTrackDataB` / `track_data` (0x8014A250, 0x808 x 6) is the N64 `MODELDAT` array, not track data "
      "(REPORT.md section 3.2, inferred from `func_800E4B58`). Proposed: `model_data_8014A250` until the struct lands.")
    w("- `gPlayerCarState1` / `player_array` / `game_car` (0x80152818, 0x3B8 x 6) is the car array; three names for one base.")
    w("")
    w("## 4. One name, several addresses")
    w("")
    w("| name | bindings |")
    w("|---|---|")
    for name in sorted(multi_addr):
        w("| `%s` | %s |" % (name, "; ".join("0x%08X in %s" % (a, src) for a, src in multi_addr[name])))
    w("")
    w("## 5. Types implied by libultra calls (all, including unnamed addresses)")
    w("")
    w("Input for the struct TODO block of `include/game_globals.h`.")
    w("")
    w("| address | current name(s) | implied type | evidence |")
    w("|---|---|---|---|")
    for f in sorted((f for f in plain if f["rule"] in ("sdk-arg", "cursor")), key=lambda f: (f["addr"], f["rule"])):
        names = ", ".join("`%s`" % n for n in sorted({n for n, _s, _c in f["names"]})) or "-"
        w("| %08X | %s | %s | %s |" % (f["addr"], names, f["proposal"].split(" : ")[-1], f["evidence"].replace("|", "/")))
    w("")
    w("## 6. Addresses carrying several hand names")
    w("")
    game = sorted(a for a in multi_name if a >= IMAGE_BASE)
    w("%d addresses in or after the game image; a rename has to retire all of an address's names together." % len(game))
    w("")
    w("| address | names |")
    w("|---|---|")
    for addr in game:
        w("| %08X | %s |" % (addr, ", ".join("`%s`" % n for n in multi_name[addr])))
    w("")
    return "\n".join(out) + "\n"


def cmd_names(args):
    model = Model().load_evidence()
    model.load_calls()
    names = [row for row in hand_names() if row[0] >= IMAGE_BASE]
    holders = collections.defaultdict(set)
    for func, calls in model.calls.items():
        for queue in critical_sections(calls):
            holders[queue].add(func)
    global_findings = audit_globals(names, model.evidence, model.typed, model.bumps, model.bump_funcs,
                                    model.entity_at, holders)
    function_findings = audit_functions(names, model.calls, model.funcs, model.wrappers)
    multi_addr, multi_name = audit_name_collisions(names, model.symbols)
    Path(args.out).write_text(render_names(model, global_findings, function_findings, multi_addr, multi_name))
    print("global findings   %d (%d contradict a hand name)" % (
        len(global_findings), sum(1 for f in global_findings if f["contradicted"])))
    print("function findings %d" % len(function_findings))
    print("names at >1 addr  %d" % len(multi_addr))
    print("wrote %s" % args.out)
    return 0


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    sub = parser.add_subparsers(dest="command", required=True)
    gen = sub.add_parser("generate", help="write the shared header and the conflict report")
    gen.add_argument("--header", default=str(HEADER))
    gen.add_argument("--conflicts", default=str(CONFLICTS_MD))
    gen.set_defaults(func=cmd_generate)
    chk = sub.add_parser("check", help="compare each matched source with the header on disk")
    chk.add_argument("--header", default=str(HEADER))
    chk.add_argument("--json", default=str(CHECK_JSON))
    chk.set_defaults(func=cmd_check)
    nam = sub.add_parser("names", help="write the names audit")
    nam.add_argument("--out", default=str(NAMES_MD))
    nam.set_defaults(func=cmd_names)
    args = parser.parse_args(argv)
    return args.func(args)


if __name__ == "__main__":
    sys.exit(main())
