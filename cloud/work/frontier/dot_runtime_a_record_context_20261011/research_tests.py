"""Image-qualified record navigation with real caller context and no guard relaxation."""
import importlib.util
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import pytest

ROOT=Path(__file__).resolve().parents[4]
PACKET=ROOT/'cloud/work/frontier/dot_runtime_a_record_context_20261011'
spec=importlib.util.spec_from_file_location('record_context_verify',PACKET/'verify.py')
verify=importlib.util.module_from_spec(spec);spec.loader.exec_module(verify)


def test_source_delta_and_claim_scope():
    assert (PACKET/'group.c').read_text()==verify.expected_source((verify.BASELINE/'group.c').read_text())
    config=json.loads((PACKET/'group.json').read_text())
    assert config['claims']==['func_8039E200']
    assert config['keep']==['func_803A1EAC']
    assert len(config['members'])==4 and len(config['context'])==8


def test_failure_guard():
    with pytest.raises(ValueError,match='test guard'):verify.require(False,'test guard')


def test_reject_optimized_python():
    result=subprocess.run([sys.executable,'-O',str(PACKET/'verify.py')],capture_output=True,text=True)
    assert result.returncode!=0
    assert 'Run without Python -O' in result.stderr


def test_complete_compiler_native_GNU_and_host_proof():
    ido=Path(os.environ.get('IDO_DIR',ROOT/'tools/cloud/ido'))/'cc'
    required=('gcc','mips-linux-gnu-as','mips-linux-gnu-ld')
    if not ido.is_file() or any(not shutil.which(tool) for tool in required):
        if os.environ.get('REQUIRE_TOOLCHAIN')=='1':pytest.fail('required toolchain missing')
        pytest.skip('toolchain unavailable')
    import score as shared_score
    original_image=shared_score.ASM_DIR
    receipt=verify.verify()
    assert shared_score.ASM_DIR==original_image
    assert receipt['status']=='PASS'
    assert receipt['new_candidate_bytes']==444
    assert receipt['accepted_image_bytes']==receipt['cartridge_bytes']==0
    assert receipt['scores']['func_8039E200']['differing']==0
    assert receipt['scores']['func_8039D05C']['differing']==18
    assert receipt['semantics']['instructions_covered']==111
    assert receipt['semantics']['cases_per_native_and_GNU_route']==6546
    assert receipt['MIPS_layouts']['RecordList first']==8
    assert receipt['MIPS_layouts']['RecordList last']==12
    assert receipt['negative_controls']['cached_file_access']['differing']==1


def test_model_zero_direction_and_callback_rewrite():
    semantics=verify.semantics
    records=[(3,2,0,0,1,0) for _ in range(9)]
    records[6]=(3,2,1,0,1,2)
    case=(0,-1,0,3,2,(-1,-1,-1),records)
    assert semantics.oracle(case)==(-1,[],[])
    case=(0,6,0,3,2,(-1,-1,-1),records)
    assert semantics.oracle(case)==(6,[6],[6])


def test_decoder_rejects_unmapped_memory_and_unknown_opcode():
    semantics=verify.semantics
    with pytest.raises(ValueError,match='unmapped read'):semantics.read({},0,4)
    case=next(semantics.cases())
    with pytest.raises(ValueError,match='unknown opcode'):semantics.run([0xfc000000],case,0)
