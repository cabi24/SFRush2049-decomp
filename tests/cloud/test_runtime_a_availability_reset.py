"""One cheap input-bound stock/GNU replay for the player-state reset group."""
import importlib.util
import json
from pathlib import Path
import shutil
import pytest
ROOT=Path(__file__).resolve().parents[2]
PACKET=ROOT/'cloud/work/frontier/dot_runtime_a_availability_reset_20261012'

def test_availability_reset_packet():
    spec=importlib.util.spec_from_file_location('availability_reset_verify',PACKET/'verify.py')
    v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)
    receipt=json.loads((PACKET/'verification.json').read_text());before=v.score.ASM_DIR
    assert v.inputs()==receipt['inputs']
    assert v.score.ASM_DIR==before
    assert (receipt['new_claim_bytes'],receipt['preserved_bytes'],receipt['accepted_image_bytes'])==(728,1120,0)
    missing=[x for x in ('mips-linux-gnu-as','mips-linux-gnu-ld') if not shutil.which(x)]
    if missing:pytest.skip('MIPS binutils missing: '+', '.join(missing))
    assert v.verify()==receipt
    assert v.score.ASM_DIR==before
