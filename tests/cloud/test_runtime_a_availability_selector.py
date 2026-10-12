"""One compact stock/GNU replay; no production writes or private inputs."""
import importlib.util
import json
from pathlib import Path
import shutil

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/frontier/dot_runtime_a_availability_selector_20261012'


def test_availability_selector_packet():
    spec = importlib.util.spec_from_file_location('availability_selector_verify', PACKET / 'verify.py')
    verifier = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(verifier)
    receipt = json.loads((PACKET / 'verification.json').read_text())
    before = verifier.score.ASM_DIR
    assert verifier.inputs() == receipt['inputs']
    assert verifier.score.ASM_DIR == before
    assert receipt['new_claim_bytes'] == 720
    assert receipt['preserved_bytes'] == 400
    assert receipt['accepted_image_bytes'] == 0
    missing = [tool for tool in ('mips-linux-gnu-as', 'mips-linux-gnu-ld') if not shutil.which(tool)]
    if missing:
        pytest.skip('MIPS binutils missing: ' + ', '.join(missing))
    assert verifier.verify() == receipt
    assert verifier.score.ASM_DIR == before
