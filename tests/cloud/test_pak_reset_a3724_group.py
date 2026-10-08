"""Source binding, native contract, guarded canonical replay; no live integration pins."""
import importlib.util
from pathlib import Path
import shutil
from types import SimpleNamespace
import pytest

ROOT=Path(__file__).resolve().parents[2]
PATH=ROOT/'cloud/work/pak_reset_a3724_o3/verify.py'
spec=importlib.util.spec_from_file_location('pak_reset_a3724_packet_verify',PATH)
v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)

def test_source_and_claim_binding():
    manifest,group=v.source_contract()
    assert group['claims']==['func_800A3724']
    assert group['members']==['func_800A3724']
    assert len(group['keep'])==8 and len(group['context'])==11
    assert manifest['compiled_caller_behavior']['execution_count']==1164

def test_native_three_callers_and_signed_reset(tmp_path):
    score=v.snapshot(tmp_path/'base')
    manifest,_=v.source_contract()
    body=score.targets()[v.CLAIM]
    assert v.sha(b''.join(w.to_bytes(4,'big') for w in body))==manifest['target_sha256'][v.CLAIM]
    assert len(v.native_call_contract(score))==3
    addresses=score.image_symbols()
    result=v.reset_contract(body,addresses[v.CLAIM],addresses['memset'])
    assert result['executions']==2048

def test_missing_compiler_skips_without_calling_ido(tmp_path,monkeypatch):
    def forbidden(*args,**kwargs):raise AssertionError('compiler must not run')
    score=SimpleNamespace(IDO=tmp_path/'missing-ido',compile_group=forbidden,ido=forbidden)
    monkeypatch.setattr(v,'snapshot',lambda work:score)
    result=v.verify()
    assert result['status']=='SKIP' and result['target_compilations']==0

def test_missing_linker_skips_without_calling_ido(tmp_path,monkeypatch):
    def forbidden(*args,**kwargs):raise AssertionError('compiler must not run')
    ido=tmp_path/'ido';ido.mkdir();(ido/'cc').write_text('not executable; guard only')
    score=SimpleNamespace(IDO=ido,compile_group=forbidden,ido=forbidden)
    monkeypatch.setattr(v,'snapshot',lambda work:score)
    monkeypatch.setattr(v.shutil,'which',lambda name:None)
    result=v.verify()
    assert result['status']=='SKIP' and result['target_compilations']==0

def test_source_mutation_fails_closed(tmp_path,monkeypatch):
    shutil.copytree(v.GROUP,tmp_path/'group')
    source=tmp_path/'group/group.c';source.write_text(source.read_text().replace('blocked = pak->blocked;','blocked = 0;'))
    monkeypatch.setattr(v,'GROUP',tmp_path/'group')
    with pytest.raises(AssertionError):v.source_contract()

def test_unsupported_reset_instruction_fails_closed(tmp_path):
    score=v.snapshot(tmp_path/'base');body=list(score.targets()[v.CLAIM]);addresses=score.image_symbols()
    body[1]=0xFFFFFFFF
    with pytest.raises(AssertionError,match='unsupported opcode'):
        v.reset_contract(body,addresses[v.CLAIM],addresses['memset'])

def test_canonical_group_replay(tmp_path):
    score=v.snapshot(tmp_path/'base')
    if not (score.IDO/'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('pinned IDO and MIPS GNU linker required')
    result=v.verify()
    assert result['status']=='PASS' and result['claim_bytes']==88
