/*
 * func_8010A53C (0x8010A53C, 616 B): credits page text (attract). GC counterpart by strings: hud.c
 * animCreditText/ShowHUD credit-name table (41 names "ABRAHAM ARANDA"...). Hidden unless D_801174B4 bit 3
 * and D_80138660; the page corner D_801543A0 (0..3) picks x = 30 or 290 - D_801543A8 and y = 30 or
 * 155 - 10 * D_801543B0; draws the title strings[lang + D_80154394] in colour 14, then D_801543B0 names
 * of page D_80154394 (D_80114A0C, 11 pointers per page) 10 px apart in colour 1.
 * Shaping (disclosed): font_set_A53C is the inlined font-switch helper (non-static so umerge leaves no
 * deleted-procedure stub); its returned old font is the retail `move s1,v0`. Declaration order x before
 * hide places x's spill home at 60(sp). `i = 0; y += 15; for (;;)` gives retail's addiu order.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct { u32 opaque[6]; } OSMesgQueue;

typedef struct Blit {
    u8 pad0[0x1A];
    s8 hidden;
} Blit;

typedef struct LangInfo {
    u16 lang;
} LangInfo;

typedef struct StrTable {
    u8 pad0[12];
    LangInfo *info;
    char **strings;
} StrTable;

typedef struct CreditPage {
    char *names[11];
} CreditPage;

extern OSMesgQueue D_801461D0;
extern u32 D_801174B4;
extern s32 D_80138660;
extern s32 D_801543A0;
extern s32 D_801543A8;
extern s32 D_801543B0;
extern s32 D_80154394;
extern StrTable D_8017A4E0;
extern CreditPage D_80114A0C[];

s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osSendMesg(OSMesgQueue *, void *, s32);
s32 slot_state_setup(s32 selection);
void Input_ApplyPadConfig(void *);
void dispatch_handler(s32);
void state_utility(s16, s16, void *);

s32 font_set_A53C(s32 font)
{
    s32 old;

    osRecvMesg(&D_801461D0, (void *)0, 1);
    old = slot_state_setup(font);
    osSendMesg(&D_801461D0, (void *)0, 0);
    return old;
}

s32 func_8010A53C(Blit *blt)
{
    s32 x;
    s32 hide;
    s32 y;
    s16 sx;
    s32 i;

    hide = !(D_801174B4 & 8) || !D_80138660;
    if (blt->hidden != hide) {
        blt->hidden = hide;
        Input_ApplyPadConfig(blt);
    }
    if (blt->hidden) {
        return 1;
    }
    switch (D_801543A0) {
    case 0:
        x = 30;
        y = 30;
        break;
    case 1:
        x = 290 - D_801543A8;
        y = 30;
        break;
    case 2:
        x = 30;
        y = 155 - D_801543B0 * 10;
        break;
    default:
        x = 290 - D_801543A8;
        y = 155 - D_801543B0 * 10;
        break;
    }
    font_set_A53C(11);
    dispatch_handler(14);
    sx = x;
    state_utility(sx, y, D_8017A4E0.strings[D_8017A4E0.info->lang + D_80154394]);
    dispatch_handler(1);
    i = 0;
    y += 15;
    for (; i < D_801543B0; i++) {
        state_utility(sx, y, D_80114A0C[D_80154394].names[i]);
        y += 10;
    }
    return 1;
}
