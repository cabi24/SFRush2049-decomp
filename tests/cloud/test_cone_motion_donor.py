"""The donor packet remains a fully measured, reproducible NONMATCH."""
import importlib.util
import json
from pathlib import Path
import shutil
import struct

import pytest

ROOT=Path(__file__).resolve().parents[2]
PACKET=ROOT/'cloud/work/frontier/dot_cone_motion_donor_20261005'
spec=importlib.util.spec_from_file_location('cone_donor_verify',PACKET/'verify.py')
v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)

def _require_toolchain():
    if not (v.score.IDO/'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('pinned IDO and MIPS GNU linker required')

@pytest.fixture(scope='module')
def replay():
    _require_toolchain()
    return v.verify()

def test_fresh_source_bound_receipt(replay):
    assert replay==json.loads((PACKET/'verification.json').read_text())

def test_complete_body_and_remaining_stack_difference(replay):
    p=replay['standalone']
    assert replay['claims']==[]
    assert p['symbol_bytes']==p['native_bytes']==432
    assert p['differing']==17 and p['all_differences_stack_immediates']
    assert p['candidate_frame']==80 and p['native_frame']==104
    assert not any(p[n] for n in ('unresolved','unverified','errors','extra_words'))

def test_genuine_unchanged_dependency_context(replay):
    p=replay['context']['bodies']
    assert set(p)==set(v.CONTEXT+[v.FN])
    assert all(v.complete(p[n]) for n in v.CONTEXT)
    assert p[v.FN]['differing']==17

def test_semantics_and_adverse_controls(replay):
    p=replay['runtime']
    assert p['cases']==5978 and p['mips_executions']==11956
    assert p['instruction_coverage']==[108,108]
    assert len(p['source_mutants_rejected'])==5 and min(p['source_mutants_rejected'].values())>0
    assert p['decoder_unknown_and_stack_write_controls_rejected']
    assert p['stack_canaries_saved_registers_return_address_and_complete_nonstack_memory_checked']

def test_fixed_source_hypotheses(replay):
    p=replay['controls']
    assert p['archive']['differing']==72 and p['archive']['symbol_bytes']==412
    assert p['owned_literal_only']['differing']==68
    assert p['repeated_clock_only']['differing']==42
    assert p['without_descriptor_pointer']['differing']==38
    assert p['cached_clock']['differing']==68
    assert p['o2']['differing']==105
    assert len(p['o2']['unverified'])==2

def test_wrong_owned_literal_rejected(tmp_path):
    _require_toolchain()
    source=tmp_path/'wrong.c';obj=tmp_path/'wrong.o'
    source.write_text(v.SOURCE.read_text().replace('0.15f','0.2f'))
    v.score.compile_single(source,v.FLAGS,obj)
    with pytest.raises(AssertionError):v.linked_proof(obj,tmp_path)

def test_truncated_elf_symbol_rejected(tmp_path):
    _require_toolchain()
    obj=tmp_path/'candidate.o';v.score.compile_single(v.SOURCE,v.FLAGS,obj)
    data,secs=v.score._elf(obj);data=bytearray(data)
    for i,sec in enumerate(secs):
        if sec['type']!=2:continue
        for j,s in enumerate(v.score._symbol_table(bytes(data),secs,i)):
            if s['name']==v.FN and s['type']==2:
                struct.pack_into('>I',data,sec['off']+j*16+8,428)
    obj.write_bytes(data)
    with pytest.raises(AssertionError):v.linked_proof(obj,tmp_path)

def test_early_returns_and_timer_boundary():
    x=v.corpus()[0]
    assert v.oracle(x)[13:15]==[1,4]
    x[15]=1;x[16]=1
    assert v.oracle(x)[13]==0
    x[16]=0;x[9]=x[10];x[17]=0x2000;x[18]=0
    assert v.oracle(x)[13:18]==[4,1,2,3,4]
