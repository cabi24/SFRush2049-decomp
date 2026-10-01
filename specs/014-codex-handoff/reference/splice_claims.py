import json, shutil, sys
from pathlib import Path
from tools.conveyor.pipeline import blob_group as bg, blob_layout, blob_splice
WANT = sys.argv[1:]
src_root, dst_root = Path("cloud/work/ipa-groups"), Path("src/blob/groups")
doc = blob_layout.load()
for g in WANT:
    lock = blob_splice.load_lock()
    cj = json.load(open(src_root / g / "group.json"))
    claims = [c for c in cj["claims"] if c not in lock]
    dst = dst_root / g
    backup = None
    if dst.exists():
        print(g, "already has a src/blob dir; skipping"); continue
    shutil.copytree(src_root / g, dst, ignore=shutil.ignore_patterns("STATUS.md", "*.orig", "gen", "__pycache__"))
    spec = json.load(open(dst / "group.json"))
    allf = spec["members"] + (spec.get("context") or [])
    spec["members"] = claims
    spec["context"] = [f for f in allf if f not in claims]
    for k in ("claims", "allow_unverified"):
        spec.pop(k, None)
    spec["provenance"] = f"cloud rounds 4-5 (PR #6), cloud/work/ipa-groups/{g}"
    (dst / "group.json").write_text(json.dumps(spec, indent=2) + "\n")
    try:
        r = bg.splice(g, doc)
        print(g, "SPLICED", r["spliced"], flush=True)
    except Exception as exc:
        print(g, "REFUSED", str(exc).splitlines()[0][:230], flush=True)
        shutil.rmtree(dst)
