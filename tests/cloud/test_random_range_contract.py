"""Donor-backed range RNG: authentic mask, full extents, native/host proof."""
import hashlib
import importlib.util
import json
from pathlib import Path
import shutil

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT/'cloud/work/ipa-groups/dot_random_range_b2e4_20261005'
spec = importlib.util.spec_from_file_location('range_rng_proof',PACKET/'verify.py')
proof = importlib.util.module_from_spec(spec)
spec.loader.exec_module(proof)

@pytest.fixture(scope='module')
def replay(tmp_path_factory):
    if not (proof.score.IDO/'cc').is_file() or not shutil.which('mips-linux-gnu-ld') or not shutil.which('cc'):
        pytest.skip('IDO, GNU MIPS tools and host C compiler required')
    return proof.verify(tmp_path_factory.mktemp('random-range'))

def test_frozen_bindings_and_claim():
    saved = json.loads((PACKET/'verification.json').read_text())
    for name,digest in {**saved['source_sha256'],**saved['packet_sha256']}.items():
        assert hashlib.sha256((PACKET/name).read_bytes()).hexdigest()==digest
    spec = json.loads((PACKET/'group.json').read_text())
    assert spec['claims']==spec['members']==['func_8008B2E4']
    assert spec['context']==['func_8008B2B4']
    assert saved['new_candidate_bytes']==72 and saved['accepted_byte_gain']==0
    assert (PACKET/'rand.c').read_bytes()==proof.base_bytes('src/blob/func_8008B2B4.c')

def test_whole_elf_and_independent_link(replay):
    assert replay['object']['symbol_bytes']==72
    assert replay['object']['full_extent_equal'] and replay['object']['all_relocations_resolved']
    assert replay['gnu_link']['full_pair_equal']
    assert replay['zero_alignment_bytes_outside_symbols']==8
    assert replay['no_owned_data_or_literals']
    assert replay['production_reader_equal']=={'func_8008B2B4':True,'func_8008B2E4':True}

def test_authentic_context_and_causal_controls(replay):
    assert replay['accepted_context']['full_extent_equal']
    assert replay['accepted_context']['symbol_bytes']==48
    assert replay['controls']['without_outer_mask']['differing']==3
    assert replay['controls']['direct_return']['full_extent_equal']
    assert replay['controls']['arcade_double_denominator']['differing']==12

def test_exhaustive_sample_and_bounded_scale_proof(replay):
    r = replay['host_native_linked']
    assert r['cases']==135204 and r['native_executions']==270408
    assert r['all_32768_random_outputs_covered'] and r['instruction_offsets_executed']==18
    assert r['host_ubsan'] and r['host_wrap_semantics']=='-fwrapv' and r['linked_replay']
    assert set(r['mutation_discriminators'])=={'wrong_mask','wrong_denominator','ignore_scale_sign'}

def test_native_decoder_rejects_unknown_instruction():
    words = list(proof.score.targets()[proof.FN]); words[0]=0
    with pytest.raises(AssertionError,match='unsupported native instruction'):
        proof.semantics.Native(words).run(1,proof.semantics.bits(1.0))

def test_native_decoder_rejects_redirected_write():
    words = list(proof.score.targets()[proof.FN])
    stores = [i for i,w in enumerate(words) if w>>26==43]
    assert len(stores)==1
    words[stores[0]] |= 4
    with pytest.raises(AssertionError):
        proof.semantics.Native(words).run(1,proof.semantics.bits(1.0))

def test_negative_zero_preserved():
    scale=proof.semantics.bits(-0.0)
    expected=proof.semantics.oracle(1,scale)
    assert expected[1]==0x80000000
    assert proof.semantics.Native(proof.score.targets()[proof.FN]).run(1,scale)==expected


def test_complete_portable_receipt(replay):
    assert proof.portable(replay)==proof.portable(json.loads((PACKET/'verification.json').read_text()))


def test_live_source_and_lock_churn_does_not_change_base_context(monkeypatch):
    old_read=Path.read_bytes
    def changed(path):
        if path==ROOT/'src/blob/func_8008B2B4.c':return b'changed integration source'
        return old_read(path)
    monkeypatch.setattr(Path,'read_bytes',changed)
    assert (PACKET/'rand.c').read_bytes()==proof.base_bytes('src/blob/func_8008B2B4.c')


def test_portable_receipt_keeps_packet_and_native_proof():
    import copy
    saved=json.loads((PACKET/'verification.json').read_text())
    changed=copy.deepcopy(saved)
    changed['target_manifest_sha256']='0'*64
    changed['tools_sha256']={}
    assert proof.portable(changed)==proof.portable(saved)
    changed['selected_native_body_sha256'][proof.FN]='0'*64
    assert proof.portable(changed)!=proof.portable(saved)
