# Source: game_code.bin (decompressed)
# Address: 0x80097470

glabel entity_visibility_calc
    /* 80097470 27BDFFE0 */  addiu $sp, $sp, -32
    /* 80097474 AFA40020 */  sw $a0, 32($sp)
    /* 80097478 AFBF0014 */  sw $ra, 20($sp)
    /* 8009747C AFA50024 */  sw $a1, 36($sp)
    /* 80097480 3C048015 */  lui $a0, 0x8015
    /* 80097484 24842770 */  addiu $a0, $a0, 10096
    /* 80097488 00002825 */  or $a1, $zero, $zero
    /* 8009748C 0C001C9C */  jal 0x80007270
    /* 80097490 24060001 */  addiu $a2, $zero, 1
    /* 80097494 8FA30020 */  lw $v1, 32($sp)
    /* 80097498 8FA40024 */  lw $a0, 36($sp)
    /* 8009749C 00004025 */  or $t0, $zero, $zero
    /* 800974A0 10600003 */  beq $v1, $zero, 0x800974B0
    /* 800974A4 00004825 */  or $t1, $zero, $zero
    /* 800974A8 10000003 */  beq $zero, $zero, 0x800974B8
    /* 800974AC 00601025 */  or $v0, $v1, $zero
    /* 800974B0 3C028015 */  lui $v0, 0x8015
    /* 800974B4 8C4227C8 */  lw $v0, 10184($v0)
    /* 800974B8 0C025CE1 */  jal 0x80097384
    /* 800974BC 00403025 */  or $a2, $v0, $zero
    /* 800974C0 3C048015 */  lui $a0, 0x8015
    /* 800974C4 AFA2001C */  sw $v0, 28($sp)
    /* 800974C8 24842770 */  addiu $a0, $a0, 10096
    /* 800974CC 00002825 */  or $a1, $zero, $zero
    /* 800974D0 0C001D78 */  jal 0x800075E0
    /* 800974D4 00003025 */  or $a2, $zero, $zero
    /* 800974D8 8FBF0014 */  lw $ra, 20($sp)
    /* 800974DC 8FA2001C */  lw $v0, 28($sp)
    /* 800974E0 27BD0020 */  addiu $sp, $sp, 32
    /* 800974E4 03E00008 */  jr $ra
    /* 800974E8 00000000 */  nop
