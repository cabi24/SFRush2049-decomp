# Source: game_code.bin (decompressed)
# Address: 0x800CA300

glabel state_change_preprocess
    /* 800CA300 3C028011 */  lui $v0, 0x8011
    /* 800CA304 8C4274B4 */  lw $v0, 29876($v0)
    /* 800CA308 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800CA30C AFBF0014 */  sw $ra, 20($sp)
    /* 800CA310 304E4000 */  andi $t6, $v0, 0x4000
    /* 800CA314 11C00005 */  beq $t6, $zero, 0x800CA32C
    /* 800CA318 304F0100 */  andi $t7, $v0, 0x0100
    /* 800CA31C 0C0292B1 */  jal 0x800A4AC4
    /* 800CA320 24040039 */  addiu $a0, $zero, 57
    /* 800CA324 10000020 */  beq $zero, $zero, 0x800CA3A8
    /* 800CA328 8FBF0014 */  lw $ra, 20($sp)
    /* 800CA32C 11E00007 */  beq $t7, $zero, 0x800CA34C
    /* 800CA330 30580080 */  andi $t8, $v0, 0x0080
    /* 800CA334 0C0292B1 */  jal 0x800A4AC4
    /* 800CA338 24040038 */  addiu $a0, $zero, 56
    /* 800CA33C 0C0296EE */  jal 0x800A5BB8
    /* 800CA340 00000000 */  nop
    /* 800CA344 10000018 */  beq $zero, $zero, 0x800CA3A8
    /* 800CA348 8FBF0014 */  lw $ra, 20($sp)
    /* 800CA34C 13000005 */  beq $t8, $zero, 0x800CA364
    /* 800CA350 0002C880 */  sll $t9, $v0, 2
    /* 800CA354 0C0292B1 */  jal 0x800A4AC4
    /* 800CA358 2404003C */  addiu $a0, $zero, 60
    /* 800CA35C 10000012 */  beq $zero, $zero, 0x800CA3A8
    /* 800CA360 8FBF0014 */  lw $ra, 20($sp)
    /* 800CA364 0721000B */  bgez $t9, 0x800CA394
    /* 800CA368 000270C0 */  sll $t6, $v0, 3
    /* 800CA36C 0C0292B1 */  jal 0x800A4AC4
    /* 800CA370 24040037 */  addiu $a0, $zero, 55
    /* 800CA374 0C0324CD */  jal 0x800C9334
    /* 800CA378 24080036 */  addiu $t0, $zero, 54
    /* 800CA37C 0C0324CD */  jal 0x800C9334
    /* 800CA380 2408003A */  addiu $t0, $zero, 58
    /* 800CA384 0C0324CD */  jal 0x800C9334
    /* 800CA388 2408003B */  addiu $t0, $zero, 59
    /* 800CA38C 10000006 */  beq $zero, $zero, 0x800CA3A8
    /* 800CA390 8FBF0014 */  lw $ra, 20($sp)
    /* 800CA394 05C30004 */  .word 0x05C30004
    /* 800CA398 8FBF0014 */  lw $ra, 20($sp)
    /* 800CA39C 0C032438 */  jal 0x800C90E0
    /* 800CA3A0 00000000 */  nop
    /* 800CA3A4 8FBF0014 */  lw $ra, 20($sp)
    /* 800CA3A8 27BD0018 */  addiu $sp, $sp, 24
    /* 800CA3AC 03E00008 */  jr $ra
    /* 800CA3B0 00000000 */  nop
