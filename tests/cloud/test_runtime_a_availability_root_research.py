"""One inexpensive stock/GNU replay, input checks before optional IDO skip."""
import importlib.util
import json
from pathlib import Path
import shutil
import pytest
ROOT=Path(__file__).resolve().parents[2]
PACKET=ROOT/'cloud/work/frontier/dot_runtime_a_availability_root_20261012'

def test_availability_root_research():
    spec=importlib.util.spec_from_file_location('availability_root_verify_test',PACKET/'verify.py')
    v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)
    receipt=json.loads((PACKET/'verification.json').read_text());before=v.score.ASM_DIR
    assert v.inputs()==receipt['inputs'] and v.score.ASM_DIR==before
    assert receipt['claims']==[] and not receipt['installation_proposed']
    assert receipt['new_individually_strict_bytes']==1308
    assert (receipt['new_installable_bytes'],receipt['accepted_image_bytes'],receipt['rom_bytes'])==(0,0,0)
    assert receipt['results']['candidate']['gnu']['owned_data_bytes']==52
    assert receipt['results']['candidate']['callee_comparison']['differing']==0
    missing=[n for n in ('mips-linux-gnu-as','mips-linux-gnu-ld') if not shutil.which(n)]
    if missing:pytest.skip('MIPS binutils missing: '+', '.join(missing))
    assert v.verify()==receipt and v.score.ASM_DIR==before
