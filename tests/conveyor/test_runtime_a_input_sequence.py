"""One bounded protocol-v2 replay; shared conftest enforces toolchain skips."""
import importlib.util
import json
from pathlib import Path
import shutil

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/frontier/dot_runtime_a_input_sequence_20261012'


def test_input_sequence_packet():
    spec = importlib.util.spec_from_file_location('input_sequence_packet_verify', PACKET / 'verify.py')
    verifier = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(verifier)
    receipt = json.loads((PACKET / 'verification.json').read_text())
    before = verifier.score.ASM_DIR
    assert verifier.inputs() == receipt['inputs']
    assert verifier.score.ASM_DIR == before
    assert receipt['inputs']['claims'] == ['func_8038F648']
    assert receipt['claimed_bytes'] == receipt['new_claim_bytes'] == 236
    assert receipt['accepted_image_bytes'] == 0
    assert receipt['behavior']['instructions'] == 59
    missing = [tool for tool in ('mips-linux-gnu-ld', 'gcc') if not shutil.which(tool)]
    if missing:
        pytest.skip('MIPS binutils/C compiler missing: ' + ', '.join(missing))
    assert verifier.verify() == receipt
    assert verifier.score.ASM_DIR == before
