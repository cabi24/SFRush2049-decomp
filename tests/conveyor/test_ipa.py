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
        "jr $ra", "addiu $sp,$sp,24"))
    assert result == {"live_in": set(), "preserved_reads": set()}


def test_jal_delay_slot_runs_before_the_call():
    # A temp read in the delay slot is not a read after the call.
    result = ipa.analyze(_asm("li $t6,1", "jal g", "sw $t6,28($sp)",
                              "jr $ra", "nop"))
    assert result["preserved_reads"] == set()


def test_argument_passed_through_a_call_is_flagged():
    # audio_buffer_sync: $a1 set before the first call, consumed by the
    # second with only $a0/$a2 refreshed in between.
    result = ipa.analyze(_asm(
        "lw $a1,40($sp)", "jal f1", "move $a0,$a1", "move $a0,$v0",
        "jal f2", "move $a2,$zero", "jr $ra", "nop"))
    assert result["preserved_reads"] == {"a1"}


def test_float_first_argument_leaves_a0_unset_legitimately():
    # sound_position_set: callee(f32, s32) takes $f12 and $a1; $a0 is unused.
    result = ipa.analyze(_asm(
        "sw $a0,24($sp)", "jal f1", "lwc1 $f12,4($a0)", "lw $t7,24($sp)",
        "lw $a1,28($sp)", "jal f2", "lwc1 $f12,0($t7)", "jr $ra", "nop"))
    assert result["preserved_reads"] == set()
