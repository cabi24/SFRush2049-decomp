# Source: F160.s
# Address: 0x8000E5F4

glabel func_8000E5F4
    /* F1F4 8000E5F4 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* F1F8 8000E5F8 AFBF0014 */  sw         $ra, 0x14($sp)
    /* F1FC 8000E5FC 3C048003 */  lui        $a0, %hi(__osSiMesg)
    /* F200 8000E600 24847C98 */  addiu      $a0, $a0, %lo(__osSiMesg)
    /* F204 8000E604 00002825 */  or         $a1, $zero, $zero
    /* F208 8000E608 0C001D78 */  jal        func_800075E0
    /* F20C 8000E60C 00003025 */   or        $a2, $zero, $zero
    /* F210 8000E610 8FBF0014 */  lw         $ra, 0x14($sp)
    /* F214 8000E614 27BD0018 */  addiu      $sp, $sp, 0x18
    /* F218 8000E618 03E00008 */  jr         $ra
    /* F21C 8000E61C 00000000 */   nop