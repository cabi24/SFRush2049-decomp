nonmatching func_8001BE14, 0x388

glabel func_8001BE14
    /* 1CA14 8001BE14 27BDFFD0 */  addiu      $sp, $sp, -0x30
    /* 1CA18 8001BE18 AFA50034 */  sw         $a1, 0x34($sp)
    /* 1CA1C 8001BE1C 30A5FFFF */  andi       $a1, $a1, 0xFFFF
    /* 1CA20 8001BE20 AFA60038 */  sw         $a2, 0x38($sp)
    /* 1CA24 8001BE24 30C600FF */  andi       $a2, $a2, 0xFF
    /* 1CA28 8001BE28 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 1CA2C 8001BE2C AFA40030 */  sw         $a0, 0x30($sp)
    /* 1CA30 8001BE30 14A00003 */  bnez       $a1, .L8001BE40
    /* 1CA34 8001BE34 308700FF */   andi      $a3, $a0, 0xFF
    /* 1CA38 8001BE38 24A50001 */  addiu      $a1, $a1, 0x1
    /* 1CA3C 8001BE3C 30A5FFFF */  andi       $a1, $a1, 0xFFFF
  .L8001BE40:
    /* 1CA40 8001BE40 27A40028 */  addiu      $a0, $sp, 0x28
    /* 1CA44 8001BE44 A7A50036 */  sh         $a1, 0x36($sp)
    /* 1CA48 8001BE48 A3A6003B */  sb         $a2, 0x3B($sp)
    /* 1CA4C 8001BE4C A3A70033 */  sb         $a3, 0x33($sp)
    /* 1CA50 8001BE50 0C007A4C */  jal        func_8001E930
    /* 1CA54 8001BE54 AFA50028 */   sw        $a1, 0x28($sp)
    /* 1CA58 8001BE58 93A6003B */  lbu        $a2, 0x3B($sp)
    /* 1CA5C 8001BE5C 97A50036 */  lhu        $a1, 0x36($sp)
    /* 1CA60 8001BE60 93A70033 */  lbu        $a3, 0x33($sp)
    /* 1CA64 8001BE64 24CEFF06 */  addiu      $t6, $a2, -0xFA
    /* 1CA68 8001BE68 2DC10006 */  sltiu      $at, $t6, 0x6
    /* 1CA6C 8001BE6C 102000B0 */  beqz       $at, .L8001C130
    /* 1CA70 8001BE70 8FA80028 */   lw        $t0, 0x28($sp)
    /* 1CA74 8001BE74 000E7080 */  sll        $t6, $t6, 2
    /* 1CA78 8001BE78 3C018003 */  lui        $at, %hi(jtbl_8002D8E8_main)
    /* 1CA7C 8001BE7C 002E0821 */  addu       $at, $at, $t6
    /* 1CA80 8001BE80 8C2ED8E8 */  lw         $t6, %lo(jtbl_8002D8E8_main)($at)
    /* 1CA84 8001BE84 01C00008 */  jr         $t6
    /* 1CA88 8001BE88 00000000 */   nop
    /* 1CA8C 8001BE8C 3C038005 */  lui        $v1, %hi(D_8004F300)
    /* 1CA90 8001BE90 3C068005 */  lui        $a2, %hi(D_8004F800)
    /* 1CA94 8001BE94 24C6F800 */  addiu      $a2, $a2, %lo(D_8004F800)
    /* 1CA98 8001BE98 2463F300 */  addiu      $v1, $v1, %lo(D_8004F300)
    /* 1CA9C 8001BE9C 8FA80028 */  lw         $t0, 0x28($sp)
    /* 1CAA0 8001BEA0 24040001 */  addiu      $a0, $zero, 0x1
    /* 1CAA4 8001BEA4 90620014 */  lbu        $v0, 0x14($v1)
  .L8001BEA8:
    /* 1CAA8 8001BEA8 50400004 */  beql       $v0, $zero, .L8001BEBC
    /* 1CAAC 8001BEAC 8C6F0018 */   lw        $t7, 0x18($v1)
    /* 1CAB0 8001BEB0 54820013 */  bnel       $a0, $v0, .L8001BF00
    /* 1CAB4 8001BEB4 9062003C */   lbu       $v0, 0x3C($v1)
    /* 1CAB8 8001BEB8 8C6F0018 */  lw         $t7, 0x18($v1)
  .L8001BEBC:
    /* 1CABC 8001BEBC 00071400 */  sll        $v0, $a3, 16
    /* 1CAC0 8001BEC0 AC62001C */  sw         $v0, 0x1C($v1)
    /* 1CAC4 8001BEC4 004FC023 */  subu       $t8, $v0, $t7
    /* 1CAC8 8001BEC8 0305001A */  div        $zero, $t8, $a1
    /* 1CACC 8001BECC 0000C812 */  mflo       $t9
    /* 1CAD0 8001BED0 AC680024 */  sw         $t0, 0x24($v1)
    /* 1CAD4 8001BED4 14A00002 */  bnez       $a1, .L8001BEE0
    /* 1CAD8 8001BED8 00000000 */   nop
    /* 1CADC 8001BEDC 0007000D */  break      7
  .L8001BEE0:
    /* 1CAE0 8001BEE0 2401FFFF */  addiu      $at, $zero, -0x1
    /* 1CAE4 8001BEE4 14A10004 */  bne        $a1, $at, .L8001BEF8
    /* 1CAE8 8001BEE8 3C018000 */   lui       $at, (0x80000000 >> 16)
    /* 1CAEC 8001BEEC 17010002 */  bne        $t8, $at, .L8001BEF8
    /* 1CAF0 8001BEF0 00000000 */   nop
    /* 1CAF4 8001BEF4 0006000D */  break      6
  .L8001BEF8:
    /* 1CAF8 8001BEF8 AC790020 */  sw         $t9, 0x20($v1)
    /* 1CAFC 8001BEFC 9062003C */  lbu        $v0, 0x3C($v1)
  .L8001BF00:
    /* 1CB00 8001BF00 50400004 */  beql       $v0, $zero, .L8001BF14
    /* 1CB04 8001BF04 8C6A0040 */   lw        $t2, 0x40($v1)
    /* 1CB08 8001BF08 54820013 */  bnel       $a0, $v0, .L8001BF58
    /* 1CB0C 8001BF0C 24630050 */   addiu     $v1, $v1, 0x50
    /* 1CB10 8001BF10 8C6A0040 */  lw         $t2, 0x40($v1)
  .L8001BF14:
    /* 1CB14 8001BF14 00071400 */  sll        $v0, $a3, 16
    /* 1CB18 8001BF18 AC620044 */  sw         $v0, 0x44($v1)
    /* 1CB1C 8001BF1C 004A5823 */  subu       $t3, $v0, $t2
    /* 1CB20 8001BF20 0165001A */  div        $zero, $t3, $a1
    /* 1CB24 8001BF24 00006012 */  mflo       $t4
    /* 1CB28 8001BF28 AC6C0048 */  sw         $t4, 0x48($v1)
    /* 1CB2C 8001BF2C AC68004C */  sw         $t0, 0x4C($v1)
    /* 1CB30 8001BF30 14A00002 */  bnez       $a1, .L8001BF3C
    /* 1CB34 8001BF34 00000000 */   nop
    /* 1CB38 8001BF38 0007000D */  break      7
  .L8001BF3C:
    /* 1CB3C 8001BF3C 2401FFFF */  addiu      $at, $zero, -0x1
    /* 1CB40 8001BF40 14A10004 */  bne        $a1, $at, .L8001BF54
    /* 1CB44 8001BF44 3C018000 */   lui       $at, (0x80000000 >> 16)
    /* 1CB48 8001BF48 15610002 */  bne        $t3, $at, .L8001BF54
    /* 1CB4C 8001BF4C 00000000 */   nop
    /* 1CB50 8001BF50 0006000D */  break      6
  .L8001BF54:
    /* 1CB54 8001BF54 24630050 */  addiu      $v1, $v1, 0x50
  .L8001BF58:
    /* 1CB58 8001BF58 5466FFD3 */  bnel       $v1, $a2, .L8001BEA8
    /* 1CB5C 8001BF5C 90620014 */   lbu       $v0, 0x14($v1)
    /* 1CB60 8001BF60 1000008A */  b          .L8001C18C
    /* 1CB64 8001BF64 AFA80028 */   sw        $t0, 0x28($sp)
    /* 1CB68 8001BF68 3C038005 */  lui        $v1, %hi(D_8004F300)
    /* 1CB6C 8001BF6C 3C098005 */  lui        $t1, %hi(D_8004F800)
    /* 1CB70 8001BF70 2529F800 */  addiu      $t1, $t1, %lo(D_8004F800)
    /* 1CB74 8001BF74 2463F300 */  addiu      $v1, $v1, %lo(D_8004F300)
    /* 1CB78 8001BF78 8FA80028 */  lw         $t0, 0x28($sp)
    /* 1CB7C 8001BF7C 24060003 */  addiu      $a2, $zero, 0x3
    /* 1CB80 8001BF80 24040002 */  addiu      $a0, $zero, 0x2
    /* 1CB84 8001BF84 90620014 */  lbu        $v0, 0x14($v1)
  .L8001BF88:
    /* 1CB88 8001BF88 50820004 */  beql       $a0, $v0, .L8001BF9C
    /* 1CB8C 8001BF8C 8C6D0018 */   lw        $t5, 0x18($v1)
    /* 1CB90 8001BF90 54C20013 */  bnel       $a2, $v0, .L8001BFE0
    /* 1CB94 8001BF94 9062003C */   lbu       $v0, 0x3C($v1)
    /* 1CB98 8001BF98 8C6D0018 */  lw         $t5, 0x18($v1)
  .L8001BF9C:
    /* 1CB9C 8001BF9C 00071400 */  sll        $v0, $a3, 16
    /* 1CBA0 8001BFA0 AC62001C */  sw         $v0, 0x1C($v1)
    /* 1CBA4 8001BFA4 004D7023 */  subu       $t6, $v0, $t5
    /* 1CBA8 8001BFA8 01C5001A */  div        $zero, $t6, $a1
    /* 1CBAC 8001BFAC 00007812 */  mflo       $t7
    /* 1CBB0 8001BFB0 AC680024 */  sw         $t0, 0x24($v1)
    /* 1CBB4 8001BFB4 14A00002 */  bnez       $a1, .L8001BFC0
    /* 1CBB8 8001BFB8 00000000 */   nop
    /* 1CBBC 8001BFBC 0007000D */  break      7
  .L8001BFC0:
    /* 1CBC0 8001BFC0 2401FFFF */  addiu      $at, $zero, -0x1
    /* 1CBC4 8001BFC4 14A10004 */  bne        $a1, $at, .L8001BFD8
    /* 1CBC8 8001BFC8 3C018000 */   lui       $at, (0x80000000 >> 16)
    /* 1CBCC 8001BFCC 15C10002 */  bne        $t6, $at, .L8001BFD8
    /* 1CBD0 8001BFD0 00000000 */   nop
    /* 1CBD4 8001BFD4 0006000D */  break      6
  .L8001BFD8:
    /* 1CBD8 8001BFD8 AC6F0020 */  sw         $t7, 0x20($v1)
    /* 1CBDC 8001BFDC 9062003C */  lbu        $v0, 0x3C($v1)
  .L8001BFE0:
    /* 1CBE0 8001BFE0 50820004 */  beql       $a0, $v0, .L8001BFF4
    /* 1CBE4 8001BFE4 8C780040 */   lw        $t8, 0x40($v1)
    /* 1CBE8 8001BFE8 54C20013 */  bnel       $a2, $v0, .L8001C038
    /* 1CBEC 8001BFEC 24630050 */   addiu     $v1, $v1, 0x50
    /* 1CBF0 8001BFF0 8C780040 */  lw         $t8, 0x40($v1)
  .L8001BFF4:
    /* 1CBF4 8001BFF4 00071400 */  sll        $v0, $a3, 16
    /* 1CBF8 8001BFF8 AC620044 */  sw         $v0, 0x44($v1)
    /* 1CBFC 8001BFFC 0058C823 */  subu       $t9, $v0, $t8
    /* 1CC00 8001C000 0325001A */  div        $zero, $t9, $a1
    /* 1CC04 8001C004 00005012 */  mflo       $t2
    /* 1CC08 8001C008 AC6A0048 */  sw         $t2, 0x48($v1)
    /* 1CC0C 8001C00C AC68004C */  sw         $t0, 0x4C($v1)
    /* 1CC10 8001C010 14A00002 */  bnez       $a1, .L8001C01C
    /* 1CC14 8001C014 00000000 */   nop
    /* 1CC18 8001C018 0007000D */  break      7
  .L8001C01C:
    /* 1CC1C 8001C01C 2401FFFF */  addiu      $at, $zero, -0x1
    /* 1CC20 8001C020 14A10004 */  bne        $a1, $at, .L8001C034
    /* 1CC24 8001C024 3C018000 */   lui       $at, (0x80000000 >> 16)
    /* 1CC28 8001C028 17210002 */  bne        $t9, $at, .L8001C034
    /* 1CC2C 8001C02C 00000000 */   nop
    /* 1CC30 8001C030 0006000D */  break      6
  .L8001C034:
    /* 1CC34 8001C034 24630050 */  addiu      $v1, $v1, 0x50
  .L8001C038:
    /* 1CC38 8001C038 5469FFD3 */  bnel       $v1, $t1, .L8001BF88
    /* 1CC3C 8001C03C 90620014 */   lbu       $v0, 0x14($v1)
    /* 1CC40 8001C040 10000052 */  b          .L8001C18C
    /* 1CC44 8001C044 AFA80028 */   sw        $t0, 0x28($sp)
    /* 1CC48 8001C048 10000006 */  b          .L8001C064
    /* 1CC4C 8001C04C 24020002 */   addiu     $v0, $zero, 0x2
    /* 1CC50 8001C050 10000004 */  b          .L8001C064
    /* 1CC54 8001C054 24020003 */   addiu     $v0, $zero, 0x3
    /* 1CC58 8001C058 10000002 */  b          .L8001C064
    /* 1CC5C 8001C05C 00001025 */   or        $v0, $zero, $zero
    /* 1CC60 8001C060 24020001 */  addiu      $v0, $zero, 0x1
  .L8001C064:
    /* 1CC64 8001C064 3C038005 */  lui        $v1, %hi(D_8004F300)
    /* 1CC68 8001C068 3C068005 */  lui        $a2, %hi(D_8004F800)
    /* 1CC6C 8001C06C 24C6F800 */  addiu      $a2, $a2, %lo(D_8004F800)
    /* 1CC70 8001C070 2463F300 */  addiu      $v1, $v1, %lo(D_8004F300)
    /* 1CC74 8001C074 00402025 */  or         $a0, $v0, $zero
    /* 1CC78 8001C078 8FA80028 */  lw         $t0, 0x28($sp)
    /* 1CC7C 8001C07C 906B0014 */  lbu        $t3, 0x14($v1)
  .L8001C080:
    /* 1CC80 8001C080 00071400 */  sll        $v0, $a3, 16
    /* 1CC84 8001C084 548B0012 */  bnel       $a0, $t3, .L8001C0D0
    /* 1CC88 8001C088 906F003C */   lbu       $t7, 0x3C($v1)
    /* 1CC8C 8001C08C 8C6C0018 */  lw         $t4, 0x18($v1)
    /* 1CC90 8001C090 AC62001C */  sw         $v0, 0x1C($v1)
    /* 1CC94 8001C094 AC680024 */  sw         $t0, 0x24($v1)
    /* 1CC98 8001C098 004C6823 */  subu       $t5, $v0, $t4
    /* 1CC9C 8001C09C 01A5001A */  div        $zero, $t5, $a1
    /* 1CCA0 8001C0A0 00007012 */  mflo       $t6
    /* 1CCA4 8001C0A4 AC6E0020 */  sw         $t6, 0x20($v1)
    /* 1CCA8 8001C0A8 14A00002 */  bnez       $a1, .L8001C0B4
    /* 1CCAC 8001C0AC 00000000 */   nop
    /* 1CCB0 8001C0B0 0007000D */  break      7
  .L8001C0B4:
    /* 1CCB4 8001C0B4 2401FFFF */  addiu      $at, $zero, -0x1
    /* 1CCB8 8001C0B8 14A10004 */  bne        $a1, $at, .L8001C0CC
    /* 1CCBC 8001C0BC 3C018000 */   lui       $at, (0x80000000 >> 16)
    /* 1CCC0 8001C0C0 15A10002 */  bne        $t5, $at, .L8001C0CC
    /* 1CCC4 8001C0C4 00000000 */   nop
    /* 1CCC8 8001C0C8 0006000D */  break      6
  .L8001C0CC:
    /* 1CCCC 8001C0CC 906F003C */  lbu        $t7, 0x3C($v1)
  .L8001C0D0:
    /* 1CCD0 8001C0D0 00071400 */  sll        $v0, $a3, 16
    /* 1CCD4 8001C0D4 548F0012 */  bnel       $a0, $t7, .L8001C120
    /* 1CCD8 8001C0D8 24630050 */   addiu     $v1, $v1, 0x50
    /* 1CCDC 8001C0DC 8C780040 */  lw         $t8, 0x40($v1)
    /* 1CCE0 8001C0E0 AC620044 */  sw         $v0, 0x44($v1)
    /* 1CCE4 8001C0E4 AC68004C */  sw         $t0, 0x4C($v1)
    /* 1CCE8 8001C0E8 0058C823 */  subu       $t9, $v0, $t8
    /* 1CCEC 8001C0EC 0325001A */  div        $zero, $t9, $a1
    /* 1CCF0 8001C0F0 00005012 */  mflo       $t2
    /* 1CCF4 8001C0F4 AC6A0048 */  sw         $t2, 0x48($v1)
    /* 1CCF8 8001C0F8 14A00002 */  bnez       $a1, .L8001C104
    /* 1CCFC 8001C0FC 00000000 */   nop
    /* 1CD00 8001C100 0007000D */  break      7
  .L8001C104:
    /* 1CD04 8001C104 2401FFFF */  addiu      $at, $zero, -0x1
    /* 1CD08 8001C108 14A10004 */  bne        $a1, $at, .L8001C11C
    /* 1CD0C 8001C10C 3C018000 */   lui       $at, (0x80000000 >> 16)
    /* 1CD10 8001C110 17210002 */  bne        $t9, $at, .L8001C11C
    /* 1CD14 8001C114 00000000 */   nop
    /* 1CD18 8001C118 0006000D */  break      6
  .L8001C11C:
    /* 1CD1C 8001C11C 24630050 */  addiu      $v1, $v1, 0x50
  .L8001C120:
    /* 1CD20 8001C120 5466FFD7 */  bnel       $v1, $a2, .L8001C080
    /* 1CD24 8001C124 906B0014 */   lbu       $t3, 0x14($v1)
    /* 1CD28 8001C128 10000018 */  b          .L8001C18C
    /* 1CD2C 8001C12C AFA80028 */   sw        $t0, 0x28($sp)
  .L8001C130:
    /* 1CD30 8001C130 00065880 */  sll        $t3, $a2, 2
    /* 1CD34 8001C134 01665821 */  addu       $t3, $t3, $a2
    /* 1CD38 8001C138 3C0C8005 */  lui        $t4, %hi(D_8004F300)
    /* 1CD3C 8001C13C 258CF300 */  addiu      $t4, $t4, %lo(D_8004F300)
    /* 1CD40 8001C140 000B58C0 */  sll        $t3, $t3, 3
    /* 1CD44 8001C144 016C1821 */  addu       $v1, $t3, $t4
    /* 1CD48 8001C148 8C6D0018 */  lw         $t5, 0x18($v1)
    /* 1CD4C 8001C14C 00071400 */  sll        $v0, $a3, 16
    /* 1CD50 8001C150 AC62001C */  sw         $v0, 0x1C($v1)
    /* 1CD54 8001C154 004D7023 */  subu       $t6, $v0, $t5
    /* 1CD58 8001C158 01C5001A */  div        $zero, $t6, $a1
    /* 1CD5C 8001C15C 00007812 */  mflo       $t7
    /* 1CD60 8001C160 AC680024 */  sw         $t0, 0x24($v1)
    /* 1CD64 8001C164 14A00002 */  bnez       $a1, .L8001C170
    /* 1CD68 8001C168 00000000 */   nop
    /* 1CD6C 8001C16C 0007000D */  break      7
  .L8001C170:
    /* 1CD70 8001C170 2401FFFF */  addiu      $at, $zero, -0x1
    /* 1CD74 8001C174 14A10004 */  bne        $a1, $at, .L8001C188
    /* 1CD78 8001C178 3C018000 */   lui       $at, (0x80000000 >> 16)
    /* 1CD7C 8001C17C 15C10002 */  bne        $t6, $at, .L8001C188
    /* 1CD80 8001C180 00000000 */   nop
    /* 1CD84 8001C184 0006000D */  break      6
  .L8001C188:
    /* 1CD88 8001C188 AC6F0020 */  sw         $t7, 0x20($v1)
  .L8001C18C:
    /* 1CD8C 8001C18C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 1CD90 8001C190 27BD0030 */  addiu      $sp, $sp, 0x30
    /* 1CD94 8001C194 03E00008 */  jr         $ra
    /* 1CD98 8001C198 00000000 */   nop
endlabel func_8001BE14
