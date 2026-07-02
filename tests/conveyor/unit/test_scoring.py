"""Scoring wrapper test with real MIPS objects assembled on the fly."""
import shutil
import subprocess

import pytest

AS = shutil.which("mips-linux-gnu-as")
pytestmark = pytest.mark.skipif(AS is None, reason="mips-linux-gnu-as not installed")

ASM_A = """
.set noreorder
.globl func
func:
    addiu $sp, $sp, -8
    sw    $ra, 0($sp)
    move  $v0, $a0
    lw    $ra, 0($sp)
    jr    $ra
    addiu $sp, $sp, 8
"""

# Same shape, one different instruction (v0 <- a1 instead of a0).
ASM_B = ASM_A.replace("move  $v0, $a0", "move  $v0, $a1")


def _assemble(tmp_path, name, text):
    src = tmp_path / f"{name}.s"
    obj = tmp_path / f"{name}.o"
    src.write_text(text)
    subprocess.run(
        [AS, "-march=vr4300", "-mabi=32", "-o", str(obj), str(src)], check=True
    )
    return obj


def test_identical_scores_zero_and_diff_scores_positive(tmp_path):
    from tools.conveyor.jobs import scoring

    target = _assemble(tmp_path, "target", ASM_A)
    same = _assemble(tmp_path, "same", ASM_A)
    diff = _assemble(tmp_path, "diff", ASM_B)

    assert scoring.score(target, same) == 0
    assert scoring.score(target, diff) > 0
    assert scoring.score(target, None) == scoring.PENALTY_INF
