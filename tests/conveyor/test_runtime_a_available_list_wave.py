"""Bounded image-A list-builder regression without relaxing production guards."""
import importlib.util
import os
from pathlib import Path
import shutil
import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT/'cloud/work/frontier/dot_runtime_a_available_list_20261011'
spec=importlib.util.spec_from_file_location('available_list_wave',PACKET/'verify.py')
verify=importlib.util.module_from_spec(spec)
spec.loader.exec_module(verify)


def test_source_delta_and_claim_scope():
    original=(ROOT/verify.BASELINE/'group.c').read_text()
    assert (PACKET/'group.c').read_text()==verify.expected_source(original)
    import json
    cfg=json.loads((PACKET/'group.json').read_text())
    assert cfg['claims']==['func_8039E3BC']
    assert cfg['keep']==['func_803A1EAC']
    assert cfg['flags']=='-g0 -O3 -mips2 -G 0 -non_shared'


def test_failure_guard_is_optimization_safe():
    with pytest.raises(ValueError,match='test guard'):
        verify.require(False,'test guard')


def test_compile_full_body_and_negative_controls():
    ido=Path(os.environ.get('IDO_DIR',ROOT/'tools/cloud/ido'))/'cc'
    if not ido.is_file():
        if os.environ.get('REQUIRE_TOOLCHAIN')=='1':
            pytest.fail('required IDO toolchain missing')
        pytest.skip('IDO toolchain unavailable')
    import sys
    sys.path.insert(0,str(ROOT/"tools/cloud"))
    import score as shared_score
    image_before=shared_score.ASM_DIR
    receipt=verify.verify()
    assert shared_score.ASM_DIR==image_before
    assert receipt['new_candidate_bytes']==136
    assert receipt['accepted_bytes']==0
    assert receipt['scores']['func_8039E3BC']['differing']==0
    assert receipt['negative_controls']['wrong_loop_bound']['differing']==1


def test_independent_reviewer_rejects_optimized_python():
    import subprocess
    import sys
    result=subprocess.run([sys.executable,'-O',str(PACKET/'independent_review.py')],
                          capture_output=True,text=True)
    assert result.returncode != 0
    assert 'Run without Python -O' in result.stderr


def test_independent_full_body_and_paired_semantics():
    import subprocess
    import sys
    ido=Path(os.environ.get('IDO_DIR',ROOT/'tools/cloud/ido'))/'cc'
    needed=('gcc','mips-linux-gnu-as','mips-linux-gnu-ld')
    if not ido.is_file() or any(not shutil.which(tool) for tool in needed):
        if os.environ.get('REQUIRE_TOOLCHAIN')=='1':
            pytest.fail('required independent verification toolchain missing')
        pytest.skip('independent verification toolchain unavailable')
    result=subprocess.run([sys.executable,str(PACKET/'independent_review.py')],
                          capture_output=True,text=True)
    assert result.returncode==0,result.stdout+'\n'+result.stderr
