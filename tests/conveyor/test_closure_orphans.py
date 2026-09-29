"""Orphan functions: complete, uncalled functions in the gaps between extents."""
import shutil
import struct

import pytest

from tools.conveyor.pipeline import closure, targets

pytestmark = pytest.mark.skipif(shutil.which("mips-linux-gnu-objdump") is None,
                                reason="needs the mips binutils")
BASE = targets.GAME_CODE_BASE
JR_RA, NOP = 0x03E00008, 0x00000000
SW_A0_AT = 0xAC244008        # sw $a0,0x4008($at)
LUI_AT = 0x3C018014          # lui $at,0x8014


def _image(*words):
    return b"".join(struct.pack(">I", w) for w in words)


def test_empty_and_setter_functions_in_a_gap_are_found():
    # [known fn: jr ra/nop] [gap: jr ra/nop (empty fn), lui at/jr ra/sw (setter), pad]
    image = _image(JR_RA, NOP, JR_RA, NOP, LUI_AT, JR_RA, SW_A0_AT, NOP)
    rows = [{"address": BASE, "insn_count": 2}]
    assert closure.find_orphans(image, rows) == [(BASE + 8, 2), (BASE + 16, 3)]


def test_gap_reached_by_fall_through_is_not_claimed():
    # the known function's extent is too short: its code runs on into the gap
    image = _image(0x24420001, 0x24420001, 0x24420001, JR_RA, NOP)
    rows = [{"address": BASE, "insn_count": 2}]
    assert closure.find_orphans(image, rows) == []
