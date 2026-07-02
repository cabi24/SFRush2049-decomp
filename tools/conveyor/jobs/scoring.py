"""Scoring wrapper: one canonical similarity metric for the whole pipeline.

Wraps the decomp-permuter Scorer (score 0 = byte-identical assembly) using the
toolkit-bundled mips objdump. Works both inside a toolkit on a node
($CONVEYOR_TOOLKIT set) and in the repo (vendored permuter + system objdump).
"""
import os
import shutil
import sys
from pathlib import Path

_HERE = Path(__file__).resolve().parent
if str(_HERE) not in sys.path:
    sys.path.insert(0, str(_HERE))

import _permuter  # noqa: E402

Scorer = _permuter.Scorer


# A custom objdump_command replaces the permuter's executable AND arguments,
# so the MIPS disassembly flags must be restated here.
_MIPS_ARGS = "-drz -m mips:4300"


def objdump_command():
    toolkit = os.environ.get("CONVEYOR_TOOLKIT")
    if toolkit:
        bundled = Path(toolkit) / "bin" / "objdump"
        if bundled.is_file():
            lib = Path(toolkit) / "lib"
            if lib.is_dir():
                # Bundled shared libs travel with the binary.
                current = os.environ.get("LD_LIBRARY_PATH", "")
                os.environ["LD_LIBRARY_PATH"] = f"{lib}:{current}" if current else str(lib)
            return f"{bundled} {_MIPS_ARGS}"
    for name in ("mips-linux-gnu-objdump", "mips64-elf-objdump", "mips-elf-objdump"):
        found = shutil.which(name)
        if found:
            return f"{found} {_MIPS_ARGS}"
    raise RuntimeError("no mips objdump available (toolkit or system)")


_scorer_cache = {}


def get_scorer(target_o):
    """Scorer for a target object file, cached per path."""
    key = str(target_o)
    if key not in _scorer_cache:
        _scorer_cache[key] = Scorer(
            target_o=key,
            stack_differences=False,
            algorithm="difflib",
            debug_mode=False,
            ign_branch_targets=True,
            objdump_command=objdump_command(),
        )
    return _scorer_cache[key]


def score(target_o, cand_o):
    """Score a candidate object against a target object. 0 = identical."""
    result, _ = get_scorer(target_o).score(str(cand_o) if cand_o else None)
    return result


PENALTY_INF = Scorer.PENALTY_INF
