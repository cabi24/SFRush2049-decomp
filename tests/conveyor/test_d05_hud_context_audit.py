"""Read-only structural evidence checks, not gameplay/ROM acceptance tests."""
import importlib.util
import json
from pathlib import Path
import pytest

ROOT = Path(__file__).resolve().parents[2]
PACKET = ROOT / 'cloud/work/d05_hud_context_audit'
spec = importlib.util.spec_from_file_location('d05_audit', PACKET / 'audit.py')
audit = importlib.util.module_from_spec(spec)
spec.loader.exec_module(audit)


def test_signed_immediate_boundaries():
    assert [audit.signed16(x) for x in (0, 32767, 32768, 65535)] == [0, 32767, -32768, -1]


def test_reject_non_stack_prologue():
    with pytest.raises(ValueError):
        audit.geometry([0], 0x80000000)


@pytest.mark.parametrize('name,frame,size,homes,calls', [
    ('func_80109A60', 40, 1268, {40}, {'0x80094EC8': 3, '0x800FE5B0': 2}),
    ('game_results_input', 120, 1732, {120}, {'0x80094EC8': 5, '0x800FE5B0': 2, '0x800A6244': 2, '0x800EF5B0': 1}),
])
def test_current_targets_reproduce_geometry(name, frame, size, homes, calls):
    from collections import Counter
    result = audit.geometry(audit.score.targets()[name], audit.score.image_symbols()[name])
    evidence = json.loads((PACKET / 'evidence.json').read_text())
    assert result == evidence['native'][name]
    assert (result['frame_bytes'], result['native_bytes']) == (frame, size)
    assert {entry['slot'] for entry in result['caller_home_accesses']} == homes
    assert Counter(entry['callee'] for entry in result['calls']) == calls
    assert result['prologue_saves'] == [{'register': 31, 'slot': 20 if frame == 40 else 28}]


def test_minimap_slot_is_signed_and_return_reloads_hidden():
    words = audit.score.targets()['func_80109A60']
    # Slot mapping is a signed byte load, whereas the old seed uses u8.
    load = words[(0x80109A80 - 0x80109A60) // 4]
    assert load >> 26 == 32  # lb, not lbu
    assert (load >> 16) & 31 == 15  # t7
    assert audit.signed16(load) == 11700
    # Last Hidden path rereads signed byte 26, then moves it to return v0.
    reload = words[(0x80109F2C - 0x80109A60) // 4]
    assert reload >> 26 == 32
    assert (reload >> 16) & 31 == 3
    assert audit.signed16(reload) == 26
    move = words[(0x80109F34 - 0x80109A60) // 4]
    assert move >> 26 == 0 and move & 63 == 37
    assert (move >> 21) & 31 == 3 and (move >> 11) & 31 == 2


def test_missing_slot_sentinel_changes_hidden_condition():
    # Exhaustively show why zero-extending the mapping byte is not equivalent.
    differing = []
    for byte in range(256):
        signed = byte - 256 if byte >= 128 else byte
        if ((signed + 1) == 0) != ((byte + 1) == 0):
            differing.append(byte)
    assert differing == [255]


def test_evidence_discloses_archive_gap_and_nonmatch():
    report = json.loads((PACKET / 'evidence.json').read_text())
    assert report['claims'] == []
    assert all(row['manifest_verified'] for row in report['archive']['marker_files'])
    assert report['archive']['minimap_packet_paths'] == []
    baseline = report['replay']['marker_O2']
    assert not baseline['source_matches_archived_receipt']
    assert baseline['matches_archived_comparison']
    grouped = report['replay']['marker_actual_input_O3']
    assert grouped['source_matches_archived_receipt']
    assert grouped['matches_archived_comparison']
    assert grouped['comparison']['extra_words'] == 9
    assert grouped['comparison']['errors']
    assert all(not replay['accepted'] for replay in report['replay'].values())
