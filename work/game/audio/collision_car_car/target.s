# Source: game_code.bin (decompressed)
# Address: 0x800BB400

glabel collision_car_car
    /* 800BB400 02997021 */  addu $t6, $s4, $t9
    /* 800BB404 8DCF0160 */  lw $t7, 352($t6)
    /* 800BB408 24790001 */  addiu $t9, $v1, 1
    /* 800BB40C 25F80200 */  addiu $t8, $t7, 512
    /* 800BB410 0058082B */  sltu $at, $v0, $t8
    /* 800BB414 50200035 */  .word 0x50200035
    /* 800BB418 24640058 */  addiu $a0, $v1, 88
    /* 800BB41C 0335001A */  div $t9, $s5
    /* 800BB420 00007010 */  mfhi $t6
    /* 800BB424 A22E0001 */  sb $t6, 1($s1)
    /* 800BB428 16A00002 */  bne $s5, $zero, 0x800BB434
    /* 800BB42C 00000000 */  nop
    /* 800BB430 0007000D */  .word 0x0007000D
    /* 800BB434 2401FFFF */  addiu $at, $zero, -1
    /* 800BB438 16A10004 */  bne $s5, $at, 0x800BB44C
    /* 800BB43C 3C018000 */  lui $at, 0x8000
    /* 800BB440 17210002 */  bne $t9, $at, 0x800BB44C
    /* 800BB444 00000000 */  nop
    /* 800BB448 0006000D */  .word 0x0006000D
    /* 800BB44C 1615FFE7 */  bne $s0, $s5, 0x800BB3EC
    /* 800BB450 00000000 */  nop
    /* 800BB454 44972000 */  .word 0x44972000
    /* 800BB458 3C038015 */  lui $v1, 0x8015
    /* 800BB45C 8463A108 */  lh $v1, -24312($v1)
    /* 800BB460 8E640000 */  lw $a0, 0($s3)
    /* 800BB464 468020A0 */  .word 0x468020A0
    /* 800BB468 00960019 */  multu $a0, $s6
    /* 800BB46C 0077082A */  slt $at, $v1, $s7
    /* 800BB470 00007812 */  mflo $t7
    /* 800BB474 25E43039 */  addiu $a0, $t7, 12345
    /* 800BB478 0004CC03 */  sra $t9, $a0, 16
    /* 800BB47C 332E7FFF */  andi $t6, $t9, 0x7FFF
    /* 800BB480 448E3000 */  .word 0x448E3000
    /* 800BB484 AE640000 */  sw $a0, 0($s3)
    /* 800BB488 46803220 */  .word 0x46803220
    /* 800BB48C 46024282 */  .word 0x46024282
    /* 800BB490 46145003 */  .word 0x46145003
    /* 800BB494 4600010D */  .word 0x4600010D
    /* 800BB498 44022000 */  .word 0x44022000
    /* 800BB49C 10200007 */  beq $at, $zero, 0x800BB4BC
    /* 800BB4A0 00408025 */  or $s0, $v0, $zero
    /* 800BB4A4 0010C0C0 */  sll $t8, $s0, 3
    /* 800BB4A8 03D8C821 */  addu $t9, $fp, $t8
    /* 800BB4AC 932E0001 */  lbu $t6, 1($t9)
    /* 800BB4B0 29C1000D */  slti $at, $t6, 13
    /* 800BB4B4 1020FFEC */  beq $at, $zero, 0x800BB468
    /* 800BB4B8 00000000 */  nop
    /* 800BB4BC 000278C0 */  sll $t7, $v0, 3
    /* 800BB4C0 03CFC021 */  addu $t8, $fp, $t7
    /* 800BB4C4 93190001 */  lbu $t9, 1($t8)
    /* 800BB4C8 3C0F8014 */  lui $t7, 0x8014
    /* 800BB4CC 00027040 */  sll $t6, $v0, 1
    /* 800BB4D0 01EE7821 */  addu $t7, $t7, $t6
    /* 800BB4D4 A2390001 */  sb $t9, 1($s1)
    /* 800BB4D8 85EF2B08 */  lh $t7, 11016($t7)
    /* 800BB4DC 8FB8005C */  lw $t8, 92($sp)
    /* 800BB4E0 1000000A */  beq $zero, $zero, 0x800BB50C
    /* 800BB4E4 A70F0000 */  sh $t7, 0($t8)
    /* 800BB4E8 24640058 */  addiu $a0, $v1, 88
    /* 800BB4EC 24050001 */  addiu $a1, $zero, 1
    /* 800BB4F0 00003025 */  or $a2, $zero, $zero
    /* 800BB4F4 00003825 */  or $a3, $zero, $zero
    /* 800BB4F8 0C025DE6 */  jal 0x80097798
    /* 800BB4FC AFA00010 */  sw $zero, 16($sp)
    /* 800BB500 8FB9005C */  lw $t9, 92($sp)
    /* 800BB504 26F70001 */  addiu $s7, $s7, 1
    /* 800BB508 A7220000 */  sh $v0, 0($t9)
    /* 800BB50C 02402025 */  or $a0, $s2, $zero
    /* 800BB510 92250001 */  lbu $a1, 1($s1)
    /* 800BB514 0C02EC0B */  jal 0x800BB02C
    /* 800BB518 00003025 */  or $a2, $zero, $zero
    /* 800BB51C 3C058015 */  lui $a1, 0x8015
    /* 800BB520 1000004A */  beq $zero, $zero, 0x800BB64C
    /* 800BB524 24A5A250 */  addiu $a1, $a1, -23984
    /* 800BB528 2A410006 */  slti $at, $s2, 6
    /* 800BB52C 10200041 */  beq $at, $zero, 0x800BB634
    /* 800BB530 00127200 */  sll $t6, $s2, 8
    /* 800BB534 01D27021 */  addu $t6, $t6, $s2
    /* 800BB538 000E70C0 */  sll $t6, $t6, 3
    /* 800BB53C 00AE7821 */  addu $t7, $a1, $t6
    /* 800BB540 81F807CC */  lb $t8, 1996($t7)
    /* 800BB544 24010002 */  addiu $at, $zero, 2
    /* 800BB548 5701003B */  .word 0x5701003B
    /* 800BB54C 93D80001 */  lbu $t8, 1($fp)
    /* 800BB550 0C02EBE6 */  jal 0x800BAF98
    /* 800BB554 00002025 */  or $a0, $zero, $zero
    /* 800BB558 92230001 */  lbu $v1, 1($s1)
    /* 800BB55C 0003C880 */  sll $t9, $v1, 2
    /* 800BB560 02997021 */  addu $t6, $s4, $t9
    /* 800BB564 8DCF0160 */  lw $t7, 352($t6)
    /* 800BB568 301900FF */  andi $t9, $zero, 0x00FF
    /* 800BB56C 25F80200 */  addiu $t8, $t7, 512
    /* 800BB570 0058082B */  sltu $at, $v0, $t8
    /* 800BB574 1020001F */  beq $at, $zero, 0x800BB5F4
    /* 800BB578 2B21000D */  slti $at, $t9, 13
    /* 800BB57C 10200010 */  beq $at, $zero, 0x800BB5C0
    /* 800BB580 A2200001 */  sb $zero, 1($s1)
    /* 800BB584 0C02EBE6 */  jal 0x800BAF98
    /* 800BB588 00002025 */  or $a0, $zero, $zero
    /* 800BB58C 92250001 */  lbu $a1, 1($s1)
    /* 800BB590 00057080 */  sll $t6, $a1, 2
    /* 800BB594 028E7821 */  addu $t7, $s4, $t6
    /* 800BB598 8DF80160 */  lw $t8, 352($t7)
    /* 800BB59C 24AE0001 */  addiu $t6, $a1, 1
    /* 800BB5A0 31CF00FF */  andi $t7, $t6, 0x00FF
    /* 800BB5A4 27190200 */  addiu $t9, $t8, 512
    /* 800BB5A8 0059082B */  sltu $at, $v0, $t9
    /* 800BB5AC 10200011 */  beq $at, $zero, 0x800BB5F4
    /* 800BB5B0 00A01825 */  or $v1, $a1, $zero
    /* 800BB5B4 29E1000D */  slti $at, $t7, 13
    /* 800BB5B8 1420FFF2 */  bne $at, $zero, 0x800BB584
    /* 800BB5BC A22E0001 */  sb $t6, 1($s1)
    /* 800BB5C0 93D80001 */  lbu $t8, 1($fp)
    /* 800BB5C4 3C198014 */  lui $t9, 0x8014
    /* 800BB5C8 8FAE005C */  lw $t6, 92($sp)
    /* 800BB5CC A2380001 */  sb $t8, 1($s1)
    /* 800BB5D0 87392B08 */  lh $t9, 11016($t9)
    /* 800BB5D4 02402025 */  or $a0, $s2, $zero
    /* 800BB5D8 00003025 */  or $a2, $zero, $zero
    /* 800BB5DC A5D90000 */  sh $t9, 0($t6)
    /* 800BB5E0 0C02EC0B */  jal 0x800BB02C
    /* 800BB5E4 92250001 */  lbu $a1, 1($s1)
    /* 800BB5E8 3C058015 */  lui $a1, 0x8015
    /* 800BB5EC 10000017 */  beq $zero, $zero, 0x800BB64C
    /* 800BB5F0 24A5A250 */  addiu $a1, $a1, -23984
    /* 800BB5F4 24640058 */  addiu $a0, $v1, 88
    /* 800BB5F8 24050001 */  addiu $a1, $zero, 1
    /* 800BB5FC 00003025 */  or $a2, $zero, $zero
    /* 800BB600 00003825 */  or $a3, $zero, $zero
    /* 800BB604 0C025DE6 */  jal 0x80097798
    /* 800BB608 AFA00010 */  sw $zero, 16($sp)
    /* 800BB60C 8FAF005C */  lw $t7, 92($sp)
    /* 800BB610 02402025 */  or $a0, $s2, $zero
    /* 800BB614 00003025 */  or $a2, $zero, $zero
    /* 800BB618 A5E20000 */  sh $v0, 0($t7)
    /* 800BB61C 0C02EC0B */  jal 0x800BB02C
    /* 800BB620 92250001 */  lbu $a1, 1($s1)
    /* 800BB624 26F70001 */  addiu $s7, $s7, 1
    /* 800BB628 3C058015 */  lui $a1, 0x8015
    /* 800BB62C 10000007 */  beq $zero, $zero, 0x800BB64C
    /* 800BB630 24A5A250 */  addiu $a1, $a1, -23984
    /* 800BB634 93D80001 */  lbu $t8, 1($fp)
    /* 800BB638 3C198014 */  lui $t9, 0x8014
    /* 800BB63C 8FAE005C */  lw $t6, 92($sp)
    /* 800BB640 A2380001 */  sb $t8, 1($s1)
    /* 800BB644 87392B08 */  lh $t9, 11016($t9)
    /* 800BB648 A5D90000 */  sh $t9, 0($t6)
    /* 800BB64C 8FAF005C */  lw $t7, 92($sp)
    /* 800BB650 26520001 */  addiu $s2, $s2, 1
    /* 800BB654 26310008 */  addiu $s1, $s1, 8
    /* 800BB658 25F80002 */  addiu $t8, $t7, 2
    /* 800BB65C 1655FEE0 */  bne $s2, $s5, 0x800BB1E0
    /* 800BB660 AFB8005C */  sw $t8, 92($sp)
    /* 800BB664 8FBF0054 */  lw $ra, 84($sp)
    /* 800BB668 D7B40020 */  .word 0xD7B40020
    /* 800BB66C D7B60028 */  .word 0xD7B60028
    /* 800BB670 8FB00030 */  lw $s0, 48($sp)
    /* 800BB674 8FB10034 */  lw $s1, 52($sp)
    /* 800BB678 8FB20038 */  lw $s2, 56($sp)
    /* 800BB67C 8FB3003C */  lw $s3, 60($sp)
    /* 800BB680 8FB40040 */  lw $s4, 64($sp)
    /* 800BB684 8FB50044 */  lw $s5, 68($sp)
    /* 800BB688 8FB60048 */  lw $s6, 72($sp)
    /* 800BB68C 8FB7004C */  lw $s7, 76($sp)
    /* 800BB690 8FBE0050 */  lw $fp, 80($sp)
    /* 800BB694 03E00008 */  jr $ra
    /* 800BB698 27BD00A8 */  addiu $sp, $sp, 168
