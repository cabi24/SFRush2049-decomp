#!/usr/bin/env python3
"""Summarise N64Recomp output against the generated symbol file (Phase 0).

    python3 tools/recomp/check_output.py [--recomp-dir build/recomp] [--json OUT]

Reads <recomp-dir>/rush2049.syms.toml and <recomp-dir>/RecompiledFuncs/*.c and
reports, per section: functions in the symbol file, functions emitted (a
`RECOMP_FUNC void NAME(` definition, NAME or NAME_recomp), and which are not
emitted; plus the recompiler-invented static functions, and every function
the emitted code CALLS that no emitted file defines (the runtime must provide
those: N64Recomp's reimplemented list, ignored functions, the Phase 1 shim
list). Stdlib only, Python 3.9+.
"""
import argparse
import json
import re
import sys
from collections import defaultdict
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]

_SECTION_RE = re.compile(r'\[\[section\]\]\s*name = "([^"]+)"(.*?)(?=\[\[section\]\]|\Z)', re.S)
_FUNC_RE = re.compile(r'\{ name = "(\w+)", vram = (0x[0-9A-Fa-f]+), size = (0x[0-9A-Fa-f]+) \}')
_DEF_RE = re.compile(r"^RECOMP_FUNC void (\w+)\(uint8_t\* rdram, recomp_context\* ctx\) \{", re.M)
_CALL_RE = re.compile(r"^\s+(\w+)\(rdram, ctx\);", re.M)
_DECL_RE = re.compile(r"^void (\w+)\(uint8_t\* rdram, recomp_context\* ctx\);", re.M)


def parse_symbols(path):
    out = {}
    for m in _SECTION_RE.finditer(Path(path).read_text()):
        out[m.group(1)] = [(name, int(vram, 16), int(size, 16))
                           for name, vram, size in _FUNC_RE.findall(m.group(2))]
    return out


def summarise(recomp_dir):
    recomp_dir = Path(recomp_dir)
    sections = parse_symbols(recomp_dir / "rush2049.syms.toml")
    funcs_dir = recomp_dir / "RecompiledFuncs"
    defined, callers = set(), defaultdict(set)
    for path in sorted(funcs_dir.glob("*.c")):
        text = path.read_text()
        current = None
        for line in text.splitlines():
            m = _DEF_RE.match(line)
            if m:
                current = m.group(1)
                defined.add(current)
                continue
            m = _CALL_RE.match(line)
            if m and current:
                callers[m.group(1)].add(current)
    header = (funcs_dir / "funcs.h").read_text() if (funcs_dir / "funcs.h").is_file() else ""
    declared = set(_DECL_RE.findall(header))

    report = {"sections": {}, "statics": sorted(n for n in defined if n.startswith("static_")),
              "defined_total": len(defined)}
    for name, funcs in sections.items():
        emitted = [f for f, _, _ in funcs if f in defined or f + "_recomp" in defined
                   or (f == "entrypoint" and "recomp_entrypoint" in defined)]
        missing = sorted(f for f, _, _ in funcs if f not in set(emitted))
        report["sections"][name] = {"functions": len(funcs), "emitted": len(emitted),
                                    "not_emitted": missing}
    external = sorted(n for n in callers if n not in defined)
    report["called_not_defined"] = {n: {"declared_in_funcs_h": n in declared,
                                        "callers": len(callers[n])} for n in external}
    return report


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__,
                                     formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument("--recomp-dir", default=str(REPO / "build" / "recomp"))
    parser.add_argument("--json", help="also write the full report here")
    args = parser.parse_args(argv)
    report = summarise(args.recomp_dir)
    for name, sec in report["sections"].items():
        print(f"{name:7s} {sec['functions']:5d} functions, {sec['emitted']:5d} emitted, "
              f"{len(sec['not_emitted']):3d} not emitted")
    print(f"recompiler-invented statics: {', '.join(report['statics']) or 'none'}")
    ext = report["called_not_defined"]
    decl = sorted(n for n, v in ext.items() if v["declared_in_funcs_h"])
    undecl = sorted(n for n, v in ext.items() if not v["declared_in_funcs_h"])
    print(f"called but not defined: {len(ext)} ({len(decl)} declared in funcs.h as reimplemented; "
          f"{len(undecl)} undeclared)")
    for n in undecl:
        print(f"  undeclared  {n}  ({ext[n]['callers']} callers)")
    if args.json:
        Path(args.json).write_text(json.dumps(report, indent=1, sort_keys=True) + "\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())
