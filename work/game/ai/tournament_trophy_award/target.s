# Source: game_code.bin (decompressed)
# Address: 0x800DC3F8

glabel tournament_trophy_award
    /* 800DC3F8 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800DC3FC AFBF0014 */  sw $ra, 20($sp)
    /* 800DC400 0C037048 */  jal 0x800DC120
    /* 800DC404 00806025 */  or $t4, $a0, $zero
    /* 800DC408 3C088011 */  lui $t0, 0x8011
    /* 800DC40C 950870F0 */  lhu $t0, 28912($t0)
    /* 800DC410 3C0B8011 */  lui $t3, 0x8011
    /* 800DC414 956B70EC */  lhu $t3, 28908($t3)
    /* 800DC418 00403025 */  or $a2, $v0, $zero
    /* 800DC41C 00003825 */  or $a3, $zero, $zero
    /* 800DC420 1900001A */  blez $t0, 0x800DC48C
    /* 800DC424 01602825 */  or $a1, $t3, $zero
    /* 800DC428 3C098013 */  lui $t1, 0x8013
    /* 800DC42C 2529E618 */  addiu $t1, $t1, -6632
    /* 800DC430 000570C3 */  sra $t6, $a1, 3
    /* 800DC434 012E1021 */  addu $v0, $t1, $t6
    /* 800DC438 904F0000 */  lbu $t7, 0($v0)
    /* 800DC43C 30A40007 */  andi $a0, $a1, 0x0007
    /* 800DC440 24180001 */  addiu $t8, $zero, 1
    /* 800DC444 0098C804 */  .word 0x0098C804
    /* 800DC448 03207027 */  nor $t6, $t9, $zero
    /* 800DC44C 01EEC824 */  and $t9, $t7, $t6
    /* 800DC450 30CF0001 */  andi $t7, $a2, 0x0001
    /* 800DC454 008F7004 */  .word 0x008F7004
    /* 800DC458 24E70001 */  addiu $a3, $a3, 1
    /* 800DC45C 032EC025 */  or $t8, $t9, $t6
    /* 800DC460 30EFFFFF */  andi $t7, $a3, 0xFFFF
    /* 800DC464 24A50001 */  addiu $a1, $a1, 1
    /* 800DC468 A0590000 */  sb $t9, 0($v0)
    /* 800DC46C 30B9FFFF */  andi $t9, $a1, 0xFFFF
    /* 800DC470 01E8082A */  slt $at, $t7, $t0
    /* 800DC474 00067042 */  srl $t6, $a2, 1
    /* 800DC478 01C03025 */  or $a2, $t6, $zero
    /* 800DC47C 03202825 */  or $a1, $t9, $zero
    /* 800DC480 01E03825 */  or $a3, $t7, $zero
    /* 800DC484 1420FFEA */  bne $at, $zero, 0x800DC430
    /* 800DC488 A0580000 */  sb $t8, 0($v0)
    /* 800DC48C 01681021 */  addu $v0, $t3, $t0
    /* 800DC490 3C098013 */  lui $t1, 0x8013
    /* 800DC494 304AFFFF */  andi $t2, $v0, 0xFFFF
    /* 800DC498 2529E618 */  addiu $t1, $t1, -6632
    /* 800DC49C 00003825 */  or $a3, $zero, $zero
    /* 800DC4A0 00002825 */  or $a1, $zero, $zero
    /* 800DC4A4 00002025 */  or $a0, $zero, $zero
    /* 800DC4A8 19400023 */  blez $t2, 0x800DC538
    /* 800DC4AC 00003025 */  or $a2, $zero, $zero
    /* 800DC4B0 3C0B8011 */  lui $t3, 0x8011
    /* 800DC4B4 256B6FE4 */  addiu $t3, $t3, 28644
    /* 800DC4B8 00001825 */  or $v1, $zero, $zero
    /* 800DC4BC 24080004 */  addiu $t0, $zero, 4
    /* 800DC4C0 000378C3 */  sra $t7, $v1, 3
    /* 800DC4C4 012FC821 */  addu $t9, $t1, $t7
    /* 800DC4C8 932E0000 */  lbu $t6, 0($t9)
    /* 800DC4CC 30780007 */  andi $t8, $v1, 0x0007
    /* 800DC4D0 24A50001 */  addiu $a1, $a1, 1
    /* 800DC4D4 030E7807 */  .word 0x030E7807
    /* 800DC4D8 31F90001 */  andi $t9, $t7, 0x0001
    /* 800DC4DC 00997004 */  .word 0x00997004
    /* 800DC4E0 00CE3025 */  or $a2, $a2, $t6
    /* 800DC4E4 30D800FF */  andi $t8, $a2, 0x00FF
    /* 800DC4E8 1504000C */  bne $t0, $a0, 0x800DC51C
    /* 800DC4EC 03003025 */  or $a2, $t8, $zero
    /* 800DC4F0 8D6F0000 */  lw $t7, 0($t3)
    /* 800DC4F4 00003025 */  or $a2, $zero, $zero
    /* 800DC4F8 00002025 */  or $a0, $zero, $zero
    /* 800DC4FC 030FC821 */  addu $t9, $t8, $t7
    /* 800DC500 0187C021 */  addu $t8, $t4, $a3
    /* 800DC504 932E0000 */  lbu $t6, 0($t9)
    /* 800DC508 24E70001 */  addiu $a3, $a3, 1
    /* 800DC50C 30EFFFFF */  andi $t7, $a3, 0xFFFF
    /* 800DC510 01E03825 */  or $a3, $t7, $zero
    /* 800DC514 10000004 */  beq $zero, $zero, 0x800DC528
    /* 800DC518 A30E0000 */  sb $t6, 0($t8)
    /* 800DC51C 24840001 */  addiu $a0, $a0, 1
    /* 800DC520 3099FFFF */  andi $t9, $a0, 0xFFFF
    /* 800DC524 03202025 */  or $a0, $t9, $zero
    /* 800DC528 30A3FFFF */  andi $v1, $a1, 0xFFFF
    /* 800DC52C 006A082A */  slt $at, $v1, $t2
    /* 800DC530 1420FFE3 */  bne $at, $zero, 0x800DC4C0
    /* 800DC534 00602825 */  or $a1, $v1, $zero
    /* 800DC538 3C0B8011 */  lui $t3, 0x8011
    /* 800DC53C 10800009 */  beq $a0, $zero, 0x800DC564
    /* 800DC540 256B6FE4 */  addiu $t3, $t3, 28644
    /* 800DC544 8D780000 */  lw $t8, 0($t3)
    /* 800DC548 01877021 */  addu $t6, $t4, $a3
    /* 800DC54C 24E70001 */  addiu $a3, $a3, 1
    /* 800DC550 00D87821 */  addu $t7, $a2, $t8
    /* 800DC554 91F90000 */  lbu $t9, 0($t7)
    /* 800DC558 30F8FFFF */  andi $t8, $a3, 0xFFFF
    /* 800DC55C 03003825 */  or $a3, $t8, $zero
    /* 800DC560 A1D90000 */  sb $t9, 0($t6)
    /* 800DC564 01877821 */  addu $t7, $t4, $a3
    /* 800DC568 A1E00000 */  sb $zero, 0($t7)
    /* 800DC56C 8FBF0014 */  lw $ra, 20($sp)
    /* 800DC570 27BD0018 */  addiu $sp, $sp, 24
    /* 800DC574 03E00008 */  jr $ra
    /* 800DC578 00000000 */  nop
