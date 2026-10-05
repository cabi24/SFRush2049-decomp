nonmatching func_80018FEC, 0x80

glabel func_80018FEC
    /* 19BEC 80018FEC 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 19BF0 80018FF0 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 19BF4 80018FF4 0C005D91 */  jal        func_80017644
    /* 19BF8 80018FF8 00000000 */   nop
    /* 19BFC 80018FFC 2401FFFF */  addiu      $at, $zero, -0x1
    /* 19C00 80019000 10410016 */  beq        $v0, $at, .L8001905C
    /* 19C04 80019004 00027000 */   sll       $t6, $v0, 0
    /* 19C08 80019008 05C00009 */  bltz       $t6, .L80019030
    /* 19C0C 8001900C 3C017FFF */   lui       $at, (0x7FFFFFFF >> 16)
    /* 19C10 80019010 0002C240 */  sll        $t8, $v0, 9
    /* 19C14 80019014 0302C023 */  subu       $t8, $t8, $v0
    /* 19C18 80019018 0018C0C0 */  sll        $t8, $t8, 3
    /* 19C1C 8001901C 3C018004 */  lui        $at, %hi(D_80043EB8 + 0xFC0)
    /* 19C20 80019020 00380821 */  addu       $at, $at, $t8
    /* 19C24 80019024 240F0001 */  addiu      $t7, $zero, 0x1
    /* 19C28 80019028 1000000C */  b          .L8001905C
    /* 19C2C 8001902C A02F4E78 */   sb        $t7, %lo(D_80043EB8 + 0xFC0)($at)
  .L80019030:
    /* 19C30 80019030 3421FFFF */  ori        $at, $at, (0x7FFFFFFF & 0xFFFF)
    /* 19C34 80019034 0041C824 */  and        $t9, $v0, $at
    /* 19C38 80019038 00194240 */  sll        $t0, $t9, 9
    /* 19C3C 8001903C 01194023 */  subu       $t0, $t0, $t9
    /* 19C40 80019040 3C098004 */  lui        $t1, %hi(D_80043EB8)
    /* 19C44 80019044 25293EB8 */  addiu      $t1, $t1, %lo(D_80043EB8)
    /* 19C48 80019048 000840C0 */  sll        $t0, $t0, 3
    /* 19C4C 8001904C 01091821 */  addu       $v1, $t0, $t1
    /* 19C50 80019050 906A0FEE */  lbu        $t2, 0xFEE($v1)
    /* 19C54 80019054 314BFFF7 */  andi       $t3, $t2, 0xFFF7
    /* 19C58 80019058 A06B0FEE */  sb         $t3, 0xFEE($v1)
  .L8001905C:
    /* 19C5C 8001905C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 19C60 80019060 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 19C64 80019064 03E00008 */  jr         $ra
    /* 19C68 80019068 00000000 */   nop
endlabel func_80018FEC
