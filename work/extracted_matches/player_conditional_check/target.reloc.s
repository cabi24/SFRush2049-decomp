.set noreorder
.set noat
.text
.globl player_conditional_check
player_conditional_check:
.L800D54E0:
    addiu   $sp,$sp,-24
.L800D54E4:
    sw      $ra,20($sp)
.L800D54E8:
    beqz    $a1,.L800D5500
.L800D54EC:
    sw      $a0,24($sp)
.L800D54F0:
    jal     results_screen_update
.L800D54F4:
    lw      $a0,0($a0)
.L800D54F8:
    b       .L800D550C
.L800D54FC:
    nop
.L800D5500:
    lw      $t7,24($sp)
.L800D5504:
    jal     scheduler_recv
.L800D5508:
    lw      $a0,0($t7)
.L800D550C:
    jal     player_conditional_call
.L800D5510:
    lw      $a0,24($sp)
.L800D5514:
    lw      $ra,20($sp)
.L800D5518:
    addiu   $sp,$sp,24
.L800D551C:
    jr      $ra
.L800D5520:
    nop
