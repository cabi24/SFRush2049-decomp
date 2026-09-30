#!/usr/bin/env python3
"""probe.py: controlled-experiment runner for IPA group register allocation (stdlib only).

Given a group directory and a member, generate a matrix of source/group variants along these axes,
compile every variant with builder.compile_group (in parallel, cached), and record per variant the
emitted sizes, strict diff and aligned scores, and the REGISTER USAGE of the member and of the helper
functions around it (decoded from the emitted words with ipakit/mipsdec.py):

    keep    each function in / out of the group.json "keep" list
    sites   0..3 extra stand-in call sites of a focus function (stand-in callers are added to keep)
    params  declared parameter count of a focus function vs how many are actually used
            (extra parameters are declared; "used" adds `__probe_sink += (int) p;` for every parameter;
            extern functions also get the K&R `f()` form)
    ret     return type void / int / short / float
    args    integer parameter width short / unsigned char / int
    inline  the dead `if (0) { switch (x) {...} }` inlining blocker on / off
    flags   -O3 <-> -O2 for the whole group (group.json has one flags string, so not per unit)
    order   function order inside the files (reverse, member first/last, callees first, callers first)
            and file order

Stage 1 varies one thing at a time (OFAT) around the unmodified baseline. Stage 2 (--combine N, default
8) combines the most useful single changes (distinct axis/focus pairs) pairwise and three at a time.
The "focus" functions are the member plus its callees (defined or only declared in the group files),
two call levels deep; --focus overrides.

Register usage per function (signature): entry-read registers (values live at entry: ABI a0-a3 and
non-ABI t*/s*/f16+ IPA parameters; callee-save stores to the stack are not reads), the registers set up
just before each `jal` and not consumed (caller side of an IPA parameter: `li t0,0` before the call),
caller-save registers live across each `jal`, frame size, callee-saved registers written but never
saved ("unsaved" s-registers), and the a/t/s/f registers that appear at all.

Output: markdown table sorted by closeness to the target (member strict diff, aligned score,
register-signature distance to the target), `--json` for machines, and a "rule discovery" summary:
which axis changed which register assignment (series over the levels of an axis).

    python3 cloud/work/tools/amatch/probe.py DIR --member FN [--axes keep,sites,params,ret,args,inline,flags,order]
                                              [--focus F,G] [--jobs J] [--combine N] [--json] [--all]
    python3 cloud/work/tools/amatch/probe.py --validate [--json]     (reproduce this session's findings)
"""
import argparse
import hashlib
import itertools
import json
import os
import random
import re
import shutil
import sys
import tempfile
import time
from concurrent.futures import ProcessPoolExecutor
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[3]
sys.path.insert(0, str(HERE.parent))
from amatch import aligned, builder  # noqa: E402
from ipakit import mipsdec  # noqa: E402

ALL_AXES = ["keep", "sites", "params", "ret", "args", "inline", "flags", "order"]
EXTRA_AXES = ["patch"]      # only with --patches
SINK = "__probe_sink"
INT_WORDS = {"int", "short", "char", "long", "unsigned", "signed", "const", "volatile", "register",
             "s8", "s16", "s32", "s64", "u8", "u16", "u32", "u64", "S08", "S16", "S32", "U08", "U16",
             "U32", "BOOL", "size_t"}
FLOAT_WORDS = {"float", "double", "f32", "f64", "F32"}
STORAGE = {"static", "extern", "inline"}
NOT_DECL = {"return", "goto", "else", "case", "break", "continue", "if", "while", "for", "do", "switch",
            "default", "sizeof"}
DEAD_SWITCH = ("if (0) { switch (%s) { case 1: " + SINK + " = 3; case 2: " + SINK + " = 4; case 3: "
               + SINK + " = 5; } }")


# =====================================================================================
# Lightweight C structure scanning (enough for decomp-style group files)
# =====================================================================================

_MASK_RE = re.compile(r"/\*.*?\*/|//[^\n]*|\"(?:\\.|[^\"\\\n])*\"|'(?:\\.|[^'\\\n])*'", re.S)


def mask_c(text):
    """Same-length copy with comment/string contents blanked (newlines kept)."""
    def blank(m):
        s = m.group(0)
        if s[0] in "\"'":
            return s[0] + " " * (len(s) - 2) + s[-1]
        return re.sub(r"[^\n]", " ", s)
    return _MASK_RE.sub(blank, text)


class Item:
    __slots__ = ("start", "end", "kind")

    def __init__(self, start, end, kind):
        self.start, self.end, self.kind = start, end, kind


def split_items(m):
    """Top-level items: 'pp' (preprocessor), 'fn' (function definition), 'decl' (anything else)."""
    items, n, i = [], len(m), 0
    start = 0
    while i < n:
        j = i
        while j < n and m[j] in " \t\r\n":
            j += 1
        if j >= n:
            break
        if m[j] == "#":
            k = j
            while True:
                e = m.find("\n", k)
                if e < 0:
                    e = n
                    break
                if m[e - 1] == "\\":
                    k = e + 1
                    continue
                break
            items.append(Item(start, e, "pp"))
            i = start = e
            continue
        depth = par = 0
        fn = False
        k = j
        end = None
        while k < n:
            c = m[k]
            if c == "(":
                par += 1
            elif c == ")":
                par -= 1
            elif c == "{":
                if depth == 0 and par == 0 and not fn:
                    p = k - 1
                    while p >= j and m[p] in " \t\r\n":
                        p -= 1
                    if p >= j and m[p] == ")":
                        fn = True
                depth += 1
            elif c == "}":
                depth -= 1
                if depth <= 0 and fn:
                    end = k + 1
                    break
            elif c == ";" and depth == 0 and par == 0:
                end = k + 1
                break
            k += 1
        if end is None:
            end = n
        items.append(Item(start, end, "fn" if fn else "decl"))
        i = start = end
    return items


def match_close(m, open_idx, o="(", c=")"):
    depth = 0
    for k in range(open_idx, len(m)):
        if m[k] == o:
            depth += 1
        elif m[k] == c:
            depth -= 1
            if depth == 0:
                return k
    return -1


def split_top(s):
    """Split a masked string at top-level commas -> [(start, end)] spans (not stripped)."""
    spans, depth, st = [], 0, 0
    for i, c in enumerate(s):
        if c in "([{":
            depth += 1
        elif c in ")]}":
            depth -= 1
        elif c == "," and depth == 0:
            spans.append((st, i))
            st = i + 1
    spans.append((st, len(s)))
    return spans


class Param:
    def __init__(self, text):
        self.text = text.strip()
        mm = re.match(r"^(.*?)(\w+)\s*(\[[^\]]*\])?\s*$", self.text, re.S)
        if mm and mm.group(1).strip():
            self.type, self.name = mm.group(1).strip(), mm.group(2)
            self.array = bool(mm.group(3))
        else:
            self.type, self.name, self.array = self.text, None, False
        words = set(re.findall(r"\w+", self.type))
        if "*" in self.type or self.array:
            self.cls = "ptr"
        elif words & FLOAT_WORDS and not (words - FLOAT_WORDS - {"const", "volatile"}):
            self.cls = "float"
        elif words and words <= INT_WORDS:
            self.cls = "int"
        else:
            self.cls = "other"


class Func:
    """A parsed function definition or declaration."""
    def __init__(self, name, kind, ret, params_span, params, proto, item, body=None):
        self.name, self.kind, self.ret = name, kind, ret
        self.params_span = params_span      # absolute span between the parentheses
        self.params = params                # [Param]
        self.proto = proto                  # False for `f()`
        self.item = item
        self.body = body                    # (open_idx, close_idx) absolute for definitions
        self.parsed = None                  # the Parsed file it came from


class Parsed:
    def __init__(self, text):
        self.text = text
        self.m = mask_c(text)
        self.items = split_items(self.m)
        self.defs = []
        self.decls = []
        for it in self.items:
            if it.kind == "fn":
                f = self._parse_def(it)
                if f:
                    f.parsed = self
                    self.defs.append(f)
            elif it.kind == "decl":
                f = self._parse_decl(it)
                if f:
                    f.parsed = self
                    self.decls.append(f)

    def _params(self, s_abs, e_abs):
        inner = self.m[s_abs:e_abs]
        if not inner.strip():
            return [], False
        if inner.strip() == "void":
            return [], True
        out = []
        for a, b in split_top(inner):
            out.append(Param(self.text[s_abs + a:s_abs + b]))
        return out, True

    def _parse_def(self, it):
        seg = self.m[it.start:it.end]
        b = seg.find("{")
        hdr = seg[:b].rstrip()
        if not hdr.endswith(")"):
            return None
        rp = len(hdr) - 1
        depth, k = 0, rp
        while k >= 0:
            if hdr[k] == ")":
                depth += 1
            elif hdr[k] == "(":
                depth -= 1
                if depth == 0:
                    break
            k -= 1
        if k < 0:
            return None
        mm = re.search(r"(\w+)\s*$", hdr[:k])
        if not mm:
            return None
        lead = len(seg) - len(seg.lstrip())
        ret = (it.start + lead, it.start + mm.start(1))
        ps = (it.start + k + 1, it.start + rp)
        params, proto = self._params(*ps)
        open_idx = it.start + b
        close_idx = match_close(self.m, open_idx, "{", "}")
        return Func(mm.group(1), "def", ret, ps, params, proto, it, (open_idx, close_idx))

    def _parse_decl(self, it):
        seg = self.m[it.start:it.end]
        if not seg.rstrip().endswith(";") or "=" in seg or re.search(r"\btypedef\b", seg):
            return None
        s = seg.rstrip()[:-1].rstrip()
        if not s.endswith(")"):
            return None
        depth, k = 0, len(s) - 1
        while k >= 0:
            if s[k] == ")":
                depth += 1
            elif s[k] == "(":
                depth -= 1
                if depth == 0:
                    break
            k -= 1
        if k < 0:
            return None
        mm = re.search(r"(\w+)\s*$", s[:k])
        if not mm or "(" in s[:mm.start(1)] or ")" in s[:mm.start(1)]:
            return None
        lead = len(seg) - len(seg.lstrip())
        ps = (it.start + k + 1, it.start + len(s) - 1)
        params, proto = self._params(*ps)
        return Func(mm.group(1), "decl", (it.start + lead, it.start + mm.start(1)), ps, params, proto, it)

    def calls_in_bodies(self, name):
        """[(args_span_abs)] for every `name(...)` occurring inside a function body."""
        out = []
        pat = re.compile(r"(?<![\w.>])" + re.escape(name) + r"\s*\(")
        for f in self.defs:
            o, c = f.body
            for mm in pat.finditer(self.m, o, c):
                lp = mm.end() - 1
                rp = match_close(self.m, lp)
                if rp > 0:
                    out.append((lp + 1, rp))
        return out

    def decl_statement_end(self, f):
        """Absolute index just after the last leading declaration in f's body (insert point)."""
        o, c = f.body
        pos = o + 1
        i = pos
        depth = 0
        stmt_start = pos
        last_end = pos
        while i < c:
            ch = self.m[i]
            if ch in "({[":
                depth += 1
            elif ch in ")}]":
                depth -= 1
            elif ch == ";" and depth == 0:
                stmt = self.m[stmt_start:i + 1]
                mm = re.match(r"\s*(?:(?:const|volatile|register|unsigned|signed|struct|union|enum)\s+)*"
                              r"(\w+)\s+[\w*\s]*?\**\s*\w+\s*(\[[^\]]*\]\s*)*(=|;|,)", stmt)
                first = re.match(r"\s*(\w+)", stmt)
                if mm and first and first.group(1) not in NOT_DECL:
                    last_end = i + 1
                    stmt_start = i + 1
                else:
                    break
            elif ch == "{" and depth == 0:
                break
            i += 1
        return last_end

    def find(self, name, kind=None):
        for f in self.defs + self.decls:
            if f.name == name and (kind is None or f.kind == kind):
                return f
        return None


_parse_cache = {}


def parse(text):
    k = hashlib.sha1(text.encode()).hexdigest()
    p = _parse_cache.get(k)
    if p is None:
        if len(_parse_cache) > 64:
            _parse_cache.clear()
        p = _parse_cache[k] = Parsed(text)
    return p


def apply_edits(text, edits):
    """edits: [(a, b, new)] non-overlapping."""
    for a, b, new in sorted(edits, key=lambda e: (e[0], e[1]), reverse=True):
        text = text[:a] + new + text[b:]
    return text


# =====================================================================================
# Source transformations (each: files dict -> files dict / keep list)
# =====================================================================================

def _param_text(p):
    return p.text


def _new_param_list(params, n, nextra_from):
    kept = [p.text for p in params[:n]]
    for k in range(len(params), n):
        kept.append("int __p%d" % k)
    return ", ".join(kept) if kept else "void"


def tx_params(files, fn, declared, used_all, demote=False):
    """Set the declared parameter count of fn (defs and prototyped decls), fix calls in files that
    see a prototype, optionally use every parameter in the definition bodies. declared=None -> K&R decl."""
    out = dict(files)
    ref = None
    for name, text in files.items():
        f = parse(text).find(fn)
        if f:
            ref = f if (ref is None or f.kind == "def") else ref
    if ref is None:
        return out
    for name, text in files.items():
        p = parse(text)
        edits = []
        proto_visible = False
        for f in p.defs + p.decls:
            if f.name != fn:
                continue
            if declared is None:
                if f.kind == "decl":
                    edits.append((f.params_span[0], f.params_span[1], ""))
                continue
            if f.kind == "decl" and not f.proto:
                continue
            proto_visible = True
            edits.append((f.params_span[0], f.params_span[1], _new_param_list(f.params, declared, 0)))
            if f.kind == "def" and demote and declared < len(f.params):
                locs = "".join(" %s %s = 0;" % (q.type, q.name) for q in f.params[declared:]
                               if q.name and q.cls != "other" and not q.array)
                edits.append((f.body[0] + 1, f.body[0] + 1, locs))
            if f.kind == "def" and used_all:
                names = [q.name for q in f.params[:declared] if q.name]
                names += ["__p%d" % k for k in range(len(f.params), declared)]
                stmts = "".join(" %s += (int) %s;" % (SINK, nm) for nm in names)
                if stmts:
                    ins = p.decl_statement_end(f)
                    edits.append((ins, ins, stmts))
        if proto_visible and declared is not None:
            for a, b in p.calls_in_bodies(fn):
                inner = p.m[a:b]
                spans = split_top(inner) if inner.strip() else []
                args = [p.text[a + s:a + e].strip() for s, e in spans]
                if len(args) > declared:
                    args = args[:declared]
                elif len(args) < declared:
                    args += ["0"] * (declared - len(args))
                edits.append((a, b, ", ".join(args)))
        if edits:
            out[name] = apply_edits(text, edits)
    return out


def _split_ret(text):
    """(storage prefix, type text)."""
    words = text.split()
    pre = [w for w in words if w in STORAGE]
    ty = [w for w in words if w not in STORAGE]
    return (" ".join(pre) + " " if pre else ""), " ".join(ty)


def tx_ret(files, fn, newtype):
    out = dict(files)
    for name, text in files.items():
        p = parse(text)
        edits = []
        for f in p.defs + p.decls:
            if f.name != fn:
                continue
            old = text[f.ret[0]:f.ret[1]]
            pre, _ = _split_ret(old)
            edits.append((f.ret[0], f.ret[1], pre + newtype + " "))
            if f.kind == "def" and newtype == "void":
                o, c = f.body
                for mm in re.finditer(r"\breturn\b[^;]*;", p.m[o:c]):
                    edits.append((o + mm.start(), o + mm.end(), "return;"))
        if edits:
            out[name] = apply_edits(text, edits)
    return out


def tx_argtype(files, fn, idx, newtype):
    out = dict(files)
    for name, text in files.items():
        p = parse(text)
        edits = []
        for f in p.defs + p.decls:
            if f.name != fn or not f.proto or idx >= len(f.params):
                continue
            q = f.params[idx]
            spans = split_top(p.m[f.params_span[0]:f.params_span[1]])
            a, b = spans[idx]
            a += f.params_span[0]
            b += f.params_span[0]
            new = newtype + (" " + q.name if q.name else "")
            lead = len(text[a:b]) - len(text[a:b].lstrip())
            trail = len(text[a:b]) - len(text[a:b].rstrip())
            edits.append((a + lead, b - trail, new))
        if edits:
            out[name] = apply_edits(text, edits)
    return out


def _dead_switch_span(p, f):
    o, c = f.body
    mm = re.search(r"if\s*\(\s*0\s*\)\s*\{\s*switch\b", p.m[o:c])
    if not mm:
        return None
    a = o + mm.start()
    lb = p.m.index("{", a)
    return a, match_close(p.m, lb, "{", "}") + 1


def has_dead_switch(files, fn):
    for text in files.values():
        p = parse(text)
        f = p.find(fn, "def")
        if f and _dead_switch_span(p, f):
            return True
    return False


def tx_inline(files, fn, on):
    out = dict(files)
    for name, text in files.items():
        p = parse(text)
        f = p.find(fn, "def")
        if not f:
            continue
        span = _dead_switch_span(p, f)
        if on and not span:
            arg = next((q.name for q in f.params if q.name and q.cls in ("int", "ptr")), SINK)
            ins = p.decl_statement_end(f)
            out[name] = apply_edits(text, [(ins, ins, " " + DEAD_SWITCH % arg)])
        elif not on and span:
            out[name] = apply_edits(text, [(span[0], span[1], "")])
    return out


def site_args(f, k):
    args = []
    for q in f.params:
        args.append("0" if q.cls in ("ptr", "other") else str(k + 1))
    return ", ".join(args)


def tx_sites(files, keep, fn, count):
    """Append `count` stand-in callers of fn (kept out of line). Returns (files, keep)."""
    out = dict(files)
    keep = list(keep)
    home = None
    f = None
    for name, text in files.items():
        g = parse(text).find(fn, "def")
        if g:
            home, f = name, g
            break
    if home is None:
        for name, text in files.items():
            g = parse(text).find(fn)
            if g:
                home, f = name, g
                break
    if home is None:
        return out, keep
    text = out[home]
    for k in range(count):
        nm = "__probe_site_%s_%d" % (fn, k)
        text += "\nvoid %s(void) { %s(%s); }\n" % (nm, fn, site_args(f, k) if f.proto else "")
        keep.append(nm)
    out[home] = text
    return out, keep


def call_statements(p, name):
    """[(start, end)] of `name(...);` expression statements inside function bodies."""
    out = []
    pat = re.compile(r"(?<![\w.>])" + re.escape(name) + r"\s*\(")
    for f in p.defs:
        o, c = f.body
        for mm in pat.finditer(p.m, o + 1, c):
            rp = match_close(p.m, mm.end() - 1)
            if rp < 0:
                continue
            after = re.match(r"\s*;", p.m[rp + 1:rp + 40])
            before = re.search(r"(?:[;{}):]|\belse|\bdo)\s*$", p.m[max(o, mm.start() - 40):mm.start()])
            if after and (before or p.m[o + 1:mm.start()].strip() == ""):
                out.append((mm.start(), rp + 1 + after.end()))
    return out


def tx_sites_remove(files, fn, which):
    """Replace existing `fn(...);` statements with `;` (which: 'last' or 'all')."""
    out = dict(files)
    hits = []
    for n, t in files.items():
        for a, b in call_statements(parse(t), fn):
            hits.append((n, a, b))
    if which == "last":
        hits = hits[-1:]
    per = {}
    for n, a, b in hits:
        per.setdefault(n, []).append((a, b, ";"))
    for n, edits in per.items():
        out[n] = apply_edits(files[n], edits)
    return out


def count_call_statements(files, fn):
    return sum(len(call_statements(parse(t), fn)) for t in files.values())


def tx_order(files, mode, member, callees_first=None):
    """Reorder function definitions within each file; add prototypes for what moved."""
    out = dict(files)
    for name, text in files.items():
        p = parse(text)
        fn_items = [it for it in p.items if it.kind == "fn"]
        if len(fn_items) < 2:
            continue
        infos = {id(it): next(f for f in p.defs if f.item is it) for it in fn_items}
        names = [infos[id(it)].name for it in fn_items]
        order = list(range(len(fn_items)))
        if mode == "reverse":
            order.reverse()
        elif mode in ("member_first", "member_last") and member in names:
            mi = names.index(member)
            rest = [i for i in order if i != mi]
            order = [mi] + rest if mode == "member_first" else rest + [mi]
        elif mode in ("callees_first", "callers_first") and callees_first:
            rank = {n: i for i, n in enumerate(callees_first)}
            order.sort(key=lambda i: rank.get(names[i], 10 ** 6))
            if mode == "callers_first":
                order.reverse()
        if order == list(range(len(fn_items))):
            continue
        pieces = [text[it.start:it.end] for it in fn_items]
        protos = []
        for it in fn_items:
            f = infos[id(it)]
            protos.append("%s %s(%s);" % (re.sub(r"\s+", " ", text[f.ret[0]:f.ret[1]]).strip(),
                                          f.name, text[f.params_span[0]:f.params_span[1]].strip()))
        edits = []
        for slot, src in zip(fn_items, order):
            edits.append((slot.start, slot.end, pieces[src]))
        first = fn_items[0].start
        edits.append((first, first, "\n" + "\n".join(protos) + "\n"))
        out[name] = apply_edits(text, edits)
    return out


def tx_patch(files, pt):
    """User patch: {"name", "find" (regex), "replace", "file" (optional), "count" (default 1),
    "edits": [{find, replace, count}] more regex edits, "keep_add": [names]}. Replacement uses Python re syntax (\\1 groups); "append": text adds to the end of "file"."""
    out = dict(files)
    edits = list(pt.get("edits", []))
    if pt.get("find"):
        edits.insert(0, {"find": pt["find"], "replace": pt["replace"], "count": pt.get("count", 1)})
    for e in edits:
        for n in list(out):
            if pt.get("file") and pt["file"] != n:
                continue
            out[n] = re.sub(e["find"], e["replace"], out[n], count=e.get("count", 1), flags=re.S)
    if pt.get("append") is not None:
        n = pt.get("file") or list(files)[-1]
        out[n] = out[n] + "\n" + pt["append"] + "\n"
    return out


def with_sink_decl(files):
    out = {}
    for n, t in files.items():
        if SINK in t and not re.search(r"extern\s+int\s+" + SINK, t):
            t = "extern int %s;\n" % SINK + t
        out[n] = t
    return out


# =====================================================================================
# Register-usage extraction
# =====================================================================================

R = mipsdec.REGBIT
GPR = mipsdec.GPR
CALLEE_S = [R[n] for n in ("s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "s8")]
CALLER_SAVE_MASK = mipsdec.names_mask(
    ["a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7", "t8", "t9"]
    + ["f%d" % i for i in range(4, 20)])
RET_MASK = mipsdec.names_mask(["v0", "v1", "f0", "f1", "f2", "f3", "ra"])
IGNORE_ENTRY = mipsdec.names_mask(["zero", "sp", "ra", "gp", "at", "k0", "k1"])
ABI_ENTRY = mipsdec.names_mask(["a0", "a1", "a2", "a3", "f12", "f13", "f14", "f15"])
SETUP_MASK = mipsdec.names_mask(
    ["a0", "a1", "a2", "a3", "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7", "t8", "t9"]
    + ["f%d" % i for i in range(12, 20)])
CTRL = {"branch", "b", "j", "jal", "jr", "ret", "jalr"}


def _eff(ins):
    """(uses, defs) with callee-save stores (sw s0,x(sp)) not counted as reads."""
    u, d = ins.uses, ins.defs
    if ins.kind == "store" and ins.mem and ins.mem[0] == 29:
        if ins.mnem in ("sw", "sd") and (16 <= ins.rt <= 23 or ins.rt in (30, 31)):
            u &= ~(1 << ins.rt)
        elif ins.mnem in ("swc1", "sdc1") and ins.rt >= 20:
            u &= ~(1 << (32 + ins.rt))
            if ins.mnem == "sdc1":
                u &= ~(1 << (32 + (ins.rt ^ 1)))
    if ins.kind in ("jal", "jalr"):
        d |= RET_MASK
    return u, d


def _saved_regs(ins):
    if ins.kind == "store" and ins.mem and ins.mem[0] == 29:
        if ins.mnem in ("sw", "sd") and (16 <= ins.rt <= 23 or ins.rt == 30):
            return 1 << ins.rt
        if ins.mnem in ("swc1", "sdc1") and ins.rt >= 20:
            v = 1 << (32 + ins.rt)
            return v | (1 << (32 + (ins.rt ^ 1)) if ins.mnem == "sdc1" else 0)
    return 0


def _succ(insns):
    n = len(insns)
    succ = [[i + 1] if i + 1 < n else [] for i in range(n)]
    for i, ins in enumerate(insns):
        if not ins.delay or i + 1 >= n:
            continue
        d = i + 1
        tgt = ins.target // 4 if ins.target is not None and ins.kind in ("branch", "b") else None
        if tgt is not None and not 0 <= tgt < n:
            tgt = None
        fall = [d + 1] if d + 1 < n else []
        if ins.kind == "branch":
            succ[d] = ([tgt] if tgt is not None else []) + (fall if not ins.likely else [])
            if ins.likely:
                succ[i] = [d] + fall
        elif ins.kind == "b":
            succ[d] = [tgt] if tgt is not None else []
        elif ins.kind in ("jal", "jalr"):
            succ[d] = fall
        else:                                   # j, jr, ret: leaves the function (jump tables: unmodelled)
            succ[d] = []
    return succ


def _names(mask, only=None):
    return [n for n in mipsdec.mask_names(mask) if only is None or n in only]


def _order_regs(mask, first_use):
    names = mipsdec.mask_names(mask)
    return sorted(names, key=lambda n: (first_use.get(n, 1 << 30), mipsdec.REGBIT[n]))


def analyze_words(words):
    """Register-usage signature of one emitted function (list of 32-bit words)."""
    words = list(words)
    while words and words[-1] == 0 and len(words) > 1:
        words.pop()                              # alignment padding
    insns = mipsdec.decode_words(words, 0)
    n = len(insns)
    if not n:
        return {"n": 0, "entry": [], "setup": [], "across": [], "frame": 0, "saved": [], "unsaved": [],
                "used": {}, "calls": 0}
    succ = _succ(insns)
    eff = [_eff(i) for i in insns]
    live_in = [0] * n
    changed = True
    while changed:
        changed = False
        for i in range(n - 1, -1, -1):
            out = 0
            for s in succ[i]:
                out |= live_in[s]
            u, d = eff[i]
            new = u | (out & ~d)
            if new != live_in[i]:
                live_in[i] = new
                changed = True
    first_use = {}
    for i, (u, d) in enumerate(eff):
        for nm in mipsdec.mask_names(u):
            first_use.setdefault(nm, i)
    entry = _order_regs(live_in[0] & ~IGNORE_ENTRY, first_use)
    setup, across, call_idx = [], [], []
    for j, ins in enumerate(insns):
        if ins.kind not in ("jal", "jalr"):
            continue
        call_idx.append(j)
        # registers defined just before the call (incl. delay slot) and not consumed before it
        lo = j - 1
        window = []
        while lo >= 0 and insns[lo].kind not in CTRL and j - lo <= 14:
            window.append(lo)
            lo -= 1
        window = sorted(window)
        if j + 1 < n:
            window.append(j + 1)
        pending = {}
        for k in window:
            u, d = eff[k]
            for nm in mipsdec.mask_names(u):
                pending.pop(nm, None)
            for nm in mipsdec.mask_names(d & SETUP_MASK):
                pending[nm] = k
        setup.append(sorted(pending, key=lambda x: mipsdec.REGBIT[x]))
        live_after = live_in[j + 2] if j + 2 < n else 0
        across.append(_names(live_after & CALLER_SAVE_MASK & ~RET_MASK))
    frame = 0
    for ins in insns[:6]:
        if ins.mnem == "addiu" and ins.rs == 29 and ins.rt == 29 and ins.imm < 0:
            frame = -ins.imm
            break
    saved = 0
    defs_all = uses_all = 0
    for ins, (u, d) in zip(insns, eff):
        saved |= _saved_regs(ins)
        defs_all |= d
        uses_all |= u
    callee = 0
    for r in CALLEE_S:
        callee |= 1 << r
    for r in range(52, 64):
        callee |= 1 << r                         # f20..f31
    unsaved = _names(defs_all & callee & ~saved)
    used = {}
    allm = (defs_all | uses_all)
    for cls, test in (("a", lambda nm: nm in ("a0", "a1", "a2", "a3")),
                      ("t", lambda nm: re.fullmatch(r"t\d", nm) is not None),
                      ("s", lambda nm: re.fullmatch(r"s\d", nm) is not None),
                      ("f", lambda nm: re.fullmatch(r"f\d+", nm) is not None)):
        used[cls] = [nm for nm in mipsdec.mask_names(allm) if test(nm)]
    return {"n": n, "entry": entry, "setup": setup, "across": across, "frame": frame,
            "saved": _names(saved), "unsaved": unsaved, "used": used, "calls": len(call_idx)}


def abi_expected(params):
    """Registers the standard o32 ABI would use for a declared parameter list."""
    regs, slot, fp_ok, fpn = [], 0, True, 0
    for q in params:
        if q.cls == "float" and fp_ok and fpn < 2:
            regs.append("f%d" % (12 + 2 * fpn))
            fpn += 1
            slot += 1
        elif slot < 4:
            fp_ok = False
            regs.append("a%d" % slot)
            slot += 1
        else:
            fp_ok = False
    return regs


SIG_FIELDS = ("entry", "setup", "across", "frame", "unsaved")


def sig_diff(a, b):
    """{field: (a_value, b_value)} for the fields that differ."""
    out = {}
    for f in SIG_FIELDS:
        if a.get(f) != b.get(f):
            out[f] = (a.get(f), b.get(f))
    return out


def _setdist(x, y):
    return len(set(x) ^ set(y))


def sig_dist(a, b):
    """Distance between two signatures (0 = identical register usage)."""
    if not a or not b:
        return 99
    d = _setdist(a["entry"], b["entry"]) + _setdist(a["unsaved"], b["unsaved"])
    d += 1 if a["frame"] != b["frame"] else 0
    for k in range(max(len(a["setup"]), len(b["setup"]))):
        x = a["setup"][k] if k < len(a["setup"]) else ["?"]
        y = b["setup"][k] if k < len(b["setup"]) else ["?"]
        d += _setdist(x, y)
        x = a["across"][k] if k < len(a["across"]) else ["?"]
        y = b["across"][k] if k < len(b["across"]) else ["?"]
        d += _setdist(x, y)
    return d


def fmt_regs(v):
    if isinstance(v, list) and v and isinstance(v[0], list):
        return " ".join("#%d[%s]" % (i + 1, ",".join(x)) for i, x in enumerate(v)) or "-"
    if isinstance(v, list):
        return ",".join(v) or "-"
    return str(v)


# =====================================================================================
# Variant generation
# =====================================================================================

class Ctx:
    """Everything about the base group needed to build variants."""

    def __init__(self, group_dir, member):
        self.dir = Path(group_dir)
        self.spec = json.loads((self.dir / "group.json").read_text())
        self.files = {n: (self.dir / n).read_text() for n in self.spec["files"]}
        self.member = member
        names = list(self.spec["members"]) + list(self.spec.get("context", []))
        if member not in names:
            raise SystemExit("%s is neither a member nor a context entry of %s" % (member, self.dir))
        self.keep = list(self.spec.get("keep", []))
        self.flags = self.spec["flags"]
        self.defined = {}
        for n, t in self.files.items():
            for f in parse(t).defs:
                self.defined.setdefault(f.name, n)
        self.declared = set()
        for t in self.files.values():
            for f in parse(t).decls:
                self.declared.add(f.name)
        self.graph = self._callgraph()

    def _callgraph(self):
        g = {}
        names = set(self.defined) | self.declared
        pat = re.compile(r"(?<![\w.>])(\w+)\s*\(")
        for n, t in self.files.items():
            p = parse(t)
            for f in p.defs:
                o, c = f.body
                g.setdefault(f.name, set())
                for mm in pat.finditer(p.m, o + 1, c):
                    if mm.group(1) in names and mm.group(1) != f.name:
                        g[f.name].add(mm.group(1))
        return g

    def callees(self, fn, depth):
        seen, frontier = [], [fn]
        for _ in range(depth):
            nxt = []
            for f in frontier:
                for c in sorted(self.graph.get(f, ())):
                    if c not in seen and c != self.member:
                        seen.append(c)
                        nxt.append(c)
            frontier = nxt
        return seen

    def callers(self, fn, depth=2):
        seen, frontier = [], [fn]
        for _ in range(depth):
            nxt = []
            for f in frontier:
                for c, cs in sorted(self.graph.items()):
                    if f in cs and c not in seen and c != fn:
                        seen.append(c)
                        nxt.append(c)
            frontier = nxt
        return seen


def default_focus(ctx, limit=6):
    foc = [ctx.member] + ctx.callees(ctx.member, 2)
    return foc[:limit]


class Variant:
    def __init__(self, axis, focus, value, steps):
        self.axis, self.focus, self.value, self.steps = axis, focus, value, steps
        self.id = None

    @property
    def label(self):
        if not self.steps:
            return "base"
        return "; ".join("%s(%s)=%s" % (a, f, v) if f else "%s=%s" % (a, v) for a, f, v, _ in self.steps)

    def combined(self, other):
        return Variant("combo", None, None, self.steps + other.steps)


def step(axis, focus, value, args):
    return (axis, focus, value, args)


def build_variants(ctx, axes, focus, keep_limit=30, patches=None):
    """Stage-1 (one change at a time) variants; returns [Variant] starting with the baseline."""
    base = Variant("base", None, "base", [])
    out = [base]
    files = ctx.files

    def add(axis, f, value, args):
        out.append(Variant(axis, f, value, [step(axis, f, value, args)]))

    if "keep" in axes:
        related = [ctx.member] + ctx.callees(ctx.member, 3) + ctx.callers(ctx.member, 3)
        cands = []
        for n in ctx.keep + related:
            if n not in cands and (n in ctx.defined or n in ctx.keep):
                cands.append(n)
        for n in cands[:keep_limit]:
            add("keep", n, "out" if n in ctx.keep else "in", {"name": n, "to": n not in ctx.keep})
    for f in focus:
        defined = f in ctx.defined
        ref = ref_func(files, f)
        if ref is None:
            continue
        if "sites" in axes and (defined or ref.proto):
            for k in (1, 2, 3):
                add("sites", f, "+%d" % k, {"fn": f, "count": k})
            ncall = count_call_statements(files, f)
            if ncall >= 1 and f != ctx.member:
                add("sites", f, "-1", {"fn": f, "remove": "last"})
                if ncall >= 2:
                    add("sites", f, "-all", {"fn": f, "remove": "all"})
        if "params" in axes and (ref.proto or not defined):
            orig = len(ref.params)
            used = 0
            if defined:
                for i, q in enumerate(ref.params):
                    body = ref.parsed.m[ref.body[0]:ref.body[1]] if ref.kind == "def" else ""
                    if q.name and re.search(r"\b%s\b" % re.escape(q.name), body):
                        used = i + 1
                lo = 0 if f != ctx.member else orig
                levels = range(lo, orig + 3)
                ua_opts = (False, True) if f != ctx.member else (False,)
            else:
                levels = range(0, orig + 3)
                ua_opts = (False,)
            for n in levels:
                for ua in ua_opts:
                    if n == orig and not ua:
                        continue
                    dem = defined and n < used and f != ctx.member
                    if dem and ua:
                        continue
                    add("params", f, "d%d%s%s" % (n, "+use" if ua else "", "-demote" if dem else ""),
                        {"fn": f, "declared": n, "used_all": ua, "demote": dem})
            if not defined and ref.proto:
                add("params", f, "K&R()", {"fn": f, "declared": None, "used_all": False})
        if "ret" in axes:
            cur = _split_ret(re.sub(r"\s+", " ", files_text_ret(ctx, f)))[1]
            for ty in ("void", "int", "short", "float"):
                if cur.replace(" ", "") not in (ty, {"s16": "short", "s32": "int", "f32": "float"}.get(ty, ty)):
                    add("ret", f, ty, {"fn": f, "type": ty})
        if "args" in axes and ref.proto:
            for i, q in enumerate(ref.params[:4]):
                if q.cls != "int":
                    continue
                for ty in ("short", "unsigned char", "int"):
                    if q.type.replace("const ", "").replace("volatile ", "").strip() != ty:
                        add("args", f, "p%d:%s" % (i, ty), {"fn": f, "idx": i, "type": ty})
        if "inline" in axes and defined:
            on = has_dead_switch(files, f)
            add("inline", f, "off" if on else "on", {"fn": f, "on": not on})
    for pt in (patches or []):
        add("patch", None, pt["name"], {"patch": pt})
    if "flags" in axes:
        fl = ctx.flags
        if "-O3" in fl:
            add("flags", None, "-O2", {"flags": fl.replace("-O3", "-O2")})
        elif "-O2" in fl:
            add("flags", None, "-O3", {"flags": fl.replace("-O2", "-O3")})
    if "order" in axes:
        ccount = sum(len(parse(t).defs) for t in files.values())
        if ccount > 1:
            chain = [ctx.member] + ctx.callees(ctx.member, 4)
            for mode in ("reverse", "member_first", "member_last", "callees_first", "callers_first"):
                add("order", None, mode, {"mode": mode, "chain": list(reversed(chain))})
        if len(files) > 1:
            add("order", None, "files_reversed", {"files_reversed": True})
    return out


def ref_func(files, fn):
    """Best description of fn across the files: a definition, else a prototyped declaration, else any."""
    best = None
    for t in files.values():
        p = parse(t)
        g = p.find(fn, "def")
        if g:
            return g
        for d in p.decls:
            if d.name == fn and (best is None or (d.proto and not best.proto)):
                best = d
    return best


def files_text_ret(ctx, fn):
    g = ref_func(ctx.files, fn)
    return g.parsed.text[g.ret[0]:g.ret[1]] if g else ""


def realize(ctx, variant):
    """Apply a variant's steps -> (files, keep, flags, reorder_files)."""
    files = dict(ctx.files)
    keep = list(ctx.keep)
    flags = ctx.flags
    rev_files = False
    for axis, f, value, a in variant.steps:
        if axis == "keep":
            if a["to"] and a["name"] not in keep:
                keep.append(a["name"])
            elif not a["to"] and a["name"] in keep:
                keep.remove(a["name"])
        elif axis == "sites":
            if a.get("remove"):
                files = tx_sites_remove(files, a["fn"], a["remove"])
            else:
                files, keep = tx_sites(files, keep, a["fn"], a["count"])
        elif axis == "params":
            files = tx_params(files, a["fn"], a["declared"], a["used_all"], a.get("demote", False))
        elif axis == "ret":
            files = tx_ret(files, a["fn"], a["type"])
        elif axis == "args":
            files = tx_argtype(files, a["fn"], a["idx"], a["type"])
        elif axis == "inline":
            files = tx_inline(files, a["fn"], a["on"])
        elif axis == "patch":
            files = tx_patch(files, a["patch"])
            for k in a["patch"].get("keep_add", []):
                if k not in keep:
                    keep.append(k)
        elif axis == "flags":
            flags = a["flags"]
        elif axis == "order":
            if a.get("files_reversed"):
                rev_files = True
            else:
                files = tx_order(files, a["mode"], ctx.member, a.get("chain"))
    files = with_sink_decl(files)
    return files, keep, flags, rev_files


# =====================================================================================
# Running
# =====================================================================================

def _worker_init():
    builder._init_worker()


def _run(job):
    tg = builder.get_targets()
    for n in job.get("aux", ()):
        tg.setdefault(n, [0])                   # dummy target: we only want the emitted words
    try:
        return builder.compile_group(job["dir"], job.get("overrides"), timeout=job.get("timeout", 120.0),
                                     use_cache=job.get("use_cache", True))
    finally:
        if job.get("cleanup"):
            shutil.rmtree(job["cleanup"], ignore_errors=True)


def run_jobs(jobs, jobs_n):
    if jobs_n <= 1 or len(jobs) <= 1:
        return [_run(j) for j in jobs]
    with ProcessPoolExecutor(max_workers=jobs_n, initializer=_worker_init) as ex:
        return list(ex.map(_run, jobs, chunksize=1))


def make_job(ctx, variant, aux, scratch, use_cache=True, timeout=120.0):
    files, keep, flags, rev_files = realize(ctx, variant)
    changed = {n: t for n, t in files.items() if t != ctx.files.get(n)}
    ov = {"keep": keep, "flags": flags}
    ctxn = list(ctx.spec.get("context", []))
    for n in aux:
        if n not in ctxn and n not in ctx.spec["members"]:
            ctxn.append(n)
    ov["context"] = ctxn
    job = {"aux": [n for n in aux if n not in ctx.spec["members"]], "use_cache": use_cache,
           "timeout": timeout}
    if rev_files:
        d = Path(tempfile.mkdtemp(prefix="probe-", dir=scratch))
        spec = dict(ctx.spec)
        spec["files"] = list(reversed(spec["files"]))
        (d / "group.json").write_text(json.dumps(spec))
        for n, t in files.items():
            (d / n).write_text(t)
        job["dir"] = str(d)
        job["cleanup"] = str(d)
        job["overrides"] = ov
    else:
        job["dir"] = str(ctx.dir)
        ov["files"] = changed
        job["overrides"] = ov
    return job


def choose_aux(ctx, focus):
    names = []
    for n in focus + ctx.callers(ctx.member, 2):
        if n in ctx.defined and n != ctx.member and n not in names and not n.startswith("__probe"):
            names.append(n)
    return names[:12]


def target_words(ctx, name):
    tg = dict(builder.get_targets())
    tg.update(builder.extra_targets(ctx.spec.get("targets")))
    return tg.get(name)


def summarize(ctx, variant, res, target_sig, views):
    mem_views = {}
    for n in views:
        d = res.get("members", {}).get(n) or res.get("context", {}).get(n)
        if d and not d.get("err") and d.get("words"):
            mem_views[n] = analyze_words(d["words"])
    m = res.get("members", {}).get(ctx.member) or res.get("context", {}).get(ctx.member) or {}
    row = {
        "id": variant.id, "label": variant.label, "axis": variant.axis, "focus": variant.focus,
        "value": variant.value, "steps": [(a, f, v) for a, f, v, _ in variant.steps],
        "err": (res.get("err") or m.get("err")),
        "strict_diff": m.get("strict_diff"), "size": m.get("size"), "target_size": m.get("target_size"),
        "aligned_exact": m.get("aligned_exact"), "aligned_opcode": m.get("aligned_opcode"),
        "aligned_opcode_reg": m.get("aligned_opcode_reg"), "matched": bool(m.get("matched")),
        "group_strict": sum(d.get("strict_diff", 0) for d in res.get("members", {}).values()),
        "group_matched": bool(res.get("matched")),
        "emitted": res.get("emitted", {}), "cached": res.get("cached"),
        "sig": mem_views,
    }
    msig = mem_views.get(ctx.member)
    row["reg_dist"] = sig_dist(msig, target_sig) if (msig and target_sig) else (99 if target_sig else None)
    if row["err"]:
        row["strict_diff"] = m.get("strict_diff", m.get("target_size", 10 ** 6)) if m else 10 ** 6
    return row


def rank_key(r):
    return (r["err"] is not None, 0 if r["matched"] else 1, r["strict_diff"] if r["strict_diff"] is not None else 10 ** 6,
            -(r["aligned_exact"] or 0), r["reg_dist"] if r["reg_dist"] is not None else 99,
            r["group_strict"], -(r["aligned_opcode_reg"] or 0), r["id"])


def probe(group_dir, member, axes=None, focus=None, jobs_n=None, combine=8, max_combined=120,
          use_cache=True, timeout=120.0, keep_limit=30, exhaust=False, patches=None):
    """Run the probe. Returns the result dict (see CLI)."""
    t0 = time.time()
    if not builder.ido_available():
        return {"error": "IDO missing: run tools/cloud/setup.sh", "rows": []}
    axes = list(axes or ALL_AXES)
    ctx = Ctx(group_dir, member)
    focus = list(focus) if focus else default_focus(ctx)
    aux = choose_aux(ctx, focus)
    views = [member] + [n for n in aux if n != member]
    tw = target_words(ctx, member)
    target_sig = analyze_words(tw) if tw else None
    jobs_n = jobs_n or os.cpu_count() or 1
    scratch = tempfile.mkdtemp(prefix="probe-scratch-")
    try:
        variants = build_variants(ctx, axes, focus, keep_limit, patches)
        for i, v in enumerate(variants):
            v.id = i
        rows = _run_variants(ctx, variants, aux, views, target_sig, scratch, jobs_n, use_cache, timeout)
        stage1 = len(rows)
        combos = []
        if (combine or exhaust) and len(rows) > 2:
            combos = plan_combos(rows, variants, combine, max_combined, exhaust)
            for v in combos:
                v.id = len(variants)
                variants.append(v)
            if combos:
                rows += _run_variants(ctx, combos, aux, views, target_sig, scratch, jobs_n, use_cache, timeout)
    finally:
        shutil.rmtree(scratch, ignore_errors=True)
    rows_sorted = sorted(rows, key=rank_key)
    out = {
        "group": str(group_dir), "member": member, "axes": axes, "focus": focus, "aux": aux,
        "target_sig": target_sig, "target_words": len(tw) if tw else None,
        "stage1": stage1, "total": len(rows), "secs": time.time() - t0,
        "rows": rows_sorted, "by_id": {r["id"]: r for r in rows},
    }
    out["rules"] = discover_rules(out)
    return out


def _sig_changed(r, base):
    for fn, sig in r["sig"].items():
        bs = base["sig"].get(fn)
        if bs and sig_diff(bs, sig):
            return True
    return False


def plan_combos(rows, variants, combine, max_combined, exhaust):
    """Stage-2 variants. Default: the best level of each useful (axis, focus) group, combined two and
    three at a time. exhaust: full factorial over every (axis, focus) group's levels (seeded sample
    when larger than max_combined)."""
    base = rows[0]
    groups = {}
    for r, v in zip(rows[1:], variants[1:]):
        if r["err"] or not v.steps:
            continue
        groups.setdefault((v.axis, v.focus), []).append((r, v))
    combos = []
    if exhaust:
        # levels without any effect on scores or registers cannot interact: leave them out
        eff = {k: [v for r, v in lst if _sig_changed(r, base) or
                   (r["strict_diff"], r["aligned_exact"]) != (base["strict_diff"], base["aligned_exact"])]
               for k, lst in groups.items()}
        keys = sorted((k for k in eff if eff[k]), key=str)
        level_lists = [[None] + eff[k] for k in keys]
        total = 1
        for ll in level_lists:
            total *= len(ll)
        if total <= max_combined * 4:
            picks = itertools.product(*level_lists)
        else:
            rnd = random.Random(1)
            picks = (tuple(rnd.choice(ll) for ll in level_lists) for _ in range(max_combined * 2))
        seen = set()
        for pick in picks:
            steps = [st for v in pick if v for st in v.steps]
            if len(steps) < 2:
                continue
            k = json.dumps([(a, f, val) for a, f, val, _ in steps])
            if k in seen:
                continue
            seen.add(k)
            combos.append(Variant("combo", None, None, steps))
            if len(combos) >= max_combined:
                break
        return combos
    best = []
    for key, lst in groups.items():
        useful = [(rank_key(r), v) for r, v in lst if _sig_changed(r, base) or
                  (r["strict_diff"], -(r["aligned_exact"] or 0)) < (base["strict_diff"], -(base["aligned_exact"] or 0))]
        if useful:
            useful.sort(key=lambda x: x[0])
            best.append(useful[0])
    best.sort(key=lambda x: x[0])
    top = [v for _, v in best[:combine]]
    for size in (2, 3):
        for sub in itertools.combinations(top, size):
            steps = [st for s in sub for st in s.steps]
            combos.append(Variant("combo", None, None, steps))
            if len(combos) >= max_combined:
                return combos
    return combos


def _run_variants(ctx, variants, aux, views, target_sig, scratch, jobs_n, use_cache, timeout):
    jobs = [make_job(ctx, v, aux, scratch, use_cache, timeout) for v in variants]
    results = run_jobs(jobs, jobs_n)
    return [summarize(ctx, v, r, target_sig, views) for v, r in zip(variants, results)]


# =====================================================================================
# Rule discovery
# =====================================================================================

def discover_rules(result):
    """Which axis changed which register assignment."""
    by_id = result["by_id"]
    base = by_id[0]
    rules = {"changes": [], "series": [], "fixes": [], "target_hits": [], "no_effect": []}
    if base["err"]:
        rules["error"] = "baseline does not compile: %s" % (base["err"].splitlines()[0] if base["err"] else "")
        return rules
    series = {}
    touched = set()
    for r in sorted(result["rows"], key=lambda x: x["id"]):
        if r["axis"] in ("base", "combo") or r["err"]:
            continue
        diffs = []
        for fn, sig in r["sig"].items():
            bs = base["sig"].get(fn)
            if not bs:
                continue
            d = sig_diff(bs, sig)
            for fld, (a, b) in d.items():
                diffs.append((fn, fld, a, b))
                key = (r["axis"], r["focus"], fn, fld)
                series.setdefault(key, {"base": bs.get(fld)})[r["value"]] = sig.get(fld)
        if diffs:
            touched.add((r["axis"], r["focus"], r["value"]))
            rules["changes"].append({"variant": r["label"], "id": r["id"], "axis": r["axis"],
                                     "focus": r["focus"], "value": r["value"],
                                     "diffs": [{"function": fn, "field": fld, "from": a, "to": b}
                                               for fn, fld, a, b in diffs]})
        else:
            rules["no_effect"].append(r["label"])
        if (r["strict_diff"], -(r["aligned_exact"] or 0)) < (base["strict_diff"], -(base["aligned_exact"] or 0)):
            rules["fixes"].append({"variant": r["label"], "id": r["id"], "strict_diff": r["strict_diff"],
                                   "base_strict_diff": base["strict_diff"], "matched": r["matched"]})
        if r["reg_dist"] == 0 and r["id"] != 0:
            rules["target_hits"].append(r["label"])
    member = result["member"]
    fld_rank = {"entry": 0, "across": 1, "setup": 2, "unsaved": 3, "frame": 4}
    for (axis, focus, fn, fld), vals in sorted(
            series.items(), key=lambda kv: ((ALL_AXES + EXTRA_AXES).index(kv[0][0]), str(kv[0][1]), kv[0][2] != member,
                                            fld_rank[kv[0][3]], kv[0][2])):
        if len({json.dumps(v) for v in vals.values()}) > 1:
            rules["series"].append({"axis": axis, "focus": focus, "function": fn, "field": fld,
                                    "values": {k: v for k, v in vals.items()}})
    return rules


def rule_sentences(result, member_only=False, limit=None):
    """Human-readable rule lines from the series. Non-member functions only report the fields that
    carry IPA evidence (entry, live-across, unsaved); call-setup and frame are member-only."""
    out = []
    member = result["member"]
    for s in result["rules"].get("series", []):
        if member_only and s["function"] != member:
            continue
        if s["function"] != member and s["field"] in ("setup", "frame"):
            continue
        vals = s["values"]
        parts = ["%s:%s" % (k, fmt_regs(v)) for k, v in vals.items()]
        what = {"entry": "entry-read regs", "setup": "call-setup regs", "across": "live-across-call regs",
                "frame": "frame size", "unsaved": "unsaved s-regs"}[s["field"]]
        target = "(%s) " % s["focus"] if s["focus"] else ""
        out.append("%s%s -> `%s` %s:  %s" % (s["axis"], target, s["function"], what, " | ".join(parts)))
    return out[:limit] if limit else out


# =====================================================================================
# Output
# =====================================================================================

def _short(label, n=150):
    label = label.replace("|", "/")
    return label if len(label) <= n else label[:n - 1] + "..."


def _msig(r, member):
    return r["sig"].get(member) or {}


def render_markdown(result, top=25, show_all=False, max_rules=40):
    if result.get("error"):
        return "ERROR: " + result["error"] + "\n"
    member = result["member"]
    rows = result["rows"]
    tsig = result.get("target_sig")
    L = []
    L.append("# probe: %s / %s" % (Path(result["group"]).name, member))
    L.append("")
    L.append("axes: %s; focus: %s; %d variants (%d one-at-a-time, %d combined); %.1fs"
             % (",".join(result["axes"]), ", ".join(result["focus"]), result["total"], result["stage1"],
                result["total"] - result["stage1"], result["secs"]))
    if tsig:
        L.append("")
        L.append("target `%s` (%d words): entry [%s]  call-setup %s  live-across %s  frame %d  unsaved [%s]"
                 % (member, result["target_words"], fmt_regs(tsig["entry"]), fmt_regs(tsig["setup"]),
                    fmt_regs(tsig["across"]), tsig["frame"], fmt_regs(tsig["unsaved"])))
    L.append("")
    L.append("| # | variant | strict | aligned | opc+reg | size | group | dist | entry | call-setup | live-across | frame | unsaved |")
    L.append("|---|---|---|---|---|---|---|---|---|---|---|---|---|")
    shown = rows if show_all else rows[:top]
    for i, r in enumerate(shown, 1):
        s = _msig(r, member)
        if r["err"]:
            L.append("| %d | %s | ERROR | | | | | | %s | | | | |" % (i, r["label"], (r["err"].splitlines() or [""])[0][:60].replace("|", "/")))
            continue
        L.append("| %d | %s%s | %s | %s | %s | %s/%s | %s | %s | %s | %s | %s | %s | %s |" % (
            i, "**MATCH** " if r["matched"] else "", r["label"].replace("|", "/"), r["strict_diff"],
            r["aligned_exact"], r["aligned_opcode_reg"], r["size"], r["target_size"], r["group_strict"],
            r["reg_dist"] if r["reg_dist"] is not None else "-", fmt_regs(s.get("entry", [])),
            fmt_regs(s.get("setup", [])), fmt_regs(s.get("across", [])), s.get("frame", "-"),
            fmt_regs(s.get("unsaved", []))))
    if not show_all and len(rows) > top:
        L.append("")
        L.append("(%d more variants; --all shows them, --json has everything)" % (len(rows) - top))
    L.append("")
    L.append("## Rule discovery")
    L.append("")
    rules = result["rules"]
    if rules.get("error"):
        L.append(rules["error"])
    else:
        sents = rule_sentences(result)
        if sents:
            for s in sents[:max_rules]:
                L.append("- " + s)
            if len(sents) > max_rules:
                L.append("- (%d more; --max-rules N, or --json)" % (len(sents) - max_rules))
        else:
            L.append("- no axis changed any register assignment of the observed functions")
        if rules["fixes"]:
            L.append("")
            best = sorted(rules["fixes"], key=lambda f: f["strict_diff"])[:5]
            L.append("Single changes that improve the member score (baseline strict %s): %s" % (
                rules["fixes"][0]["base_strict_diff"],
                "; ".join("%s -> %s%s" % (f["variant"], f["strict_diff"], " MATCH" if f["matched"] else "")
                          for f in best)))
        else:
            L.append("")
            L.append("No single change improves the member score over the baseline.")
        if rules["target_hits"]:
            L.append("Variants whose register signature equals the target: %s" % "; ".join(rules["target_hits"][:8]))
        elif tsig:
            L.append("No variant reproduces the target's register signature (best distance %s)." % min(
                (r["reg_dist"] for r in rows if r["reg_dist"] is not None and not r["err"]), default="n/a"))
        if rules["no_effect"]:
            L.append("No register effect (observed functions): %s" % ", ".join(rules["no_effect"][:20])
                     + (" ..." if len(rules["no_effect"]) > 20 else ""))
    best = rows[0] if rows else None
    if best:
        L.append("")
        L.append("Best: `%s` strict %s, aligned %s/%s%s" % (best["label"], best["strict_diff"], best["aligned_exact"],
                                                          best["target_size"], " (MATCH)" if best["matched"] else ""))
    return "\n".join(L) + "\n"


def to_json(result):
    r = dict(result)
    r.pop("by_id", None)
    return r


# =====================================================================================
# Validation: reproduce this session's findings
# =====================================================================================

GROUPS = ROOT / "cloud" / "work" / "ipa-groups"


def _row(result, axis, focus, value):
    for r in result["rows"]:
        if r["axis"] == axis and r["focus"] == focus and r["value"] == value:
            return r
    return None


_STANDIN = ("void vector_normalize_length(void *a, void *b) { if (0) { switch ((int) a) { case 1: " + SINK +
            " = 3; case 2: " + SINK + " = 4; case 3: " + SINK + " = 5; } } " + SINK + " = (int) a + (int) b; }")
B640_PATCHES = [
    {"name": "vnl-call-via-fnptr", "find": r"vector_normalize_length\(&v\[9\], v\);",
     "replace": "((void (*)(void *, void *)) vector_normalize_length)(&v[9], v);"},
    {"name": "register-idx", "find": r"void func_8008B640\(s32 idx, f32 x",
     "replace": "void func_8008B640(register s32 idx, f32 x"},
    {"name": "vnl-standin-abi", "find": "", "append": _STANDIN, "keep_add": ["vector_normalize_length"]},
    {"name": "vnl-standin-ipa", "find": "", "append": _STANDIN},
    {"name": "idx-used-after-call", "find": r"v\[11\] = nz;", "replace": "v[11] = nz + (f32) idx;"},
    {"name": "v-reloaded-after-call",
     "find": r"vector_normalize_length\(&v\[9\], v\);\s*v\[9\] = nx;",
     "replace": "vector_normalize_length(&v[9], v); v = *(f32 **) ((u8 *) &D_8012E708 + ((s16) idx * 0x44)); v[9] = nx;"},
    {"name": "idx-s32-no-cast", "find": r"\(s16\) idx \* 0x44", "replace": "idx * 0x44"},
    {"name": "two-step-ptr", "find": r"v = \*\(f32 \*\*\) \(\(u8 \*\) &D_8012E708 \+ \(\(s16\) idx \* 0x44\)\);",
     "replace": "{ s32 o = (s16) idx * 0x44; v = *(f32 **) ((u8 *) &D_8012E708 + o); }"},
]


def validate(jobs_n=None, use_cache=True, say=print):
    """Run the scenarios; returns {name: {"ok": bool|None, "notes": [...], "rules": [...]}}."""
    out = {}
    common = dict(jobs_n=jobs_n, use_cache=use_cache)

    # (1) mode_byte_set: the callee's parameter count picks the IPA register t0/t1/t2
    res = probe(GROUPS / "mode_byte_set", "mode_byte_set", ["params", "sites", "inline"],
                focus=["func_80096288", "sound_update_channel"], combine=0, **common)
    if res.get("error"):
        out["mode_byte_set"] = {"ok": None, "notes": [res["error"]], "rules": []}
    else:
        mp = {}
        for r in res["rows"]:
            if (r["axis"] == "params" and r["focus"] == "func_80096288" and not r["err"]) or r["axis"] == "base":
                s_ = _msig(r, "mode_byte_set")
                reg = [x for grp in s_.get("setup", []) for x in grp if x.startswith("t")]
                mp[r["value"] if r["axis"] != "base" else "base(3 declared, 2 used)"] = (reg, r["strict_diff"])
        order = sorted(mp, key=lambda k: (0 if k.startswith("base") else 1, k))
        notes = ["func_80096288 declared/used -> temp register mode_byte_set's caller setup uses (strict diff): "
                 + "; ".join("%s:%s(%s)" % (k, ",".join(mp[k][0]) or "-", mp[k][1]) for k in order)]
        g = lambda k: (mp.get(k) or (None,))[0]
        ok = g("base(3 declared, 2 used)") == ["t0"] and g("d3+use") == ["t1"] and g("d4+use") == ["t2"]
        notes.append("rule: used parameter count 2/3/4 of the callee -> IPA register t0/t1/t2 (5 -> t3); declared-but-unused "
                     "parameters (d2, d4 unused) do not move it; expected [t0]/[t1]/[t2], got %s/%s/%s"
                     % (g("base(3 declared, 2 used)"), g("d3+use"), g("d4+use")))
        sites = _row(res, "sites", "func_80096288", "-1")
        inl = _row(res, "inline", "func_80096288", "off")
        if sites and inl:
            ncalls = lambda r: _msig(r, "mode_byte_set").get("calls", 0)
            notes.append("inlining: dropping one of the two call sites of func_80096288 in sound_update_channel -> mode_byte_set "
                         "loses its call (calls %s -> %s, strict %s); removing the dead switch -> calls %s -> %s, strict %s"
                         % (ncalls(res["by_id"][0]), ncalls(sites), sites["strict_diff"], ncalls(res["by_id"][0]), ncalls(inl),
                            inl["strict_diff"]))
        out["mode_byte_set"] = {"ok": ok, "notes": notes, "rules": rule_sentences(res, True, 6)}
    say("[1] mode_byte_set param count -> t0/t1/t2: %s" % out["mode_byte_set"]["ok"])

    # (2) func_8008705C: the stand-in callee has to use a0-a3 (clobber a2/a3) to leave t0 for the mask
    res = probe(GROUPS / "func_8008705C", "func_8008705C", ["params", "sites", "inline"],
                focus=["func_80086A50"], combine=0, **common)
    if res.get("error"):
        out["func_8008705C"] = {"ok": None, "notes": [res["error"]], "rules": []}
    else:
        base = res["by_id"][0]
        ser = []
        for r in res["rows"]:
            if r["axis"] == "params" and r["focus"] == "func_80086A50" and not r["err"]:
                ser.append((r["id"], r["value"], r["strict_diff"], _msig(r, "func_8008705C").get("across", [[]])[0]))
        ser.sort()
        notes = ["baseline (stub uses 4 params a0-a3): strict %s, mask live across the call in %s"
                 % (base["strict_diff"], fmt_regs(_msig(base, "func_8008705C").get("across", [])[:1])),
                 "stub params -> (strict, register holding the mask across the call): " + "; ".join(
                     "%s:%s %s" % (v, sd, fmt_regs(a)) for _, v, sd, a in ser)]
        d2 = _row(res, "params", "func_80086A50", "d2-demote")
        d3 = _row(res, "params", "func_80086A50", "d3-demote")
        ok = bool(base["strict_diff"] == 0 and d2 and d3 and d2["strict_diff"] > 0 and d3["strict_diff"] > 0
                  and _msig(d2, "func_8008705C")["across"][0] == ["a2"]
                  and _msig(d3, "func_8008705C")["across"][0] == ["a3"])
        notes.append("rule: the caller's live-across value takes the first register the callee does not use: callee using "
                     "a0-a1 -> a2, a0-a2 -> a3, a0-a3 -> t0, then t1, t2. The stub must use all four (clobber a2/a3) for t0.")
        out["func_8008705C"] = {"ok": ok, "notes": notes, "rules": rule_sentences(res, True, 4)}
    say("[2] func_8008705C stand-in must clobber a2/a3: %s" % out["func_8008705C"]["ok"])

    # (2b) func_800878E0 group: func_8008A644 (target a2/a1; ours a3/a2 - real callers hold a3 live?)
    res = probe(GROUPS / "func_800878E0", "func_8008A644", ["keep", "sites", "params", "inline"], combine=6, **common)
    if res.get("error"):
        out["func_800878E0"] = {"ok": None, "notes": [res["error"]], "rules": []}
    else:
        base = res["by_id"][0]
        best = res["rows"][0]
        ts = res.get("target_sig") or {}
        out["func_800878E0"] = {
            "ok": None,
            "notes": ["member func_8008A644: baseline strict %s; best of %d variants `%s` strict %s; target entry %s / setup %s, "
                      "baseline entry %s / setup %s"
                      % (base["strict_diff"], res["total"], best["label"], best["strict_diff"], fmt_regs(ts.get("entry", [])),
                         fmt_regs(ts.get("setup", [])), fmt_regs(_msig(base, "func_8008A644").get("entry", [])),
                         fmt_regs(_msig(base, "func_8008A644").get("setup", [])))],
            "rules": rule_sentences(res, True, 4)}
    say("[2b] func_800878E0/func_8008A644: baseline %s best %s" % (base["strict_diff"], best["strict_diff"]))

    # (3) func_800AD650 must NOT be in keep
    res = probe(GROUPS / "func_800AD4C8", "func_800AD650", ["keep"], focus=["func_800AD650"], combine=0,
                keep_limit=40, **common)
    if res.get("error"):
        out["func_800AD650"] = {"ok": None, "notes": [res["error"]], "rules": []}
    else:
        base = res["by_id"][0]
        inkeep = _row(res, "keep", "func_800AD650", "in")
        notes = ["baseline (not in keep): member strict %s (MATCH=%s), group strict sum %s"
                 % (base["strict_diff"], base["matched"], base["group_strict"])]
        ok = False
        if inkeep:
            notes.append("with func_800AD650 in keep: member strict %s, aligned %s/%s, group strict sum %s"
                         % (inkeep["strict_diff"], inkeep["aligned_exact"], inkeep["target_size"], inkeep["group_strict"]))
            ok = base["matched"] and not inkeep["matched"] and inkeep["strict_diff"] > base["strict_diff"]
        else:
            notes.append("func_800AD650 is already in keep in this group")
        out["func_800AD650"] = {"ok": ok, "notes": notes, "rules": rule_sentences(res, True, 4)}
    say("[3] func_800AD650 must not be in keep: %s" % out["func_800AD650"]["ok"])

    # (4) func_8008B640: the open puzzle (index parameter in $a2, target $a0)
    res = probe(GROUPS / "func_8008B640", "func_8008B640", ALL_AXES + ["patch"], combine=10, exhaust=True,
                max_combined=400, patches=B640_PATCHES, **common)
    if res.get("error"):
        out["func_8008B640"] = {"ok": None, "notes": [res["error"]], "rules": []}
    else:
        base = res["by_id"][0]
        best = res["rows"][0]
        bs, ts = _msig(base, "func_8008B640"), res.get("target_sig") or {}
        a0 = [r for r in res["rows"] if not r["err"] and r["sig"].get("func_8008B640", {}).get("entry", [""])[:1] == ["a0"]]
        notes = ["%d variants (%d one-at-a-time + %d combinations) over axes %s; focus %s"
                 % (res["total"], res["stage1"], res["total"] - res["stage1"], ",".join(res["axes"]), ",".join(res["focus"])),
                 "target entry-read regs %s, baseline %s" % (fmt_regs(ts.get("entry", [])), fmt_regs(bs.get("entry", []))),
                 "baseline strict %s; best variant `%s` strict %s (aligned %s/%s), matched=%s"
                 % (base["strict_diff"], _short(best["label"], 90), best["strict_diff"], best["aligned_exact"],
                    best["target_size"], best["matched"]),
                 "variants putting the index in $a0: %d (best strict %s%s)"
                 % (len(a0), min((r["strict_diff"] for r in a0), default="n/a"),
                    "; " + _short(min(a0, key=rank_key)["label"], 90) if a0 else ""),
                 "variants reproducing the full target register signature: %d"
                 % len([r for r in res["rows"] if r["reg_dist"] == 0])]
        out["func_8008B640"] = {"ok": bool(best["matched"]), "cracked": bool(best["matched"]), "notes": notes,
                                "rules": rule_sentences(res, True, 6), "best": best["label"],
                                "best_strict": best["strict_diff"]}
    say("[4] func_8008B640 cracked=%s" % out["func_8008B640"].get("cracked"))
    return out


# =====================================================================================
# CLI
# =====================================================================================

def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("dir", nargs="?", help="group directory (with group.json)")
    ap.add_argument("--member", help="member or context function to score")
    ap.add_argument("--axes", default=",".join(ALL_AXES))
    ap.add_argument("--focus", default=None, help="comma list of functions to vary (default: member + callees)")
    ap.add_argument("--jobs", type=int, default=None)
    ap.add_argument("--combine", type=int, default=8, help="combine the best N single changes (0 = off)")
    ap.add_argument("--exhaust", action="store_true",
                    help="stage 2 = full factorial over every (axis, focus) group (sampled above --max-combined)")
    ap.add_argument("--patches", default=None, help="JSON list of {name, find, replace, file?, count?} source patches")
    ap.add_argument("--max-combined", type=int, default=120)
    ap.add_argument("--keep-limit", type=int, default=30)
    ap.add_argument("--top", type=int, default=25)
    ap.add_argument("--all", action="store_true", help="print every variant")
    ap.add_argument("--max-rules", type=int, default=40)
    ap.add_argument("--json", action="store_true")
    ap.add_argument("--no-cache", action="store_true")
    ap.add_argument("--timeout", type=float, default=120.0)
    ap.add_argument("--validate", action="store_true", help="reproduce this session's findings")
    a = ap.parse_args(argv)
    if a.validate:
        log = (lambda s: print(s, file=sys.stderr))
        res = validate(a.jobs, not a.no_cache, log)
        if a.json:
            print(json.dumps(res, indent=1))
        else:
            for k, v in res.items():
                print("## %s: %s" % (k, {True: "REPRODUCED", False: "NOT reproduced", None: "informational"}[v["ok"]]))
                for n in v["notes"]:
                    print("- " + n)
                for n in v.get("rules", []):
                    print("  rule: " + n)
        return 0
    if not a.dir or not a.member:
        ap.error("DIR and --member are required")
    axes = [x for x in a.axes.split(",") if x]
    bad = [x for x in axes if x not in ALL_AXES + EXTRA_AXES]
    if bad:
        ap.error("unknown axes: %s (have %s)" % (",".join(bad), ",".join(ALL_AXES)))
    patches = json.loads(Path(a.patches).read_text()) if a.patches else None
    res = probe(a.dir, a.member, axes, a.focus.split(",") if a.focus else None, a.jobs, a.combine,
                a.max_combined, not a.no_cache, a.timeout, a.keep_limit, a.exhaust, patches)
    if a.json:
        print(json.dumps(to_json(res)))
    else:
        sys.stdout.write(render_markdown(res, a.top, a.all, a.max_rules))
    return 1 if res.get("error") else 0


if __name__ == "__main__":
    sys.exit(main())
