# Source: game_code.bin (decompressed)
# Address: 0x800EDDC0

glabel session_start_handler
    /* 800EDDC0 27BDFFB0 */  addiu $sp, $sp, -80
    /* 800EDDC4 3C0E8016 */  lui $t6, 0x8016
    /* 800EDDC8 AFBF0014 */  sw $ra, 20($sp)
    /* 800EDDCC 25CEF72D */  addiu $t6, $t6, -2259
    /* 800EDDD0 81CF0000 */  lb $t7, 0($t6)
    /* 800EDDD4 3C198015 */  lui $t9, 0x8015
    /* 800EDDD8 27396BE0 */  addiu $t9, $t9, 27616
    /* 800EDDDC 000FC1C0 */  sll $t8, $t7, 7
    /* 800EDDE0 03195021 */  addu $t2, $t8, $t9
    /* 800EDDE4 8D4E0058 */  lw $t6, 88($t2)
    /* 800EDDE8 3C0B8016 */  lui $t3, 0x8016
    /* 800EDDEC 256BB250 */  addiu $t3, $t3, -19888
    /* 800EDDF0 3C018016 */  lui $at, 0x8016
    /* 800EDDF4 AD6E0000 */  sw $t6, 0($t3)
    /* 800EDDF8 25CF06C0 */  addiu $t7, $t6, 1728
    /* 800EDDFC AC2FB260 */  sw $t7, -19872($at)
    /* 800EDE00 34019CC0 */  ori $at, $zero, 0x9CC0
    /* 800EDE04 3C058015 */  lui $a1, 0x8015
    /* 800EDE08 01C1C021 */  addu $t8, $t6, $at
    /* 800EDE0C 24A597C8 */  addiu $a1, $a1, -26680
    /* 800EDE10 ACB80000 */  sw $t8, 0($a1)
    /* 800EDE14 27190008 */  addiu $t9, $t8, 8
    /* 800EDE18 ACB90000 */  sw $t9, 0($a1)
    /* 800EDE1C 3C0EDB06 */  lui $t6, 0xDB06
    /* 800EDE20 AF0E0000 */  sw $t6, 0($t8)
    /* 800EDE24 AF000004 */  sw $zero, 4($t8)
    /* 800EDE28 8CA30000 */  lw $v1, 0($a1)
    /* 800EDE2C 3C188003 */  lui $t8, 0x8003
    /* 800EDE30 3C0CDE00 */  lui $t4, 0xDE00
    /* 800EDE34 246F0008 */  addiu $t7, $v1, 8
    /* 800EDE38 ACAF0000 */  sw $t7, 0($a1)
    /* 800EDE3C 2718E528 */  addiu $t8, $t8, -6872
    /* 800EDE40 AC780004 */  sw $t8, 4($v1)
    /* 800EDE44 AC6C0000 */  sw $t4, 0($v1)
    /* 800EDE48 8CA30000 */  lw $v1, 0($a1)
    /* 800EDE4C 3C0E8003 */  lui $t6, 0x8003
    /* 800EDE50 25CEE4A0 */  addiu $t6, $t6, -7008
    /* 800EDE54 24790008 */  addiu $t9, $v1, 8
    /* 800EDE58 ACB90000 */  sw $t9, 0($a1)
    /* 800EDE5C AC6C0000 */  sw $t4, 0($v1)
    /* 800EDE60 AC6E0004 */  sw $t6, 4($v1)
    /* 800EDE64 8CA30000 */  lw $v1, 0($a1)
    /* 800EDE68 3C18FE00 */  lui $t8, 0xFE00
    /* 800EDE6C 3C048003 */  lui $a0, 0x8003
    /* 800EDE70 246F0008 */  addiu $t7, $v1, 8
    /* 800EDE74 ACAF0000 */  sw $t7, 0($a1)
    /* 800EDE78 AC780000 */  sw $t8, 0($v1)
    /* 800EDE7C AFAA0048 */  sw $t2, 72($sp)
    /* 800EDE80 8C84EBB0 */  lw $a0, -5200($a0)
    /* 800EDE84 0C003570 */  jal 0x8000D5C0
    /* 800EDE88 AFA30038 */  sw $v1, 56($sp)
    /* 800EDE8C 8FA90038 */  lw $t1, 56($sp)
    /* 800EDE90 3C058015 */  lui $a1, 0x8015
    /* 800EDE94 24A597C8 */  addiu $a1, $a1, -26680
    /* 800EDE98 AD220004 */  sw $v0, 4($t1)
    /* 800EDE9C 8CA30000 */  lw $v1, 0($a1)
    /* 800EDEA0 3C0DE700 */  lui $t5, 0xE700
    /* 800EDEA4 3C0FE300 */  lui $t7, 0xE300
    /* 800EDEA8 24790008 */  addiu $t9, $v1, 8
    /* 800EDEAC ACB90000 */  sw $t9, 0($a1)
    /* 800EDEB0 AC6D0000 */  sw $t5, 0($v1)
    /* 800EDEB4 AC600004 */  sw $zero, 4($v1)
    /* 800EDEB8 8CA30000 */  lw $v1, 0($a1)
    /* 800EDEBC 35EF0A01 */  ori $t7, $t7, 0x0A01
    /* 800EDEC0 3C180030 */  lui $t8, 0x0030
    /* 800EDEC4 246E0008 */  addiu $t6, $v1, 8
    /* 800EDEC8 ACAE0000 */  sw $t6, 0($a1)
    /* 800EDECC 3C068003 */  lui $a2, 0x8003
    /* 800EDED0 AC6F0000 */  sw $t7, 0($v1)
    /* 800EDED4 AC780004 */  sw $t8, 4($v1)
    /* 800EDED8 24C6AFC0 */  addiu $a2, $a2, -20544
    /* 800EDEDC 8CCE0000 */  lw $t6, 0($a2)
    /* 800EDEE0 8CA30000 */  lw $v1, 0($a1)
    /* 800EDEE4 3C01FF10 */  lui $at, 0xFF10
    /* 800EDEE8 25CFFFFF */  addiu $t7, $t6, -1
    /* 800EDEEC 24790008 */  addiu $t9, $v1, 8
    /* 800EDEF0 ACB90000 */  sw $t9, 0($a1)
    /* 800EDEF4 31F80FFF */  andi $t8, $t7, 0x0FFF
    /* 800EDEF8 0301C825 */  or $t9, $t8, $at
    /* 800EDEFC 3C048003 */  lui $a0, 0x8003
    /* 800EDF00 AC790000 */  sw $t9, 0($v1)
    /* 800EDF04 8C84EBB0 */  lw $a0, -5200($a0)
    /* 800EDF08 0C003570 */  jal 0x8000D5C0
    /* 800EDF0C AFA3002C */  sw $v1, 44($sp)
    /* 800EDF10 8FAC002C */  lw $t4, 44($sp)
    /* 800EDF14 3C058015 */  lui $a1, 0x8015
    /* 800EDF18 24A597C8 */  addiu $a1, $a1, -26680
    /* 800EDF1C AD820004 */  sw $v0, 4($t4)
    /* 800EDF20 8CA30000 */  lw $v1, 0($a1)
    /* 800EDF24 3C18FFFC */  lui $t8, 0xFFFC
    /* 800EDF28 3C068003 */  lui $a2, 0x8003
    /* 800EDF2C 246E0008 */  addiu $t6, $v1, 8
    /* 800EDF30 ACAE0000 */  sw $t6, 0($a1)
    /* 800EDF34 3718FFFC */  ori $t8, $t8, 0xFFFC
    /* 800EDF38 3C0FF700 */  lui $t7, 0xF700
    /* 800EDF3C 24C6AFC0 */  addiu $a2, $a2, -20544
    /* 800EDF40 AC6F0000 */  sw $t7, 0($v1)
    /* 800EDF44 AC780004 */  sw $t8, 4($v1)
    /* 800EDF48 8CCE0000 */  lw $t6, 0($a2)
    /* 800EDF4C 8CA30000 */  lw $v1, 0($a1)
    /* 800EDF50 3C078003 */  lui $a3, 0x8003
    /* 800EDF54 25CFFFFF */  addiu $t7, $t6, -1
    /* 800EDF58 24E7AFC4 */  addiu $a3, $a3, -20540
    /* 800EDF5C 31F803FF */  andi $t8, $t7, 0x03FF
    /* 800EDF60 24790008 */  addiu $t9, $v1, 8
    /* 800EDF64 8CEF0000 */  lw $t7, 0($a3)
    /* 800EDF68 ACB90000 */  sw $t9, 0($a1)
    /* 800EDF6C 0018CB80 */  sll $t9, $t8, 14
    /* 800EDF70 3C1FF600 */  lui $ra, 0xF600
    /* 800EDF74 033F7025 */  or $t6, $t9, $ra
    /* 800EDF78 25F8FFFF */  addiu $t8, $t7, -1
    /* 800EDF7C 331903FF */  andi $t9, $t8, 0x03FF
    /* 800EDF80 00197880 */  sll $t7, $t9, 2
    /* 800EDF84 01CFC025 */  or $t8, $t6, $t7
    /* 800EDF88 AC780000 */  sw $t8, 0($v1)
    /* 800EDF8C AC600004 */  sw $zero, 4($v1)
    /* 800EDF90 8CA30000 */  lw $v1, 0($a1)
    /* 800EDF94 3C0ED9FF */  lui $t6, 0xD9FF
    /* 800EDF98 35CEFFFF */  ori $t6, $t6, 0xFFFF
    /* 800EDF9C 24790008 */  addiu $t9, $v1, 8
    /* 800EDFA0 ACB90000 */  sw $t9, 0($a1)
    /* 800EDFA4 240F0001 */  addiu $t7, $zero, 1
    /* 800EDFA8 AC6F0004 */  sw $t7, 4($v1)
    /* 800EDFAC AC6E0000 */  sw $t6, 0($v1)
    /* 800EDFB0 8CA30000 */  lw $v1, 0($a1)
    /* 800EDFB4 3C188012 */  lui $t8, 0x8012
    /* 800EDFB8 8F184FC8 */  lw $t8, 20424($t8)
    /* 800EDFBC 24790008 */  addiu $t9, $v1, 8
    /* 800EDFC0 ACB90000 */  sw $t9, 0($a1)
    /* 800EDFC4 AFB8004C */  sw $t8, 76($sp)
    /* 800EDFC8 3C0DE700 */  lui $t5, 0xE700
    /* 800EDFCC AC6D0000 */  sw $t5, 0($v1)
    /* 800EDFD0 AC600004 */  sw $zero, 4($v1)
    /* 800EDFD4 8CCF0000 */  lw $t7, 0($a2)
    /* 800EDFD8 8CA30000 */  lw $v1, 0($a1)
    /* 800EDFDC 3C01FF10 */  lui $at, 0xFF10
    /* 800EDFE0 25F8FFFF */  addiu $t8, $t7, -1
    /* 800EDFE4 246E0008 */  addiu $t6, $v1, 8
    /* 800EDFE8 ACAE0000 */  sw $t6, 0($a1)
    /* 800EDFEC 33190FFF */  andi $t9, $t8, 0x0FFF
    /* 800EDFF0 03217025 */  or $t6, $t9, $at
    /* 800EDFF4 AFA30018 */  sw $v1, 24($sp)
    /* 800EDFF8 AC6E0000 */  sw $t6, 0($v1)
    /* 800EDFFC 8FB80048 */  lw $t8, 72($sp)
    /* 800EE000 0C003570 */  jal 0x8000D5C0
    /* 800EE004 8F04007C */  lw $a0, 124($t8)
    /* 800EE008 8FB90018 */  lw $t9, 24($sp)
    /* 800EE00C 3C058015 */  lui $a1, 0x8015
    /* 800EE010 24A597C8 */  addiu $a1, $a1, -26680
    /* 800EE014 AF220004 */  sw $v0, 4($t9)
    /* 800EE018 8CA30000 */  lw $v1, 0($a1)
    /* 800EE01C 3C0FF700 */  lui $t7, 0xF700
    /* 800EE020 3C068003 */  lui $a2, 0x8003
    /* 800EE024 246E0008 */  addiu $t6, $v1, 8
    /* 800EE028 ACAE0000 */  sw $t6, 0($a1)
    /* 800EE02C AC6F0000 */  sw $t7, 0($v1)
    /* 800EE030 8FB8004C */  lw $t8, 76($sp)
    /* 800EE034 24C6AFC0 */  addiu $a2, $a2, -20544
    /* 800EE038 3C078003 */  lui $a3, 0x8003
    /* 800EE03C AC780004 */  sw $t8, 4($v1)
    /* 800EE040 8CCE0000 */  lw $t6, 0($a2)
    /* 800EE044 8CA30000 */  lw $v1, 0($a1)
    /* 800EE048 24E7AFC4 */  addiu $a3, $a3, -20540
    /* 800EE04C 25CFFFFF */  addiu $t7, $t6, -1
    /* 800EE050 31F803FF */  andi $t8, $t7, 0x03FF
    /* 800EE054 24790008 */  addiu $t9, $v1, 8
    /* 800EE058 8CEF0000 */  lw $t7, 0($a3)
    /* 800EE05C ACB90000 */  sw $t9, 0($a1)
    /* 800EE060 0018CB80 */  sll $t9, $t8, 14
    /* 800EE064 3C1FF600 */  lui $ra, 0xF600
    /* 800EE068 033F7025 */  or $t6, $t9, $ra
    /* 800EE06C 25F8FFFF */  addiu $t8, $t7, -1
    /* 800EE070 331903FF */  andi $t9, $t8, 0x03FF
    /* 800EE074 00197880 */  sll $t7, $t9, 2
    /* 800EE078 01CFC025 */  or $t8, $t6, $t7
    /* 800EE07C 00002025 */  or $a0, $zero, $zero
    /* 800EE080 AC780000 */  sw $t8, 0($v1)
    /* 800EE084 0C029AF9 */  jal 0x800A6BE4
    /* 800EE088 AC600004 */  sw $zero, 4($v1)
    /* 800EE08C 3C058015 */  lui $a1, 0x8015
    /* 800EE090 3C068003 */  lui $a2, 0x8003
    /* 800EE094 3C078003 */  lui $a3, 0x8003
    /* 800EE098 24010001 */  addiu $at, $zero, 1
    /* 800EE09C 24E7AFC4 */  addiu $a3, $a3, -20540
    /* 800EE0A0 24C6AFC0 */  addiu $a2, $a2, -20544
    /* 800EE0A4 24A597C8 */  addiu $a1, $a1, -26680
    /* 800EE0A8 3C0DE700 */  lui $t5, 0xE700
    /* 800EE0AC 1041007D */  beq $v0, $at, 0x800EE2A4
    /* 800EE0B0 3C1FF600 */  lui $ra, 0xF600
    /* 800EE0B4 240B0002 */  addiu $t3, $zero, 2
    /* 800EE0B8 144B00D2 */  bne $v0, $t3, 0x800EE404
    /* 800EE0BC 3C198015 */  lui $t9, 0x8015
    /* 800EE0C0 87391AD0 */  lh $t9, 6864($t9)
    /* 800EE0C4 24010003 */  addiu $at, $zero, 3
    /* 800EE0C8 5721002E */  .word 0x5721002E
    /* 800EE0CC 8CA20000 */  lw $v0, 0($a1)
    /* 800EE0D0 8CA20000 */  lw $v0, 0($a1)
    /* 800EE0D4 3C190001 */  lui $t9, 0x0001
    /* 800EE0D8 37390001 */  ori $t9, $t9, 0x0001
    /* 800EE0DC 244E0008 */  addiu $t6, $v0, 8
    /* 800EE0E0 ACAE0000 */  sw $t6, 0($a1)
    /* 800EE0E4 AC400004 */  sw $zero, 4($v0)
    /* 800EE0E8 AC4D0000 */  sw $t5, 0($v0)
    /* 800EE0EC 8CA40000 */  lw $a0, 0($a1)
    /* 800EE0F0 3C18F700 */  lui $t8, 0xF700
    /* 800EE0F4 248F0008 */  addiu $t7, $a0, 8
    /* 800EE0F8 ACAF0000 */  sw $t7, 0($a1)
    /* 800EE0FC AC990004 */  sw $t9, 4($a0)
    /* 800EE100 AC980000 */  sw $t8, 0($a0)
    /* 800EE104 8CCF0000 */  lw $t7, 0($a2)
    /* 800EE108 8CA30000 */  lw $v1, 0($a1)
    /* 800EE10C 25F8FFFF */  addiu $t8, $t7, -1
    /* 800EE110 331903FF */  andi $t9, $t8, 0x03FF
    /* 800EE114 246E0008 */  addiu $t6, $v1, 8
    /* 800EE118 8CF80000 */  lw $t8, 0($a3)
    /* 800EE11C ACAE0000 */  sw $t6, 0($a1)
    /* 800EE120 00197380 */  sll $t6, $t9, 14
    /* 800EE124 01DF7825 */  or $t7, $t6, $ra
    /* 800EE128 2719FFFF */  addiu $t9, $t8, -1
    /* 800EE12C 332E03FF */  andi $t6, $t9, 0x03FF
    /* 800EE130 000EC080 */  sll $t8, $t6, 2
    /* 800EE134 01F8C825 */  or $t9, $t7, $t8
    /* 800EE138 AC790000 */  sw $t9, 0($v1)
    /* 800EE13C 8CCE0000 */  lw $t6, 0($a2)
    /* 800EE140 00604025 */  or $t0, $v1, $zero
    /* 800EE144 05C10003 */  bgez $t6, 0x800EE154
    /* 800EE148 000E7843 */  sra $t7, $t6, 1
    /* 800EE14C 25C10001 */  addiu $at, $t6, 1
    /* 800EE150 00017843 */  sra $t7, $at, 1
    /* 800EE154 31F803FF */  andi $t8, $t7, 0x03FF
    /* 800EE158 8CEE0000 */  lw $t6, 0($a3)
    /* 800EE15C 0018CB80 */  sll $t9, $t8, 14
    /* 800EE160 05C10003 */  bgez $t6, 0x800EE170
    /* 800EE164 000E7843 */  sra $t7, $t6, 1
    /* 800EE168 25C10001 */  addiu $at, $t6, 1
    /* 800EE16C 00017843 */  sra $t7, $at, 1
    /* 800EE170 31F803FF */  andi $t8, $t7, 0x03FF
    /* 800EE174 00187080 */  sll $t6, $t8, 2
    /* 800EE178 032E7825 */  or $t7, $t9, $t6
    /* 800EE17C AD0F0004 */  sw $t7, 4($t0)
    /* 800EE180 8CA20000 */  lw $v0, 0($a1)
    /* 800EE184 3C0EF700 */  lui $t6, 0xF700
    /* 800EE188 3C0F0001 */  lui $t7, 0x0001
    /* 800EE18C 24580008 */  addiu $t8, $v0, 8
    /* 800EE190 ACB80000 */  sw $t8, 0($a1)
    /* 800EE194 AC400004 */  sw $zero, 4($v0)
    /* 800EE198 AC4D0000 */  sw $t5, 0($v0)
    /* 800EE19C 8CA40000 */  lw $a0, 0($a1)
    /* 800EE1A0 35EF0001 */  ori $t7, $t7, 0x0001
    /* 800EE1A4 24990008 */  addiu $t9, $a0, 8
    /* 800EE1A8 ACB90000 */  sw $t9, 0($a1)
    /* 800EE1AC AC8E0000 */  sw $t6, 0($a0)
    /* 800EE1B0 AC8F0004 */  sw $t7, 4($a0)
    /* 800EE1B4 8CD90000 */  lw $t9, 0($a2)
    /* 800EE1B8 8CA30000 */  lw $v1, 0($a1)
    /* 800EE1BC 24780008 */  addiu $t8, $v1, 8
    /* 800EE1C0 ACB80000 */  sw $t8, 0($a1)
    /* 800EE1C4 00604025 */  or $t0, $v1, $zero
    /* 800EE1C8 07210003 */  bgez $t9, 0x800EE1D8
    /* 800EE1CC 00197043 */  sra $t6, $t9, 1
    /* 800EE1D0 27210001 */  addiu $at, $t9, 1
    /* 800EE1D4 00017043 */  sra $t6, $at, 1
    /* 800EE1D8 31CF03FF */  andi $t7, $t6, 0x03FF
    /* 800EE1DC 8CEE0000 */  lw $t6, 0($a3)
    /* 800EE1E0 000FC380 */  sll $t8, $t7, 14
    /* 800EE1E4 031FC825 */  or $t9, $t8, $ra
    /* 800EE1E8 25CFFFFF */  addiu $t7, $t6, -1
    /* 800EE1EC 31F803FF */  andi $t8, $t7, 0x03FF
    /* 800EE1F0 00187080 */  sll $t6, $t8, 2
    /* 800EE1F4 032E7825 */  or $t7, $t9, $t6
    /* 800EE1F8 AD0F0000 */  sw $t7, 0($t0)
    /* 800EE1FC 8CD80000 */  lw $t8, 0($a2)
    /* 800EE200 07010003 */  bgez $t8, 0x800EE210
    /* 800EE204 0018C843 */  sra $t9, $t8, 1
    /* 800EE208 27010001 */  addiu $at, $t8, 1
    /* 800EE20C 0001C843 */  sra $t9, $at, 1
    /* 800EE210 272EFFFF */  addiu $t6, $t9, -1
    /* 800EE214 31CF03FF */  andi $t7, $t6, 0x03FF
    /* 800EE218 000FC380 */  sll $t8, $t7, 14
    /* 800EE21C AD180004 */  sw $t8, 4($t0)
    /* 800EE220 8CA90000 */  lw $t1, 0($a1)
    /* 800EE224 25390008 */  addiu $t9, $t1, 8
    /* 800EE228 ACB90000 */  sw $t9, 0($a1)
    /* 800EE22C AD200004 */  sw $zero, 4($t1)
    /* 800EE230 AD2D0000 */  sw $t5, 0($t1)
    /* 800EE234 8CCF0000 */  lw $t7, 0($a2)
    /* 800EE238 8CA30000 */  lw $v1, 0($a1)
    /* 800EE23C 25F8FFFF */  addiu $t8, $t7, -1
    /* 800EE240 246E0008 */  addiu $t6, $v1, 8
    /* 800EE244 ACAE0000 */  sw $t6, 0($a1)
    /* 800EE248 331903FF */  andi $t9, $t8, 0x03FF
    /* 800EE24C 00197380 */  sll $t6, $t9, 14
    /* 800EE250 8CF80000 */  lw $t8, 0($a3)
    /* 800EE254 01DF7825 */  or $t7, $t6, $ra
    /* 800EE258 00605025 */  or $t2, $v1, $zero
    /* 800EE25C 07010003 */  bgez $t8, 0x800EE26C
    /* 800EE260 0018C843 */  sra $t9, $t8, 1
    /* 800EE264 27010001 */  addiu $at, $t8, 1
    /* 800EE268 0001C843 */  sra $t9, $at, 1
    /* 800EE26C 332E03FF */  andi $t6, $t9, 0x03FF
    /* 800EE270 000EC080 */  sll $t8, $t6, 2
    /* 800EE274 01F8C825 */  or $t9, $t7, $t8
    /* 800EE278 AD590000 */  sw $t9, 0($t2)
    /* 800EE27C 8CEE0000 */  lw $t6, 0($a3)
    /* 800EE280 05C10003 */  bgez $t6, 0x800EE290
    /* 800EE284 000E7843 */  sra $t7, $t6, 1
    /* 800EE288 25C10001 */  addiu $at, $t6, 1
    /* 800EE28C 00017843 */  sra $t7, $at, 1
    /* 800EE290 25F8FFFF */  addiu $t8, $t7, -1
    /* 800EE294 331903FF */  andi $t9, $t8, 0x03FF
    /* 800EE298 00197080 */  sll $t6, $t9, 2
    /* 800EE29C 10000059 */  beq $zero, $zero, 0x800EE404
    /* 800EE2A0 AD4E0004 */  sw $t6, 4($t2)
    /* 800EE2A4 8CA20000 */  lw $v0, 0($a1)
    /* 800EE2A8 3C0E0001 */  lui $t6, 0x0001
    /* 800EE2AC 35CE0001 */  ori $t6, $t6, 0x0001
    /* 800EE2B0 244F0008 */  addiu $t7, $v0, 8
    /* 800EE2B4 ACAF0000 */  sw $t7, 0($a1)
    /* 800EE2B8 AC400004 */  sw $zero, 4($v0)
    /* 800EE2BC AC4D0000 */  sw $t5, 0($v0)
    /* 800EE2C0 8CA40000 */  lw $a0, 0($a1)
    /* 800EE2C4 3C19F700 */  lui $t9, 0xF700
    /* 800EE2C8 240B0002 */  addiu $t3, $zero, 2
    /* 800EE2CC 24980008 */  addiu $t8, $a0, 8
    /* 800EE2D0 ACB80000 */  sw $t8, 0($a1)
    /* 800EE2D4 AC8E0004 */  sw $t6, 4($a0)
    /* 800EE2D8 AC990000 */  sw $t9, 0($a0)
    /* 800EE2DC 8CD80000 */  lw $t8, 0($a2)
    /* 800EE2E0 8CA30000 */  lw $v1, 0($a1)
    /* 800EE2E4 2719FFFF */  addiu $t9, $t8, -1
    /* 800EE2E8 332E03FF */  andi $t6, $t9, 0x03FF
    /* 800EE2EC 246F0008 */  addiu $t7, $v1, 8
    /* 800EE2F0 8CF90000 */  lw $t9, 0($a3)
    /* 800EE2F4 ACAF0000 */  sw $t7, 0($a1)
    /* 800EE2F8 000E7B80 */  sll $t7, $t6, 14
    /* 800EE2FC 01FFC025 */  or $t8, $t7, $ra
    /* 800EE300 272EFFFF */  addiu $t6, $t9, -1
    /* 800EE304 31CF03FF */  andi $t7, $t6, 0x03FF
    /* 800EE308 000FC880 */  sll $t9, $t7, 2
    /* 800EE30C 03197025 */  or $t6, $t8, $t9
    /* 800EE310 AC6E0000 */  sw $t6, 0($v1)
    /* 800EE314 8CCF0000 */  lw $t7, 0($a2)
    /* 800EE318 00604025 */  or $t0, $v1, $zero
    /* 800EE31C 000FC080 */  sll $t8, $t7, 2
    /* 800EE320 030FC023 */  subu $t8, $t8, $t7
    /* 800EE324 07010003 */  bgez $t8, 0x800EE334
    /* 800EE328 0018C883 */  sra $t9, $t8, 2
    /* 800EE32C 27010003 */  addiu $at, $t8, 3
    /* 800EE330 0001C883 */  sra $t9, $at, 2
    /* 800EE334 332E03FF */  andi $t6, $t9, 0x03FF
    /* 800EE338 000E7B80 */  sll $t7, $t6, 14
    /* 800EE33C AD0F0004 */  sw $t7, 4($t0)
    /* 800EE340 8CA90000 */  lw $t1, 0($a1)
    /* 800EE344 25380008 */  addiu $t8, $t1, 8
    /* 800EE348 ACB80000 */  sw $t8, 0($a1)
    /* 800EE34C AD200004 */  sw $zero, 4($t1)
    /* 800EE350 AD2D0000 */  sw $t5, 0($t1)
    /* 800EE354 8CA30000 */  lw $v1, 0($a1)
    /* 800EE358 8CCE0000 */  lw $t6, 0($a2)
    /* 800EE35C 24790008 */  addiu $t9, $v1, 8
    /* 800EE360 25CFFFFF */  addiu $t7, $t6, -1
    /* 800EE364 31F803FF */  andi $t8, $t7, 0x03FF
    /* 800EE368 ACB90000 */  sw $t9, 0($a1)
    /* 800EE36C 0018CB80 */  sll $t9, $t8, 14
    /* 800EE370 8CEF0000 */  lw $t7, 0($a3)
    /* 800EE374 033F7025 */  or $t6, $t9, $ra
    /* 800EE378 00605025 */  or $t2, $v1, $zero
    /* 800EE37C 05E10003 */  bgez $t7, 0x800EE38C
    /* 800EE380 000FC043 */  sra $t8, $t7, 1
    /* 800EE384 25E10001 */  addiu $at, $t7, 1
    /* 800EE388 0001C043 */  sra $t8, $at, 1
    /* 800EE38C 331903FF */  andi $t9, $t8, 0x03FF
    /* 800EE390 00197880 */  sll $t7, $t9, 2
    /* 800EE394 01CFC025 */  or $t8, $t6, $t7
    /* 800EE398 AD580000 */  sw $t8, 0($t2)
    /* 800EE39C 8CF90000 */  lw $t9, 0($a3)
    /* 800EE3A0 07210003 */  bgez $t9, 0x800EE3B0
    /* 800EE3A4 00197043 */  sra $t6, $t9, 1
    /* 800EE3A8 27210001 */  addiu $at, $t9, 1
    /* 800EE3AC 00017043 */  sra $t6, $at, 1
    /* 800EE3B0 25CFFFFF */  addiu $t7, $t6, -1
    /* 800EE3B4 31F803FF */  andi $t8, $t7, 0x03FF
    /* 800EE3B8 0018C880 */  sll $t9, $t8, 2
    /* 800EE3BC AD590004 */  sw $t9, 4($t2)
    /* 800EE3C0 8CA30000 */  lw $v1, 0($a1)
    /* 800EE3C4 3C01F600 */  lui $at, 0xF600
    /* 800EE3C8 34214000 */  ori $at, $at, 0x4000
    /* 800EE3CC 246E0008 */  addiu $t6, $v1, 8
    /* 800EE3D0 ACAE0000 */  sw $t6, 0($a1)
    /* 800EE3D4 AC600004 */  sw $zero, 4($v1)
    /* 800EE3D8 AC6D0000 */  sw $t5, 0($v1)
    /* 800EE3DC 8CF80000 */  lw $t8, 0($a3)
    /* 800EE3E0 8CA30000 */  lw $v1, 0($a1)
    /* 800EE3E4 2719FFFF */  addiu $t9, $t8, -1
    /* 800EE3E8 246F0008 */  addiu $t7, $v1, 8
    /* 800EE3EC ACAF0000 */  sw $t7, 0($a1)
    /* 800EE3F0 332E03FF */  andi $t6, $t9, 0x03FF
    /* 800EE3F4 000E7880 */  sll $t7, $t6, 2
    /* 800EE3F8 01E1C025 */  or $t8, $t7, $at
    /* 800EE3FC AC780000 */  sw $t8, 0($v1)
    /* 800EE400 AC600004 */  sw $zero, 4($v1)
    /* 800EE404 8CA30000 */  lw $v1, 0($a1)
    /* 800EE408 3C028016 */  lui $v0, 0x8016
    /* 800EE40C 24010001 */  addiu $at, $zero, 1
    /* 800EE410 24790008 */  addiu $t9, $v1, 8
    /* 800EE414 ACB90000 */  sw $t9, 0($a1)
    /* 800EE418 AC600004 */  sw $zero, 4($v1)
    /* 800EE41C AC6D0000 */  sw $t5, 0($v1)
    /* 800EE420 8C42F738 */  lw $v0, -2248($v0)
    /* 800EE424 3C0FE300 */  lui $t7, 0xE300
    /* 800EE428 50400008 */  .word 0x50400008
    /* 800EE42C 8CA30000 */  lw $v1, 0($a1)
    /* 800EE430 1041000D */  beq $v0, $at, 0x800EE468
    /* 800EE434 3C0EE300 */  lui $t6, 0xE300
    /* 800EE438 504B0014 */  .word 0x504B0014
    /* 800EE43C 8CA30000 */  lw $v1, 0($a1)
    /* 800EE440 10000018 */  beq $zero, $zero, 0x800EE4A4
    /* 800EE444 00000000 */  nop
    /* 800EE448 8CA30000 */  lw $v1, 0($a1)
    /* 800EE44C 35EF1201 */  ori $t7, $t7, 0x1201
    /* 800EE450 24182000 */  addiu $t8, $zero, 8192
    /* 800EE454 246E0008 */  addiu $t6, $v1, 8
    /* 800EE458 ACAE0000 */  sw $t6, 0($a1)
    /* 800EE45C AC780004 */  sw $t8, 4($v1)
    /* 800EE460 10000010 */  beq $zero, $zero, 0x800EE4A4
    /* 800EE464 AC6F0000 */  sw $t7, 0($v1)
    /* 800EE468 8CA30000 */  lw $v1, 0($a1)
    /* 800EE46C 35CE1201 */  ori $t6, $t6, 0x1201
    /* 800EE470 240F3000 */  addiu $t7, $zero, 12288
    /* 800EE474 24790008 */  addiu $t9, $v1, 8
    /* 800EE478 ACB90000 */  sw $t9, 0($a1)
    /* 800EE47C AC6F0004 */  sw $t7, 4($v1)
    /* 800EE480 10000008 */  beq $zero, $zero, 0x800EE4A4
    /* 800EE484 AC6E0000 */  sw $t6, 0($v1)
    /* 800EE488 8CA30000 */  lw $v1, 0($a1)
    /* 800EE48C 3C19E300 */  lui $t9, 0xE300
    /* 800EE490 37391201 */  ori $t9, $t9, 0x1201
    /* 800EE494 24780008 */  addiu $t8, $v1, 8
    /* 800EE498 ACB80000 */  sw $t8, 0($a1)
    /* 800EE49C AC600004 */  sw $zero, 4($v1)
    /* 800EE4A0 AC790000 */  sw $t9, 0($v1)
    /* 800EE4A4 3C028016 */  lui $v0, 0x8016
    /* 800EE4A8 8C421380 */  lw $v0, 4992($v0)
    /* 800EE4AC 24010001 */  addiu $at, $zero, 1
    /* 800EE4B0 3C0FE300 */  lui $t7, 0xE300
    /* 800EE4B4 5040000A */  .word 0x5040000A
    /* 800EE4B8 8CA30000 */  lw $v1, 0($a1)
    /* 800EE4BC 1041000F */  beq $v0, $at, 0x800EE4FC
    /* 800EE4C0 3C0EE300 */  lui $t6, 0xE300
    /* 800EE4C4 104B0015 */  beq $v0, $t3, 0x800EE51C
    /* 800EE4C8 24010003 */  addiu $at, $zero, 3
    /* 800EE4CC 1041001B */  beq $v0, $at, 0x800EE53C
    /* 800EE4D0 3C0FE300 */  lui $t7, 0xE300
    /* 800EE4D4 10000020 */  beq $zero, $zero, 0x800EE558
    /* 800EE4D8 00000000 */  nop
    /* 800EE4DC 8CA30000 */  lw $v1, 0($a1)
    /* 800EE4E0 35EF1801 */  ori $t7, $t7, 0x1801
    /* 800EE4E4 241800C0 */  addiu $t8, $zero, 192
    /* 800EE4E8 246E0008 */  addiu $t6, $v1, 8
    /* 800EE4EC ACAE0000 */  sw $t6, 0($a1)
    /* 800EE4F0 AC780004 */  sw $t8, 4($v1)
    /* 800EE4F4 10000018 */  beq $zero, $zero, 0x800EE558
    /* 800EE4F8 AC6F0000 */  sw $t7, 0($v1)
    /* 800EE4FC 8CA30000 */  lw $v1, 0($a1)
    /* 800EE500 35CE1801 */  ori $t6, $t6, 0x1801
    /* 800EE504 240F0040 */  addiu $t7, $zero, 64
    /* 800EE508 24790008 */  addiu $t9, $v1, 8
    /* 800EE50C ACB90000 */  sw $t9, 0($a1)
    /* 800EE510 AC6F0004 */  sw $t7, 4($v1)
    /* 800EE514 10000010 */  beq $zero, $zero, 0x800EE558
    /* 800EE518 AC6E0000 */  sw $t6, 0($v1)
    /* 800EE51C 8CA30000 */  lw $v1, 0($a1)
    /* 800EE520 3C19E300 */  lui $t9, 0xE300
    /* 800EE524 37391801 */  ori $t9, $t9, 0x1801
    /* 800EE528 24780008 */  addiu $t8, $v1, 8
    /* 800EE52C ACB80000 */  sw $t8, 0($a1)
    /* 800EE530 AC600004 */  sw $zero, 4($v1)
    /* 800EE534 10000008 */  beq $zero, $zero, 0x800EE558
    /* 800EE538 AC790000 */  sw $t9, 0($v1)
    /* 800EE53C 8CA30000 */  lw $v1, 0($a1)
    /* 800EE540 35EF1801 */  ori $t7, $t7, 0x1801
    /* 800EE544 24180080 */  addiu $t8, $zero, 128
    /* 800EE548 246E0008 */  addiu $t6, $v1, 8
    /* 800EE54C ACAE0000 */  sw $t6, 0($a1)
    /* 800EE550 AC780004 */  sw $t8, 4($v1)
    /* 800EE554 AC6F0000 */  sw $t7, 0($v1)
    /* 800EE558 3C028016 */  lui $v0, 0x8016
    /* 800EE55C 8C421398 */  lw $v0, 5016($v0)
    /* 800EE560 24010001 */  addiu $at, $zero, 1
    /* 800EE564 10400005 */  beq $v0, $zero, 0x800EE57C
    /* 800EE568 00000000 */  nop
    /* 800EE56C 10410007 */  beq $v0, $at, 0x800EE58C
    /* 800EE570 00000000 */  nop
    /* 800EE574 10000007 */  beq $zero, $zero, 0x800EE594
    /* 800EE578 00000000 */  nop
    /* 800EE57C 0C001F48 */  jal 0x80007D20
    /* 800EE580 24040080 */  addiu $a0, $zero, 128
    /* 800EE584 10000003 */  beq $zero, $zero, 0x800EE594
    /* 800EE588 00000000 */  nop
    /* 800EE58C 0C001F48 */  jal 0x80007D20
