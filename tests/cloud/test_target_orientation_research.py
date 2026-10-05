"""Complete source-bound NONMATCH evidence, with no acceptance shortcut."""
import hashlib
import importlib.util
import json
import os
from pathlib import Path
import re
import shutil
import pytest

ROOT = Path(__file__).resolve().parents[2]
HERE = ROOT / 'cloud/work/frontier/dot_target_orientation_20261005'
spec = importlib.util.spec_from_file_location('target_orientation_verify', HERE / 'verify.py')
verify = importlib.util.module_from_spec(spec)
spec.loader.exec_module(verify)


def receipt(): return json.loads((HERE / 'verification.json').read_text())


def test_receipt_bound_to_sources_and_honest_nonmatch():
    result = receipt()
    for name, digest in result['packet_sha256'].items():
        assert hashlib.sha256((HERE / name).read_bytes()).hexdigest() == digest
    assert result['status'] == 'NONMATCH' and result['claims'] == []
    assert result['new_matching_bytes'] == result['accepted_byte_gain'] == 0
    assert not verify.complete_match(result['object'])
    assert result['range'] == ['0x8010E72C', '0x8010E828']


def test_full_extent_relocations_and_exact_residual():
    result = receipt()
    assert result['object']['symbol_bytes'] == result['object']['native_bytes'] == 252
    assert result['object']['differing'] == 6
    assert result['text_relocation_count'] == 14
    assert result['own_data_bytes'] == 0
    assert result['text_alignment_bytes_outside_symbol'] == 4
    assert result['complete_gnu_link_agrees_with_project_relocation']
    assert result['differing_byte_offsets'] == verify.DIFFS
    assert result['all_differences_are_eight_byte_stack_displacements']
    assert result['native_frame_bytes'] == 96 and result['candidate_frame_bytes'] == 88
    assert result['native_abi_compile_checks'] == 18


def test_donor_controls_and_real_context():
    result = receipt()
    assert result['controls']['archived_B77_resource']['differing'] == 24
    assert result['controls']['basis36_car_local']['frame_bytes'] == 80
    assert result['controls']['matrix48_car_local']['frame_bytes'] == 88
    for size in ['basis36', 'matrix48']:
        assert result['controls'][size + '_direct_array']['differing'] == 37
        assert result['controls'][size + '_car_local']['differing'] == 6
    assert result['context_candidate_body_unchanged']
    assert all(verify.complete_match(result['context'][name]) for name in verify.CONTEXT)
    for name, digest in result['context_source_sha256'].items():
        assert hashlib.sha256((ROOT / 'src/blob' / (name + '.c')).read_bytes()).hexdigest() == digest


def test_source_has_only_meaningful_locals_and_no_shaping():
    source = re.sub(r'/\*.*?\*/', '', (HERE / 'candidate.c').read_text(), flags=re.S)
    assert not re.search(r'\b(?:volatile|asm|__asm__|M2C_ERROR|register)\b', source)
    function = source[source.index('void func_8010E72C'):]
    assert 'Node24 *node;\n    Matrix matrix;\n    Model952 *car;' in function
    assert 'car = &player_array[actor->player];' in function
    assert 'vector_normalize_length(car->direction, matrix.mat3.uvs);' in function
    assert function.count('actor->index') == 2
    assert 'stat_lap_split((int)D_8011753C[actor->index].event,' in function
    assert not re.search(r'\b(?:unused|pad|filler)\b', function)


def test_behavior_covers_complete_bodies_and_rejects_wrong_contracts():
    behavior = receipt()['behavior']
    assert behavior['cases'] == 3584 and behavior['native_executions'] == 10752
    assert behavior['instruction_offsets_covered'] == [63, 63, 63]
    assert behavior['ubsan_same_corpus'] == 'passed'
    controls = behavior['wrong_contract_controls']
    assert len(controls) == 7 and all(controls.values())
    assert controls['stale_sound_player'] > 0 and controls['stale_list_head'] > 0


def test_callback_pointer_evidence_is_limited():
    result = receipt()
    assert len(result['registration']['protected_data_pointer_addresses']) == 10
    assert result['registration']['direct_jal_callers'] == []
    assert result['external_time_scalar']['candidate_owns_literal'] is False
    claim = json.loads((HERE / 'claim.json').read_text())
    assert claim['donor']['revision'] == '845329d7b36f5a384c5625ed9a0aef584ab46139'
    assert set(claim['donor']['files']) == {'game/targets.c', 'game/visuals.c', 'LIB/fmath.h'}


@pytest.mark.parametrize('field,value', [
    ('differing', 1), ('symbol_bytes', 248), ('symbol_bytes', 256),
    ('extra_words', 1), ('unresolved', ['bad']), ('unverified', ['data']), ('errors', ['relocation'])])
def test_complete_match_rejects_incomplete_context(field, value):
    record = dict(receipt()['context']['func_80090284'])
    record[field] = value
    assert not verify.complete_match(record)


def test_complete_compiler_native_host_replay(tmp_path):
    if not (verify.score.IDO / 'cc').is_file() or not shutil.which('mips-linux-gnu-ld'):
        if os.environ.get('REQUIRE_TOOLCHAIN') == '1': pytest.fail('required pinned toolchain is unavailable')
        pytest.skip('pinned IDO and GNU MIPS linker required')
    assert verify.verify(tmp_path) == receipt()
