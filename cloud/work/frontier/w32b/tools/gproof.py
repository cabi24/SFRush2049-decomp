#!/usr/bin/env python3
"""gproof.py GROUP_DIR — READ-ONLY replay of the blob_group splice proof for a lane packet (w32b; after w31b's
indep_tables_proof.py). Builds the group object with blob_group.builder_script in the lane's builder scratch
(~/rush2049/scratch/frontier/w32b/gp/<group>), fetches it to build/gproof/<group>.o, then runs
blob_group.group_bodies (relocation + own-data proofs, incl. the wave-31 jump-table/independent-regions rules)
and compares each member's bytes with the image. Nothing under tools/ or src/ is modified."""
import subprocess, sys
from pathlib import Path
ROOT = Path(__file__).resolve().parents[5]
sys.path.insert(0, str(ROOT))
from tools.conveyor.pipeline import blob_group as bg, blob_layout

gdir = Path(sys.argv[1]).resolve()
name = gdir.name
spec = bg.load(name, root=gdir.parent)
work = f"~/rush2049/scratch/frontier/w32b/gp/{name}"
subprocess.run(["ssh", "watchman2", f"rm -rf {work} && mkdir -p {work}"], check=True)
files = [str(gdir / f) for f in spec["files"]]
keep = gdir.parent / f".{name}.keep.txt"
keep.write_text("".join(k + "\n" for k in spec["keep"]))
subprocess.run(["scp", "-q", *files, f"watchman2:{work}/"], check=True)
subprocess.run(["scp", "-q", str(keep), f"watchman2:{work}/keep.txt"], check=True)
keep.unlink()
r = subprocess.run(["ssh", "watchman2", bg.builder_script(spec, workdir=work)], capture_output=True, text=True)
print(r.stdout.strip(), r.stderr.strip()[-2000:])
out = ROOT / "build" / "gproof"
out.mkdir(parents=True, exist_ok=True)
subprocess.run(["scp", "-q", f"watchman2:{work}/group.o", str(out / f"{name}.o")], check=True)
doc = blob_layout.load()
bodies = bg.group_bodies(name, doc, obj_dir=out, root=gdir.parent)
print("group_bodies OK:", ", ".join(sorted(bodies)))
img = Path(doc["image"]["path"]); img = ROOT / img if not img.is_absolute() else img
data = img.read_bytes(); base = int(doc["image"]["base"], 16)
ext = {e["target_id"]: e for reg in doc["regions"] for e in reg["entries"] if e["kind"] == "function"}
bad = 0
for m, b in sorted(bodies.items()):
    e = ext[m]
    a = e["vaddr"]
    want = data[a - base:a - base + len(b)]
    diff = sum(1 for i in range(0, len(b), 4) if b[i:i+4] != want[i:i+4])
    bad += diff != 0
    print(f"  {m}: {len(b)//4} words, {diff} differ from the image")
print(f"{len(bodies) - bad}/{len(bodies)} members image-identical")
