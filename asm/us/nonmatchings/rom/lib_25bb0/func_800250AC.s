nonmatching func_800250AC, 0x44

glabel func_800250AC
    /* 25CAC 800250AC 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* 25CB0 800250B0 AFBF0014 */  sw         $ra, 0x14($sp)
    /* 25CB4 800250B4 3C048006 */  lui        $a0, %hi(D_800586A8)
    /* 25CB8 800250B8 3C058006 */  lui        $a1, %hi(D_800586C0)
    /* 25CBC 800250BC 24A586C0 */  addiu      $a1, $a1, %lo(D_800586C0)
    /* 25CC0 800250C0 248486A8 */  addiu      $a0, $a0, %lo(D_800586A8)
    /* 25CC4 800250C4 0C001A80 */  jal        osCreateMesgQueue
    /* 25CC8 800250C8 24060001 */   addiu     $a2, $zero, 0x1
    /* 25CCC 800250CC 3C048006 */  lui        $a0, %hi(D_800586A8)
    /* 25CD0 800250D0 248486A8 */  addiu      $a0, $a0, %lo(D_800586A8)
    /* 25CD4 800250D4 00002825 */  or         $a1, $zero, $zero
    /* 25CD8 800250D8 0C001D78 */  jal        osJamMesg
    /* 25CDC 800250DC 00003025 */   or        $a2, $zero, $zero
    /* 25CE0 800250E0 8FBF0014 */  lw         $ra, 0x14($sp)
    /* 25CE4 800250E4 27BD0018 */  addiu      $sp, $sp, 0x18
    /* 25CE8 800250E8 03E00008 */  jr         $ra
    /* 25CEC 800250EC 00000000 */   nop
endlabel func_800250AC
