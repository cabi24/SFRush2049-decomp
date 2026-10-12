#!/usr/bin/env python3
"""Check that the host runtime defines everything the recompiled code needs.

    python3 tools/recomp/runtime/check_contract.py --manifest build/recomp/manifest.json \
            [--runtime tools/recomp/runtime/src] [--n64modernruntime DIR]

The generator (tools/recomp/gen_config.py) records in manifest.json
"runtime_contract": "shims" (names called by the emitted code that N64Recomp
does not emit and N64ModernRuntime does not implement) and "hooks" (C
functions its [[patches.hook]] entries call). A runtime source defines a name
with SHIM(name) / INERT(name) / NOPACK(name) / FORWARD(name, ...) or
`extern "C" void name(`. Exit 1 listing what is missing; 0 otherwise.
Stdlib only.
"""
import argparse
import json
import re
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
_DEF = re.compile(r'^\s*(?:SHIM|INERT|NOPACK|FORWARD)\(\s*([A-Za-z_][A-Za-z0-9_]*)'
                  r'|(?:extern\s+"C"\s+)?void\s+([A-Za-z_][A-Za-z0-9_]*)\s*\(\s*(?:uint8_t\s*\*|RDRAM_ARG)', re.M)


def defined_names(src_dir, recursive=False, patterns=("*.cpp",)):
    names = set()
    paths = []
    for pat in patterns:
        paths += Path(src_dir).glob("**/" + pat if recursive else pat)
    for path in sorted(paths):
        text = re.sub(r"//[^\n]*", "", path.read_text())
        for m in _DEF.finditer(text):
            names.add(m.group(1) or m.group(2))
    return names


def missing(contract, names):
    need = sorted(set(contract.get("shims", {})) | set(contract.get("hooks", [])))
    return [n for n in need if n not in names]


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--manifest", required=True)
    ap.add_argument("--runtime", default=str(HERE / "src"))
    ap.add_argument("--n64modernruntime", help="N64ModernRuntime checkout: names its librecomp/"
                    "ultramodern sources define (e.g. osYieldThread_recomp) count as provided")
    ap.add_argument("--extra-src", action="append", default=[],
                    help="more sources (*.c, *.cpp) defining names: the generated native "
                    "replacement wrappers (build/recomp/replace/native)")
    args = ap.parse_args(argv)
    contract = json.loads(Path(args.manifest).read_text()).get("runtime_contract")
    if contract is None:
        print("check_contract: manifest has no runtime_contract (rerun gen_config.py)", file=sys.stderr)
        return 1
    names = defined_names(args.runtime)
    if args.n64modernruntime:
        for sub in ("librecomp/src", "ultramodern/src"):
            names |= defined_names(Path(args.n64modernruntime) / sub, recursive=True)
    for extra in args.extra_src:
        if Path(extra).is_dir():
            names |= defined_names(extra, patterns=("*.c", "*.cpp"))
    gone = missing(contract, names)
    for name in gone:
        if name in contract.get("hooks", []):
            why = "hook"
        else:
            callers = contract.get("shims", {}).get(name)
            why = "called by " + ", ".join(callers) if callers else "in the section table"
        print(f"check_contract: runtime does not define {name} ({why})", file=sys.stderr)
    if not gone:
        print(f"check_contract: {len(contract.get('shims', {}))} shims and "
              f"{len(contract.get('hooks', []))} hooks defined")
    return 1 if gone else 0


if __name__ == "__main__":
    sys.exit(main())
