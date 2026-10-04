nonmatching func_80019F48, 0x328

glabel func_80019F48
    /* 1AB48 80019F48 27BDFF50 */  addiu      $sp, $sp, -0xB0
    /* 1AB4C 80019F4C AFA400B0 */  sw         $a0, 0xB0($sp)
    /* 1AB50 80019F50 AFBF005C */  sw         $ra, 0x5C($sp)
    /* 1AB54 80019F54 AFA500B4 */  sw         $a1, 0xB4($sp)
    /* 1AB58 80019F58 240EFFFF */  addiu      $t6, $zero, -0x1
    /* 1AB5C 80019F5C 00042402 */  srl        $a0, $a0, 16
    /* 1AB60 80019F60 AFBE0058 */  sw         $fp, 0x58($sp)
    /* 1AB64 80019F64 AFB70054 */  sw         $s7, 0x54($sp)
    /* 1AB68 80019F68 AFB60050 */  sw         $s6, 0x50($sp)
    /* 1AB6C 80019F6C AFB5004C */  sw         $s5, 0x4C($sp)
    /* 1AB70 80019F70 AFB40048 */  sw         $s4, 0x48($sp)
    /* 1AB74 80019F74 AFB30044 */  sw         $s3, 0x44($sp)
    /* 1AB78 80019F78 AFB20040 */  sw         $s2, 0x40($sp)
    /* 1AB7C 80019F7C AFB1003C */  sw         $s1, 0x3C($sp)
    /* 1AB80 80019F80 AFB00038 */  sw         $s0, 0x38($sp)
    /* 1AB84 80019F84 AFA600B8 */  sw         $a2, 0xB8($sp)
    /* 1AB88 80019F88 AFA700BC */  sw         $a3, 0xBC($sp)
    /* 1AB8C 80019F8C AFAE00A8 */  sw         $t6, 0xA8($sp)
    /* 1AB90 80019F90 3084FFFF */  andi       $a0, $a0, 0xFFFF
    /* 1AB94 80019F94 0C005BE0 */  jal        func_80016F80
    /* 1AB98 80019F98 27A500AE */   addiu     $a1, $sp, 0xAE
    /* 1AB9C 80019F9C 104000A7 */  beqz       $v0, .L8001A23C
    /* 1ABA0 80019FA0 97AF00AE */   lhu       $t7, 0xAE($sp)
    /* 1ABA4 80019FA4 19E000A5 */  blez       $t7, .L8001A23C
    /* 1ABA8 80019FA8 0000A025 */   or        $s4, $zero, $zero
    /* 1ABAC 80019FAC 3C178005 */  lui        $s7, %hi(D_8004BEB8)
    /* 1ABB0 80019FB0 26F7BEB8 */  addiu      $s7, $s7, %lo(D_8004BEB8)
    /* 1ABB4 80019FB4 00408825 */  or         $s1, $v0, $zero
    /* 1ABB8 80019FB8 241E01A0 */  addiu      $fp, $zero, 0x1A0
    /* 1ABBC 80019FBC 93B600CB */  lbu        $s6, 0xCB($sp)
    /* 1ABC0 80019FC0 93B500C7 */  lbu        $s5, 0xC7($sp)
    /* 1ABC4 80019FC4 2413FFFF */  addiu      $s3, $zero, -0x1
    /* 1ABC8 80019FC8 8FB200A4 */  lw         $s2, 0xA4($sp)
    /* 1ABCC 80019FCC 92210000 */  lbu        $at, 0x0($s1)
  .L80019FD0:
    /* 1ABD0 80019FD0 92380001 */  lbu        $t8, 0x1($s1)
    /* 1ABD4 80019FD4 93A900BB */  lbu        $t1, 0xBB($sp)
    /* 1ABD8 80019FD8 00010A00 */  sll        $at, $at, 8
    /* 1ABDC 80019FDC 0301C025 */  or         $t8, $t8, $at
    /* 1ABE0 80019FE0 3401FFFF */  ori        $at, $zero, 0xFFFF
    /* 1ABE4 80019FE4 5301008F */  beql       $t8, $at, .L8001A224
    /* 1ABE8 80019FE8 97AB00AE */   lhu       $t3, 0xAE($sp)
    /* 1ABEC 80019FEC 92390002 */  lbu        $t9, 0x2($s1)
    /* 1ABF0 80019FF0 3122007F */  andi       $v0, $t1, 0x7F
    /* 1ABF4 80019FF4 0059082A */  slt        $at, $v0, $t9
    /* 1ABF8 80019FF8 5420008A */  bnel       $at, $zero, .L8001A224
    /* 1ABFC 80019FFC 97AB00AE */   lhu       $t3, 0xAE($sp)
    /* 1AC00 8001A000 922A0003 */  lbu        $t2, 0x3($s1)
    /* 1AC04 8001A004 32A500FF */  andi       $a1, $s5, 0xFF
    /* 1AC08 8001A008 32C600FF */  andi       $a2, $s6, 0xFF
    /* 1AC0C 8001A00C 0142082A */  slt        $at, $t2, $v0
    /* 1AC10 8001A010 54200084 */  bnel       $at, $zero, .L8001A224
    /* 1AC14 8001A014 97AB00AE */   lhu       $t3, 0xAE($sp)
    /* 1AC18 8001A018 822B0004 */  lb         $t3, 0x4($s1)
    /* 1AC1C 8001A01C 01628021 */  addu       $s0, $t3, $v0
    /* 1AC20 8001A020 2A010080 */  slti       $at, $s0, 0x80
    /* 1AC24 8001A024 14200003 */  bnez       $at, .L8001A034
    /* 1AC28 8001A028 00000000 */   nop
    /* 1AC2C 8001A02C 10000006 */  b          .L8001A048
    /* 1AC30 8001A030 2410007F */   addiu     $s0, $zero, 0x7F
  .L8001A034:
    /* 1AC34 8001A034 06010003 */  bgez       $s0, .L8001A044
    /* 1AC38 8001A038 02001025 */   or        $v0, $s0, $zero
    /* 1AC3C 8001A03C 10000001 */  b          .L8001A044
    /* 1AC40 8001A040 00001025 */   or        $v0, $zero, $zero
  .L8001A044:
    /* 1AC44 8001A044 00408025 */  or         $s0, $v0, $zero
  .L8001A048:
    /* 1AC48 8001A048 320400FF */  andi       $a0, $s0, 0xFF
    /* 1AC4C 8001A04C 0C0067B4 */  jal        func_80019ED0
    /* 1AC50 8001A050 AFA90074 */   sw        $t1, 0x74($sp)
    /* 1AC54 8001A054 10530003 */  beq        $v0, $s3, .L8001A064
    /* 1AC58 8001A058 8FA90074 */   lw        $t1, 0x74($sp)
    /* 1AC5C 8001A05C 10000079 */  b          .L8001A244
    /* 1AC60 8001A060 8FBF005C */   lw        $ra, 0x5C($sp)
  .L8001A064:
    /* 1AC64 8001A064 8FA300B0 */  lw         $v1, 0xB0($sp)
    /* 1AC68 8001A068 92220008 */  lbu        $v0, 0x8($s1)
    /* 1AC6C 8001A06C 93A400BF */  lbu        $a0, 0xBF($sp)
    /* 1AC70 8001A070 00032A02 */  srl        $a1, $v1, 8
    /* 1AC74 8001A074 304C0080 */  andi       $t4, $v0, 0x80
    /* 1AC78 8001A078 30A500FF */  andi       $a1, $a1, 0xFF
    /* 1AC7C 8001A07C 1580000E */  bnez       $t4, .L8001A0B8
    /* 1AC80 8001A080 306600FF */   andi      $a2, $v1, 0xFF
    /* 1AC84 8001A084 93AD00C3 */  lbu        $t5, 0xC3($sp)
    /* 1AC88 8001A088 2448FFC0 */  addiu      $t0, $v0, -0x40
    /* 1AC8C 8001A08C 010D4021 */  addu       $t0, $t0, $t5
    /* 1AC90 8001A090 05010003 */  bgez       $t0, .L8001A0A0
    /* 1AC94 8001A094 29010080 */   slti      $at, $t0, 0x80
    /* 1AC98 8001A098 10000008 */  b          .L8001A0BC
    /* 1AC9C 8001A09C 00004025 */   or        $t0, $zero, $zero
  .L8001A0A0:
    /* 1ACA0 8001A0A0 14200003 */  bnez       $at, .L8001A0B0
    /* 1ACA4 8001A0A4 01001025 */   or        $v0, $t0, $zero
    /* 1ACA8 8001A0A8 10000004 */  b          .L8001A0BC
    /* 1ACAC 8001A0AC 2408007F */   addiu     $t0, $zero, 0x7F
  .L8001A0B0:
    /* 1ACB0 8001A0B0 10000002 */  b          .L8001A0BC
    /* 1ACB4 8001A0B4 00404025 */   or        $t0, $v0, $zero
  .L8001A0B8:
    /* 1ACB8 8001A0B8 24080080 */  addiu      $t0, $zero, 0x80
  .L8001A0BC:
    /* 1ACBC 8001A0BC 922E0005 */  lbu        $t6, 0x5($s1)
    /* 1ACC0 8001A0C0 2401007F */  addiu      $at, $zero, 0x7F
    /* 1ACC4 8001A0C4 922F0007 */  lbu        $t7, 0x7($s1)
    /* 1ACC8 8001A0C8 01C40019 */  multu      $t6, $a0
    /* 1ACCC 8001A0CC 8FAC00A8 */  lw         $t4, 0xA8($sp)
    /* 1ACD0 8001A0D0 00003812 */  mflo       $a3
    /* 1ACD4 8001A0D4 00000000 */  nop
    /* 1ACD8 8001A0D8 00000000 */  nop
    /* 1ACDC 8001A0DC 00E1001A */  div        $zero, $a3, $at
    /* 1ACE0 8001A0E0 82210006 */  lb         $at, 0x6($s1)
    /* 1ACE4 8001A0E4 00003812 */  mflo       $a3
    /* 1ACE8 8001A0E8 30E700FF */  andi       $a3, $a3, 0xFF
    /* 1ACEC 8001A0EC 00010A00 */  sll        $at, $at, 8
    /* 1ACF0 8001A0F0 01E17825 */  or         $t7, $t7, $at
    /* 1ACF4 8001A0F4 00AF1821 */  addu       $v1, $a1, $t7
    /* 1ACF8 8001A0F8 28610100 */  slti       $at, $v1, 0x100
    /* 1ACFC 8001A0FC 14200003 */  bnez       $at, .L8001A10C
    /* 1AD00 8001A100 00000000 */   nop
    /* 1AD04 8001A104 10000006 */  b          .L8001A120
    /* 1AD08 8001A108 240300FF */   addiu     $v1, $zero, 0xFF
  .L8001A10C:
    /* 1AD0C 8001A10C 04610003 */  bgez       $v1, .L8001A11C
    /* 1AD10 8001A110 00601025 */   or        $v0, $v1, $zero
    /* 1AD14 8001A114 10000001 */  b          .L8001A11C
    /* 1AD18 8001A118 00001025 */   or        $v0, $zero, $zero
  .L8001A11C:
    /* 1AD1C 8001A11C 00401825 */  or         $v1, $v0, $zero
  .L8001A120:
    /* 1AD20 8001A120 92210000 */  lbu        $at, 0x0($s1)
    /* 1AD24 8001A124 92220001 */  lbu        $v0, 0x1($s1)
    /* 1AD28 8001A128 0003C200 */  sll        $t8, $v1, 8
    /* 1AD2C 8001A12C 00010A00 */  sll        $at, $at, 8
    /* 1AD30 8001A130 00411025 */  or         $v0, $v0, $at
    /* 1AD34 8001A134 00025400 */  sll        $t2, $v0, 16
    /* 1AD38 8001A138 00D8C825 */  or         $t9, $a2, $t8
    /* 1AD3C 8001A13C 304BC000 */  andi       $t3, $v0, 0xC000
    /* 1AD40 8001A140 15600037 */  bnez       $t3, .L8001A220
    /* 1AD44 8001A144 032A2025 */   or        $a0, $t9, $t2
    /* 1AD48 8001A148 15930019 */  bne        $t4, $s3, .L8001A1B0
    /* 1AD4C 8001A14C 31220080 */   andi      $v0, $t1, 0x80
    /* 1AD50 8001A150 97AD00CE */  lhu        $t5, 0xCE($sp)
    /* 1AD54 8001A154 97AE00D2 */  lhu        $t6, 0xD2($sp)
    /* 1AD58 8001A158 93AF00D7 */  lbu        $t7, 0xD7($sp)
    /* 1AD5C 8001A15C 00503025 */  or         $a2, $v0, $s0
    /* 1AD60 8001A160 30C600FF */  andi       $a2, $a2, 0xFF
    /* 1AD64 8001A164 97A500B6 */  lhu        $a1, 0xB6($sp)
    /* 1AD68 8001A168 AFA80010 */  sw         $t0, 0x10($sp)
    /* 1AD6C 8001A16C AFB50014 */  sw         $s5, 0x14($sp)
    /* 1AD70 8001A170 AFB60018 */  sw         $s6, 0x18($sp)
    /* 1AD74 8001A174 AFA00024 */  sw         $zero, 0x24($sp)
    /* 1AD78 8001A178 AFAD001C */  sw         $t5, 0x1C($sp)
    /* 1AD7C 8001A17C AFAE0020 */  sw         $t6, 0x20($sp)
    /* 1AD80 8001A180 0C009262 */  jal        func_80024988
    /* 1AD84 8001A184 AFAF0028 */   sw        $t7, 0x28($sp)
    /* 1AD88 8001A188 10530025 */  beq        $v0, $s3, .L8001A220
    /* 1AD8C 8001A18C 00409025 */   or        $s2, $v0, $zero
    /* 1AD90 8001A190 305800FF */  andi       $t8, $v0, 0xFF
    /* 1AD94 8001A194 031E0019 */  multu      $t8, $fp
    /* 1AD98 8001A198 0000C812 */  mflo       $t9
    /* 1AD9C 8001A19C 02F92021 */  addu       $a0, $s7, $t9
    /* 1ADA0 8001A1A0 0C007B38 */  jal        func_8001ECE0
    /* 1ADA4 8001A1A4 00000000 */   nop
    /* 1ADA8 8001A1A8 1000001D */  b          .L8001A220
    /* 1ADAC 8001A1AC AFA200A8 */   sw        $v0, 0xA8($sp)
  .L8001A1B0:
    /* 1ADB0 8001A1B0 97AA00CE */  lhu        $t2, 0xCE($sp)
    /* 1ADB4 8001A1B4 97AB00D2 */  lhu        $t3, 0xD2($sp)
    /* 1ADB8 8001A1B8 93AC00D7 */  lbu        $t4, 0xD7($sp)
    /* 1ADBC 8001A1BC 00503025 */  or         $a2, $v0, $s0
    /* 1ADC0 8001A1C0 30C600FF */  andi       $a2, $a2, 0xFF
    /* 1ADC4 8001A1C4 97A500B6 */  lhu        $a1, 0xB6($sp)
    /* 1ADC8 8001A1C8 AFA80010 */  sw         $t0, 0x10($sp)
    /* 1ADCC 8001A1CC AFB50014 */  sw         $s5, 0x14($sp)
    /* 1ADD0 8001A1D0 AFB60018 */  sw         $s6, 0x18($sp)
    /* 1ADD4 8001A1D4 AFA00024 */  sw         $zero, 0x24($sp)
    /* 1ADD8 8001A1D8 AFAA001C */  sw         $t2, 0x1C($sp)
    /* 1ADDC 8001A1DC AFAB0020 */  sw         $t3, 0x20($sp)
    /* 1ADE0 8001A1E0 0C009262 */  jal        func_80024988
    /* 1ADE4 8001A1E4 AFAC0028 */   sw        $t4, 0x28($sp)
    /* 1ADE8 8001A1E8 1053000D */  beq        $v0, $s3, .L8001A220
    /* 1ADEC 8001A1EC 324D00FF */   andi      $t5, $s2, 0xFF
    /* 1ADF0 8001A1F0 01BE0019 */  multu      $t5, $fp
    /* 1ADF4 8001A1F4 305800FF */  andi       $t8, $v0, 0xFF
    /* 1ADF8 8001A1F8 00007012 */  mflo       $t6
    /* 1ADFC 8001A1FC 02EE7821 */  addu       $t7, $s7, $t6
    /* 1AE00 8001A200 A9E20010 */  swl        $v0, 0x10($t7)
    /* 1AE04 8001A204 031E0019 */  multu      $t8, $fp
    /* 1AE08 8001A208 B9E20013 */  swr        $v0, 0x13($t7)
    /* 1AE0C 8001A20C 0000C812 */  mflo       $t9
    /* 1AE10 8001A210 02F95021 */  addu       $t2, $s7, $t9
    /* 1AE14 8001A214 A9520014 */  swl        $s2, 0x14($t2)
    /* 1AE18 8001A218 B9520017 */  swr        $s2, 0x17($t2)
    /* 1AE1C 8001A21C 00409025 */  or         $s2, $v0, $zero
  .L8001A220:
    /* 1AE20 8001A220 97AB00AE */  lhu        $t3, 0xAE($sp)
  .L8001A224:
    /* 1AE24 8001A224 26940001 */  addiu      $s4, $s4, 0x1
    /* 1AE28 8001A228 2631000C */  addiu      $s1, $s1, 0xC
    /* 1AE2C 8001A22C 028B082A */  slt        $at, $s4, $t3
    /* 1AE30 8001A230 5420FF67 */  bnel       $at, $zero, .L80019FD0
    /* 1AE34 8001A234 92210000 */   lbu       $at, 0x0($s1)
    /* 1AE38 8001A238 AFB200A4 */  sw         $s2, 0xA4($sp)
  .L8001A23C:
    /* 1AE3C 8001A23C 8FA200A8 */  lw         $v0, 0xA8($sp)
    /* 1AE40 8001A240 8FBF005C */  lw         $ra, 0x5C($sp)
  .L8001A244:
    /* 1AE44 8001A244 8FB00038 */  lw         $s0, 0x38($sp)
    /* 1AE48 8001A248 8FB1003C */  lw         $s1, 0x3C($sp)
    /* 1AE4C 8001A24C 8FB20040 */  lw         $s2, 0x40($sp)
    /* 1AE50 8001A250 8FB30044 */  lw         $s3, 0x44($sp)
    /* 1AE54 8001A254 8FB40048 */  lw         $s4, 0x48($sp)
    /* 1AE58 8001A258 8FB5004C */  lw         $s5, 0x4C($sp)
    /* 1AE5C 8001A25C 8FB60050 */  lw         $s6, 0x50($sp)
    /* 1AE60 8001A260 8FB70054 */  lw         $s7, 0x54($sp)
    /* 1AE64 8001A264 8FBE0058 */  lw         $fp, 0x58($sp)
    /* 1AE68 8001A268 03E00008 */  jr         $ra
    /* 1AE6C 8001A26C 27BD00B0 */   addiu     $sp, $sp, 0xB0
endlabel func_80019F48
