nonmatching __osPiReadDeviceType, 0x9C

glabel __osPiReadDeviceType
    /* 8D28 80008128 240E0007 */  addiu      $t6, $zero, 0x7
    /* 8D2C 8000812C 3C018003 */  lui        $at, %hi(gSpTaskFlags0)
    /* 8D30 80008130 A02E67E4 */  sb         $t6, %lo(gSpTaskFlags0)($at)
    /* 8D34 80008134 3C0FA460 */  lui        $t7, %hi(PI_BSD_DOM1_LAT_REG)
    /* 8D38 80008138 8DF80014 */  lw         $t8, %lo(PI_BSD_DOM1_LAT_REG)($t7)
    /* 8D3C 8000813C 3C018003 */  lui        $at, %hi(gSpTaskFlags1)
    /* 8D40 80008140 3C19A460 */  lui        $t9, %hi(PI_BSD_DOM1_PWD_REG)
    /* 8D44 80008144 A03867E5 */  sb         $t8, %lo(gSpTaskFlags1)($at)
    /* 8D48 80008148 8F280018 */  lw         $t0, %lo(PI_BSD_DOM1_PWD_REG)($t9)
    /* 8D4C 8000814C 3C018003 */  lui        $at, %hi(gSpTaskFlags4)
    /* 8D50 80008150 3C09A460 */  lui        $t1, %hi(PI_BSD_DOM1_PGS_REG)
    /* 8D54 80008154 A02867E8 */  sb         $t0, %lo(gSpTaskFlags4)($at)
    /* 8D58 80008158 8D2A001C */  lw         $t2, %lo(PI_BSD_DOM1_PGS_REG)($t1)
    /* 8D5C 8000815C 3C018003 */  lui        $at, %hi(gSpTaskFlags2)
    /* 8D60 80008160 3C0BA460 */  lui        $t3, %hi(PI_BSD_DOM1_RLS_REG)
    /* 8D64 80008164 A02A67E6 */  sb         $t2, %lo(gSpTaskFlags2)($at)
    /* 8D68 80008168 8D6C0020 */  lw         $t4, %lo(PI_BSD_DOM1_RLS_REG)($t3)
    /* 8D6C 8000816C 3C018003 */  lui        $at, %hi(gSpTaskFlags3)
    /* 8D70 80008170 240D0007 */  addiu      $t5, $zero, 0x7
    /* 8D74 80008174 A02C67E7 */  sb         $t4, %lo(gSpTaskFlags3)($at)
    /* 8D78 80008178 3C018003 */  lui        $at, %hi(gSpTaskResultA)
    /* 8D7C 8000817C A02D685C */  sb         $t5, %lo(gSpTaskResultA)($at)
    /* 8D80 80008180 3C0EA460 */  lui        $t6, %hi(PI_BSD_DOM2_LAT_REG)
    /* 8D84 80008184 8DCF0024 */  lw         $t7, %lo(PI_BSD_DOM2_LAT_REG)($t6)
    /* 8D88 80008188 3C018003 */  lui        $at, %hi(gSpTaskResultB)
    /* 8D8C 8000818C 3C18A460 */  lui        $t8, %hi(PI_BSD_DOM2_LWD_REG)
    /* 8D90 80008190 A02F685D */  sb         $t7, %lo(gSpTaskResultB)($at)
    /* 8D94 80008194 8F190028 */  lw         $t9, %lo(PI_BSD_DOM2_LWD_REG)($t8)
    /* 8D98 80008198 3C018003 */  lui        $at, %hi(gSpTaskResultE)
    /* 8D9C 8000819C 3C08A460 */  lui        $t0, %hi(PI_BSD_DOM2_PGS_REG)
    /* 8DA0 800081A0 A0396860 */  sb         $t9, %lo(gSpTaskResultE)($at)
    /* 8DA4 800081A4 8D09002C */  lw         $t1, %lo(PI_BSD_DOM2_PGS_REG)($t0)
    /* 8DA8 800081A8 3C018003 */  lui        $at, %hi(gSpTaskResultC)
    /* 8DAC 800081AC 3C0AA460 */  lui        $t2, %hi(PI_BSD_DOM2_RLS_REG)
    /* 8DB0 800081B0 A029685E */  sb         $t1, %lo(gSpTaskResultC)($at)
    /* 8DB4 800081B4 8D4B0030 */  lw         $t3, %lo(PI_BSD_DOM2_RLS_REG)($t2)
    /* 8DB8 800081B8 3C018003 */  lui        $at, %hi(gSpTaskResultD)
    /* 8DBC 800081BC 03E00008 */  jr         $ra
    /* 8DC0 800081C0 A02B685F */   sb        $t3, %lo(gSpTaskResultD)($at)
endlabel __osPiReadDeviceType
    /* 8DC4 800081C4 00000000 */  nop
    /* 8DC8 800081C8 00000000 */  nop
    /* 8DCC 800081CC 00000000 */  nop
