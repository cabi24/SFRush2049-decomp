"""Locate the vendored decomp-permuter and re-export its Scorer.

Resolution order:
1. $CONVEYOR_TOOLKIT/decomp-permuter — inside an unpacked toolkit bundle on a node
2. <repo>/tools/decomp-permuter      — the vendored checkout, for Pi-side use and tests
"""
import os
import sys
from pathlib import Path


def permuter_root():
    toolkit = os.environ.get("CONVEYOR_TOOLKIT")
    if toolkit:
        candidate = Path(toolkit) / "decomp-permuter"
        if candidate.is_dir():
            return candidate
    repo_candidate = Path(__file__).resolve().parents[2] / "decomp-permuter"
    if repo_candidate.is_dir():
        return repo_candidate
    raise RuntimeError(
        "decomp-permuter not found (checked $CONVEYOR_TOOLKIT and tools/decomp-permuter)"
    )


_root = permuter_root()
if str(_root) not in sys.path:
    sys.path.insert(0, str(_root))

from src.scorer import Scorer  # noqa: E402

__all__ = ["Scorer", "permuter_root"]
