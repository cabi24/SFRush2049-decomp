# Source: game_code.bin (decompressed)
# Address: 0x800FDA90

glabel large_update_routine
    /* 800FDA90 C4460004 */  lwc1 $f6, 4($v0)
    /* 800FDA94 C452000C */  lwc1 $f18, 12($v0)
    /* 800FDA98 00001825 */  or $v1, $zero, $zero
    /* 800FDA9C 460A903C */  .word 0x460A903C
    /* 800FDAA0 00000000 */  nop
    /* 800FDAA4 45020003 */  .word 0x45020003
    /* 800FDAA8 C4460004 */  lwc1 $f6, 4($v0)
    /* 800FDAAC 24030001 */  addiu $v1, $zero, 1
    /* 800FDAB0 C4460004 */  lwc1 $f6, 4($v0)
    /* 800FDAB4 00603825 */  or $a3, $v1, $zero
    /* 800FDAB8 00001825 */  or $v1, $zero, $zero
    /* 800FDABC 4606203C */  .word 0x4606203C
    /* 800FDAC0 00000000 */  nop
    /* 800FDAC4 45000002 */  .word 0x45000002
    /* 800FDAC8 00000000 */  nop
    /* 800FDACC 24030001 */  addiu $v1, $zero, 1
    /* 800FDAD0 10600008 */  beq $v1, $zero, 0x800FDAF4
    /* 800FDAD4 00000000 */  nop
    /* 800FDAD8 C4480000 */  lwc1 $f8, 0($v0)
    /* 800FDADC 00001825 */  or $v1, $zero, $zero
    /* 800FDAE0 4604403C */  .word 0x4604403C
    /* 800FDAE4 00000000 */  nop
    /* 800FDAE8 45000002 */  .word 0x45000002
    /* 800FDAEC 00000000 */  nop
    /* 800FDAF0 24030001 */  addiu $v1, $zero, 1
    /* 800FDAF4 50E0000C */  .word 0x50E0000C
    /* 800FDAF8 C5100788 */  lwc1 $f16, 1928($t0)
    /* 800FDAFC 5060000A */  .word 0x5060000A
    /* 800FDB00 C5100788 */  lwc1 $f16, 1928($t0)
    /* 800FDB04 810E0640 */  lb $t6, 1600($t0)
    /* 800FDB08 01002025 */  or $a0, $t0, $zero
    /* 800FDB0C 51C00006 */  .word 0x51C00006
    /* 800FDB10 C5100788 */  lwc1 $f16, 1928($t0)
    /* 800FDB14 0C03F637 */  jal 0x800FD8DC
    /* 800FDB18 8FA50084 */  lw $a1, 132($sp)
    /* 800FDB1C 10000117 */  beq $zero, $zero, 0x800FDF7C
    /* 800FDB20 8FBF0014 */  lw $ra, 20($sp)
    /* 800FDB24 C5100788 */  lwc1 $f16, 1928($t0)
    /* 800FDB28 C5320034 */  lwc1 $f18, 52($t1)
    /* 800FDB2C 27A40060 */  addiu $a0, $sp, 96
    /* 800FDB30 27A50048 */  addiu $a1, $sp, 72
    /* 800FDB34 46128281 */  .word 0x46128281
    /* 800FDB38 8FA6001C */  lw $a2, 28($sp)
    /* 800FDB3C E7AA0060 */  swc1 $f10, 96($sp)
    /* 800FDB40 C5280038 */  lwc1 $f8, 56($t1)
    /* 800FDB44 C506078C */  lwc1 $f6, 1932($t0)
    /* 800FDB48 46083101 */  .word 0x46083101
    /* 800FDB4C E7A40064 */  swc1 $f4, 100($sp)
    /* 800FDB50 C532003C */  lwc1 $f18, 60($t1)
    /* 800FDB54 C5100790 */  lwc1 $f16, 1936($t0)
    /* 800FDB58 AFA90080 */  sw $t1, 128($sp)
    /* 800FDB5C AFA8007C */  sw $t0, 124($sp)
    /* 800FDB60 46128281 */  .word 0x46128281
    /* 800FDB64 AFA70030 */  sw $a3, 48($sp)
    /* 800FDB68 AFA30024 */  sw $v1, 36($sp)
    /* 800FDB6C 0C02986C */  jal 0x800A61B0
    /* 800FDB70 E7AA0068 */  swc1 $f10, 104($sp)
    /* 800FDB74 8FA70030 */  lw $a3, 48($sp)
    /* 800FDB78 8FA30024 */  lw $v1, 36($sp)
    /* 800FDB7C 8FA8007C */  lw $t0, 124($sp)
    /* 800FDB80 10E00008 */  beq $a3, $zero, 0x800FDBA4
    /* 800FDB84 8FA90080 */  lw $t1, 128($sp)
    /* 800FDB88 14600006 */  bne $v1, $zero, 0x800FDBA4
    /* 800FDB8C 00000000 */  nop
    /* 800FDB90 44803000 */  .word 0x44803000
    /* 800FDB94 44807000 */  .word 0x44807000
    /* 800FDB98 8FA20088 */  lw $v0, 136($sp)
    /* 800FDB9C 1000004A */  beq $zero, $zero, 0x800FDCC8
    /* 800FDBA0 E7A60074 */  swc1 $f6, 116($sp)
    /* 800FDBA4 3C018012 */  lui $at, 0x8012
    /* 800FDBA8 C4244808 */  lwc1 $f4, 18440($at)
    /* 800FDBAC C7A80050 */  lwc1 $f8, 80($sp)
    /* 800FDBB0 8FA20088 */  lw $v0, 136($sp)
    /* 800FDBB4 44807000 */  .word 0x44807000
    /* 800FDBB8 46044402 */  .word 0x46044402
    /* 800FDBBC 3C01C57A */  lui $at, 0xC57A
    /* 800FDBC0 E7B00074 */  swc1 $f16, 116($sp)
    /* 800FDBC4 C4400008 */  lwc1 $f0, 8($v0)
    /* 800FDBC8 4600703C */  .word 0x4600703C
    /* 800FDBCC 00000000 */  nop
    /* 800FDBD0 4502001E */  .word 0x4502001E
    /* 800FDBD4 460E003C */  .word 0x460E003C
    /* 800FDBD8 44811000 */  .word 0x44811000
    /* 800FDBDC 00000000 */  nop
    /* 800FDBE0 4610103C */  .word 0x4610103C
    /* 800FDBE4 00000000 */  nop
    /* 800FDBE8 45000002 */  .word 0x45000002
    /* 800FDBEC 00000000 */  nop
    /* 800FDBF0 E7A20074 */  swc1 $f2, 116($sp)
    /* 800FDBF4 50600035 */  .word 0x50600035
    /* 800FDBF8 3C01C57A */  lui $at, 0xC57A
    /* 800FDBFC 8D2F0000 */  lw $t7, 0($t1)
    /* 800FDC00 C4520008 */  lwc1 $f18, 8($v0)
    /* 800FDC04 3C018012 */  lui $at, 0x8012
    /* 800FDC08 C5EA0008 */  lwc1 $f10, 8($t7)
    /* 800FDC0C C424480C */  lwc1 $f4, 18444($at)
    /* 800FDC10 460A9181 */  .word 0x460A9181
    /* 800FDC14 C7AA0074 */  lwc1 $f10, 116($sp)
    /* 800FDC18 E7A60068 */  swc1 $f6, 104($sp)
    /* 800FDC1C C7A80068 */  lwc1 $f8, 104($sp)
    /* 800FDC20 46004005 */  .word 0x46004005
    /* 800FDC24 46002402 */  .word 0x46002402
    /* 800FDC28 00000000 */  nop
    /* 800FDC2C 46104482 */  .word 0x46104482
    /* 800FDC30 460A903C */  .word 0x460A903C
    /* 800FDC34 E7B20068 */  swc1 $f18, 104($sp)
    /* 800FDC38 45000023 */  .word 0x45000023
    /* 800FDC3C 00000000 */  nop
    /* 800FDC40 10000021 */  beq $zero, $zero, 0x800FDCC8
    /* 800FDC44 E7B20074 */  swc1 $f18, 116($sp)
    /* 800FDC48 460E003C */  .word 0x460E003C
    /* 800FDC4C 3C01457A */  lui $at, 0x457A
    /* 800FDC50 C7A60074 */  lwc1 $f6, 116($sp)
    /* 800FDC54 4502001D */  .word 0x4502001D
    /* 800FDC58 3C01C57A */  lui $at, 0xC57A
    /* 800FDC5C 44816000 */  .word 0x44816000
    /* 800FDC60 00000000 */  nop
    /* 800FDC64 460C303C */  .word 0x460C303C
    /* 800FDC68 00000000 */  nop
    /* 800FDC6C 45000002 */  .word 0x45000002
    /* 800FDC70 00000000 */  nop
    /* 800FDC74 E7AC0074 */  swc1 $f12, 116($sp)
    /* 800FDC78 50600014 */  .word 0x50600014
    /* 800FDC7C 3C01C57A */  lui $at, 0xC57A
    /* 800FDC80 8D380000 */  lw $t8, 0($t1)
    /* 800FDC84 C4440008 */  lwc1 $f4, 8($v0)
    /* 800FDC88 3C018012 */  lui $at, 0x8012
    /* 800FDC8C C708000C */  lwc1 $f8, 12($t8)
    /* 800FDC90 C4324810 */  lwc1 $f18, 18448($at)
    /* 800FDC94 46082401 */  .word 0x46082401
    /* 800FDC98 C7A80074 */  lwc1 $f8, 116($sp)
    /* 800FDC9C E7B00068 */  swc1 $f16, 104($sp)
    /* 800FDCA0 C7AA0068 */  lwc1 $f10, 104($sp)
    /* 800FDCA4 46005005 */  .word 0x46005005
    /* 800FDCA8 46009182 */  .word 0x46009182
    /* 800FDCAC 00000000 */  nop
    /* 800FDCB0 46065102 */  .word 0x46065102
    /* 800FDCB4 4604403C */  .word 0x4604403C
    /* 800FDCB8 E7A40068 */  swc1 $f4, 104($sp)
    /* 800FDCBC 45000002 */  .word 0x45000002
    /* 800FDCC0 00000000 */  nop
    /* 800FDCC4 E7A40074 */  swc1 $f4, 116($sp)
    /* 800FDCC8 3C01C57A */  lui $at, 0xC57A
    /* 800FDCCC 44811000 */  .word 0x44811000
    /* 800FDCD0 3C01457A */  lui $at, 0x457A
    /* 800FDCD4 44816000 */  .word 0x44816000
    /* 800FDCD8 10600006 */  beq $v1, $zero, 0x800FDCF4
    /* 800FDCDC C7B20048 */  lwc1 $f18, 72($sp)
    /* 800FDCE0 14E00004 */  bne $a3, $zero, 0x800FDCF4
    /* 800FDCE4 00000000 */  nop
    /* 800FDCE8 44808000 */  .word 0x44808000
    /* 800FDCEC 10000041 */  beq $zero, $zero, 0x800FDDF4
    /* 800FDCF0 E7B0006C */  swc1 $f16, 108($sp)
    /* 800FDCF4 3C018012 */  lui $at, 0x8012
    /* 800FDCF8 C42A4814 */  lwc1 $f10, 18452($at)
    /* 800FDCFC 460A9182 */  .word 0x460A9182
    /* 800FDD00 E7A6006C */  swc1 $f6, 108($sp)
    /* 800FDD04 C4400000 */  lwc1 $f0, 0($v0)
    /* 800FDD08 4600703C */  .word 0x4600703C
    /* 800FDD0C 00000000 */  nop
    /* 800FDD10 4502001C */  .word 0x4502001C
    /* 800FDD14 460E003C */  .word 0x460E003C
    /* 800FDD18 4606103C */  .word 0x4606103C
    /* 800FDD1C 00000000 */  nop
    /* 800FDD20 45000002 */  .word 0x45000002
    /* 800FDD24 00000000 */  nop
    /* 800FDD28 E7A2006C */  swc1 $f2, 108($sp)
    /* 800FDD2C 50E00032 */  .word 0x50E00032
    /* 800FDD30 3C0142C8 */  lui $at, 0x42C8
    /* 800FDD34 8D390000 */  lw $t9, 0($t1)
    /* 800FDD38 C4480000 */  lwc1 $f8, 0($v0)
    /* 800FDD3C 3C018012 */  lui $at, 0x8012
    /* 800FDD40 C7240004 */  lwc1 $f4, 4($t9)
    /* 800FDD44 C42A4818 */  lwc1 $f10, 18456($at)
    /* 800FDD48 46044401 */  .word 0x46044401
    /* 800FDD4C C7A4006C */  lwc1 $f4, 108($sp)
    /* 800FDD50 E7B00060 */  swc1 $f16, 96($sp)
    /* 800FDD54 C7B20060 */  lwc1 $f18, 96($sp)
    /* 800FDD58 46009005 */  .word 0x46009005
    /* 800FDD5C 46005182 */  .word 0x46005182
    /* 800FDD60 00000000 */  nop
    /* 800FDD64 46069202 */  .word 0x46069202
    /* 800FDD68 4604403C */  .word 0x4604403C
    /* 800FDD6C E7A80060 */  swc1 $f8, 96($sp)
    /* 800FDD70 45000020 */  .word 0x45000020
    /* 800FDD74 00000000 */  nop
    /* 800FDD78 1000001E */  beq $zero, $zero, 0x800FDDF4
    /* 800FDD7C E7A8006C */  swc1 $f8, 108($sp)
    /* 800FDD80 460E003C */  .word 0x460E003C
    /* 800FDD84 C7B0006C */  lwc1 $f16, 108($sp)
    /* 800FDD88 4502001B */  .word 0x4502001B
    /* 800FDD8C 3C0142C8 */  lui $at, 0x42C8
    /* 800FDD90 460C803C */  .word 0x460C803C
    /* 800FDD94 00000000 */  nop
    /* 800FDD98 45000002 */  .word 0x45000002
    /* 800FDD9C 00000000 */  nop
    /* 800FDDA0 E7AC006C */  swc1 $f12, 108($sp)
    /* 800FDDA4 50E00014 */  .word 0x50E00014
    /* 800FDDA8 3C0142C8 */  lui $at, 0x42C8
    /* 800FDDAC 8D2A0000 */  lw $t2, 0($t1)
    /* 800FDDB0 C44A0000 */  lwc1 $f10, 0($v0)
    /* 800FDDB4 3C018012 */  lui $at, 0x8012
    /* 800FDDB8 C5520000 */  lwc1 $f18, 0($t2)
    /* 800FDDBC C428481C */  lwc1 $f8, 18460($at)
    /* 800FDDC0 46125181 */  .word 0x46125181
    /* 800FDDC4 C7B2006C */  lwc1 $f18, 108($sp)
    /* 800FDDC8 E7A60060 */  swc1 $f6, 96($sp)
    /* 800FDDCC C7A40060 */  lwc1 $f4, 96($sp)
    /* 800FDDD0 46002005 */  .word 0x46002005
    /* 800FDDD4 46004402 */  .word 0x46004402
    /* 800FDDD8 00000000 */  nop
    /* 800FDDDC 46102282 */  .word 0x46102282
    /* 800FDDE0 460A903C */  .word 0x460A903C
    /* 800FDDE4 E7AA0060 */  swc1 $f10, 96($sp)
    /* 800FDDE8 45000002 */  .word 0x45000002
    /* 800FDDEC 00000000 */  nop
    /* 800FDDF0 E7AA006C */  swc1 $f10, 108($sp)
    /* 800FDDF4 3C0142C8 */  lui $at, 0x42C8
    /* 800FDDF8 44814000 */  .word 0x44814000
    /* 800FDDFC C7A6004C */  lwc1 $f6, 76($sp)
    /* 800FDE00 3C013F00 */  lui $at, 0x3F00
    /* 800FDE04 27A4006C */  addiu $a0, $sp, 108
    /* 800FDE08 46083102 */  .word 0x46083102
    /* 800FDE0C 44813000 */  .word 0x44813000
    /* 800FDE10 27A50060 */  addiu $a1, $sp, 96
    /* 800FDE14 8FA6001C */  lw $a2, 28($sp)
    /* 800FDE18 E7A40070 */  swc1 $f4, 112($sp)
    /* 800FDE1C 8D2B0000 */  lw $t3, 0($t1)
    /* 800FDE20 C51005C4 */  lwc1 $f16, 1476($t0)
    /* 800FDE24 C572001C */  lwc1 $f18, 28($t3)
    /* 800FDE28 AFA90080 */  sw $t1, 128($sp)
    /* 800FDE2C AFA8007C */  sw $t0, 124($sp)
    /* 800FDE30 46128280 */  .word 0x46128280
    /* 800FDE34 46065202 */  .word 0x46065202
    /* 800FDE38 0C027A08 */  jal 0x8009E820
    /* 800FDE3C E7A80038 */  swc1 $f8, 56($sp)
    /* 800FDE40 8FA8007C */  lw $t0, 124($sp)
    /* 800FDE44 27A40060 */  addiu $a0, $sp, 96
    /* 800FDE48 27A5003C */  addiu $a1, $sp, 60
    /* 800FDE4C 0C02986C */  jal 0x800A61B0
    /* 800FDE50 250607A0 */  addiu $a2, $t0, 1952
    /* 800FDE54 8FA90080 */  lw $t1, 128($sp)
    /* 800FDE58 C7B00038 */  lwc1 $f16, 56($sp)
    /* 800FDE5C C7B2003C */  lwc1 $f18, 60($sp)
    /* 800FDE60 8D2C0000 */  lw $t4, 0($t1)
    /* 800FDE64 C7A60040 */  lwc1 $f6, 64($sp)
    /* 800FDE68 8FA8007C */  lw $t0, 124($sp)
    /* 800FDE6C C584001C */  lwc1 $f4, 28($t4)
    /* 800FDE70 3C028018 */  lui $v0, 0x8018
    /* 800FDE74 24050002 */  addiu $a1, $zero, 2
    /* 800FDE78 46102003 */  .word 0x46102003
    /* 800FDE7C C7A40044 */  lwc1 $f4, 68($sp)
    /* 800FDE80 24010001 */  addiu $at, $zero, 1
    /* 800FDE84 46009282 */  .word 0x46009282
    /* 800FDE88 00000000 */  nop
    /* 800FDE8C 46003202 */  .word 0x46003202
    /* 800FDE90 00000000 */  nop
    /* 800FDE94 46002402 */  .word 0x46002402
    /* 800FDE98 E7AA003C */  swc1 $f10, 60($sp)
    /* 800FDE9C E7A80040 */  swc1 $f8, 64($sp)
    /* 800FDEA0 E7B00044 */  swc1 $f16, 68($sp)
    /* 800FDEA4 C5120124 */  lwc1 $f18, 292($t0)
    /* 800FDEA8 C5080128 */  lwc1 $f8, 296($t0)
    /* 800FDEAC 810D07CC */  lb $t5, 1996($t0)
    /* 800FDEB0 460A9181 */  .word 0x460A9181
    /* 800FDEB4 C512012C */  lwc1 $f18, 300($t0)
    /* 800FDEB8 00AD1826 */  xor $v1, $a1, $t5
    /* 800FDEBC 2C630001 */  sltiu $v1, $v1, 1
    /* 800FDEC0 E5060124 */  swc1 $f6, 292($t0)
    /* 800FDEC4 C7A40040 */  lwc1 $f4, 64($sp)
    /* 800FDEC8 46044401 */  .word 0x46044401
    /* 800FDECC E5100128 */  swc1 $f16, 296($t0)
    /* 800FDED0 C7AA0044 */  lwc1 $f10, 68($sp)
    /* 800FDED4 460A9181 */  .word 0x460A9181
    /* 800FDED8 E506012C */  swc1 $f6, 300($t0)
    /* 800FDEDC 8042A634 */  lb $v0, -22988($v0)
    /* 800FDEE0 10410010 */  beq $v0, $at, 0x800FDF24
    /* 800FDEE4 00000000 */  nop
    /* 800FDEE8 14A20003 */  bne $a1, $v0, 0x800FDEF8
    /* 800FDEEC 27A4003C */  addiu $a0, $sp, 60
    /* 800FDEF0 1460000C */  bne $v1, $zero, 0x800FDF24
    /* 800FDEF4 00000000 */  nop
    /* 800FDEF8 0C022CF2 */  jal 0x8008B3C8
    /* 800FDEFC AFA8007C */  sw $t0, 124($sp)
    /* 800FDF00 8FA8007C */  lw $t0, 124($sp)
    /* 800FDF04 3C018012 */  lui $at, 0x8012
    /* 800FDF08 C4244820 */  lwc1 $f4, 18464($at)
    /* 800FDF0C C508063C */  lwc1 $f8, 1596($t0)
    /* 800FDF10 24050002 */  addiu $a1, $zero, 2
    /* 800FDF14 46044402 */  .word 0x46044402
    /* 800FDF18 4600803C */  .word 0x4600803C
    /* 800FDF1C 00000000 */  nop
    /* 800FDF20 45000015 */  .word 0x45000015
    /* 800FDF24 3C0E8015 */  lui $t6, 0x8015
    /* 800FDF28 8DCEA110 */  lw $t6, -24304($t6)
    /* 800FDF2C 24010006 */  addiu $at, $zero, 6
    /* 800FDF30 51C10012 */  .word 0x51C10012
    /* 800FDF34 8FBF0014 */  lw $ra, 20($sp)
    /* 800FDF38 810F0640 */  lb $t7, 1600($t0)
    /* 800FDF3C 55E0000F */  .word 0x55E0000F
    /* 800FDF40 8FBF0014 */  lw $ra, 20($sp)
    /* 800FDF44 851807C6 */  lh $t8, 1990($t0)
    /* 800FDF48 3C0A8015 */  lui $t2, 0x8015
    /* 800FDF4C 240B0001 */  addiu $t3, $zero, 1
    /* 800FDF50 0018C900 */  sll $t9, $t8, 4
    /* 800FDF54 0338C823 */  subu $t9, $t9, $t8
    /* 800FDF58 0019C8C0 */  sll $t9, $t9, 3
    /* 800FDF5C 0338C823 */  subu $t9, $t9, $t8
    /* 800FDF60 0019C8C0 */  sll $t9, $t9, 3
    /* 800FDF64 01595021 */  addu $t2, $t2, $t9
    /* 800FDF68 814A2B71 */  lb $t2, 11121($t2)
    /* 800FDF6C 50AA0003 */  .word 0x50AA0003
    /* 800FDF70 8FBF0014 */  lw $ra, 20($sp)
    /* 800FDF74 A10B0640 */  sb $t3, 1600($t0)
    /* 800FDF78 8FBF0014 */  lw $ra, 20($sp)
    /* 800FDF7C 27BD0078 */  addiu $sp, $sp, 120
    /* 800FDF80 03E00008 */  jr $ra
    /* 800FDF84 00000000 */  nop
