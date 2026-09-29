"""Stranded function heads: a start the old inventory placed too late."""
import struct

from tools.conveyor.pipeline import targets

BASE = targets.GAME_CODE_BASE
JR_RA, NOP = 0x03E00008, 0x00000000
LUI_T7 = 0x3C0F8014          # lui $t7,0x8014
ADDIU_T7 = 0x25EF0BF0        # addiu $t7,$t7,3056
BEQZ_S0 = 0x12000011         # beqz $s0,+17 (conditional)
LW_A0 = 0x8E040000           # lw $a0,0($s0)


def _image(*words):
    return b"".join(struct.pack(">I", w) for w in words)


def test_head_after_a_return_is_claimed():
    # prev: jr ra / nop | head: lui t7 | function starts at addiu t7
    image = _image(JR_RA, NOP, LUI_T7, ADDIU_T7, JR_RA, NOP)
    assert targets.stranded_head(image, BASE + 12) == 1


def test_head_after_padding_is_claimed():
    image = _image(JR_RA, NOP, NOP, NOP, LUI_T7, ADDIU_T7, JR_RA, NOP)
    assert targets.stranded_head(image, BASE + 20) == 1


def test_delay_slot_nop_of_a_conditional_branch_is_not_padding():
    # beqz / nop / lw ... : mid-function, nothing to claim
    image = _image(BEQZ_S0, NOP, LW_A0, ADDIU_T7, JR_RA, NOP)
    assert targets.stranded_head(image, BASE + 12) == 0


def test_a_complete_tiny_function_is_not_a_head():
    # jr ra / nop | lui at / jr ra / sw  -> a separate setter function
    image = _image(JR_RA, NOP, 0x3C018011, JR_RA, 0xAC2474B8, ADDIU_T7, JR_RA, NOP)
    assert targets.stranded_head(image, BASE + 20) == 0


def test_branch_into_the_head_blocks_the_claim():
    # An unconditional b whose target is the would-be head: the boundary rule
    # alone would accept it (padding after a transfer), the branch rule rejects.
    b_to_head = 0x10000000 | 0x0002     # b at +0 lands at +12 (the lui)
    image = _image(b_to_head, NOP, NOP, LUI_T7, ADDIU_T7, JR_RA, NOP)
    assert targets.stranded_head(image, BASE + 16) == 0


def test_an_address_live_code_falls_into_is_not_a_start():
    # lw / addiu run straight into the address: it is inside a routine
    image = _image(LW_A0, ADDIU_T7, 0x24420001, JR_RA, NOP)
    assert targets.falls_through_into(image, BASE + 8) is True


def test_a_start_after_a_return_or_padding_is_not_fall_through():
    assert targets.falls_through_into(_image(JR_RA, NOP, ADDIU_T7), BASE + 8) is False
    assert targets.falls_through_into(_image(JR_RA, NOP, NOP, NOP, ADDIU_T7), BASE + 16) is False
