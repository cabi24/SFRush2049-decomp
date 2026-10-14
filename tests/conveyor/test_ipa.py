"""010 Phase 0: detecting IDO -O3 interprocedural register allocation."""
from tools.conveyor.pipeline import ipa


def _asm(*lines):
    return "glabel f\n" + "".join(f"    {line}\n" for line in lines)


def test_callee_reading_temporaries_on_entry_is_flagged():
    # audio_helper: stores $t0/$t1 that nothing in the function set.
    result = ipa.analyze(_asm("lw $a1,8($a2)", "sw $t0,16($a1)",
                              "sb $t1,21($a1)", "jr $ra", "addiu $v0,$a1,32"))
    assert result["live_in"] == {"t0", "t1"}


def test_o32_function_is_clean():
    result = ipa.analyze(_asm(
        "addiu $sp,$sp,-24", "sw $ra,20($sp)", "sw $s0,16($sp)",
        "sdc1 $f20,8($sp)", "move $s0,$a0", "jal g", "lwc1 $f12,4($a0)",
        "mov.s $f4,$f0", "lw $ra,20($sp)", "lw $s0,16($sp)",
        "ldc1 $f20,8($sp)", "jr $ra", "addiu $sp,$sp,24"))
    assert result == {"live_in": set(), "preserved_reads": set()}


def test_callee_saved_register_stored_as_a_stack_argument_is_a_read():
    # control_cell_text (w31r, tools/recomp/ipa_detect.py): s7 is a register
    # parameter passed on as the callee's fifth argument; the body never
    # writes or reloads s7, and it calls, so the store is no prologue save.
    result = ipa.analyze(_asm(
        "addiu $sp,$sp,-40", "sw $ra,36($sp)", "sll $t6,$s5,2", "sll $t7,$s6,2",
        "jal camera_auto_follow", "sw $s7,24($sp)", "lw $ra,36($sp)",
        "jr $ra", "addiu $sp,$sp,40"))
    assert result["live_in"] == {"s5", "s6", "s7"}


def test_callee_saved_store_without_restore_in_a_leaf_is_a_save():
    # No call: the store cannot be an outgoing argument.
    result = ipa.analyze(_asm(
        "addiu $sp,$sp,-8", "sw $s0,4($sp)", "jr $ra", "addiu $sp,$sp,8"))
    assert result["live_in"] == set()


def test_stack_argument_register_reused_later_is_still_a_read():
    # control_cell_text2 (w32n): s8 goes out as a stack argument, then the
    # (internal) body reuses s8 without restoring it.
    result = ipa.analyze(_asm(
        "addiu $sp,$sp,-48", "sw $ra,36($sp)", "jal camera_auto_follow",
        "sw $s8,24($sp)", "addu $s8,$v0,$zero", "lw $ra,36($sp)",
        "jr $ra", "addiu $sp,$sp,48"))
    assert result["live_in"] == {"s8"}


def test_saved_register_reloaded_from_its_slot_is_not_a_read():
    result = ipa.analyze(_asm(
        "addiu $sp,$sp,-32", "sw $ra,28($sp)", "sw $s1,0x18($sp)",
        "move $s1,$a0", "jal g", "nop", "lw $ra,28($sp)", "lw $s1,24($sp)",
        "jr $ra", "addiu $sp,$sp,32"))
    assert result["live_in"] == set()


def test_synthetic_lui_of_the_derived_asm_is_dropped():
    # animResults (w32n): the m2c asm renders the delay-slot load `lh $t9,0x7c6($s3)`
    # as an unlabelled `lh $t9,%lo(D)($s3)` after a synthetic labelled `lui $s3,%hi(D)`;
    # read literally, the taken path skips the t9 load and t9 looks live on entry.
    text = ("glabel f\n"
            ".L0:\n    lb      $t8,0($a2)\n"
            ".L4:\n    bnel    $a3,$t8,.L10\n"
            ".L8:\n    lui     $s3,%hi(D)\n    lh      $t9,%lo(D)($s3)\n"
            ".LC:\n    lh      $t9,0($a2)\n"
            ".L10:\n    multu   $t9,$a1\n"
            ".L14:\n    jr      $ra\n"
            ".L18:\n    nop\n")
    insns = ipa.parse(text)
    assert [op for _, op, _ in insns][:4] == ["lb", "bnel", "lh", "lh"]
    assert insns[2][0] == [".L8"]
    assert ipa.analyze(text)["live_in"] == {"s3"}
