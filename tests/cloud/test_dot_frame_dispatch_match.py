"""Frame-delta source observation, complete body and mutation-aware dispatch proof."""
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shutil
import pytest

ROOT=Path(__file__).resolve().parents[2]
PACKET=ROOT/'cloud/work/frontier/dot_frame_dispatch_20261005'
spec=importlib.util.spec_from_file_location('frame_dispatch_verify',PACKET/'verify.py')
v=importlib.util.module_from_spec(spec);spec.loader.exec_module(v)

def receipt():return json.loads((PACKET/'verification.json').read_text())

def test_bound_packet_source_and_native_words():
    for name,digest in v.comparable_receipt(receipt())['source_bindings'].items():
        assert hashlib.sha256((ROOT/name).read_bytes()).hexdigest()==digest
    v.assert_native_bindings(receipt()['native_bindings'])

def test_global_manifest_digest_is_historical_only():
    saved=receipt();current=json.loads(json.dumps(saved))
    assert 'historical_native_provenance' not in current
    current['historical_native_provenance']={'legacy manifest': 'unrelated revision'}
    for name in v.HISTORICAL_SOURCES:current['source_bindings'][name]='0'*64
    assert v.comparable_receipt(current)==v.comparable_receipt(saved)

def test_selected_symbols_cover_all_consumed_relocations():
    r=receipt()
    rows=[r['O2'],r['O3']]+list(r['genuine_context'].values())
    consumed={entry['symbol'] for row in rows for entry in row['gnu']['resolved_relocations']}
    assert consumed<=set(r['native_bindings']['symbols'])

@pytest.mark.parametrize('kind',['body','symbol','data'])
def test_selected_native_corruption_is_rejected(monkeypatch,kind):
    if kind=='body':
        original=v.score.targets
        def changed():
            result={name:list(words) for name,words in original().items()}
            result[v.FN][0]^=1
            return result
        monkeypatch.setattr(v.score,'targets',changed)
    elif kind=='symbol':
        original=v.score.image_symbols
        def changed():
            result=dict(original());result['D_8002EB94']+=4
            return result
        monkeypatch.setattr(v.score,'image_symbols',changed)
    else:
        original=v.score.owndata.ImageData.from_artifact
        def changed(directory):
            current=original(directory)
            class ChangedData:
                def read(self,address,size):
                    data=bytearray(current.read(address,size));data[0]^=1
                    return bytes(data)
            return ChangedData()
        monkeypatch.setattr(v.score.owndata.ImageData,'from_artifact',changed)
    with pytest.raises(AssertionError,match='selected native input changed'):
        v.assert_native_bindings(receipt()['native_bindings'])

@pytest.mark.parametrize('kind',['code','data'])
def test_current_manifest_validation_is_required(monkeypatch,kind):
    def rejected(*args):raise SystemExit('current manifest integrity failure')
    if kind=='code':monkeypatch.setattr(v.score,'targets',rejected)
    else:
        v.score.own_data()  # A populated scorer cache cannot bypass fresh validation.
        monkeypatch.setattr(v.score.owndata.ImageData,'from_artifact',rejected)
    with pytest.raises(SystemExit,match='current manifest integrity failure'):
        v.assert_native_bindings(receipt()['native_bindings'])

def test_complete_body_and_no_coverage_credit():
    r=receipt();assert r['candidate_bytes']==208 and r['accepted_byte_gain']==0
    assert r['status']=='MATCHING_CANDIDATE_PENDING_INDEPENDENT_REVIEW'
    for level in ('O3','O2'):
        row=r[level];assert row['elf_bytes']==row['native_bytes']==208
        assert row['gnu']['full_body_equal'] and len(row['gnu']['resolved_relocations'])==16
        assert not any(row[k] for k in ('differing','unresolved','unverified','errors','extra_words','owned_data_bytes'))
    assert r['ordinary_clock_baseline']['differing']==6

def test_clock_contract_is_separate_and_witnessed():
    c=v.clock_witness()
    assert c==receipt()['clock_contract']
    assert c['address']=='0x8002eb94' and c['distinct_from_elapsed_time']=='0x8002eb90'
    assert c['repeated_read_offsets']==[0x4c,0x50,0x64]
    assert c['no_intervening_call_or_clock_store']
    assert len(c['writers'])==2 and c['writer_callers']['viUpdateTime']=={'game_mode_handler':[208]}
    assert 'original header' in c['admission_limit']

def test_genuine_direct_caller_is_preserved():
    r=receipt();assert set(r['direct_callers'])=={v.CALLER}
    assert set(r['genuine_context'])=={v.FN,v.CALLER}
    for name,row in r['genuine_context'].items():
        assert row['gnu']['full_body_equal'] and row['elf_bytes']==row['native_bytes']
    assert r['genuine_context'][v.CALLER]['elf_bytes']==256

def test_behavior_and_mutation_coverage():
    b=receipt()['behavior'];assert b['host_c89_ubsan_cases']==8920
    assert b['native_executions']==17840 and b['every_branch_both_outcomes']
    assert b['executed_instruction_offsets']==list(range(0,208,4))
    assert len(b['source_mutants'])==5 and all(x['counterexamples']>0 and x['rejected'] for x in b['source_mutants'].values())
    assert all(b['adverse_controls'].values())

@pytest.mark.parametrize('field,value',[('elf_bytes',204),('elf_bytes',212),('unverified',['bad']),('extra_words',1)])
def test_strict_refuses_incomplete_or_unverified(monkeypatch,field,value):
    row=dict(elf_bytes=208,native_bytes=208,differing=0,unresolved=[],unverified=[],errors=[],extra_words=0)
    row[field]=value
    monkeypatch.setattr(v,'inspect',lambda *a:row)
    with pytest.raises(AssertionError):v.strict(None,v.FN)

def test_callback_mode_must_be_reread():
    n=v.native
    case=(0x200000,1,0,n.word(.1),n.word(12.0),1,6,0,0)
    result=n.Machine(v.score.targets()[v.FN],case).run()
    assert result==n.reference(case) and result[2]==2 and result[6]==2
    case=(0x200000,1,4,n.word(.1),n.word(12.0),1,7,0,0)
    assert n.Machine(v.score.targets()[v.FN],case).run()[2]==1

def test_fresh_full_replay(tmp_path,monkeypatch):
    reject_live_context(monkeypatch)
    if not (v.score.IDO/'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        if os.environ.get('REQUIRE_TOOLCHAIN')=='1':pytest.fail('pinned toolchain missing')
        pytest.skip('pinned IDO and GNU MIPS toolchain required')
    assert v.comparable_receipt(v.verify(tmp_path))==v.comparable_receipt(receipt())


def reject_live_context(monkeypatch):
    for method in ('read_bytes','read_text'):
        original=getattr(Path,method)
        def guarded(path,*args,_original=original,**kwargs):
            if path==ROOT/'blob_matched.lock.json' or (ROOT/'src/blob') in path.parents:
                raise AssertionError('packet read live production context: '+str(path))
            return _original(path,*args,**kwargs)
        monkeypatch.setattr(Path,method,guarded)


def test_portable_receipt_keeps_packet_and_native_proof():
    import copy
    saved=json.loads((PACKET/'verification.json').read_text())
    changed=copy.deepcopy(saved)
    for key in v.HISTORICAL_SOURCES:changed['source_bindings'][key]='0'*64
    changed['historical_native_provenance']={}
    assert v.comparable_receipt(changed)==v.comparable_receipt(saved)
    changed['O3']['gnu']['linked_body_sha256']='0'*64
    assert v.comparable_receipt(changed)!=v.comparable_receipt(saved)
