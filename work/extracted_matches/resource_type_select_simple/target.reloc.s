.set noreorder
.set noat
.text
.globl resource_type_select_simple
resource_type_select_simple:
.L800FEC60:
    addiu   $sp,$sp,-24
.L800FEC64:
    andi    $t6,$a0,0x1
.L800FEC68:
    beqz    $t6,.L800FEC78
.L800FEC6C:
    sw      $ra,20($sp)
.L800FEC70:
    b       .L800FEC7C
.L800FEC74:
    li      $a0,46
.L800FEC78:
    li      $a0,38
.L800FEC7C:
    move    $a1,$zero
.L800FEC80:
    li      $a2,1
.L800FEC84:
    jal     entity_flags_apply
.L800FEC88:
    move    $a3,$zero
.L800FEC8C:
    lw      $ra,20($sp)
.L800FEC90:
    addiu   $sp,$sp,24
.L800FEC94:
    jr      $ra
.L800FEC98:
    nop
