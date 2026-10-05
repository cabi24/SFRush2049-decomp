"""Natural SelectBlit matching source, full ELF proof, and synthetic contracts."""
import hashlib
import importlib.util
import json
from pathlib import Path
import shutil

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT/'cloud/work/frontier/dot_select_blit_20261005'
spec = importlib.util.spec_from_file_location('select_blit_proof',PACKET/'verify.py')
proof = importlib.util.module_from_spec(spec)
spec.loader.exec_module(proof)


@pytest.fixture(scope='module')
def replay(tmp_path_factory):
    if not (proof.score.IDO/'cc').exists():
        pytest.skip('IDO toolchain unavailable')
    if not shutil.which('mips-linux-gnu-ld'):
        pytest.skip('mips-linux-gnu binutils unavailable')
    if not shutil.which('cc'):
        pytest.skip('C compiler unavailable')
    return proof.verify(tmp_path_factory.mktemp('select-blit'))


def test_frozen_source_and_proof_bindings():
    saved=json.loads((PACKET/'verification.json').read_text())
    assert saved['source_sha256']==hashlib.sha256(proof.SOURCE.read_bytes()).hexdigest()
    for name,digest in saved['packet_sha256'].items():
        assert digest==hashlib.sha256((PACKET/name).read_bytes()).hexdigest()
    assert saved['accepted_byte_gain']==0
    assert saved['candidate_bytes']==396


def test_complete_symbol_and_gnu_relocations(replay):
    assert replay['object']['full_extent_equal']
    assert replay['object']['all_relocations_resolved']
    assert replay['object']['extra_words']==0
    assert replay['object']['symbol_bytes']==396
    assert replay['gnu_link']['full_symbol_equal']
    assert replay['zero_alignment_bytes_outside_symbol']==4
    assert [(r['offset'],r['type']) for r in replay['relocations']]==[(64,4),(372,4)]


def test_all_accepted_context_bodies_stay_exact(replay):
    assert len(replay['real_context'])==6
    assert all(v['full_extent_equal'] for v in replay['real_context'].values())
    assert all(v['canonical_verdict']=='MATCH' for v in replay['real_context'].values())


def test_o2_and_archived_controls_remain_nonmatches(replay):
    assert replay['controls']['o2']['differing']==98
    assert replay['controls']['archived_a71']['differing']==45


def test_native_host_and_linked_contracts(replay):
    result=replay['host_native_linked']
    assert result['cases']==4304
    assert result['host_ubsan'] and result['linked_replay']
    assert result['native_trap_cases']==2
    assert result['executed_instruction_offsets']==95
    assert result['native_words']==99


def test_five_wrong_contracts_are_detected(replay):
    assert set(replay['host_native_linked']['mutation_discriminators'])=={
        'reverse_column','zero_quotient','signed_index','wrong_flip','missing_update'}


def test_null_owner_and_missing_texture_are_noops():
    for case in [(0,1,16,4,8,2,1,1,32),(1,0,16,4,8,2,1,0,32)]:
        result=proof.semantics.oracle(case)
        assert result[:4]==[-30000,1234,-5678,30000]
        assert result[11]==0
        assert proof.semantics.Native().run(case)==result


def test_negative_index_uses_truncating_division():
    case=(1,1,37,-8,8,5,0,1,0)
    result=proof.semantics.oracle(case)
    assert result[:4]==[-10,-6,0,7]
    assert proof.semantics.Native().run(case)==result
