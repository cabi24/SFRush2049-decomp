#!/usr/bin/env python3
"""Corrupt one jump-table entry in a COPY of the staged camera_scene_manager
object (build/grouprodata/obj/, made by staged.py) and require a refusal.
The first .rodata word is entry 0 of func_800C1B60's first table."""
import struct
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[4]
sys.path.insert(0, str(REPO))
from tools.cloud import owndata  # noqa: E402
from tools.conveyor.pipeline import blob_group, blob_layout  # noqa: E402

src = REPO / "build/grouprodata/obj/camera_scene_manager.o"
out = REPO / "build/grouprodata/table-mutant-obj"
out.mkdir(parents=True, exist_ok=True)
blob = bytearray(src.read_bytes())
elf = owndata._Object(src)
rodata = next(s for s in elf.sections if s["name"] == ".rodata")
word, = struct.unpack_from(">I", blob, rodata["off"])
struct.pack_into(">I", blob, rodata["off"], word + 4)        # entry 0 lands one insn late
(out / "camera_scene_manager.o").write_bytes(blob)
try:
    blob_group.group_bodies("camera_scene_manager", blob_layout.load(), obj_dir=out,
                            root=REPO / "cloud/work/frontier/staged_groups")
except blob_group.GroupError as exc:
    print(f"REFUSED as required: {exc}")
    sys.exit(0)
print("ACCEPTED: THIS IS A BUG")
sys.exit(1)
