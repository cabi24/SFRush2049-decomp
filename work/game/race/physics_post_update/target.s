# Source: game_code.bin (decompressed)
# Address: 0x800F7454

glabel physics_post_update
    /* 800F7454 27BDFFE0 */  addiu $sp, $sp, -32
    /* 800F7458 AFBF0014 */  sw $ra, 20($sp)
    /* 800F745C 246E0008 */  addiu $t6, $v1, 8
    /* 800F7460 AFA40020 */  sw $a0, 32($sp)
    /* 800F7464 ACAE0000 */  sw $t6, 0($a1)
    /* 800F7468 3C0FE700 */  lui $t7, 0xE700
    /* 800F746C AC6F0000 */  sw $t7, 0($v1)
    /* 800F7470 AC600004 */  sw $zero, 4($v1)
    /* 800F7474 8CA80000 */  lw $t0, 0($a1)
    /* 800F7478 3C198003 */  lui $t9, 0x8003
    /* 800F747C 3C01FF10 */  lui $at, 0xFF10
    /* 800F7480 25180008 */  addiu $t8, $t0, 8
    /* 800F7484 ACB80000 */  sw $t8, 0($a1)
    /* 800F7488 8F39AFC0 */  lw $t9, -20544($t9)
    /* 800F748C 3C0D8016 */  lui $t5, 0x8016
    /* 800F7490 25ADF72D */  addiu $t5, $t5, -2259
    /* 800F7494 272AFFFF */  addiu $t2, $t9, -1
    /* 800F7498 314B0FFF */  andi $t3, $t2, 0x0FFF
    /* 800F749C 01616025 */  or $t4, $t3, $at
    /* 800F74A0 AD0C0000 */  sw $t4, 0($t0)
    /* 800F74A4 81AE0000 */  lb $t6, 0($t5)
    /* 800F74A8 3C048015 */  lui $a0, 0x8015
    /* 800F74AC AFA80018 */  sw $t0, 24($sp)
    /* 800F74B0 000E79C0 */  sll $t7, $t6, 7
    /* 800F74B4 008F2021 */  addu $a0, $a0, $t7
    /* 800F74B8 0C003570 */  jal 0x8000D5C0
    /* 800F74BC 8C846C5C */  lw $a0, 27740($a0)
    /* 800F74C0 8FA80018 */  lw $t0, 24($sp)
    /* 800F74C4 97A90022 */  lhu $t1, 34($sp)
    /* 800F74C8 3C058015 */  lui $a1, 0x8015
    /* 800F74CC 24A597C8 */  addiu $a1, $a1, -26680
    /* 800F74D0 AD020004 */  sw $v0, 4($t0)
    /* 800F74D4 8CA30000 */  lw $v1, 0($a1)
    /* 800F74D8 00095400 */  sll $t2, $t1, 16
    /* 800F74DC 3C19F700 */  lui $t9, 0xF700
    /* 800F74E0 24780008 */  addiu $t8, $v1, 8
    /* 800F74E4 ACB80000 */  sw $t8, 0($a1)
    /* 800F74E8 01495825 */  or $t3, $t2, $t1
    /* 800F74EC AC6B0004 */  sw $t3, 4($v1)
    /* 800F74F0 AC790000 */  sw $t9, 0($v1)
    /* 800F74F4 8CA30000 */  lw $v1, 0($a1)
    /* 800F74F8 3C0D8003 */  lui $t5, 0x8003
    /* 800F74FC 3C0A8003 */  lui $t2, 0x8003
    /* 800F7500 246C0008 */  addiu $t4, $v1, 8
    /* 800F7504 ACAC0000 */  sw $t4, 0($a1)
    /* 800F7508 8DADAFC0 */  lw $t5, -20544($t5)
    /* 800F750C 8D4AAFC4 */  lw $t2, -20540($t2)
    /* 800F7510 3C01F600 */  lui $at, 0xF600
    /* 800F7514 25AEFFFF */  addiu $t6, $t5, -1
    /* 800F7518 31CF03FF */  andi $t7, $t6, 0x03FF
    /* 800F751C 254BFFFF */  addiu $t3, $t2, -1
    /* 800F7520 316C03FF */  andi $t4, $t3, 0x03FF
    /* 800F7524 000FC380 */  sll $t8, $t7, 14
    /* 800F7528 0301C825 */  or $t9, $t8, $at
    /* 800F752C 000C6880 */  sll $t5, $t4, 2
    /* 800F7530 032D7025 */  or $t6, $t9, $t5
    /* 800F7534 AC6E0000 */  sw $t6, 0($v1)
    /* 800F7538 AC600004 */  sw $zero, 4($v1)
    /* 800F753C 8CA30000 */  lw $v1, 0($a1)
    /* 800F7540 3C18E700 */  lui $t8, 0xE700
    /* 800F7544 246F0008 */  addiu $t7, $v1, 8
    /* 800F7548 ACAF0000 */  sw $t7, 0($a1)
    /* 800F754C AC600004 */  sw $zero, 4($v1)
    /* 800F7550 AC780000 */  sw $t8, 0($v1)
    /* 800F7554 8FBF0014 */  lw $ra, 20($sp)
    /* 800F7558 27BD0020 */  addiu $sp, $sp, 32
    /* 800F755C 03E00008 */  jr $ra
    /* 800F7560 00000000 */  nop
