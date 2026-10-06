"""Bounded regression proof for the animation object's genuine setter boundary."""
import copy
import hashlib
import importlib.util
import json
from pathlib import Path
import struct
import shutil

import pytest

ROOT=Path(__file__).resolve().parents[2]
PACKET=ROOT/'cloud/work/frontier/dot_animation_setter_contract_20261006'
spec=importlib.util.spec_from_file_location('animation_setter_proof',PACKET/'verify.py')
proof=importlib.util.module_from_spec(spec);spec.loader.exec_module(proof)


def saved():return json.loads((PACKET/'verification.json').read_text())


def test_source_change_is_only_genuine_boundary():
    original=(PACKET/'controls/archive.c').read_text()
    start=original.index('static void resource_set_object')
    end=original.index('void audio_channel_setup',start)
    expected=original[:start]+'extern void func_80090770(s16 index, u16 object);\n\n'+original[end:]
    expected=expected.replace('resource_set_object(m->id,','func_80090770(m->id,')
    expected=expected.replace('extern Resource68 D_8012E700[];\n','')
    expected=expected.replace('typedef struct Resource68 { u8 prefix[20]; u16 object; u8 tail[46]; } Resource68;\n','')
    assert expected==(PACKET/'group/caller.c').read_text()


def test_accepted_context_is_unchanged():
    original=proof.base_bytes('src/blob/func_80090770.c')
    assert original==(PACKET/'group/setter.c').read_bytes()
    assert hashlib.sha256(original).hexdigest()=='74beff9a631d97d9f734372aea9fbc8a0a1378d897ff4222df3ed121058f74b7'
    recipe=json.loads((PACKET/'group/group.json').read_text())
    assert recipe['claims']==[] and recipe['context']==['func_80090770']
    assert recipe['keep']==['audio_channel_setup','func_80090770']


def test_donor_and_n64_identity_are_separate():
    p=json.loads((PACKET/'provenance.json').read_text())
    assert p['donor']['commit']=='845329d7b36f5a384c5625ed9a0aef584ab46139'
    assert p['donor']['git_blob']=='54bb56793a3a5fbfbcc2e3e60a61a30153a9943d'
    assert p['reservation']['start']=='0x80094888' and p['reservation']['end']=='0x800949D4'
    assert p['source_limits']


def test_full_replay():
    if not (proof.score.IDO/'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('pinned IDO and MIPS GNU linker required')
    assert proof.portable(proof.verify())==proof.portable(saved())


@pytest.mark.parametrize('name',['audio_channel_setup','func_80090770','func_8010E694'])
def test_replay_rejects_selected_entry_address_drift(monkeypatch,name):
    addresses=proof.score.image_symbols();addresses[name]+=0x100000
    monkeypatch.setattr(proof.score,'image_symbols',lambda:dict(addresses))
    with pytest.raises(AssertionError,match='selected entry address drift'):proof.verify()


def test_portability_preserves_selected_bindings():
    receipt=saved();historical=copy.deepcopy(receipt)
    historical['target_manifest_provenance']='unrelated authenticated manifest refresh'
    assert proof.portable(receipt)==proof.portable(historical)
    for change in ['target','context','symbol','witness','source']:
        altered=copy.deepcopy(receipt)
        if change=='target':altered['complete_gnu_proof'][proof.NAME]['target_sha256']='changed'
        elif change=='context':altered['complete_gnu_proof'][proof.CONTEXT]['target_sha256']='changed'
        elif change=='symbol':altered['complete_gnu_proof'][proof.NAME]['consumed_symbols']['D_8002EB94']+=4
        elif change=='witness':altered['clock_witness']['target_sha256']='changed'
        else:altered['input_sha256'][str((PACKET/'group/caller.c').relative_to(ROOT))]='changed'
        assert proof.portable(receipt)!=proof.portable(altered)
    for name in proof.ENTRY_ADDRESSES:
        altered=copy.deepcopy(receipt);altered['entry_addresses'][name]+=4
        assert proof.portable(receipt)!=proof.portable(altered)


def test_receipt_reports_complete_nonmatch():
    report=saved();caller=report['complete_gnu_proof'][proof.NAME]
    assert report['status']=='NONMATCH' and report['claims']==[] and report['accepted_byte_gain']==0
    assert report['entry_addresses']=={'audio_channel_setup':0x80094888,'func_80090770':0x80090770,'func_8010E694':0x8010E694}
    assert caller['elf_function_bytes']==caller['full_target_bytes']==332
    assert caller['complete_differing_offsets']==[252,280,284]
    assert report['complete_gnu_proof'][proof.CONTEXT]['complete_differing_offsets']==[]
    assert len(report['behavior']['semantic_mutants_rejected'])==5
    assert len(report['behavior']['malformed_native_controls'])==4


def test_native_all_paths_and_memory():
    addresses=proof.score.image_symbols();words=proof.score.targets()[proof.NAME]
    seen=set();edges=set()
    for case in proof.corpus():
        result,memory,covered,branches=proof.native.execute(words,addresses[proof.NAME],case,addresses)
        expected,oracle=proof.native.oracle(case,addresses)
        assert result==expected
        assert all(memory[k]==v for k,v in oracle.items() if not proof.native.STACK-64<=k<proof.native.STACK+64)
        seen|=covered;edges|=branches
    assert seen==set(range(0,332,4))
    assert len(edges)==16


def test_native_rejects_unmapped_and_unknown():
    addresses=proof.score.image_symbols();words=list(proof.score.targets()[proof.NAME])
    case=[1,0,0,proof.native.bits(0),proof.native.bits(.0625),3,10,99,0,3,2,0]
    unknown=list(words);unknown[0]=0xffffffff
    with pytest.raises(AssertionError,match='unsupported'):proof.native.execute(unknown,addresses[proof.NAME],case,addresses)
    bad=list(words);bad[19]=(bad[19]&0xffff0000)|4  # Clock read goes outside the single mapped word.
    with pytest.raises(AssertionError,match='unmapped'):proof.native.execute(bad,addresses[proof.NAME],case,addresses)


def test_independent_clock_observation_witness():
    targets=proof.score.targets();addresses=proof.score.image_symbols()
    witness=proof.clock_witness(targets,addresses)
    assert witness['clock_read_offsets']==[76,80,100]
    altered=dict(targets);altered['func_8010E694']=list(targets['func_8010E694'])
    altered['func_8010E694'][17]+=4
    with pytest.raises(AssertionError):proof.clock_witness(altered,addresses)


def test_native_mode_upper_bits_are_narrowed():
    a=proof.score.image_symbols();words=proof.score.targets()[proof.NAME]
    for raw in [0x12340000,0xffff0000,0x12340001,0x1234ffff,0x12348000]:
        case=[raw,0,0,proof.native.bits(0),proof.native.bits(.0625),3,10,99,0,3,2,1]
        expected,_=proof.native.oracle(case,a)
        got,_,_,_=proof.native.execute(words,a[proof.NAME],case,a)
        assert got==expected


def test_linker_identity_is_provenance_without_weakening_native_evidence():
    receipt=saved();different_linker=copy.deepcopy(receipt)
    different_linker['gnu_ld_sha256']='different GNU linker build'
    assert proof.portable(receipt)==proof.portable(different_linker)
    assert receipt==saved()  # Normalizing a receipt must not discard its provenance.
    assert 'gnu_ld_sha256' in different_linker
    for field in ['linked_sha256','elf_function_bytes','full_target_bytes',
                  'complete_differing_offsets','relocation_count','owned_data_bytes',
                  'gnu_and_project_equal','gnu_readelf_size_verified']:
        altered=copy.deepcopy(different_linker)
        altered['complete_gnu_proof'][proof.NAME][field]='changed'
        assert proof.portable(receipt)!=proof.portable(altered),field
    for field in ['compiler_sha256','compiler_stage_sha256','flags','behavior',
                  'controls','ordinary_clock_complete_gnu','o32_layout_assertions']:
        altered=copy.deepcopy(different_linker);altered[field]='changed'
        assert proof.portable(receipt)!=proof.portable(altered),field
