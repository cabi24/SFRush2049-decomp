# Source: game_code.bin (decompressed)
# Address: 0x800DD0D0

glabel audio_settings
    /* 800DD0D0 27BDFE48 */  addiu $sp, $sp, -440
    /* 800DD0D4 3C178014 */  lui $s7, 0x8014
    /* 800DD0D8 02817024 */  and $t6, $s4, $at
    /* 800DD0DC 26F761D0 */  addiu $s7, $s7, 25040
    /* 800DD0E0 AFBF0024 */  sw $ra, 36($sp)
    /* 800DD0E4 2DD60001 */  sltiu $s6, $t6, 1
    /* 800DD0E8 02C0A025 */  or $s4, $s6, $zero
    /* 800DD0EC 02E02025 */  or $a0, $s7, $zero
    /* 800DD0F0 00002825 */  or $a1, $zero, $zero
    /* 800DD0F4 0C001C9C */  jal 0x80007270
    /* 800DD0F8 24060001 */  addiu $a2, $zero, 1
    /* 800DD0FC 0C02D080 */  jal 0x800B4200
    /* 800DD100 2412000B */  addiu $s2, $zero, 11
    /* 800DD104 00408025 */  or $s0, $v0, $zero
    /* 800DD108 02E02025 */  or $a0, $s7, $zero
    /* 800DD10C 00002825 */  or $a1, $zero, $zero
    /* 800DD110 0C001D78 */  jal 0x800075E0
    /* 800DD114 00003025 */  or $a2, $zero, $zero
    /* 800DD118 0C02DD28 */  jal 0x800B74A0
    /* 800DD11C 24040001 */  addiu $a0, $zero, 1
    /* 800DD120 3C1E8018 */  lui $fp, 0x8018
    /* 800DD124 27DEA4E0 */  addiu $fp, $fp, -23328
    /* 800DD128 8FD80004 */  lw $t8, 4($fp)
    /* 800DD12C 27B000AC */  addiu $s0, $sp, 172
    /* 800DD130 02002025 */  or $a0, $s0, $zero
    /* 800DD134 0C02F9A9 */  jal 0x800BE6A4
    /* 800DD138 8F0501A0 */  lw $a1, 416($t8)
    /* 800DD13C 12800004 */  beq $s4, $zero, 0x800DD150
    /* 800DD140 02002025 */  or $a0, $s0, $zero
    /* 800DD144 8FD90004 */  lw $t9, 4($fp)
    /* 800DD148 10000003 */  beq $zero, $zero, 0x800DD158
    /* 800DD14C 8F34019C */  lw $s4, 412($t9)
    /* 800DD150 8FCE0004 */  lw $t6, 4($fp)
    /* 800DD154 8DD40198 */  lw $s4, 408($t6)
    /* 800DD158 0C02F93C */  jal 0x800BE4F0
    /* 800DD15C 02802825 */  or $a1, $s4, $zero
    /* 800DD160 3C088012 */  lui $t0, 0x8012
    /* 800DD164 25088E20 */  addiu $t0, $t0, -29152
    /* 800DD168 24030001 */  addiu $v1, $zero, 1
    /* 800DD16C AD030000 */  sw $v1, 0($t0)
    /* 800DD170 12C00003 */  beq $s6, $zero, 0x800DD180
    /* 800DD174 AD030004 */  sw $v1, 4($t0)
    /* 800DD178 10000002 */  beq $zero, $zero, 0x800DD184
    /* 800DD17C 24030028 */  addiu $v1, $zero, 40
    /* 800DD180 24030014 */  addiu $v1, $zero, 20
    /* 800DD184 12C00003 */  beq $s6, $zero, 0x800DD194
    /* 800DD188 8FAF01B8 */  lw $t7, 440($sp)
    /* 800DD18C 10000002 */  beq $zero, $zero, 0x800DD198
    /* 800DD190 24140028 */  addiu $s4, $zero, 40
    /* 800DD194 24140014 */  addiu $s4, $zero, 20
    /* 800DD198 8FB901BC */  lw $t9, 444($sp)
    /* 800DD19C 000FC080 */  sll $t8, $t7, 2
    /* 800DD1A0 030FC023 */  subu $t8, $t8, $t7
    /* 800DD1A4 00197080 */  sll $t6, $t9, 2
    /* 800DD1A8 01D97023 */  subu $t6, $t6, $t9
    /* 800DD1AC 000E7080 */  sll $t6, $t6, 2
    /* 800DD1B0 0018C080 */  sll $t8, $t8, 2
    /* 800DD1B4 01D97023 */  subu $t6, $t6, $t9
    /* 800DD1B8 030FC023 */  subu $t8, $t8, $t7
    /* 800DD1BC 0018C0C0 */  sll $t8, $t8, 3
    /* 800DD1C0 000E7080 */  sll $t6, $t6, 2
    /* 800DD1C4 3C198015 */  lui $t9, 0x8015
    /* 800DD1C8 27393FD8 */  addiu $t9, $t9, 16344
    /* 800DD1CC 030E7821 */  addu $t7, $t8, $t6
    /* 800DD1D0 01F9A821 */  addu $s5, $t7, $t9
    /* 800DD1D4 86A6001C */  lh $a2, 28($s5)
    /* 800DD1D8 86AE0018 */  lh $t6, 24($s5)
    /* 800DD1DC 86A2001E */  lh $v0, 30($s5)
    /* 800DD1E0 04C10003 */  bgez $a2, 0x800DD1F0
    /* 800DD1E4 0006C043 */  sra $t8, $a2, 1
    /* 800DD1E8 24C10001 */  addiu $at, $a2, 1
    /* 800DD1EC 0001C043 */  sra $t8, $at, 1
    /* 800DD1F0 030E2021 */  addu $a0, $t8, $t6
    /* 800DD1F4 00047C00 */  sll $t7, $a0, 16
    /* 800DD1F8 000FCC03 */  sra $t9, $t7, 16
    /* 800DD1FC 86AF001A */  lh $t7, 26($s5)
    /* 800DD200 0043C023 */  subu $t8, $v0, $v1
    /* 800DD204 03202025 */  or $a0, $t9, $zero
    /* 800DD208 07010003 */  bgez $t8, 0x800DD218
    /* 800DD20C 00187043 */  sra $t6, $t8, 1
    /* 800DD210 27010001 */  addiu $at, $t8, 1
    /* 800DD214 00017043 */  sra $t6, $at, 1
    /* 800DD218 01CF2821 */  addu $a1, $t6, $t7
    /* 800DD21C 24A50014 */  addiu $a1, $a1, 20
    /* 800DD220 0005CC00 */  sll $t9, $a1, 16
    /* 800DD224 00192C03 */  sra $a1, $t9, 16
    /* 800DD228 00543823 */  subu $a3, $v0, $s4
    /* 800DD22C 00077400 */  sll $t6, $a3, 16
    /* 800DD230 2419FFFF */  addiu $t9, $zero, -1
    /* 800DD234 AFB90010 */  sw $t9, 16($sp)
    /* 800DD238 000E3C03 */  sra $a3, $t6, 16
    /* 800DD23C AFA00014 */  sw $zero, 20($sp)
    /* 800DD240 0C02F81E */  jal 0x800BE078
    /* 800DD244 AFB00018 */  sw $s0, 24($sp)
    /* 800DD248 12C0006C */  beq $s6, $zero, 0x800DD3FC
    /* 800DD24C 00000000 */  nop
    /* 800DD250 0C02CFD4 */  jal 0x800B3F50
    /* 800DD254 00000000 */  nop
    /* 800DD258 86B8001C */  lh $t8, 28($s5)
    /* 800DD25C 86AF0018 */  lh $t7, 24($s5)
    /* 800DD260 86B9001E */  lh $t9, 30($s5)
    /* 800DD264 00408825 */  or $s1, $v0, $zero
    /* 800DD268 07010003 */  bgez $t8, 0x800DD278
    /* 800DD26C 00187043 */  sra $t6, $t8, 1
    /* 800DD270 27010001 */  addiu $at, $t8, 1
    /* 800DD274 00017043 */  sra $t6, $at, 1
    /* 800DD278 86B8001A */  lh $t8, 26($s5)
    /* 800DD27C 01CF9821 */  addu $s3, $t6, $t7
    /* 800DD280 00027880 */  sll $t7, $v0, 2
    /* 800DD284 03387021 */  addu $t6, $t9, $t8
    /* 800DD288 82B80001 */  lb $t8, 1($s5)
    /* 800DD28C 01E27823 */  subu $t7, $t7, $v0
    /* 800DD290 01CFC823 */  subu $t9, $t6, $t7
    /* 800DD294 13000003 */  beq $t8, $zero, 0x800DD2A4
    /* 800DD298 AFB901B0 */  sw $t9, 432($sp)
    /* 800DD29C 10000002 */  beq $zero, $zero, 0x800DD2A8
    /* 800DD2A0 2414000B */  addiu $s4, $zero, 11
    /* 800DD2A4 2414000A */  addiu $s4, $zero, 10
    /* 800DD2A8 02E02025 */  or $a0, $s7, $zero
    /* 800DD2AC 00002825 */  or $a1, $zero, $zero
    /* 800DD2B0 0C001C9C */  jal 0x80007270
    /* 800DD2B4 24060001 */  addiu $a2, $zero, 1
    /* 800DD2B8 02809025 */  or $s2, $s4, $zero
    /* 800DD2BC AFB101AC */  sw $s1, 428($sp)
    /* 800DD2C0 0C02D080 */  jal 0x800B4200
    /* 800DD2C4 AFB301B4 */  sw $s3, 436($sp)
    /* 800DD2C8 8FB101AC */  lw $s1, 428($sp)
    /* 800DD2CC 8FB301B4 */  lw $s3, 436($sp)
    /* 800DD2D0 00408025 */  or $s0, $v0, $zero
    /* 800DD2D4 02E02025 */  or $a0, $s7, $zero
    /* 800DD2D8 00002825 */  or $a1, $zero, $zero
    /* 800DD2DC 0C001D78 */  jal 0x800075E0
    /* 800DD2E0 00003025 */  or $a2, $zero, $zero
    /* 800DD2E4 82AE0001 */  lb $t6, 1($s5)
    /* 800DD2E8 51C00004 */  .word 0x51C00004
    /* 800DD2EC 24140016 */  addiu $s4, $zero, 22
    /* 800DD2F0 10000002 */  beq $zero, $zero, 0x800DD2FC
    /* 800DD2F4 24140001 */  addiu $s4, $zero, 1
    /* 800DD2F8 24140016 */  addiu $s4, $zero, 22
    /* 800DD2FC 0C02DD28 */  jal 0x800B74A0
    /* 800DD300 02802025 */  or $a0, $s4, $zero
    /* 800DD304 82AF0001 */  lb $t7, 1($s5)
    /* 800DD308 0013B400 */  sll $s6, $s3, 16
    /* 800DD30C 0016CC03 */  sra $t9, $s6, 16
    /* 800DD310 11E00003 */  beq $t7, $zero, 0x800DD320
    /* 800DD314 0320B025 */  or $s6, $t9, $zero
    /* 800DD318 10000002 */  beq $zero, $zero, 0x800DD324
    /* 800DD31C 0000A025 */  or $s4, $zero, $zero
    /* 800DD320 24140001 */  addiu $s4, $zero, 1
    /* 800DD324 8FAE01B0 */  lw $t6, 432($sp)
    /* 800DD328 00192400 */  sll $a0, $t9, 16
    /* 800DD32C 0004C403 */  sra $t8, $a0, 16
    /* 800DD330 01D47823 */  subu $t7, $t6, $s4
    /* 800DD334 8FCE0004 */  lw $t6, 4($fp)
    /* 800DD338 01F12821 */  addu $a1, $t7, $s1
    /* 800DD33C 0005CC00 */  sll $t9, $a1, 16
    /* 800DD340 00192C03 */  sra $a1, $t9, 16
    /* 800DD344 03002025 */  or $a0, $t8, $zero
    /* 800DD348 0C02DC75 */  jal 0x800B71D4
    /* 800DD34C 8DC603B0 */  lw $a2, 944($t6)
    /* 800DD350 82AF0001 */  lb $t7, 1($s5)
    /* 800DD354 02E02025 */  or $a0, $s7, $zero
    /* 800DD358 00002825 */  or $a1, $zero, $zero
    /* 800DD35C 11E00003 */  beq $t7, $zero, 0x800DD36C
    /* 800DD360 2414000B */  addiu $s4, $zero, 11
    /* 800DD364 10000001 */  beq $zero, $zero, 0x800DD36C
    /* 800DD368 2414000A */  addiu $s4, $zero, 10
    /* 800DD36C 0C001C9C */  jal 0x80007270
    /* 800DD370 24060001 */  addiu $a2, $zero, 1
    /* 800DD374 0C02D080 */  jal 0x800B4200
    /* 800DD378 02809025 */  or $s2, $s4, $zero
    /* 800DD37C 00408025 */  or $s0, $v0, $zero
    /* 800DD380 02E02025 */  or $a0, $s7, $zero
    /* 800DD384 00002825 */  or $a1, $zero, $zero
    /* 800DD388 0C001D78 */  jal 0x800075E0
    /* 800DD38C 00003025 */  or $a2, $zero, $zero
    /* 800DD390 82B90001 */  lb $t9, 1($s5)
    /* 800DD394 24140001 */  addiu $s4, $zero, 1
    /* 800DD398 13200003 */  beq $t9, $zero, 0x800DD3A8
    /* 800DD39C 00000000 */  nop
    /* 800DD3A0 10000001 */  beq $zero, $zero, 0x800DD3A8
    /* 800DD3A4 24140016 */  addiu $s4, $zero, 22
    /* 800DD3A8 0C02DD28 */  jal 0x800B74A0
    /* 800DD3AC 02802025 */  or $a0, $s4, $zero
    /* 800DD3B0 82B80001 */  lb $t8, 1($s5)
    /* 800DD3B4 00162400 */  sll $a0, $s6, 16
    /* 800DD3B8 8FAF01B0 */  lw $t7, 432($sp)
    /* 800DD3BC 13000003 */  beq $t8, $zero, 0x800DD3CC
    /* 800DD3C0 0000A025 */  or $s4, $zero, $zero
    /* 800DD3C4 10000001 */  beq $zero, $zero, 0x800DD3CC
    /* 800DD3C8 24140001 */  addiu $s4, $zero, 1
    /* 800DD3CC 01F42823 */  subu $a1, $t7, $s4
    /* 800DD3D0 8FCF000C */  lw $t7, 12($fp)
    /* 800DD3D4 0005CC00 */  sll $t9, $a1, 16
    /* 800DD3D8 00192C03 */  sra $a1, $t9, 16
    /* 800DD3DC 00047403 */  sra $t6, $a0, 16
    /* 800DD3E0 95F90032 */  lhu $t9, 50($t7)
    /* 800DD3E4 01C02025 */  or $a0, $t6, $zero
    /* 800DD3E8 8FCE0010 */  lw $t6, 16($fp)
    /* 800DD3EC 0019C080 */  sll $t8, $t9, 2
    /* 800DD3F0 01D87821 */  addu $t7, $t6, $t8
    /* 800DD3F4 0C02DC75 */  jal 0x800B71D4
    /* 800DD3F8 8DE60000 */  lw $a2, 0($t7)
    /* 800DD3FC 3C028012 */  lui $v0, 0x8012
    /* 800DD400 24428E20 */  addiu $v0, $v0, -29152
    /* 800DD404 24190003 */  addiu $t9, $zero, 3
    /* 800DD408 AC590004 */  sw $t9, 4($v0)
    /* 800DD40C AC400000 */  sw $zero, 0($v0)
    /* 800DD410 02E02025 */  or $a0, $s7, $zero
    /* 800DD414 00002825 */  or $a1, $zero, $zero
    /* 800DD418 0C001C9C */  jal 0x80007270
    /* 800DD41C 24060001 */  addiu $a2, $zero, 1
    /* 800DD420 0C02D080 */  jal 0x800B4200
    /* 800DD424 2412000B */  addiu $s2, $zero, 11
    /* 800DD428 00408025 */  or $s0, $v0, $zero
    /* 800DD42C 02E02025 */  or $a0, $s7, $zero
    /* 800DD430 00002825 */  or $a1, $zero, $zero
    /* 800DD434 0C001D78 */  jal 0x800075E0
    /* 800DD438 00003025 */  or $a2, $zero, $zero
    /* 800DD43C 8FBF0024 */  lw $ra, 36($sp)
    /* 800DD440 27BD01B8 */  addiu $sp, $sp, 440
    /* 800DD444 03E00008 */  jr $ra
    /* 800DD448 00000000 */  nop
