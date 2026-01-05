# Source: game_code.bin (decompressed)
# Address: 0x800DE868

glabel main_menu_handler
    /* 800DE868 27BDFFF0 */  addiu $sp, $sp, -16
    /* 800DE86C 24070001 */  addiu $a3, $zero, 1
    /* 800DE870 AFB0000C */  sw $s0, 12($sp)
    /* 800DE874 14EE0006 */  bne $a3, $t6, 0x800DE890
    /* 800DE878 F7B40000 */  .word 0xF7B40000
    /* 800DE87C 3C088015 */  lui $t0, 0x8015
    /* 800DE880 25082030 */  addiu $t0, $t0, 8240
    /* 800DE884 810F0000 */  lb $t7, 0($t0)
    /* 800DE888 29E10005 */  slti $at, $t7, 5
    /* 800DE88C 14200006 */  bne $at, $zero, 0x800DE8A8
    /* 800DE890 3C028015 */  lui $v0, 0x8015
    /* 800DE894 80420F14 */  lb $v0, 3860($v0)
    /* 800DE898 3C088015 */  lui $t0, 0x8015
    /* 800DE89C 25082030 */  addiu $t0, $t0, 8240
    /* 800DE8A0 14400043 */  bne $v0, $zero, 0x800DE9B0
    /* 800DE8A4 2403FFFF */  addiu $v1, $zero, -1
    /* 800DE8A8 3C028015 */  lui $v0, 0x8015
    /* 800DE8AC 90423E8F */  lbu $v0, 16015($v0)
    /* 800DE8B0 3C198015 */  lui $t9, 0x8015
    /* 800DE8B4 24050006 */  addiu $a1, $zero, 6
    /* 800DE8B8 10400002 */  beq $v0, $zero, 0x800DE8C4
    /* 800DE8BC 27393E88 */  addiu $t9, $t9, 16008
    /* 800DE8C0 14A20005 */  bne $a1, $v0, 0x800DE8D8
    /* 800DE8C4 3C013F80 */  lui $at, 0x3F80
    /* 800DE8C8 44817000 */  .word 0x44817000
    /* 800DE8CC 3C018015 */  lui $at, 0x8015
    /* 800DE8D0 24050006 */  addiu $a1, $zero, 6
    /* 800DE8D4 E42EA650 */  swc1 $f14, -22960($at)
    /* 800DE8D8 3C028015 */  lui $v0, 0x8015
    /* 800DE8DC 90423E97 */  lbu $v0, 16023($v0)
    /* 800DE8E0 3C013F80 */  lui $at, 0x3F80
    /* 800DE8E4 44817000 */  .word 0x44817000
    /* 800DE8E8 10400003 */  beq $v0, $zero, 0x800DE8F8
    /* 800DE8EC 3C018015 */  lui $at, 0x8015
    /* 800DE8F0 54A20003 */  .word 0x54A20003
    /* 800DE8F4 24020002 */  addiu $v0, $zero, 2
    /* 800DE8F8 E42EAE58 */  swc1 $f14, -20904($at)
    /* 800DE8FC 24020002 */  addiu $v0, $zero, 2
    /* 800DE900 0002C0C0 */  sll $t8, $v0, 3
    /* 800DE904 03191821 */  addu $v1, $t8, $t9
    /* 800DE908 90640007 */  lbu $a0, 7($v1)
    /* 800DE90C 240A0808 */  addiu $t2, $zero, 2056
    /* 800DE910 10800003 */  beq $a0, $zero, 0x800DE920
    /* 800DE914 00000000 */  nop
    /* 800DE918 54A40008 */  .word 0x54A40008
    /* 800DE91C 9064000F */  lbu $a0, 15($v1)
    /* 800DE920 004A0019 */  multu $v0, $t2
    /* 800DE924 3C098015 */  lui $t1, 0x8015
    /* 800DE928 2529A250 */  addiu $t1, $t1, -23984
    /* 800DE92C 00007012 */  mflo $t6
    /* 800DE930 012E7821 */  addu $t7, $t1, $t6
    /* 800DE934 E5EE0400 */  swc1 $f14, 1024($t7)
    /* 800DE938 9064000F */  lbu $a0, 15($v1)
    /* 800DE93C 3C098015 */  lui $t1, 0x8015
    /* 800DE940 2529A250 */  addiu $t1, $t1, -23984
    /* 800DE944 10800003 */  beq $a0, $zero, 0x800DE954
    /* 800DE948 240A0808 */  addiu $t2, $zero, 2056
    /* 800DE94C 54A40006 */  .word 0x54A40006
    /* 800DE950 90640017 */  lbu $a0, 23($v1)
    /* 800DE954 004A0019 */  multu $v0, $t2
    /* 800DE958 0000C012 */  mflo $t8
    /* 800DE95C 0138C821 */  addu $t9, $t1, $t8
    /* 800DE960 E72E0C08 */  swc1 $f14, 3080($t9)
    /* 800DE964 90640017 */  lbu $a0, 23($v1)
    /* 800DE968 10800003 */  beq $a0, $zero, 0x800DE978
    /* 800DE96C 00000000 */  nop
    /* 800DE970 54A40006 */  .word 0x54A40006
    /* 800DE974 9064001F */  lbu $a0, 31($v1)
    /* 800DE978 004A0019 */  multu $v0, $t2
    /* 800DE97C 00007012 */  mflo $t6
    /* 800DE980 012E7821 */  addu $t7, $t1, $t6
    /* 800DE984 E5EE1410 */  swc1 $f14, 5136($t7)
    /* 800DE988 9064001F */  lbu $a0, 31($v1)
    /* 800DE98C 10800003 */  beq $a0, $zero, 0x800DE99C
    /* 800DE990 00000000 */  nop
    /* 800DE994 54A40082 */  .word 0x54A40082
    /* 800DE998 D7B40000 */  .word 0xD7B40000
    /* 800DE99C 004A0019 */  multu $v0, $t2
    /* 800DE9A0 0000C012 */  mflo $t8
    /* 800DE9A4 0138C821 */  addu $t9, $t1, $t8
    /* 800DE9A8 1000007C */  beq $zero, $zero, 0x800DEB9C
    /* 800DE9AC E72E1C18 */  swc1 $f14, 7192($t9)
    /* 800DE9B0 3C0B8015 */  lui $t3, 0x8015
    /* 800DE9B4 3C098015 */  lui $t1, 0x8015
    /* 800DE9B8 2529A250 */  addiu $t1, $t1, -23984
    /* 800DE9BC 256B2818 */  addiu $t3, $t3, 10264
    /* 800DE9C0 00002025 */  or $a0, $zero, $zero
    /* 800DE9C4 24100005 */  addiu $s0, $zero, 5
    /* 800DE9C8 240D0002 */  addiu $t5, $zero, 2
    /* 800DE9CC 240C03B8 */  addiu $t4, $zero, 952
    /* 800DE9D0 240A0808 */  addiu $t2, $zero, 2056
    /* 800DE9D4 008A0019 */  multu $a0, $t2
    /* 800DE9D8 00007012 */  mflo $t6
    /* 800DE9DC 012E2821 */  addu $a1, $t1, $t6
    /* 800DE9E0 84AF07C8 */  lh $t7, 1992($a1)
    /* 800DE9E4 51E00022 */  .word 0x51E00022
    /* 800DE9E8 24840001 */  addiu $a0, $a0, 1
    /* 800DE9EC 008C0019 */  multu $a0, $t4
    /* 800DE9F0 0000C012 */  mflo $t8
    /* 800DE9F4 01783021 */  addu $a2, $t3, $t8
    /* 800DE9F8 80D90359 */  lb $t9, 857($a2)
    /* 800DE9FC 2B210002 */  slti $at, $t9, 2
    /* 800DEA00 5020001B */  .word 0x5020001B
    /* 800DEA04 24840001 */  addiu $a0, $a0, 1
    /* 800DEA08 80AE07CC */  lb $t6, 1996($a1)
    /* 800DEA0C 2401FFFF */  addiu $at, $zero, -1
    /* 800DEA10 11AE0004 */  beq $t5, $t6, 0x800DEA24
    /* 800DEA14 00000000 */  nop
    /* 800DEA18 810F0000 */  lb $t7, 0($t0)
    /* 800DEA1C 560F0014 */  .word 0x560F0014
    /* 800DEA20 24840001 */  addiu $a0, $a0, 1
    /* 800DEA24 14610005 */  bne $v1, $at, 0x800DEA3C
    /* 800DEA28 00000000 */  nop
    /* 800DEA2C 00041C00 */  sll $v1, $a0, 16
    /* 800DEA30 0003C403 */  sra $t8, $v1, 16
    /* 800DEA34 1000000D */  beq $zero, $zero, 0x800DEA6C
    /* 800DEA38 03001825 */  or $v1, $t8, $zero
    /* 800DEA3C 006C0019 */  multu $v1, $t4
    /* 800DEA40 C4C60100 */  lwc1 $f6, 256($a2)
    /* 800DEA44 0000C812 */  mflo $t9
    /* 800DEA48 01797021 */  addu $t6, $t3, $t9
    /* 800DEA4C C5C40100 */  lwc1 $f4, 256($t6)
    /* 800DEA50 4606203C */  .word 0x4606203C
    /* 800DEA54 00000000 */  nop
    /* 800DEA58 45020005 */  .word 0x45020005
    /* 800DEA5C 24840001 */  addiu $a0, $a0, 1
    /* 800DEA60 00041C00 */  sll $v1, $a0, 16
    /* 800DEA64 00037C03 */  sra $t7, $v1, 16
    /* 800DEA68 01E01825 */  or $v1, $t7, $zero
    /* 800DEA6C 24840001 */  addiu $a0, $a0, 1
    /* 800DEA70 0004C400 */  sll $t8, $a0, 16
    /* 800DEA74 00182403 */  sra $a0, $t8, 16
    /* 800DEA78 28810006 */  slti $at, $a0, 6
    /* 800DEA7C 1420FFD5 */  bne $at, $zero, 0x800DE9D4
    /* 800DEA80 00000000 */  nop
    /* 800DEA84 3C013F80 */  lui $at, 0x3F80
    /* 800DEA88 4481A000 */  .word 0x4481A000
    /* 800DEA8C 00037100 */  sll $t6, $v1, 4
    /* 800DEA90 3C018012 */  lui $at, 0x8012
    /* 800DEA94 01C37023 */  subu $t6, $t6, $v1
    /* 800DEA98 C4324310 */  lwc1 $f18, 17168($at)
    /* 800DEA9C 000E70C0 */  sll $t6, $t6, 3
    /* 800DEAA0 3C018012 */  lui $at, 0x8012
    /* 800DEAA4 01C37023 */  subu $t6, $t6, $v1
    /* 800DEAA8 C4304314 */  lwc1 $f16, 17172($at)
    /* 800DEAAC 000E70C0 */  sll $t6, $t6, 3
    /* 800DEAB0 3C013F80 */  lui $at, 0x3F80
    /* 800DEAB4 016E7821 */  addu $t7, $t3, $t6
    /* 800DEAB8 44817000 */  .word 0x44817000
    /* 800DEABC C5E20100 */  lwc1 $f2, 256($t7)
    /* 800DEAC0 00002025 */  or $a0, $zero, $zero
    /* 800DEAC4 008A0019 */  multu $a0, $t2
    /* 800DEAC8 0000C012 */  mflo $t8
    /* 800DEACC 01382821 */  addu $a1, $t1, $t8
    /* 800DEAD0 84B907C8 */  lh $t9, 1992($a1)
    /* 800DEAD4 5320002C */  .word 0x5320002C
    /* 800DEAD8 24840001 */  addiu $a0, $a0, 1
    /* 800DEADC 008C0019 */  multu $a0, $t4
    /* 800DEAE0 00007012 */  mflo $t6
    /* 800DEAE4 016E3021 */  addu $a2, $t3, $t6
    /* 800DEAE8 80CF0359 */  lb $t7, 857($a2)
    /* 800DEAEC 29E10002 */  slti $at, $t7, 2
    /* 800DEAF0 50200025 */  .word 0x50200025
    /* 800DEAF4 24840001 */  addiu $a0, $a0, 1
    /* 800DEAF8 80B807CC */  lb $t8, 1996($a1)
    /* 800DEAFC 51B80005 */  .word 0x51B80005
    /* 800DEB00 C4C80100 */  lwc1 $f8, 256($a2)
    /* 800DEB04 81190000 */  lb $t9, 0($t0)
    /* 800DEB08 5619001F */  .word 0x5619001F
    /* 800DEB0C 24840001 */  addiu $a0, $a0, 1
    /* 800DEB10 C4C80100 */  lwc1 $f8, 256($a2)
    /* 800DEB14 3C013F00 */  lui $at, 0x3F00
    /* 800DEB18 46081001 */  .word 0x46081001
    /* 800DEB1C 4600803C */  .word 0x4600803C
    /* 800DEB20 00000000 */  nop
    /* 800DEB24 45000003 */  .word 0x45000003
    /* 800DEB28 00000000 */  nop
    /* 800DEB2C 10000004 */  beq $zero, $zero, 0x800DEB40
    /* 800DEB30 460E9300 */  .word 0x460E9300
    /* 800DEB34 46120282 */  .word 0x46120282
    /* 800DEB38 46105103 */  .word 0x46105103
    /* 800DEB3C 46142300 */  .word 0x46142300
    /* 800DEB40 14E20006 */  bne $a3, $v0, 0x800DEB5C
    /* 800DEB44 00000000 */  nop
    /* 800DEB48 460CA181 */  .word 0x460CA181
    /* 800DEB4C 44814000 */  .word 0x44814000
    /* 800DEB50 00000000 */  nop
    /* 800DEB54 46083282 */  .word 0x46083282
    /* 800DEB58 46145300 */  .word 0x46145300
    /* 800DEB5C 3C018012 */  lui $at, 0x8012
    /* 800DEB60 C4264318 */  lwc1 $f6, 17176($at)
    /* 800DEB64 C4A40400 */  lwc1 $f4, 1024($a1)
    /* 800DEB68 3C018012 */  lui $at, 0x8012
    /* 800DEB6C C42A431C */  lwc1 $f10, 17180($at)
    /* 800DEB70 46062202 */  .word 0x46062202
    /* 800DEB74 00000000 */  nop
    /* 800DEB78 460C5102 */  .word 0x460C5102
    /* 800DEB7C 46044180 */  .word 0x46044180
    /* 800DEB80 E4A60400 */  swc1 $f6, 1024($a1)
    /* 800DEB84 24840001 */  addiu $a0, $a0, 1
    /* 800DEB88 00047400 */  sll $t6, $a0, 16
    /* 800DEB8C 000E2403 */  sra $a0, $t6, 16
    /* 800DEB90 28810006 */  slti $at, $a0, 6
    /* 800DEB94 1420FFCB */  bne $at, $zero, 0x800DEAC4
    /* 800DEB98 00000000 */  nop
    /* 800DEB9C D7B40000 */  .word 0xD7B40000
    /* 800DEBA0 8FB0000C */  lw $s0, 12($sp)
    /* 800DEBA4 03E00008 */  jr $ra
    /* 800DEBA8 27BD0010 */  addiu $sp, $sp, 16
