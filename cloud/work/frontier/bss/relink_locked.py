#!/usr/bin/env python3
"""Read-only regression check: relink EVERY locked game body with the current
blob_splice/blob_group code (cached objects, no compile, no lock write) and
compare each with the retail image bytes at its extent.

    PYTHONPATH=. python3 cloud/work/frontier/bss/relink_locked.py
"""
import sys
from pathlib import Path

from tools.cloud import owndata
from tools.conveyor.pipeline import blob_layout, blob_splice


def main():
    document = blob_layout.load()
    image = Path(document["image"]["path"]).read_bytes()
    base = int(document["image"]["base"], 16)
    extents = {e["target_id"]: e for region in document["regions"]
               for e in region["entries"] if e["kind"] == "function"}
    lock = blob_splice.load_lock()
    bodies = blob_splice.spliced_bodies(lock, document)
    bad = []
    for name, body in sorted(bodies.items()):
        e = extents[name]
        if body != image[e["vaddr"] - base:e["vaddr"] - base + e["size"]]:
            bad.append(name)
    singles = [t for t, e in lock.items() if not e.get("group")]
    own_bss = 0
    for name in singles:
        obj = blob_splice.OBJ_DIR / f"{name}.o"
        elf = owndata._Object(obj)
        if any(sec["name"] in owndata.UNINITIALISED and sec["size"] for sec in elf.sections):
            own_bss += 1
    print(f"lock {len(lock)} entries ({len(singles)} singles); relinked {len(bodies)}; "
          f"{len(bad)} differ from the image{': ' + ', '.join(bad) if bad else ''}; "
          f"singles whose object has a non-empty .bss: {own_bss}")
    return 1 if bad or len(bodies) != len(lock) else 0


if __name__ == "__main__":
    sys.exit(main())
