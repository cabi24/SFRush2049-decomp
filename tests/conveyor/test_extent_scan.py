"""Contract tests for extracted-target control-flow extent scanning."""
import struct

from tools.conveyor.pipeline import targets as T


BASE = T.GAME_CODE_BASE
NOP = 0x00000000
JR_RA = 0x03E00008
JR_T9 = 0x03200008


def _image(*words):
    return b"".join(struct.pack(">I", word) for word in words)


def _branch(pc_index, target_index, opcode=0x04):
    """Encode a direct I-format branch between word indices."""
    displacement = target_index - (pc_index + 1)
    return (opcode << 26) | (displacement & 0xFFFF)


def _bc1(pc_index, target_index):
    displacement = target_index - (pc_index + 1)
    return (0x11 << 26) | (0x08 << 21) | (displacement & 0xFFFF)


def test_early_jr_ra_before_furthest_forward_target_does_not_terminate():
    image = _image(
        _branch(0, 4),
        JR_RA,
        NOP,
        NOP,
        NOP,
        JR_RA,
        NOP,
    )
    assert T.scan_extent(image, BASE) == 7


def test_jump_table_jr_non_ra_does_not_terminate():
    image = _image(JR_T9, NOP, JR_RA, NOP)
    assert T.scan_extent(image, BASE) == 4


def test_terminating_delay_slot_is_included():
    image = _image(JR_RA, 0x24020001)
    assert T.scan_extent(image, BASE) == 2


def test_backward_branch_does_not_extend_extent():
    image = _image(NOP, _branch(1, 0), JR_RA, NOP, JR_RA, NOP)
    assert T.scan_extent(image, BASE) == 4


def test_scan_overrun_at_16_kib_cap():
    image = _image(*([NOP] * (4096 + 1)))
    assert T.scan_extent(image, BASE) == "scan_overrun"


def test_scan_is_pure_for_same_image_and_address():
    image = _image(_bc1(0, 2), NOP, NOP, JR_RA, NOP)
    first = T.scan_extent(image, BASE)
    assert first == 5
    assert T.scan_extent(image, BASE) == first
