/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800CCB40: default value of option `selector` (0..40) for the settings initialiser
 * func_800CCCCC (selectors 0..20 then 21..40, each stored through audio_bus_mix) and func_800DE45C.
 * Case 9 is "PAL console": osTvType (0x80000300) == OS_TV_PAL.  Own .rodata: the 41-entry jump table.
 * Case 9 needs `|| <compile-time 0>` to reproduce the bytes; it is written as a compiled-out
 * development switch (DEBUG_FORCE_PAL).  That switch is INFERRED from the bytes, not evidenced: no
 * symbol or string for it survives.  The bare `(D_80000300 == 0) || 0` spelling had been rejected
 * (cloud/work/ipa-groups/codex_switch_a84/STATUS.md); held in wave 4 (w4d), accepted by the owner in
 * wave 10 (2026-10-06) with this disclosure.
 */
typedef signed char s8;
#define OS_TV_PAL 0
#define DEBUG_FORCE_PAL 0 /* compiled-out development switch */
extern unsigned int D_80000300; /* osTvType */
s8 func_800CCB40(int selector)
{
    switch(selector) {
    case 21:return 3;
    case 22:return 5;
    case 0:return 1;
    case 23:return 1;
    case 1:return 1;
    case 2:return 1;
    case 3:return 1;
    case 4:return 1;
    case 5:return 1;
    case 6:return 1;
    case 7:return 1;
    case 8:return 1;
    case 24:return 2;
    case 25:return 0;
    case 26:return 2;
    case 27:return 0;
    case 28:return 0;
    case 29:return 0;
    case 30:return 0;
    case 9:return D_80000300 == OS_TV_PAL || DEBUG_FORCE_PAL;
    case 31:return 0;
    case 10:return 1;
    case 11:return 1;
    case 12:return 10;
    case 13:return 10;
    case 14:return 12;
    case 15:return 0;
    case 16:return 0;
    case 17:return 0;
    case 18:return 1;
    case 19:return 0;
    case 20:return 0;
    case 32:return 0;
    case 33:return 0;
    case 35:return 5;
    case 34:return 69;
    case 36:return 30;
    case 37:return 0;
    case 39:return 8;
    case 38:return 10;
    case 40:return 0;
    default:break;
    }
}
