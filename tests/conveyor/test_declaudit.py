"""Hand-declaration audit: the hand context must not contradict the binary."""
from pathlib import Path

import pytest

from tools.conveyor.pipeline import declaudit

JR_RA = "    jr      $ra"


def _asm(tmp_path, name, body):
    (tmp_path / f"{name}.s").write_text(f"glabel {name}\n{body}")


def test_flags_void_declaration_whose_caller_consumes_the_return(tmp_path):
    header = tmp_path / "game_types.h"
    header.write_text("extern void slot_state_setup(void);\n")
    _asm(tmp_path, "slot_state_setup",
         "    move    $v0,$s3\n" + JR_RA + "\n    nop\n")
    _asm(tmp_path, "caller",
         "    jal     slot_state_setup\n    nop\n"
         "    sw      $v0,132($sp)\n" + JR_RA + "\n    nop\n")

    findings, checked = declaudit.audit(header, tmp_path)

    assert checked == 1
    assert findings == [("slot_state_setup", "void_but_returns",
                         "a caller consumes its $v0")]


def test_v0_written_near_a_return_is_not_evidence_on_its_own(tmp_path):
    """audio_frame_sync, visual_objects_update and func_80087110 all set $v0
    close to their return as scratch; only a consuming caller counts."""
    header = tmp_path / "game_types.h"
    header.write_text("extern void scratchy(void);\n")
    _asm(tmp_path, "scratchy",
         "    lw      $v0,0($s0)\n    sw      $v0,4($s0)\n"
         "    lw      $ra,20($sp)\n" + JR_RA + "\n    addiu   $sp,$sp,24\n")
    _asm(tmp_path, "caller",
         "    jal     scratchy\n    nop\n    nop\n" + JR_RA + "\n    nop\n")

    assert declaudit.audit(header, tmp_path)[0] == []


def test_v0_read_at_a_branch_target_is_not_attributed_to_the_call(tmp_path):
    """The derived asm labels every instruction, so only real branch targets
    end a block; a $v0 defined on another path must not count."""
    header = tmp_path / "game_types.h"
    header.write_text("extern void plain(void);\n")
    _asm(tmp_path, "plain", "    nop\n" + JR_RA + "\n    nop\n")
    _asm(tmp_path, "caller",
         "    beqz    $a0,.L80090010\n    nop\n"
         "    jal     plain\n    nop\n"
         ".L80090010:\n    sw      $v0,16($sp)\n" + JR_RA + "\n    nop\n")

    assert declaudit.audit(header, tmp_path)[0] == []


def test_flags_declaration_taking_fewer_parameters_than_the_body_reads(tmp_path):
    header = tmp_path / "game_types.h"
    header.write_text("extern void hud_setup(s32);\n")
    _asm(tmp_path, "hud_setup",
         "    addu    $t0,$a0,$a1\n    addu    $t1,$t0,$a2\n" + JR_RA + "\n    nop\n")

    findings, _ = declaudit.audit(header, tmp_path)

    assert findings == [("hud_setup", "too_few_params",
                         "declared 1, reads 3 arg registers")]


def test_argument_registers_written_before_use_are_not_parameters(tmp_path):
    header = tmp_path / "game_types.h"
    header.write_text("extern void start(void);\n")
    _asm(tmp_path, "start",
         "    li      $a0,1\n    jal     other\n    nop\n" + JR_RA + "\n    nop\n")

    assert declaudit.audit(header, tmp_path)[0] == []


def test_the_live_hand_context_agrees_with_the_binary():
    """Regression guard for the 2026-09-23 finding: `slot_state_setup` and
    `audio_frame_sync` were declared `void` while callers consumed their
    return, and m2c turned that into unset-$v0 errors across 60+ targets."""
    if not declaudit.ASM_DIR.is_dir():
        pytest.skip("derived asm not present")
    findings, checked = declaudit.audit()
    assert checked > 0
    assert findings == []
