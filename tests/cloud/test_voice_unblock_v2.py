"""One cheap, input-bound packet replay; shared conftest handles missing IDO."""
from pathlib import Path
import subprocess
import sys

import pytest

ROOT = Path(__file__).resolve().parents[2]
VERIFY = ROOT / 'cloud/work/boot_tail_promotion/voice_unblock_v2/verify.py'


def test_voice_unblock_v2():
    # Always validate sources/targets before a missing compiler can skip replay.
    inputs = subprocess.run([sys.executable, str(VERIFY), '--check', '--inputs-only'],
                            cwd=ROOT, capture_output=True, text=True)
    assert inputs.returncode == 0, inputs.stdout + inputs.stderr
    result = subprocess.run([sys.executable, str(VERIFY), '--check'],
                            cwd=ROOT, capture_output=True, text=True, timeout=25)
    output = result.stdout + result.stderr
    if ' absent; pinned IDO and MIPS binutils/C compiler required' in output:
        pytest.skip(output)
    assert result.returncode == 0, output
