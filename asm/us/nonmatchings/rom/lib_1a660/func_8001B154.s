nonmatching func_8001B154, 0x7C

glabel func_8001B154
    /* 1BD54 8001B154 3C028005 */  lui        $v0, %hi(D_8004F808)
    /* 1BD58 8001B158 8C42F808 */  lw         $v0, %lo(D_8004F808)($v0)
    /* 1BD5C 8001B15C 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 1BD60 8001B160 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 1BD64 8001B164 10400016 */  beqz       $v0, .L8001B1C0
    /* 1BD68 8001B168 00027140 */   sll       $t6, $v0, 5
    /* 1BD6C 8001B16C 01C27023 */  subu       $t6, $t6, $v0
    /* 1BD70 8001B170 000E7080 */  sll        $t6, $t6, 2
    /* 1BD74 8001B174 01C27021 */  addu       $t6, $t6, $v0
    /* 1BD78 8001B178 3C0F8005 */  lui        $t7, %hi(D_8004F800)
    /* 1BD7C 8001B17C 8DEFF800 */  lw         $t7, %lo(D_8004F800)($t7)
    /* 1BD80 8001B180 000E7200 */  sll        $t6, $t6, 8
    /* 1BD84 8001B184 01CF001A */  div        $zero, $t6, $t7
    /* 1BD88 8001B188 15E00002 */  bnez       $t7, .L8001B194
    /* 1BD8C 8001B18C 00000000 */   nop
    /* 1BD90 8001B190 0007000D */  break      7
  .L8001B194:
    /* 1BD94 8001B194 2401FFFF */  addiu      $at, $zero, -0x1
    /* 1BD98 8001B198 15E10004 */  bne        $t7, $at, .L8001B1AC
    /* 1BD9C 8001B19C 3C018000 */   lui       $at, (0x80000000 >> 16)
    /* 1BDA0 8001B1A0 15C10002 */  bne        $t6, $at, .L8001B1AC
    /* 1BDA4 8001B1A4 00000000 */   nop
    /* 1BDA8 8001B1A8 0006000D */  break      6
  .L8001B1AC:
    /* 1BDAC 8001B1AC 0000C012 */  mflo       $t8
    /* 1BDB0 8001B1B0 0018C8C0 */  sll        $t9, $t8, 3
    /* 1BDB4 8001B1B4 3C018005 */  lui        $at, %hi(D_8004BE90)
    /* 1BDB8 8001B1B8 0C006996 */  jal        func_8001A658
    /* 1BDBC 8001B1BC AC39BE90 */   sw        $t9, %lo(D_8004BE90)($at)
  .L8001B1C0:
    /* 1BDC0 8001B1C0 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 1BDC4 8001B1C4 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 1BDC8 8001B1C8 03E00008 */  jr         $ra
    /* 1BDCC 8001B1CC 00000000 */   nop
endlabel func_8001B154
