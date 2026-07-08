"""compile_score executor: batch cells of (candidate × flagset × targets).

Manifest shape:
    {
      "job_type": "compile_score",
      "toolkit_sha": "...",
      "include_dirs": ["include"],            # optional, relative to inputs/
      "cells": [
        {"candidate_id": "game.c:game",
         "source": "game_c_game.c",           # file under inputs/
         "flagset": "-g0 -O2 -mips2 -G 0 -non_shared",
         "targets": [{"target_id": "func_800FD464", "file": "func_800FD464.o"}]}
      ]
    }

Each cell compiles once, then scores against each listed target.
"""
import os
import shlex
import subprocess
import sys
import tempfile
from pathlib import Path

_HERE = Path(__file__).resolve().parent
if str(_HERE) not in sys.path:
    sys.path.insert(0, str(_HERE))

import scoring  # noqa: E402

COMPILE_TIMEOUT = 120


def _ido_cc():
    toolkit = os.environ.get("CONVEYOR_TOOLKIT")
    if toolkit:
        cc = Path(toolkit) / "ido" / "cc"
        if cc.is_file():
            return str(cc)
    raise RuntimeError("IDO cc not found in toolkit")


def compile_one(source, flagset, out_o, include_dirs=()):
    """Compile a C file with IDO. Returns (ok: bool, message: str)."""
    cc = _ido_cc()
    cmd = [cc, "-c"] + shlex.split(flagset)
    for inc in include_dirs:
        cmd += ["-I", str(inc)]
    cmd += ["-o", str(out_o), str(source)]
    try:
        proc = subprocess.run(
            cmd, capture_output=True, text=True, timeout=COMPILE_TIMEOUT
        )
    except subprocess.TimeoutExpired:
        return False, "timeout"
    if proc.returncode != 0 or not Path(out_o).is_file():
        # Keep the head: cfe reports each undefined identifier once, in
        # order of first occurrence, so the first errors carry the signal
        # the failure-clustering report (pipeline.matrix failures) needs.
        message = (proc.stderr or proc.stdout or "").strip()
        return False, message[:2000] or f"cc exited {proc.returncode}"
    return True, "ok"


def run(job_dir, manifest, progress):
    job_dir = Path(job_dir)
    inputs = job_dir / "inputs"
    include_dirs = [inputs / d for d in manifest.get("include_dirs", [])]
    toolkit = os.environ.get("CONVEYOR_TOOLKIT")
    if toolkit and (Path(toolkit) / "shim").is_dir():
        include_dirs.append(Path(toolkit) / "shim")

    cells_out = []
    total = len(manifest["cells"])
    with tempfile.TemporaryDirectory() as tmp:
        for i, cell in enumerate(manifest["cells"]):
            out_o = Path(tmp) / f"cell{i}.o"
            ok, message = compile_one(
                inputs / cell["source"], cell["flagset"], out_o, include_dirs
            )
            for target in cell["targets"]:
                # Echo the target object identity for attribution (003) when the
                # manifest carries it; absent for pre-003 manifests replayed
                # from cache, ingested as NULL. Present on ok and fail alike.
                attribution = (
                    {"target_o_sha": target["target_o_sha"]}
                    if "target_o_sha" in target else {}
                )
                if not ok:
                    cells_out.append(
                        {
                            "candidate_id": cell["candidate_id"],
                            "flagset": cell["flagset"],
                            "target_id": target["target_id"],
                            "score": None,
                            "compile": f"fail:{message}",
                            **attribution,
                        }
                    )
                    continue
                value = scoring.score(inputs / target["file"], out_o)
                cells_out.append(
                    {
                        "candidate_id": cell["candidate_id"],
                        "flagset": cell["flagset"],
                        "target_id": target["target_id"],
                        "score": value,
                        "score_reloc_blind": scoring.reloc_blind_score(
                            inputs / target["file"], out_o
                        ),
                        "compile": "ok",
                        **attribution,
                    }
                )
            progress.update(stage="compile_score", cells_done=i + 1, cells_total=total)
    return {"cells": cells_out}, {}
