"""One whole-program IDO -O3 unit of every locked game body (shadow gate).

    python3 -m tools.conveyor.pipeline.blob_unit manifest
    python3 -m tools.conveyor.pipeline.blob_unit check [--tag T] [--no-build]
    python3 -m tools.conveyor.pipeline.blob_unit score NAME [NAME...] [--with FILE.c ...]
    python3 -m tools.conveyor.pipeline.blob_unit layout

The game image is one whole-program `-O3` `uld -kp` unit. The splice lock
still holds the bodies as ~550 standalone objects and ~70 ad-hoc groups, each
with a private keep list and private copies of its neighbours. This module
builds ALL of them as one unit and compares every locked body with the retail
image words. It is a SHADOW build: it proves the bodies are consistent with
one another in one program; it does not feed the image or the ROM (blob_splice
/ blob_group / blob_rom still do).

What the unit needs that no single rule derives is a manifest
(`build/blob_unit.json`, generated, never hand-edited):

    files      one staged file per locked single and per group source, linked
               in DESCENDING address order (uld emits in reverse link order)
    functions  exactly one definition per name; every other definition of the
               name is stripped to a prototype in the staged copy
    keep       the `uld -kp` list; everything else is internal (takes part in
               interprocedural optimisation, like C `static`)
    blockers   tiny callees retail still calls by `jal`; the STAGED copy gets a
               dead `if (0) {...}` block so umerge does not inline them

Derivation: a function is internal when a locked group defines it and does
not keep it; everything else (every single, every new lock entry) is kept.
Hand decisions live in `src/blob/unit_overrides.json`, each with a reason.
The locked sources are never edited: stripping, renaming and blockers are
applied to staged copies only (`build/blob_unit/<tag>/stage/`).

Sources are preprocessed with IDO `cc -E` first (most carry #define/#if
lines; definitions cannot be found or stripped reliably otherwise). The
preprocessed text is cached by content under `build/blob_unit/cache/`.

Builder: stages run in `~/rush2049/scratch/frontier/unit/<tag>` (never the
splice pipeline's /tmp/blobsplice or /tmp/blobgroup, never ~/rush2049/repo).
"""
import argparse
import bisect
import fcntl
import hashlib
import json
import re
import shutil
import struct
import subprocess
import sys
import tempfile
import time
from pathlib import Path

from . import blob_layout, blob_splice

REPO = blob_splice.REPO
BUILDER = blob_splice.BUILDER
TOOLKIT = blob_splice.TOOLKIT
LOCKFILE = blob_splice.LOCKFILE
GROUP_DIR = blob_splice.SRC_DIR / "groups"
OVERRIDES = blob_splice.SRC_DIR / "unit_overrides.json"
WORK = REPO / "build" / "blob_unit"
MANIFEST = REPO / "build" / "blob_unit.json"
REMOTE_DIR = "rush2049/scratch/frontier/unit"      # relative to the builder's $HOME
DEFAULT_TAG = "gate"
FLAGS = "-g0 -O3 -mips2 -G 0 -non_shared"
MAX_JOBS = 4                                        # the builder is shared
KEYWORDS = {"if", "while", "for", "switch", "return", "sizeof", "do", "else"}
DEAD_SYMBOL = "__unit_dead"
DEAD = ("\n  if (0) {\n"
        + "".join(f"    {DEAD_SYMBOL}[{i}] = {i + 1};\n" for i in range(8)) + "  }\n")


OVERRIDE_KEYS = ("inline_blockers", "force_keep", "force_internal", "prefer_definition",
                 "align")
ALIGN = 32          # as1 pads some code to 32 bytes, counted from the start of .text


class UnitError(Exception):
    pass


# --- C source scanning (pure) -----------------------------------------------

_KR = re.compile(r"^[^=(]*\([\w\s,]*\)\s*[A-Za-z_]")
_COMMENT = re.compile(r"/\*.*?\*/", re.S)
# comments, literals and preprocessor lines are skipped as units; the rest are
# the only characters the scanner acts on
_TOKEN = re.compile(r"""/\*.*?(?:\*/|\Z)|//[^\n]*|"(?:\\.|[^"\\])*"|'(?:\\.|[^'\\])*'"""
                    r"""|^\#[^\n]*|[(){};]""", re.S | re.M)


def scan_defs(text):
    """Top-level function definitions of preprocessed C, in file order:
    [{name, static, head, open, close, kr}]. `head` is where the definition's
    declarator starts, `open`/`close` the offsets of the body's braces, `kr`
    the offset of the parameter list of a K&R definition (else None)."""
    out = []
    depth = pdepth = 0
    stmt = 0
    cur = None
    for tok in _TOKEN.finditer(text):
        c = tok.group()[0]
        i = tok.start()
        if c == "#":
            if depth == 0:
                stmt = tok.end()
        elif c == "(":
            pdepth += 1
        elif c == ")":
            pdepth -= 1
        elif c == "{":
            if depth == 0 and pdepth == 0:
                head = text[stmt:i]
                clean = _COMMENT.sub(" ", head)
                kr = bool(_KR.search(clean)) and clean.rstrip().endswith(";")
                if (kr or re.search(r"\)\s*$", clean)) and "=" not in clean:
                    m = None
                    for m in re.finditer(r"([A-Za-z_]\w*)\s*\(", clean):
                        if m.group(1) not in KEYWORDS:
                            break
                    if m:
                        cur = dict(name=m.group(1),
                                   static=bool(re.search(r"\bstatic\b", clean)),
                                   head=stmt, open=i, kr=None)
                        if kr:
                            k = re.search(re.escape(m.group(1)) + r"\s*\(", head)
                            cur["kr"] = stmt + k.end() - 1
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0 and cur is not None:
                cur["close"] = i
                out.append(cur)
                cur = None
                stmt = i + 1
        elif c == ";" and depth == 0 and pdepth == 0:
            # a K&R definition's parameter declarations sit between ) and {
            if not _KR.search(_COMMENT.sub(" ", text[stmt:i])):
                stmt = i + 1
    return out


def transform(text, keep_names, block=(), renames=None, defs=None):
    """The staged copy of one source: every global definition whose name is
    not in `keep_names` becomes a prototype, names in `renames` are renamed
    throughout, and kept functions named in `block` get a dead `if (0)` block
    before their closing brace (umerge sizes a callee before optimisation, so
    the block stops it being inlined without changing its code).

    Returns (text, [names defined by the staged copy, statics included])."""
    for old, new in sorted((renames or {}).items()):
        text = re.sub(r"\b%s\b" % re.escape(old), new, text)
    if defs is None or renames:
        defs = scan_defs(text)
    pieces, kept, pos = [], [], 0
    if any(d["name"] in block and (d["static"] or d["name"] in keep_names) for d in defs):
        pieces.append(f"extern int {DEAD_SYMBOL}[];\n")
    for d in defs:
        if d["static"] or d["name"] in keep_names:
            kept.append(d["name"])
            if d["name"] in block:
                pieces.append(text[pos:d["close"]])
                pieces.append(DEAD)
                pos = d["close"]
            continue
        if d["kr"] is not None:
            pieces.append(text[pos:d["kr"]])
            pieces.append("();")
        else:
            pieces.append(text[pos:d["open"]])
            pieces.append(";")
        pos = d["close"] + 1
    pieces.append(text[pos:])
    return "".join(pieces), kept


# --- manifest (pure) ----------------------------------------------------------

def load_overrides(path=OVERRIDES):
    """The tracked hand decisions. Every entry must carry a reason."""
    try:
        doc = json.loads(Path(path).read_text())
    except FileNotFoundError:
        doc = {}
    return validate_overrides(doc, path)


def validate_overrides(doc, where="overrides"):
    out = {}
    for key in OVERRIDE_KEYS:
        entries = doc.get(key, [])
        seen = set()
        for entry in entries:
            if not isinstance(entry, dict) or not entry.get("name"):
                raise UnitError(f"{where}: {key} entries need a name")
            if not str(entry.get("reason", "")).strip():
                raise UnitError(f"{where}: {key} {entry['name']} has no reason")
            if key == "prefer_definition" and not entry.get("file"):
                raise UnitError(f"{where}: prefer_definition {entry['name']} needs a file")
            if entry["name"] in seen:
                raise UnitError(f"{where}: {key} lists {entry['name']} twice")
            seen.add(entry["name"])
        out[key] = entries
    unknown = set(doc) - set(out) - {"comment"}
    if unknown:
        raise UnitError(f"{where}: unknown keys {sorted(unknown)}")
    both = ({e["name"] for e in out["force_keep"]}
            & {e["name"] for e in out["force_internal"]})
    if both:
        raise UnitError(f"{where}: {sorted(both)} both force_keep and force_internal")
    return out


def unit_files(lock, group_specs, extras=()):
    """([file records], {name: problem}) for a lock snapshot.

    One record per locked single (`s_<name>.c`), per source of every group
    that has a locked member (`g_<group>__<file>`) and per candidate source
    (`c_<stem>.c`, `score --with`). The ids are the staged file names."""
    files, problems, groups = [], {}, {}
    for name, entry in sorted(lock.items()):
        if entry.get("group"):
            groups.setdefault(entry["group"], []).append(name)
        else:
            files.append(dict(id=f"s_{name}.c", source=entry["source"], kind="single",
                              owner=name, xcpluscomm="-Xcpluscomm" in entry.get("flagset", "")))
    for group in sorted(groups):
        spec = group_specs.get(group)
        if spec is None:
            for name in groups[group]:
                problems[name] = f"group {group} has no group.json"
            continue
        for fname in spec["files"]:
            files.append(dict(id=f"g_{group}__{fname}",
                              source=f"src/blob/groups/{group}/{fname}", kind="group",
                              owner=group, xcpluscomm="-Xcpluscomm" in spec.get("flags", "")))
    for extra in extras:
        path = Path(extra)
        files.append(dict(id="c_" + re.sub(r"\W", "_", path.stem) + ".c", source=str(path),
                          kind="candidate", owner=None, xcpluscomm=None))
    ids = [f["id"] for f in files]
    if len(set(ids)) != len(ids):
        raise UnitError("two unit files share a staged name: "
                        + ", ".join(sorted({i for i in ids if ids.count(i) > 1})))
    return files, problems


PAD_SHAPE = {0: (0, 0), 1: (0, 3), 2: (1, 0), 3: (0, 1), 4: (2, 0), 5: (1, 1), 6: (0, 2),
             7: (2, 1)}       # words mod 8 -> (empty functions, storing functions)
PAD_SIZE = {k: 2 * e + 3 * st for k, (e, st) in PAD_SHAPE.items()}


def pad_source(name, words):
    """(C text, [function names]) of a pad of `words` instruction words modulo
    ALIGN/4: an empty kept function is 2 words, one storing zero is 3."""
    tag = re.sub(r"\W", "_", name)
    empty, store = PAD_SHAPE[words % (ALIGN // 4)]
    text, names = [f"extern int {DEAD_SYMBOL}[];\n"], []
    for k in range(empty):
        names.append(f"__unit_pad_{tag}_e{k}")
        text.append(f"void {names[-1]}(void)\n{{\n}}\n")
    for k in range(store):
        names.append(f"__unit_pad_{tag}_s{k}")
        text.append(f"void {names[-1]}(void)\n{{\n  {DEAD_SYMBOL}[{k}] = 0;\n}}\n")
    return "".join(text), names


def derive(lock, group_specs, files, defs, addresses, overrides=None,
           internal=(), keep=(), block=(), pads=None):
    """The manifest for a lock snapshot. Pure: no file or network access.

    lock         {name: lock entry}
    group_specs  {group: group.json} for the groups the lock names
    files        records from `unit_files`
    defs         {file id: [{name, static}, ...]} definitions of each
                 preprocessed source, in file order (`scan_defs`)
    addresses    {image function name: vaddr}
    overrides    `validate_overrides` output
    internal / keep / block   ad-hoc additions (`score` options)
    pads         {function: instruction words} of padding to emit before the
                 file that defines it (see `align` in the overrides)
    """
    overrides = overrides or validate_overrides({})
    by_id = {f["id"]: f for f in files}
    order = sorted(by_id)
    globals_of = {fid: [d["name"] for d in defs.get(fid, []) if not d["static"]]
                  for fid in order}
    defined_in = {}
    for fid in order:
        for name in globals_of[fid]:
            defined_in.setdefault(name, []).append(fid)

    # 1. one definition per function
    canon, why = {}, {}
    for fid in order:
        rec = by_id[fid]
        for name in globals_of[fid]:
            entry = lock.get(name)
            if rec["kind"] == "single" and rec["owner"] == name:
                canon[name], why[name] = fid, "locked single"
            elif (rec["kind"] == "group" and entry is not None
                  and entry.get("group") == rec["owner"]):
                canon[name], why[name] = fid, "locked group member"
    for fid in order:
        for name in globals_of[fid]:
            if name not in canon:
                canon[name], why[name] = fid, "context (first file by name)"
    for pref in overrides["prefer_definition"]:
        name = pref["name"]
        match = [f for f in order if by_id[f]["source"] == pref["file"]
                 and name in globals_of[f]]
        if match:
            canon[name], why[name] = match[0], "override: " + pref["reason"]
        elif name in canon:
            raise UnitError(f"prefer_definition {name}: {pref['file']} does not define it "
                            "(stale override?)")
    for fid in order:                       # a candidate under test wins outright
        if by_id[fid]["kind"] == "candidate":
            for name in globals_of[fid]:
                canon[name], why[name] = fid, "candidate source"

    # Functions with no image address (stand-in callers) that several files
    # define under one name are different procedures: rename them per file.
    renames = {}
    for name, fids in sorted(defined_in.items()):
        if len(fids) > 1 and name not in addresses and name not in lock:
            for k, fid in enumerate(fids):
                renames.setdefault(fid, {})[name] = f"{name}__u{k}"
            canon.pop(name)
            why.pop(name)
    for fid, mapping in renames.items():
        for new in mapping.values():
            canon[new], why[new] = fid, "stand-in, renamed per file"

    # 2. internal set: what a locked group defines and does not keep
    nonkeep = set()
    for group in sorted({f["owner"] for f in files if f["kind"] == "group"}):
        spec = group_specs[group]
        gdefs, gkeep = set(), set()
        for fid in order:
            if by_id[fid]["kind"] == "group" and by_id[fid]["owner"] == group:
                rn = renames.get(fid, {})
                gkeep |= {rn.get(k, k) for k in spec["keep"]}
                gdefs |= {rn.get(n, n) for n in globals_of[fid]}
        nonkeep |= gdefs - gkeep
    defined = set(canon)
    forced_keep = {e["name"] for e in overrides["force_keep"]} | set(keep)
    forced_internal = {e["name"] for e in overrides["force_internal"]} | set(internal)
    keep_set = ((defined - nonkeep) | (forced_keep & defined)) - forced_internal
    blockers = ({e["name"] for e in overrides["inline_blockers"]} | set(block)) & defined

    # 3. link order: descending address of the lowest locked function each
    # staged file defines (uld emits in reverse link order, callees first)
    def first_address(fid):
        # only what the staged file still defines counts: a group's private
        # copy of a locked neighbour is stripped and must not place the file
        rn = renames.get(fid, {})
        names = [n for n in globals_of[fid] if canon.get(rn.get(n, n)) == fid]
        locked = [addresses[n] for n in names if n in lock and n in addresses]
        if not locked and by_id[fid]["kind"] == "candidate":
            locked = [addresses[n] for n in names if n in addresses]
        return min(locked or [0xFFFFFFFF])

    link = sorted(order, key=lambda fid: (-first_address(fid), fid))

    out_files = []
    for fid in link:
        rec = by_id[fid]
        rn = renames.get(fid, {})
        mine = sorted(n for n, f in canon.items() if f == fid)
        out_files.append(dict(
            id=fid, source=rec["source"], kind=rec["kind"], owner=rec["owner"],
            xcpluscomm=bool(rec["xcpluscomm"]), address=first_address(fid),
            defines=mine,
            stripped=sorted(n for n in globals_of[fid] if rn.get(n, n) not in mine),
            renames=rn, blocked=sorted(blockers & set(mine))))
    for name, words in sorted((pads or {}).items()):
        if name not in canon or not words % (ALIGN // 4):
            continue
        # uld emits in reverse link order: linked after its file, the pad is
        # emitted immediately before it
        at = next(i for i, rec in enumerate(out_files) if rec["id"] == canon[name])
        _, pad_names = pad_source(name, words)
        out_files.insert(at + 1, dict(
            id="p_" + re.sub(r"\W", "_", name) + ".c", source=None, kind="pad", owner=name,
            xcpluscomm=False, address=out_files[at]["address"], defines=pad_names,
            stripped=[], renames={}, blocked=[], pad_words=words % (ALIGN // 4)))
        keep_set |= set(pad_names)
        for pad_name in pad_names:
            canon[pad_name] = out_files[at + 1]["id"]
    defined = set(canon)
    duplicates = {name: dict(chosen=canon[name], reason=why[name],
                             stripped_from=[f for f in fids if f != canon[name]])
                  for name, fids in sorted(defined_in.items())
                  if len(fids) > 1 and name in canon}
    missing = {name: "no definition of it in its locked source"
               for name in sorted(lock) if name not in canon}
    return dict(
        version=1, flags=FLAGS,
        files=out_files,
        keep=sorted(keep_set), internal=sorted(defined - keep_set),
        blockers=sorted(blockers),
        align=sorted(e["name"] for e in overrides["align"] if e["name"] in canon),
        forced_keep=sorted(forced_keep & defined),
        forced_internal=sorted(forced_internal & defined),
        functions={n: canon[n] for n in sorted(canon)},
        members=sorted(n for n in lock if n in canon),
        duplicates=duplicates, problems=missing,
        counts=dict(files=len(out_files), locked=len(lock), defined=len(defined),
                    keep=len(keep_set), internal=len(defined - keep_set),
                    blockers=len(blockers), duplicates=len(duplicates),
                    duplicates_locked=sum(1 for n in duplicates if n in lock),
                    renamed=sum(len(v) for v in renames.values())),
        unused_overrides=sorted(
            {e["name"] for key in ("inline_blockers", "force_keep", "force_internal", "align")
             for e in overrides[key]} - defined))


def stage(manifest, texts, out_dir, defs=None):
    """Write the staged sources, keep list and build script. Files are only
    rewritten when their content changed (rsync then moves almost nothing)."""
    out_dir = Path(out_dir)
    out_dir.mkdir(parents=True, exist_ok=True)
    wanted = {}
    blockers = set(manifest["blockers"])
    for rec in manifest["files"]:
        if rec["kind"] == "pad":
            wanted[rec["id"]] = pad_source(rec["owner"], rec["pad_words"])[0]
            continue
        text, kept = transform(texts[rec["id"]], set(rec["defines"]), blockers,
                               rec["renames"], (defs or {}).get(rec["id"]))
        wanted[rec["id"]] = text
    wanted["keep.txt"] = "".join(f"{k}\n" for k in manifest["keep"])
    wanted["files.txt"] = "".join(
        f"{rec['id']}{' -Xcpluscomm' if rec['xcpluscomm'] else ''}\n"
        for rec in manifest["files"])
    wanted["units.txt"] = "".join(re.sub(r"\.c$", ".u", rec["id"]) + "\n"
                                  for rec in manifest["files"])
    wanted["build.sh"] = build_script()
    for stale in out_dir.iterdir():
        if stale.name not in wanted and stale.is_file():
            stale.unlink()
    for name, text in wanted.items():
        path = out_dir / name
        if not path.is_file() or path.read_text() != text:
            path.write_text(text)
    return out_dir


def build_script(toolkit=TOOLKIT, flags=FLAGS):
    """Shell run in the remote stage directory. The stage commands are those of
    `blob_group.builder_script` (`cc -j`, `uld -kp`, usplit, umerge, uopt,
    ugen, `as1 -r4300_mul`), with `umerge -v` so inlining is logged."""
    opt = "-O3"
    return f"""#!/bin/sh
# GENERATED by pipeline.blob_unit: whole-program shadow build. Do not edit.
T=$HOME/rush2049/cache/toolkits/{toolkit}/ido
J=${{JOBS:-{MAX_JOBS}}}
rm -f *.u unit.o cc.fail linked* split* merged* opt* gen*
now() {{ date +%s.%N; }}
s=$(now)
xargs -P "$J" -L 1 sh -c '"$0"/cc -j {flags} $2 "$1" >"$1.log" 2>&1 || echo "$1" >>cc.fail' "$T" <files.txt
if [ -s cc.fail ]; then
  echo "STAGEFAIL cc"
  for f in $(sort cc.fail); do echo "== $f"; tail -4 "$f.log"; done
  exit 1
fi
echo "TIME cc $s $(now)"
step() {{
  n=$1; shift; s=$(now)
  "$@" >"$n.log" 2>&1 || {{ echo "STAGEFAIL $n"; tail -20 "$n.log"; exit 1; }}
  echo "TIME $n $s $(now)"
}}
step uld $T/uld -L/usr/lib/mips2/nonshared -_SYSTYPE_SVR4 -mips2 -non_shared -g0 \\
  -no_AutoGnum -kp keep.txt $(cat units.txt) -ko linked
step usplit $T/usplit -mips2 -o split -t st linked
step umerge $T/umerge -v -Olimit 5000 -mips2 -EB -g0 {opt} split -o merged -t st
step uopt $T/uopt -G 0 -Olimit 5000 -mips2 -EB -g0 {opt} merged opt -t st optlog
step ugen $T/ugen -G 0 -mips2 -EB -g0 {opt} opt -o gen -t st -temp ugtmp
step as1 $T/as1 -elf -G 0 -p0 -mips2 -EB -g0 {opt} -r4300_mul -Olimit 5000 gen -o unit.o -t st
echo DONE
"""


# --- ELF object (pure) --------------------------------------------------------

R_MIPS_32, R_MIPS_26, R_MIPS_HI16, R_MIPS_LO16 = 2, 4, 5, 6
RELOC_NAMES = {2: "R_MIPS_32", 4: "R_MIPS_26", 5: "R_MIPS_HI16", 6: "R_MIPS_LO16"}
SHT_PROGBITS, SHT_SYMTAB, SHT_NOBITS, SHT_REL = 1, 2, 8, 9
STT_SECTION, STT_FUNC = 3, 2


class Obj:
    """A big-endian ELF32 relocatable, as the comparison needs it.

    sections  [{name, type, offset, size, link, info}]
    symbols   [{name, value, size, type, bind, shndx}]  (section symbols are
              named after their section)
    relocs    {section name: [(offset, type, symbol index)]} in table order
    """

    def __init__(self, sections, symbols, relocs, data):
        self.sections, self.symbols, self.relocs, self._data = sections, symbols, relocs, data
        self.index = {s["name"]: i for i, s in enumerate(sections)}

    @classmethod
    def parse(cls, blob):
        if blob[:6] != b"\x7fELF\x01\x02":
            raise UnitError("not a big-endian ELF32 object")
        shoff, = struct.unpack_from(">I", blob, 0x20)
        shentsize, shnum, shstrndx = struct.unpack_from(">HHH", blob, 0x2E)
        raw = [struct.unpack_from(">IIIIIIIIII", blob, shoff + i * shentsize)
               for i in range(shnum)]
        strtab = raw[shstrndx]

        def string(table, off):
            start = table[4] + off
            return blob[start:blob.index(b"\0", start)].decode("latin-1")

        sections = [dict(name=string(strtab, r[0]), type=r[1], offset=r[4], size=r[5],
                         link=r[6], info=r[7]) for r in raw]
        symbols, relocs, data = [], {}, {}
        for i, sec in enumerate(sections):
            if sec["type"] == SHT_PROGBITS:
                data[sec["name"]] = blob[sec["offset"]:sec["offset"] + sec["size"]]
        for sec, r in zip(sections, raw):
            if sec["type"] != SHT_SYMTAB:
                continue
            names = raw[sec["link"]]
            for off in range(sec["offset"], sec["offset"] + sec["size"], 16):
                name, value, size, info, _other, shndx = struct.unpack_from(">IIIBBH", blob, off)
                stype = info & 0xF
                label = string(names, name)
                if stype == STT_SECTION and not label and shndx < len(sections):
                    label = sections[shndx]["name"]
                symbols.append(dict(name=label, value=value, size=size, type=stype,
                                    bind=info >> 4, shndx=shndx))
        for sec in sections:
            if sec["type"] != SHT_REL:
                continue
            target = sections[sec["info"]]["name"]
            out = relocs.setdefault(target, [])
            for off in range(sec["offset"], sec["offset"] + sec["size"], 8):
                r_offset, r_info = struct.unpack_from(">II", blob, off)
                out.append((r_offset, r_info & 0xFF, r_info >> 8))
        return cls(sections, symbols, relocs, data)

    def data(self, name):
        return self._data.get(name, b"")

    def section_name(self, shndx):
        return self.sections[shndx]["name"] if 0 < shndx < len(self.sections) else None

    def functions(self, section=".text"):
        """{name: offset} of the function symbols defined in `section`."""
        ndx = self.index.get(section)
        return {s["name"]: s["value"] for s in self.symbols
                if s["type"] == STT_FUNC and s["shndx"] == ndx and s["name"]}


def _sext16(v):
    return v - 0x10000 if v & 0x8000 else v


STUB_WORDS = (0x03E00008, 0x00000000)       # jr ra; nop
NOTE_STUBTAIL = "stub tail"
NOTE_BSS = "own zero-initialised data (addresses consistent, nothing to compare)"


def compare_unit(obj, names, extents, extern, image, base, show=6):
    """Relocate each named function's slice of the unit to its image address
    and compare it with the retail words.

    obj      Obj
    names    functions to report
    extents  {image function: (vaddr, size)}
    extern   name -> image address or None (data symbols, functions outside
             the unit, address-named symbols)
    image    the retail image bytes; `base` its load address

    Returns {name: {status, words, differing, first, notes, error}}; status is
    "ok" or "fail". A body is "ok" only when every word equals the image AND
    every reference into the unit's own data sections is to bytes that equal
    the image at the address the retail words encode (zero-initialised data
    has no bytes; it is noted, and one offset must still mean one address). Any relocation the function cannot resolve is
    a failure, never a mask.
    """
    text = obj.data(".text")
    text_ndx = obj.index.get(".text")
    funcs = obj.functions()
    starts = sorted(set(funcs.values()))
    limit = len(text)
    placed = sorted((off, min(off + extents[n][1],
                              next((s for s in starts[bisect.bisect_right(starts, off):]), limit)),
                     extents[n][0]) for n, off in funcs.items() if n in extents)
    placed_lo = [p[0] for p in placed]

    def text_addr(off):
        k = bisect.bisect_right(placed_lo, off) - 1
        if k >= 0 and placed[k][0] <= off < placed[k][1]:
            return placed[k][2] + off - placed[k][0]
        return None

    def owner(off):
        k = bisect.bisect_right(starts, off) - 1
        return starts[k] if k >= 0 else None

    def word(buf, off):
        return struct.unpack_from(">I", buf, off)[0]

    def image_word(vaddr):
        off = vaddr - base
        if not 0 <= off <= len(image) - 4:
            return None
        return struct.unpack_from(">I", image, off)[0]

    def classify(sym):
        """('text'|'extern'|'own'|'undef', section or None)"""
        shndx = sym["shndx"]
        if shndx == text_ndx:
            return "text", ".text"
        section = obj.section_name(shndx)
        named = sym["type"] != STT_SECTION and sym["name"]
        if named and extern(sym["name"]) is not None:
            return "extern", section
        if section is None:
            # undefined, or a common block the unit itself allocates
            return ("undef" if shndx == 0 else "own"), (None if shndx == 0 else "COMMON:" + sym["name"])
        return "own", section

    # Pass 1: pair the relocations of every function (members or not), so the
    # unit-wide set of referenced own-data offsets is known.
    per_func = {}
    for offset, rtype, symndx in obj.relocs.get(".text", []):
        per_func.setdefault(owner(offset), []).append((offset, rtype, obj.symbols[symndx]))
    resolved = {}                     # function start -> [entry]
    referenced = {}                   # own section -> sorted offsets referenced
    for start, rels in per_func.items():
        pending, last, entries = {}, {}, []
        for offset, rtype, sym in rels:
            key = id(sym)
            if rtype == R_MIPS_26:
                entries.append(("J", offset, sym, (word(text, offset) & 0x03FFFFFF) << 2))
            elif rtype == R_MIPS_HI16:
                pending.setdefault(key, []).append(offset)
            elif rtype == R_MIPS_LO16:
                his = pending.pop(key, None)
                if his:
                    last[key] = his
                elif key in last:
                    his = None
                else:
                    entries.append(("ERR", offset, sym, "R_MIPS_LO16 without R_MIPS_HI16"))
                    continue
                hi_site = last[key][0]
                addend = ((word(text, hi_site) & 0xFFFF) << 16) + _sext16(word(text, offset) & 0xFFFF)
                entries.append(("HL", offset, sym, addend, his or [], last[key]))
                kind, section = classify(sym)
                if kind == "own":
                    referenced.setdefault(section, set()).add(
                        (sym["value"] if not section.startswith("COMMON:") else 0) + addend)
            else:
                entries.append(("ERR", offset, sym,
                                f"unsupported relocation type {RELOC_NAMES.get(rtype, rtype)}"))
        for key, his in pending.items():
            entries.append(("ERR", his[0], None, "unpaired R_MIPS_HI16"))
        resolved[start] = entries
    referenced = {s: sorted(v) for s, v in referenced.items()}

    # The unit's own data sections with their internal relocations applied
    # (a switch's jump table: R_MIPS_32 against .text). Words that cannot be
    # given an image address are remembered and never count as verified.
    section_bytes, unknown_words = {}, {}
    for section, rels in obj.relocs.items():
        if section == ".text" or not obj.data(section):
            continue
        buf = bytearray(obj.data(section))
        bad = set()
        for offset, rtype, symndx in rels:
            sym = obj.symbols[symndx]
            target = None
            if rtype == R_MIPS_32 and offset + 4 <= len(buf):
                addend = word(buf, offset)
                kind, _ = classify(sym)
                if kind == "text":
                    target = text_addr(sym["value"] + addend)
                elif kind == "extern":
                    target = extern(sym["name"]) + addend
                elif kind == "undef":
                    target = extern(sym["name"])
                    target = None if target is None else target + addend
            if target is None:
                bad.add(offset & ~3)
            else:
                struct.pack_into(">I", buf, offset, target & 0xFFFFFFFF)
        section_bytes[section], unknown_words[section] = bytes(buf), bad

    def own_bytes(section):
        if section not in section_bytes:
            section_bytes[section] = obj.data(section)
            unknown_words[section] = set()
        return section_bytes[section]

    def verify_own(section, ours, retail):
        """'ok' | 'bss' | error text, for one own-data reference: the bytes
        from the referenced offset up to the next offset any function of the
        unit references must equal the image at the address retail encodes."""
        ndx = obj.index.get(section)
        if ndx is None or obj.sections[ndx]["type"] == SHT_NOBITS:
            return "bss"
        if obj.sections[ndx]["type"] != SHT_PROGBITS:
            return f"reference into {section}, which is not a data section"
        data = own_bytes(section)
        refs = referenced.get(section, [])
        k = bisect.bisect_right(refs, ours)
        end = min(refs[k] if k < len(refs) else len(data), len(data))
        if not 0 <= ours < len(data):
            return f"{section}+0x{ours:x} is outside the section"
        lo = retail - base
        if any(o in unknown_words[section] for o in range(ours & ~3, end, 4)):
            return (f"{section}+0x{ours:x}: an address stored there has no image address "
                    "(jump table into a stand-in?)")
        if not (0 <= lo and lo + (end - ours) <= len(image)):
            return f"{section}+0x{ours:x}: retail words encode 0x{retail:08x}, outside the image"
        window = data[ours:end]
        if image[lo:lo + len(window)] == window:
            return "ok"
        # alignment padding after the object is the next unit's data in retail
        trimmed = window.rstrip(b"\0")
        used = max((len(trimmed) + 3) & ~3, 4)
        if used < len(window) and image[lo:lo + used] == window[:used]:
            return "ok"
        at = next(i for i in range(0, used, 4) if image[lo + i:lo + i + 4] != window[i:i + 4])
        return (f"own {section}+0x{ours + at:x} ({window[at:at + 4].hex()}) differs from the "
                f"image at 0x{retail + at:08x} ({image[lo + at:lo + at + 4].hex()})")

    results = {}
    own_map = {}                 # (section, offset) -> {retail address: member}
    for name in names:
        res = dict(status="fail", words=0, differing=0, first=[], notes=[], error=None,
                   own_data=0)
        results[name] = res
        if name not in extents:
            res["error"] = "no extent in the layout"
            continue
        vaddr, size = extents[name]
        res["words"] = size // 4
        if name not in funcs:
            res["error"] = ("absent from the unit object (not defined, or an internal "
                            "procedure that was inlined everywhere and deleted)")
            continue
        off = funcs[name]
        k = bisect.bisect_right(starts, off)
        end = starts[k] if k < len(starts) else limit
        buf = bytearray(text[off:min(off + size, end)])
        errors, notes = [], set()

        def put(site, value, mask):
            o = site - off
            if 0 <= o <= len(buf) - 4:
                struct.pack_into(">I", buf, o, (word(buf, o) & ~mask & 0xFFFFFFFF) | (value & mask))

        for entry in resolved.get(off, []):
            site = entry[1]
            if not off <= site < off + len(buf):
                continue
            if entry[0] == "ERR":
                errors.append(f"+0x{site - off:x}: {entry[3]}")
                continue
            sym, addend = entry[2], entry[3]
            kind, section = classify(sym)
            label = sym["name"] or "?"
            if kind == "text":
                target = text_addr(sym["value"] + addend)
                if target is None:
                    who = next((n for n, o in funcs.items()
                                if o == owner(sym["value"] + addend)), label)
                    errors.append(f"+0x{site - off:x}: refers to {who}, which has no image "
                                  "address (a stand-in or deleted context)")
                    continue
            elif kind == "extern":
                target = extern(sym["name"]) + addend
            elif kind == "undef":
                target = extern(sym["name"])
                if target is None:
                    errors.append(f"+0x{site - off:x}: unresolved symbol {label}")
                    continue
                target += addend
            else:                                           # the unit's own data
                if entry[0] == "J":
                    errors.append(f"+0x{site - off:x}: call into {section}")
                    continue
                hi_site = entry[5][0]
                if not off <= hi_site < off + len(buf):
                    errors.append(f"+0x{site - off:x}: HI16 outside the function")
                    continue
                img_hi = image_word(vaddr + hi_site - off)
                img_lo = image_word(vaddr + site - off)
                target = ((img_hi & 0xFFFF) << 16) + _sext16(img_lo & 0xFFFF)
                ours = (0 if section.startswith("COMMON:") else sym["value"]) + addend
                verdict = verify_own(section, ours, target)
                own_map.setdefault((section, ours), {}).setdefault(target, name)
                res["own_data"] += verdict == "ok"
                if verdict == "bss":
                    notes.add(NOTE_BSS)
                elif verdict != "ok":
                    errors.append(f"+0x{site - off:x}: {verdict}")
            if entry[0] == "J":
                put(site, target >> 2, 0x03FFFFFF)
            else:
                for hi in entry[4]:
                    put(hi, (target + 0x8000) >> 16, 0xFFFF)
                put(site, target, 0xFFFF)

        lo = vaddr - base
        want = image[lo:lo + size]
        diffs = [i for i in range(0, min(len(buf), size), 4) if buf[i:i + 4] != want[i:i + 4]]
        res["differing"] = len(diffs)
        res["first"] = [(i // 4, word(want, i), word(buf, i)) for i in diffs[:show]]
        if len(buf) < size:
            errors.insert(0, f"compiled body is {len(buf) // 4} words, target {size // 4}")
        else:
            tail = [word(text, o) for o in range(off + size, end, 4)]
            if any(tail):
                if all(w in STUB_WORDS for w in tail):
                    # symbol-less `jr ra; nop` bodies of deleted procedures
                    same = image[lo + size:lo + size + 4 * len(tail)] == text[off + size:end]
                    notes.add(f"{NOTE_STUBTAIL}: {len(tail)} words of deleted-procedure stubs "
                              f"follow ({'as' if same else 'NOT as'} in the image)")
                else:
                    errors.insert(0, f"compiled body is {(end - off) // 4} words, "
                                     f"target {size // 4}")
        if diffs:
            errors.insert(0, f"{len(diffs)} of {size // 4} words differ")
        res["notes"] = sorted(notes)
        res["offset"] = off
        if errors:
            res["error"] = "; ".join(errors[:4]) + (f" (+{len(errors) - 4} more)" if len(errors) > 4 else "")
        else:
            res["status"] = "ok"
    # One object offset must mean one image address across the whole unit.
    for (section, ours), found in sorted(own_map.items()):
        if len(found) > 1:
            where = ", ".join(f"0x{a:08x} ({n})" for a, n in sorted(found.items()))
            for member in found.values():
                res = results[member]
                res["status"] = "fail"
                extra = f"{section}+0x{ours:x} is placed at different image addresses: {where}"
                res["error"] = extra if not res["error"] else res["error"] + "; " + extra
    return results


def emission(obj, extents):
    """How the unit object's `.text` layout relates to the image layout."""
    funcs = obj.functions()
    order = sorted(funcs.items(), key=lambda kv: kv[1])
    text = obj.data(".text")
    known = [(n, o) for n, o in order if n in extents]
    addrs = [extents[n][0] for n, _ in known]
    tails = []
    for a in addrs:
        k = bisect.bisect_left(tails, a)
        if k == len(tails):
            tails.append(a)
        else:
            tails[k] = a
    by_addr = sorted(extents.items(), key=lambda kv: kv[1][0])
    successor = {a[0]: b[0] for a, b in zip(by_addr, by_addr[1:])
                 if a[1][0] + a[1][1] == b[1][0]}
    in_unit = {n for n, _ in known}
    pairs = [(a, b) for a, b in successor.items() if a in in_unit and b in in_unit]
    exact = sum(1 for a, b in pairs if funcs[b] - funcs[a] == extents[a][1])
    following = sum(1 for a, b in pairs if funcs[b] > funcs[a])
    starts = [o for _, o in order] + [len(text)]
    gaps = 0
    for (name, off), nxt in zip(order, starts[1:]):
        if name in extents and nxt - off != extents[name][1]:
            gaps += 1
    return dict(functions=len(order), with_image_address=len(known),
                text_bytes=len(text),
                adjacent_inversions=sum(1 for a, b in zip(addrs, addrs[1:]) if b < a),
                longest_increasing_run=len(tails),
                image_adjacent_pairs=len(pairs), pairs_adjacent_in_unit=exact,
                pairs_in_image_order=following,
                slices_not_extent_sized=gaps)


# --- I/O: lock, sources, builder ---------------------------------------------

def _ssh(builder, command, **kwargs):
    return subprocess.run(["ssh", "-o", "BatchMode=yes", builder, command],
                          capture_output=True, text=True, **kwargs)


def read_inputs(lockfile=LOCKFILE, extras=(), repo=REPO):
    """(lock snapshot, group specs, file records, problems). The lock is read
    once: another process may be splicing while this runs."""
    lock = blob_splice.load_lock(lockfile)
    specs = {}
    for group in sorted({e["group"] for e in lock.values() if e.get("group")}):
        try:
            spec = json.loads((Path(repo) / "src" / "blob" / "groups" / group
                               / "group.json").read_text())
            if all(k in spec for k in ("files", "keep")):
                specs[group] = spec
        except (OSError, ValueError):
            pass
    files, problems = unit_files(lock, specs, extras)
    present = []
    for rec in files:
        path = Path(rec["source"])
        path = path if path.is_absolute() else Path(repo) / path
        try:
            rec["raw"] = path.read_bytes()
        except OSError:
            if rec["kind"] == "candidate":
                raise UnitError(f"cannot read {path}")
            for name, entry in lock.items():
                if (rec["kind"] == "single" and name == rec["owner"]) or (
                        rec["kind"] == "group" and entry.get("group") == rec["owner"]):
                    problems[name] = f"source missing ({rec['source']})"
            continue
        if rec["xcpluscomm"] is None:       # a candidate: flags from its first line
            rec["xcpluscomm"] = b"-Xcpluscomm" in rec["raw"].split(b"\n", 1)[0]
        present.append(rec)
    return lock, specs, present, problems


def preprocess(files, builder=BUILDER, remote=None, cache=None, toolkit=TOOLKIT):
    """{file id: IDO `cc -E` text}; cached by content, one builder round trip
    for everything not cached."""
    cache = Path(cache or WORK / "cache")
    cache.mkdir(parents=True, exist_ok=True)
    remote = remote or f"{REMOTE_DIR}/{DEFAULT_TAG}"
    keys, todo = {}, {}
    for rec in files:
        flag = "-Xcpluscomm" if rec["xcpluscomm"] else ""
        key = hashlib.sha256(rec["raw"] + b"|E|" + flag.encode() + toolkit.encode()).hexdigest()[:40]
        keys[rec["id"]] = key
        if not (cache / f"{key}.i").is_file():
            todo[key] = (rec, flag)
    if todo:
        send = Path(tempfile.mkdtemp(prefix="send-", dir=cache))
        for key, (rec, flag) in todo.items():
            (send / f"{key}.c").write_bytes(rec["raw"])
        (send / "list.txt").write_text("".join(f"{k}.c {flag}".rstrip() + "\n" for k, (_, flag) in todo.items()))
        pp = f"{remote}/pp"
        proc = _ssh(builder, f"rm -rf {pp} && mkdir -p {pp}")
        if proc.returncode:
            raise UnitError("builder unreachable: " + proc.stderr.strip()[:300])
        subprocess.run(["rsync", "-a", f"{send}/", f"{builder}:{pp}/"], check=True,
                       capture_output=True)
        script = (f"T=$HOME/rush2049/cache/toolkits/{toolkit}/ido; cd {pp} && "
                  f"xargs -P {MAX_JOBS} -L 1 sh -c '\"$0\"/cc -E $2 \"$1\" >\"${{1%.c}}.i\" "
                  f"2>\"$1.err\" || echo \"FAIL $1\"' \"$T\" <list.txt; "
                  f"tar czf out.tgz *.i; echo DONE")
        proc = _ssh(builder, script)
        failed = [line.split()[1][:-2] for line in proc.stdout.splitlines()
                  if line.startswith("FAIL ")]
        if "DONE" not in proc.stdout:
            raise UnitError("cc -E round trip failed: " + (proc.stderr or proc.stdout)[-300:])
        subprocess.run(["rsync", "-a", f"{builder}:{pp}/out.tgz", str(send / "out.tgz")],
                       check=True, capture_output=True)
        subprocess.run(["tar", "xzf", str(send / "out.tgz"), "-C", str(send)], check=True,
                       capture_output=True)
        for key, (rec, _) in todo.items():
            out = send / f"{key}.i"
            if key in failed or not out.is_file() or not out.read_text(errors="replace").strip():
                raise UnitError(f"cc -E failed for {rec['source']}")
            shutil.move(str(out), str(cache / f"{key}.i"))
        shutil.rmtree(send)
    return {rec["id"]: (cache / f"{keys[rec['id']]}.i").read_text(errors="replace")
            for rec in files}


def scan_cached(text, cache=None):
    """`scan_defs`, remembered by content (every source carries the same
    ~100 KB prelude; scanning 600 of them in Python takes seconds)."""
    cache = Path(cache or WORK / "cache")
    path = cache / (hashlib.sha1(text.encode("utf-8", "replace")).hexdigest() + ".defs.json")
    try:
        return json.loads(path.read_text())
    except (OSError, ValueError):
        defs = scan_defs(text)
        cache.mkdir(parents=True, exist_ok=True)
        path.write_text(json.dumps(defs))
        return defs


def image_extents(document):
    return {e["target_id"]: (e["vaddr"], e["size"]) for region in document["regions"]
            for e in region["entries"] if e["kind"] == "function"}


def load_image(document):
    path = Path(document["image"]["path"])
    path = path if path.is_absolute() else REPO / path
    return path.read_bytes(), int(document["image"]["base"], 16)


def make_extern(document):
    table = blob_splice.image_symbols(document)

    def extern(name):
        if name in table:
            return table[name]
        return blob_splice.address_named(name)
    return extern


class Inputs:
    """Everything the manifest is derived from, read once (the lock is a
    snapshot: another process may be splicing while this runs)."""

    def __init__(self, extras=(), builder=BUILDER, remote=None, lockfile=LOCKFILE,
                 document=None):
        self.document = document or blob_layout.load()
        self.lock, self.specs, self.files, self.problems = read_inputs(lockfile, extras)
        self.texts = preprocess(self.files, builder, remote)
        self.defs = {fid: scan_cached(text) for fid, text in self.texts.items()}
        self.extents = image_extents(self.document)
        self.addresses = {n: v for n, (v, _) in self.extents.items()}
        self.overrides = load_overrides()

    def manifest(self, internal=(), keep=(), block=(), pads=None):
        manifest = derive(self.lock, self.specs, self.files, self.defs, self.addresses,
                          self.overrides, internal=internal, keep=keep, block=block,
                          pads=pads)
        manifest["problems"] = {**manifest["problems"], **self.problems}
        manifest["lock_sha256"] = hashlib.sha256(
            json.dumps(self.lock, sort_keys=True).encode()).hexdigest()
        return manifest


def alignment_pads(funcs, extents, base, names, pads):
    """New {function: pad words} so every function in `names` starts at the
    same offset modulo ALIGN as in the image, given the offsets `funcs` of a
    unit built with `pads`. Equal to `pads` when everything is aligned."""
    out, shift = dict(pads), 0
    for name in sorted((n for n in names if n in funcs and n in extents),
                       key=lambda n: funcs[n]):
        old = pads.get(name, 0) % (ALIGN // 4)
        delta = ((extents[name][0] - base) - (funcs[name] + shift)) % ALIGN
        new = (old + delta // 4) % (ALIGN // 4)
        shift += (PAD_SIZE[new] - PAD_SIZE[old]) * 4
        out[name] = new
    return {n: w for n, w in out.items() if w}


def remote_build(stage_dir, out_dir, builder=BUILDER, remote=None, jobs=MAX_JOBS):
    """Sync the staged unit to the builder, run the stages, fetch the object.
    Returns (object path, {stage: seconds}, uld log)."""
    remote = remote or f"{REMOTE_DIR}/{DEFAULT_TAG}"
    jobs = max(1, min(int(jobs), MAX_JOBS))
    rdir = f"{remote}/stage"
    proc = _ssh(builder, f"mkdir -p {rdir}")
    if proc.returncode:
        raise UnitError("builder unreachable: " + proc.stderr.strip()[:300])
    sync = subprocess.run(
        ["rsync", "-a", "--delete", "--include=*.c", "--include=keep.txt",
         "--include=files.txt", "--include=units.txt", "--include=build.sh", "--exclude=*",
         f"{stage_dir}/", f"{builder}:{rdir}/"], capture_output=True, text=True)
    if sync.returncode:
        raise UnitError("rsync to the builder failed: " + sync.stderr.strip()[:300])
    proc = _ssh(builder, f"cd {rdir} && JOBS={jobs} flock ../.lock sh build.sh")
    times = {}
    for line in proc.stdout.splitlines():
        parts = line.split()
        if len(parts) == 4 and parts[0] == "TIME":
            times[parts[1]] = round(float(parts[3]) - float(parts[2]), 2)
    if "DONE" not in proc.stdout.split():
        detail = proc.stdout[proc.stdout.find("STAGEFAIL"):] if "STAGEFAIL" in proc.stdout \
            else (proc.stderr or proc.stdout)
        raise UnitError("unit build failed on the builder:\n" + detail.strip()[-3000:])
    out_dir = Path(out_dir)
    fetch = subprocess.run(
        ["rsync", "-a", f"{builder}:{rdir}/unit.o", f"{builder}:{rdir}/umerge.log",
         f"{builder}:{rdir}/uld.log", f"{out_dir}/"], capture_output=True, text=True)
    if fetch.returncode:
        raise UnitError("fetching the unit object failed: " + fetch.stderr.strip()[:300])
    return out_dir / "unit.o", times, (out_dir / "uld.log").read_text(errors="replace")


def inlined(umerge_log):
    """{caller: [callees umerge inlined into it]} from `umerge -v` output."""
    out, current = {}, None
    for line in umerge_log.splitlines():
        m = re.match(r"\s*inlining\s+(\w+)", line)
        if m and current:
            out.setdefault(current, []).append(m.group(1))
        elif line.strip() and not m:
            current = line.split()[0]
    return out


class Run:
    """One staged-and-built unit: the manifest, the object and its timings."""

    def __init__(self, tag=DEFAULT_TAG, builder=BUILDER, remote_dir=REMOTE_DIR):
        if not re.fullmatch(r"[\w.-]+", tag):
            raise UnitError("tag must be a plain name")
        self.tag, self.builder = tag, builder
        self.remote = f"{remote_dir.rstrip('/')}/{tag}"
        self.dir = WORK / tag
        self.times = {}

    def build(self, extras=(), internal=(), keep=(), block=(), jobs=MAX_JOBS, rebuild=True):
        self.dir.mkdir(parents=True, exist_ok=True)
        with open(self.dir / ".lock", "w") as handle:
            fcntl.flock(handle, fcntl.LOCK_EX)
            t = time.time()
            inputs = Inputs(extras, self.builder, self.remote)
            self.document, self.lock = inputs.document, inputs.lock
            base = int(self.document["image"]["base"], 16)
            pads_path = self.dir / "pads.json"
            pads = {}
            for seed in (pads_path, WORK / DEFAULT_TAG / "pads.json"):
                try:
                    pads = json.loads(seed.read_text())
                    break
                except (OSError, ValueError):
                    continue
            self.times["inputs"] = round(time.time() - t, 2)
            self.object = self.dir / "unit.o"
            self.builds = 0
            while True:
                self.manifest = inputs.manifest(internal, keep, block, pads)
                pads = {n: w for n, w in pads.items() if n in self.manifest["align"]}
                if not rebuild:
                    if not self.object.is_file():
                        raise UnitError(f"no unit object at {self.object}; "
                                        "run without --no-build")
                    self.obj = Obj.parse(self.object.read_bytes())
                    break
                t = time.time()
                stage(self.manifest, inputs.texts, self.dir / "stage", inputs.defs)
                self.times["stage"] = round(self.times.get("stage", 0) + time.time() - t, 2)
                t = time.time()
                self.object, stages, uld_log = remote_build(
                    self.dir / "stage", self.dir, self.builder, self.remote, jobs)
                self.builds += 1
                self.times["builder"] = round(self.times.get("builder", 0) + time.time() - t, 2)
                for k, v in stages.items():
                    self.times["ido_" + k] = round(self.times.get("ido_" + k, 0) + v, 2)
                dup = [line.strip() for line in uld_log.splitlines()
                       if "multiply defined" in line]
                if dup:
                    raise UnitError("uld saw a name defined twice (the manifest must leave one "
                                    "definition per function):\n  " + "\n  ".join(dup[:10]))
                self.obj = Obj.parse(self.object.read_bytes())
                # Position-dependent bodies (`align` overrides): pad until each
                # starts at its image offset modulo ALIGN, then remember the pads.
                wanted = alignment_pads(self.obj.functions(), inputs.extents, base,
                                        self.manifest["align"], pads)
                if wanted == pads or self.builds >= 4:
                    self.unaligned = sorted(set(wanted) ^ set(pads)
                                            | {n for n in wanted if wanted[n] != pads.get(n)})
                    break
                pads = wanted
            pads_path.write_text(json.dumps(pads, indent=1, sort_keys=True) + "\n")
            path = MANIFEST if self.tag == DEFAULT_TAG else self.dir / "manifest.json"
            path.parent.mkdir(parents=True, exist_ok=True)
            path.write_text(json.dumps(self.manifest, indent=1) + "\n")
            self.manifest_path = path
        return self

    def compare(self, names):
        t = time.time()
        image, base = load_image(self.document)
        results = compare_unit(self.obj, names, image_extents(self.document),
                               make_extern(self.document), image, base)
        for name, why in self.manifest["problems"].items():
            if name in results and results[name]["status"] != "ok":
                results[name]["error"] = why
        self.times["compare"] = round(time.time() - t, 2)
        return results

    def inlined(self):
        try:
            return inlined((self.dir / "umerge.log").read_text(errors="replace"))
        except OSError:
            return {}


# --- commands ---------------------------------------------------------------

def _describe(name, res, run, verbose=True):
    lines = [f"  FAIL {name}: {res['error']}"]
    if not verbose:
        return lines
    fid = run.manifest["functions"].get(name)
    kept = "kept" if name in run.manifest["keep"] else "internal"
    if fid:
        lines.append(f"       defined by {fid} ({kept})")
    for index, want, got in res["first"]:
        lines.append(f"       +0x{index * 4:03x}  image {want:08x}  unit {got:08x}")
    callees = run.inlined().get(name)
    if callees:
        lines.append("       umerge inlined into it: " + ", ".join(sorted(set(callees))))
    return lines


def _summary(results):
    ok = [r for r in results.values() if r["status"] == "ok"]
    noted = lambda key: sum(1 for r in ok if any(n.startswith(key) for n in r["notes"]))
    plain = sum(1 for r in ok if not r["notes"])
    return ok, plain, dict(stubtail=noted(NOTE_STUBTAIL), bss=noted(NOTE_BSS),
                           own=sum(1 for r in ok if r["own_data"]),
                           own_sites=sum(r["own_data"] for r in ok))


def cmd_manifest(args):
    remote = f"{args.remote_dir.rstrip('/')}/{args.tag}"
    try:
        pads = json.loads((WORK / args.tag / "pads.json").read_text())
    except (OSError, ValueError):
        pads = {}
    manifest = Inputs(builder=args.builder, remote=remote).manifest(pads=pads)
    MANIFEST.parent.mkdir(parents=True, exist_ok=True)
    MANIFEST.write_text(json.dumps(manifest, indent=1) + "\n")
    c = manifest["counts"]
    print(f"{MANIFEST.relative_to(REPO)}: {c['files']} files, {c['locked']} locked bodies, "
          f"{c['defined']} functions ({c['keep']} kept, {c['internal']} internal), "
          f"{c['blockers']} inline blockers, {c['duplicates']} names defined more than once "
          f"({c['duplicates_locked']} locked), {c['renamed']} stand-ins renamed")
    for name, why in sorted(manifest["problems"].items()):
        print(f"  problem {name}: {why}")
    if manifest["unused_overrides"]:
        print("  overrides naming functions the unit does not define: "
              + ", ".join(manifest["unused_overrides"]))
    return 1 if manifest["problems"] else 0


def cmd_check(args):
    start = time.time()
    run = Run(args.tag, args.builder, args.remote_dir).build(jobs=args.jobs,
                                                             rebuild=not args.no_build)
    results = run.compare(sorted(run.lock))
    failed = {n: r for n, r in results.items() if r["status"] != "ok"}
    for name in sorted(failed):
        print("\n".join(_describe(name, failed[name], run)))
    ok, plain, notes = _summary(results)
    if args.verbose:
        for name, res in sorted(results.items()):
            for note in res["notes"]:
                print(f"  note {name}: {note}")
    (run.dir / "result.json").write_text(json.dumps(
        dict(lock_sha256=run.manifest["lock_sha256"], times=run.times, results=results),
        indent=1) + "\n")
    c = run.manifest["counts"]
    ido = sum(v for k, v in run.times.items() if k.startswith("ido_"))
    print(f"blob_unit: {len(results)} locked bodies, {len(ok)} equal to the image in one "
          f"{c['files']}-file unit, {len(failed)} differ "
          f"[{plain} plain; stub tail {notes['stubtail']}, own bss {notes['bss']}; "
          f"{notes['own_sites']} own .rodata/.data references in {notes['own']} bodies "
          f"checked against image bytes] "
          f"({c['keep']} kept, {c['internal']} internal; IDO {ido:.1f}s, "
          f"total {time.time() - start:.1f}s)")
    return 1 if failed else 0


def cmd_score(args):
    start = time.time()
    run = Run(args.tag, args.builder, args.remote_dir).build(
        extras=args.sources, internal=args.internal, keep=args.keep, block=args.block,
        jobs=args.jobs)
    results = run.compare(args.names)
    bad = 0
    for name in args.names:
        res = results[name]
        kept = "kept" if name in run.manifest["keep"] else "internal"
        where = run.manifest["functions"].get(name, "not defined in the unit")
        if res["status"] == "ok":
            notes = ("; " + "; ".join(res["notes"])) if res["notes"] else ""
            print(f"  EQUAL {name}: {res['words']} words ({kept}, {where}){notes}")
        else:
            bad += 1
            print("\n".join(_describe(name, res, run)))
    if args.neighbours:
        others = run.compare(sorted(run.lock))
        broken = sorted(n for n, r in others.items()
                        if r["status"] != "ok" and n not in args.names)
        print(f"  locked bodies that differ in this unit: {len(broken)}"
              + (": " + ", ".join(broken[:20]) if broken else ""))
        bad += len(broken)
    print(f"blob_unit score: {len(args.names) - sum(1 for n in args.names if results[n]['status'] != 'ok')}"
          f"/{len(args.names)} equal; object {run.object.relative_to(REPO)} "
          f"({time.time() - start:.1f}s)")
    return 1 if bad else 0


def cmd_layout(args):
    run = Run(args.tag, args.builder, args.remote_dir).build(rebuild=False)
    info = emission(run.obj, image_extents(run.document))
    for key, value in info.items():
        print(f"  {key}: {value}")
    for name in (".rodata", ".data", ".sdata", ".lit4", ".lit8", ".bss", ".sbss"):
        if name in run.obj.index:
            print(f"  section {name}: {run.obj.sections[run.obj.index[name]]['size']} bytes")
    return 0


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--tag", default=DEFAULT_TAG,
                        help="work directory name, local and on the builder (use your own "
                             "for `score` so runs do not collide)")
    parser.add_argument("--builder", default=BUILDER)
    parser.add_argument("--remote-dir", default=REMOTE_DIR,
                        help="builder directory, relative to its $HOME")
    parser.add_argument("--jobs", type=int, default=MAX_JOBS)
    sub = parser.add_subparsers(dest="command", required=True)
    sub.add_parser("manifest")
    p = sub.add_parser("check")
    p.add_argument("--no-build", action="store_true",
                   help="compare the object of the previous run again")
    p.add_argument("--verbose", action="store_true", help="also print per-function notes")
    p = sub.add_parser("score")
    p.add_argument("names", nargs="+")
    p.add_argument("--with", dest="sources", action="append", default=[], metavar="FILE.c",
                   help="candidate source to add to the unit (its definitions win)")
    p.add_argument("--internal", action="append", default=[], metavar="NAME",
                   help="make NAME internal in this run (not on the keep list)")
    p.add_argument("--keep", action="append", default=[], metavar="NAME")
    p.add_argument("--block", action="append", default=[], metavar="NAME",
                   help="give NAME an inline blocker in this run")
    p.add_argument("--neighbours", action="store_true",
                   help="also report locked bodies the candidates break")
    sub.add_parser("layout")
    args = parser.parse_args(argv)
    try:
        return dict(manifest=cmd_manifest, check=cmd_check, score=cmd_score,
                    layout=cmd_layout)[args.command](args)
    except UnitError as exc:
        print(f"blob_unit: {exc}")
        return 2


if __name__ == "__main__":
    sys.exit(main())
