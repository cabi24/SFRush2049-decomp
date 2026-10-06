"""Install a matched group from a work directory and splice it, superseding old lock entries.

    PYTHONPATH=. python3 cloud/work/frontier/tools/install_group.py SRC_DIR NAME \
        [--revert-group OLD ...] [--revert-single FN ...] [--provenance TEXT]

Copies group.json (without "claims") and its files to src/blob/groups/NAME, reverts the
named old groups/singles through the pipeline, splices NAME, and checks that no function
that was locked before is unlocked after. On any failure the lock, the group directories
and the reverted single sources are restored and the image is rebuilt from the old lock.
Run blob_group check, blob_splice check, blob_unit check and blob_rom rom afterwards.
"""
import argparse, json, shutil, sys, tempfile
from pathlib import Path
from tools.conveyor.pipeline import blob_group, blob_splice, blob_layout

p = argparse.ArgumentParser()
p.add_argument("src"); p.add_argument("name")
p.add_argument("--revert-group", action="append", default=[])
p.add_argument("--revert-single", action="append", default=[])
p.add_argument("--provenance", default="")
a = p.parse_args()

root = blob_group.GROUP_DIR
src = Path(a.src)
spec = json.loads((src / "group.json").read_text())
spec.pop("claims", None)
if a.provenance:
    spec["provenance"] = a.provenance
lock_before = blob_splice.LOCKFILE.read_text()
before = set(json.loads(lock_before))
backup = Path(tempfile.mkdtemp(prefix="install-group-"))
touched = set(a.revert_group) | {a.name}
for g in touched:
    if (root / g).exists():
        shutil.copytree(root / g, backup / "groups" / g)
for fn in a.revert_single:
    f = blob_splice.SRC_DIR / f"{fn}.c"
    if f.exists():
        (backup / "singles").mkdir(exist_ok=True)
        shutil.copy(f, backup / "singles" / f.name)

def restore(why):
    print(f"FAILED: {why}\nrestoring lock and sources", file=sys.stderr)
    blob_splice.LOCKFILE.write_text(lock_before)
    for g in touched:
        shutil.rmtree(root / g, ignore_errors=True)
        if (backup / "groups" / g).exists():
            shutil.copytree(backup / "groups" / g, root / g)
    restored = {}
    for f in (backup / "singles").glob("*.c") if (backup / "singles").exists() else ():
        shutil.copy(f, blob_splice.SRC_DIR / f.name)
        restored[f.stem] = blob_splice.SRC_DIR / f.name
    lock = json.loads(lock_before)
    by_flags = {}                             # a revert deleted their objects
    for fn, path in restored.items():
        by_flags.setdefault(lock[fn]["flagset"], {})[fn] = path
    for flags, sources in by_flags.items():
        blob_splice.compile_on_builder(sources, flags)
    for g in touched:                         # a failed splice left a fresh object behind
        if (root / g / "group.json").exists():
            blob_group.compile_group(blob_group.load(g))
    doc = blob_layout.load()
    ok, _, msg = blob_splice.build_with(blob_splice.spliced_bodies(document=doc), doc)
    print("image after restore:", "OK" if ok else msg, file=sys.stderr)
    sys.exit(1)

try:
    for g in a.revert_group:
        if g != a.name:
            members, ok, msg = blob_group.revert(g)
            if not ok:
                raise RuntimeError(f"revert {g}: {msg}")
    if a.revert_single:
        r = blob_splice.revert(a.revert_single, blob_layout.load())
        if not r["image_ok"]:
            raise RuntimeError(f"revert singles: {r['message']}")
    dst = root / a.name
    shutil.rmtree(dst, ignore_errors=True)
    dst.mkdir(parents=True)
    for f in spec["files"]:
        shutil.copy(src / f, dst / f)
    (dst / "group.json").write_text(json.dumps(spec, indent=1) + "\n")
    result = blob_group.splice(a.name)
    after = set(json.loads(blob_splice.LOCKFILE.read_text()))
    lost = before - after
    if lost:
        raise RuntimeError(f"functions locked before are unlocked now: {sorted(lost)}")
except Exception as exc:                      # noqa: BLE001 - restore on anything
    restore(exc)
for g in a.revert_group:
    if g != a.name:
        shutil.rmtree(root / g, ignore_errors=True)
print(f"spliced {a.name}: {len(result['spliced'])} members, new: {sorted(after - before)}")
blob_splice._print_coverage(blob_splice.coverage())
