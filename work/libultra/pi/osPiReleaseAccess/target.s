# Source: E7C0.s
# Address: 0x8000DC54

glabel func_8000DC54
    /* E854 8000DC54 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* E858 8000DC58 AFBF0014 */  sw         $ra, 0x14($sp)
    /* E85C 8000DC5C 3C048003 */  lui        $a0, %hi(D_80037C78)
    /* E860 8000DC60 24847C78 */  addiu      $a0, $a0, %lo(D_80037C78)
    /* E864 8000DC64 00002825 */  or         $a1, $zero, $zero
    /* E868 8000DC68 0C001D78 */  jal        func_800075E0
    /* E86C 8000DC6C 00003025 */   or        $a2, $zero, $zero
    /* E870 8000DC70 8FBF0014 */  lw         $ra, 0x14($sp)
    /* E874 8000DC74 27BD0018 */  addiu      $sp, $sp, 0x18
    /* E878 8000DC78 03E00008 */  jr         $ra
    /* E87C 8000DC7C 00000000 */   nop