# Source: game_code.bin (decompressed)
# Address: 0x800FAF6C

glabel init_state_continue
    /* 800FAF6C 3C128015 */  lui $s2, 0x8015
    /* 800FAF70 2652A108 */  addiu $s2, $s2, -24312
    /* 800FAF74 86430000 */  lh $v1, 0($s2)
    /* 800FAF78 3C0B8014 */  lui $t3, 0x8014
    /* 800FAF7C 856B2724 */  lh $t3, 10020($t3)
    /* 800FAF80 24040006 */  addiu $a0, $zero, 6
    /* 800FAF84 00831023 */  subu $v0, $a0, $v1
    /* 800FAF88 004B082A */  slt $at, $v0, $t3
    /* 800FAF8C 10200002 */  beq $at, $zero, 0x800FAF98
    /* 800FAF90 00006025 */  or $t4, $zero, $zero
    /* 800FAF94 00405825 */  or $t3, $v0, $zero
    /* 800FAF98 3C028015 */  lui $v0, 0x8015
    /* 800FAF9C 8C42A110 */  lw $v0, -24304($v0)
    /* 800FAFA0 24010004 */  addiu $at, $zero, 4
    /* 800FAFA4 3C088015 */  lui $t0, 0x8015
    /* 800FAFA8 10410006 */  beq $v0, $at, 0x800FAFC4
    /* 800FAFAC 250843CA */  addiu $t0, $t0, 17354
    /* 800FAFB0 24010005 */  addiu $at, $zero, 5
    /* 800FAFB4 50410004 */  .word 0x50410004
    /* 800FAFB8 00005825 */  or $t3, $zero, $zero
    /* 800FAFBC 54820003 */  .word 0x54820003
    /* 800FAFC0 3C014700 */  lui $at, 0x4700
    /* 800FAFC4 00005825 */  or $t3, $zero, $zero
    /* 800FAFC8 3C014700 */  lui $at, 0x4700
    /* 800FAFCC 006B7021 */  addu $t6, $v1, $t3
    /* 800FAFD0 3C068015 */  lui $a2, 0x8015
    /* 800FAFD4 3C168015 */  lui $s6, 0x8015
    /* 800FAFD8 3C148015 */  lui $s4, 0x8015
    /* 800FAFDC 3C138015 */  lui $s3, 0x8015
    /* 800FAFE0 3C108015 */  lui $s0, 0x8015
    /* 800FAFE4 3C0941C6 */  lui $t1, 0x41C6
    /* 800FAFE8 3C078011 */  lui $a3, 0x8011
    /* 800FAFEC 44816000 */  .word 0x44816000
    /* 800FAFF0 A50E0000 */  sh $t6, 0($t0)
    /* 800FAFF4 24E7735C */  addiu $a3, $a3, 29532
    /* 800FAFF8 35294E6D */  ori $t1, $t1, 0x4E6D
    /* 800FAFFC 261043D4 */  addiu $s0, $s0, 17364
    /* 800FB000 26734628 */  addiu $s3, $s3, 17960
    /* 800FB004 26944450 */  addiu $s4, $s4, 17488
    /* 800FB008 26D6978C */  addiu $s6, $s6, -26740
    /* 800FB00C 24C63E88 */  addiu $a2, $a2, 16008
    /* 800FB010 00002825 */  or $a1, $zero, $zero
    /* 800FB014 241E0007 */  addiu $fp, $zero, 7
    /* 800FB018 24170001 */  addiu $s7, $zero, 1
    /* 800FB01C 2415004C */  addiu $s5, $zero, 76
    /* 800FB020 24110006 */  addiu $s1, $zero, 6
    /* 800FB024 240D0003 */  addiu $t5, $zero, 3
    /* 800FB028 240A00B0 */  addiu $t2, $zero, 176
    /* 800FB02C 3C028015 */  lui $v0, 0x8015
    /* 800FB030 8C42A110 */  lw $v0, -24304($v0)
    /* 800FB034 55A20022 */  .word 0x55A20022
    /* 800FB038 864E0000 */  lh $t6, 0($s2)
    /* 800FB03C 850F0000 */  lh $t7, 0($t0)
    /* 800FB040 00AF082A */  slt $at, $a1, $t7
    /* 800FB044 5020001E */  .word 0x5020001E
    /* 800FB048 864E0000 */  lh $t6, 0($s2)
    /* 800FB04C 82780000 */  lb $t8, 0($s3)
    /* 800FB050 5B00001B */  .word 0x5B00001B
    /* 800FB054 864E0000 */  lh $t6, 0($s2)
    /* 800FB058 14A00003 */  bne $a1, $zero, 0x800FB068
    /* 800FB05C 00000000 */  nop
    /* 800FB060 10000008 */  beq $zero, $zero, 0x800FB084
    /* 800FB064 92020000 */  lbu $v0, 0($s0)
    /* 800FB068 14B70005 */  bne $a1, $s7, 0x800FB080
    /* 800FB06C 00A01825 */  or $v1, $a1, $zero
    /* 800FB070 92030000 */  lbu $v1, 0($s0)
    /* 800FB074 38790001 */  xori $t9, $v1, 0x0001
    /* 800FB078 10000001 */  beq $zero, $zero, 0x800FB080
    /* 800FB07C 03201825 */  or $v1, $t9, $zero
    /* 800FB080 00601025 */  or $v0, $v1, $zero
    /* 800FB084 00550019 */  multu $v0, $s5
    /* 800FB088 00007012 */  mflo $t6
    /* 800FB08C 028E7821 */  addu $t7, $s4, $t6
    /* 800FB090 91F80001 */  lbu $t8, 1($t7)
    /* 800FB094 A0CA0006 */  sb $t2, 6($a2)
    /* 800FB098 A0D80005 */  sb $t8, 5($a2)
    /* 800FB09C 86590000 */  lh $t9, 0($s2)
    /* 800FB0A0 00B9082A */  slt $at, $a1, $t9
    /* 800FB0A4 10200003 */  beq $at, $zero, 0x800FB0B4
    /* 800FB0A8 00000000 */  nop
    /* 800FB0AC 10000059 */  beq $zero, $zero, 0x800FB214
    /* 800FB0B0 A0D10007 */  sb $s1, 7($a2)
    /* 800FB0B4 10000057 */  beq $zero, $zero, 0x800FB214
    /* 800FB0B8 A0C00007 */  sb $zero, 7($a2)
    /* 800FB0BC 864E0000 */  lh $t6, 0($s2)
    /* 800FB0C0 00AE082A */  slt $at, $a1, $t6
    /* 800FB0C4 50200007 */  .word 0x50200007
    /* 800FB0C8 850F0000 */  lh $t7, 0($t0)
    /* 800FB0CC A0CB0005 */  sb $t3, 5($a2)
    /* 800FB0D0 256B0001 */  addiu $t3, $t3, 1
    /* 800FB0D4 A0D10007 */  sb $s1, 7($a2)
    /* 800FB0D8 1000004E */  beq $zero, $zero, 0x800FB214
    /* 800FB0DC A0CA0006 */  sb $t2, 6($a2)
    /* 800FB0E0 850F0000 */  lh $t7, 0($t0)
    /* 800FB0E4 00AF082A */  slt $at, $a1, $t7
    /* 800FB0E8 50200049 */  .word 0x50200049
    /* 800FB0EC A0DE0007 */  sb $fp, 7($a2)
    /* 800FB0F0 15A2003F */  bne $t5, $v0, 0x800FB1F0
    /* 800FB0F4 00001825 */  or $v1, $zero, $zero
    /* 800FB0F8 8CF90000 */  lw $t9, 0($a3)
    /* 800FB0FC 85180000 */  lh $t8, 0($t0)
    /* 800FB100 3C028015 */  lui $v0, 0x8015
    /* 800FB104 03290019 */  multu $t9, $t1
    /* 800FB108 44982000 */  .word 0x44982000
    /* 800FB10C 24180001 */  addiu $t8, $zero, 1
    /* 800FB110 3C014F00 */  lui $at, 0x4F00
    /* 800FB114 468020A0 */  .word 0x468020A0
    /* 800FB118 24423E88 */  addiu $v0, $v0, 16008
    /* 800FB11C 00007012 */  mflo $t6
    /* 800FB120 25CF3039 */  addiu $t7, $t6, 12345
    /* 800FB124 000FCC03 */  sra $t9, $t7, 16
    /* 800FB128 332E7FFF */  andi $t6, $t9, 0x7FFF
    /* 800FB12C 448E3000 */  .word 0x448E3000
    /* 800FB130 ACEF0000 */  sw $t7, 0($a3)
    /* 800FB134 46803220 */  .word 0x46803220
    /* 800FB138 46024282 */  .word 0x46024282
    /* 800FB13C 460C5003 */  .word 0x460C5003
    /* 800FB140 444FF800 */  .word 0x444FF800
    /* 800FB144 44D8F800 */  .word 0x44D8F800
    /* 800FB148 00000000 */  nop
    /* 800FB14C 46000124 */  .word 0x46000124
    /* 800FB150 4458F800 */  .word 0x4458F800
    /* 800FB154 00000000 */  nop
    /* 800FB158 33180078 */  andi $t8, $t8, 0x0078
    /* 800FB15C 53000013 */  .word 0x53000013
    /* 800FB160 44182000 */  .word 0x44182000
    /* 800FB164 44812000 */  .word 0x44812000
    /* 800FB168 24180001 */  addiu $t8, $zero, 1
    /* 800FB16C 46040101 */  .word 0x46040101
    /* 800FB170 44D8F800 */  .word 0x44D8F800
    /* 800FB174 00000000 */  nop
    /* 800FB178 46002124 */  .word 0x46002124
    /* 800FB17C 4458F800 */  .word 0x4458F800
    /* 800FB180 00000000 */  nop
    /* 800FB184 33180078 */  andi $t8, $t8, 0x0078
    /* 800FB188 17000005 */  bne $t8, $zero, 0x800FB1A0
    /* 800FB18C 00000000 */  nop
    /* 800FB190 44182000 */  .word 0x44182000
    /* 800FB194 3C018000 */  lui $at, 0x8000
    /* 800FB198 10000007 */  beq $zero, $zero, 0x800FB1B8
    /* 800FB19C 0301C025 */  or $t8, $t8, $at
    /* 800FB1A0 10000005 */  beq $zero, $zero, 0x800FB1B8
    /* 800FB1A4 2418FFFF */  addiu $t8, $zero, -1
    /* 800FB1A8 44182000 */  .word 0x44182000
    /* 800FB1AC 00000000 */  nop
    /* 800FB1B0 0700FFFB */  bltz $t8, 0x800FB1A0
    /* 800FB1B4 00000000 */  nop
    /* 800FB1B8 44CFF800 */  .word 0x44CFF800
    /* 800FB1BC 18A00008 */  blez $a1, 0x800FB1E0
    /* 800FB1C0 A0D80005 */  sb $t8, 5($a2)
    /* 800FB1C4 330400FF */  andi $a0, $t8, 0x00FF
    /* 800FB1C8 90590005 */  lbu $t9, 5($v0)
    /* 800FB1CC 10990004 */  beq $a0, $t9, 0x800FB1E0
    /* 800FB1D0 00000000 */  nop
    /* 800FB1D4 24630001 */  addiu $v1, $v1, 1
    /* 800FB1D8 1465FFFB */  bne $v1, $a1, 0x800FB1C8
    /* 800FB1DC 24420008 */  addiu $v0, $v0, 8
    /* 800FB1E0 50A30008 */  .word 0x50A30008
    /* 800FB1E4 A0C00007 */  sb $zero, 7($a2)
    /* 800FB1E8 1000FFC3 */  beq $zero, $zero, 0x800FB0F8
    /* 800FB1EC 00001825 */  or $v1, $zero, $zero
    /* 800FB1F0 A0CC0005 */  sb $t4, 5($a2)
    /* 800FB1F4 258C0001 */  addiu $t4, $t4, 1
    /* 800FB1F8 318E00FF */  andi $t6, $t4, 0x00FF
    /* 800FB1FC 01C06025 */  or $t4, $t6, $zero
    /* 800FB200 A0C00007 */  sb $zero, 7($a2)
    /* 800FB204 10000003 */  beq $zero, $zero, 0x800FB214
    /* 800FB208 A0CA0006 */  sb $t2, 6($a2)
    /* 800FB20C A0DE0007 */  sb $fp, 7($a2)
    /* 800FB210 A0C00006 */  sb $zero, 6($a2)
    /* 800FB214 82CF0000 */  lb $t7, 0($s6)
    /* 800FB218 24A50001 */  addiu $a1, $a1, 1
    /* 800FB21C 24010006 */  addiu $at, $zero, 6
    /* 800FB220 24C60008 */  addiu $a2, $a2, 8
    /* 800FB224 14A1FF81 */  bne $a1, $at, 0x800FB02C
    /* 800FB228 A0CFFFF8 */  sb $t7, -8($a2)
    /* 800FB22C 03E00008 */  jr $ra
    /* 800FB230 00000000 */  nop
