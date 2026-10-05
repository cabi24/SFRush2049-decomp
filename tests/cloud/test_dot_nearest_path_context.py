"""Complete proof replay and fail-closed controls for the nearest-path helper."""
import importlib.util
import json
from pathlib import Path
import struct
import tempfile
import pytest
ROOT=Path(__file__).resolve().parents[2]
PACKET=ROOT/'cloud/work/ipa-groups/dot_nearest_path_context_20261005'
SPEC=importlib.util.spec_from_file_location('nearest_path_proof',PACKET/'verify.py')
proof=importlib.util.module_from_spec(SPEC);SPEC.loader.exec_module(proof)

@pytest.fixture(scope='module')
def checked(tmp_path_factory):
    return proof.verification(tmp_path_factory.mktemp('nearest-path-test'))

def test_complete_replay(checked):
    assert checked==json.loads((PACKET/'verification.json').read_text())

def test_full_symbol_and_literals(checked):
    row=checked['objects']['func_800E4300']
    assert row['elf_bytes']==540 and row['GNU_complete_word_differences']==0
    assert len(row['relocations'])==8
    assert checked['object_metadata']['verified_context_literal_bytes']==20
    assert checked['object_metadata']['O32_layout_assertions']==16

def test_branch_and_instruction_coverage(checked):
    b=checked['behavior']
    assert b['cases']==1536 and b['native_executions']==3072
    assert b['instruction_coverage']==134 and b['unexecuted_offsets']==[520]
    assert all(v==[False,True] for k,v in b['branches'].items() if k not in ('164','252','512'))

def test_compiled_wrong_contracts(checked):
    assert len(checked['behavior']['wrong_contracts_rejected'])==5
    assert all(checked['behavior']['wrong_contracts_rejected'].values())

@pytest.fixture(scope='module')
def object_file(tmp_path_factory):
    directory=tmp_path_factory.mktemp('nearest-path-object');obj=directory/'original.o'
    proof.score.compile_group(PACKET,obj)
    return obj

def test_shortened_elf_symbol_rejected(object_file,tmp_path):
    raw,sections,tab,syms=proof.object_records(object_file);changed=bytearray(raw)
    index=next(i for i,s in enumerate(syms) if s['name']==proof.NAME)
    struct.pack_into('>I',changed,sections[tab]['off']+index*16+8,536)
    obj=tmp_path/'short.o';obj.write_bytes(changed)
    with pytest.raises(AssertionError):proof.object_proof(obj,tmp_path)

def test_redirected_global_relocation_rejected(object_file,tmp_path):
    raw,sections,tab,syms=proof.object_records(object_file);changed=bytearray(raw)
    fn=next(s for s in syms if s['name']==proof.NAME)
    src=next(i for i,s in enumerate(syms) if s['name']=='D_80151CE8')
    dst=next(i for i,s in enumerate(syms) if s['name']=='D_8012E5E8')
    count=0
    for sec in sections:
        if sec['type']!=9:continue
        for j in range(sec['size']//8):
            off,info=struct.unpack_from('>II',raw,sec['off']+j*8)
            if fn['value']<=off<fn['value']+fn['size'] and info>>8==src:
                struct.pack_into('>I',changed,sec['off']+j*8+4,(dst<<8)|(info&255));count+=1
    assert count==2
    obj=tmp_path/'wrong-symbol.o';obj.write_bytes(changed)
    with pytest.raises(AssertionError):proof.object_proof(obj,tmp_path)

def test_wrong_literal_rejected(object_file,tmp_path):
    raw,sections,_,_=proof.object_records(object_file);changed=bytearray(raw)
    ro=next(s for s in sections if s['name']=='.rodata');changed[ro['off']+7]^=1
    obj=tmp_path/'literal.o';obj.write_bytes(changed)
    with pytest.raises(AssertionError):proof.object_proof(obj,tmp_path)

def test_unknown_native_opcode():
    case=next(proof.corpus());words=proof.score.targets()[proof.NAME]
    with pytest.raises(AssertionError,match='unsupported word'):
        proof.native_case([0xffffffff]+words[1:],case,struct.pack('>f',1e20))

def test_unmapped_position_fails_closed():
    words=proof.score.targets()[proof.NAME]
    with pytest.raises(AssertionError,match='unmapped'):
        proof.native.execute(words,0x800e4300,[],[0x100000,0,0,0,0])

def test_only_real_named_context_is_registered():
    spec=json.loads((PACKET/'group.json').read_text())
    assert spec['claims']==spec['members']==[proof.NAME]
    assert set(spec['context'])==set(proof.CONTEXT)
    assert spec['keep']==['func_800E4B58']
    source=(PACKET/'group.c').read_text()
    assert 'volatile' not in source and '__standin' not in source and 'M2C_' not in source

def test_protected_native_call_interface(checked):
    contract=checked['caller_contract']
    assert contract['verified_native_call_setup']
    assert not contract['caller_end_to_end_runtime_tested']
    assert contract['native_direct_call_graph'][proof.NAME]=={'func_800E451C':['0x800e4748']}
