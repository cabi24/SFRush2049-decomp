# Source: game_code.bin (decompressed)
# Address: 0x800DE20C

glabel title_prompt_handler
    /* 800DE20C 27BDFF68 */  addiu $sp, $sp, -152
    /* 800DE210 AFB70044 */  sw $s7, 68($sp)
    /* 800DE214 AFA600A0 */  sw $a2, 160($sp)
    /* 800DE218 00A0B825 */  or $s7, $a1, $zero
    /* 800DE21C AFBF004C */  sw $ra, 76($sp)
    /* 800DE220 AFA40098 */  sw $a0, 152($sp)
    /* 800DE224 AFA700A4 */  sw $a3, 164($sp)
    /* 800DE228 3C068011 */  lui $a2, 0x8011
    /* 800DE22C AFBE0048 */  sw $fp, 72($sp)
    /* 800DE230 AFB60040 */  sw $s6, 64($sp)
    /* 800DE234 AFB5003C */  sw $s5, 60($sp)
    /* 800DE238 AFB40038 */  sw $s4, 56($sp)
    /* 800DE23C AFB30034 */  sw $s3, 52($sp)
    /* 800DE240 AFB20030 */  sw $s2, 48($sp)
    /* 800DE244 AFB1002C */  sw $s1, 44($sp)
    /* 800DE248 AFB00028 */  sw $s0, 40($sp)
    /* 800DE24C A3A00097 */  sb $zero, 151($sp)
    /* 800DE250 24C66DBC */  addiu $a2, $a2, 28092
    /* 800DE254 24070001 */  addiu $a3, $zero, 1
    /* 800DE258 00002025 */  or $a0, $zero, $zero
    /* 800DE25C 0C02CDFA */  jal 0x800B37E8
    /* 800DE260 00002825 */  or $a1, $zero, $zero
    /* 800DE264 240E012C */  addiu $t6, $zero, 300
    /* 800DE268 240F00B4 */  addiu $t7, $zero, 180
    /* 800DE26C AFA20090 */  sw $v0, 144($sp)
    /* 800DE270 AFAF0014 */  sw $t7, 20($sp)
    /* 800DE274 AFAE0010 */  sw $t6, 16($sp)
    /* 800DE278 8FA40098 */  lw $a0, 152($sp)
    /* 800DE27C 02E02825 */  or $a1, $s7, $zero
    /* 800DE280 2406000A */  addiu $a2, $zero, 10
    /* 800DE284 0C0377EB */  jal 0x800DDFAC
    /* 800DE288 2407000A */  addiu $a3, $zero, 10
    /* 800DE28C 3C1E8015 */  lui $fp, 0x8015
    /* 800DE290 27DE1AD0 */  addiu $fp, $fp, 6864
    /* 800DE294 87D80000 */  lh $t8, 0($fp)
    /* 800DE298 0000B025 */  or $s6, $zero, $zero
    /* 800DE29C 27B00088 */  addiu $s0, $sp, 136
    /* 800DE2A0 1B000021 */  blez $t8, 0x800DE328
    /* 800DE2A4 27B10080 */  addiu $s1, $sp, 128
    /* 800DE2A8 27B2007C */  addiu $s2, $sp, 124
    /* 800DE2AC 27B30078 */  addiu $s3, $sp, 120
    /* 800DE2B0 27B40074 */  addiu $s4, $sp, 116
    /* 800DE2B4 27B50070 */  addiu $s5, $sp, 112
    /* 800DE2B8 02002025 */  or $a0, $s0, $zero
    /* 800DE2BC 02202825 */  or $a1, $s1, $zero
    /* 800DE2C0 02403025 */  or $a2, $s2, $zero
    /* 800DE2C4 02603825 */  or $a3, $s3, $zero
    /* 800DE2C8 AFB40010 */  sw $s4, 16($sp)
    /* 800DE2CC AFB50014 */  sw $s5, 20($sp)
    /* 800DE2D0 0C029D0A */  jal 0x800A7428
    /* 800DE2D4 AFB60018 */  sw $s6, 24($sp)
    /* 800DE2D8 241900FF */  addiu $t9, $zero, 255
    /* 800DE2DC AFB90014 */  sw $t9, 20($sp)
    /* 800DE2E0 240403E8 */  addiu $a0, $zero, 1000
    /* 800DE2E4 240503E7 */  addiu $a1, $zero, 999
    /* 800DE2E8 00003025 */  or $a2, $zero, $zero
    /* 800DE2EC 00003825 */  or $a3, $zero, $zero
    /* 800DE2F0 AFA00010 */  sw $zero, 16($sp)
    /* 800DE2F4 0C029D20 */  jal 0x800A7480
    /* 800DE2F8 AFB60018 */  sw $s6, 24($sp)
    /* 800DE2FC 87C80000 */  lh $t0, 0($fp)
    /* 800DE300 26D60001 */  addiu $s6, $s6, 1
    /* 800DE304 26100002 */  addiu $s0, $s0, 2
    /* 800DE308 02C8082A */  slt $at, $s6, $t0
    /* 800DE30C 26310002 */  addiu $s1, $s1, 2
    /* 800DE310 26520001 */  addiu $s2, $s2, 1
    /* 800DE314 26730001 */  addiu $s3, $s3, 1
    /* 800DE318 26940001 */  addiu $s4, $s4, 1
    /* 800DE31C 1420FFE6 */  bne $at, $zero, 0x800DE2B8
    /* 800DE320 26B50001 */  addiu $s5, $s5, 1
    /* 800DE324 0000B025 */  or $s6, $zero, $zero
    /* 800DE328 83A90097 */  lb $t1, 151($sp)
    /* 800DE32C 2415000C */  addiu $s5, $zero, 12
    /* 800DE330 2414000B */  addiu $s4, $zero, 11
    /* 800DE334 1520001D */  bne $t1, $zero, 0x800DE3AC
    /* 800DE338 24130007 */  addiu $s3, $zero, 7
    /* 800DE33C 8FB200B0 */  lw $s2, 176($sp)
    /* 800DE340 8FB100AC */  lw $s1, 172($sp)
    /* 800DE344 8FB000A8 */  lw $s0, 168($sp)
    /* 800DE348 0C002F94 */  jal 0x8000BE50
    /* 800DE34C 00002025 */  or $a0, $zero, $zero
    /* 800DE350 14530003 */  bne $v0, $s3, 0x800DE360
    /* 800DE354 00000000 */  nop
    /* 800DE358 0C0326B8 */  jal 0x800C9AE0
    /* 800DE35C 00000000 */  nop
    /* 800DE360 52F4000A */  .word 0x52F4000A
    /* 800DE364 8FA40098 */  lw $a0, 152($sp)
    /* 800DE368 12F50007 */  beq $s7, $s5, 0x800DE388
    /* 800DE36C 2401000D */  addiu $at, $zero, 13
    /* 800DE370 12E10005 */  beq $s7, $at, 0x800DE388
    /* 800DE374 2401000A */  addiu $at, $zero, 10
    /* 800DE378 12E10003 */  beq $s7, $at, 0x800DE388
    /* 800DE37C 00002025 */  or $a0, $zero, $zero
    /* 800DE380 0C03204F */  jal 0x800C813C
    /* 800DE384 00002825 */  or $a1, $zero, $zero
    /* 800DE388 8FA40098 */  lw $a0, 152($sp)
    /* 800DE38C 02002825 */  or $a1, $s0, $zero
    /* 800DE390 02203025 */  or $a2, $s1, $zero
    /* 800DE394 27A70097 */  addiu $a3, $sp, 151
    /* 800DE398 0C03752B */  jal 0x800DD4AC
    /* 800DE39C AFB20010 */  sw $s2, 16($sp)
    /* 800DE3A0 83AA0097 */  lb $t2, 151($sp)
    /* 800DE3A4 1140FFE8 */  beq $t2, $zero, 0x800DE348
    /* 800DE3A8 00000000 */  nop
    /* 800DE3AC 0C02CD63 */  jal 0x800B358C
    /* 800DE3B0 8FA40090 */  lw $a0, 144($sp)
    /* 800DE3B4 87CB0000 */  lh $t3, 0($fp)
    /* 800DE3B8 27B00088 */  addiu $s0, $sp, 136
    /* 800DE3BC 27B10080 */  addiu $s1, $sp, 128
    /* 800DE3C0 19600018 */  blez $t3, 0x800DE424
    /* 800DE3C4 27B2007C */  addiu $s2, $sp, 124
    /* 800DE3C8 27B30078 */  addiu $s3, $sp, 120
    /* 800DE3CC 27B40074 */  addiu $s4, $sp, 116
    /* 800DE3D0 27B50070 */  addiu $s5, $sp, 112
    /* 800DE3D4 928C0000 */  lbu $t4, 0($s4)
    /* 800DE3D8 86040000 */  lh $a0, 0($s0)
    /* 800DE3DC 86250000 */  lh $a1, 0($s1)
    /* 800DE3E0 92460000 */  lbu $a2, 0($s2)
    /* 800DE3E4 92670000 */  lbu $a3, 0($s3)
    /* 800DE3E8 AFAC0010 */  sw $t4, 16($sp)
    /* 800DE3EC 92AD0000 */  lbu $t5, 0($s5)
    /* 800DE3F0 AFB60018 */  sw $s6, 24($sp)
    /* 800DE3F4 0C029D20 */  jal 0x800A7480
    /* 800DE3F8 AFAD0014 */  sw $t5, 20($sp)
    /* 800DE3FC 87CE0000 */  lh $t6, 0($fp)
    /* 800DE400 26D60001 */  addiu $s6, $s6, 1
    /* 800DE404 26100002 */  addiu $s0, $s0, 2
    /* 800DE408 02CE082A */  slt $at, $s6, $t6
    /* 800DE40C 26310002 */  addiu $s1, $s1, 2
    /* 800DE410 26520001 */  addiu $s2, $s2, 1
    /* 800DE414 26730001 */  addiu $s3, $s3, 1
    /* 800DE418 26940001 */  addiu $s4, $s4, 1
    /* 800DE41C 1420FFED */  bne $at, $zero, 0x800DE3D4
    /* 800DE420 26B50001 */  addiu $s5, $s5, 1
    /* 800DE424 8FBF004C */  lw $ra, 76($sp)
    /* 800DE428 8FB00028 */  lw $s0, 40($sp)
    /* 800DE42C 8FB1002C */  lw $s1, 44($sp)
    /* 800DE430 8FB20030 */  lw $s2, 48($sp)
    /* 800DE434 8FB30034 */  lw $s3, 52($sp)
    /* 800DE438 8FB40038 */  lw $s4, 56($sp)
    /* 800DE43C 8FB5003C */  lw $s5, 60($sp)
    /* 800DE440 8FB60040 */  lw $s6, 64($sp)
    /* 800DE444 8FB70044 */  lw $s7, 68($sp)
    /* 800DE448 8FBE0048 */  lw $fp, 72($sp)
    /* 800DE44C 03E00008 */  jr $ra
    /* 800DE450 27BD0098 */  addiu $sp, $sp, 152
