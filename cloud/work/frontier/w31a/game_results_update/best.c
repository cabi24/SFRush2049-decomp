/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * Audio-options screen text (GC audioscr.c), w30a draft.
 * game_results_render (0x800FF298, kept) = GC animText@audioscr.c; game_results_update (0x800FEE04, internal,
 * only caller game_results_render) = GC animButtonText@audioscr.c. Labels are wrong (not results screens).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct OSMesgQueue OSMesgQueue;
typedef struct Color4 { u8 r, g, b, a; } Color4;
typedef struct AuIdx { u8 pad0[32]; u16 songStrings; u8 pad34[44]; u16 optionNames; u16 musicTypes; } AuIdx;
typedef struct TextState { void *word0; char **fixed; void *word8; AuIdx *indices; char **strings; } TextState;
typedef struct Options { u8 pad[12]; s8 sfxVolume; s8 musicVolume; s8 pad14; s8 musicType; } Options;
typedef struct ButtonInfo { u8 pad[8]; f32 angle; u8 pad12[36]; u8 pos[16]; } ButtonInfo;

extern OSMesgQueue D_801461D0;
extern TextState countdown_state;                /* gText */
extern s32 D_801105B4;                           /* buttonsInited */
extern s32 state_word_a;                         /* gGameState */
extern s8 D_8011064C;                            /* gControllerRemoved */
extern s32 D_8015698C;                           /* playerInPause */
extern s32 D_8011065C;                           /* menuRect.x (right edge of the option column) */
extern s32 D_80110638;                           /* gAudioMenuSelect */
extern s32 D_80110640;                           /* gSong */
extern char *D_80110030[];                       /* gSongNames */
extern Options D_80146108;                       /* gOptions */
typedef union TextColor { u8 c[4]; u32 rgba; } TextColor;   /* GC union __anon_21da */
extern TextColor D_801146BC[2];                  /* flashColors */
extern ButtonInfo D_801102B4[];                  /* buttonInfo (N64 copy starts at the first button) */
extern u8 D_801105A4[];                          /* buttonInfo + 0x2f0 (title position) */
extern u8 gCamera[];                          /* gCamera */
extern s32 D_80118E20[2];                        /* TB_SetJust pair */
extern s32 osRecvMesg(OSMesgQueue *, void **, s32), osSendMesg(OSMesgQueue *, void *, s32);
extern s32 slot_state_setup(s32);
extern u32 object_manager_update(char *, s32);   /* HAL_GetTextWidth */
extern s16 object_bytes_sum_global(void);        /* TB_GetFontLineHeight */
extern void dispatch_handler(s32);               /* TB_SetTextColor */
extern void state_utility(s16, s16, char *);     /* TB_DrawText */
extern s32 fcvt_wrapper(char *, char *, ...);    /* TB_Sprintf */
extern void render_helper(f32);                  /* HAL_SetTextDepth */
extern void func_800ED66C(f32);                  /* HAL_SetTextTranslucency */
extern u8 *func_800BE6A4(u8 *, void *);          /* TB_Strcpy */
extern void brake_light_update(s32, void *, void *, void *, s16 *);  /* ZOID_ProjectWindow */
extern TextColor D_80118E28, D_80118E2C;
void func_800BEA3C(TextColor first, TextColor second)   /* TB_FlashTextColors (GC: two union __anon_21da) */
{
    D_80118E28.rgba = first.rgba;
    D_80118E2C.rgba = second.rgba;
}

void au_gfx_lock(void) { osRecvMesg(&D_801461D0, 0, 1); }
void au_gfx_unlock(void) { osSendMesg(&D_801461D0, 0, 0); }
s32 au_font_set(s32 font)
{
    s32 old;
    au_gfx_lock();
    old = slot_state_setup(font);
    au_gfx_unlock();
    return old;
}
s16 au_CenterText(char *text, s16 centerX)
{
    return centerX - (object_manager_update(text, -1) >> 1);
}
s16 au_RightText(char *text, s16 rightX)
{
    return rightX - object_manager_update(text, -1);
}

void au_SetJust(s32 h, s32 v)
{
    D_80118E20[1] = v;
    D_80118E20[0] = h;
}

void au_SetFont(s32 font)
{
    au_font_set(font);
}

s8 au_validOption(s32 i)
{
    return 1;
}

void game_results_update(void)
{
    s32 i;
    s32 x;
    s32 y;
    char str[40];
    s16 screenPos[2];
    char *charPtr;

    render_helper(0.0f);
    au_SetJust(0, 1);
    au_SetFont(13);
    dispatch_handler(1);
    brake_light_update(0, D_801105A4, gCamera, 0, screenPos);
    x = screenPos[0];
    y = screenPos[1];
    state_utility(au_CenterText(countdown_state.fixed[52], x), y, countdown_state.fixed[52]);
    au_font_set(11);
    for (i = 0; i < 4; i++) {
        if (D_801102B4[i].angle > 0.69813168f && D_801102B4[i].angle < 2.443461f) continue;
        if (!au_validOption(i)) continue;
        if (D_801102B4[i].angle > 0.34906584f && D_801102B4[i].angle < 2.7925267f) func_800ED66C(128.0f);
        else func_800ED66C(-1.0f);
        if (D_801102B4[i].angle < 1.5707964f) {
            func_800BEA3C(D_801146BC[0], D_801146BC[1]);
            dispatch_handler(1);
        } else {
            func_800BEA3C(D_801146BC[0], D_801146BC[1]);
            dispatch_handler(22);
        }
        brake_light_update(0, D_801102B4[i].pos, gCamera, 0, screenPos);
        x = screenPos[0];
        y = screenPos[1];
        x -= 55;
        state_utility(x, y,
                      countdown_state.strings[countdown_state.indices->optionNames + i]);
        switch (i) {
        case 0:
            fcvt_wrapper(str, "%d", D_80146108.musicVolume);
            break;
        case 1:
            fcvt_wrapper(str, "%d", D_80146108.sfxVolume);
            break;
        case 2:
            if (D_80110640 == 12) func_800BE6A4((u8 *)str, countdown_state.fixed[34]);
            else if (D_80110640 == 13) func_800BE6A4((u8 *)str, countdown_state.strings[countdown_state.indices->songStrings]);
            else func_800BE6A4((u8 *)str, D_80110030[D_80110640]);
            break;
        case 3:
            func_800BE6A4((u8 *)str, countdown_state.strings[D_80146108.musicType + countdown_state.indices->musicTypes]);
            break;
        }
        for (charPtr = str; *charPtr; charPtr++) {
            if (*charPtr >= 'a' && *charPtr <= 'z') *charPtr = *charPtr ^ 0x20;
            else *charPtr = *charPtr;
        }
        brake_light_update(0, D_801102B4[i + 4].pos, gCamera, 0, screenPos);
        x = screenPos[0];
        y = screenPos[1];
        x -= 35;
        state_utility(x, y, str);
    }
    au_SetJust(0, 3);
    func_800ED66C(-1.0f);
    func_800BEA3C(D_801146BC[0], D_801146BC[1]);
    render_helper(-1.0f);
}


s32 game_results_render(s32 arg0)
{
    s32 i;
    s32 x;
    s32 y;
    char str[40];
    char *charPtr;

    if (D_801105B4 == 1) {
        game_results_update();
        return 1;
    }
    render_helper(0.0f);
    au_SetFont(13);
    dispatch_handler(1);
    if ((state_word_a & 0x7C0000) && D_8011064C) {
        fcvt_wrapper(str, "CONTROLLER %d %s", D_8015698C + 1, countdown_state.fixed[233]);
        state_utility(au_CenterText(str, 160), 10, str);
    } else {
        state_utility(au_CenterText(countdown_state.fixed[52], 160), 10, countdown_state.fixed[52]);
    }
    y = object_bytes_sum_global() * 2 + 10;
    au_font_set(10);
    x = D_8011065C;
    for (i = 0; i < 4; i++) {
        if (!au_validOption(i)) continue;
        dispatch_handler(i == D_80110638 ? 22 : 1);
        state_utility(au_RightText(countdown_state.strings[countdown_state.indices->optionNames + i], x - 5), y,
                      countdown_state.strings[countdown_state.indices->optionNames + i]);
        switch (i) {
        case 0:
            fcvt_wrapper(str, "%d", D_80146108.musicVolume);
            break;
        case 1:
            fcvt_wrapper(str, "%d", D_80146108.sfxVolume);
            break;
        case 2:
            if (D_80110640 == 12) func_800BE6A4((u8 *)str, countdown_state.fixed[34]);
            else if (D_80110640 == 13) func_800BE6A4((u8 *)str, countdown_state.strings[countdown_state.indices->songStrings]);
            else func_800BE6A4((u8 *)str, D_80110030[D_80110640]);
            break;
        case 3:
            func_800BE6A4((u8 *)str, countdown_state.strings[D_80146108.musicType + countdown_state.indices->musicTypes]);
            break;
        }
        for (charPtr = str; *charPtr; charPtr++) {
            if (*charPtr >= 'a' && *charPtr <= 'z') *charPtr = *charPtr ^ 0x20;
            else *charPtr = *charPtr;
        }
        state_utility(x + 5, y, str);
        y += object_bytes_sum_global();
    }
    func_800ED66C(-1.0f);
    func_800BEA3C(D_801146BC[0], D_801146BC[1]);
    render_helper(-1.0f);
    return 1;
}
