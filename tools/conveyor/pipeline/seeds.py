"""Seed builder: turn (source, target, flags, budget) into a permuter_search
job bundle (FR-009 input). Used by the farm for automated seeds and by
`cli seed` for manual ones (FR-015).
"""
import tempfile
from pathlib import Path

from ..bundles.build_job import build_job_bundle

DEFAULT_BUDGET = {"wall_seconds": 4 * 3600, "iterations": None}
SHIM_INCLUDE = '#include "conveyor_shim.h"\n\n'


def seed_source(body, with_shim=True):
    """Candidate/seed body -> compilable translation unit text."""
    text = body if body.endswith("\n") else body + "\n"
    if with_shim and "conveyor_shim.h" not in text:
        text = SHIM_INCLUDE + text
    return text


def build_search_bundle(conn, store, target_id, source_text, flagset,
                        toolkit_sha, budget=None, out_dir=None):
    """Returns (bundle_path, manifest_sha, job_dict) ready for POST /work."""
    row = conn.execute(
        "SELECT target_o_sha FROM n64_target WHERE target_id = ?", (target_id,)
    ).fetchone()
    if row is None or row["target_o_sha"] is None:
        raise KeyError(f"no target object for {target_id}")
    target_o = store.get(row["target_o_sha"])
    if target_o is None:
        raise KeyError(f"target blob missing for {target_id}")

    manifest = {
        "job_type": "permuter_search",
        "toolkit_sha": toolkit_sha,
        "target_id": target_id,
        "seed_file": "base.c",
        "target_file": "target.o",
        "compile_flags": flagset,
        "budget": budget or dict(DEFAULT_BUDGET),
    }
    out_dir = Path(out_dir or tempfile.mkdtemp(prefix="seed-"))
    bundle_path, manifest_sha = build_job_bundle(
        manifest,
        {"base.c": source_text.encode(), "target.o": target_o.read_bytes()},
        out_dir / f"search-{target_id}.tar.gz",
    )
    job = {
        "job_type": "permuter_search",
        "manifest_sha": manifest_sha,
        "toolkit_sha": toolkit_sha,
        "batch": False,          # searches are stochastic: never cache-hit
        "max_attempts": None,    # progress is checkpointed; re-issue freely
    }
    return bundle_path, manifest_sha, job
