#!/usr/bin/env python3
"""Compare umerge's inlining decisions in a run with the retail image.

    python3 wp/inline_edges.py RUN

For every "inlining X" under caller Y in runs/RUN/umerge.log:
  retail_jal      Y's retail body still calls X by jal  -> retail did NOT inline (all sites)
  retail_no_jal   Y's retail body has no jal to X        -> consistent with retail inlining
  no_target       X or Y has no retail body (stand-in, or a helper that only
                  exists inlined, e.g. zlib statics)
"""
import json
import re
import sys
from pathlib import Path
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import wp  # noqa: E402

tag = sys.argv[1]
run = json.loads((HERE / "runs" / tag / "result.json").read_text())
callers, addr = wp.real_callgraph()
tg = wp.score.targets()
keep = set(run["keep"])
cur = None
edges = []
for line in (HERE / "runs" / tag / "umerge.log").read_text().splitlines():
    m = re.match(r"\s+inlining (\S+)", line)
    if m:
        edges.append((cur, m.group(1)))
    elif line.strip():
        cur = line.strip()
out = {"retail_jal": [], "retail_no_jal": [], "no_target": []}
for y, x in sorted(set(edges)):
    if x not in tg or y not in tg:
        out["no_target"].append((y, x))
    elif y in callers.get(x, ()):
        out["retail_jal"].append((y, x))
    else:
        out["retail_no_jal"].append((y, x))
print(f"{tag}: {len(edges)} inlined call sites, {len(set(edges))} distinct caller/callee pairs")
for k, v in out.items():
    print(f"  {k}: {len(v)}")
    for y, x in v:
        st = run["members"].get(y, {}).get("status", "-")
        print(f"      {y} <- {x} [{'kept' if x in keep else 'internal'}, {len(tg.get(x, []))}w, "
              f"retail callers {len(callers.get(x, []))}]  caller status {st}")

# names umerge inlined although retail still calls them: candidates for a blocker
if len(sys.argv) > 2:
    names = sorted({x for _, x in out["retail_jal"]})
    old = set(Path(sys.argv[2]).read_text().split()) if Path(sys.argv[2]).exists() else set()
    Path(sys.argv[2]).write_text("".join(n + "\n" for n in sorted(old | set(names))))
    print(f"blocker list {sys.argv[2]}: {len(old)} -> {len(old | set(names))}")
