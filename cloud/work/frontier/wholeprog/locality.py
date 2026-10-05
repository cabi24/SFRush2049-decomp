#!/usr/bin/env python3
"""Are internal (IPA) functions file-local, as C `static` would require?

    python3 wp/locality.py RUN

For every function of the run with a retail body and at least one retail jal
caller: the span (bytes) between it and its farthest retail caller, split by
internal vs kept in that run's keep list.
"""
import json
import statistics
import sys
from pathlib import Path
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import wp  # noqa: E402

run = json.loads((HERE / "runs" / sys.argv[1] / "result.json").read_text())
callers, addr = wp.real_callgraph()
keep = set(run["keep"])
rows = {"internal": [], "kept": []}
for n in run["refs"]:
    cs = callers.get(n)
    if not cs or n not in addr:
        continue
    span = max(abs(addr[c] - addr[n]) for c in cs)
    rows["internal" if n not in keep else "kept"].append((span, n, len(cs)))
for k, v in rows.items():
    spans = sorted(s for s, _, _ in v)
    q = lambda p: spans[min(len(spans) - 1, int(p * len(spans)))]
    print(f"{k}: {len(v)} functions with retail callers; farthest-caller span bytes: "
          f"median {q(.5)}, p90 {q(.9)}, max {spans[-1]}; "
          f"share within 16 KB: {sum(s <= 0x4000 for s in spans) / len(spans):.0%}, "
          f"within 64 KB: {sum(s <= 0x10000 for s in spans) / len(spans):.0%}")
print("internal functions with the farthest callers:")
for span, n, c in sorted(rows["internal"], reverse=True)[:12]:
    print(f"   {n} @{addr[n]:#x}: {c} retail callers, farthest {span:#x} bytes away")
