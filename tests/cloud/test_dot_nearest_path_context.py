"""Complete proof replay and fail-closed controls for the nearest-path helper."""
import importlib.util
import json
from pathlib import Path
import struct
import shutil
import tempfile
import pytest
ROOT=Path(__file__).resolve().parents[2]
PACKET=ROOT/'cloud/work/ipa-groups/dot_nearest_path_context_20261005'
SPEC=importlib.util.spec_from_file_location('nearest_path_proof',PACKET/'verify.py')
proof=importlib.util.module_from_spec(SPEC);SPEC.loader.exec_module(proof)

@pytest.fixture(scope='module')
def checked(tmp_path_factory):
    if not (proof.score.IDO/'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('pinned IDO and MIPS GNU linker required')
    return proof.verification(tmp_path_factory.mktemp('nearest-path-test'))

def test_complete_replay(checked):
    assert proof.portable_receipt(checked)==proof.portable_receipt(json.loads((PACKET/'verification.json').read_text()))

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
    if not (proof.score.IDO/'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('pinned IDO and MIPS GNU linker required')
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


def test_historical_manifest_drift_is_portable(checked):
    import copy
    current=copy.deepcopy(checked)
    for key in ('asm/us/blob/SHA256SUMS','asm/us/blob/symbols.json','asm/us/blob_data/SHA256SUMS',
                'tools/cloud/score.py','tools/cloud/owndata.py'):
        current.setdefault('protected_inputs_sha256', {})[key]='0'*64
    assert proof.portable_receipt(current)==proof.portable_receipt(checked)

@pytest.mark.parametrize('field', ['native_bodies','data_windows','symbol_addresses_sha256'])
def test_selected_input_drift_is_not_portable(checked,field):
    import copy
    current=copy.deepcopy(checked)
    if field=='symbol_addresses_sha256':current['selected_inputs_sha256'][field]='0'*64
    else:
        key=next(iter(current['selected_inputs_sha256'][field]))
        current['selected_inputs_sha256'][field][key]='0'*64
    assert proof.portable_receipt(current)!=proof.portable_receipt(checked)

@pytest.fixture
def copied_artifacts(tmp_path,monkeypatch):
    import shutil
    target=tmp_path/'blob'
    shutil.copytree(proof.score.ASM_DIR,target)
    shutil.copytree(proof.owndata.artifact_dir(proof.score.ASM_DIR),tmp_path/'blob_data')
    monkeypatch.setattr(proof.score,'ASM_DIR',target)
    monkeypatch.setattr(proof.score,'_targets',None)
    monkeypatch.setattr(proof.score,'_target_fingerprint',None)
    return target

def test_actual_unrelated_manifest_refresh_is_portable(copied_artifacts,checked):
    import hashlib
    target=copied_artifacts
    file=target/'blob_800947f0.s'
    file.write_bytes(file.read_bytes()+b'\n# Unrelated authenticated source annotation.\n')
    manifest=target/'SHA256SUMS'
    before_manifest=hashlib.sha256(manifest.read_bytes()).hexdigest()
    lines=manifest.read_text().splitlines()
    manifest.write_text('\n'.join(hashlib.sha256(file.read_bytes()).hexdigest()+'  '+file.name
                                  if line.split()[-1]==file.name else line for line in lines)+'\n')
    names=checked['object_metadata']['referenced_symbols']
    actual=proof.selected_inputs(set(names)|{'func_800E56F8'})
    assert actual==checked['selected_inputs_sha256']
    assert hashlib.sha256(manifest.read_bytes()).hexdigest()!=before_manifest
    assert 'protected_inputs_sha256' not in checked

def test_current_target_manifest_still_validated(copied_artifacts,checked):
    file=copied_artifacts/'blob_800de454.s'
    file.write_bytes(file.read_bytes()+b'\n# Unauthenticated mutation.\n')
    with pytest.raises(SystemExit,match='SHA-256 mismatch'):
        proof.selected_inputs(checked['object_metadata']['referenced_symbols'])

def test_current_data_manifest_still_validated(copied_artifacts,checked):
    file=copied_artifacts.parent/'blob_data'/'opaque.hex'
    file.write_bytes(file.read_bytes()+b'\n')
    with pytest.raises(SystemExit,match='SHA-256 mismatch'):
        proof.selected_inputs(checked['object_metadata']['referenced_symbols'])

def authenticate_fixture(file):
    import hashlib
    manifest=file.parent/'SHA256SUMS'
    before_manifest=hashlib.sha256(manifest.read_bytes()).hexdigest()
    lines=manifest.read_text().splitlines()
    manifest.write_text('\n'.join(hashlib.sha256(file.read_bytes()).hexdigest()+'  '+file.name
                                  if line.split()[-1]==file.name else line for line in lines)+'\n')

def selected_names(checked):
    return set(checked['object_metadata']['referenced_symbols'])|{'func_800E56F8'}

def test_authenticated_selected_body_change_rejected(copied_artifacts,checked):
    import re
    file=copied_artifacts/'blob_800de454.s';source=file.read_text()
    start=source.index('.section .text.func_800E398C,')
    match=re.search(r'\.word\s+(0x[0-9a-fA-F]+)',source[start:])
    begin,end=start+match.start(1),start+match.end(1)
    file.write_text(source[:begin]+hex(int(match[1],16)^1)+source[end:]);authenticate_fixture(file)
    assert proof.selected_inputs(selected_names(checked))!=checked['selected_inputs_sha256']

def test_authenticated_selected_symbol_change_rejected(copied_artifacts,checked):
    file=copied_artifacts/'symbols.json';data=json.loads(file.read_text())
    data['symbols']['D_80151CE8']=hex(int(data['symbols']['D_80151CE8'],16)+4)
    file.write_text(json.dumps(data));authenticate_fixture(file)
    assert proof.selected_inputs(selected_names(checked))!=checked['selected_inputs_sha256']

def test_authenticated_selected_data_change_rejected(copied_artifacts,checked):
    file=copied_artifacts.parent/'blob_data'/'opaque.hex';lines=file.read_text().splitlines();count=0
    for i,line in enumerate(lines):
        if not line or line.startswith('#'):continue
        address,payload=line.split();base=int(address,16);data=bytearray.fromhex(payload)
        if base<=0x8012443c<base+len(data):
            data[0x8012443c-base]^=1;lines[i]=address+' '+data.hex();count+=1
    assert count==1;file.write_text('\n'.join(lines)+'\n');authenticate_fixture(file)
    assert proof.selected_inputs(selected_names(checked))!=checked['selected_inputs_sha256']


def test_portable_receipt_keeps_packet_and_native_proof():
    import copy
    saved=json.loads((PACKET/'verification.json').read_text())
    changed=copy.deepcopy(saved)
    for key in ('asm/us/blob/SHA256SUMS','asm/us/blob/symbols.json','asm/us/blob_data/SHA256SUMS','tools/cloud/score.py','tools/cloud/owndata.py'):
        changed.setdefault('protected_inputs_sha256', {})[key]='0'*64
    assert proof.portable_receipt(changed)==proof.portable_receipt(saved)
    changed['objects'][proof.NAME]['body_sha256']='0'*64
    assert proof.portable_receipt(changed)!=proof.portable_receipt(saved)
