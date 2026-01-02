# Source: E7C0.s
# Address: 0x8000DC10

glabel func_8000DC10
    /* E810 8000DC10 3C0E8003 */  lui        $t6, %hi(D_8002C4A0)
    /* E814 8000DC14 8DCEC4A0 */  lw         $t6, %lo(D_8002C4A0)($t6)
    /* E818 8000DC18 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* E81C 8000DC1C AFBF0014 */  sw         $ra, 0x14($sp)
    /* E820 8000DC20 15C00003 */  bnez       $t6, .L8000DC30
    /* E824 8000DC24 00000000 */   nop
    /* E828 8000DC28 0C0036F0 */  jal        func_8000DBC0
    /* E82C 8000DC2C 00000000 */   nop
  .L8000DC30:
    /* E830 8000DC30 3C048003 */  lui        $a0, %hi(D_80037C78)
    /* E834 8000DC34 24847C78 */  addiu      $a0, $a0, %lo(D_80037C78)
    /* E838 8000DC38 27A5001C */  addiu      $a1, $sp, 0x1C
    /* E83C 8000DC3C 0C001C9C */  jal        func_80007270
    /* E840 8000DC40 24060001 */   addiu     $a2, $zero, 0x1
    /* E844 8000DC44 8FBF0014 */  lw         $ra, 0x14($sp)
    /* E848 8000DC48 27BD0020 */  addiu      $sp, $sp, 0x20
    /* E84C 8000DC4C 03E00008 */  jr         $ra
    /* E850 8000DC50 00000000 */   nop