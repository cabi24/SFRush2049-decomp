nonmatching func_80019370, 0x58

glabel func_80019370
    /* 19F70 80019370 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 19F74 80019374 3C0E8003 */  lui        $t6, %hi(D_8002C630)
    /* 19F78 80019378 91CEC630 */  lbu        $t6, %lo(D_8002C630)($t6)
    /* 19F7C 8001937C AFBF0014 */  sw         $ra, 0x14($sp)
    /* 19F80 80019380 AFA40018 */  sw         $a0, 0x18($sp)
    /* 19F84 80019384 AFA5001C */  sw         $a1, 0x1C($sp)
    /* 19F88 80019388 AFA60020 */  sw         $a2, 0x20($sp)
    /* 19F8C 8001938C 11C0000A */  beqz       $t6, .L800193B8
    /* 19F90 80019390 AFA70024 */   sw        $a3, 0x24($sp)
    /* 19F94 80019394 0C005165 */  jal        func_80014594
    /* 19F98 80019398 00000000 */   nop
    /* 19F9C 8001939C 93A4001B */  lbu        $a0, 0x1B($sp)
    /* 19FA0 800193A0 97A5001E */  lhu        $a1, 0x1E($sp)
    /* 19FA4 800193A4 8FA60020 */  lw         $a2, 0x20($sp)
    /* 19FA8 800193A8 0C006465 */  jal        func_80019194
    /* 19FAC 800193AC 93A70027 */   lbu       $a3, 0x27($sp)
    /* 19FB0 800193B0 0C005177 */  jal        func_800145DC
    /* 19FB4 800193B4 00000000 */   nop
  .L800193B8:
    /* 19FB8 800193B8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 19FBC 800193BC 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 19FC0 800193C0 03E00008 */  jr         $ra
    /* 19FC4 800193C4 00000000 */   nop
endlabel func_80019370
