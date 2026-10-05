"""SDK-free regression coverage for the independently reviewed rectangle audit.

No network, SDK header, host compiler, or IDO installation is required. Full
SDK/host/IDO replay remains an explicit local reproduction, not a CI substitute
using fabricated SDK context.
"""
import ast
import hashlib
import importlib.util
import json
from pathlib import Path

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/texture_rect_fresh_behavior'


def load(name, path):
    spec = importlib.util.spec_from_file_location(name, path)
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    return module


FRESH = load('fresh_rectangle_behavior', PACKET / 'verify_behavior.py')
REPLAY = load('fresh_rectangle_reference', ROOT / 'cloud/work/texture_rect_verification/replay.py')
RECEIPT = json.loads((PACKET / 'fresh_verification.json').read_text())
CONTEXT = json.loads((PACKET / 'context_verification.json').read_text())
MUTANTS = {'reject_zero_area', 'narrow_y_to_short', 'stretch_in_mode_zero',
           'post_stretch_texture_height', 'phase_without_vertical_flip', 'reclip_command_bottom'}


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def narrow_y(arguments):
    return (arguments[0], REPLAY.machine.signed(arguments[1], 16), *arguments[2:])


def test_fresh_source_and_design_are_receipt_bound():
    assert digest(PACKET / 'rectangle.c') == RECEIPT['source_sha256']
    assert digest(PACKET / 'DESIGN.md') == RECEIPT['design_sha256']
    assert RECEIPT['status'] == 'SEMANTIC_RESEARCH_ONLY'
    assert RECEIPT['cases'] == RECEIPT['host_UBSan_cases'] == 6926


def test_historical_candidate_body_is_preserved_in_context_receipt():
    text = REPLAY.DEFAULT.read_text()
    body = text[text.index('void func_80087110('):]
    assert digest(REPLAY.DEFAULT) == CONTEXT['baseline_sha256']
    assert hashlib.sha256(body.encode()).hexdigest() == CONTEXT['body_sha256']
    assert CONTEXT['variants']['frozen_baseline']['source_sha256'] == CONTEXT['baseline_sha256']


def test_sdk_context_is_pinned_and_not_silently_substituted():
    expected = {'libreultra_gbi.h': 'b418e0321f48acd86187a066a873211f0e0ff9cb',
                'mbi.h': '9956ef20eeb3090533be9d0f513ada78e208041c'}
    assert FRESH.HEADERS == RECEIPT['SDK_git_blob_ids'] == CONTEXT['SDK_git_blob_ids'] == expected
    source = (PACKET / 'rectangle.c').read_text()
    assert '#include "sdk_context.h"' in source
    assert source.count('gSPTextureRectangle(') == 1


def test_factored_source_remains_a_shorter_nonmatch():
    compiled = RECEIPT['stock_compile']
    proof = compiled['full_link_proof']
    assert proof['elf_function_bytes'] == 540 and proof['target_bytes'] == 1780
    assert not proof['exact_extent'] and not proof['byte_equal']
    assert not proof['accepted_exact_match'] and not proof['project_scorer_accepted']
    assert proof['gnu_linker_equals_project_relocator']
    assert compiled['compiled_behavior_cases'] == 6926
    for key in ['unresolved', 'unverified', 'relocation_errors']:
        assert proof[key] == []


def test_all_contexts_retain_identical_four_word_residual():
    assert set(CONTEXT['variants']) == {'frozen_baseline', 'authentic_macros_minimal_Gfx', 'full_SDK_Gfx_and_macros'}
    assert len({v['linked_body_sha256'] for v in CONTEXT['variants'].values()}) == 1
    for result in CONTEXT['variants'].values():
        proof = result['full_link_proof']
        assert result['elf_function_bytes'] == 1780 and result['differing_words'] == 4
        assert result['residual_offsets'] == ['0x4c8', '0x4cc', '0x4d0', '0x4d4']
        assert not proof['accepted_exact_match'] and proof['gnu_linker_equals_project_relocator']
        assert result['semantics']['result'] == 'PASS'


def test_all_six_wrong_contract_controls_remain_executable_and_rejected():
    tree = ast.parse((PACKET / 'verify_behavior.py').read_text())
    function = next(node for node in tree.body if isinstance(node, ast.FunctionDef)
                    and node.name == 'falsification_controls')
    assignment = next(node for node in function.body if isinstance(node, ast.Assign)
                      and any(isinstance(t, ast.Name) and t.id == 'variants' for t in node.targets))
    controls = ast.literal_eval(assignment.value)
    assert {name for name, before, after in controls} == MUTANTS
    assert set(RECEIPT['falsification_controls']) == MUTANTS
    source = (PACKET / 'rectangle.c').read_text()
    for name, before, after in controls:
        assert source.count(before) == 1 and before != after
        assert RECEIPT['falsification_controls'][name]['rejected_by_new_cases'] > 0
    narrowing = RECEIPT['falsification_controls']['narrow_y_to_short']
    assert narrowing['rejected_by_new_cases'] == 2
    assert narrowing['rejected_by_prior_host_domain_cases'] == 0


def test_98_new_cases_equal_complete_protected_native_execution():
    words, manifest = REPLAY.independent_target()
    symbols = REPLAY.score.image_symbols()
    cases = list(FRESH.extra_cases())
    assert len(cases) == 98 and len(manifest) == 23
    categories = {}
    for category, arguments, state, unused in cases:
        expected = REPLAY.oracle(arguments, state)
        actual, advance, events, visited = REPLAY.machine.execute(
            words, symbols[REPLAY.NAME], symbols, arguments, state)
        assert actual == expected
        assert advance == 4 * len(expected)
        categories[category] = categories.get(category, 0) + 1
    assert categories == {'fresh_boundary_and_phase': 96, 'y_word_not_short': 1, 'caller_style_y_sum': 1}


def test_prior_host_matrix_really_misses_y_narrowing():
    prior = [(arguments, state) for _, arguments, state, check_host in REPLAY.cases() if check_host]
    assert len(prior) == 3828
    # This measures an actual sensitivity gap, not just an archived receipt.
    assert all(REPLAY.oracle(arguments, state) == REPLAY.oracle(narrow_y(arguments), state)
               for arguments, state in prior)


@pytest.mark.parametrize('category', ['y_word_not_short', 'caller_style_y_sum'])
def test_new_y_cases_reject_narrowing_computationally(category):
    case, = [item for item in FRESH.extra_cases() if item[0] == category]
    _, arguments, state, unused = case
    expected = REPLAY.oracle(arguments, state)
    incorrectly_narrowed = REPLAY.oracle(narrow_y(arguments), state)
    assert incorrectly_narrowed != expected
    words, _ = REPLAY.independent_target()
    symbols = REPLAY.score.image_symbols()
    actual, advance, events, visited = REPLAY.machine.execute(
        words, symbols[REPLAY.NAME], symbols, arguments, state)
    assert actual == expected and advance == 4 * len(expected)


def test_peer_review_binds_the_reviewed_packet_files():
    peer = json.loads((PACKET / 'peer_review.json').read_text())
    assert peer['status'] == 'APPROVED_RESEARCH_ONLY'
    assert not peer['matching_claim'] and not peer['cartridge_claim']
    for filename, expected in peer['packet_sha256'].items():
        assert digest(PACKET / filename) == expected
