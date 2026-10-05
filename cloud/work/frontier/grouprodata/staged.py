#!/usr/bin/env python3
"""Compile a staged group on the builder and compare its members with the
retail image, without touching src/blob/groups, build/blob/obj or the lock.

    python3 cloud/work/frontier/grouprodata/staged.py GROUP [GROUP ...]
    python3 cloud/work/frontier/grouprodata/staged.py --no-compile GROUP
    python3 cloud/work/frontier/grouprodata/staged.py --mutate GROUP FILE OLD NEW

Groups are read from cloud/work/frontier/staged_groups/. Objects go to
build/grouprodata/obj/ (never build/blob/obj/groups/: the locked
camera_scene_manager object has the same name). `--mutate` copies the group
to build/grouprodata/mutants/<GROUP>/, replaces OLD with NEW (exactly once)
in FILE and compiles the copy as <GROUP>: it must be refused.
"""
import shutil
import struct
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(REPO))
from tools.cloud import owndata  # noqa: E402
from tools.conveyor.pipeline import blob_group, blob_layout, blob_splice  # noqa: E402

STAGED = REPO / "cloud" / "work" / "frontier" / "staged_groups"
OUT = REPO / "build" / "grouprodata"


def report(group, root, obj_dir, compile_it=True):
    doc = blob_layout.load()
    spec = blob_group.load(group, root)
    if compile_it:
        blob_group.compile_group(spec, obj_dir=obj_dir)
    obj = blob_group.object_path(group, obj_dir)
    print(f"{group}: {obj.relative_to(REPO)}")
    try:
        bodies = blob_group.group_bodies(group, doc, obj_dir=obj_dir, root=root)
    except blob_group.GroupError as exc:
        print(f"  REFUSED: {exc}")
        return 1
    diffs = blob_group.word_diffs(bodies, doc)
    wrong = blob_group.image_mismatches(bodies, doc)
    # What was verified, straight from owndata (the same call blob_group makes).
    image = Path(doc["image"]["path"])
    image = (REPO / image if not image.is_absolute() else image).read_bytes()
    base = int(doc["image"]["base"], 16)
    extents = {e["target_id"]: e for r in doc["regions"] for e in r["entries"]
               if e["kind"] == "function"}
    slices, _ = blob_group.member_slices(obj, spec["members"] + spec["context"], extents)
    retail = owndata.ImageData.from_image(image, base)
    for member in spec["members"]:
        off, vaddr, size = slices[member]
        want = list(struct.unpack(f">{size // 4}I", image[vaddr - base:vaddr - base + size]))
        res = owndata.verify(obj, member, want, address=vaddr, image=retail, start=off,
                             addresses=lambda n: slices[n][1] if n in slices
                             else blob_splice.address_named(n))
        state = ("equal to the image" if member not in wrong
                 else f"{diffs[member]} words differ, first at +0x{wrong[member]:x}")
        own = (f"; own data: {res.references} references, {len(res.sites)} sites verified"
               + (" (" + "; ".join(res.notes) + ")" if res.notes else "")
               if res.references else "")
        print(f"  member {member} @0x{vaddr:08X} {size // 4} words: {state}{own}")
    return 1 if wrong else 0


def main(argv):
    obj_dir = OUT / "obj"
    if argv[:1] == ["--mutate"]:
        group, name, old, new = argv[1:5]
        root = OUT / "mutants"
        if (root / group).exists():
            shutil.rmtree(root / group)
        shutil.copytree(STAGED / group, root / group)
        path = root / group / name
        text = path.read_text()
        if text.count(old) != 1:
            raise SystemExit(f"{name}: {old!r} occurs {text.count(old)} times, need exactly 1")
        path.write_text(text.replace(old, new))
        print(f"mutant: {name}: {old!r} -> {new!r}")
        status = report(group, root, OUT / "mutant-obj")
        print("mutant " + ("REFUSED as required" if status else "ACCEPTED: THIS IS A BUG"))
        return 0 if status else 1
    compile_it = "--no-compile" not in argv
    status = 0
    for group in [a for a in argv if not a.startswith("--")]:
        status |= report(group, STAGED, obj_dir, compile_it)
    return status


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
