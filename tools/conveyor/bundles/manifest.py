"""Work-unit manifest: the canonical description of one job.

`manifest_sha` (sha256 of the canonical JSON) is the identity of a batch job's
work — the coordinator uses it as the result cache key (FR-006), so
serialization must be stable: sorted keys, no whitespace variance.
"""
import hashlib
import json

JOB_TYPES = (
    "compile_score",
    "flag_sweep",
    "cluster_score",
    "permuter_search",
    "verify_promote",
)

# Batch job types are deterministic pure functions of their manifest.
BATCH_TYPES = ("compile_score", "flag_sweep", "cluster_score")

_REQUIRED = {
    "compile_score": ("cells",),
    "flag_sweep": ("tu", "flagsets", "functions"),
    "cluster_score": ("mode", "targets"),
    "permuter_search": ("target_id", "seed_file", "target_file", "compile_flags"),
    "verify_promote": ("target_id", "source_sha", "search_job_id"),
}


def canonical_json(obj):
    return json.dumps(obj, sort_keys=True, separators=(",", ":"), ensure_ascii=True)


def manifest_sha(manifest):
    return hashlib.sha256(canonical_json(manifest).encode("ascii")).hexdigest()


def validate(manifest):
    """Raise ValueError on a malformed manifest; return it unchanged if valid."""
    if not isinstance(manifest, dict):
        raise ValueError("manifest must be an object")
    job_type = manifest.get("job_type")
    if job_type not in JOB_TYPES:
        raise ValueError(f"unknown job_type: {job_type!r}")
    if not manifest.get("toolkit_sha") and job_type != "verify_promote":
        raise ValueError("manifest missing toolkit_sha")
    for field in _REQUIRED[job_type]:
        if field not in manifest:
            raise ValueError(f"{job_type} manifest missing field {field!r}")
    budget = manifest.get("budget")
    if budget is not None:
        if not isinstance(budget, dict):
            raise ValueError("budget must be an object")
        for key in budget:
            if key not in ("wall_seconds", "iterations"):
                raise ValueError(f"unknown budget field {key!r}")
    if job_type == "permuter_search" and budget is None:
        raise ValueError("permuter_search requires a budget (never unbounded)")
    return manifest


def is_batch(manifest):
    return manifest["job_type"] in BATCH_TYPES
