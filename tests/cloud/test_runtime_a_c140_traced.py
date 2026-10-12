"""One cheap source-bound proof; no production installation is performed."""
import importlib.util
import json
from pathlib import Path
import shutil

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/frontier/dot_runtime_a_c140_traced_20261012'


def test_c140_strict_research_packet():
    spec = importlib.util.spec_from_file_location('c140_packet_verify', PACKET / 'verify.py')
    verifier = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(verifier)
    receipt = json.loads((PACKET / 'verification.json').read_text())
    before = verifier.score.ASM_DIR
    assert verifier.inputs() == receipt['inputs']
    assert verifier.score.ASM_DIR == before
    assert receipt['gross_claim_count'] == 10 and receipt['gross_claim_bytes'] == 4864
    assert receipt['new_claim_count'] == 1 and receipt['new_claim_bytes'] == 1984
    assert receipt['accepted_image_bytes'] == 0
    missing = [tool for tool in ('mips-linux-gnu-as', 'mips-linux-gnu-ld') if not shutil.which(tool)]
    if missing:
        pytest.skip('MIPS binutils missing: ' + ', '.join(missing))
    assert verifier.verify() == receipt
    assert verifier.score.ASM_DIR == before
