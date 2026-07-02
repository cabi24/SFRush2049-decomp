"""verify_promote executor: the one stateful job, pinned to the builder node.

Manifest:
    {
      "job_type": "verify_promote",
      "target_id": "...", "source_sha": "...", "search_job_id": "...",
      "candidate_id": "game/game.c:game",     # provenance for the doc header
      "compile_flags": "-g0 -O2 ...",
      "score_history": "base 143 -> 0",
    }
Inputs: promoted.c, target.o.

Runs on the builder (agent started with --repo): re-verifies byte-identity
with toolkit IDO, injects the Constitution-V doc header, writes the source
into the function's work/ directory in the canonical repo clone, runs the
full-build gate, and commits — or rolls back and reports.

NOTE (V1 scope): promotion lands in work/<...>/<target>/matched.c and STATUS,
not yet spliced into a src/*.c translation unit — TU splicing needs the
function->file layout map, tracked for a later phase. The ROM-hash gate is
still run to guarantee the repo stays green.
"""
import datetime
import os
import subprocess
import sys
import tempfile
from pathlib import Path

_HERE = Path(__file__).resolve().parent
if str(_HERE) not in sys.path:
    sys.path.insert(0, str(_HERE))

import compile_score  # noqa: E402
import scoring  # noqa: E402

BUILD_CMD = ["make", "test"]
BUILD_TIMEOUT = 1800


def _doc_header(manifest):
    today = datetime.date.today().isoformat()
    return (
        "/*\n"
        f" * MATCHED by conveyor on {today} (Constitution V)\n"
        f" * Arcade equivalent: {manifest.get('candidate_id', 'unknown')}\n"
        f" * Compiler flags:    {manifest['compile_flags']}\n"
        f" * Score history:     {manifest.get('score_history', 'n/a')}\n"
        f" * Search job:        {manifest.get('search_job_id', 'n/a')}\n"
        " */\n"
    )


def _find_work_dir(repo, target_id):
    matches = [p for p in (repo / "work").rglob(target_id) if p.is_dir()]
    return matches[0] if matches else None


def run(job_dir, manifest, progress):
    job_dir = Path(job_dir)
    inputs = job_dir / "inputs"
    repo = os.environ.get("CONVEYOR_REPO")
    if not repo:
        raise RuntimeError("verify_promote requires a builder node (--repo)")
    repo = Path(repo)
    target_id = manifest["target_id"]
    payload = {"target_id": target_id, "build_ok": False, "sha1_ok": False,
               "commit_hash": None, "doc_header_injected": False,
               "outcome": None}

    # 1. Independent byte-identity check (never trust the search's claim).
    progress.update(stage="verify")
    with tempfile.TemporaryDirectory() as tmp:
        out_o = Path(tmp) / "v.o"
        ok, message = compile_score.compile_one(
            inputs / "promoted.c", manifest["compile_flags"], out_o,
            include_dirs=[Path(os.environ["CONVEYOR_TOOLKIT"]) / "shim"],
        )
        if not ok:
            payload["outcome"] = f"rolled_back:recompile_failed:{message}"
            return payload, {}
        value = scoring.score(inputs / "target.o", out_o)
        if value != 0:
            payload["outcome"] = f"rolled_back:score_{value}_not_zero"
            return payload, {}

    # 2. Write into the canonical repo (work dir for this function).
    work_dir = _find_work_dir(repo, target_id)
    if work_dir is None:
        payload["outcome"] = "rolled_back:no_work_dir"
        return payload, {}
    matched_c = work_dir / "matched.c"
    status_file = work_dir / "STATUS"
    written = [matched_c, status_file]
    try:
        matched_c.write_text(
            _doc_header(manifest) + (inputs / "promoted.c").read_text()
        )
        payload["doc_header_injected"] = True
        status_file.write_text("MATCHED\n")

        # 3. Full-build gate (SHA-1 verification via `make test`).
        progress.update(stage="build_gate")
        proc = subprocess.run(
            BUILD_CMD, cwd=str(repo), capture_output=True, text=True,
            timeout=BUILD_TIMEOUT,
        )
        payload["build_ok"] = proc.returncode == 0
        payload["sha1_ok"] = proc.returncode == 0
        if proc.returncode != 0:
            raise RuntimeError(f"build gate failed: {proc.stdout[-300:]}")

        # 4. Commit.
        progress.update(stage="commit")
        rels = [str(p.relative_to(repo)) for p in written]
        subprocess.run(["git", "add", *rels], cwd=str(repo), check=True)
        proc = subprocess.run(
            ["git", "commit", "-m",
             f"conveyor: match {target_id} (score 0)\n\n"
             f"Arcade equivalent: {manifest.get('candidate_id', 'unknown')}\n\n"
             "Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>"],
            cwd=str(repo), capture_output=True, text=True, check=True,
        )
        commit = subprocess.run(
            ["git", "rev-parse", "HEAD"], cwd=str(repo),
            capture_output=True, text=True, check=True,
        ).stdout.strip()
        payload["commit_hash"] = commit
        payload["outcome"] = "promoted"
        return payload, {}
    except (RuntimeError, subprocess.CalledProcessError, subprocess.TimeoutExpired) as exc:
        subprocess.run(
            ["git", "checkout", "--", "."], cwd=str(repo / "work"),
            capture_output=True,
        )
        for p in written:
            if p.name == "matched.c" and p.exists():
                p.unlink()
        if payload["outcome"] is None:
            payload["outcome"] = f"rolled_back:{exc}"[:200]
        return payload, {}
