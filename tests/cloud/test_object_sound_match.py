"""Object half-turn/sound callback: exact-byte and bounded semantic evidence."""
import importlib.util
import json
import os
from pathlib import Path
import shutil
import struct
import sys
import pytest

ROOT=Path(__file__).resolve().parents[2]
HERE=ROOT/'cloud/work/ipa-groups/dot_object_sound_20261005'
sys.path.insert(0,str(ROOT))
from tools.cloud import score
spec=importlib.util.spec_from_file_location('object_sound_verify',HERE/'verify.py')
verify=importlib.util.module_from_spec(spec);spec.loader.exec_module(verify)

@pytest.fixture(scope='module')
def receipt():return json.loads((HERE/'verification.json').read_text())

def machine(words=None):
    names=['func_8010DBB8','func_80090284','func_80090E9C'];targets=score.targets()
    bodies={n:targets[n] for n in names}
    if words is not None:bodies[names[0]]=words
    own=score.own_data()
    return verify.semantics.native.Machine(score.image_symbols(),bodies,
        {0x801249c4:own.read(0x801249c4,8),0x801239d0:own.read(0x801239d0,8)})

def test_receipt_binds_all_sources(receipt):
    assert receipt['candidate_bytes']==324 and receipt['accepted_byte_gain']==0
    for name,digest in receipt['packet_sha256'].items():assert verify.sha((HERE/name).read_bytes())==digest
    for name,digest in receipt['context_sources'].items():assert verify.sha(verify.context_path(name).read_bytes())==digest
    assert receipt['gnu_link']['claimed_body_sha256']==verify.sha(struct.pack('>81I',*score.targets()[verify.FN]))
    assert verify.complete(receipt['object'])
    assert len(receipt['genuine_context'])==10 and all(verify.complete(r) for r in receipt['genuine_context'].values())

def test_only_full_callback_is_claimed(receipt):
    recipe=json.loads((HERE/'group.json').read_text())
    assert recipe['claims']==recipe['members']==recipe['keep']==[verify.FN]
    assert receipt['unclaimed_object_text']=={'deleted_donor_helper_bytes_before_candidate':8,
        'zero_alignment_bytes_after_candidate':4,'donor_stub_is_not_a_native_name_or_coverage_claim':True}
    assert receipt['owned_data']['verified_bytes']==8 and len(receipt['relocations'])==15

def test_controls_reject_weaker_evidence(receipt):
    controls=receipt['source_controls']
    assert controls['archived_b10']['differing']==14
    assert controls['collapsed_node_carriers']['differing']==10
    assert controls['expanded_dot_expression']['verdict']!='MATCH'
    assert controls['wrong_literal']['differing']==0 and not verify.complete(controls['wrong_literal'])

def test_native_null_allocation_preserves_nonstate_fields():
    sem=verify.semantics;case=(0,7,15,255,sem.IDENTITY+(1.,2.,3.)+(4.,5.,6.))
    out,calls,reads,writes,regions=machine().run(case)
    assert out==sem.oracle(case)[0] and calls==[] and out[0:2]==[7,255]
    assert len([w for w in writes if w[0]==sem.native.ACTOR+90])==1

def test_direction_boundary_is_strict():
    sem=verify.semantics;m=machine()
    for speed in (-1.,-0.,0.,1e-30,1.):
        case=(1,0,0,255,sem.IDENTITY+(0.,0.,speed)+(0.,0.,0.))
        expected,rotate=sem.oracle(case);out,calls,*_=m.run(case)
        assert out==expected
        assert [c[0] for c in calls]==(['sin','cos','sound'] if rotate else ['sound'])
        assert rotate==(speed>0)

def test_native_rejects_unknown_instruction():
    words=list(score.targets()[verify.FN]);words[0]=0xfc000000
    with pytest.raises(AssertionError,match='opcode'):
        machine(words).run(verify.semantics.cases()[0])

def test_host_native_coverage_and_mutations_recorded(receipt):
    behavior=receipt['behavior']
    assert behavior['cases']==4424 and behavior['rotation_cases']==1816
    assert behavior['executed_instruction_offsets'][verify.FN]==list(range(0,324,4))
    assert len(behavior['rejected_source_mutants'])==6
    assert behavior['host_ubsan'] and behavior['all_native_GNU_reads_writes_calls_memory_equal']

@pytest.mark.skipif(not Path(os.environ.get('IDO_DIR',ROOT/'tools/cloud/ido'),'cc').exists() or not shutil.which('mips-linux-gnu-ld'),reason='Pinned IDO and MIPS GNU linker required')
def test_fresh_compiler_and_behavior_replay(tmp_path,receipt):
    current=verify.verify(tmp_path)
    for name in ('object','gnu_link','owned_data','source_controls','genuine_context','behavior'):
        assert current[name]==receipt[name]
