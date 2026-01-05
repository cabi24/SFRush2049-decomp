# Source: game_code.bin (decompressed)
# Address: 0x800EEA7C

glabel game_late_init
    /* 800EEA7C 27BDFF80 */  addiu $sp, $sp, -128
    /* 800EEA80 AFBF002C */  sw $ra, 44($sp)
    /* 800EEA84 AFB10028 */  sw $s1, 40($sp)
    /* 800EEA88 AFB00024 */  sw $s0, 36($sp)
    /* 800EEA8C 24040032 */  addiu $a0, $zero, 50
    /* 800EEA90 0C039F43 */  jal 0x800E7D0C
    /* 800EEA94 2405000A */  addiu $a1, $zero, 10
    /* 800EEA98 0C0337BB */  jal 0x800CDEEC
    /* 800EEA9C 00000000 */  nop
    /* 800EEAA0 3C018016 */  lui $at, 0x8016
    /* 800EEAA4 AC20F738 */  sw $zero, -2248($at)
    /* 800EEAA8 3C018016 */  lui $at, 0x8016
    /* 800EEAAC AC201380 */  sw $zero, 4992($at)
    /* 800EEAB0 3C018016 */  lui $at, 0x8016
    /* 800EEAB4 AC201398 */  sw $zero, 5016($at)
    /* 800EEAB8 3C018016 */  lui $at, 0x8016
    /* 800EEABC AC2013A4 */  sw $zero, 5028($at)
    /* 800EEAC0 3C0142B4 */  lui $at, 0x42B4
    /* 800EEAC4 3C028000 */  lui $v0, 0x8000
    /* 800EEAC8 8C420300 */  lw $v0, 768($v0)
    /* 800EEACC 44812000 */  .word 0x44812000
    /* 800EEAD0 3C018015 */  lui $at, 0x8015
    /* 800EEAD4 14400003 */  bne $v0, $zero, 0x800EEAE4
    /* 800EEAD8 E4244188 */  swc1 $f4, 16776($at)
    /* 800EEADC 1040FFFF */  beq $v0, $zero, 0x800EEADC
    /* 800EEAE0 00000000 */  nop
    /* 800EEAE4 0C00077F */  jal 0x80001DFC
    /* 800EEAE8 00000000 */  nop
    /* 800EEAEC 3C048000 */  lui $a0, 0x8000
    /* 800EEAF0 3C068003 */  lui $a2, 0x8003
    /* 800EEAF4 3C078003 */  lui $a3, 0x8003
    /* 800EEAF8 8CE7AFC4 */  lw $a3, -20540($a3)
    /* 800EEAFC 8CC6AFC0 */  lw $a2, -20544($a2)
    /* 800EEB00 8C840300 */  lw $a0, 768($a0)
    /* 800EEB04 0C0006D1 */  jal 0x80001B44
    /* 800EEB08 00402825 */  or $a1, $v0, $zero
    /* 800EEB0C 3C050002 */  lui $a1, 0x0002
    /* 800EEB10 34A52620 */  ori $a1, $a1, 0x2620
    /* 800EEB14 0C025D1C */  jal 0x80097470
    /* 800EEB18 00002025 */  or $a0, $zero, $zero
    /* 800EEB1C 3C038003 */  lui $v1, 0x8003
    /* 800EEB20 2463EBB0 */  addiu $v1, $v1, -5200
    /* 800EEB24 AC620000 */  sw $v0, 0($v1)
    /* 800EEB28 00402025 */  or $a0, $v0, $zero
    /* 800EEB2C 0C0258B5 */  jal 0x800962D4
    /* 800EEB30 00002825 */  or $a1, $zero, $zero
    /* 800EEB34 3C050002 */  lui $a1, 0x0002
    /* 800EEB38 34A52620 */  ori $a1, $a1, 0x2620
    /* 800EEB3C 0C0330F0 */  jal 0x800CC3C0
    /* 800EEB40 00002025 */  or $a0, $zero, $zero
    /* 800EEB44 3C038015 */  lui $v1, 0x8015
    /* 800EEB48 24636CEC */  addiu $v1, $v1, 27884
    /* 800EEB4C AC620000 */  sw $v0, 0($v1)
    /* 800EEB50 00402025 */  or $a0, $v0, $zero
    /* 800EEB54 0C0258B5 */  jal 0x800962D4
    /* 800EEB58 00002825 */  or $a1, $zero, $zero
    /* 800EEB5C 3C050002 */  lui $a1, 0x0002
    /* 800EEB60 34A52620 */  ori $a1, $a1, 0x2620
    /* 800EEB64 0C0330F0 */  jal 0x800CC3C0
    /* 800EEB68 00002025 */  or $a0, $zero, $zero
    /* 800EEB6C 3C068015 */  lui $a2, 0x8015
    /* 800EEB70 24C67240 */  addiu $a2, $a2, 29248
    /* 800EEB74 ACC20000 */  sw $v0, 0($a2)
    /* 800EEB78 00402025 */  or $a0, $v0, $zero
    /* 800EEB7C 0C0258B5 */  jal 0x800962D4
    /* 800EEB80 00002825 */  or $a1, $zero, $zero
    /* 800EEB84 3C098003 */  lui $t1, 0x8003
    /* 800EEB88 2529EBB0 */  addiu $t1, $t1, -5200
    /* 800EEB8C 8D2E0000 */  lw $t6, 0($t1)
    /* 800EEB90 3C088015 */  lui $t0, 0x8015
    /* 800EEB94 25086CEC */  addiu $t0, $t0, 27884
    /* 800EEB98 2404FFC0 */  addiu $a0, $zero, -64
    /* 800EEB9C 25CF0020 */  addiu $t7, $t6, 32
    /* 800EEBA0 8D190000 */  lw $t9, 0($t0)
    /* 800EEBA4 01E4C024 */  and $t8, $t7, $a0
    /* 800EEBA8 3C068015 */  lui $a2, 0x8015
    /* 800EEBAC 24C67240 */  addiu $a2, $a2, 29248
    /* 800EEBB0 AD380000 */  sw $t8, 0($t1)
    /* 800EEBB4 8CD80000 */  lw $t8, 0($a2)
    /* 800EEBB8 272E0020 */  addiu $t6, $t9, 32
    /* 800EEBBC 01C47824 */  and $t7, $t6, $a0
    /* 800EEBC0 3C028015 */  lui $v0, 0x8015
    /* 800EEBC4 27190020 */  addiu $t9, $t8, 32
    /* 800EEBC8 03247024 */  and $t6, $t9, $a0
    /* 800EEBCC 24426BE0 */  addiu $v0, $v0, 27616
    /* 800EEBD0 AC4E00FC */  sw $t6, 252($v0)
    /* 800EEBD4 ACCE0000 */  sw $t6, 0($a2)
    /* 800EEBD8 3C0E8007 */  lui $t6, 0x8007
    /* 800EEBDC 3C198006 */  lui $t9, 0x8006
    /* 800EEBE0 24050002 */  addiu $a1, $zero, 2
    /* 800EEBE4 273986D0 */  addiu $t9, $t9, -31024
    /* 800EEBE8 25CEA090 */  addiu $t6, $t6, -24432
    /* 800EEBEC AC4E00D8 */  sw $t6, 216($v0)
    /* 800EEBF0 AC590058 */  sw $t9, 88($v0)
    /* 800EEBF4 A445005C */  sh $a1, 92($v0)
    /* 800EEBF8 A44500DC */  sh $a1, 220($v0)
    /* 800EEBFC AC4F007C */  sw $t7, 124($v0)
    /* 800EEC00 AD0F0000 */  sw $t7, 0($t0)
    /* 800EEC04 3C018016 */  lui $at, 0x8016
    /* 800EEC08 AC39B250 */  sw $t9, -19888($at)
    /* 800EEC0C 3C018016 */  lui $at, 0x8016
    /* 800EEC10 272F06C0 */  addiu $t7, $t9, 1728
    /* 800EEC14 AC2FB260 */  sw $t7, -19872($at)
    /* 800EEC18 34019CC0 */  ori $at, $zero, 0x9CC0
    /* 800EEC1C 0321C021 */  addu $t8, $t9, $at
    /* 800EEC20 3C078015 */  lui $a3, 0x8015
    /* 800EEC24 24E797C8 */  addiu $a3, $a3, -26680
    /* 800EEC28 ACF80000 */  sw $t8, 0($a3)
    /* 800EEC2C 3C018015 */  lui $at, 0x8015
    /* 800EEC30 0C0296CF */  jal 0x800A5B3C
    /* 800EEC34 AC3897F4 */  sw $t8, -26636($at)
    /* 800EEC38 0C029522 */  jal 0x800A5488
    /* 800EEC3C 00000000 */  nop
    /* 800EEC40 0C02E0B2 */  jal 0x800B82C8
    /* 800EEC44 00000000 */  nop
    /* 800EEC48 3C013F80 */  lui $at, 0x3F80
    /* 800EEC4C 44813000 */  .word 0x44813000
    /* 800EEC50 3C014348 */  lui $at, 0x4348
    /* 800EEC54 44814000 */  .word 0x44814000
    /* 800EEC58 240E0040 */  addiu $t6, $zero, 64
    /* 800EEC5C AFAE0010 */  sw $t6, 16($sp)
    /* 800EEC60 24040020 */  addiu $a0, $zero, 32
    /* 800EEC64 24050010 */  addiu $a1, $zero, 16
    /* 800EEC68 24060010 */  addiu $a2, $zero, 16
    /* 800EEC6C 24070005 */  addiu $a3, $zero, 5
    /* 800EEC70 AFA0001C */  sw $zero, 28($sp)
    /* 800EEC74 E7A60014 */  swc1 $f6, 20($sp)
    /* 800EEC78 0C0322E3 */  jal 0x800C8B8C
    /* 800EEC7C E7A80018 */  swc1 $f8, 24($sp)
    /* 800EEC80 3C0F8015 */  lui $t7, 0x8015
    /* 800EEC84 81EF6994 */  lb $t7, 27028($t7)
    /* 800EEC88 3C028011 */  lui $v0, 0x8011
    /* 800EEC8C 15E00026 */  bne $t7, $zero, 0x800EED28
    /* 800EEC90 00000000 */  nop
    /* 800EEC94 8C42025C */  lw $v0, 604($v0)
    /* 800EEC98 3C048015 */  lui $a0, 0x8015
    /* 800EEC9C 24842770 */  addiu $a0, $a0, 10096
    /* 800EECA0 1040000E */  beq $v0, $zero, 0x800EECDC
    /* 800EECA4 00002825 */  or $a1, $zero, $zero
    /* 800EECA8 AFA20048 */  sw $v0, 72($sp)
    /* 800EECAC 0C001C9C */  jal 0x80007270
    /* 800EECB0 24060001 */  addiu $a2, $zero, 1
    /* 800EECB4 8FA50048 */  lw $a1, 72($sp)
    /* 800EECB8 0C0257F6 */  jal 0x80095FD8
    /* 800EECBC 00003025 */  or $a2, $zero, $zero
    /* 800EECC0 3C048015 */  lui $a0, 0x8015
    /* 800EECC4 24842770 */  addiu $a0, $a0, 10096
    /* 800EECC8 00002825 */  or $a1, $zero, $zero
    /* 800EECCC 0C001D78 */  jal 0x800075E0
    /* 800EECD0 00003025 */  or $a2, $zero, $zero
    /* 800EECD4 3C018011 */  lui $at, 0x8011
    /* 800EECD8 AC20025C */  sw $zero, 604($at)
    /* 800EECDC 3C028011 */  lui $v0, 0x8011
    /* 800EECE0 8C420260 */  lw $v0, 608($v0)
    /* 800EECE4 3C048015 */  lui $a0, 0x8015
    /* 800EECE8 24842770 */  addiu $a0, $a0, 10096
    /* 800EECEC 1040000E */  beq $v0, $zero, 0x800EED28
    /* 800EECF0 00002825 */  or $a1, $zero, $zero
    /* 800EECF4 AFA20030 */  sw $v0, 48($sp)
    /* 800EECF8 0C001C9C */  jal 0x80007270
    /* 800EECFC 24060001 */  addiu $a2, $zero, 1
    /* 800EED00 8FA50030 */  lw $a1, 48($sp)
    /* 800EED04 0C0257F6 */  jal 0x80095FD8
    /* 800EED08 00003025 */  or $a2, $zero, $zero
    /* 800EED0C 3C048015 */  lui $a0, 0x8015
    /* 800EED10 24842770 */  addiu $a0, $a0, 10096
    /* 800EED14 00002825 */  or $a1, $zero, $zero
    /* 800EED18 0C001D78 */  jal 0x800075E0
    /* 800EED1C 00003025 */  or $a2, $zero, $zero
    /* 800EED20 3C018011 */  lui $at, 0x8011
    /* 800EED24 AC200260 */  sw $zero, 608($at)
    /* 800EED28 0C03BA08 */  jal 0x800EE820
    /* 800EED2C 00000000 */  nop
    /* 800EED30 2404FFFF */  addiu $a0, $zero, -1
    /* 800EED34 0C032456 */  jal 0x800C9158
    /* 800EED38 00002825 */  or $a1, $zero, $zero
    /* 800EED3C 0C037917 */  jal 0x800DE45C
    /* 800EED40 00000000 */  nop
    /* 800EED44 0C0004D4 */  jal 0x80001350
    /* 800EED48 00000000 */  nop
    /* 800EED4C 3C188011 */  lui $t8, 0x8011
    /* 800EED50 831847C0 */  lb $t8, 18368($t8)
    /* 800EED54 24190001 */  addiu $t9, $zero, 1
    /* 800EED58 3C018011 */  lui $at, 0x8011
    /* 800EED5C 1700000C */  bne $t8, $zero, 0x800EED90
    /* 800EED60 3C048014 */  lui $a0, 0x8014
    /* 800EED64 3C058014 */  lui $a1, 0x8014
    /* 800EED68 24A561FC */  addiu $a1, $a1, 25084
    /* 800EED6C A03947C0 */  sb $t9, 18368($at)
    /* 800EED70 248461D0 */  addiu $a0, $a0, 25040
    /* 800EED74 0C001A80 */  jal 0x80006A00
    /* 800EED78 24060001 */  addiu $a2, $zero, 1
    /* 800EED7C 3C048014 */  lui $a0, 0x8014
    /* 800EED80 248461D0 */  addiu $a0, $a0, 25040
    /* 800EED84 00002825 */  or $a1, $zero, $zero
    /* 800EED88 0C001D78 */  jal 0x800075E0
    /* 800EED8C 00003025 */  or $a2, $zero, $zero
    /* 800EED90 8FBF002C */  lw $ra, 44($sp)
    /* 800EED94 240EFFFF */  addiu $t6, $zero, -1
    /* 800EED98 3C018015 */  lui $at, 0x8015
    /* 800EED9C 8FB00024 */  lw $s0, 36($sp)
    /* 800EEDA0 8FB10028 */  lw $s1, 40($sp)
    /* 800EEDA4 A02E9DA0 */  sb $t6, -25184($at)
    /* 800EEDA8 03E00008 */  jr $ra
    /* 800EEDAC 27BD0080 */  addiu $sp, $sp, 128
