# Source: game_code.bin (decompressed)
# Address: 0x800D5BB0

glabel stat_best_times_show
    /* 800D5BB0 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800D5BB4 AFB20018 */  sw $s2, 24($sp)
    /* 800D5BB8 00127400 */  sll $t6, $s2, 16
    /* 800D5BBC 000E9403 */  sra $s2, $t6, 16
    /* 800D5BC0 0012C100 */  sll $t8, $s2, 4
    /* 800D5BC4 0312C023 */  subu $t8, $t8, $s2
    /* 800D5BC8 3C198014 */  lui $t9, 0x8014
    /* 800D5BCC 27390808 */  addiu $t9, $t9, 2056
    /* 800D5BD0 0018C0C0 */  sll $t8, $t8, 3
    /* 800D5BD4 4480A000 */  .word 0x4480A000
    /* 800D5BD8 03191821 */  addu $v1, $t8, $t9
    /* 800D5BDC 24040001 */  addiu $a0, $zero, 1
    /* 800D5BE0 00047080 */  sll $t6, $a0, 2
    /* 800D5BE4 01C47023 */  subu $t6, $t6, $a0
    /* 800D5BE8 000E70C0 */  sll $t6, $t6, 3
    /* 800D5BEC AFBF0014 */  sw $ra, 20($sp)
    /* 800D5BF0 AC600014 */  sw $zero, 20($v1)
    /* 800D5BF4 006E1021 */  addu $v0, $v1, $t6
    /* 800D5BF8 E4740000 */  swc1 $f20, 0($v1)
    /* 800D5BFC E4740010 */  swc1 $f20, 16($v1)
    /* 800D5C00 3C0F8014 */  lui $t7, 0x8014
    /* 800D5C04 25EF0AE0 */  addiu $t7, $t7, 2784
    /* 800D5C08 00128080 */  sll $s0, $s2, 2
    /* 800D5C0C AC40005C */  sw $zero, 92($v0)
    /* 800D5C10 AC400044 */  sw $zero, 68($v0)
    /* 800D5C14 AC40002C */  sw $zero, 44($v0)
    /* 800D5C18 AC400014 */  sw $zero, 20($v0)
    /* 800D5C1C 020F8821 */  addu $s1, $s0, $t7
    /* 800D5C20 E4540058 */  swc1 $f20, 88($v0)
    /* 800D5C24 E4540048 */  swc1 $f20, 72($v0)
    /* 800D5C28 E4540040 */  swc1 $f20, 64($v0)
    /* 800D5C2C E4540030 */  swc1 $f20, 48($v0)
    /* 800D5C30 E4540028 */  swc1 $f20, 40($v0)
    /* 800D5C34 E4540018 */  swc1 $f20, 24($v0)
    /* 800D5C38 E4540000 */  swc1 $f20, 0($v0)
    /* 800D5C3C E4540010 */  swc1 $f20, 16($v0)
    /* 800D5C40 0C024701 */  jal 0x80091C04
    /* 800D5C44 8E240000 */  lw $a0, 0($s1)
    /* 800D5C48 2418FFFF */  addiu $t8, $zero, -1
    /* 800D5C4C 0012C840 */  sll $t9, $s2, 1
    /* 800D5C50 3C018014 */  lui $at, 0x8014
    /* 800D5C54 AE380000 */  sw $t8, 0($s1)
    /* 800D5C58 00390821 */  addu $at, $at, $t9
    /* 800D5C5C A4200A08 */  sh $zero, 2568($at)
    /* 800D5C60 3C018014 */  lui $at, 0x8014
    /* 800D5C64 00300821 */  addu $at, $at, $s0
    /* 800D5C68 E4340B10 */  swc1 $f20, 2832($at)
    /* 800D5C6C 3C018014 */  lui $at, 0x8014
    /* 800D5C70 00300821 */  addu $at, $at, $s0
    /* 800D5C74 8FBF0014 */  lw $ra, 20($sp)
    /* 800D5C78 E4340BE0 */  swc1 $f20, 3040($at)
    /* 800D5C7C 3C018014 */  lui $at, 0x8014
    /* 800D5C80 00300821 */  addu $at, $at, $s0
    /* 800D5C84 E4342518 */  swc1 $f20, 9496($at)
    /* 800D5C88 03E00008 */  jr $ra
    /* 800D5C8C 27BD0018 */  addiu $sp, $sp, 24
