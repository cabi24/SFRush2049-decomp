# Source: 5610.s
# Address: 0x80004D50

glabel func_80004D50
    /* 5950 80004D50 3C018003 */  lui        $at, %hi(D_800354C4)
    /* 5954 80004D54 AC2454C4 */  sw         $a0, %lo(D_800354C4)($at)
    /* 5958 80004D58 3C018003 */  lui        $at, %hi(D_800354CC)
    /* 595C 80004D5C AC2554CC */  sw         $a1, %lo(D_800354CC)($at)
    /* 5960 80004D60 3C018003 */  lui        $at, %hi(D_800354C8)
    /* 5964 80004D64 03E00008 */  jr         $ra
    /* 5968 80004D68 AC2054C8 */   sw        $zero, %lo(D_800354C8)($at)