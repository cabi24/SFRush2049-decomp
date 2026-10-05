"""SDK-free bounds and provenance checks for the one packet-tail join probe."""
import ast
import hashlib
import importlib.util
import json
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/texture_rect_structural_factoring'
RECEIPT = json.loads((PACKET / 'verification.json').read_text())


def digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def test_packet_has_exactly_two_predeclared_source_controls():
    assert {p.stem for p in PACKET.glob('*.c')} == {'unsigned_eight', 'stretched_tail_join'}
    assert set(RECEIPT['variants']) == {'unsigned_eight', 'stretched_tail_join'}
    assert digest(PACKET / 'DESIGN.md') == RECEIPT['design_sha256']
    assert RECEIPT['status'] == 'STRUCTURAL_RESEARCH_ONLY'
    assert not RECEIPT['matching_claim'] and not RECEIPT['cartridge_claim']


def test_unsigned_baseline_and_join_preserve_prefix_and_original_branch_order():
    baseline = (PACKET / 'unsigned_eight.c').read_text()
    joined = (PACKET / 'stretched_tail_join.c').read_text()
    normalized = joined.replace('    unsigned int ds,dt,t_fixed;\n', '')
    split = '        if((D_8012E608&4)&&(D_8012E608&8)) {'
    # Everything before the second flip dispatch is byte-for-byte identical,
    # including clipping, rejection, all mode-zero sites and stretch setup.
    assert baseline.split(split)[0:2] == normalized.split(split)[0:2]
    dispatch = ['if((D_8012E608&4)&&(D_8012E608&8))',
                'else if(D_8012E608&4)', 'else if(D_8012E608&8)']
    for text in [baseline, joined]:
        assert all(text.count(part) == 2 for part in dispatch)
        assert 'void func_80087110(int x,int y,int right,int bottom,int s,int t)' in text
        assert '#include "sdk_context.h"' in text
        assert 'if(right<x || bottom<y)return;' in text
        assert '(int)((unsigned int)bottom+height+1U)' in text
    assert baseline.count('gSPTextureRectangle(') == 8
    assert joined.count('gSPTextureRectangle(') == 5


def test_pinned_inputs_stay_bound():
    assert RECEIPT['SDK_git_blob_ids'] == {
        'libreultra_gbi.h': 'b418e0321f48acd86187a066a873211f0e0ff9cb',
        'mbi.h': '9956ef20eeb3090533be9d0f513ada78e208041c'}
    assert RECEIPT['flags'] == '-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul'
    assert RECEIPT['historical_source_sha256'] == digest(ROOT / 'cloud/work/frontier/w4a/func_80087110/best.c')
    assert RECEIPT['fresh_source_sha256'] == digest(ROOT / 'cloud/work/texture_rect_fresh_behavior/rectangle.c')
    for name, result in RECEIPT['variants'].items():
        assert result['source_sha256'] == digest(PACKET / (name + '.c'))


def test_complete_extent_and_relocation_receipts_reject_both_controls():
    for name, size, sites in [('unsigned_eight', 1776, 8), ('stretched_tail_join', 1320, 5)]:
        result = RECEIPT['variants'][name]
        proof = result['full_link_proof']
        assert proof['elf_function_bytes'] == size
        assert proof['target_bytes'] == 1780
        assert not proof['exact_extent'] and not proof['byte_equal']
        assert not proof['accepted_exact_match'] and not proof['project_scorer_accepted']
        assert proof['gnu_linker_equals_project_relocator']
        assert proof['protected_manifest_entries_verified'] == 23
        assert proof['alignment_padding_all_zero']
        for key in ['unresolved', 'unverified', 'relocation_errors', 'own_literal_or_data_sections']:
            assert proof[key] == []
        assert result['source_macro_sites'] == sites
        assert set(result['shape']['packet_tag_lui_count'].values()) == {sites}


def test_full_word_host_and_linked_replays_preserve_native_event_order():
    for result in RECEIPT['variants'].values():
        for key in ['host_UBSan', 'linked_semantics']:
            semantics = result[key]
            assert semantics['result'] == 'PASS' and semantics['cases'] == 6926
            assert semantics['all_mode_flip_stretch_paths'] == 16
        linked = result['linked_semantics']
        assert linked['external_event_order_difference_cases'] == 0
        assert linked['external_read_order_difference_cases'] == 0
        assert linked['external_write_order_difference_cases'] == 0
        assert linked['first_external_order_difference'] is None
        assert linked['candidate_unexecuted_offsets'] == []
        assert linked['candidate_instruction_offsets_executed'] == result['shape']['instruction_words']


def test_native_topology_is_derived_from_protected_target():
    spec = importlib.util.spec_from_file_location('factoring_test_verify', PACKET / 'verify.py')
    module = importlib.util.module_from_spec(spec)
    spec.loader.exec_module(module)
    native, manifest = module.REPLAY.independent_target()
    assert len(manifest) == 23
    assert module.shape(native) == RECEIPT['native_shape']
    assert set(RECEIPT['native_shape']['packet_tag_lui_count'].values()) == {8}
    assert RECEIPT['native_shape']['instruction_words'] == 445


def test_wrong_join_controls_are_bound_and_detected():
    tree = ast.parse((PACKET / 'verify.py').read_text())
    function = next(node for node in tree.body if isinstance(node, ast.FunctionDef)
                    and node.name == 'wrong_join_controls')
    assignment = next(node for node in function.body if isinstance(node, ast.Assign)
                      and any(isinstance(t, ast.Name) and t.id == 'variants' for t in node.targets))
    variants = ast.literal_eval(assignment.value)
    source = (PACKET / 'stretched_tail_join.c').read_text()
    assert {name for name, before, after, count in variants} == set(RECEIPT['wrong_join_controls'])
    for name, before, after, count in variants:
        assert source.count(before) == count and before != after
        proof = RECEIPT['wrong_join_controls'][name]
        assert proof['rejected_cases'] > 0
        assert hashlib.sha256(source.replace(before, after).encode()).hexdigest() == proof['source_sha256']
