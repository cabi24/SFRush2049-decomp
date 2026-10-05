"""Meaningful negative acceptance cases; no live tooling or artifact edits."""
import sys
from pathlib import Path
sys.path.insert(0, str(Path(__file__).resolve().parent))
import pytest
from logical_extent_proposal import ExtentRefusal, Slot, compose_exact, logical_section, sha256


TABLE = bytes(range(89)) * 4  # 356 distinct source bytes, not ROM instructions.
PAYLOAD = TABLE + bytes(12)
CONTAINER = b'prefix!!' + TABLE + b'nonzero-next' + bytes(16)
SLOT = Slot(8, sha256(TABLE), TABLE, 4)


def test_real_word_alignment_keeps_all_356_bytes_and_nonzero_neighbor():
    retained = logical_section(PAYLOAD, 356, input_alignment=16, slot_alignment=4)
    assert retained == TABLE and len(retained) == 356
    composed = compose_exact(CONTAINER, [SLOT], original_container_sha256=sha256(CONTAINER))
    assert composed == CONTAINER
    assert composed[364:376] == b'nonzero-next'


def test_historical_16_byte_section_remains_exact():
    original = bytes(range(32))
    assert logical_section(original, 32, input_alignment=16, slot_alignment=16) == original


def test_nonzero_excluded_tail_refused():
    with pytest.raises(ExtentRefusal, match='nonzero content'):
        logical_section(TABLE + bytes(11) + b'X', 356, input_alignment=16, slot_alignment=4)


@pytest.mark.parametrize('extent', [0, -4, 355, 357, 372, True, '356'])
def test_malformed_logical_extent_refused(extent):
    with pytest.raises(ExtentRefusal):
        logical_section(PAYLOAD, extent, input_alignment=16, slot_alignment=4)


@pytest.mark.parametrize('input_alignment,slot_alignment', [(3,4),(2,4),(16,8),(16,True),(8192,4),(True,4)])
def test_malformed_alignment_refused(input_alignment, slot_alignment):
    with pytest.raises(ExtentRefusal):
        logical_section(PAYLOAD,356,input_alignment=input_alignment,slot_alignment=slot_alignment)


@pytest.mark.parametrize('payload', [TABLE + bytes(8), TABLE + bytes(16), PAYLOAD + bytes(16)])
def test_only_natural_final_section_padding_may_be_excluded(payload):
    with pytest.raises(ExtentRefusal, match='logical alignment extent'):
        logical_section(payload,356,input_alignment=16,slot_alignment=4)


def test_overlapping_slots_refused_even_if_both_prove_original_bytes():
    overlapping = Slot(360, sha256(CONTAINER[360:364]), CONTAINER[360:364], 4)
    with pytest.raises(ExtentRefusal,match='overlap'):
        compose_exact(CONTAINER,[SLOT,overlapping],original_container_sha256=sha256(CONTAINER))


def test_changed_neighbor_bytes_refused_without_changing_slot():
    changed = CONTAINER[:364] + b'X' + CONTAINER[365:]
    with pytest.raises(ExtentRefusal,match='neighboring bytes changed'):
        compose_exact(changed,[SLOT],original_container_sha256=sha256(CONTAINER))


def test_source_built_table_difference_refused():
    changed = TABLE[:-1] + bytes([TABLE[-1] ^ 1])
    with pytest.raises(ExtentRefusal,match='not exact'):
        compose_exact(CONTAINER,[Slot(8,sha256(TABLE),changed,4)],original_container_sha256=sha256(CONTAINER))


@pytest.mark.parametrize('offset,alignment', [(-4,4),(10,4),(8,16),(8,8),(8,True)])
def test_malformed_slot_or_address_alignment_refused(offset, alignment):
    with pytest.raises(ExtentRefusal):
        compose_exact(CONTAINER,[Slot(offset,sha256(TABLE),TABLE,alignment)],original_container_sha256=sha256(CONTAINER))


def test_original_slot_sha_drift_refused():
    with pytest.raises(ExtentRefusal,match='slot identity changed'):
        compose_exact(CONTAINER,[Slot(8,'0'*64,TABLE,4)],original_container_sha256=sha256(CONTAINER))


@pytest.mark.parametrize('offset',[356,360,-4,3,True])
def test_even_zero_tail_with_relocation_is_not_padding(offset):
    with pytest.raises(ExtentRefusal,match='outside complete logical section'):
        logical_section(PAYLOAD,356,input_alignment=16,slot_alignment=4,relocation_offsets=[offset])
