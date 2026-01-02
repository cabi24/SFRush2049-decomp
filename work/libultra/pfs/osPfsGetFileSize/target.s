# Source: B9F0.s
# Address: 0x8000AEC4

glabel func_8000AEC4
    /* BAC4 8000AEC4 27BDFE90 */  addiu      $sp, $sp, -0x170
    /* BAC8 8000AEC8 AFBF002C */  sw         $ra, 0x2C($sp)
    /* BACC 8000AECC AFB30028 */  sw         $s3, 0x28($sp)
    /* BAD0 8000AED0 AFB20024 */  sw         $s2, 0x24($sp)
    /* BAD4 8000AED4 AFB10020 */  sw         $s1, 0x20($sp)
    /* BAD8 8000AED8 AFB0001C */  sw         $s0, 0x1C($sp)
    /* BADC 8000AEDC AFA60178 */  sw         $a2, 0x178($sp)
    /* BAE0 8000AEE0 8C8E0050 */  lw         $t6, 0x50($a0)
    /* BAE4 8000AEE4 30D000FF */  andi       $s0, $a2, 0xFF
    /* BAE8 8000AEE8 00E08825 */  or         $s1, $a3, $zero
    /* BAEC 8000AEEC 00AE082A */  slt        $at, $a1, $t6
    /* BAF0 8000AEF0 10200003 */  beqz       $at, .L8000AF00
    /* BAF4 8000AEF4 00809025 */   or        $s2, $a0, $zero
    /* BAF8 8000AEF8 04A10003 */  bgez       $a1, .L8000AF08
    /* BAFC 8000AEFC 8FA20180 */   lw        $v0, 0x180($sp)
  .L8000AF00:
    /* BB00 8000AF00 100000C7 */  b          .L8000B220
    /* BB04 8000AF04 24020005 */   addiu     $v0, $zero, 0x5
  .L8000AF08:
    /* BB08 8000AF08 18400003 */  blez       $v0, .L8000AF18
    /* BB0C 8000AF0C 304F001F */   andi      $t7, $v0, 0x1F
    /* BB10 8000AF10 11E00003 */  beqz       $t7, .L8000AF20
    /* BB14 8000AF14 00000000 */   nop
  .L8000AF18:
    /* BB18 8000AF18 100000C1 */  b          .L8000B220
    /* BB1C 8000AF1C 24020005 */   addiu     $v0, $zero, 0x5
  .L8000AF20:
    /* BB20 8000AF20 06200003 */  bltz       $s1, .L8000AF30
    /* BB24 8000AF24 3238001F */   andi      $t8, $s1, 0x1F
    /* BB28 8000AF28 53000004 */  beql       $t8, $zero, .L8000AF3C
    /* BB2C 8000AF2C 8E590000 */   lw        $t9, 0x0($s2)
  .L8000AF30:
    /* BB30 8000AF30 100000BB */  b          .L8000B220
    /* BB34 8000AF34 24020005 */   addiu     $v0, $zero, 0x5
    /* BB38 8000AF38 8E590000 */  lw         $t9, 0x0($s2)
  .L8000AF3C:
    /* BB3C 8000AF3C 02402025 */  or         $a0, $s2, $zero
    /* BB40 8000AF40 33280001 */  andi       $t0, $t9, 0x1
    /* BB44 8000AF44 15000003 */  bnez       $t0, .L8000AF54
    /* BB48 8000AF48 00000000 */   nop
    /* BB4C 8000AF4C 100000B4 */  b          .L8000B220
    /* BB50 8000AF50 24020005 */   addiu     $v0, $zero, 0x5
  .L8000AF54:
    /* BB54 8000AF54 0C003CB4 */  jal        func_8000F2D0
    /* BB58 8000AF58 AFA50174 */   sw        $a1, 0x174($sp)
    /* BB5C 8000AF5C 24010002 */  addiu      $at, $zero, 0x2
    /* BB60 8000AF60 54410004 */  bnel       $v0, $at, .L8000AF74
    /* BB64 8000AF64 92490065 */   lbu       $t1, 0x65($s2)
    /* BB68 8000AF68 100000AD */  b          .L8000B220
    /* BB6C 8000AF6C 24020002 */   addiu     $v0, $zero, 0x2
    /* BB70 8000AF70 92490065 */  lbu        $t1, 0x65($s2)
  .L8000AF74:
    /* BB74 8000AF74 02402025 */  or         $a0, $s2, $zero
    /* BB78 8000AF78 51200008 */  beql       $t1, $zero, .L8000AF9C
    /* BB7C 8000AF7C 8E4A005C */   lw        $t2, 0x5C($s2)
    /* BB80 8000AF80 0C003A14 */  jal        func_8000E850
    /* BB84 8000AF84 00002825 */   or        $a1, $zero, $zero
    /* BB88 8000AF88 50400004 */  beql       $v0, $zero, .L8000AF9C
    /* BB8C 8000AF8C 8E4A005C */   lw        $t2, 0x5C($s2)
    /* BB90 8000AF90 100000A4 */  b          .L8000B224
    /* BB94 8000AF94 8FBF002C */   lw        $ra, 0x2C($sp)
    /* BB98 8000AF98 8E4A005C */  lw         $t2, 0x5C($s2)
  .L8000AF9C:
    /* BB9C 8000AF9C 8FAB0174 */  lw         $t3, 0x174($sp)
    /* BBA0 8000AFA0 8E440004 */  lw         $a0, 0x4($s2)
    /* BBA4 8000AFA4 8E450008 */  lw         $a1, 0x8($s2)
    /* BBA8 8000AFA8 014B3021 */  addu       $a2, $t2, $t3
    /* BBAC 8000AFAC 30CCFFFF */  andi       $t4, $a2, 0xFFFF
    /* BBB0 8000AFB0 01803025 */  or         $a2, $t4, $zero
    /* BBB4 8000AFB4 0C003A34 */  jal        func_8000E8D0
    /* BBB8 8000AFB8 27A7014C */   addiu     $a3, $sp, 0x14C
    /* BBBC 8000AFBC 10400003 */  beqz       $v0, .L8000AFCC
    /* BBC0 8000AFC0 97AD0150 */   lhu       $t5, 0x150($sp)
    /* BBC4 8000AFC4 10000097 */  b          .L8000B224
    /* BBC8 8000AFC8 8FBF002C */   lw        $ra, 0x2C($sp)
  .L8000AFCC:
    /* BBCC 8000AFCC 11A00003 */  beqz       $t5, .L8000AFDC
    /* BBD0 8000AFD0 8FAE014C */   lw        $t6, 0x14C($sp)
    /* BBD4 8000AFD4 15C00003 */  bnez       $t6, .L8000AFE4
    /* BBD8 8000AFD8 97AF0152 */   lhu       $t7, 0x152($sp)
  .L8000AFDC:
    /* BBDC 8000AFDC 10000090 */  b          .L8000B220
    /* BBE0 8000AFE0 24020005 */   addiu     $v0, $zero, 0x5
  .L8000AFE4:
    /* BBE4 8000AFE4 8E580060 */  lw         $t8, 0x60($s2)
    /* BBE8 8000AFE8 93B90152 */  lbu        $t9, 0x152($sp)
    /* BBEC 8000AFEC 97AA0152 */  lhu        $t2, 0x152($sp)
    /* BBF0 8000AFF0 01F8082A */  slt        $at, $t7, $t8
    /* BBF4 8000AFF4 5420000A */  bnel       $at, $zero, .L8000B020
    /* BBF8 8000AFF8 24010001 */   addiu     $at, $zero, 0x1
    /* BBFC 8000AFFC 92480064 */  lbu        $t0, 0x64($s2)
    /* BC00 8000B000 93A90153 */  lbu        $t1, 0x153($sp)
    /* BC04 8000B004 0328082A */  slt        $at, $t9, $t0
    /* BC08 8000B008 50200005 */  beql       $at, $zero, .L8000B020
    /* BC0C 8000B00C 24010001 */   addiu     $at, $zero, 0x1
    /* BC10 8000B010 19200002 */  blez       $t1, .L8000B01C
    /* BC14 8000B014 29210080 */   slti      $at, $t1, 0x80
    /* BC18 8000B018 14200007 */  bnez       $at, .L8000B038
  .L8000B01C:
    /* BC1C 8000B01C 24010001 */   addiu     $at, $zero, 0x1
  .L8000B020:
    /* BC20 8000B020 15410003 */  bne        $t2, $at, .L8000B030
    /* BC24 8000B024 00000000 */   nop
    /* BC28 8000B028 1000007D */  b          .L8000B220
    /* BC2C 8000B02C 24020005 */   addiu     $v0, $zero, 0x5
  .L8000B030:
    /* BC30 8000B030 1000007B */  b          .L8000B220
    /* BC34 8000B034 24020003 */   addiu     $v0, $zero, 0x3
  .L8000B038:
    /* BC38 8000B038 16000007 */  bnez       $s0, .L8000B058
    /* BC3C 8000B03C AFB00034 */   sw        $s0, 0x34($sp)
    /* BC40 8000B040 93AC0154 */  lbu        $t4, 0x154($sp)
    /* BC44 8000B044 318D0002 */  andi       $t5, $t4, 0x2
    /* BC48 8000B048 55A00004 */  bnel       $t5, $zero, .L8000B05C
    /* BC4C 8000B04C 97AF0152 */   lhu       $t7, 0x152($sp)
    /* BC50 8000B050 10000073 */  b          .L8000B220
    /* BC54 8000B054 24020006 */   addiu     $v0, $zero, 0x6
  .L8000B058:
    /* BC58 8000B058 97AF0152 */  lhu        $t7, 0x152($sp)
  .L8000B05C:
    /* BC5C 8000B05C 240E00FF */  addiu      $t6, $zero, 0xFF
    /* BC60 8000B060 A3AE003B */  sb         $t6, 0x3B($sp)
    /* BC64 8000B064 06210003 */  bgez       $s1, .L8000B074
    /* BC68 8000B068 00111143 */   sra       $v0, $s1, 5
    /* BC6C 8000B06C 2621001F */  addiu      $at, $s1, 0x1F
    /* BC70 8000B070 00011143 */  sra        $v0, $at, 5
  .L8000B074:
    /* BC74 8000B074 28410008 */  slti       $at, $v0, 0x8
    /* BC78 8000B078 00408025 */  or         $s0, $v0, $zero
    /* BC7C 8000B07C 1420000D */  bnez       $at, .L8000B0B4
    /* BC80 8000B080 A7AF0048 */   sh        $t7, 0x48($sp)
    /* BC84 8000B084 02402025 */  or         $a0, $s2, $zero
  .L8000B088:
    /* BC88 8000B088 27A5003B */  addiu      $a1, $sp, 0x3B
    /* BC8C 8000B08C 27A6004C */  addiu      $a2, $sp, 0x4C
    /* BC90 8000B090 0C002B7C */  jal        func_8000ADF0
    /* BC94 8000B094 27A70048 */   addiu     $a3, $sp, 0x48
    /* BC98 8000B098 10400003 */  beqz       $v0, .L8000B0A8
    /* BC9C 8000B09C 2610FFF8 */   addiu     $s0, $s0, -0x8
    /* BCA0 8000B0A0 10000060 */  b          .L8000B224
    /* BCA4 8000B0A4 8FBF002C */   lw        $ra, 0x2C($sp)
  .L8000B0A8:
    /* BCA8 8000B0A8 2A010008 */  slti       $at, $s0, 0x8
    /* BCAC 8000B0AC 5020FFF6 */  beql       $at, $zero, .L8000B088
    /* BCB0 8000B0B0 02402025 */   or        $a0, $s2, $zero
  .L8000B0B4:
    /* BCB4 8000B0B4 8FA20180 */  lw         $v0, 0x180($sp)
    /* BCB8 8000B0B8 8FB10184 */  lw         $s1, 0x184($sp)
    /* BCBC 8000B0BC 04410003 */  bgez       $v0, .L8000B0CC
    /* BCC0 8000B0C0 0002C143 */   sra       $t8, $v0, 5
    /* BCC4 8000B0C4 2441001F */  addiu      $at, $v0, 0x1F
    /* BCC8 8000B0C8 0001C143 */  sra        $t8, $at, 5
  .L8000B0CC:
    /* BCCC 8000B0CC 1B000031 */  blez       $t8, .L8000B194
    /* BCD0 8000B0D0 03009825 */   or        $s3, $t8, $zero
  .L8000B0D4:
    /* BCD4 8000B0D4 24010008 */  addiu      $at, $zero, 0x8
    /* BCD8 8000B0D8 16010009 */  bne        $s0, $at, .L8000B100
    /* BCDC 8000B0DC 02402025 */   or        $a0, $s2, $zero
    /* BCE0 8000B0E0 27A5003B */  addiu      $a1, $sp, 0x3B
    /* BCE4 8000B0E4 27A6004C */  addiu      $a2, $sp, 0x4C
    /* BCE8 8000B0E8 0C002B7C */  jal        func_8000ADF0
    /* BCEC 8000B0EC 27A70048 */   addiu     $a3, $sp, 0x48
    /* BCF0 8000B0F0 10400003 */  beqz       $v0, .L8000B100
    /* BCF4 8000B0F4 00008025 */   or        $s0, $zero, $zero
    /* BCF8 8000B0F8 1000004A */  b          .L8000B224
    /* BCFC 8000B0FC 8FBF002C */   lw        $ra, 0x2C($sp)
  .L8000B100:
    /* BD00 8000B100 93B90048 */  lbu        $t9, 0x48($sp)
    /* BD04 8000B104 92480065 */  lbu        $t0, 0x65($s2)
    /* BD08 8000B108 02402025 */  or         $a0, $s2, $zero
    /* BD0C 8000B10C 53280008 */  beql       $t9, $t0, .L8000B130
    /* BD10 8000B110 93A90049 */   lbu       $t1, 0x49($sp)
    /* BD14 8000B114 0C003A14 */  jal        func_8000E850
    /* BD18 8000B118 03202825 */   or        $a1, $t9, $zero
    /* BD1C 8000B11C 50400004 */  beql       $v0, $zero, .L8000B130
    /* BD20 8000B120 93A90049 */   lbu       $t1, 0x49($sp)
    /* BD24 8000B124 1000003F */  b          .L8000B224
    /* BD28 8000B128 8FBF002C */   lw        $ra, 0x2C($sp)
    /* BD2C 8000B12C 93A90049 */  lbu        $t1, 0x49($sp)
  .L8000B130:
    /* BD30 8000B130 8FAC0034 */  lw         $t4, 0x34($sp)
    /* BD34 8000B134 02203825 */  or         $a3, $s1, $zero
    /* BD38 8000B138 000950C0 */  sll        $t2, $t1, 3
    /* BD3C 8000B13C 01503021 */  addu       $a2, $t2, $s0
    /* BD40 8000B140 30CBFFFF */  andi       $t3, $a2, 0xFFFF
    /* BD44 8000B144 15800007 */  bnez       $t4, .L8000B164
    /* BD48 8000B148 01603025 */   or        $a2, $t3, $zero
    /* BD4C 8000B14C 8E440004 */  lw         $a0, 0x4($s2)
    /* BD50 8000B150 8E450008 */  lw         $a1, 0x8($s2)
    /* BD54 8000B154 0C003A34 */  jal        func_8000E8D0
    /* BD58 8000B158 02203825 */   or        $a3, $s1, $zero
    /* BD5C 8000B15C 10000006 */  b          .L8000B178
    /* BD60 8000B160 00401825 */   or        $v1, $v0, $zero
  .L8000B164:
    /* BD64 8000B164 8E440004 */  lw         $a0, 0x4($s2)
    /* BD68 8000B168 8E450008 */  lw         $a1, 0x8($s2)
    /* BD6C 8000B16C 0C003DA0 */  jal        func_8000F680
    /* BD70 8000B170 AFA00010 */   sw        $zero, 0x10($sp)
    /* BD74 8000B174 00401825 */  or         $v1, $v0, $zero
  .L8000B178:
    /* BD78 8000B178 10400003 */  beqz       $v0, .L8000B188
    /* BD7C 8000B17C 26310020 */   addiu     $s1, $s1, 0x20
    /* BD80 8000B180 10000027 */  b          .L8000B220
    /* BD84 8000B184 00601025 */   or        $v0, $v1, $zero
  .L8000B188:
    /* BD88 8000B188 2673FFFF */  addiu      $s3, $s3, -0x1
    /* BD8C 8000B18C 1660FFD1 */  bnez       $s3, .L8000B0D4
    /* BD90 8000B190 26100001 */   addiu     $s0, $s0, 0x1
  .L8000B194:
    /* BD94 8000B194 8FAD0034 */  lw         $t5, 0x34($sp)
    /* BD98 8000B198 93A20154 */  lbu        $v0, 0x154($sp)
    /* BD9C 8000B19C 24010001 */  addiu      $at, $zero, 0x1
    /* BDA0 8000B1A0 15A1001C */  bne        $t5, $at, .L8000B214
    /* BDA4 8000B1A4 304E0002 */   andi      $t6, $v0, 0x2
    /* BDA8 8000B1A8 15C0001A */  bnez       $t6, .L8000B214
    /* BDAC 8000B1AC 344F0002 */   ori       $t7, $v0, 0x2
    /* BDB0 8000B1B0 A3AF0154 */  sb         $t7, 0x154($sp)
    /* BDB4 8000B1B4 92580065 */  lbu        $t8, 0x65($s2)
    /* BDB8 8000B1B8 02402025 */  or         $a0, $s2, $zero
    /* BDBC 8000B1BC 53000008 */  beql       $t8, $zero, .L8000B1E0
    /* BDC0 8000B1C0 8E48005C */   lw        $t0, 0x5C($s2)
    /* BDC4 8000B1C4 0C003A14 */  jal        func_8000E850
    /* BDC8 8000B1C8 00002825 */   or        $a1, $zero, $zero
    /* BDCC 8000B1CC 50400004 */  beql       $v0, $zero, .L8000B1E0
    /* BDD0 8000B1D0 8E48005C */   lw        $t0, 0x5C($s2)
    /* BDD4 8000B1D4 10000013 */  b          .L8000B224
    /* BDD8 8000B1D8 8FBF002C */   lw        $ra, 0x2C($sp)
    /* BDDC 8000B1DC 8E48005C */  lw         $t0, 0x5C($s2)
  .L8000B1E0:
    /* BDE0 8000B1E0 8FB90174 */  lw         $t9, 0x174($sp)
    /* BDE4 8000B1E4 8E440004 */  lw         $a0, 0x4($s2)
    /* BDE8 8000B1E8 8E450008 */  lw         $a1, 0x8($s2)
    /* BDEC 8000B1EC 01193021 */  addu       $a2, $t0, $t9
    /* BDF0 8000B1F0 30C9FFFF */  andi       $t1, $a2, 0xFFFF
    /* BDF4 8000B1F4 01203025 */  or         $a2, $t1, $zero
    /* BDF8 8000B1F8 AFA00010 */  sw         $zero, 0x10($sp)
    /* BDFC 8000B1FC 0C003DA0 */  jal        func_8000F680
    /* BE00 8000B200 27A7014C */   addiu     $a3, $sp, 0x14C
    /* BE04 8000B204 50400004 */  beql       $v0, $zero, .L8000B218
    /* BE08 8000B208 8E440004 */   lw        $a0, 0x4($s2)
    /* BE0C 8000B20C 10000005 */  b          .L8000B224
    /* BE10 8000B210 8FBF002C */   lw        $ra, 0x2C($sp)
  .L8000B214:
    /* BE14 8000B214 8E440004 */  lw         $a0, 0x4($s2)
  .L8000B218:
    /* BE18 8000B218 0C003988 */  jal        func_8000E620
    /* BE1C 8000B21C 8E450008 */   lw        $a1, 0x8($s2)
  .L8000B220:
    /* BE20 8000B220 8FBF002C */  lw         $ra, 0x2C($sp)
  .L8000B224:
    /* BE24 8000B224 8FB0001C */  lw         $s0, 0x1C($sp)
    /* BE28 8000B228 8FB10020 */  lw         $s1, 0x20($sp)
    /* BE2C 8000B22C 8FB20024 */  lw         $s2, 0x24($sp)
    /* BE30 8000B230 8FB30028 */  lw         $s3, 0x28($sp)
    /* BE34 8000B234 03E00008 */  jr         $ra
    /* BE38 8000B238 27BD0170 */   addiu     $sp, $sp, 0x170