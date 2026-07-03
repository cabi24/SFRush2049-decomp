#!/usr/bin/env python3
"""Job runner: entry point executed by the node agent inside a toolkit.

    python3 <toolkit>/jobs/runner.py --job <unpacked-bundle-dir> --out results.tar.gz

Reads manifest.json from the job dir, dispatches by job_type, maintains
progress.json (picked up by the agent's heartbeats), and packs the result
bundle with the contract's result.json envelope.
"""
import argparse
import hashlib
import json
import os
import sys
import tarfile
import tempfile
import time
from pathlib import Path

_HERE = Path(__file__).resolve().parent
if str(_HERE) not in sys.path:
    sys.path.insert(0, str(_HERE))


def canonical_json(obj):
    return json.dumps(obj, sort_keys=True, separators=(",", ":"), ensure_ascii=True)


class Progress:
    """Atomic progress.json writer for the agent's heartbeat thread."""

    def __init__(self, job_dir):
        self.path = Path(job_dir) / "progress.json"

    def update(self, **fields):
        tmp = self.path.with_suffix(".tmp")
        tmp.write_text(json.dumps(fields))
        os.replace(tmp, self.path)


def _dispatch(job_type):
    if job_type == "compile_score":
        import compile_score

        return compile_score.run
    if job_type == "flag_sweep":
        import flag_sweep

        return flag_sweep.run
    if job_type == "permuter_search":
        import permuter_search

        return permuter_search.run
    if job_type == "verify_promote":
        import verify_promote

        return verify_promote.run
    raise SystemExit(f"runner: unknown job_type {job_type!r}")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--job", required=True)
    parser.add_argument("--out", required=True)
    args = parser.parse_args()

    job_dir = Path(args.job)
    manifest = json.loads((job_dir / "manifest.json").read_text())
    progress = Progress(job_dir)
    started = time.monotonic()

    error = None
    payload = {}
    artifacts = {}  # archive_name -> path, packed next to result.json
    try:
        run = _dispatch(manifest["job_type"])
        payload, artifacts = run(job_dir, manifest, progress)
        exit_status = "ok"
    except Exception as exc:  # noqa: BLE001 — report, don't crash the node
        exit_status = "error"
        error = f"{type(exc).__name__}: {exc}"

    result = {
        "job_id": os.environ.get("CONVEYOR_JOB_ID", ""),
        "job_type": manifest["job_type"],
        "manifest_sha": hashlib.sha256(canonical_json(manifest).encode()).hexdigest(),
        "toolkit_sha": manifest.get("toolkit_sha"),
        "exit": exit_status,
        "error": error,
        "wall_seconds": round(time.monotonic() - started, 1),
        "payload": payload,
    }

    with tempfile.TemporaryDirectory() as tmp:
        rj = Path(tmp) / "result.json"
        rj.write_text(json.dumps(result))
        with tarfile.open(args.out, "w:gz") as tar:
            tar.add(rj, arcname="result.json")
            for name, path in sorted(artifacts.items()):
                tar.add(path, arcname=name)
    return 0 if exit_status == "ok" else 1


if __name__ == "__main__":
    sys.exit(main())
