"""Read-only dry run (w15j method): compile GROUP from SP/groups in builder scratch, fetch the object to
SP/obj, relocate through blob_group.group_bodies (the gate splice() runs), compare bodies with the
extracted image, and show MEMBER's own-data proof with and without data runs. Writes nothing in the repo."""
import sys, struct, subprocess
from pathlib import Path
from tools.conveyor.pipeline import blob_group, blob_layout, blob_splice
from tools.cloud import owndata
sp, group, member = sys.argv[1], sys.argv[2], sys.argv[3]
bdir = "~/rush2049/scratch/frontier/w16f/gdry/" + group
spec = blob_group.load(group, sp + "/groups")
print("source_sha", blob_group.source_sha(spec))
subprocess.run(["ssh", "watchman2", f"rm -rf {bdir} && mkdir -p {bdir}"], check=True)
files = [str(spec["dir"] / f) for f in spec["files"]]
Path(sp, "keep.txt").write_text("".join(k + "\n" for k in spec["keep"]))
subprocess.run(["scp", "-q", *files, sp + "/keep.txt", f"watchman2:{bdir}/"], check=True)
p = subprocess.run(["ssh", "watchman2", blob_group.builder_script(spec, workdir=bdir)], capture_output=True, text=True)
print("build:", p.stdout.strip(), p.stderr.strip()[:300])
out = blob_group.object_path(group, sp + "/obj")
subprocess.run(["scp", "-q", f"watchman2:{bdir}/group.o", str(out)], check=True)
doc = blob_layout.load()
try:
    bodies = blob_group.group_bodies(group, doc, obj_dir=sp + "/obj", root=sp + "/groups")
    print("members relocated:", sorted(bodies))
    print("image mismatches:", blob_group.image_mismatches(bodies, doc) or "none")
    print("word diffs:", blob_group.word_diffs(bodies, doc))
except blob_group.GroupError as e:
    print("REFUSED:", e)
slices, ndx = blob_group.member_slices(out, spec["members"], {e["target_id"]: e for r in doc["regions"] for e in r["entries"] if e["kind"] == "function"})
img = Path(doc["image"]["path"]); img = (blob_group.REPO / img if not img.is_absolute() else img).read_bytes()
base = int(doc["image"]["base"], 16)
off, vaddr, size = slices[member]
want = list(struct.unpack(f">{size // 4}I", img[vaddr - base:vaddr - base + size]))
ext = blob_splice.image_symbols(doc)
named = lambda n: slices[n][1] if n in slices else (ext.get(n) or blob_splice.address_named(n))
for runs in (None, blob_group.opaque_runs(doc)):
    r = blob_group.verify_own_data(out, member, want, vaddr, owndata.ImageData.from_image(img, base), start=off,
                                   addresses=named, data_runs=runs)
    print("data_runs" if runs else "owndata only", "ok=", r.ok, "failures=", r.failures, "unverified=", r.unverified)
    print("  placements=", {k: [(hex(a), hex(b), hex(c), d) for a, b, c, d in v] for k, v in r.placements.items()})
    print("  notes=", r.notes)
