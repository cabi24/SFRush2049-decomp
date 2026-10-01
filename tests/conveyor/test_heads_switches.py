"""Switch extent proofs on synthetic images; no cartridge bytes."""
import struct

import pytest

from tools.conveyor.pipeline import targets

BASE = targets.GAME_CODE_BASE
JR = targets.JR_RA
FRAME = 0x27BDFFE8
RESTORE = 0x27BD0018


def switch_image(prefix=None, table_offset=64, entries=None):
    prefix = [FRAME] if prefix is None else prefix
    start = len(prefix)
    table = BASE + table_offset
    low = table & 0xFFFF
    hi = (table + (0x10000 if low & 0x8000 else 0)) >> 16
    words = prefix + [
        0x2C810002,                 # sltiu at,a0,2
        0x1020000A,                 # beq at,zero,default (10 words ahead)
        0x00042080,                 # sll a0,a0,2 (branch delay slot)
        0x3C010000 | hi,            # lui at,table's adjusted high half
        0x00240821,                 # addu at,at,a0
        0x8C2F0000 | low,           # lw t7,table's signed low half(at)
        0x01E00008, 0,              # jr t7; nop
        JR, RESTORE,                # first case
        JR, RESTORE,                # second case
        JR, RESTORE,                # default
    ]
    bound = BASE + len(words) * 4
    destinations = [BASE + (start + 8) * 4, BASE + (start + 10) * 4]
    entries = destinations if entries is None else entries
    data = bytearray(struct.pack('>%dI' % len(words), *words))
    data.extend(b'\0' * max(0, table_offset + 8 - len(data)))
    struct.pack_into('>2I', data, table_offset, *entries)
    return data, bound, start


def put(data, index, word):
    struct.pack_into('>I', data, index * 4, word)


def test_switch_accounts_for_every_case_default_and_return_delay_slot():
    data, bound, start = switch_image()
    proof = targets.scan_head_extent(data, BASE, bound)
    assert proof['insn_count'] == 15
    assert proof['returns'] == [BASE + 36, BASE + 44, BASE + 52]
    assert proof['switches'] == [{
        'at': BASE + 28, 'check': BASE + 4, 'table': BASE + 64,
        'entries': 2, 'destinations': [BASE + 36, BASE + 44]}]


def test_duplicate_case_entries_do_not_truncate_the_table():
    data, bound, _ = switch_image(entries=[BASE + 36, BASE + 36])
    proof = targets.scan_head_extent(data, BASE, bound)
    assert proof['switches'][0]['entries'] == 2
    assert proof['returns'] == [BASE + 36, BASE + 52]


@pytest.mark.parametrize('entry', [BASE - 4, BASE + 60, BASE + 37])
def test_every_entry_must_be_aligned_and_inside_the_bound(entry):
    data, bound, _ = switch_image(entries=[BASE + 36, entry])
    proof = targets.scan_head_extent(data, BASE, bound)
    assert proof['reason'] == 'indirect_jump'
    assert proof['switch_reason'] == 'escaping_switch_entry'
    assert proof['switch_detail']['entry'] == 1


def test_incomplete_table_is_not_a_shorter_valid_switch():
    data, bound, _ = switch_image()
    proof = targets.scan_head_extent(data[:-4], BASE, bound)
    assert proof['switch_reason'] == 'invalid_switch_table'


@pytest.mark.parametrize('target_index', [4, 6])
def test_dispatch_cannot_be_entered_after_its_range_check(target_index):
    # A separate path jumps to the predicate branch or the lui, bypassing sltiu.
    prefix = [FRAME, 0x10A00000 | (target_index - 2), 0]
    data, bound, _ = switch_image(prefix=prefix, table_offset=80)
    proof = targets.scan_head_extent(data, BASE, bound)
    assert proof['reason'] == 'unproven_switch_guard'


@pytest.mark.parametrize('offset,word', [
    (0, 0x28810002),                # signed slti is not an unsigned bound
    (0, 0x2C810000),                # empty table
    (0, 0x2C81FFFF),                # unreasonably large count
    (1, 0x1420000A),                # bnez inverts the guarded arm
    (2, 0x00042040),                # scale is two bytes, not four
    (2, 0x00042880),                # shift writes a different register
    (4, 0x00240825),                # or is not a table-address addition
    (5, 0x8C2E0040),                # load doesn't define the jump register
])
def test_similar_but_unproved_dispatches_are_refused(offset, word):
    data, bound, start = switch_image()
    put(data, start + offset, word)
    proof = targets.scan_head_extent(data, BASE, bound)
    assert proof['reason'] == 'indirect_jump'
    assert proof['switch_reason'] == 'unsupported_switch_dispatch'


def test_signed_low_half_is_added_to_the_lui_base():
    data, bound, _ = switch_image(table_offset=0x9000 - (BASE & 0xFFFF))
    proof = targets.scan_head_extent(data, BASE, bound)
    assert proof['switches'][0]['table'] == (BASE & 0xFFFF0000) + 0x9000


def test_switch_case_with_an_unbalanced_return_is_refused():
    data, bound, start = switch_image()
    put(data, start + 11, 0)
    assert targets.scan_head_extent(data, BASE, bound)['reason'] == 'unbalanced_return'


def test_default_cannot_escape_the_function_bound():
    data, bound, start = switch_image()
    put(data, start + 1, 0x10200020)
    assert targets.scan_head_extent(data, BASE, bound)['reason'] == 'escaping_branch'


def test_jump_delay_slot_cannot_contain_another_transfer():
    data, bound, start = switch_image()
    put(data, start + 7, JR)
    assert targets.scan_head_extent(data, BASE, bound)['reason'] == 'transfer_in_delay_slot'
