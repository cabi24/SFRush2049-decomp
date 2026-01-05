# Source: game_code.bin (decompressed)
# Address: 0x800F7344

glabel graphics_chunk_c
    /* 800F7344 27BDFFD8 */  addiu $sp, $sp, -40
    /* 800F7348 AFBF0024 */  sw $ra, 36($sp)
    /* 800F734C AFB20020 */  sw $s2, 32($sp)
    /* 800F7350 AFB1001C */  sw $s1, 28($sp)
    /* 800F7354 19C00023 */  blez $t6, 0x800F73E4
    /* 800F7358 AFB00018 */  sw $s0, 24($sp)
    /* 800F735C 3C118015 */  lui $s1, 0x8015
    /* 800F7360 26319450 */  addiu $s1, $s1, -27568
    /* 800F7364 2412FFFF */  addiu $s2, $zero, -1
    /* 800F7368 8E300000 */  lw $s0, 0($s1)
    /* 800F736C 8E020028 */  lw $v0, 40($s0)
    /* 800F7370 10400012 */  beq $v0, $zero, 0x800F73BC
    /* 800F7374 00000000 */  nop
    /* 800F7378 8E0F0000 */  lw $t7, 0($s0)
    /* 800F737C 124F000F */  beq $s2, $t7, 0x800F73BC
    /* 800F7380 00000000 */  nop
    /* 800F7384 0040F809 */  jalr $ra, $v0
    /* 800F7388 02002025 */  or $a0, $s0, $zero
    /* 800F738C 1440000B */  bne $v0, $zero, 0x800F73BC
    /* 800F7390 00000000 */  nop
    /* 800F7394 0C02CD63 */  jal 0x800B358C
    /* 800F7398 02002025 */  or $a0, $s0, $zero
    /* 800F739C 3C028015 */  lui $v0, 0x8015
    /* 800F73A0 8C429788 */  lw $v0, -26744($v0)
    /* 800F73A4 3C198015 */  lui $t9, 0x8015
    /* 800F73A8 27399450 */  addiu $t9, $t9, -27568
    /* 800F73AC 0002C080 */  sll $t8, $v0, 2
    /* 800F73B0 2631FFFC */  addiu $s1, $s1, -4
    /* 800F73B4 10000007 */  beq $zero, $zero, 0x800F73D4
    /* 800F73B8 03191821 */  addu $v1, $t8, $t9
    /* 800F73BC 3C028015 */  lui $v0, 0x8015
    /* 800F73C0 8C429788 */  lw $v0, -26744($v0)
    /* 800F73C4 3C098015 */  lui $t1, 0x8015
    /* 800F73C8 25299450 */  addiu $t1, $t1, -27568
    /* 800F73CC 00024080 */  sll $t0, $v0, 2
    /* 800F73D0 01091821 */  addu $v1, $t0, $t1
    /* 800F73D4 26310004 */  addiu $s1, $s1, 4
    /* 800F73D8 0223082B */  sltu $at, $s1, $v1
    /* 800F73DC 5420FFE3 */  .word 0x5420FFE3
    /* 800F73E0 8E300000 */  lw $s0, 0($s1)
    /* 800F73E4 8FBF0024 */  lw $ra, 36($sp)
    /* 800F73E8 8FB00018 */  lw $s0, 24($sp)
    /* 800F73EC 8FB1001C */  lw $s1, 28($sp)
    /* 800F73F0 8FB20020 */  lw $s2, 32($sp)
    /* 800F73F4 03E00008 */  jr $ra
    /* 800F73F8 27BD0028 */  addiu $sp, $sp, 40
