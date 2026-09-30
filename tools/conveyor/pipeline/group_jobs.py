"""Queue and apply group_search jobs (permuter search inside an IPA call group).

    python3 -m tools.conveyor.pipeline.group_jobs candidates [--root DIR]
    python3 -m tools.conveyor.pipeline.group_jobs submit [--budget S] [--limit N] [GROUP[:FN] ...]
    python3 -m tools.conveyor.pipeline.group_jobs results
    python3 -m tools.conveyor.pipeline.group_jobs apply JOB_ID

The job itself lives in jobs/group_search.py (it runs on a node, in the
toolkit). This module builds its input bundle from a group directory and the
extracted image, and turns a zero-score result back into a spliced group.
Nothing here counts as a match: `apply` goes through blob_group.splice, which
is the image gate, and the ROM SHA-1 gate follows as for any splice.
"""
import argparse
import gzip
import json
import shutil
import sys
import tempfile
from pathlib import Path

from ..bundles.build_job import build_job_bundle
from . import blob_group, blob_layout, blob_splice

REPO = Path(__file__).resolve().parents[3]
CLOUD_ROOT = REPO / "cloud" / "work" / "ipa-groups"
DEFAULT_BUDGET = {"wall_seconds": 1200, "iterations": None}


class JobError(Exception):
    pass


def _extents(document):
    return {e["target_id"]: e for r in document["regions"] for e in r["entries"]
            if e["kind"] == "function"}


def _retail(document, entry):
    path = Path(document["image"]["path"])
    image = (REPO / path if not path.is_absolute() else path).read_bytes()
    off = entry["vaddr"] - int(document["image"]["base"], 16)
    return image[off:off + entry["size"]]


def job_spec(spec, target, document):
    """The groupdump spec: what a node needs to link the target's slice."""
    extents = _extents(document)
    if target not in extents:
        raise JobError(f"{target} has no extent in the layout")
    entry = extents[target]
    slices = {f: extents[f]["vaddr"] for f in spec["members"] + spec["context"]
              if f in extents}
    return {
        "target": target, "size": entry["size"], "vaddr": entry["vaddr"],
        "retail": _retail(document, entry).hex(),
        "slices": slices,
        "symbols": blob_splice.image_symbols(document),
    }


def seed_file_of(spec, target):
    for name in spec["files"]:
        text = (spec["dir"] / name).read_text()
        if blob_group._definition(text, target):
            return name
    raise JobError(f"no definition of {target} in {spec['files']}")


def build_bundle(spec, target, toolkit_sha, budget=None, document=None, out_dir=None):
    """(bundle path, manifest sha, job dict) ready for POST /work."""
    document = document or blob_layout.load()
    seed = seed_file_of(spec, target)
    manifest = {
        "job_type": "group_search",
        "toolkit_sha": toolkit_sha,
        "target_id": target,
        "group": spec["name"],
        "seed_file": "base.c",
        "seed_name": seed,
        "extra_files": [f for f in spec["files"] if f != seed],
        "keep": spec["keep"],
        "compile_flags": spec["flags"],
        "spec_file": "spec.json",
        "budget": budget or dict(DEFAULT_BUDGET),
    }
    inputs = {"base.c": (spec["dir"] / seed).read_bytes(),
              "spec.json": json.dumps(job_spec(spec, target, document)).encode()}
    for name in manifest["extra_files"]:
        inputs[name] = (spec["dir"] / name).read_bytes()
    out_dir = Path(out_dir or tempfile.mkdtemp(prefix="grpseed-"))
    bundle, manifest_sha = build_job_bundle(
        manifest, inputs, out_dir / f"group-{spec['name']}-{target}.tar.gz")
    job = {
        "job_type": "group_search", "manifest_sha": manifest_sha,
        "toolkit_sha": toolkit_sha, "batch": False, "max_attempts": None,
        "budget": manifest["budget"], "target_id": target,
    }
    return bundle, manifest_sha, job


def candidates(root=CLOUD_ROOT, document=None, lock=None):
    """[(group, function, size)] for functions of buildable groups that are not
    yet in the ROM, smallest first."""
    document = document or blob_layout.load()
    lock = blob_splice.load_lock() if lock is None else lock
    extents = _extents(document)
    out = []
    for gdir in sorted(Path(root).iterdir()):
        if not (gdir / "group.json").is_file():
            continue
        try:
            spec = blob_group.load(gdir.name, root)
            for fn in dict.fromkeys(spec["members"] + spec["context"]):
                if fn in extents and fn not in lock:
                    seed_file_of(spec, fn)
                    out.append((gdir.name, fn, extents[fn]["size"]))
        except (blob_group.GroupError, JobError, OSError):
            continue
    return sorted(out, key=lambda c: (c[2], c[0], c[1]))


# --- results --------------------------------------------------------------

def group_results(conn):
    return conn.execute(
        "SELECT job_id, target_id, state, best_score, best_source_sha, result_sha,"
        " budget, updated_at FROM work_unit WHERE job_type='group_search'"
        " ORDER BY best_score IS NULL, best_score, updated_at").fetchall()


def apply_result(conn, store, job_id, root=CLOUD_ROOT, dest=blob_group.GROUP_DIR,
                 document=None):
    """Splice a zero-score group_search result: the improved source replaces the
    group's, the target joins the members, and the image gate decides."""
    row = conn.execute("SELECT * FROM work_unit WHERE job_id=? AND job_type='group_search'",
                       (job_id,)).fetchone()
    if row is None:
        raise JobError(f"no group_search job {job_id}")
    if row["best_score"] != 0 or not row["best_source_sha"]:
        raise JobError(f"job {job_id} has no zero-score source (best {row['best_score']})")
    blob = store.get(row["best_source_sha"])
    if blob is None:
        raise JobError("best source blob is missing")
    source = gzip.decompress(blob.read_bytes())
    manifest = json.loads(_manifest_text(conn, store, row))
    group, target = manifest["group"], row["target_id"]
    src_dir = Path(dest) / group
    backup = None
    if src_dir.exists():
        backup = Path(tempfile.mkdtemp(prefix="grpbak-")) / group
        shutil.copytree(src_dir, backup)
    else:
        shutil.copytree(Path(root) / group, src_dir,
                        ignore=shutil.ignore_patterns("STATUS.md", "*.orig", "gen"))
    try:
        spec_path = src_dir / "group.json"
        gj = json.loads(spec_path.read_text())
        if target not in gj["members"]:
            gj["members"].append(target)
        gj["context"] = [f for f in gj.get("context", []) if f != target]
        for key in ("claims", "allow_unverified"):
            gj.pop(key, None)
        spec_path.write_text(json.dumps(gj, indent=2) + "\n")
        (src_dir / manifest["seed_name"]).write_bytes(source)
        return blob_group.splice(group, document)
    except Exception:
        if backup is not None:
            shutil.rmtree(src_dir)
            shutil.copytree(backup, src_dir)
        else:
            shutil.rmtree(src_dir, ignore_errors=True)
        raise


def _manifest_text(conn, store, row):
    """The job's manifest, read back from its bundle."""
    import tarfile
    path = store.get(row["bundle_sha"])
    with tarfile.open(path) as tar:
        member = tar.extractfile("manifest.json")
        return member.read().decode()


# --- CLI ------------------------------------------------------------------

def main(argv=None):
    from ..cli import API, _client, _open_db
    parser = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    parser.add_argument("--data", default=str(Path.home() / ".conveyor"))
    parser.add_argument("--coordinator", default="http://127.0.0.1:8323")
    parser.add_argument("--token", default=None)
    sub = parser.add_subparsers(dest="cmd", required=True)
    c = sub.add_parser("candidates")
    c.add_argument("--root", default=str(CLOUD_ROOT))
    s = sub.add_parser("submit")
    s.add_argument("--root", default=str(CLOUD_ROOT))
    s.add_argument("--budget", type=int, default=DEFAULT_BUDGET["wall_seconds"])
    s.add_argument("--limit", type=int, default=0)
    s.add_argument("--priority", type=int, default=50)
    s.add_argument("picks", nargs="*", help="GROUP or GROUP:FUNCTION")
    sub.add_parser("results")
    a = sub.add_parser("apply")
    a.add_argument("job_id")
    args = parser.parse_args(argv)
    conn, store = _open_db(args)

    if args.cmd == "candidates":
        for group, fn, size in candidates(args.root):
            print(f"{size:6d}  {group}:{fn}")
        return 0
    if args.cmd == "results":
        for r in group_results(conn):
            print(f"{r['job_id'][:8]} {r['state']:9} best={r['best_score']!s:>6} "
                  f"{r['target_id']}")
        return 0
    if args.cmd == "apply":
        try:
            done = apply_result(conn, store, args.job_id)
        except (JobError, blob_group.GroupError) as exc:
            print(f"refused: {exc}", file=sys.stderr)
            return 1
        print(f"spliced {done['spliced']}")
        return 0

    client = _client(args)
    toolkit = client.pinned_toolkit()
    document = blob_layout.load()
    wanted = [c for c in candidates(args.root, document)
              if not args.picks or c[0] in args.picks or f"{c[0]}:{c[1]}" in args.picks]
    open_targets = {(r["target_id"]) for r in conn.execute(
        "SELECT target_id FROM work_unit WHERE job_type='group_search'"
        " AND state IN ('PENDING','LEASED')")}
    if args.limit:
        wanted = wanted[:args.limit]
    queued = 0
    for group, fn, _size in wanted:
        if fn in open_targets:
            continue
        spec = blob_group.load(group, Path(args.root))
        bundle, _sha, job = build_bundle(spec, fn, toolkit, {"wall_seconds": args.budget,
                                                            "iterations": None}, document)
        status, out = client.call("POST", f"{API}/blobs", raw=bundle.read_bytes())
        job.update(bundle_sha=out["sha256"], priority=args.priority)
        client.call("POST", f"{API}/work", body=[job])
        open_targets.add(fn)
        queued += 1
    print(f"queued {queued} group_search jobs (budget {args.budget}s)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
