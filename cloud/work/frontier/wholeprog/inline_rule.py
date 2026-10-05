#!/usr/bin/env python3
"""Synthetic probe of umerge's inlining rule (callee size x number of call sites x kept).

    python3 wp/inline_rule.py
Prints, per (statements in callee, call sites), whether umerge -v reports "inlining".
"""
import shutil
import sys
from pathlib import Path
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import wp  # noqa: E402


def probe(stmts, sites, kept, dead=0, calls_in_callee=0, callers=1):
    work = wp.RUNS / "rule"
    if work.exists():
        shutil.rmtree(work)
    work.mkdir(parents=True)
    body = "".join(f"  D[{i}] = a + {i};\n" for i in range(stmts))
    body += "".join(f"  ext{i}(a);\n" for i in range(calls_in_callee))
    if dead:
        body += "  if (0) {\n" + "".join(f"    D[{i}] = a * {i + 3};\n" for i in range(dead)) + "  }\n"
    src = "extern int D[]; extern void ext0(int), ext1(int), ext2(int);\n"
    src += f"void g(int a)\n{{\n{body}}}\n"
    per = [sites // callers + (1 if k < sites % callers else 0) for k in range(callers)]
    for k, n in enumerate(per):
        src += f"void f{k}(int a)\n{{\n" + "".join(f"  g(a + {i});\n" for i in range(n)) + "}\n"
    (work / "t.c").write_text(src)
    _, rc, msg = wp.cc_j(work, "t.c", wp.O3)
    assert rc == 0, msg
    keep = [f"f{k}" for k in range(callers)] + (["g"] if kept else [])
    (work / "keep.txt").write_text("".join(k + "\n" for k in keep))
    times, err = wp.stages(work, ["t.u"], work / "o.o", work / "log")
    assert not err, err
    return (work / "umerge.log").read_text().count("inlining g")


if __name__ == "__main__":
    for kept in (True, False):
        print(f"callee {'kept' if kept else 'internal'}: rows = statements in callee, cols = call sites 1,2,3,5,9 (one caller each)")
        for stmts in (0, 1, 2, 3, 4, 6, 8, 12, 16, 24, 40, 80):
            row = [probe(stmts, s, kept, callers=s) for s in (1, 2, 3, 5, 9)]
            print(f"   {stmts:3d} stmts: " + " ".join(f"{r:2d}" for r in row))
    print("empty callee + dead `if (0)` block of N statements, 1 call site / 3 call sites (kept):")
    for dead in (0, 1, 2, 4, 8, 16, 32, 64):
        print(f"   dead {dead:3d}: {probe(0, 1, True, dead=dead)} {probe(0, 3, True, dead=dead, callers=3)}")
    print("callee that itself calls N externals (1 stmt), 1 / 3 call sites (kept):")
    for c in (0, 1, 2, 3):
        print(f"   calls {c}: {probe(1, 1, True, calls_in_callee=c)} {probe(1, 3, True, calls_in_callee=c, callers=3)}")
