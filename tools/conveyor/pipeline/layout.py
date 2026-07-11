"""ROM layout map + ROM-aligned TU generation (004, promotion splicing).

`layout derive`   -> build/layout.us.json (deterministic), the function->TU->ROM
                     map for the static code range, from splat.us.yaml segment
                     boundaries + 003 index_asm_regions + symbol_addrs canonical
                     names + flag_registry pins.
`layout report`   -> human summary (clean vs refused segments).
`layout convert`  -> flip a splat asm subsegment to c, re-split, generate the
                     all-passthrough ROM-aligned TU (see contracts/layout-map.md).
`layout coverage` -> linked-C functions/bytes, promoted vs passthrough, from the
                     live TUs cross-checked against the map (FR-009).

Pi-local, stdlib only. Matching builds themselves run on the builder.
"""
import argparse
import hashlib
import json
import re
import subprocess
import sys
from pathlib import Path

from . import targets as targetsmod

REPO = targetsmod.REPO
SPLAT_YAML = REPO / "splat.us.yaml"
SYMBOL_ADDRS = REPO / "symbol_addrs.us.txt"
LAYOUT_JSON = REPO / "build" / "layout.us.json"
LOCKFILE = REPO / "matched.lock.json"
ROM_SRC_DIR = REPO / "src" / "rom"
NONMATCHINGS = REPO / "asm" / "us" / "nonmatchings"

# vram = rom_offset + DELTA (code segment start 0x1050 -> vram 0x80000450).
VRAM_DELTA = 0x7FFFF400
# The compressed game code (dynamic population) lives past this vram; out of
# scope for promotion (blob rebuild is a separate feature, FR-011).
DYNAMIC_VRAM = targetsmod.GAME_CODE_BASE  # 0x80086A50

_SUBSEG_RE = re.compile(
    r"^(?P<indent>\s*)-\s*\[\s*(?P<off>0x[0-9A-Fa-f]+)\s*,\s*"
    r"(?P<type>[A-Za-z0-9_]+)\s*(?:,\s*(?P<name>[^\]]+?)\s*)?\]\s*$"
)
_SEG_START_RE = re.compile(r"^\s*start:\s*(0x[0-9A-Fa-f]+)")
_SYM_RE = re.compile(
    r"^(?P<name>[A-Za-z_]\w*)\s*=\s*(?P<addr>0x[0-9A-Fa-f]+)\s*;.*?type:(?P<type>\w+)"
)
_FUNC_AUTONAME_RE = re.compile(r"^func_[0-9A-Fa-f]+$")

CODE_TYPES = ("asm", "hasm")


# --- inputs -----------------------------------------------------------------

def parse_subsegments(splat_path=None):
    """Ordered code subsegments of the one `type: code` segment, plus the rom
    offset that ends the segment. Returns (subsegs, code_end) where each subseg
    is {off, type, name, lineno, indent, raw}. Line-based (not a yaml round
    trip) so `convert` can do a minimal single-line edit."""
    splat_path = splat_path or SPLAT_YAML
    lines = Path(splat_path).read_text().splitlines()
    subsegs = []
    in_code = False
    seg_indent = None
    code_end = None
    for i, line in enumerate(lines):
        stripped = line.strip()
        if stripped == "type: code":
            in_code = True
            continue
        if not in_code:
            continue
        m = _SUBSEG_RE.match(line)
        if m:
            subsegs.append({
                "off": int(m.group("off"), 16),
                "type": m.group("type"),
                "name": (m.group("name") or "").strip() or None,
                "lineno": i,
                "indent": m.group("indent"),
                "raw": line,
            })
    # code_end = the start of the first top-level segment after the code block.
    code_end = _next_top_segment_start(lines, subsegs)
    return subsegs, code_end


def _next_top_segment_start(lines, subsegs):
    """The rom offset where the code segment ends: the start of the next
    top-level segment (a `- type: ...`/`start:` block or a bare `- [0xADDR]`)."""
    if not subsegs:
        return None
    last_line = subsegs[-1]["lineno"]
    for line in lines[last_line + 1:]:
        s = line.strip()
        m = re.match(r"^-\s*\[\s*(0x[0-9A-Fa-f]+)", s)
        if m:
            return int(m.group(1), 16)
        m = _SEG_START_RE.match(line)
        if m:
            return int(m.group(1), 16)
    return None


def parse_symbol_addrs(path=None):
    """{vaddr: (name, type)} from symbol_addrs.us.txt."""
    path = path or SYMBOL_ADDRS
    out = {}
    for line in Path(path).read_text().splitlines():
        m = _SYM_RE.match(line.strip())
        if m:
            out[int(m.group("addr"), 16)] = (m.group("name"), m.group("type"))
    return out


def collect_regions():
    """{vaddr: Region} across asm/us/*.s and any converted segment's
    nonmatchings export, so derivation works before and after conversion."""
    regions = dict(targetsmod.index_asm_regions())
    if NONMATCHINGS.is_dir():
        for sub in sorted(p for p in NONMATCHINGS.rglob("*") if p.is_dir()):
            if any(sub.glob("*.s")):
                regions.update(targetsmod.index_asm_regions(sub))
    return regions


def load_flagsets():
    """{function_name: flagset} evidence, from matched.lock.json (per-function
    verified flags — the concrete authority for the acceptance batch)."""
    out = {}
    if LOCKFILE.is_file():
        for spec, e in json.loads(LOCKFILE.read_text()).items():
            tid = e.get("target_id") or spec.rpartition(":")[2]
            if e.get("flagset"):
                out[tid] = e["flagset"]
    return out


# --- derivation -------------------------------------------------------------

def _rom_tu_name(off):
    return f"rom/lib_{off:x}"


def _nonzero_gap(vaddr, nbytes):
    """None if the [vaddr, vaddr+nbytes) ROM bytes are all zero (nop alignment
    padding); otherwise the vaddr of the first non-zero word (real data)."""
    if nbytes <= 0:
        return None
    try:
        words = targetsmod.function_words(vaddr, nbytes)
    except ValueError:
        return vaddr  # outside known image: treat as data, refuse
    for i, w in enumerate(words):
        if w != "00000000":
            return vaddr + i * 4
    return None


def _segment_flagset(func_names, flag_by_fn):
    """The single flagset shared by the segment's functions that carry
    evidence, or None (no evidence, or a conflict — FR-005 remedy is a sweep)."""
    seen = {flag_by_fn[n] for n in func_names if n in flag_by_fn}
    return seen.pop() if len(seen) == 1 else None


def derive(splat_path=None):
    """Build the layout map dict (deterministic)."""
    subsegs, code_end = parse_subsegments(splat_path)
    symbols = parse_symbol_addrs()
    regions = collect_regions()
    flag_by_fn = load_flagsets()

    segments = []
    for idx, sub in enumerate(subsegs):
        if sub["type"] not in CODE_TYPES + ("c",):
            continue
        rom_start = sub["off"]
        rom_end = subsegs[idx + 1]["off"] if idx + 1 < len(subsegs) else code_end
        vram_start = rom_start + VRAM_DELTA
        vram_end = (rom_end + VRAM_DELTA) if rom_end is not None else None
        converted = sub["type"] == "c"

        seg = _derive_segment(sub, rom_start, vram_start, vram_end,
                              converted, symbols, regions, flag_by_fn)
        segments.append(seg)

    # Synthetic dynamic-population entry (FR-011): unpromotable in V1.
    segments.append({
        "yaml_name": "dynamic", "rom_tu": None,
        "vram_start": hex(DYNAMIC_VRAM), "size": None, "converted": False,
        "flagset": None, "functions": [],
        "refusal": "dynamic-population (blob rebuild feature)",
    })
    return {"segments": segments}


def _derive_segment(sub, rom_start, vram_start, vram_end, converted,
                    symbols, regions, flag_by_fn):
    yaml_name = hex(rom_start)
    rom_tu = sub["name"] if converted and sub["name"] else _rom_tu_name(rom_start)
    seg = {
        "yaml_name": yaml_name, "rom_tu": rom_tu,
        "vram_start": hex(vram_start),
        "size": (vram_end - vram_start) if vram_end is not None else None,
        "converted": converted, "flagset": None, "functions": [], "refusal": None,
    }
    if vram_end is None:
        seg["refusal"] = f"unbounded@{yaml_name}: no following segment"
        return seg

    # Regions (instruction bodies) of this segment, in address order. Each
    # function's tiled size runs to the next function (or segment end) so
    # inter-function alignment padding rides with the preceding function; a
    # gap is real data (jumptable/rodata) only if its bytes are non-zero
    # (D4 — those segments stay unconverted). Leading data before the first
    # region, or a non-nop gap, refuses.
    in_range = sorted(v for v in regions if vram_start <= v < vram_end)
    funcs, names, refusal = [], [], None
    if not in_range or in_range[0] != vram_start:
        first = hex(in_range[0]) if in_range else hex(vram_end)
        seg["refusal"] = (f"gap@{hex(vram_start)}: {(in_range[0] if in_range else vram_end) - vram_start}"
                          f" bytes before first region ({first})")
        return seg
    for i, v in enumerate(in_range):
        instr_size = len(regions[v].words) * 4
        next_v = in_range[i + 1] if i + 1 < len(in_range) else vram_end
        tiled = next_v - v
        if tiled < instr_size:
            refusal = f"overrun@{hex(v)}: region exceeds slot by {instr_size - tiled}"
            break
        pad = _nonzero_gap(v + instr_size, tiled - instr_size)
        if pad is not None:
            refusal = f"data@{hex(pad)}: non-zero bytes not in any region"
            break
        sym = symbols.get(v)
        name = sym[0] if sym and sym[1] == "func" else None
        if name is None or _FUNC_AUTONAME_RE.match(name or ""):
            refusal = f"unnamed@{hex(v)}"
            break
        funcs.append({"name": name, "vaddr": hex(v), "size": tiled,
                      "state": "passthrough"})
        names.append(name)

    if refusal:
        seg["refusal"] = refusal
        return seg
    seg["functions"] = funcs
    seg["flagset"] = _segment_flagset(names, flag_by_fn)
    if converted:
        _apply_live_states(seg)
    return seg


# --- map hash + TU generation ----------------------------------------------

def structural_map(mapping):
    """The promotion-invariant projection of the map (drops per-function live
    `state`) whose sha256 is the map hash embedded in TU headers — so a TU is
    tied to the map STRUCTURE, stable across promotions into it."""
    segs = []
    for s in mapping["segments"]:
        segs.append({
            "yaml_name": s["yaml_name"], "rom_tu": s["rom_tu"],
            "vram_start": s["vram_start"], "size": s["size"],
            "converted": s["converted"], "flagset": s["flagset"],
            "refusal": s["refusal"],
            "functions": [{"name": f["name"], "vaddr": f["vaddr"],
                           "size": f["size"]} for f in s["functions"]],
        })
    return {"segments": segs}


def map_hash(mapping):
    blob = json.dumps(structural_map(mapping), sort_keys=True,
                      separators=(",", ":")).encode()
    return hashlib.sha256(blob).hexdigest()


def _segment_by_name(mapping, yaml_name):
    for s in mapping["segments"]:
        if s["yaml_name"] == yaml_name:
            return s
    return None


def generate_tu(seg, mh):
    """The generated ROM-aligned TU text for a segment (all-passthrough slots
    unless a slot is already promoted). Deterministic given (seg, map hash)."""
    name = seg["rom_tu"]
    out = [
        f"/* GENERATED ROM-aligned TU — segment {seg['yaml_name']} ({name})",
        f" * layout map {mh}; regenerate via `pipeline.layout convert`.",
        " * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit",
        " * passthrough lines. */",
        '#include "rom_tu.h"',
        "",
    ]
    tu_base = name  # e.g. rom/lib_8800
    for f in seg["functions"]:
        if f.get("state") == "promoted" and f.get("body"):
            out.append(f["body"].rstrip("\n"))
        else:
            out.append(f'#pragma GLOBAL_ASM("asm/us/nonmatchings/{tu_base}/{f["name"]}.s")')
    return "\n".join(out) + "\n"


# --- live TU parsing (coverage / state) -------------------------------------

_PRAGMA_RE = re.compile(r'#pragma GLOBAL_ASM\("[^"]*/(?P<fn>[A-Za-z_]\w*)\.s"\)')
_PROMOTED_HDR_RE = re.compile(r"/\*\s*PROMOTED\b.*?—\s*(?P<fn>[A-Za-z_]\w*)")


def parse_tu_states(tu_path):
    """{function_name: 'passthrough'|'promoted'} from a live TU: a slot is
    passthrough iff its GLOBAL_ASM pragma is present, else promoted (its
    PROMOTED provenance header names it)."""
    states = {}
    if not Path(tu_path).is_file():
        return states
    for line in Path(tu_path).read_text().splitlines():
        m = _PRAGMA_RE.search(line)
        if m:
            states[m.group("fn")] = "passthrough"
            continue
        m = _PROMOTED_HDR_RE.search(line)
        if m:
            states[m.group("fn")] = "promoted"
    return states


def _apply_live_states(seg):
    tu_path = REPO / "src" / (seg["rom_tu"] + ".c")
    states = parse_tu_states(tu_path)
    for f in seg["functions"]:
        f["state"] = states.get(f["name"], f["state"])


def coverage(mapping=None):
    """Linked-C coverage: promoted functions/bytes over the static code range,
    derived from live TUs cross-checked against the map. Returns a dict."""
    mapping = mapping or derive()
    total_funcs = total_bytes = 0
    promoted_funcs = promoted_bytes = 0
    per_segment = []
    for s in mapping["segments"]:
        if s["refusal"] or not s["functions"]:
            continue
        pf = pb = 0
        for f in s["functions"]:
            total_funcs += 1
            total_bytes += f["size"]
            if f.get("state") == "promoted":
                pf += 1
                pb += f["size"]
        promoted_funcs += pf
        promoted_bytes += pb
        if s["converted"]:
            per_segment.append({"segment": s["yaml_name"], "rom_tu": s["rom_tu"],
                                "promoted": pf, "functions": len(s["functions"]),
                                "bytes": pb})
    return {
        "promoted_functions": promoted_funcs, "promoted_bytes": promoted_bytes,
        "static_functions": total_funcs, "static_bytes": total_bytes,
        "segments": per_segment,
    }


# --- commands ---------------------------------------------------------------

def cmd_derive(args):
    mapping = derive()
    LAYOUT_JSON.parent.mkdir(parents=True, exist_ok=True)
    LAYOUT_JSON.write_text(json.dumps(mapping, indent=2, sort_keys=True) + "\n")
    clean = sum(1 for s in mapping["segments"] if not s["refusal"])
    refused = len(mapping["segments"]) - clean
    print(f"layout: {len(mapping['segments'])} segments "
          f"({clean} derivable, {refused} refused) -> {LAYOUT_JSON}")
    print(f"map hash {map_hash(mapping)}")


def cmd_report(args):
    mapping = derive()
    for s in mapping["segments"]:
        if s["refusal"]:
            print(f"  REFUSE {s['yaml_name']:8} {s['refusal']}")
        else:
            conv = "C" if s["converted"] else "asm"
            fl = s["flagset"] or "(no flag pin)"
            print(f"  ok     {s['yaml_name']:8} {conv:3} {len(s['functions'])} fns"
                  f"  {s['size']} bytes  [{fl}]")
            if args.verbose:
                for f in s["functions"]:
                    print(f"           {f['state']:11} {f['vaddr']} {f['name']} "
                          f"({f['size']}b)")


def cmd_coverage(args):
    cov = coverage()
    pct = (100 * cov["promoted_bytes"] // cov["static_bytes"]
           if cov["static_bytes"] else 0)
    print(f"linked-C coverage: {cov['promoted_functions']}/{cov['static_functions']} "
          f"functions, {cov['promoted_bytes']}/{cov['static_bytes']} bytes ({pct}%)")
    for s in cov["segments"]:
        print(f"  {s['segment']:8} {s['rom_tu']:16} "
              f"{s['promoted']}/{s['functions']} promoted, {s['bytes']}b")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest="command", required=True)
    p = sub.add_parser("derive"); p.set_defaults(func=cmd_derive)
    p = sub.add_parser("report"); p.add_argument("--verbose", action="store_true")
    p.set_defaults(func=cmd_report)
    p = sub.add_parser("coverage"); p.set_defaults(func=cmd_coverage)
    _add_convert_parsers(sub)
    args = parser.parse_args()
    args.func(args)


def _add_convert_parsers(sub):
    p = sub.add_parser("convert")
    p.add_argument("segment")
    p.add_argument("--revert", action="store_true")
    p.set_defaults(func=_cmd_convert)


def _cmd_convert(args):
    """Convert a splat asm subsegment to a ROM-aligned generated C TU (or
    revert). Extraction is reproducible again (2026-07-10 remediation):
    `make extract` re-splits idempotently and the rebuilt ROM hash-matches."""
    mapping = derive()
    seg = _segment_by_name(mapping, args.segment)
    if seg is None:
        sys.exit(f"no such segment {args.segment!r} (see `layout report`)")
    if args.revert:
        return _do_revert(seg)
    return _do_convert(mapping, seg)


def _yaml_subseg_line(off, current):
    return f"      - [0x{off:X}, {current}]"


def _rewrite_yaml(off, old_entry, new_entry):
    path = REPO / "splat.us.yaml"
    text = path.read_text()
    old = _yaml_subseg_line(off, old_entry)
    new = _yaml_subseg_line(off, new_entry)
    if old not in text:
        sys.exit(f"splat.us.yaml: expected line {old!r} not found")
    path.write_text(text.replace(old, new, 1))


def _run_extract():
    proc = subprocess.run(["make", "extract"], cwd=REPO,
                          capture_output=True, text=True)
    if proc.returncode != 0:
        sys.exit("make extract failed:\n" + (proc.stderr or proc.stdout)[-2000:])


def _affected_paths(seg):
    off = int(seg["yaml_name"], 16)
    return [
        REPO / "splat.us.yaml",
        REPO / f"asm/us/{off:X}.s",
        REPO / "src" / f"{seg['rom_tu']}.c",
        REPO / "rush2049.us.ld",
    ]


def _dirty(paths):
    rels = [str(p.relative_to(REPO)) for p in paths]
    out = subprocess.run(["git", "status", "--porcelain", "--"] + rels,
                         cwd=REPO, capture_output=True, text=True).stdout
    return out.strip()


def _do_convert(mapping, seg):
    if seg["refusal"]:
        sys.exit(f"refusing: segment {seg['yaml_name']} is unconvertible: "
                 f"{seg['refusal']}")
    if seg["converted"]:
        sys.exit(f"refusing: {seg['yaml_name']} is already converted")
    dirty = _dirty(_affected_paths(seg))
    if dirty:
        sys.exit("refusing: working tree dirty under affected paths:\n" + dirty)

    off = int(seg["yaml_name"], 16)
    _rewrite_yaml(off, "asm", f"c, {seg['rom_tu']}")
    # The old whole-segment asm file is replaced by splat's per-function
    # nonmatchings output; leaving it would let the Makefile wildcard link a
    # stale duplicate object.
    stale = REPO / f"asm/us/{off:X}.s"
    if stale.is_file():
        stale.unlink()
    _run_extract()

    hdr = REPO / "src/rom/rom_tu.h"
    if not hdr.is_file():
        hdr.parent.mkdir(parents=True, exist_ok=True)
        hdr.write_text(
            "/* Common includes for ROM-aligned TUs (004). Passthrough slots\n"
            " * need nothing; promoted bodies add what they use here. */\n"
            '#include "types.h"\n')
    tu = REPO / "src" / f"{seg['rom_tu']}.c"
    tu.parent.mkdir(parents=True, exist_ok=True)
    tu.write_text(generate_tu(seg, map_hash(mapping)))
    nm = REPO / f"asm/us/nonmatchings/{seg['rom_tu']}"
    n_asm = len(list(nm.glob("*.s"))) if nm.is_dir() else 0
    print(f"converted {seg['yaml_name']} -> {tu.relative_to(REPO)} "
          f"({len(seg['functions'])} passthrough slots, {n_asm} nonmatchings files)")
    print("next: matching build + SHA-1 on the builder "
          "(make COMPILER=ido && make test)")


def _do_revert(seg):
    if not seg["converted"]:
        sys.exit(f"refusing: {seg['yaml_name']} is not converted")
    off = int(seg["yaml_name"], 16)
    _rewrite_yaml(off, f"c, {seg['rom_tu']}", "asm")
    tu = REPO / "src" / f"{seg['rom_tu']}.c"
    if tu.is_file():
        tu.unlink()
    nm = REPO / f"asm/us/nonmatchings/{seg['rom_tu']}"
    if nm.is_dir():
        for f in nm.glob("*.s"):
            f.unlink()
        nm.rmdir()
    _run_extract()
    print(f"reverted {seg['yaml_name']} to asm "
          "(whole-segment asm file regenerated by splat)")


if __name__ == "__main__":
    main()
