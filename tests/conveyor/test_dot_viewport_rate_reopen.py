"""Fail-closed documentation and bounded replay tests for the B83 reopen."""
import hashlib
import importlib.util
import json
from pathlib import Path

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/dot_viewport_rate_reopen'
RECEIPT = json.loads((PACKET / 'verification.json').read_text())


def load_replay():
    spec = importlib.util.spec_from_file_location('viewport_rate_replay', PACKET / 'replay.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


def test_receipt_does_not_claim_a_match_or_coverage():
    assert RECEIPT['status'] == 'COMPLETE-NONMATCH'
    assert RECEIPT['claims'] == []
    candidate = RECEIPT['candidate']
    assert candidate['elf_function_bytes'] == candidate['target_bytes'] == 312
    assert len(candidate['full_extent_different_offsets']) == 31
    assert candidate['missing_bytes'] == candidate['excess_bytes'] == 0
    assert not any(candidate['full_extent_relocations'].values())
    assert candidate['canonical_scorer']['differing'] == 31


def test_source_and_context_are_freshly_bound():
    assert hashlib.sha256((PACKET / 'candidate.c').read_bytes()).hexdigest() == RECEIPT['source_sha256']
    context = ROOT / 'src/blob/exhaust_smoke_effect.c'
    assert hashlib.sha256(context.read_bytes()).hexdigest() == RECEIPT['group_verification'][
        'callee_unchanged_source_sha256']
    assert hashlib.sha256((ROOT / 'asm/us/blob/SHA256SUMS').read_bytes()).hexdigest() == RECEIPT[
        'protected_target_manifest_sha256']
    assert hashlib.sha256((ROOT / 'asm/us/blob_data/SHA256SUMS').read_bytes()).hexdigest() == RECEIPT[
        'owned_data_manifest_sha256']


def test_callee_whole_body_and_literals_are_proven():
    proof = RECEIPT['group_verification']
    assert proof['callee_bytes'] == 452 and proof['callee_strict_words_equal'] == 113
    assert proof['callee_owned_literal_references'] == 3
    assert proof['callee_owned_literal_relocation_sites'] == 6
    assert proof['callee_owned_literal_bytes'] == 12
    assert proof['callee_owned_data_failures'] == proof['callee_owned_data_unverified'] == []
    assert proof['independent_gnu_link_agrees']
    assert proof['outside_function_zero_alignment_bytes'] == 4


def test_four_literal_controls_and_real_context_are_bounded():
    rows = RECEIPT['experiments']
    assert len(rows) == 18
    assert len({r['variant'] for r in rows}) == 14
    for name, size, words in [('float', 316, 73), ('integer', 324, 35),
                               ('double', 344, 76), ('short_float', 316, 73)]:
        pair = [r for r in rows if r['variant'] == name]
        assert {r['mode'] for r in pair} == {'single', 'real_callee_group'}
        assert all(r['elf_function_bytes'] == size for r in pair)
        assert all(len(r['full_extent_different_offsets']) == words for r in pair)
        assert all(not any(r['full_extent_relocations'].values()) for r in pair)


def test_prefix_scorer_caution_does_not_hide_the_full_extent():
    integer = next(r for r in RECEIPT['experiments']
                   if r['variant'] == 'integer' and r['mode'] == 'single')
    assert integer['canonical_scorer']['differing'] == 36
    assert integer['canonical_scorer']['errors']
    assert len(integer['full_extent_different_offsets']) == 35
    assert integer['excess_bytes'] == 12
    assert not integer['full_extent_relocations']['errors']


def test_semantic_scope_is_explicit():
    proof = RECEIPT['semantics']
    assert proof['cases'] == proof['host_c_comparisons'] == 4160
    assert proof['native_and_candidate_executions'] == 8320
    assert proof['captured_arguments_per_call'] == 7
    assert proof['output_words_per_case'] == 85
    assert proof['invalid_nan_infinite_or_out_of_range_inputs_tested'] is False
    assert 'modeled accepted callee' in proof['scope']


def test_native_model_detects_an_alpha_regression():
    replay = load_replay()
    import verify_semantics as semantics
    words = list(replay.score.targets()['arb_rate_set'])
    values = [0, 0x1000, 0x2000] + [semantics.fbits(v) for v in (60, 45, 320, 240, 160, 120)]
    correct = semantics.execute(words, values)
    # Flip only the immediate alpha bit in the existing opaque-color OR.
    assert words[0x104 // 4] >> 26 == 13
    words[0x104 // 4] ^= 1
    wrong = semantics.execute(words, values)
    assert correct != wrong
    assert correct[77] == 65537 and wrong[77] == 0


def test_native_model_rejects_a_wrong_callee():
    replay = load_replay()
    import verify_semantics as semantics
    words = list(replay.score.targets()['arb_rate_set'])
    values = [0, 0x1000, 0x2000] + [semantics.fbits(v) for v in (60, 45, 320, 240, 160, 120)]
    words[0xC4 // 4] ^= 1
    with pytest.raises(AssertionError):
        semantics.execute(words, values)


@pytest.mark.skipif(not (load_replay().score.IDO / 'cc').is_file(),
                    reason='requires pinned local IDO and MIPS binutils')
def test_fresh_compile_full_extent_link_and_semantics(tmp_path):
    replay = load_replay()
    fresh = replay.run(tmp_path / 'fresh.json', include_variants=False)
    assert fresh['candidate']['full_extent_different_offsets'] == RECEIPT['candidate'][
        'full_extent_different_offsets']
    assert fresh['group_verification']['caller_relocated_sha256'] == RECEIPT[
        'group_verification']['caller_relocated_sha256']
