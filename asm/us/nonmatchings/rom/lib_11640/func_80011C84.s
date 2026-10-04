nonmatching func_80011C84, 0x54

glabel func_80011C84
    /* 12884 80011C84 3085FFFF */  andi       $a1, $a0, 0xFFFF
    /* 12888 80011C88 00057080 */  sll        $t6, $a1, 2
    /* 1288C 80011C8C 01C57023 */  subu       $t6, $t6, $a1
    /* 12890 80011C90 3C0F8004 */  lui        $t7, %hi(D_80038294)
    /* 12894 80011C94 8DEF8294 */  lw         $t7, %lo(D_80038294)($t7)
    /* 12898 80011C98 000E7080 */  sll        $t6, $t6, 2
    /* 1289C 80011C9C 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 128A0 80011CA0 01C57021 */  addu       $t6, $t6, $a1
    /* 128A4 80011CA4 AFA40020 */  sw         $a0, 0x20($sp)
    /* 128A8 80011CA8 000E70C0 */  sll        $t6, $t6, 3
    /* 128AC 80011CAC AFBF0014 */  sw         $ra, 0x14($sp)
    /* 128B0 80011CB0 01CF2021 */  addu       $a0, $t6, $t7
    /* 128B4 80011CB4 0C004684 */  jal        func_80011A10
    /* 128B8 80011CB8 AFA4001C */   sw        $a0, 0x1C($sp)
    /* 128BC 80011CBC 8FA4001C */  lw         $a0, 0x1C($sp)
    /* 128C0 80011CC0 24180001 */  addiu      $t8, $zero, 0x1
    /* 128C4 80011CC4 A0980000 */  sb         $t8, 0x0($a0)
    /* 128C8 80011CC8 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 128CC 80011CCC 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 128D0 80011CD0 03E00008 */  jr         $ra
    /* 128D4 80011CD4 00000000 */   nop
endlabel func_80011C84
