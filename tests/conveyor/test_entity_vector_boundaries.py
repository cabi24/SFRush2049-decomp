"""Fail-closed source-bound checks for eight deliberately unclaimed controls."""
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shutil
import struct

import pytest

ROOT=Path(__file__).resolve().parents[2]
HERE=ROOT/'cloud/work/frontier/dot_entity_vector_boundaries_20261005'
spec=importlib.util.spec_from_file_location('entity_vector_proof',HERE/'verify.py')
v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)
RECEIPT=json.loads((HERE/'verification.json').read_text())

def test_source_binding_and_empty_claims():
    assert RECEIPT['status']=='NONMATCH' and RECEIPT['claims']==[]
    assert RECEIPT['accepted_byte_gain']==0
    for path,expected in RECEIPT['source_bindings'].items():
        assert hashlib.sha256((ROOT/path).read_bytes()).hexdigest()==expected,path
    assert json.loads((HERE/'claim.json').read_text())['claims']==[]

def test_native_metadata_and_call_contract():
    assert v.native_metadata()==RECEIPT['native_metadata']
    assert RECEIPT['range']==['0x800C69C0','0x800C6AA0']
    assert RECEIPT['native_metadata']['native_frame_bytes']==88

def test_fixed_source_boundaries_have_no_shaping_storage():
    controls=v.sources()
    assert len(controls)==5
    for src in controls.values():
        assert 'volatile' not in src and '__asm' not in src and '__inline' not in src
        assert src.count('Basis basis;')==1 and src.count('f32 difference[3];')==1
    assert controls['authentic_vecsub'].count('static void')==1
    assert controls['authentic_scale_add'].count('static void')==2
    assert controls['authentic_all_functions'].count('static void')==3

def test_whole_extent_and_excess_are_not_masked():
    c=RECEIPT['controls']
    expected={'archived_direct':(224,80,29),'authentic_vecsub':(224,96,29),
              'authentic_scale_add':(300,128,64),'authentic_all_functions':(300,144,64),
              'authentic_fmath_macros':(224,80,29)}
    for name,(extent,frame,diffs) in expected.items():
        r=c[name][v.FN]
        assert (r['elf_bytes'],r['stack_frame'],r['whole_body_differing_words'])==(extent,frame,diffs)
        assert r['extra_elf_words']==max(0,(extent-224)//4)
        assert r['all_full_body_relocations_gnu_verified'] and not r['full_symbol_equal']
        assert r['owned_data_bytes']==0 and len(r['relocations'])==2
    assert len(c)==8

def test_accepted_helpers_preserved_without_caller_match():
    for name,rows in RECEIPT['controls'].items():
        if name.endswith('_accepted_context'):
            assert not rows[v.FN]['full_symbol_equal']
            for fn,size in [('func_8008E098',32),('func_8008E0B8',140)]:
                assert rows[fn]['full_symbol_equal'] and rows[fn]['elf_bytes']==size

def test_bounded_behavior_and_wrong_contracts():
    b=RECEIPT['behavior']
    assert b['cases']==1920 and b['protected_native_runs']==1920
    assert b['gnu_linked_runs']==15360 and b['host_runs']==15360
    assert b['target_native_instructions_covered']==56 and b['target_branch_outcomes']==2
    assert b['normalizer_native_instructions_covered']==34
    assert all(r['rejected'] for r in b['negative_controls'].values())
    assert set(b['negative_controls'])=={'wrong_correction','reverse_delta','skip_normalization','unknown_opcode','wrong_call'}

@pytest.fixture(scope='module')
def replay(tmp_path_factory):
    missing=[p for p in ('cc','mips-linux-gnu-ld') if not shutil.which(p)]
    if not (v.score.IDO/'cc').exists():missing.append('IDO_DIR/cc')
    if missing:
        if os.environ.get('REQUIRE_TOOLCHAIN')=='1':pytest.fail('Required toolchain absent: '+', '.join(missing))
        pytest.skip('Toolchain absent: '+', '.join(missing))
    d=tmp_path_factory.mktemp('entity-vector-replay')
    return d,v.verify(d)

def test_fresh_eight_control_replay(replay):
    _,actual=replay
    assert actual==RECEIPT

def test_wrong_complete_extent_refused(replay,tmp_path):
    work,_=replay;original=work/'archived_direct/candidate.o'
    data,secs=v.score._elf(original);data=bytearray(data)
    for idx,sec in enumerate(secs):
        if sec['type']!=2:continue
        syms=v.score._symbol_table(data,secs,idx)
        for i,sym in enumerate(syms):
            if sym['name']==v.FN:
                struct.pack_into('>I',data,sec['off']+16*i+8,sym['size']+4)
    changed=tmp_path/'bad.o';changed.write_bytes(data)
    with pytest.raises(AssertionError):v.inspect(changed,v.FN,tmp_path/'proof')

def test_wrong_relocation_refused(replay,tmp_path):
    work,_=replay;data,secs=v.score._elf(work/'archived_direct/candidate.o');data=bytearray(data)
    rel=next(s for s in secs if s['type']==9 and s['info']==v.score._text_index(secs))
    info=struct.unpack_from('>I',data,rel['off']+4)[0]
    struct.pack_into('>I',data,rel['off']+4,(info&~255)|255)
    bad=tmp_path/'bad.o';bad.write_bytes(data)
    with pytest.raises(AssertionError):v.inspect(bad,v.FN,tmp_path/'proof')

def test_no_matching_submission_is_registered():
    from tools.cloud.check_submissions import commands
    paths=[str(p.relative_to(ROOT)) for p in HERE.iterdir() if p.is_file()]
    assert list(commands(ROOT,paths))==[]
