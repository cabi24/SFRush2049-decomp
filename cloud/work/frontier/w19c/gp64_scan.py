#!/usr/bin/env python3
"""w19c: assemble EVERY static asm region (targets.index_asm_regions) with the
pre-w19c isolated prelude and with the current targets.assemble_region, and
compare the resulting objects' .text bytes, relocations and symbols.

Run from the repo root on a tree where targets.assemble_region already takes
its mode directives from tools/asm-processor/prelude.inc (w19c). The OLD
prelude is reconstructed by dropping `.set gp=64` from the emitted source.
Prints one line per region whose outcome differs and a summary.
Usage: python3 cloud/work/frontier/w19c/gp64_scan.py [--limit N]
"""
import subprocess
import sys
import tempfile
from pathlib import Path

sys.path.insert(0, str(Path.cwd()))
from tools.conveyor.pipeline import targets as T  # noqa: E402

REAL_RUN = subprocess.run


def dump(obj):
    a = REAL_RUN(["mips-linux-gnu-objdump", "-dr", "-j", ".text", str(obj)],
                 capture_output=True, text=True).stdout
    return "\n".join(a.splitlines()[3:])


def gate(region, out, text):
    if text.startswith("ERROR"):
        return "assemble_error"
    ok, why = T.gate_target(region.words, out)
    return "gate_ok" if ok else "gate_refused:" + why


def assemble(region, name, out, drop_gp64):
    def run(cmd, **kw):
        if drop_gp64 and cmd[0] == "mips-linux-gnu-as":
            src = Path(cmd[-1])
            src.write_text("\n".join(l for l in src.read_text().splitlines()
                                     if l.strip() != ".set gp=64") + "\n")
        return REAL_RUN(cmd, **kw)
    T.subprocess.run = run
    try:
        T.assemble_region(region, name, out)
        return dump(out)
    except T.AssembleError as exc:
        return "ERROR " + str(exc)
    finally:
        T.subprocess.run = REAL_RUN


def main():
    limit = int(sys.argv[sys.argv.index("--limit") + 1]) if "--limit" in sys.argv else None
    regions = sorted(T.index_asm_regions().items())[:limit]
    same = diff = 0
    outcomes = {}
    with tempfile.TemporaryDirectory() as t:
        for vaddr, r in regions:
            old = assemble(r, r.name, Path(t) / "old.o", True)
            og = gate(r, Path(t) / "old.o", old)
            new = assemble(r, r.name, Path(t) / "new.o", False)
            ng = gate(r, Path(t) / "new.o", new)
            if old == new:
                same += 1
            else:
                diff += 1
                outcomes[(og, ng)] = outcomes.get((og, ng), 0) + 1
                print(f"DIFF {r.name} @0x{vaddr:08X} {len(r.words)} words: old {og} -> new {ng}")
    print(f"regions {len(regions)}: identical objects {same}, different {diff}")
    for (og, ng), n in sorted(outcomes.items()):
        print(f"  {n:3d} x  old {og}  ->  new {ng}")


if __name__ == "__main__":
    main()
