# Source: game_code.bin (decompressed)
# Address: 0x800FD238

glabel input_aux_handler
    /* 800FD238 3C028011 */  lui $v0, 0x8011
    /* 800FD23C 8C4274B4 */  lw $v0, 29876($v0)
    /* 800FD240 27BDFFE8 */  addiu $sp, $sp, -24
    /* 800FD244 AFBF0014 */  sw $ra, 20($sp)
    /* 800FD248 304E0002 */  andi $t6, $v0, 0x0002
    /* 800FD24C 11C00005 */  beq $t6, $zero, 0x800FD264
    /* 800FD250 00027840 */  sll $t7, $v0, 1
    /* 800FD254 0C0E68A8 */  jal 0x8039A2A0
    /* 800FD258 00000000 */  nop
    /* 800FD25C 10000066 */  beq $zero, $zero, 0x800FD3F8
    /* 800FD260 00000000 */  nop
    /* 800FD264 05E10005 */  bgez $t7, 0x800FD27C
    /* 800FD268 30580004 */  andi $t8, $v0, 0x0004
    /* 800FD26C 0C0E2900 */  jal 0x8038A400
    /* 800FD270 00000000 */  nop
    /* 800FD274 10000060 */  beq $zero, $zero, 0x800FD3F8
    /* 800FD278 00000000 */  nop
    /* 800FD27C 13000005 */  beq $t8, $zero, 0x800FD294
    /* 800FD280 30590008 */  andi $t9, $v0, 0x0008
    /* 800FD284 0C0E3D15 */  jal 0x8038F454
    /* 800FD288 00000000 */  nop
    /* 800FD28C 1000005A */  beq $zero, $zero, 0x800FD3F8
    /* 800FD290 00000000 */  nop
    /* 800FD294 13200005 */  beq $t9, $zero, 0x800FD2AC
    /* 800FD298 304E0010 */  andi $t6, $v0, 0x0010
    /* 800FD29C 0C03F409 */  jal 0x800FD024
    /* 800FD2A0 00000000 */  nop
    /* 800FD2A4 10000054 */  beq $zero, $zero, 0x800FD3F8
    /* 800FD2A8 00000000 */  nop
    /* 800FD2AC 11C0000C */  beq $t6, $zero, 0x800FD2E0
    /* 800FD2B0 304F2000 */  andi $t7, $v0, 0x2000
    /* 800FD2B4 3C018014 */  lui $at, 0x8014
    /* 800FD2B8 A0200A04 */  sb $zero, 2564($at)
    /* 800FD2BC 24040036 */  addiu $a0, $zero, 54
    /* 800FD2C0 0C025DA5 */  jal 0x80097694
    /* 800FD2C4 2405FFFF */  addiu $a1, $zero, -1
    /* 800FD2C8 0C02596F */  jal 0x800965BC
    /* 800FD2CC 00402025 */  or $a0, $v0, $zero
    /* 800FD2D0 0C0E4106 */  jal 0x80390418
    /* 800FD2D4 00000000 */  nop
    /* 800FD2D8 10000047 */  beq $zero, $zero, 0x800FD3F8
    /* 800FD2DC 00000000 */  nop
    /* 800FD2E0 11E00005 */  beq $t7, $zero, 0x800FD2F8
    /* 800FD2E4 30580020 */  andi $t8, $v0, 0x0020
    /* 800FD2E8 0C0E5016 */  jal 0x80394058
    /* 800FD2EC 00000000 */  nop
    /* 800FD2F0 10000041 */  beq $zero, $zero, 0x800FD3F8
    /* 800FD2F4 00000000 */  nop
    /* 800FD2F8 13000005 */  beq $t8, $zero, 0x800FD310
    /* 800FD2FC 30590040 */  andi $t9, $v0, 0x0040
    /* 800FD300 0C0E610B */  jal 0x8039842C
    /* 800FD304 00000000 */  nop
    /* 800FD308 1000003B */  beq $zero, $zero, 0x800FD3F8
    /* 800FD30C 00000000 */  nop
    /* 800FD310 13200005 */  beq $t9, $zero, 0x800FD328
    /* 800FD314 304E4000 */  andi $t6, $v0, 0x4000
    /* 800FD318 0C0E6744 */  jal 0x80399D10
    /* 800FD31C 00000000 */  nop
    /* 800FD320 10000035 */  beq $zero, $zero, 0x800FD3F8
    /* 800FD324 00000000 */  nop
    /* 800FD328 11C00005 */  beq $t6, $zero, 0x800FD340
    /* 800FD32C 304F0080 */  andi $t7, $v0, 0x0080
    /* 800FD330 0C0E7050 */  jal 0x8039C140
    /* 800FD334 00000000 */  nop
    /* 800FD338 1000002F */  beq $zero, $zero, 0x800FD3F8
    /* 800FD33C 00000000 */  nop
    /* 800FD340 11E00005 */  beq $t7, $zero, 0x800FD358
    /* 800FD344 30580100 */  andi $t8, $v0, 0x0100
    /* 800FD348 0C0E3977 */  jal 0x8038E5DC
    /* 800FD34C 00000000 */  nop
    /* 800FD350 10000029 */  beq $zero, $zero, 0x800FD3F8
    /* 800FD354 00000000 */  nop
    /* 800FD358 13000005 */  beq $t8, $zero, 0x800FD370
    /* 800FD35C 30590200 */  andi $t9, $v0, 0x0200
    /* 800FD360 0C0E87AB */  jal 0x803A1EAC
    /* 800FD364 00000000 */  nop
    /* 800FD368 10000023 */  beq $zero, $zero, 0x800FD3F8
    /* 800FD36C 00000000 */  nop
    /* 800FD370 13200005 */  beq $t9, $zero, 0x800FD388
    /* 800FD374 304E0400 */  andi $t6, $v0, 0x0400
    /* 800FD378 0C036C78 */  jal 0x800DB1E0
    /* 800FD37C 00000000 */  nop
    /* 800FD380 1000001D */  beq $zero, $zero, 0x800FD3F8
    /* 800FD384 00000000 */  nop
    /* 800FD388 11C00005 */  beq $t6, $zero, 0x800FD3A0
    /* 800FD38C 304F0800 */  andi $t7, $v0, 0x0800
    /* 800FD390 0C036468 */  jal 0x800D91A0
    /* 800FD394 00000000 */  nop
    /* 800FD398 10000017 */  beq $zero, $zero, 0x800FD3F8
    /* 800FD39C 00000000 */  nop
    /* 800FD3A0 11E00005 */  beq $t7, $zero, 0x800FD3B8
    /* 800FD3A4 30581000 */  andi $t8, $v0, 0x1000
    /* 800FD3A8 0C035D8D */  jal 0x800D7634
    /* 800FD3AC 00000000 */  nop
    /* 800FD3B0 10000011 */  beq $zero, $zero, 0x800FD3F8
    /* 800FD3B4 00000000 */  nop
    /* 800FD3B8 13000005 */  beq $t8, $zero, 0x800FD3D0
    /* 800FD3BC 0002C940 */  sll $t9, $v0, 5
    /* 800FD3C0 0C02E305 */  jal 0x800B8C14
    /* 800FD3C4 00000000 */  nop
    /* 800FD3C8 1000000B */  beq $zero, $zero, 0x800FD3F8
    /* 800FD3CC 00000000 */  nop
    /* 800FD3D0 07210005 */  bgez $t9, 0x800FD3E8
    /* 800FD3D4 00027100 */  sll $t6, $v0, 4
    /* 800FD3D8 0C0E6434 */  jal 0x803990D0
    /* 800FD3DC 00000000 */  nop
    /* 800FD3E0 10000005 */  beq $zero, $zero, 0x800FD3F8
    /* 800FD3E4 00000000 */  nop
    /* 800FD3E8 05C10003 */  bgez $t6, 0x800FD3F8
    /* 800FD3EC 00000000 */  nop
    /* 800FD3F0 0C0E7244 */  jal 0x8039C910
    /* 800FD3F4 00000000 */  nop
    /* 800FD3F8 0C03F27E */  jal 0x800FC9F8
    /* 800FD3FC 00000000 */  nop
    /* 800FD400 00002025 */  or $a0, $zero, $zero
    /* 800FD404 0C03204F */  jal 0x800C813C
    /* 800FD408 00002825 */  or $a1, $zero, $zero
    /* 800FD40C 8FBF0014 */  lw $ra, 20($sp)
    /* 800FD410 27BD0018 */  addiu $sp, $sp, 24
    /* 800FD414 03E00008 */  jr $ra
    /* 800FD418 00000000 */  nop
