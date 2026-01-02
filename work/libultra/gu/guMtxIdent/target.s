# Source: 9DE0.s
# Address: 0x8000941C

glabel func_8000941C
    /* A01C 8000941C 27BDFFA8 */  addiu      $sp, $sp, -0x58
    /* A020 80009420 AFBF0014 */  sw         $ra, 0x14($sp)
    /* A024 80009424 AFA40058 */  sw         $a0, 0x58($sp)
    /* A028 80009428 0C0024E5 */  jal        func_80009394
    /* A02C 8000942C 27A40018 */   addiu     $a0, $sp, 0x18
    /* A030 80009430 27A40018 */  addiu      $a0, $sp, 0x18
    /* A034 80009434 0C002478 */  jal        func_800091E0
    /* A038 80009438 8FA50058 */   lw        $a1, 0x58($sp)
    /* A03C 8000943C 8FBF0014 */  lw         $ra, 0x14($sp)
    /* A040 80009440 27BD0058 */  addiu      $sp, $sp, 0x58
    /* A044 80009444 03E00008 */  jr         $ra
    /* A048 80009448 00000000 */   nop