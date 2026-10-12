#!/usr/bin/env python3
"""w24b (builder, scratch root): write OUT/lib_1050.c = src/rom/lib_1050.c with the 7 vi* slots spliced
(lane/tools/splice.py replica, ctx lane/ctx53_vi.jsonl) and the promoted "viStub" definition removed (at -g1 -O1
its 20 bytes are viDeadlinePassed's two trailing `jr ra; nop` pairs).  usage: python3 vi1050_tu.py OUTDIR"""
import re, subprocess, sys
from pathlib import Path
out = Path(sys.argv[1]); out.mkdir(parents=True, exist_ok=True)
fns = "viTickStart viEnableAccum viDisableAccum viUpdateTime viScheduleTick viAddTicks viGetTimeToDeadline".split()
subprocess.run([sys.executable, "lane/tools/splice.py", "lane/ctx53_vi.jsonl", str(out)] + fns, check=True)
p = out / "lib_1050.c"; t = p.read_text()
i = t.index("void viStub(void)")
c = t.rfind("/*", 0, i)            # its PROMOTED header comment, if directly above
if t[c:i].count("*/") == 1 and "viStub" in t[c:i]:
    i = c
j = t.index("{", t.index("void viStub(void)")); depth = 0
while True:
    depth += {"{": 1, "}": -1}.get(t[j], 0); j += 1
    if depth == 0:
        break
t = t[:i].rstrip() + "\n" + t[j:]
assert "viStub" not in t.split("viDeadlinePassed")[-1]
p.write_text(t)
print("wrote", p)
