"""Donor-backed radar callback is behaviorally checked and remains a NONMATCH."""
import hashlib
import importlib.util
import json
from pathlib import Path
import shutil
import pytest
ROOT=Path(__file__).resolve().parents[2]
PACKET=ROOT/'cloud/work/frontier/dot_radar_traffic_20261005'
spec=importlib.util.spec_from_file_location('radar_traffic_proof',PACKET/'verify.py')
proof=importlib.util.module_from_spec(spec);spec.loader.exec_module(proof)
@pytest.fixture(scope='module')
def replay(tmp_path_factory):
    if not (proof.score.IDO/'cc').exists():pytest.skip('IDO unavailable')
    if not shutil.which('mips-linux-gnu-ld'):pytest.skip('MIPS GNU linker unavailable')
    if not shutil.which('cc'):pytest.skip('host compiler unavailable')
    return proof.verify(tmp_path_factory.mktemp('radar-traffic'))
def test_packet_source_binding():
    saved=json.loads((PACKET/'verification.json').read_text())
    assert saved['source_sha256']==hashlib.sha256((PACKET/'candidate.c').read_bytes()).hexdigest()
    assert saved['status']=='NONMATCH' and saved['accepted_byte_gain']==0
    for name,digest in saved['packet_sha256'].items():
        assert digest==hashlib.sha256((PACKET/name).read_bytes()).hexdigest()
def test_complete_symbol_and_relocated_nonmatch(replay):
    assert replay['object']['symbol_bytes']==1316
    assert replay['object']['native_bytes']==1320
    assert replay['object']['stack_frame']==152
    assert not replay['object']['full_symbol_equal']
    assert replay['object']['differing']==307
    assert replay['gnu_link']['differing_words']==308
    assert replay['gnu_link']['all_undefined_relocations_resolved']
    assert replay['object']['extra_words']==0
    assert replay['own_literals']['bytes_verified']==8
def test_authentic_vecsub_control_is_not_a_match(replay):
    c=replay['controls']['authentic_vecsub_function']
    assert c['stack_frame']==168 and c['differing']==305
    assert not c['full_symbol_equal'] and c['symbol_bytes']==1316
def test_accepted_context_sized_symbols_are_exact(replay):
    c=replay['controls']['genuine_context']
    assert all(c[n]['full_symbol_equal'] for n in proof.MEMBERS)
    assert c['Input_ApplyPadConfig']['extra_words']==1
    assert not c[proof.FN]['full_symbol_equal']
def test_four_independent_behavior_routes(replay):
    s=replay['semantics']
    assert s['cases']==1748 and s['native'] and s['gnu_linked'] and s['host_ubsan']
    assert s['instruction_offsets_executed']==328 and len(s['uncovered_offsets'])==2
    assert len(s['detected_wrong_contracts'])==5
def test_disabled_view_clears_callback_and_reloads_hidden_state():
    c=[0,1,2,1,0,1,0,64,8,1,1,2,1,160,80,-1,0,0,0,0]
    want=proof.semantics.oracle(c)
    assert want[:6]==[-1,-321,654,-1,0,1]
    assert proof.semantics.Native().run(c)==want
def test_local_car_bypasses_remote_solidity():
    c=[0,0,2,4,0,1,0,64,8,1,0,2,1,160,80,99,0,0,0,0]
    want=proof.semantics.oracle(c)
    assert 2 not in want[6:6+want[5]]
    assert proof.semantics.Native().run(c)==want


def test_public_diagnostics_redact_only_byte_values():
    # Synthetic payloads, never native or candidate bytes.
    prefix="own .rodata+0x4 referenced at +0x2f8 differs from retail 0x80110000 "
    raw=prefix+"(+0x0: retail "+"aa"*4+", got "+"bb"*4+"; 4 bytes compared)"
    public=prefix+"(+0x0: retail [redacted], got [redacted]; 4 bytes compared)"
    original={'errors':[raw],'canonical_verdict':'307/330 words differ ('+raw+')',
              'different':307,'offset':760,'compared_length':4,'status':'NONMATCH',
              'unchanged_note':'retail bytes at 0x801248CC are not available'}
    cleaned=proof.public_evidence(original)
    assert cleaned['errors']==[public]
    assert cleaned['canonical_verdict']=='307/330 words differ ('+public+')'
    assert original['errors']==[raw]
    for key in ('different','offset','compared_length','status','unchanged_note'):
        assert cleaned[key]==original[key]
    assert proof.public_evidence(cleaned)==cleaned


def test_published_receipt_contains_no_diagnostic_byte_payloads(replay):
    saved=json.loads((PACKET/'verification.json').read_text())
    for result in (saved,replay):
        text=json.dumps(result)
        assert proof._DIAGNOSTIC_BYTES.search(text) is None
        assert text.count('retail [redacted], got [redacted]; 4 bytes compared')==6
        assert result['object']['errors']
        assert 'differs from retail 0x80110000 (+0x0:' in result['object']['errors'][0]
        assert result['object']['differing']==307
