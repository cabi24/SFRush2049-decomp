#!/usr/bin/env python3
"""Describe the members of a wp.py run that do not match.

    python3 wp/analyze.py RUN [--base RUN2] [--list]
"""
import json
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent


def load(tag):
    return json.loads((HERE / "runs" / tag / "result.json").read_text())


def good(r):
    return r["status"] in ("MATCH", "MATCH_UNVERIFIED", "MATCH_STUBTAIL")


def main():
    tag = sys.argv[1]
    run = load(tag)
    base = json.loads((HERE / "runs" / "baseline" / "result.json").read_text())
    members, keep, refs = run["members"], set(run["keep"]), run["refs"]
    canon = run["canon"]
    defined = set(refs)
    callers = {}
    for n, rs in refs.items():
        for r in rs:
            if r in defined:
                callers.setdefault(r, set()).add(n)
    rows = []
    for name, r in sorted(members.items()):
        if good(r):
            continue
        internal_callees = sorted(c for c in refs[name] if c in defined and c not in keep)
        kept_callees = sorted(c for c in refs[name] if c in defined and c in keep)
        rows.append(dict(
            name=name, kind="group" if canon[name].startswith("g_") else "single",
            file=canon[name], kept=name in keep, status=r["status"],
            target_words=r.get("total"), got_words=r.get("words"), differing=r.get("differing"),
            internal_callees=internal_callees, kept_callees=kept_callees,
            unit_callers=sorted(callers.get(name, [])),
            unresolved=r.get("unresolved"), errors=r.get("errors"), detail=r.get("detail"),
            diff=r.get("diff", "")))
    cls = {}
    for row in rows:
        if row["status"] == "ABSENT":
            c = "absent (deleted/inlined away)"
        elif row["got_words"] != row["target_words"]:
            if not row["kept"] and row["internal_callees"]:
                c = "size differs; member internal AND has internal callees"
            elif row["internal_callees"]:
                c = "size differs; has internal (visible, non-kept) callees"
            elif not row["kept"]:
                c = "size differs; member itself internal"
            else:
                c = "size differs; kept, only kept/external callees"
        else:
            if not row["kept"] and row["internal_callees"]:
                c = "same size; member internal AND has internal callees"
            elif row["internal_callees"]:
                c = "same size; has internal callees"
            elif not row["kept"]:
                c = "same size; member itself internal"
            else:
                c = "same size; kept, only kept/external callees"
        row["class"] = c
        cls.setdefault(c, []).append(row["name"])
    print(f"run {tag}: {len(members)} members, {len(rows)} not matching")
    kinds = {}
    for row in rows:
        kinds[row["kind"]] = kinds.get(row["kind"], 0) + 1
    print("by locked recipe:", kinds)
    for c, names in sorted(cls.items(), key=lambda kv: -len(kv[1])):
        print(f"  {len(names):4d}  {c}   e.g. {', '.join(names[:6])}")
    if "--list" in sys.argv:
        for row in rows:
            print(f"\n{row['name']} [{row['kind']}, {'kept' if row['kept'] else 'internal'}] "
                  f"{row['status']} target {row['target_words']}w got {row['got_words']}w "
                  f"differ {row['differing']}  file {row['file']}")
            print(f"   internal callees {row['internal_callees']}  kept callees {row['kept_callees']}")
            print(f"   callers in unit {row['unit_callers']}")
            if row["unresolved"] or row["errors"] or row["detail"]:
                print("   ", row["unresolved"], row["errors"], row["detail"])
            print(row["diff"][:500].rstrip())
    (HERE / "runs" / tag / "nonmatching.json").write_text(json.dumps(rows, indent=1))


if __name__ == "__main__":
    main()
