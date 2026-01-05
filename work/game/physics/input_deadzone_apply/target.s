# Source: game_code.bin (decompressed)
# Address: 0x800ADD58

glabel input_deadzone_apply
    /* 800ADD58 27BDFE70 */  addiu $sp, $sp, -400
    /* 800ADD5C AFBF0084 */  sw $ra, 132($sp)
    /* 800ADD60 AFBE0080 */  sw $fp, 128($sp)
    /* 800ADD64 AFB7007C */  sw $s7, 124($sp)
    /* 800ADD68 AFB60078 */  sw $s6, 120($sp)
    /* 800ADD6C AFB50074 */  sw $s5, 116($sp)
    /* 800ADD70 AFB40070 */  sw $s4, 112($sp)
    /* 800ADD74 AFB3006C */  sw $s3, 108($sp)
    /* 800ADD78 AFB20068 */  sw $s2, 104($sp)
    /* 800ADD7C AFB10064 */  sw $s1, 100($sp)
    /* 800ADD80 AFB00060 */  sw $s0, 96($sp)
    /* 800ADD84 F7BE0058 */  .word 0xF7BE0058
    /* 800ADD88 F7BC0050 */  .word 0xF7BC0050
    /* 800ADD8C F7BA0048 */  .word 0xF7BA0048
    /* 800ADD90 F7B80040 */  .word 0xF7B80040
    /* 800ADD94 F7B60038 */  .word 0xF7B60038
    /* 800ADD98 F7B40030 */  .word 0xF7B40030
    /* 800ADD9C AFA60198 */  sw $a2, 408($sp)
    /* 800ADDA0 AFA000B8 */  sw $zero, 184($sp)
    /* 800ADDA4 C4860000 */  lwc1 $f6, 0($a0)
    /* 800ADDA8 4480E000 */  .word 0x4480E000
    /* 800ADDAC 4487D000 */  .word 0x4487D000
    /* 800ADDB0 E7A6014C */  swc1 $f6, 332($sp)
    /* 800ADDB4 C7A8014C */  lwc1 $f8, 332($sp)
    /* 800ADDB8 C48A0004 */  lwc1 $f10, 4($a0)
    /* 800ADDBC 00A09025 */  or $s2, $a1, $zero
    /* 800ADDC0 461C403C */  .word 0x461C403C
    /* 800ADDC4 E7AA0150 */  swc1 $f10, 336($sp)
    /* 800ADDC8 C4840008 */  lwc1 $f4, 8($a0)
    /* 800ADDCC 3C013F80 */  lui $at, 0x3F80
    /* 800ADDD0 4500000F */  .word 0x4500000F
    /* 800ADDD4 E7A40154 */  swc1 $f4, 340($sp)
    /* 800ADDD8 4600418D */  .word 0x4600418D
    /* 800ADDDC 440E3000 */  .word 0x440E3000
    /* 800ADDE0 00000000 */  nop
    /* 800ADDE4 448E5000 */  .word 0x448E5000
    /* 800ADDE8 00000000 */  nop
    /* 800ADDEC 46805120 */  .word 0x46805120
    /* 800ADDF0 46082032 */  .word 0x46082032
    /* 800ADDF4 00000000 */  nop
    /* 800ADDF8 45030006 */  .word 0x45030006
    /* 800ADDFC 4481F000 */  .word 0x4481F000
    /* 800ADE00 3C013F80 */  lui $at, 0x3F80
    /* 800ADE04 4481F000 */  .word 0x4481F000
    /* 800ADE08 10000003 */  beq $zero, $zero, 0x800ADE18
    /* 800ADE0C 461E4301 */  .word 0x461E4301
    /* 800ADE10 4481F000 */  .word 0x4481F000
    /* 800ADE14 C7AC014C */  lwc1 $f12, 332($sp)
    /* 800ADE18 C7A60154 */  lwc1 $f6, 340($sp)
    /* 800ADE1C C7A00154 */  lwc1 $f0, 340($sp)
    /* 800ADE20 461C303C */  .word 0x461C303C
    /* 800ADE24 00000000 */  nop
    /* 800ADE28 4500000D */  .word 0x4500000D
    /* 800ADE2C 00000000 */  nop
    /* 800ADE30 4600328D */  .word 0x4600328D
    /* 800ADE34 440F5000 */  .word 0x440F5000
    /* 800ADE38 00000000 */  nop
    /* 800ADE3C 448F2000 */  .word 0x448F2000
    /* 800ADE40 00000000 */  nop
    /* 800ADE44 46802220 */  .word 0x46802220
    /* 800ADE48 46064032 */  .word 0x46064032
    /* 800ADE4C 00000000 */  nop
    /* 800ADE50 45010003 */  .word 0x45010003
    /* 800ADE54 00000000 */  nop
    /* 800ADE58 10000001 */  beq $zero, $zero, 0x800ADE60
    /* 800ADE5C 461E3001 */  .word 0x461E3001
    /* 800ADE60 4600628D */  .word 0x4600628D
    /* 800ADE64 3C048012 */  lui $a0, 0x8012
    /* 800ADE68 8C844EEC */  lw $a0, 20204($a0)
    /* 800ADE6C 4600010D */  .word 0x4600010D
    /* 800ADE70 44055000 */  .word 0x44055000
    /* 800ADE74 27A70166 */  addiu $a3, $sp, 358
    /* 800ADE78 44062000 */  .word 0x44062000
    /* 800ADE7C 00057400 */  sll $t6, $a1, 16
    /* 800ADE80 000E2C03 */  sra $a1, $t6, 16
    /* 800ADE84 0006C400 */  sll $t8, $a2, 16
    /* 800ADE88 0C02B2A7 */  jal 0x800ACA9C
    /* 800ADE8C 00183403 */  sra $a2, $t8, 16
    /* 800ADE90 14400003 */  bne $v0, $zero, 0x800ADEA0
    /* 800ADE94 AFA200C4 */  sw $v0, 196($sp)
    /* 800ADE98 1000031C */  beq $zero, $zero, 0x800AEB0C
    /* 800ADE9C 00001025 */  or $v0, $zero, $zero
    /* 800ADEA0 C6400000 */  lwc1 $f0, 0($s2)
    /* 800ADEA4 461C003C */  .word 0x461C003C
    /* 800ADEA8 00000000 */  nop
    /* 800ADEAC 4502000E */  .word 0x4502000E
    /* 800ADEB0 46000306 */  .word 0x46000306
    /* 800ADEB4 4600020D */  .word 0x4600020D
    /* 800ADEB8 440F4000 */  .word 0x440F4000
    /* 800ADEBC 00000000 */  nop
    /* 800ADEC0 448F3000 */  .word 0x448F3000
    /* 800ADEC4 00000000 */  nop
    /* 800ADEC8 468032A0 */  .word 0x468032A0
    /* 800ADECC 46005032 */  .word 0x46005032
    /* 800ADED0 00000000 */  nop
    /* 800ADED4 45030004 */  .word 0x45030004
    /* 800ADED8 46000306 */  .word 0x46000306
    /* 800ADEDC 10000002 */  beq $zero, $zero, 0x800ADEE8
    /* 800ADEE0 461E0301 */  .word 0x461E0301
    /* 800ADEE4 46000306 */  .word 0x46000306
    /* 800ADEE8 C6420008 */  lwc1 $f2, 8($s2)
    /* 800ADEEC 4600628D */  .word 0x4600628D
    /* 800ADEF0 461C103C */  .word 0x461C103C
    /* 800ADEF4 44055000 */  .word 0x44055000
    /* 800ADEF8 4502000E */  .word 0x4502000E
    /* 800ADEFC 46001006 */  .word 0x46001006
    /* 800ADF00 4600110D */  .word 0x4600110D
    /* 800ADF04 440E2000 */  .word 0x440E2000
    /* 800ADF08 00000000 */  nop
    /* 800ADF0C 448E4000 */  .word 0x448E4000
    /* 800ADF10 00000000 */  nop
    /* 800ADF14 468041A0 */  .word 0x468041A0
    /* 800ADF18 46023032 */  .word 0x46023032
    /* 800ADF1C 00000000 */  nop
    /* 800ADF20 45030004 */  .word 0x45030004
    /* 800ADF24 46001006 */  .word 0x46001006
    /* 800ADF28 10000002 */  beq $zero, $zero, 0x800ADF34
    /* 800ADF2C 461E1001 */  .word 0x461E1001
    /* 800ADF30 46001006 */  .word 0x46001006
    /* 800ADF34 4600010D */  .word 0x4600010D
    /* 800ADF38 3C048012 */  lui $a0, 0x8012
    /* 800ADF3C 00057C00 */  sll $t7, $a1, 16
    /* 800ADF40 000F2C03 */  sra $a1, $t7, 16
    /* 800ADF44 44062000 */  .word 0x44062000
    /* 800ADF48 8C844EEC */  lw $a0, 20204($a0)
    /* 800ADF4C 27A70164 */  addiu $a3, $sp, 356
    /* 800ADF50 0006CC00 */  sll $t9, $a2, 16
    /* 800ADF54 0C02B2A7 */  jal 0x800ACA9C
    /* 800ADF58 00193403 */  sra $a2, $t9, 16
    /* 800ADF5C 87B80166 */  lh $t8, 358($sp)
    /* 800ADF60 8FAD00C4 */  lw $t5, 196($sp)
    /* 800ADF64 240F0010 */  addiu $t7, $zero, 16
    /* 800ADF68 0018C840 */  sll $t9, $t8, 1
    /* 800ADF6C AFA200C0 */  sw $v0, 192($sp)
    /* 800ADF70 030F6004 */  .word 0x030F6004
    /* 800ADF74 33060001 */  andi $a2, $t8, 0x0001
    /* 800ADF78 33090002 */  andi $t1, $t8, 0x0002
    /* 800ADF7C 01B95821 */  addu $t3, $t5, $t9
    /* 800ADF80 85A40004 */  lh $a0, 4($t5)
    /* 800ADF84 85A50006 */  lh $a1, 6($t5)
    /* 800ADF88 3C010001 */  lui $at, 0x0001
    /* 800ADF8C 00851021 */  addu $v0, $a0, $a1
    /* 800ADF90 0002CC00 */  sll $t9, $v0, 16
    /* 800ADF94 00197C03 */  sra $t7, $t9, 16
    /* 800ADF98 05E10004 */  bgez $t7, 0x800ADFAC
    /* 800ADF9C 01E01025 */  or $v0, $t7, $zero
    /* 800ADFA0 25E20001 */  addiu $v0, $t7, 1
    /* 800ADFA4 0002C400 */  sll $t8, $v0, 16
    /* 800ADFA8 00181403 */  sra $v0, $t8, 16
    /* 800ADFAC 85A70008 */  lh $a3, 8($t5)
    /* 800ADFB0 85A8000A */  lh $t0, 10($t5)
    /* 800ADFB4 00055400 */  sll $t2, $a1, 16
    /* 800ADFB8 00E81821 */  addu $v1, $a3, $t0
    /* 800ADFBC 0003CC00 */  sll $t9, $v1, 16
    /* 800ADFC0 00197C03 */  sra $t7, $t9, 16
    /* 800ADFC4 05E10004 */  bgez $t7, 0x800ADFD8
    /* 800ADFC8 01E01825 */  or $v1, $t7, $zero
    /* 800ADFCC 25E30001 */  addiu $v1, $t7, 1
    /* 800ADFD0 0003C400 */  sll $t8, $v1, 16
    /* 800ADFD4 00181C03 */  sra $v1, $t8, 16
    /* 800ADFD8 0002C843 */  sra $t9, $v0, 1
    /* 800ADFDC 00197C00 */  sll $t7, $t9, 16
    /* 800ADFE0 00037043 */  sra $t6, $v1, 1
    /* 800ADFE4 000FC403 */  sra $t8, $t7, 16
    /* 800ADFE8 000ECC00 */  sll $t9, $t6, 16
    /* 800ADFEC 03001025 */  or $v0, $t8, $zero
    /* 800ADFF0 10C00003 */  beq $a2, $zero, 0x800AE000
    /* 800ADFF4 00191C03 */  sra $v1, $t9, 16
    /* 800ADFF8 10000002 */  beq $zero, $zero, 0x800AE004
    /* 800ADFFC A7B800D6 */  sh $t8, 214($sp)
    /* 800AE000 A7A400D6 */  sh $a0, 214($sp)
    /* 800AE004 10C00003 */  beq $a2, $zero, 0x800AE014
    /* 800AE008 000AC403 */  sra $t8, $t2, 16
    /* 800AE00C 10000003 */  beq $zero, $zero, 0x800AE01C
    /* 800AE010 03005025 */  or $t2, $t8, $zero
    /* 800AE014 A7A200D4 */  sh $v0, 212($sp)
    /* 800AE018 87AA00D4 */  lh $t2, 212($sp)
    /* 800AE01C 11200003 */  beq $t1, $zero, 0x800AE02C
    /* 800AE020 27A600DC */  addiu $a2, $sp, 220
    /* 800AE024 10000002 */  beq $zero, $zero, 0x800AE030
    /* 800AE028 A7A700D2 */  sh $a3, 210($sp)
    /* 800AE02C A7A300D2 */  sh $v1, 210($sp)
    /* 800AE030 51200006 */  .word 0x51200006
    /* 800AE034 A7A800D0 */  sh $t0, 208($sp)
    /* 800AE038 00034400 */  sll $t0, $v1, 16
    /* 800AE03C 00087403 */  sra $t6, $t0, 16
    /* 800AE040 10000003 */  beq $zero, $zero, 0x800AE050
    /* 800AE044 01C04025 */  or $t0, $t6, $zero
    /* 800AE048 A7A800D0 */  sh $t0, 208($sp)
    /* 800AE04C 87A800D0 */  lh $t0, 208($sp)
    /* 800AE050 91B90003 */  lbu $t9, 3($t5)
    /* 800AE054 9562000C */  lhu $v0, 12($t3)
    /* 800AE058 3C0E8015 */  lui $t6, 0x8015
    /* 800AE05C 032C7824 */  and $t7, $t9, $t4
    /* 800AE060 11E00002 */  beq $t7, $zero, 0x800AE06C
    /* 800AE064 0041C025 */  or $t8, $v0, $at
    /* 800AE068 03001025 */  or $v0, $t8, $zero
    /* 800AE06C A7A800D0 */  sh $t0, 208($sp)
    /* 800AE070 A7AA00D4 */  sh $t2, 212($sp)
    /* 800AE074 10400060 */  beq $v0, $zero, 0x800AE1F8
    /* 800AE078 AFAD00C4 */  sw $t5, 196($sp)
    /* 800AE07C 8DCE2460 */  lw $t6, 9312($t6)
    /* 800AE080 2407FFFF */  addiu $a3, $zero, -1
    /* 800AE084 00001825 */  or $v1, $zero, $zero
    /* 800AE088 01C22021 */  addu $a0, $t6, $v0
    /* 800AE08C 90850000 */  lbu $a1, 0($a0)
    /* 800AE090 24840001 */  addiu $a0, $a0, 1
    /* 800AE094 AFA00168 */  sw $zero, 360($sp)
    /* 800AE098 0C02B738 */  jal 0x800ADCE0
    /* 800AE09C 00A08025 */  or $s0, $a1, $zero
    /* 800AE0A0 8FA30168 */  lw $v1, 360($sp)
    /* 800AE0A4 12000054 */  beq $s0, $zero, 0x800AE1F8
    /* 800AE0A8 AFB000A8 */  sw $s0, 168($sp)
    /* 800AE0AC 27B400DC */  addiu $s4, $sp, 220
    /* 800AE0B0 968F0000 */  lhu $t7, 0($s4)
    /* 800AE0B4 3C0E8015 */  lui $t6, 0x8015
    /* 800AE0B8 8DCE97F8 */  lw $t6, -26632($t6)
    /* 800AE0BC 000FC080 */  sll $t8, $t7, 2
    /* 800AE0C0 030FC023 */  subu $t8, $t8, $t7
    /* 800AE0C4 0018C0C0 */  sll $t8, $t8, 3
    /* 800AE0C8 030E8021 */  addu $s0, $t8, $t6
    /* 800AE0CC 96020000 */  lhu $v0, 0($s0)
    /* 800AE0D0 2401000F */  addiu $at, $zero, 15
    /* 800AE0D4 8FAF01A4 */  lw $t7, 420($sp)
    /* 800AE0D8 3059000F */  andi $t9, $v0, 0x000F
    /* 800AE0DC 53210042 */  .word 0x53210042
    /* 800AE0E0 8FAE00A8 */  lw $t6, 168($sp)
    /* 800AE0E4 11F9003F */  beq $t7, $t9, 0x800AE1E4
    /* 800AE0E8 02003825 */  or $a3, $s0, $zero
    /* 800AE0EC 461AD602 */  .word 0x461AD602
    /* 800AE0F0 27B1014C */  addiu $s1, $sp, 332
    /* 800AE0F4 27B30134 */  addiu $s3, $sp, 308
    /* 800AE0F8 27B70128 */  addiu $s7, $sp, 296
    /* 800AE0FC 27BE016C */  addiu $fp, $sp, 364
    /* 800AE100 0000B025 */  or $s6, $zero, $zero
    /* 800AE104 27B50124 */  addiu $s5, $sp, 292
    /* 800AE108 AFA30168 */  sw $v1, 360($sp)
    /* 800AE10C AFB000BC */  sw $s0, 188($sp)
    /* 800AE110 AFB20194 */  sw $s2, 404($sp)
    /* 800AE114 0C02B5CD */  jal 0x800AD734
    /* 800AE118 AFB400AC */  sw $s4, 172($sp)
    /* 800AE11C 8FA30168 */  lw $v1, 360($sp)
    /* 800AE120 8FB000BC */  lw $s0, 188($sp)
    /* 800AE124 8FB20194 */  lw $s2, 404($sp)
    /* 800AE128 1040002E */  beq $v0, $zero, 0x800AE1E4
    /* 800AE12C 8FB400AC */  lw $s4, 172($sp)
    /* 800AE130 87B800D6 */  lh $t8, 214($sp)
    /* 800AE134 C7A80134 */  lwc1 $f8, 308($sp)
    /* 800AE138 87AE00D4 */  lh $t6, 212($sp)
    /* 800AE13C 44983000 */  .word 0x44983000
    /* 800AE140 00000000 */  nop
    /* 800AE144 468032A0 */  .word 0x468032A0
    /* 800AE148 460A403C */  .word 0x460A403C
    /* 800AE14C 00000000 */  nop
    /* 800AE150 45030025 */  .word 0x45030025
    /* 800AE154 8FAE00A8 */  lw $t6, 168($sp)
    /* 800AE158 448E2000 */  .word 0x448E2000
    /* 800AE15C 87B900D2 */  lh $t9, 210($sp)
    /* 800AE160 468021A0 */  .word 0x468021A0
    /* 800AE164 4608303E */  .word 0x4608303E
    /* 800AE168 00000000 */  nop
    /* 800AE16C 4503001E */  .word 0x4503001E
    /* 800AE170 8FAE00A8 */  lw $t6, 168($sp)
    /* 800AE174 44992000 */  .word 0x44992000
    /* 800AE178 C7AA013C */  lwc1 $f10, 316($sp)
    /* 800AE17C 87AF00D0 */  lh $t7, 208($sp)
    /* 800AE180 46802220 */  .word 0x46802220
    /* 800AE184 4608503C */  .word 0x4608503C
    /* 800AE188 00000000 */  nop
    /* 800AE18C 45030016 */  .word 0x45030016
    /* 800AE190 8FAE00A8 */  lw $t6, 168($sp)
    /* 800AE194 448F3000 */  .word 0x448F3000
    /* 800AE198 C7A80134 */  lwc1 $f8, 308($sp)
    /* 800AE19C 46803120 */  .word 0x46803120
    /* 800AE1A0 460A203E */  .word 0x460A203E
    /* 800AE1A4 00000000 */  nop
    /* 800AE1A8 4503000F */  .word 0x4503000F
    /* 800AE1AC 8FAE00A8 */  lw $t6, 168($sp)
    /* 800AE1B0 E6480000 */  swc1 $f8, 0($s2)
    /* 800AE1B4 C7A60138 */  lwc1 $f6, 312($sp)
    /* 800AE1B8 27A4016C */  addiu $a0, $sp, 364
    /* 800AE1BC E6460004 */  swc1 $f6, 4($s2)
    /* 800AE1C0 C7AA013C */  lwc1 $f10, 316($sp)
    /* 800AE1C4 E64A0008 */  swc1 $f10, 8($s2)
    /* 800AE1C8 AFA30168 */  sw $v1, 360($sp)
    /* 800AE1CC 0C0235AC */  jal 0x8008D6B0
    /* 800AE1D0 8FA50198 */  lw $a1, 408($sp)
    /* 800AE1D4 97B80124 */  lhu $t8, 292($sp)
    /* 800AE1D8 8FA30168 */  lw $v1, 360($sp)
    /* 800AE1DC AFB000B8 */  sw $s0, 184($sp)
    /* 800AE1E0 A7B80122 */  sh $t8, 290($sp)
    /* 800AE1E4 8FAE00A8 */  lw $t6, 168($sp)
    /* 800AE1E8 24630001 */  addiu $v1, $v1, 1
    /* 800AE1EC 26940002 */  addiu $s4, $s4, 2
    /* 800AE1F0 546EFFB0 */  .word 0x546EFFB0
    /* 800AE1F4 968F0000 */  lhu $t7, 0($s4)
    /* 800AE1F8 8FB900B8 */  lw $t9, 184($sp)
    /* 800AE1FC 87A800D0 */  lh $t0, 208($sp)
    /* 800AE200 87AA00D4 */  lh $t2, 212($sp)
    /* 800AE204 13200026 */  beq $t9, $zero, 0x800AE2A0
    /* 800AE208 8FAD00C4 */  lw $t5, 196($sp)
    /* 800AE20C 8FAF01A0 */  lw $t7, 416($sp)
    /* 800AE210 11E00021 */  beq $t7, $zero, 0x800AE298
    /* 800AE214 00000000 */  nop
    /* 800AE218 97220000 */  lhu $v0, 0($t9)
    /* 800AE21C 03202025 */  or $a0, $t9, $zero
    /* 800AE220 97A50122 */  lhu $a1, 290($sp)
    /* 800AE224 30582000 */  andi $t8, $v0, 0x2000
    /* 800AE228 1300000B */  beq $t8, $zero, 0x800AE258
    /* 800AE22C 304F1000 */  andi $t7, $v0, 0x1000
    /* 800AE230 3C01437A */  lui $at, 0x437A
    /* 800AE234 44812000 */  .word 0x44812000
    /* 800AE238 8FAE0198 */  lw $t6, 408($sp)
    /* 800AE23C 02403025 */  or $a2, $s2, $zero
    /* 800AE240 27A70128 */  addiu $a3, $sp, 296
    /* 800AE244 AFAE0010 */  sw $t6, 16($sp)
    /* 800AE248 0C02B44A */  jal 0x800AD128
    /* 800AE24C E7A40014 */  swc1 $f4, 20($sp)
    /* 800AE250 10000011 */  beq $zero, $zero, 0x800AE298
    /* 800AE254 00000000 */  nop
    /* 800AE258 11E0000F */  beq $t7, $zero, 0x800AE298
    /* 800AE25C 8FA400B8 */  lw $a0, 184($sp)
    /* 800AE260 8FB80198 */  lw $t8, 408($sp)
    /* 800AE264 97A50122 */  lhu $a1, 290($sp)
    /* 800AE268 02403025 */  or $a2, $s2, $zero
    /* 800AE26C 27A70128 */  addiu $a3, $sp, 296
    /* 800AE270 0C02B306 */  jal 0x800ACC18
    /* 800AE274 AFB80010 */  sw $t8, 16($sp)
    /* 800AE278 0C02382E */  jal 0x8008E0B8
    /* 800AE27C 8FA40198 */  lw $a0, 408($sp)
    /* 800AE280 8FA40198 */  lw $a0, 408($sp)
    /* 800AE284 0C02382E */  jal 0x8008E0B8
    /* 800AE288 2484000C */  addiu $a0, $a0, 12
    /* 800AE28C 8FA40198 */  lw $a0, 408($sp)
    /* 800AE290 0C02382E */  jal 0x8008E0B8
    /* 800AE294 24840018 */  addiu $a0, $a0, 24
    /* 800AE298 1000021C */  beq $zero, $zero, 0x800AEB0C
    /* 800AE29C 8FA200B8 */  lw $v0, 184($sp)
    /* 800AE2A0 8FB900C0 */  lw $t9, 192($sp)
    /* 800AE2A4 87AE0166 */  lh $t6, 358($sp)
    /* 800AE2A8 87AF0164 */  lh $t7, 356($sp)
    /* 800AE2AC 15B90005 */  bne $t5, $t9, 0x800AE2C4
    /* 800AE2B0 C7A6014C */  lwc1 $f6, 332($sp)
    /* 800AE2B4 55CF0004 */  .word 0x55CF0004
    /* 800AE2B8 C6480000 */  lwc1 $f8, 0($s2)
    /* 800AE2BC 10000213 */  beq $zero, $zero, 0x800AEB0C
    /* 800AE2C0 00001025 */  or $v0, $zero, $zero
    /* 800AE2C4 C6480000 */  lwc1 $f8, 0($s2)
    /* 800AE2C8 87B800D6 */  lh $t8, 214($sp)
    /* 800AE2CC 46064281 */  .word 0x46064281
    /* 800AE2D0 C7A80150 */  lwc1 $f8, 336($sp)
    /* 800AE2D4 E7AA0140 */  swc1 $f10, 320($sp)
    /* 800AE2D8 C6440004 */  lwc1 $f4, 4($s2)
    /* 800AE2DC 46082281 */  .word 0x46082281
    /* 800AE2E0 E7AA0144 */  swc1 $f10, 324($sp)
    /* 800AE2E4 C6440008 */  lwc1 $f4, 8($s2)
    /* 800AE2E8 C7AA0154 */  lwc1 $f10, 340($sp)
    /* 800AE2EC 460A2101 */  .word 0x460A2101
    /* 800AE2F0 E7A40148 */  swc1 $f4, 328($sp)
    /* 800AE2F4 C7A40140 */  lwc1 $f4, 320($sp)
    /* 800AE2F8 461C203C */  .word 0x461C203C
    /* 800AE2FC 00000000 */  nop
    /* 800AE300 4502007B */  .word 0x4502007B
    /* 800AE304 C7A80140 */  lwc1 $f8, 320($sp)
    /* 800AE308 E7A60088 */  swc1 $f6, 136($sp)
    /* 800AE30C 44983000 */  .word 0x44983000
    /* 800AE310 E7A8008C */  swc1 $f8, 140($sp)
    /* 800AE314 C7A80088 */  lwc1 $f8, 136($sp)
    /* 800AE318 468031A0 */  .word 0x468031A0
    /* 800AE31C 87B900D2 */  lh $t9, 210($sp)
    /* 800AE320 E7A60134 */  swc1 $f6, 308($sp)
    /* 800AE324 46083181 */  .word 0x46083181
    /* 800AE328 C7A80144 */  lwc1 $f8, 324($sp)
    /* 800AE32C 46043003 */  .word 0x46043003
    /* 800AE330 C7A4008C */  lwc1 $f4, 140($sp)
    /* 800AE334 46004182 */  .word 0x46004182
    /* 800AE338 46043200 */  .word 0x46043200
    /* 800AE33C C7A60148 */  lwc1 $f6, 328($sp)
    /* 800AE340 46003102 */  .word 0x46003102
    /* 800AE344 E7A80138 */  swc1 $f8, 312($sp)
    /* 800AE348 460A2200 */  .word 0x460A2200
    /* 800AE34C 44992000 */  .word 0x44992000
    /* 800AE350 00000000 */  nop
    /* 800AE354 468020A0 */  .word 0x468020A0
    /* 800AE358 E7A8013C */  swc1 $f8, 316($sp)
    /* 800AE35C 46081032 */  .word 0x46081032
    /* 800AE360 00000000 */  nop
    /* 800AE364 45020017 */  .word 0x45020017
    /* 800AE368 44883000 */  .word 0x44883000
    /* 800AE36C 461C303C */  .word 0x461C303C
    /* 800AE370 2705FFFF */  addiu $a1, $t8, -1
    /* 800AE374 00057400 */  sll $t6, $a1, 16
    /* 800AE378 000E2C03 */  sra $a1, $t6, 16
    /* 800AE37C 45000010 */  .word 0x45000010
    /* 800AE380 3C048012 */  lui $a0, 0x8012
    /* 800AE384 87A600D2 */  lh $a2, 210($sp)
    /* 800AE388 8C844EEC */  lw $a0, 20204($a0)
    /* 800AE38C 27A70166 */  addiu $a3, $sp, 358
    /* 800AE390 24C6FFFF */  addiu $a2, $a2, -1
    /* 800AE394 0006CC00 */  sll $t9, $a2, 16
    /* 800AE398 0C02B2A7 */  jal 0x800ACA9C
    /* 800AE39C 00193403 */  sra $a2, $t9, 16
    /* 800AE3A0 C7AA0134 */  lwc1 $f10, 308($sp)
    /* 800AE3A4 C7A40138 */  lwc1 $f4, 312($sp)
    /* 800AE3A8 C7A8013C */  lwc1 $f8, 316($sp)
    /* 800AE3AC 00406825 */  or $t5, $v0, $zero
    /* 800AE3B0 E7AA014C */  swc1 $f10, 332($sp)
    /* 800AE3B4 E7A40150 */  swc1 $f4, 336($sp)
    /* 800AE3B8 100001C9 */  beq $zero, $zero, 0x800AEAE0
    /* 800AE3BC E7A80154 */  swc1 $f8, 340($sp)
    /* 800AE3C0 44883000 */  .word 0x44883000
    /* 800AE3C4 C7AA013C */  lwc1 $f10, 316($sp)
    /* 800AE3C8 C7A40148 */  lwc1 $f4, 328($sp)
    /* 800AE3CC 46803020 */  .word 0x46803020
    /* 800AE3D0 460A0032 */  .word 0x460A0032
    /* 800AE3D4 00000000 */  nop
    /* 800AE3D8 45020018 */  .word 0x45020018
    /* 800AE3DC C7A4013C */  lwc1 $f4, 316($sp)
    /* 800AE3E0 461C203C */  .word 0x461C203C
    /* 800AE3E4 87A500D6 */  lh $a1, 214($sp)
    /* 800AE3E8 3C048012 */  lui $a0, 0x8012
    /* 800AE3EC 2506FFFF */  addiu $a2, $t0, -1
    /* 800AE3F0 45000011 */  .word 0x45000011
    /* 800AE3F4 24A5FFFF */  addiu $a1, $a1, -1
    /* 800AE3F8 00057400 */  sll $t6, $a1, 16
    /* 800AE3FC 0006CC00 */  sll $t9, $a2, 16
    /* 800AE400 00193403 */  sra $a2, $t9, 16
    /* 800AE404 000E2C03 */  sra $a1, $t6, 16
    /* 800AE408 8C844EEC */  lw $a0, 20204($a0)
    /* 800AE40C 0C02B2A7 */  jal 0x800ACA9C
    /* 800AE410 27A70166 */  addiu $a3, $sp, 358
    /* 800AE414 C7A80134 */  lwc1 $f8, 308($sp)
    /* 800AE418 C7A60138 */  lwc1 $f6, 312($sp)
    /* 800AE41C C7AA013C */  lwc1 $f10, 316($sp)
    /* 800AE420 AFA200C4 */  sw $v0, 196($sp)
    /* 800AE424 00406825 */  or $t5, $v0, $zero
    /* 800AE428 E7A8014C */  swc1 $f8, 332($sp)
    /* 800AE42C E7A60150 */  swc1 $f6, 336($sp)
    /* 800AE430 100001AB */  beq $zero, $zero, 0x800AEAE0
    /* 800AE434 E7AA0154 */  swc1 $f10, 340($sp)
    /* 800AE438 C7A4013C */  lwc1 $f4, 316($sp)
    /* 800AE43C 4604103E */  .word 0x4604103E
    /* 800AE440 00000000 */  nop
    /* 800AE444 450200A6 */  .word 0x450200A6
    /* 800AE448 C7A80148 */  lwc1 $f8, 328($sp)
    /* 800AE44C 4600203E */  .word 0x4600203E
    /* 800AE450 3C048012 */  lui $a0, 0x8012
    /* 800AE454 87A500D6 */  lh $a1, 214($sp)
    /* 800AE458 450200A1 */  .word 0x450200A1
    /* 800AE45C C7A80148 */  lwc1 $f8, 328($sp)
    /* 800AE460 461C203C */  .word 0x461C203C
    /* 800AE464 C7A0013C */  lwc1 $f0, 316($sp)
    /* 800AE468 4500000D */  .word 0x4500000D
    /* 800AE46C 00000000 */  nop
    /* 800AE470 4600220D */  .word 0x4600220D
    /* 800AE474 440F4000 */  .word 0x440F4000
    /* 800AE478 00000000 */  nop
    /* 800AE47C 448F3000 */  .word 0x448F3000
    /* 800AE480 00000000 */  nop
    /* 800AE484 468032A0 */  .word 0x468032A0
    /* 800AE488 46045032 */  .word 0x46045032
    /* 800AE48C 00000000 */  nop
    /* 800AE490 45010003 */  .word 0x45010003
    /* 800AE494 00000000 */  nop
    /* 800AE498 10000001 */  beq $zero, $zero, 0x800AE4A0
    /* 800AE49C 461E2001 */  .word 0x461E2001
    /* 800AE4A0 4600020D */  .word 0x4600020D
    /* 800AE4A4 24A5FFFF */  addiu $a1, $a1, -1
    /* 800AE4A8 0005CC00 */  sll $t9, $a1, 16
    /* 800AE4AC 00192C03 */  sra $a1, $t9, 16
    /* 800AE4B0 44064000 */  .word 0x44064000
    /* 800AE4B4 8C844EEC */  lw $a0, 20204($a0)
    /* 800AE4B8 27A70166 */  addiu $a3, $sp, 358
    /* 800AE4BC 00067C00 */  sll $t7, $a2, 16
    /* 800AE4C0 0C02B2A7 */  jal 0x800ACA9C
    /* 800AE4C4 000F3403 */  sra $a2, $t7, 16
    /* 800AE4C8 C7A60134 */  lwc1 $f6, 308($sp)
    /* 800AE4CC C7AA0138 */  lwc1 $f10, 312($sp)
    /* 800AE4D0 C7A4013C */  lwc1 $f4, 316($sp)
    /* 800AE4D4 AFA200C4 */  sw $v0, 196($sp)
    /* 800AE4D8 00406825 */  or $t5, $v0, $zero
    /* 800AE4DC E7A6014C */  swc1 $f6, 332($sp)
    /* 800AE4E0 E7AA0150 */  swc1 $f10, 336($sp)
    /* 800AE4E4 1000017E */  beq $zero, $zero, 0x800AEAE0
    /* 800AE4E8 E7A40154 */  swc1 $f4, 340($sp)
    /* 800AE4EC C7A80140 */  lwc1 $f8, 320($sp)
    /* 800AE4F0 4608E03C */  .word 0x4608E03C
    /* 800AE4F4 00000000 */  nop
    /* 800AE4F8 45020079 */  .word 0x45020079
    /* 800AE4FC C7A80148 */  lwc1 $f8, 328($sp)
    /* 800AE500 448A3000 */  .word 0x448A3000
    /* 800AE504 87B800D2 */  lh $t8, 210($sp)
    /* 800AE508 468032A0 */  .word 0x468032A0
    /* 800AE50C C7A6014C */  lwc1 $f6, 332($sp)
    /* 800AE510 E7AA0134 */  swc1 $f10, 308($sp)
    /* 800AE514 C7A40134 */  lwc1 $f4, 308($sp)
    /* 800AE518 46062281 */  .word 0x46062281
    /* 800AE51C C7A40144 */  lwc1 $f4, 324($sp)
    /* 800AE520 46085003 */  .word 0x46085003
    /* 800AE524 C7AA0150 */  lwc1 $f10, 336($sp)
