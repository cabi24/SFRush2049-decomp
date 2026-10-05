#!/usr/bin/env python3
"""Recompute every locked group's member bodies from the existing objects in
build/blob/obj/groups/ and compare them with the retail image. Read-only.

    python3 cloud/work/frontier/grouprodata/regress.py [--json OUT]

Prints one line per group and a sha256 over all bodies, so two versions of
blob_group.py can be compared byte for byte (run it on each, diff the JSON).
"""
import hashlib
import json
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(REPO))
from tools.conveyor.pipeline import blob_group, blob_layout, blob_splice  # noqa: E402


def main():
    doc = blob_layout.load()
    lock = blob_splice.load_lock()
    groups = sorted({e["group"] for e in lock.values() if e.get("group")})
    extern = blob_splice.image_symbols(doc)
    out, bad, members = {}, 0, 0
    for group in groups:
        try:
            bodies = blob_group.group_bodies(group, doc, extern=extern)
            wrong = blob_group.image_mismatches(bodies, doc)
            locked = {m for m, e in lock.items() if e.get("group") == group}
            if set(bodies) != locked:
                wrong["<member set>"] = sorted(set(bodies) ^ locked)
        except blob_group.GroupError as exc:
            bodies, wrong = {}, {"<error>": str(exc)}
        members += len(bodies)
        bad += bool(wrong)
        out[group] = {m: hashlib.sha256(b).hexdigest() for m, b in sorted(bodies.items())}
        if wrong:
            out[group]["<problems>"] = {k: str(v) for k, v in wrong.items()}
            print(f"  FAIL {group}: {wrong}")
    digest = hashlib.sha256(json.dumps(out, sort_keys=True).encode()).hexdigest()
    print(f"groups {len(groups)}, members {members}, groups with problems {bad}, "
          f"bodies sha256 {digest}")
    if "--json" in sys.argv:
        Path(sys.argv[sys.argv.index("--json") + 1]).write_text(
            json.dumps(out, indent=1, sort_keys=True) + "\n")
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
