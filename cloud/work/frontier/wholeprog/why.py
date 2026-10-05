#!/usr/bin/env python3
"""Facts about functions: python3 wp/why.py RUN name [name...]"""
import json
import sys
from pathlib import Path
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import wp  # noqa: E402

run = json.loads((HERE / "runs" / sys.argv[1] / "result.json").read_text())
inv = json.loads((HERE / "inventory.json").read_text())
lock = wp.load_lock()
callers, addr = wp.real_callgraph()
tg = wp.score.targets()
keep, refs = set(run["keep"]), run["refs"]
defined = set(refs)
unit_callers = {}
for n, rs in refs.items():
    for r in rs:
        unit_callers.setdefault(r, set()).add(n)
specs = {g: json.loads((wp.ROOT / "src/blob/groups" / g / "group.json").read_text()) for g in inv["groups"]}
for name in sys.argv[2:]:
    rec = lock.get(name)
    kind = "unlocked" if rec is None else ("group:" + rec["group"] if "group" in rec else "single")
    real = callers.get(name, set())
    print(f"{name}: {kind}; retail {len(tg.get(name, []))}w at {addr.get(name, 0):#x}; "
          f"{'KEPT' if name in keep else 'internal'}; canonical {run['canon'].get(name)}")
    print(f"   retail callers {len(real)}; in unit {len(real & defined)}; "
          f"unit callers by source refs {sorted(unit_callers.get(name, []))}")
    st = run["members"].get(name)
    if st:
        print(f"   status {st['status']} got {st.get('words')}w")
    for fid, r in inv["files"].items():
        if r["kind"] == "group" and any(d["name"] == name for d in r["defs"]):
            sp = specs[r["owner"]]
            role = ("member" if name in sp["members"] else "context")
            print(f"   defined in group {r['owner']} as {role}, "
                  f"{'kept' if name in sp['keep'] else 'INTERNAL'} there")
