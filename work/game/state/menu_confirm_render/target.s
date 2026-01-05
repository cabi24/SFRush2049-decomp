# Source: game_code.bin (decompressed)
# Address: 0x800CD300

glabel menu_confirm_render
    /* 800CD300 24840012 */  addiu $a0, $a0, 18
    /* 800CD304 54400008 */  .word 0x54400008
    /* 800CD308 8E180000 */  lw $t8, 0($s0)
    /* 800CD30C 8E040000 */  lw $a0, 0($s0)
    /* 800CD310 02C02825 */  or $a1, $s6, $zero
    /* 800CD314 0C022B41 */  jal 0x8008AD04
    /* 800CD318 24840035 */  addiu $a0, $a0, 53
    /* 800CD31C 10400005 */  beq $v0, $zero, 0x800CD334
    /* 800CD320 00000000 */  nop
    /* 800CD324 8E180000 */  lw $t8, 0($s0)
    /* 800CD328 8F100000 */  lw $s0, 0($t8)
    /* 800CD32C 5600FFF2 */  .word 0x5600FFF2
    /* 800CD330 8E040000 */  lw $a0, 0($s0)
    /* 800CD334 12000003 */  beq $s0, $zero, 0x800CD344
    /* 800CD338 26520001 */  addiu $s2, $s2, 1
    /* 800CD33C 1000FF8E */  beq $zero, $zero, 0x800CD178
    /* 800CD340 00008025 */  or $s0, $zero, $zero
    /* 800CD344 3C0E4E52 */  lui $t6, 0x4E52
    /* 800CD348 35CE5545 */  ori $t6, $t6, 0x5545
    /* 800CD34C 3C068012 */  lui $a2, 0x8012
    /* 800CD350 240F3544 */  addiu $t7, $zero, 13636
    /* 800CD354 AFAF0010 */  sw $t7, 16($sp)
    /* 800CD358 24C611E0 */  addiu $a2, $a2, 4576
    /* 800CD35C AFAE0014 */  sw $t6, 20($sp)
    /* 800CD360 8FA400E0 */  lw $a0, 224($sp)
    /* 800CD364 03C02825 */  or $a1, $fp, $zero
    /* 800CD368 0C028B53 */  jal 0x800A2D4C
    /* 800CD36C 24070810 */  addiu $a3, $zero, 2064
    /* 800CD370 14400003 */  bne $v0, $zero, 0x800CD380
    /* 800CD374 00403825 */  or $a3, $v0, $zero
    /* 800CD378 10000064 */  beq $zero, $zero, 0x800CD50C
    /* 800CD37C 00001025 */  or $v0, $zero, $zero
    /* 800CD380 8CE40000 */  lw $a0, 0($a3)
    /* 800CD384 3C0E8014 */  lui $t6, 0x8014
    /* 800CD388 25CEC128 */  addiu $t6, $t6, -16088
    /* 800CD38C 90830010 */  lbu $v1, 16($a0)
    /* 800CD390 90850011 */  lbu $a1, 17($a0)
    /* 800CD394 0003C980 */  sll $t9, $v1, 6
    /* 800CD398 0005C080 */  sll $t8, $a1, 2
    /* 800CD39C 03387821 */  addu $t7, $t9, $t8
    /* 800CD3A0 0003C880 */  sll $t9, $v1, 2
    /* 800CD3A4 01EEA821 */  addu $s5, $t7, $t6
    /* 800CD3A8 0323C823 */  subu $t9, $t9, $v1
    /* 800CD3AC 0305C023 */  subu $t8, $t8, $a1
    /* 800CD3B0 0018C100 */  sll $t8, $t8, 4
    /* 800CD3B4 0019CA00 */  sll $t9, $t9, 8
    /* 800CD3B8 3C0E8014 */  lui $t6, 0x8014
    /* 800CD3BC 25CEE5D8 */  addiu $t6, $t6, -6696
    /* 800CD3C0 03387821 */  addu $t7, $t9, $t8
    /* 800CD3C4 01EEF021 */  addu $fp, $t7, $t6
    /* 800CD3C8 AEBE0000 */  sw $fp, 0($s5)
    /* 800CD3CC 8CF80000 */  lw $t8, 0($a3)
    /* 800CD3D0 8F0F0048 */  lw $t7, 72($t8)
    /* 800CD3D4 AFCF002C */  sw $t7, 44($fp)
    /* 800CD3D8 8DF00000 */  lw $s0, 0($t7)
    /* 800CD3DC AFB500DC */  sw $s5, 220($sp)
    /* 800CD3E0 AFA700CC */  sw $a3, 204($sp)
    /* 800CD3E4 0C033333 */  jal 0x800CCCCC
    /* 800CD3E8 AFB000D0 */  sw $s0, 208($sp)
    /* 800CD3EC 8FA700CC */  lw $a3, 204($sp)
    /* 800CD3F0 8FB000D0 */  lw $s0, 208($sp)
    /* 800CD3F4 8FB300DC */  lw $s3, 220($sp)
    /* 800CD3F8 AFC70008 */  sw $a3, 8($fp)
    /* 800CD3FC 8CF80000 */  lw $t8, 0($a3)
    /* 800CD400 3C198009 */  lui $t9, 0x8009
    /* 800CD404 27394C28 */  addiu $t9, $t9, 19496
    /* 800CD408 AF190008 */  sw $t9, 8($t8)
    /* 800CD40C 8FCE0008 */  lw $t6, 8($fp)
    /* 800CD410 3C0F8009 */  lui $t7, 0x8009
    /* 800CD414 25EF49D4 */  addiu $t7, $t7, 18900
    /* 800CD418 8DD90000 */  lw $t9, 0($t6)
    /* 800CD41C 3C188003 */  lui $t8, 0x8003
    /* 800CD420 2718E8E8 */  addiu $t8, $t8, -5912
    /* 800CD424 AF2F000C */  sw $t7, 12($t9)
    /* 800CD428 8F0E027C */  lw $t6, 636($t8)
    /* 800CD42C 001378C0 */  sll $t7, $s3, 3
    /* 800CD430 2611000D */  addiu $s1, $s0, 13
    /* 800CD434 AE0E0004 */  sw $t6, 4($s0)
    /* 800CD438 8FB900E0 */  lw $t9, 224($sp)
    /* 800CD43C 02202025 */  or $a0, $s1, $zero
    /* 800CD440 02E02825 */  or $a1, $s7, $zero
    /* 800CD444 01F9C025 */  or $t8, $t7, $t9
    /* 800CD448 0C0291CF */  jal 0x800A473C
    /* 800CD44C AE180008 */  sw $t8, 8($s0)
    /* 800CD450 26170004 */  addiu $s7, $s0, 4
    /* 800CD454 02102823 */  subu $a1, $s0, $s0
    /* 800CD458 24A50018 */  addiu $a1, $a1, 24
    /* 800CD45C 0C02D19B */  jal 0x800B466C
    /* 800CD460 02E02025 */  or $a0, $s7, $zero
    /* 800CD464 27D20014 */  addiu $s2, $fp, 20
    /* 800CD468 AE020000 */  sw $v0, 0($s0)
    /* 800CD46C 02402025 */  or $a0, $s2, $zero
    /* 800CD470 02202825 */  or $a1, $s1, $zero
    /* 800CD474 0C001F1A */  jal 0x80007C68
    /* 800CD478 2406000D */  addiu $a2, $zero, 13
    /* 800CD47C 27C4000C */  addiu $a0, $fp, 12
    /* 800CD480 02E02825 */  or $a1, $s7, $zero
    /* 800CD484 0C001F1A */  jal 0x80007C68
    /* 800CD488 24060008 */  addiu $a2, $zero, 8
    /* 800CD48C 8FC40008 */  lw $a0, 8($fp)
    /* 800CD490 02002825 */  or $a1, $s0, $zero
    /* 800CD494 0C028941 */  jal 0x800A2504
    /* 800CD498 24060810 */  addiu $a2, $zero, 2064
    /* 800CD49C 8FC40008 */  lw $a0, 8($fp)
    /* 800CD4A0 0C0288DE */  jal 0x800A2378
    /* 800CD4A4 00002825 */  or $a1, $zero, $zero
    /* 800CD4A8 8FCE0008 */  lw $t6, 8($fp)
    /* 800CD4AC 24050001 */  addiu $a1, $zero, 1
    /* 800CD4B0 8DCF0000 */  lw $t7, 0($t6)
    /* 800CD4B4 0C028869 */  jal 0x800A21A4
    /* 800CD4B8 91E40010 */  lbu $a0, 16($t7)
    /* 800CD4BC 3C118013 */  lui $s1, 0x8013
    /* 800CD4C0 2631E6D8 */  addiu $s1, $s1, -6440
    /* 800CD4C4 8E300008 */  lw $s0, 8($s1)
    /* 800CD4C8 5200000C */  .word 0x5200000C
    /* 800CD4CC 02202025 */  or $a0, $s1, $zero
    /* 800CD4D0 8E040000 */  lw $a0, 0($s0)
    /* 800CD4D4 02402825 */  or $a1, $s2, $zero
    /* 800CD4D8 0C022B41 */  jal 0x8008AD04
    /* 800CD4DC 24840014 */  addiu $a0, $a0, 20
    /* 800CD4E0 5C400006 */  .word 0x5C400006
    /* 800CD4E4 02202025 */  or $a0, $s1, $zero
    /* 800CD4E8 8E190000 */  lw $t9, 0($s0)
    /* 800CD4EC 8F300000 */  lw $s0, 0($t9)
    /* 800CD4F0 5600FFF8 */  .word 0x5600FFF8
    /* 800CD4F4 8E040000 */  lw $a0, 0($s0)
    /* 800CD4F8 02202025 */  or $a0, $s1, $zero
    /* 800CD4FC 02602825 */  or $a1, $s3, $zero
    /* 800CD500 0C0247EF */  jal 0x80091FBC
    /* 800CD504 02003025 */  or $a2, $s0, $zero
    /* 800CD508 02601025 */  or $v0, $s3, $zero
    /* 800CD50C 8FBF0054 */  lw $ra, 84($sp)
    /* 800CD510 D7B40020 */  .word 0xD7B40020
    /* 800CD514 D7B60028 */  .word 0xD7B60028
    /* 800CD518 8FB00030 */  lw $s0, 48($sp)
    /* 800CD51C 8FB10034 */  lw $s1, 52($sp)
    /* 800CD520 8FB20038 */  lw $s2, 56($sp)
    /* 800CD524 8FB3003C */  lw $s3, 60($sp)
    /* 800CD528 8FB40040 */  lw $s4, 64($sp)
    /* 800CD52C 8FB50044 */  lw $s5, 68($sp)
    /* 800CD530 8FB60048 */  lw $s6, 72($sp)
    /* 800CD534 8FB7004C */  lw $s7, 76($sp)
    /* 800CD538 8FBE0050 */  lw $fp, 80($sp)
    /* 800CD53C 03E00008 */  jr $ra
    /* 800CD540 27BD00E0 */  addiu $sp, $sp, 224
