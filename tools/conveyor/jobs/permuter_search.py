"""permuter_search executor: drive decomp-permuter on one seed with a budget.

Manifest:
    {
      "job_type": "permuter_search",
      "toolkit_sha": "...",
      "target_id": "func_800FD464",
      "seed_file": "base.c",            # under inputs/
      "target_file": "target.o",        # under inputs/
      "compile_flags": "-g0 -O2 -mips2 -G 0 -non_shared",
      "budget": {"wall_seconds": 14400, "iterations": null}
    }

Runs the vendored permuter CLI as a subprocess (--stop-on-zero --best-only),
polls its output-<score>-<n>/ dirs, and streams every improvement into
progress.json so the agent's heartbeats persist it (<=30 s loss on node
death). Returns the best source found as an artifact.
"""
import base64
import gzip
import os
import re
import shutil
import subprocess
import sys
import tempfile
import time
from pathlib import Path

_HERE = Path(__file__).resolve().parent
if str(_HERE) not in sys.path:
    sys.path.insert(0, str(_HERE))

import scoring  # noqa: E402

POLL_SECONDS = 5
_OUTPUT_RE = re.compile(r"^output-(\d+)-\d+$")


def _toolkit():
    toolkit = os.environ.get("CONVEYOR_TOOLKIT")
    if not toolkit:
        raise RuntimeError("CONVEYOR_TOOLKIT not set")
    return Path(toolkit)


def _write_compile_sh(perm_dir, flags):
    toolkit = _toolkit()
    script = perm_dir / "compile.sh"
    script.write_text(
        "#!/bin/sh\n"
        f'exec "{toolkit}/ido/cc" -c {flags} -I "{toolkit}/shim" "$@"\n'
    )
    script.chmod(0o755)
    # The permuter's Scorer takes a custom objdump only via settings.toml;
    # without it, it searches PATH and dies on nodes that (correctly) have no
    # system mips objdump. Route it to the toolkit-bundled one.
    (perm_dir / "settings.toml").write_text(
        f'objdump_command = "{scoring.objdump_command()}"\n'
    )


def _best_output(perm_dir):
    """(score, source_path) of the best output-*/ dir, or (None, None)."""
    best_score, best_path = None, None
    for entry in perm_dir.iterdir():
        m = _OUTPUT_RE.match(entry.name)
        if not m:
            continue
        score = int(m.group(1))
        if best_score is None or score < best_score:
            source = entry / "source.c"
            if source.is_file():
                best_score, best_path = score, source
    return best_score, best_path


def _gz_b64(path):
    return base64.b64encode(gzip.compress(Path(path).read_bytes())).decode()


def run(job_dir, manifest, progress):
    job_dir = Path(job_dir)
    inputs = job_dir / "inputs"
    toolkit = _toolkit()
    budget = manifest["budget"] or {}
    wall_budget = budget.get("wall_seconds") or 4 * 3600
    cores = int(os.environ.get("CONVEYOR_CORES", "1"))

    perm_dir = Path(tempfile.mkdtemp(prefix="permjob-"))
    try:
        shutil.copy(inputs / manifest["seed_file"], perm_dir / "base.c")
        shutil.copy(inputs / manifest["target_file"], perm_dir / "target.o")
        _write_compile_sh(perm_dir, manifest["compile_flags"])

        # Baseline: compile the untouched seed and score it, so the farm knows
        # the starting point even if the permuter never improves on it.
        base_score = None
        with tempfile.NamedTemporaryFile(suffix=".o") as base_o:
            proc = subprocess.run(
                [str(perm_dir / "compile.sh"), str(perm_dir / "base.c"),
                 "-o", base_o.name],
                capture_output=True, text=True, timeout=120,
            )
            if proc.returncode == 0:
                base_score = scoring.score(perm_dir / "target.o", base_o.name)
        if base_score is None:
            return {
                "target_id": manifest["target_id"],
                "final_best_score": None,
                "base_score": None,
                "error": "seed does not compile",
            }, {}
        progress.update(best_score=base_score,
                        best_source=_gz_b64(perm_dir / "base.c"))
        if base_score == 0:
            return {
                "target_id": manifest["target_id"],
                "final_best_score": 0,
                "base_score": 0,
                "wall_seconds_used": 0,
            }, {"best.c": str(perm_dir / "base.c")}

        permuter_py = toolkit / "decomp-permuter" / "permuter.py"
        # PYTHONPATH: toolkit root carries the permuter's pure-Python deps
        # (pycparser, toml) so stdlib-only nodes can run it.
        env = dict(os.environ)
        env["PYTHONPATH"] = str(toolkit) + (
            os.pathsep + env["PYTHONPATH"] if env.get("PYTHONPATH") else ""
        )
        stderr_log = perm_dir / "permuter-stderr.log"
        proc = subprocess.Popen(
            [sys.executable, str(permuter_py), str(perm_dir),
             "--stop-on-zero", "--best-only", "-j", str(cores)],
            stdout=subprocess.DEVNULL, stderr=open(stderr_log, "wb"),
            cwd=str(perm_dir), env=env,
        )
        started = time.monotonic()
        we_stopped_it = False
        best_reported = base_score
        while proc.poll() is None:
            if time.monotonic() - started > wall_budget:
                we_stopped_it = True
                proc.terminate()
                try:
                    proc.wait(timeout=30)
                except subprocess.TimeoutExpired:
                    proc.kill()
                break
            time.sleep(POLL_SECONDS)
            score, source = _best_output(perm_dir)
            if score is not None and score < best_reported:
                best_reported = score
                progress.update(best_score=score, best_source=_gz_b64(source))
                if score == 0:
                    we_stopped_it = True
                    break
        if proc.poll() is None:
            proc.kill()
            proc.wait(timeout=30)  # reap; no zombies for the agent's lifetime

        final_score, final_source = _best_output(perm_dir)
        # An early nonzero exit with no output is an infrastructure failure,
        # not a "search that found nothing" — surface it as a job error so
        # the farm doesn't mislabel the target as stalled.
        if (not we_stopped_it and proc.returncode not in (0, None)
                and final_score is None):
            tail = ""
            if stderr_log.is_file():
                tail = stderr_log.read_bytes()[-2000:].decode(errors="replace")
            raise RuntimeError(
                f"permuter exited {proc.returncode} without output: {tail}"
            )
        if final_score is None or final_score >= base_score:
            final_score, final_source = base_score, perm_dir / "base.c"
        # Copy out before perm_dir cleanup.
        keep = Path(tempfile.mkdtemp(prefix="permbest-")) / "best.c"
        shutil.copy(final_source, keep)
        return {
            "target_id": manifest["target_id"],
            "final_best_score": final_score,
            "base_score": base_score,
            "wall_seconds_used": round(time.monotonic() - started, 1),
        }, {"best.c": str(keep)}
    finally:
        shutil.rmtree(perm_dir, ignore_errors=True)
