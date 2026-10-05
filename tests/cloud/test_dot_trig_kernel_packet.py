"""Whole-body, private-ABI and bounded semantic proof for the isolated trig unit."""
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shutil
import struct

import pytest

ROOT=Path(__file__).resolve().parents[2]
HERE=ROOT/'cloud/work/frontier/dot_trig_kernel_20261005'
spec=importlib.util.spec_from_file_location('dot_trig_verify',HERE/'verify.py')
verify=importlib.util.module_from_spec(spec);spec.loader.exec_module(verify)


def receipt():return json.loads((HERE/'verification.json').read_text())


def test_source_and_archive_bindings():
    r=receipt()
    for name,digest in r['source_bindings'].items():
        assert hashlib.sha256((ROOT/name).read_bytes()).hexdigest()==digest
    source=verify.SOURCE.read_text()
    assert verify.ARCHIVE.read_text() in source
    for banned in ('volatile','__standin','zz_caller','__asm','M2C_ERROR'):
        assert banned not in source
    g=json.loads((verify.GROUP/'group.json').read_text())
    assert g['claims']==g['members']==verify.NAMES
    assert g['keep']==verify.NAMES[1:] and g['context']==[]


def test_complete_proof_and_scope():
    r=receipt()
    assert r['candidate_bytes']==524
    assert r['accepted_byte_gain']==r['newly_discovered_instruction_match_bytes']==0
    assert all(verify.exact(x) for x in r['object'].values())
    assert [r['object'][n]['elf_bytes'] for n in verify.NAMES]==[452,36,36]
    assert r['gnu_link']['relocation_count']==32 and r['gnu_link']['owned_literal_bytes']==48
    assert r['gnu_link']['all_complete_bodies_equal']
    assert not r['gnu_link']['wrapper_final_placement_claimed']
    assert r['gnu_link']['text_alignment_outside_functions']==4
    assert len(r['caller_census']['func_8009C3F8'])==5


def test_causal_controls_and_private_abi():
    r=receipt();c=r['controls']
    assert c['archived_a21']['func_8009C3F8']['differing']==92
    assert c['flag_first']['func_8009C3F8']['differing']==0
    assert [c['flag_first'][n]['differing'] for n in verify.NAMES[1:]]==[2,2]
    assert c['folded_half_literal']['func_8009C3F8']['elf_bytes']==440
    b=r['behavior']
    assert b['cases']==12444 and b['native_executions']==49776
    assert b['unchanged_host_c89_ubsan'] and b['dynamic_coverage_equals_static_cfg']
    assert b['private_kernel_inputs']==['f16','a0']
    assert b['ordinary_wrapper_input']=='f12'
    assert b['coverage']['func_8009C3F8']['unexecuted_offsets']==[44,80]
    assert all(m['rejected'] for m in b['rejected_source_mutants'].values())
    assert len(b['rejected_source_mutants'])==5


@pytest.mark.parametrize('field,value',[
    ('elf_bytes',448),('elf_bytes',456),('differing',1),('extra_words',1),
    ('unresolved',['bad']),('unverified',['literal']),('errors',['bad relocation']),
])
def test_incomplete_proof_rejected(field,value):
    r=receipt()['object']['func_8009C3F8'];r[field]=value
    assert not verify.exact(r)


def native_fixture():
    s=verify.score.image_symbols();t=verify.score.targets()
    code={s[n]+4*i:w for n in verify.NAMES for i,w in enumerate(t[n])}
    data={a:struct.unpack('>I',verify.score.own_data().read(a,4))[0]
          for first,length in [(verify.LITERAL,48),(verify.TABLE,16)] for a in range(first,first+length,4)}
    return s,code,data


def test_decoder_and_stack_fail_closed():
    s,code,data=native_fixture();entry=s[verify.NAMES[0]]
    broken=dict(code);broken[entry]=0xfc000000
    with pytest.raises(AssertionError,match='unknown opcode'):
        verify.native.execute(broken,entry,data,verify.bits(0.5),0,True)
    entry=s['camera_update_c'];broken=dict(code)
    # Locate the actual sole saved-return-address store, then redirect it.
    at,= [pc for pc in range(entry,entry+36,4) if code[pc]>>26==43]
    broken[at]=code[at]-4
    with pytest.raises(AssertionError,match='unexpected write'):
        verify.native.execute(broken,entry,data,verify.bits(0.5),0)


def test_private_f16_input_cannot_be_replaced_by_f12():
    s,code,data=native_fixture();entry=s[verify.NAMES[0]]
    original=verify.native.execute(code,entry,data,verify.bits(0.25),0,True)[0]
    assert original==verify.oracle(0.25,0)
    broken=dict(code);instruction=broken[entry+8]
    assert (instruction>>11)&31==16 and instruction&63==5
    broken[entry+8]=(instruction&~(31<<11))|(12<<11)
    wrong=verify.native.execute(broken,entry,data,verify.bits(0.25),0,True)[0]
    assert wrong!=original


def require_toolchain():
    if not (verify.score.IDO/'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        if os.environ.get('REQUIRE_TOOLCHAIN')=='1':pytest.fail('required pinned toolchain missing')
        pytest.skip('pinned IDO and GNU MIPS required')


def test_complete_fresh_replay(tmp_path):
    require_toolchain()
    assert verify.report(tmp_path)==receipt()


def test_gnu_refuses_owned_literal_and_code_tampering(tmp_path):
    require_toolchain();obj=tmp_path/'source.o'
    verify.score.compile_group(verify.GROUP,obj)
    raw,sections=verify.score._elf(obj)
    for label,section_name,offset in [('literal','.rodata',0),('code','.text',8)]:
        sec,=[s for s in sections if s['name']==section_name]
        damaged=bytearray(raw);damaged[sec['off']+offset+3]^=1
        candidate=tmp_path/(label+'.o');candidate.write_bytes(damaged)
        with pytest.raises(AssertionError):verify.gnu_proof(candidate,tmp_path/label)


def test_actual_camera_parent_context_keeps_trig_exact():
    r=receipt();context=r['genuine_context']
    for name,digest in context['source_bindings'].items():
        assert hashlib.sha256((ROOT/name).read_bytes()).hexdigest()==digest
    assert all(verify.exact(x) for x in context['trig_bodies'].values())
    assert context['gnu_link']['all_complete_bodies_equal']
    assert context['gnu_link']['relocation_count']==32
    assert set(context['unclaimed_context'])=={'func_800D348C','stunt_combo_display','camera_follow_path','func_8008B2E4'}
    assert all(x['differing']>0 for x in context['unclaimed_context'].values())
    for control,proof in r['controls_gnu'].items():
        assert set(proof['functions'])==set(verify.NAMES)
        assert any(p['full_extent_differing_positions'] for p in proof['functions'].values())
    # Source/compiler diagnostic messages must never publish compared ROM words.
    assert not __import__('re').search(r'retail [0-9a-f]{8}, got [0-9a-f]{8}',(HERE/'verification.json').read_text())
