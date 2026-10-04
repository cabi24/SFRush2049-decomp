nonmatching func_80010C68, 0xD4

glabel func_80010C68
    /* 11868 80010C68 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* 1186C 80010C6C AFA40020 */  sw         $a0, 0x20($sp)
    /* 11870 80010C70 AFBF001C */  sw         $ra, 0x1C($sp)
    /* 11874 80010C74 240E0001 */  addiu      $t6, $zero, 0x1
    /* 11878 80010C78 3C018004 */  lui        $at, %hi(D_80038220)
    /* 1187C 80010C7C 3C048004 */  lui        $a0, %hi(D_800381F8)
    /* 11880 80010C80 3C058004 */  lui        $a1, %hi(D_80038210)
    /* 11884 80010C84 A02E8220 */  sb         $t6, %lo(D_80038220)($at)
    /* 11888 80010C88 24A58210 */  addiu      $a1, $a1, %lo(D_80038210)
    /* 1188C 80010C8C 248481F8 */  addiu      $a0, $a0, %lo(D_800381F8)
    /* 11890 80010C90 0C001A80 */  jal        osCreateMesgQueue
    /* 11894 80010C94 24060004 */   addiu     $a2, $zero, 0x4
    /* 11898 80010C98 3C058004 */  lui        $a1, %hi(D_800381F8)
    /* 1189C 80010C9C 3C068004 */  lui        $a2, %hi(D_80038220)
    /* 118A0 80010CA0 24C68220 */  addiu      $a2, $a2, %lo(D_80038220)
    /* 118A4 80010CA4 24A581F8 */  addiu      $a1, $a1, %lo(D_800381F8)
    /* 118A8 80010CA8 0C001B84 */  jal        osSetEventMesgAlt
    /* 118AC 80010CAC 24040006 */   addiu     $a0, $zero, 0x6
    /* 118B0 80010CB0 8FAF0020 */  lw         $t7, 0x20($sp)
    /* 118B4 80010CB4 0C002FC0 */  jal        osAiSetFrequency
    /* 118B8 80010CB8 8DE40000 */   lw        $a0, 0x0($t7)
    /* 118BC 80010CBC 8FB90020 */  lw         $t9, 0x20($sp)
    /* 118C0 80010CC0 3C038004 */  lui        $v1, %hi(D_8003828C)
    /* 118C4 80010CC4 2463828C */  addiu      $v1, $v1, %lo(D_8003828C)
    /* 118C8 80010CC8 AC620000 */  sw         $v0, 0x0($v1)
    /* 118CC 80010CCC AF220000 */  sw         $v0, 0x0($t9)
    /* 118D0 80010CD0 3C198004 */  lui        $t9, %hi(D_80038018)
    /* 118D4 80010CD4 8F398018 */  lw         $t9, %lo(D_80038018)($t9)
    /* 118D8 80010CD8 24040400 */  addiu      $a0, $zero, 0x400
    /* 118DC 80010CDC 24050080 */  addiu      $a1, $zero, 0x80
    /* 118E0 80010CE0 0320F809 */  jalr       $t9
    /* 118E4 80010CE4 00000000 */   nop
    /* 118E8 80010CE8 3C038004 */  lui        $v1, %hi(D_800381F0)
    /* 118EC 80010CEC 246381F0 */  addiu      $v1, $v1, %lo(D_800381F0)
    /* 118F0 80010CF0 3C048004 */  lui        $a0, %hi(D_80038040)
    /* 118F4 80010CF4 3C068001 */  lui        $a2, %hi(func_80010A40)
    /* 118F8 80010CF8 24490400 */  addiu      $t1, $v0, 0x400
    /* 118FC 80010CFC 240A007A */  addiu      $t2, $zero, 0x7A
    /* 11900 80010D00 AC620000 */  sw         $v0, 0x0($v1)
    /* 11904 80010D04 AFAA0014 */  sw         $t2, 0x14($sp)
    /* 11908 80010D08 AFA90010 */  sw         $t1, 0x10($sp)
    /* 1190C 80010D0C 24C60A40 */  addiu      $a2, $a2, %lo(func_80010A40)
    /* 11910 80010D10 24848040 */  addiu      $a0, $a0, %lo(D_80038040)
    /* 11914 80010D14 00002825 */  or         $a1, $zero, $zero
    /* 11918 80010D18 0C001BCC */  jal        osCreateThread
    /* 1191C 80010D1C 00003825 */   or        $a3, $zero, $zero
    /* 11920 80010D20 3C048004 */  lui        $a0, %hi(D_80038040)
    /* 11924 80010D24 0C001C20 */  jal        osStartThread
    /* 11928 80010D28 24848040 */   addiu     $a0, $a0, %lo(D_80038040)
    /* 1192C 80010D2C 8FBF001C */  lw         $ra, 0x1C($sp)
    /* 11930 80010D30 27BD0020 */  addiu      $sp, $sp, 0x20
    /* 11934 80010D34 03E00008 */  jr         $ra
    /* 11938 80010D38 00000000 */   nop
endlabel func_80010C68
