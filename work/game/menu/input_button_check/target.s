# Source: game_code.bin (decompressed)
# Address: 0x800E7580

glabel input_button_check
    /* 800E7580 808D0000 */  lb $t5, 0($a0)
    /* 800E7584 A08B0000 */  sb $t3, 0($a0)
    /* 800E7588 808D0000 */  lb $t5, 0($a0)
    /* 800E758C 25290004 */  addiu $t1, $t1, 4
    /* 800E7590 55A00006 */  .word 0x55A00006
    /* 800E7594 AC800004 */  sw $zero, 4($a0)
    /* 800E7598 A0C00003 */  sb $zero, 3($a2)
    /* 800E759C 80CE0003 */  lb $t6, 3($a2)
    /* 800E75A0 A4C00000 */  sh $zero, 0($a2)
    /* 800E75A4 A0CE0002 */  sb $t6, 2($a2)
    /* 800E75A8 AC800004 */  sw $zero, 4($a0)
    /* 800E75AC 80C80003 */  lb $t0, 3($a2)
    /* 800E75B0 94C50000 */  lhu $a1, 0($a2)
    /* 800E75B4 8C6F0000 */  lw $t7, 0($v1)
    /* 800E75B8 01E5C024 */  and $t8, $t7, $a1
    /* 800E75BC 53000007 */  .word 0x53000007
    /* 800E75C0 24420001 */  addiu $v0, $v0, 1
    /* 800E75C4 8C990004 */  lw $t9, 4($a0)
    /* 800E75C8 240C0001 */  addiu $t4, $zero, 1
    /* 800E75CC 004C6804 */  .word 0x004C6804
    /* 800E75D0 032D7025 */  or $t6, $t9, $t5
    /* 800E75D4 AC8E0004 */  sw $t6, 4($a0)
    /* 800E75D8 24420001 */  addiu $v0, $v0, 1
    /* 800E75DC 1447FFF5 */  bne $v0, $a3, 0x800E75B4
    /* 800E75E0 24630004 */  addiu $v1, $v1, 4
    /* 800E75E4 80CF0002 */  lb $t7, 2($a2)
    /* 800E75E8 24840010 */  addiu $a0, $a0, 16
    /* 800E75EC 24C60006 */  addiu $a2, $a2, 6
    /* 800E75F0 A08FFFF8 */  sb $t7, -8($a0)
    /* 800E75F4 A088FFF9 */  sb $t0, -7($a0)
    /* 800E75F8 A080FFFA */  sb $zero, -6($a0)
    /* 800E75FC A080FFFB */  sb $zero, -5($a0)
    /* 800E7600 A080FFFC */  sb $zero, -4($a0)
    /* 800E7604 148AFFCC */  bne $a0, $t2, 0x800E7538
    /* 800E7608 A080FFFD */  sb $zero, -3($a0)
    /* 800E760C 0C039C4D */  jal 0x800E7134
    /* 800E7610 00000000 */  nop
    /* 800E7614 24020001 */  addiu $v0, $zero, 1
    /* 800E7618 8FBF001C */  lw $ra, 28($sp)
    /* 800E761C 8FB00018 */  lw $s0, 24($sp)
    /* 800E7620 27BD0078 */  addiu $sp, $sp, 120
    /* 800E7624 03E00008 */  jr $ra
    /* 800E7628 00000000 */  nop
