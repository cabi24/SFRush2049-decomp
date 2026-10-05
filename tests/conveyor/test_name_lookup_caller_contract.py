"""Research regression: byte equality does not repair a mismatched C contract."""
import importlib.util
import json
from pathlib import Path
import struct

import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/frontier/name_lookup_callers_20261005'
spec = importlib.util.spec_from_file_location('name_lookup_caller_audit', PACKET / 'audit.py')
audit = importlib.util.module_from_spec(spec)
spec.loader.exec_module(audit)


def test_receipt_does_not_promote_byte_equality_to_source_acceptance():
    receipt = json.loads((PACKET / 'verification.json').read_text())
    assert receipt['claims'] == []
    assert not receipt['accepted_caller_match']
    assert not receipt['rom_coverage_claimed']
    assert receipt['source_contract_status'] == 'unresolved_five_actuals_four_formals'
    assert receipt['typed_four_formal_control_rejects_five_actuals']
    assert receipt['source_sha256'][str((PACKET / 'collision_sound_play.c').relative_to(ROOT))] == audit.sha(PACKET / 'collision_sound_play.c')
    assert receipt['replay']['physics_collision_test']['differing_words'] == 5


def test_native_callers_keep_the_real_fifth_word():
    native = audit.native_audit()
    expected = {'collision_sound_play': (216, 128, 104),
                'physics_collision_test': (240, 192, 168)}
    for name, (extent, call, store) in expected.items():
        target = native['targets'][name]
        assert target['extent_bytes'] == extent
        site, = target['callee_calls']
        assert site['call_offset'] == call
        assert site['fifth_slot']['store_offset'] == store
        assert site['fifth_slot']['stack_offset'] == 16
        assert site['fifth_slot']['local_constant'] == 1
    # The accepted callee gained its genuine fifth formal (`err`) on 2026-10-05,
    # proven from the arcade MathBox library (cloud/work/frontier/w4d/RESULTS.md).
    assert native['canonical_formal_count'] == 5
    assert native['targets']['func_800B24EC']['entry_frame_bytes'] == 104
    assert native['targets']['func_800B24EC']['direct_stack_loads_at_or_above_fifth_input'] == []
    assert len(native['direct_callers']) == 16
    assert native['direct_call_count'] == 32
    assert [site['local_constant'] for site in native['zero_fifth_controls']] == [0, 0]


@pytest.fixture(scope='module')
def replay(tmp_path_factory):
    if not (audit.score.IDO / 'cc').exists():
        pytest.skip('IDO unavailable')
    build = tmp_path_factory.mktemp('name-lookup-caller-contract')
    return build, audit.replay(build)


def test_real_callee_regresses_but_contract_remains_unresolved(replay):
    _, result = replay
    assert not result['accepted_caller_match']
    assert result['claims'] == []
    assert result['typed_four_formal_control_rejects_five_actuals']
    for name in ('collision_sound_play', 'func_800B24EC', 'pool_linked_list_init'):
        item = result['replay'][name]
        assert item['strict_object_equal']
        assert item['extent_equal']
        assert item['relocation_masks'] == 0
        assert not (item['unresolved'] or item['unverified'] or item['errors'] or item['owned_sections'])
    for item in result['replay']['real_context'].values():
        assert item['strict_object_equal']
    assert result['replay']['physics_collision_test']['differing_words'] == 5


def test_extra_zero_within_declared_function_extent_is_rejected(replay):
    build, _ = replay
    original = build / 'collision_sound_play.o'
    data, sections = audit.score._elf(original)
    changed = bytearray(data)
    for section_index, section in enumerate(sections):
        if section['type'] != 2:
            continue
        for symbol_index, symbol in enumerate(audit.score._symbol_table(data, sections, section_index)):
            if symbol['name'] == 'collision_sound_play' and symbol['type'] == 2:
                struct.pack_into('>I', changed, section['off'] + symbol_index * 16 + 8,
                                 symbol['size'] + 4)
    mutated = build / 'extra_declared_zero.o'
    mutated.write_bytes(changed)
    # Existing scorer tolerates padding, but a source function's own ELF size
    # must not silently absorb it. The supplemental extent check rejects it.
    assert audit.score.compare(mutated, 'collision_sound_play', show=0).accepted()
    result = audit.strict_object(mutated, 'collision_sound_play')
    assert result['elf_extent_bytes'] == 220
    assert not result['extent_equal']
    assert not result['strict_object_equal']


def test_wrong_callee_relocation_is_rejected(replay):
    build, _ = replay
    source = (PACKET / 'collision_sound_play.c').read_text()
    source = source.replace('func_800B24EC', 'entity_name_copy')
    changed = build / 'wrong_callee.c'
    changed.write_text(source)
    obj = build / 'wrong_callee.o'
    audit.score.compile_single(changed, audit.FLAGS, obj)
    result = audit.strict_object(obj, 'collision_sound_play')
    assert result['extent_equal']
    assert result['differing_words'] > 0
    assert not result['full_relocated_words_equal']
    assert not result['strict_object_equal']
