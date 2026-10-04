nonmatching func_80018BEC, 0x40

glabel func_80018BEC
    /* 197EC 80018BEC 3C0E8003 */  lui        $t6, %hi(D_8002C630)
    /* 197F0 80018BF0 91CEC630 */  lbu        $t6, %lo(D_8002C630)($t6)
    /* 197F4 80018BF4 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 197F8 80018BF8 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 197FC 80018BFC 11C00007 */  beqz       $t6, .L80018C1C
    /* 19800 80018C00 00001025 */   or        $v0, $zero, $zero
    /* 19804 80018C04 0C005D91 */  jal        func_80017644
    /* 19808 80018C08 00000000 */   nop
    /* 1980C 80018C0C 24420001 */  addiu      $v0, $v0, 0x1
    /* 19810 80018C10 0002102B */  sltu       $v0, $zero, $v0
    /* 19814 80018C14 10000001 */  b          .L80018C1C
    /* 19818 80018C18 304200FF */   andi      $v0, $v0, 0xFF
  .L80018C1C:
    /* 1981C 80018C1C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 19820 80018C20 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 19824 80018C24 03E00008 */  jr         $ra
    /* 19828 80018C28 00000000 */   nop
endlabel func_80018BEC
