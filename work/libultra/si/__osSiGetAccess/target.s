# Source: F160.s
# Address: 0x8000E5B0

glabel func_8000E5B0
    /* F1B0 8000E5B0 3C0E8003 */  lui        $t6, %hi(__osSiInitialized)
    /* F1B4 8000E5B4 8DCEC4B0 */  lw         $t6, %lo(__osSiInitialized)($t6)
    /* F1B8 8000E5B8 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* F1BC 8000E5BC AFBF0014 */  sw         $ra, 0x14($sp)
    /* F1C0 8000E5C0 15C00003 */  bnez       $t6, .L8000E5D0
    /* F1C4 8000E5C4 00000000 */   nop
    /* F1C8 8000E5C8 0C003958 */  jal        func_8000E560
    /* F1CC 8000E5CC 00000000 */   nop
  .L8000E5D0:
    /* F1D0 8000E5D0 3C048003 */  lui        $a0, %hi(__osSiMesg)
    /* F1D4 8000E5D4 24847C98 */  addiu      $a0, $a0, %lo(__osSiMesg)
    /* F1D8 8000E5D8 27A5001C */  addiu      $a1, $sp, 0x1C
    /* F1DC 8000E5DC 0C001C9C */  jal        func_80007270
    /* F1E0 8000E5E0 24060001 */   addiu     $a2, $zero, 0x1
    /* F1E4 8000E5E4 8FBF0014 */  lw         $ra, 0x14($sp)
    /* F1E8 8000E5E8 27BD0020 */  addiu      $sp, $sp, 0x20
    /* F1EC 8000E5EC 03E00008 */  jr         $ra
    /* F1F0 8000E5F0 00000000 */   nop