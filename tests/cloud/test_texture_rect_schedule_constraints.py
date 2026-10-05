"""Rejected guarded-exit provenance and optional independent stock replay."""
import copy
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import shutil

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/texture_rect_schedule_constraints'
RECEIPT = json.loads((PACKET / 'verification.json').read_text())
DIAGNOSTIC = json.loads((PACKET / 'allocation_diagnostic_receipt.json').read_text())


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def test_complete_sources_and_rejection_stay_bound():
    assert not RECEIPT['accepted'] and RECEIPT['research_only']
    for name, expected in [('baseline', 4), ('guarded_exit', 12)]:
        control = RECEIPT['controls'][name]
        assert sha(ROOT / control['source']) == control['source_sha256']
        proof = control['full_elf_verification']
        assert proof['elf_function_bytes'] == proof['target_bytes'] == 1780
        assert proof['project_scorer_target_words'] == 445
        assert proof['project_scorer_differing_words'] == expected
        assert not proof['accepted_exact_match'] and not proof['project_scorer_accepted']
        assert proof['gnu_linker_equals_project_relocator']
        assert proof['project_scorer_extra_words'] == 0
        assert proof['alignment_padding_bytes'] == 12 and proof['alignment_padding_all_zero']
        assert proof['relocation_counts'] == {'R_MIPS_HI16': 16, 'R_MIPS_LO16': 16}
        for key in ['unresolved', 'unverified', 'relocation_errors', 'own_literal_or_data_sections']:
            assert proof[key] == []
        assert control['critical_alias_close_after_unconditional_exit']


def test_only_declared_guard_and_return_source_shape_changed():
    baseline = (ROOT / RECEIPT['controls']['baseline']['source']).read_text()
    guarded = (PACKET / 'guarded_exit.c').read_text()
    start = '        if((D_8012E608&4)&&(D_8012E608&8)) {'
    before = '''        if((D_8012E608&4)&&(D_8012E608&8)) {
            texture_edge=s+right;s=texture_edge-x;t+=height;
            gSPTextureRectangle(D_80149438++,x<<2,y<<2,(right+1)<<2,(bottom+1)<<2,0,s<<5,(t<<5)+offset,-1024,-step);
        } else if(D_8012E608&4) {'''
    after = before.replace(start, start.replace('if(', 'while(', 1))
    after = after.replace('        } else if(D_8012E608&4) {',
                          '            return;\n        }\n        if(D_8012E608&4) {')
    assert baseline.count(before) == 1
    assert baseline.replace(before, after) == guarded
    assert guarded.count('gSPTextureRectangle(D_80149438++') == 8


def test_schedule_offsets_are_distinct_from_register_exchange_offsets():
    baseline = set(RECEIPT['controls']['baseline']['full_elf_verification']['residual_offsets'])
    guarded = set(RECEIPT['controls']['guarded_exit']['full_elf_verification']['residual_offsets'])
    assert baseline == {'0x4c8', '0x4cc', '0x4d0', '0x4d4'}
    assert baseline < guarded
    assert guarded - baseline == {'0x3d0', '0x3d4', '0x3e4', '0x3ec',
                                  '0x464', '0x47c', '0x5e4', '0x5fc'}


def test_diagnostic_membership_and_no_force_status():
    assert DIAGNOSTIC['diagnostic_only'] and DIAGNOSTIC['no_force_controls']
    assert DIAGNOSTIC['source_sha256'] == sha(PACKET / 'guarded_exit.c')
    assert DIAGNOSTIC['optimized_ucode_equal_to_stock']
    assert len({DIAGNOSTIC[key] for key in ['stock_ucode_sha256',
               'native_listing_ucode_sha256', 'print_trace_ucode_sha256']}) == 1
    assert {row['role'] for row in DIAGNOSTIC['decisions']} == {'height', 'offset'}
    for row in DIAGNOSTIC['decisions']:
        count = len(row['occurrence_block_ids']) + len(row['default_live_block_ids'])
        assert count == row['normalization_input']
        assert int(row['nocs']) == (count if count < 3 else 2 + ((count - 2) >> 2))
        assert row['phase'] == 'p1' and row['forced'] == '-2'
        assert row['numintf'] == '20' and row['totalsave'] == '4.000000'
        assert abs(float(row['save']) - 4 / int(row['nocs'])) < 0.000001


def test_peer_review_binds_exact_reviewed_files():
    for name in ['texture_rect_schedule_constraints', 'texture_rect_structural_factoring']:
        folder = PACKET.parent / name
        review = json.loads((folder / 'peer_review.json').read_text())
        assert review['review_result'] == 'PASS_RESEARCH_ONLY'
        assert not review['match_or_promotion_approval']
        for filename, digest in review['reviewed_files_sha256'].items():
            assert sha(ROOT / filename) == digest


def assert_controls_reproduce(fresh_controls, current_manifest_sha256):
    """Only the global manifest's historical provenance may advance.

    Source, target body, linked body, complete extents and every other proof
    field must still equal the archived receipt. Never edit that receipt.
    """
    expected = copy.deepcopy(RECEIPT['controls'])
    assert set(fresh_controls) == set(expected)
    for name, control in fresh_controls.items():
        proof = control['full_elf_verification']
        assert proof['protected_manifest_sha256'] == current_manifest_sha256
        historical_proof = expected[name]['full_elf_verification']
        assert 'protected_manifest_sha256' in historical_proof
        historical_proof['protected_manifest_sha256'] = current_manifest_sha256
    assert fresh_controls == expected


def current_manifest_controls():
    """Test fixture: change only the global provenance hash, never native data."""
    fresh = copy.deepcopy(RECEIPT['controls'])
    current = sha(ROOT / 'asm/us/blob/SHA256SUMS')
    for control in fresh.values():
        control['full_elf_verification']['protected_manifest_sha256'] = current
    return fresh, current


def test_replay_allows_only_current_manifest_provenance_without_editing_receipt():
    unchanged = copy.deepcopy(RECEIPT)
    fresh, current = current_manifest_controls()
    assert_controls_reproduce(fresh, current)
    assert RECEIPT == unchanged


@pytest.mark.parametrize('path,value', [
    (('source_sha256',), 'different-source'),
    (('full_elf_verification', 'target_sha256'), 'different-target-body'),
    (('full_elf_verification', 'linked_body_sha256'), 'different-linked-body'),
    (('full_elf_verification', 'elf_function_bytes'), 1776),
    (('full_elf_verification', 'relocation_counts', 'R_MIPS_HI16'), 15),
    (('full_elf_verification', 'gnu_linker_equals_project_relocator'), False),
])
def test_replay_still_rejects_source_body_extent_and_link_changes(path, value):
    fresh, current = current_manifest_controls()
    node = fresh['guarded_exit']
    for key in path[:-1]:
        node = node[key]
    node[path[-1]] = value
    with pytest.raises(AssertionError):
        assert_controls_reproduce(fresh, current)


def test_replay_rejects_fresh_proof_with_wrong_current_manifest():
    fresh, current = current_manifest_controls()
    fresh['baseline']['full_elf_verification']['protected_manifest_sha256'] = 'wrong-manifest'
    with pytest.raises(AssertionError):
        assert_controls_reproduce(fresh, current)


def test_stock_replay_when_pinned_toolchain_is_available(tmp_path):
    ido = Path(os.environ.get('IDO_DIR', ROOT / 'tools/cloud/ido')).resolve()
    required = ['mips-linux-gnu-nm', 'mips-linux-gnu-readelf',
                'mips-linux-gnu-ld', 'mips-linux-gnu-objcopy']
    if not (ido / 'cc').is_file() or any(shutil.which(name) is None for name in required):
        pytest.skip('stock IDO and GNU MIPS tools are not configured')
    spec = importlib.util.spec_from_file_location('schedule_stock_replay_test', PACKET / 'verify.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    module.BUILD = tmp_path / 'stock_schedule_replay'
    fresh = module.verify()
    # Revalidate every protected entry through the unchanged native verifier.
    # A new global manifest may cover unrelated accepted target updates.
    module.REPLAY.independent_target()
    current_manifest = sha(ROOT / 'asm/us/blob/SHA256SUMS')
    assert_controls_reproduce(fresh['controls'], current_manifest)
    assert fresh['guarded_exit_equals_prior_rejected_fallback_dispatch']
