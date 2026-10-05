#!/usr/bin/env python3
"""One line per run: python3 wp/summary.py"""
import json
from pathlib import Path
HERE = Path(__file__).resolve().parent
for name in ("baseline", "o3single"):
    p = HERE / "runs" / name / "result.json"
    if p.exists():
        res = json.loads(p.read_text())
        t = {}
        for r in res.values():
            t[r["status"]] = t.get(r["status"], 0) + 1
        print(f"{name:16s} members {len(res):4d} OK {sum(v for k, v in t.items() if k.startswith('MATCH')):4d}  {t}")
print(f"{'run':16s} {'files':>5s} {'memb':>4s} {'def':>5s} {'keep':>5s} {'stand':>5s} {'block':>5s} "
      f"{'OK':>4s} {'strict':>6s} {'unver':>5s} {'stub':>4s} {'DIFF':>4s}  cc_s  link_s  order(LIS/n, inv)  callee-first")
for p in sorted((HERE / "runs").glob("*/result.json")):
    d = json.loads(p.read_text())
    s = d.get("summary")
    if not s:
        continue
    t = s.get("tally", {})
    e = s.get("emission", {})
    ok = sum(v for k, v in t.items() if k.startswith("MATCH"))
    print(f"{s['tag']:16s} {s['files']:5d} {s['members']:4d} {s['defined']:5d} {s['keep']:5d} "
          f"{s['standins']:5d} {s.get('blocked', 0):5d} {ok:4d} {t.get('MATCH', 0):6d} "
          f"{t.get('MATCH_UNVERIFIED', 0):5d} {t.get('MATCH_STUBTAIL', 0):4d} {t.get('DIFF', 0) + t.get('ABSENT', 0) + t.get('UNRESOLVED', 0):4d}  "
          f"{s['cc_seconds']:4.1f}  {s['link_to_object_seconds']:5.1f}   "
          f"{e.get('longest_increasing_subsequence', '-')}/{e.get('with_address', '-')}, {e.get('adjacent_inversions', '-')}"
          f"   {e.get('callee_before_caller', '-')}/{e.get('call_edges', '-')}")
