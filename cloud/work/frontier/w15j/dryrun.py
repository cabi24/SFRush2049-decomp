"""Read-only dry run: relocate the scratch-compiled physics_sym group, verify own data,
compare every member body with the extracted image. Writes nothing in the repo."""
import sys
from tools.conveyor.pipeline import blob_group, blob_layout, blob_splice
from tools.cloud import owndata
sp = sys.argv[1]
doc = blob_layout.load()
spec = blob_group.load("physics_sym", sp + "/groups")
print("source_sha", blob_group.source_sha(spec))
# the gate exactly as splice() runs it (group_bodies -> relocate -> per_reference -> owndata)
bodies = blob_group.group_bodies("physics_sym", doc, obj_dir=sp + "/obj", root=sp + "/groups")
print("members relocated:", sorted(bodies))
print("image mismatches:", blob_group.image_mismatches(bodies, doc) or "none")
print("word diffs:", blob_group.word_diffs(bodies, doc))
# show func_800B59F0's own-data result as the gate saw it
import struct
from pathlib import Path
obj = Path(sp) / "obj" / "physics_sym.o"
slices, ndx = blob_group.member_slices(obj, spec["members"], {e["target_id"]: e for r in doc["regions"] for e in r["entries"] if e["kind"] == "function"})
img = Path(doc["image"]["path"]); img = (blob_group.REPO / img if not img.is_absolute() else img).read_bytes()
base = int(doc["image"]["base"], 16)
off, vaddr, size = slices["func_800B59F0"]
want = list(struct.unpack(f">{size // 4}I", img[vaddr - base:vaddr - base + size]))
ext = blob_splice.image_symbols(doc)
named = lambda n: slices[n][1] if n in slices else (ext.get(n) or blob_splice.address_named(n))
for runs in (None, blob_group.opaque_runs(doc)):
    r = blob_group.verify_own_data(obj, "func_800B59F0", want, vaddr,
                       owndata.ImageData.from_image(img, base), start=off,
                       addresses=named, data_runs=runs)
    print("data_runs" if runs else "owndata only", "ok=", r.ok, "failures=", r.failures,
          "unverified=", r.unverified)
    print("  placements=", {k: [(hex(a), hex(b), hex(c), d) for a, b, c, d in v] for k, v in r.placements.items()})
    print("  notes=", r.notes)
