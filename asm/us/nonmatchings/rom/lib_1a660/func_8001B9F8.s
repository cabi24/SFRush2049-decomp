nonmatching func_8001B9F8, 0x3C0

glabel func_8001B9F8
    /* 1C5F8 8001B9F8 27BDFFC8 */  addiu      $sp, $sp, -0x38
    /* 1C5FC 8001B9FC AFA5003C */  sw         $a1, 0x3C($sp)
    /* 1C600 8001BA00 30A5FFFF */  andi       $a1, $a1, 0xFFFF
    /* 1C604 8001BA04 AFB00018 */  sw         $s0, 0x18($sp)
    /* 1C608 8001BA08 AFA60040 */  sw         $a2, 0x40($sp)
    /* 1C60C 8001BA0C 30C600FF */  andi       $a2, $a2, 0xFF
    /* 1C610 8001BA10 309000FF */  andi       $s0, $a0, 0xFF
    /* 1C614 8001BA14 AFBF001C */  sw         $ra, 0x1C($sp)
    /* 1C618 8001BA18 AFA40038 */  sw         $a0, 0x38($sp)
    /* 1C61C 8001BA1C 14A00003 */  bnez       $a1, .L8001BA2C
    /* 1C620 8001BA20 AFA70044 */   sw        $a3, 0x44($sp)
    /* 1C624 8001BA24 24A50001 */  addiu      $a1, $a1, 0x1
    /* 1C628 8001BA28 30A5FFFF */  andi       $a1, $a1, 0xFFFF
  .L8001BA2C:
    /* 1C62C 8001BA2C 27A40034 */  addiu      $a0, $sp, 0x34
    /* 1C630 8001BA30 A7A5003E */  sh         $a1, 0x3E($sp)
    /* 1C634 8001BA34 A3A60043 */  sb         $a2, 0x43($sp)
    /* 1C638 8001BA38 0C007A4C */  jal        func_8001E930
    /* 1C63C 8001BA3C AFA50034 */   sw        $a1, 0x34($sp)
    /* 1C640 8001BA40 93A60043 */  lbu        $a2, 0x43($sp)
    /* 1C644 8001BA44 97A5003E */  lhu        $a1, 0x3E($sp)
    /* 1C648 8001BA48 8FA80034 */  lw         $t0, 0x34($sp)
    /* 1C64C 8001BA4C 24CEFF06 */  addiu      $t6, $a2, -0xFA
    /* 1C650 8001BA50 2DC10006 */  sltiu      $at, $t6, 0x6
    /* 1C654 8001BA54 102000B9 */  beqz       $at, .L8001BD3C
    /* 1C658 8001BA58 00101C00 */   sll       $v1, $s0, 16
    /* 1C65C 8001BA5C 000E7080 */  sll        $t6, $t6, 2
    /* 1C660 8001BA60 3C018003 */  lui        $at, %hi(jtbl_8002D8D0_main)
    /* 1C664 8001BA64 002E0821 */  addu       $at, $at, $t6
    /* 1C668 8001BA68 8C2ED8D0 */  lw         $t6, %lo(jtbl_8002D8D0_main)($at)
    /* 1C66C 8001BA6C 01C00008 */  jr         $t6
    /* 1C670 8001BA70 00000000 */   nop
    /* 1C674 8001BA74 3C028005 */  lui        $v0, %hi(D_8004F300)
    /* 1C678 8001BA78 3C078005 */  lui        $a3, %hi(D_8004F800)
    /* 1C67C 8001BA7C 24E7F800 */  addiu      $a3, $a3, %lo(D_8004F800)
    /* 1C680 8001BA80 2442F300 */  addiu      $v0, $v0, %lo(D_8004F300)
    /* 1C684 8001BA84 8FA80034 */  lw         $t0, 0x34($sp)
    /* 1C688 8001BA88 24060001 */  addiu      $a2, $zero, 0x1
    /* 1C68C 8001BA8C 2404FFFF */  addiu      $a0, $zero, -0x1
    /* 1C690 8001BA90 90430014 */  lbu        $v1, 0x14($v0)
  .L8001BA94:
    /* 1C694 8001BA94 50600004 */  beql       $v1, $zero, .L8001BAA8
    /* 1C698 8001BA98 8C4F0000 */   lw        $t7, 0x0($v0)
    /* 1C69C 8001BA9C 54C30014 */  bnel       $a2, $v1, .L8001BAF0
    /* 1C6A0 8001BAA0 9043003C */   lbu       $v1, 0x3C($v0)
    /* 1C6A4 8001BAA4 8C4F0000 */  lw         $t7, 0x0($v0)
  .L8001BAA8:
    /* 1C6A8 8001BAA8 00101C00 */  sll        $v1, $s0, 16
    /* 1C6AC 8001BAAC AC430004 */  sw         $v1, 0x4($v0)
    /* 1C6B0 8001BAB0 006FC023 */  subu       $t8, $v1, $t7
    /* 1C6B4 8001BAB4 0305001A */  div        $zero, $t8, $a1
    /* 1C6B8 8001BAB8 0000C812 */  mflo       $t9
    /* 1C6BC 8001BABC AC48000C */  sw         $t0, 0xC($v0)
    /* 1C6C0 8001BAC0 14A00002 */  bnez       $a1, .L8001BACC
    /* 1C6C4 8001BAC4 00000000 */   nop
    /* 1C6C8 8001BAC8 0007000D */  break      7
  .L8001BACC:
    /* 1C6CC 8001BACC 2401FFFF */  addiu      $at, $zero, -0x1
    /* 1C6D0 8001BAD0 14A10004 */  bne        $a1, $at, .L8001BAE4
    /* 1C6D4 8001BAD4 3C018000 */   lui       $at, (0x80000000 >> 16)
    /* 1C6D8 8001BAD8 17010002 */  bne        $t8, $at, .L8001BAE4
    /* 1C6DC 8001BADC 00000000 */   nop
    /* 1C6E0 8001BAE0 0006000D */  break      6
  .L8001BAE4:
    /* 1C6E4 8001BAE4 AC590008 */  sw         $t9, 0x8($v0)
    /* 1C6E8 8001BAE8 AC440010 */  sw         $a0, 0x10($v0)
    /* 1C6EC 8001BAEC 9043003C */  lbu        $v1, 0x3C($v0)
  .L8001BAF0:
    /* 1C6F0 8001BAF0 50600004 */  beql       $v1, $zero, .L8001BB04
    /* 1C6F4 8001BAF4 8C4A0028 */   lw        $t2, 0x28($v0)
    /* 1C6F8 8001BAF8 54C30014 */  bnel       $a2, $v1, .L8001BB4C
    /* 1C6FC 8001BAFC 24420050 */   addiu     $v0, $v0, 0x50
    /* 1C700 8001BB00 8C4A0028 */  lw         $t2, 0x28($v0)
  .L8001BB04:
    /* 1C704 8001BB04 00101C00 */  sll        $v1, $s0, 16
    /* 1C708 8001BB08 AC43002C */  sw         $v1, 0x2C($v0)
    /* 1C70C 8001BB0C 006A5823 */  subu       $t3, $v1, $t2
    /* 1C710 8001BB10 0165001A */  div        $zero, $t3, $a1
    /* 1C714 8001BB14 00006012 */  mflo       $t4
    /* 1C718 8001BB18 AC4C0030 */  sw         $t4, 0x30($v0)
    /* 1C71C 8001BB1C AC480034 */  sw         $t0, 0x34($v0)
    /* 1C720 8001BB20 14A00002 */  bnez       $a1, .L8001BB2C
    /* 1C724 8001BB24 00000000 */   nop
    /* 1C728 8001BB28 0007000D */  break      7
  .L8001BB2C:
    /* 1C72C 8001BB2C 2401FFFF */  addiu      $at, $zero, -0x1
    /* 1C730 8001BB30 14A10004 */  bne        $a1, $at, .L8001BB44
    /* 1C734 8001BB34 3C018000 */   lui       $at, (0x80000000 >> 16)
    /* 1C738 8001BB38 15610002 */  bne        $t3, $at, .L8001BB44
    /* 1C73C 8001BB3C 00000000 */   nop
    /* 1C740 8001BB40 0006000D */  break      6
  .L8001BB44:
    /* 1C744 8001BB44 AC440038 */  sw         $a0, 0x38($v0)
    /* 1C748 8001BB48 24420050 */  addiu      $v0, $v0, 0x50
  .L8001BB4C:
    /* 1C74C 8001BB4C 5447FFD1 */  bnel       $v0, $a3, .L8001BA94
    /* 1C750 8001BB50 90430014 */   lbu       $v1, 0x14($v0)
    /* 1C754 8001BB54 10000093 */  b          .L8001BDA4
    /* 1C758 8001BB58 AFA80034 */   sw        $t0, 0x34($sp)
    /* 1C75C 8001BB5C 3C028005 */  lui        $v0, %hi(D_8004F300)
    /* 1C760 8001BB60 3C098005 */  lui        $t1, %hi(D_8004F800)
    /* 1C764 8001BB64 2529F800 */  addiu      $t1, $t1, %lo(D_8004F800)
    /* 1C768 8001BB68 2442F300 */  addiu      $v0, $v0, %lo(D_8004F300)
    /* 1C76C 8001BB6C 8FA80034 */  lw         $t0, 0x34($sp)
    /* 1C770 8001BB70 24070003 */  addiu      $a3, $zero, 0x3
    /* 1C774 8001BB74 24060002 */  addiu      $a2, $zero, 0x2
    /* 1C778 8001BB78 2404FFFF */  addiu      $a0, $zero, -0x1
    /* 1C77C 8001BB7C 90430014 */  lbu        $v1, 0x14($v0)
  .L8001BB80:
    /* 1C780 8001BB80 50C30004 */  beql       $a2, $v1, .L8001BB94
    /* 1C784 8001BB84 8C4D0000 */   lw        $t5, 0x0($v0)
    /* 1C788 8001BB88 54E30014 */  bnel       $a3, $v1, .L8001BBDC
    /* 1C78C 8001BB8C 9043003C */   lbu       $v1, 0x3C($v0)
    /* 1C790 8001BB90 8C4D0000 */  lw         $t5, 0x0($v0)
  .L8001BB94:
    /* 1C794 8001BB94 00101C00 */  sll        $v1, $s0, 16
    /* 1C798 8001BB98 AC430004 */  sw         $v1, 0x4($v0)
    /* 1C79C 8001BB9C 006D7023 */  subu       $t6, $v1, $t5
    /* 1C7A0 8001BBA0 01C5001A */  div        $zero, $t6, $a1
    /* 1C7A4 8001BBA4 00007812 */  mflo       $t7
    /* 1C7A8 8001BBA8 AC48000C */  sw         $t0, 0xC($v0)
    /* 1C7AC 8001BBAC 14A00002 */  bnez       $a1, .L8001BBB8
    /* 1C7B0 8001BBB0 00000000 */   nop
    /* 1C7B4 8001BBB4 0007000D */  break      7
  .L8001BBB8:
    /* 1C7B8 8001BBB8 2401FFFF */  addiu      $at, $zero, -0x1
    /* 1C7BC 8001BBBC 14A10004 */  bne        $a1, $at, .L8001BBD0
    /* 1C7C0 8001BBC0 3C018000 */   lui       $at, (0x80000000 >> 16)
    /* 1C7C4 8001BBC4 15C10002 */  bne        $t6, $at, .L8001BBD0
    /* 1C7C8 8001BBC8 00000000 */   nop
    /* 1C7CC 8001BBCC 0006000D */  break      6
  .L8001BBD0:
    /* 1C7D0 8001BBD0 AC4F0008 */  sw         $t7, 0x8($v0)
    /* 1C7D4 8001BBD4 AC440010 */  sw         $a0, 0x10($v0)
    /* 1C7D8 8001BBD8 9043003C */  lbu        $v1, 0x3C($v0)
  .L8001BBDC:
    /* 1C7DC 8001BBDC 50C30004 */  beql       $a2, $v1, .L8001BBF0
    /* 1C7E0 8001BBE0 8C580028 */   lw        $t8, 0x28($v0)
    /* 1C7E4 8001BBE4 54E30014 */  bnel       $a3, $v1, .L8001BC38
    /* 1C7E8 8001BBE8 24420050 */   addiu     $v0, $v0, 0x50
    /* 1C7EC 8001BBEC 8C580028 */  lw         $t8, 0x28($v0)
  .L8001BBF0:
    /* 1C7F0 8001BBF0 00101C00 */  sll        $v1, $s0, 16
    /* 1C7F4 8001BBF4 AC43002C */  sw         $v1, 0x2C($v0)
    /* 1C7F8 8001BBF8 0078C823 */  subu       $t9, $v1, $t8
    /* 1C7FC 8001BBFC 0325001A */  div        $zero, $t9, $a1
    /* 1C800 8001BC00 00005012 */  mflo       $t2
    /* 1C804 8001BC04 AC4A0030 */  sw         $t2, 0x30($v0)
    /* 1C808 8001BC08 AC480034 */  sw         $t0, 0x34($v0)
    /* 1C80C 8001BC0C 14A00002 */  bnez       $a1, .L8001BC18
    /* 1C810 8001BC10 00000000 */   nop
    /* 1C814 8001BC14 0007000D */  break      7
  .L8001BC18:
    /* 1C818 8001BC18 2401FFFF */  addiu      $at, $zero, -0x1
    /* 1C81C 8001BC1C 14A10004 */  bne        $a1, $at, .L8001BC30
    /* 1C820 8001BC20 3C018000 */   lui       $at, (0x80000000 >> 16)
    /* 1C824 8001BC24 17210002 */  bne        $t9, $at, .L8001BC30
    /* 1C828 8001BC28 00000000 */   nop
    /* 1C82C 8001BC2C 0006000D */  break      6
  .L8001BC30:
    /* 1C830 8001BC30 AC440038 */  sw         $a0, 0x38($v0)
    /* 1C834 8001BC34 24420050 */  addiu      $v0, $v0, 0x50
  .L8001BC38:
    /* 1C838 8001BC38 5449FFD1 */  bnel       $v0, $t1, .L8001BB80
    /* 1C83C 8001BC3C 90430014 */   lbu       $v1, 0x14($v0)
    /* 1C840 8001BC40 10000058 */  b          .L8001BDA4
    /* 1C844 8001BC44 AFA80034 */   sw        $t0, 0x34($sp)
    /* 1C848 8001BC48 10000006 */  b          .L8001BC64
    /* 1C84C 8001BC4C 24030002 */   addiu     $v1, $zero, 0x2
    /* 1C850 8001BC50 10000004 */  b          .L8001BC64
    /* 1C854 8001BC54 24030003 */   addiu     $v1, $zero, 0x3
    /* 1C858 8001BC58 10000002 */  b          .L8001BC64
    /* 1C85C 8001BC5C 00001825 */   or        $v1, $zero, $zero
    /* 1C860 8001BC60 24030001 */  addiu      $v1, $zero, 0x1
  .L8001BC64:
    /* 1C864 8001BC64 3C028005 */  lui        $v0, %hi(D_8004F300)
    /* 1C868 8001BC68 3C078005 */  lui        $a3, %hi(D_8004F800)
    /* 1C86C 8001BC6C 24E7F800 */  addiu      $a3, $a3, %lo(D_8004F800)
    /* 1C870 8001BC70 2442F300 */  addiu      $v0, $v0, %lo(D_8004F300)
    /* 1C874 8001BC74 00603025 */  or         $a2, $v1, $zero
    /* 1C878 8001BC78 8FA80034 */  lw         $t0, 0x34($sp)
    /* 1C87C 8001BC7C 2404FFFF */  addiu      $a0, $zero, -0x1
    /* 1C880 8001BC80 904B0014 */  lbu        $t3, 0x14($v0)
  .L8001BC84:
    /* 1C884 8001BC84 00101C00 */  sll        $v1, $s0, 16
    /* 1C888 8001BC88 54CB0013 */  bnel       $a2, $t3, .L8001BCD8
    /* 1C88C 8001BC8C 904F003C */   lbu       $t7, 0x3C($v0)
    /* 1C890 8001BC90 8C4C0000 */  lw         $t4, 0x0($v0)
    /* 1C894 8001BC94 AC430004 */  sw         $v1, 0x4($v0)
    /* 1C898 8001BC98 AC48000C */  sw         $t0, 0xC($v0)
    /* 1C89C 8001BC9C 006C6823 */  subu       $t5, $v1, $t4
    /* 1C8A0 8001BCA0 01A5001A */  div        $zero, $t5, $a1
    /* 1C8A4 8001BCA4 00007012 */  mflo       $t6
    /* 1C8A8 8001BCA8 AC4E0008 */  sw         $t6, 0x8($v0)
    /* 1C8AC 8001BCAC 14A00002 */  bnez       $a1, .L8001BCB8
    /* 1C8B0 8001BCB0 00000000 */   nop
    /* 1C8B4 8001BCB4 0007000D */  break      7
  .L8001BCB8:
    /* 1C8B8 8001BCB8 2401FFFF */  addiu      $at, $zero, -0x1
    /* 1C8BC 8001BCBC 14A10004 */  bne        $a1, $at, .L8001BCD0
    /* 1C8C0 8001BCC0 3C018000 */   lui       $at, (0x80000000 >> 16)
    /* 1C8C4 8001BCC4 15A10002 */  bne        $t5, $at, .L8001BCD0
    /* 1C8C8 8001BCC8 00000000 */   nop
    /* 1C8CC 8001BCCC 0006000D */  break      6
  .L8001BCD0:
    /* 1C8D0 8001BCD0 AC440010 */  sw         $a0, 0x10($v0)
    /* 1C8D4 8001BCD4 904F003C */  lbu        $t7, 0x3C($v0)
  .L8001BCD8:
    /* 1C8D8 8001BCD8 00101C00 */  sll        $v1, $s0, 16
    /* 1C8DC 8001BCDC 54CF0013 */  bnel       $a2, $t7, .L8001BD2C
    /* 1C8E0 8001BCE0 24420050 */   addiu     $v0, $v0, 0x50
    /* 1C8E4 8001BCE4 8C580028 */  lw         $t8, 0x28($v0)
    /* 1C8E8 8001BCE8 AC43002C */  sw         $v1, 0x2C($v0)
    /* 1C8EC 8001BCEC AC480034 */  sw         $t0, 0x34($v0)
    /* 1C8F0 8001BCF0 0078C823 */  subu       $t9, $v1, $t8
    /* 1C8F4 8001BCF4 0325001A */  div        $zero, $t9, $a1
    /* 1C8F8 8001BCF8 00005012 */  mflo       $t2
    /* 1C8FC 8001BCFC AC4A0030 */  sw         $t2, 0x30($v0)
    /* 1C900 8001BD00 14A00002 */  bnez       $a1, .L8001BD0C
    /* 1C904 8001BD04 00000000 */   nop
    /* 1C908 8001BD08 0007000D */  break      7
  .L8001BD0C:
    /* 1C90C 8001BD0C 2401FFFF */  addiu      $at, $zero, -0x1
    /* 1C910 8001BD10 14A10004 */  bne        $a1, $at, .L8001BD24
    /* 1C914 8001BD14 3C018000 */   lui       $at, (0x80000000 >> 16)
    /* 1C918 8001BD18 17210002 */  bne        $t9, $at, .L8001BD24
    /* 1C91C 8001BD1C 00000000 */   nop
    /* 1C920 8001BD20 0006000D */  break      6
  .L8001BD24:
    /* 1C924 8001BD24 AC440038 */  sw         $a0, 0x38($v0)
    /* 1C928 8001BD28 24420050 */  addiu      $v0, $v0, 0x50
  .L8001BD2C:
    /* 1C92C 8001BD2C 5447FFD5 */  bnel       $v0, $a3, .L8001BC84
    /* 1C930 8001BD30 904B0014 */   lbu       $t3, 0x14($v0)
    /* 1C934 8001BD34 1000001B */  b          .L8001BDA4
    /* 1C938 8001BD38 AFA80034 */   sw        $t0, 0x34($sp)
  .L8001BD3C:
    /* 1C93C 8001BD3C 00065880 */  sll        $t3, $a2, 2
    /* 1C940 8001BD40 01665821 */  addu       $t3, $t3, $a2
    /* 1C944 8001BD44 3C0C8005 */  lui        $t4, %hi(D_8004F300)
    /* 1C948 8001BD48 258CF300 */  addiu      $t4, $t4, %lo(D_8004F300)
    /* 1C94C 8001BD4C 000B58C0 */  sll        $t3, $t3, 3
    /* 1C950 8001BD50 016C1021 */  addu       $v0, $t3, $t4
    /* 1C954 8001BD54 8C4D0000 */  lw         $t5, 0x0($v0)
    /* 1C958 8001BD58 93B80047 */  lbu        $t8, 0x47($sp)
    /* 1C95C 8001BD5C 8FB90048 */  lw         $t9, 0x48($sp)
    /* 1C960 8001BD60 006D7023 */  subu       $t6, $v1, $t5
    /* 1C964 8001BD64 01C5001A */  div        $zero, $t6, $a1
    /* 1C968 8001BD68 00007812 */  mflo       $t7
    /* 1C96C 8001BD6C AC430004 */  sw         $v1, 0x4($v0)
    /* 1C970 8001BD70 AC48000C */  sw         $t0, 0xC($v0)
    /* 1C974 8001BD74 14A00002 */  bnez       $a1, .L8001BD80
    /* 1C978 8001BD78 00000000 */   nop
    /* 1C97C 8001BD7C 0007000D */  break      7
  .L8001BD80:
    /* 1C980 8001BD80 2401FFFF */  addiu      $at, $zero, -0x1
    /* 1C984 8001BD84 14A10004 */  bne        $a1, $at, .L8001BD98
    /* 1C988 8001BD88 3C018000 */   lui       $at, (0x80000000 >> 16)
    /* 1C98C 8001BD8C 15C10002 */  bne        $t6, $at, .L8001BD98
    /* 1C990 8001BD90 00000000 */   nop
    /* 1C994 8001BD94 0006000D */  break      6
  .L8001BD98:
    /* 1C998 8001BD98 AC4F0008 */  sw         $t7, 0x8($v0)
    /* 1C99C 8001BD9C A0580015 */  sb         $t8, 0x15($v0)
    /* 1C9A0 8001BDA0 AC590010 */  sw         $t9, 0x10($v0)
  .L8001BDA4:
    /* 1C9A4 8001BDA4 8FBF001C */  lw         $ra, 0x1C($sp)
    /* 1C9A8 8001BDA8 8FB00018 */  lw         $s0, 0x18($sp)
    /* 1C9AC 8001BDAC 27BD0038 */  addiu      $sp, $sp, 0x38
    /* 1C9B0 8001BDB0 03E00008 */  jr         $ra
    /* 1C9B4 8001BDB4 00000000 */   nop
endlabel func_8001B9F8
