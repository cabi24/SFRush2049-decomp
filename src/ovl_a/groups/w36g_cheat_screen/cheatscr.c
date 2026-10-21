/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* w36g: cheatscr.c of runtime image A. Direct typed port of reference/repos/mat3
 * decomp/src/GameSource/src/cheatscr.c (cross-checked with reference/repos/dreamcast src/cheatscr.c):
 *   func_8039C910 = updateCheatScreen (GC 0x800675B4, 0x5CC B);
 *   func_8039C908 = initScreen (GC 0x80067B80) and func_8039C900 = uninitScreen (GC 0x80067CE0), both inlined
 *   into func_8039C910 by umerge; their deleted bodies are the caller-less `jr ra; nop` stubs (non-static, off
 *   the keep list, so each stub keeps its symbol; names follow this group's emission order, uninitScreen first).
 * InitEgg@eggs.c (GC 0x80012704) is inlined too; it lives in eggs.c of this packet (its retail stub is one of the
 * eggs.c stubs func_8038F734/func_8038F73C, not claimed). CheckForEgg is the locked func_8038F648.
 * N64 names: gCheatState D_801164A4 (s16[21]), gCheatActive D_8011648C, gSelectedOption D_803B3094,
 * gCheatMenuExit D_803B3098, blit D_803B3090, inited D_803B308C, multiblit D_803B3068, gActiveEdges D_8015694C,
 * gActiveAutoEdges D_80149784, gSoftReset D_80157244; cheat flags (s8): gNoCarCollisions D_8017A4D8, gMineCones
 * D_8013FECC, gCarMines D_8013FECD, gNoAutoAbort D_801403D0, gSuperSpeed D_801427A1, gInvincible D_80142B00,
 * gInvisibleCar D_80140418, gInvisibleTrack D_8014061A, gNoBrakes D_8013F2FC, gSpecialTires D_80142DB0,
 * gMassiveMass D_80143F1C, gSuicideMode D_8017A634, gResurrectInPlace D_801407B0, gFrameScale D_801613AB,
 * gEnableTireScaling D_801407D0, gFogColor D_801427A0. AudPlayEffect entity_flags_apply, PlayLRSound audio_doppler,
 * createBG particle_lifetime_set, NewMultiBlit sound_control, RemoveBlit sound_stop, removeBG ambient_sounds_clear,
 * setGameState func_800B5570.
 * N64 differences read from the retail words: 21 cheats, not 23 (GC 14 gDemolitionBattle and 15 gRandomWeapons are
 * gone: N64 13..16 are state-only toggles, 17..20 = GC 19..22); sound ids 46/38/40/39; InitEgg stores gNextTimeOut
 * before gEggIdx.
 * Shaping, measured: (1) gActiveEdges is `s32` here (with MAT3's unsigned int the edges/dir/option webs rotate
 * through a0/a1/a2: 114 words); (2) initScreen/uninitScreen are defined after updateCheatScreen (MAT3 file order);
 * defined before it, as1 swaps two address words at the join after the init block.
 * Own data: the 21-entry switch table (.rodata), verified locally against build/ovl/a/image.bin (base 0x803B95BC). */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16; typedef int s32; typedef unsigned int u32;
typedef float f32;
typedef struct Blit Blit;
typedef struct MultiBlit {
    const char *texname;
    s16 dulx, duly, width, height, top, bot, left, right;
    u32 zdepth, alpha;
    s32 (*animfunc)(Blit *);
    u32 animid;
} MultiBlit;
typedef struct Egg { u8 size; u8 pad[3]; u32 levels[20]; u32 edges[20]; } Egg;
extern s8 D_803B308C;           /* inited */
extern Blit *D_803B3090;        /* blit */
extern s32 D_803B3094;          /* gSelectedOption */
extern s32 D_803B3098;          /* gCheatMenuExit */
extern const MultiBlit D_803B3068[];
extern s16 D_801164A4[];        /* gCheatState */
extern s8 D_8011648C[];         /* gCheatActive */
extern Egg D_803B3460[];        /* gEggs */
extern Egg *D_803B4278;         /* gEgg */
extern f32 D_803B427C;          /* gNextTimeOut */
extern s32 D_803B4280;          /* gEggIdx */
extern s32 D_8015694C;          /* gActiveEdges */
extern u32 D_80149784;          /* gActiveAutoEdges */
extern s8 D_80157244;           /* gSoftReset */
extern s8 D_8017A4D8, D_8013FECC, D_8013FECD, D_801403D0, D_801427A1, D_80142B00, D_80140418, D_8014061A;
extern s8 D_8013F2FC, D_80142DB0, D_80143F1C, D_8017A634, D_801407B0, D_801613AB, D_801407D0, D_801427A0;
extern u32 entity_flags_apply(u32, u32, u32, u8);
extern void audio_doppler(s32);
extern void particle_lifetime_set(void);
extern Blit *sound_control(s16, s16, const MultiBlit *, s16);
extern void sound_stop(Blit *);
extern void ambient_sounds_clear(void);
extern void func_800B5570(s32);
extern s32 func_8038F648(void);

extern void InitEgg(s32 egg);
void func_8039C908(void);
void func_8039C900(void);


void func_8039C910(void)
{
    if (!D_803B308C) {
        func_8039C908();
    }
    if (D_8015694C & 5) {
        entity_flags_apply((D_8015694C & 1) ? 46 : 38, 0, 1, 0);
        D_803B3098 = 2;
    } else if (D_80149784 & 0xc00) {
        if (D_80149784 & 0x400) {
            entity_flags_apply(40, 0, 1, 0);
            if (--D_803B3094 < 0) {
                D_803B3094 = 20;
            }
        } else {
            entity_flags_apply(39, 0, 1, 0);
            if (++D_803B3094 >= 21) {
                D_803B3094 = 0;
            }
        }
        InitEgg(D_803B3094);
    }
    if ((D_8015694C & 0x3000) && D_8011648C[D_803B3094]) {
        s32 dir = (D_8015694C & 0x1000) ? -1 : 1;

        audio_doppler(D_8015694C);
        switch (D_803B3094) {
        case 0:
            D_8017A4D8 ^= 1;
            D_801164A4[0] = D_8017A4D8;
            break;
        case 1:
            D_8013FECC ^= 1;
            D_801164A4[1] = D_8013FECC;
            break;
        case 2:
            D_8013FECD ^= 1;
            D_801164A4[2] = D_8013FECD;
            break;
        case 3:
            D_801164A4[3] ^= 1;
            break;
        case 4:
            D_801403D0 ^= 1;
            D_801164A4[4] = D_801403D0;
            break;
        case 5:
            D_801427A1 ^= 1;
            D_801164A4[5] = D_801427A1;
            break;
        case 6:
            D_80142B00 ^= 1;
            D_801164A4[6] = D_80142B00;
            break;
        case 7:
            D_80140418 ^= 1;
            D_801164A4[7] = D_80140418;
            break;
        case 8:
            D_8014061A ^= 1;
            D_801164A4[8] = D_8014061A;
            break;
        case 9:
            D_8013F2FC ^= 1;
            D_801164A4[9] = D_8013F2FC;
            break;
        case 10:
            D_80142DB0 = !D_80142DB0 ? 2 : 0;
            D_801164A4[10] = D_80142DB0 == 2;
            break;
        case 11:
            D_80143F1C += dir;
            if (D_80143F1C > 2) {
                D_80143F1C = 0;
            } else if (D_80143F1C < 0) {
                D_80143F1C = 2;
            }
            D_801164A4[11] = D_80143F1C;
            break;
        case 12:
            D_8017A634 += dir;
            if (D_8017A634 > 2) {
                D_8017A634 = 0;
            } else if (D_8017A634 < 0) {
                D_8017A634 = 2;
            }
            D_801164A4[12] = D_8017A634;
            break;
        case 13:
            D_801164A4[13] ^= 1;
            break;
        case 14:
            D_801164A4[14] ^= 1;
            break;
        case 15:
            D_801164A4[15] ^= 1;
            break;
        case 16:
            D_801164A4[16] ^= 1;
            break;
        case 17:
            D_801407B0 ^= 1;
            D_801164A4[17] = D_801407B0;
            break;
        case 18:
            D_801613AB += dir;
            if (D_801613AB > 3) {
                D_801613AB = 0;
            } else if (D_801613AB < 0) {
                D_801613AB = 3;
            }
            D_801164A4[18] = D_801613AB;
            break;
        case 19:
            D_801407D0 ^= 1;
            D_801164A4[19] = D_801407D0;
            break;
        case 20:
            if (D_801427A0 < 19) {
                D_801427A0 = 0;
            } else {
                D_801427A0 -= 18;
            }
            D_801427A0 += dir;
            if (D_801427A0 > 8) {
                D_801427A0 = 0;
            } else if (D_801427A0 < 0) {
                D_801427A0 = 8;
            }
            D_801164A4[20] = D_801427A0;
            if (D_801427A0) {
                D_801427A0 += 18;
            }
            break;
        }
    }
    if (!D_8011648C[D_803B3094]) {
        D_8011648C[D_803B3094] = func_8038F648();
    }
    if (D_80157244) {
        func_8039C900();
        func_800B5570(4);
    } else if (D_803B3098) {
        func_8039C900();
        func_800B5570(16);
    }
}

void func_8039C908(void)
{
    D_801164A4[0] = D_8017A4D8;
    D_801164A4[1] = D_8013FECC;
    D_801164A4[2] = D_8013FECD;
    D_801164A4[4] = D_801403D0;
    D_801164A4[5] = D_801427A1;
    D_801164A4[6] = D_80142B00;
    D_801164A4[7] = D_80140418;
    D_801164A4[8] = D_8014061A;
    D_801164A4[9] = D_8013F2FC;
    D_801164A4[10] = D_80142DB0 == 2;
    D_801164A4[11] = D_80143F1C;
    D_801164A4[12] = D_8017A634;
    D_801164A4[17] = D_801407B0;
    D_801164A4[18] = D_801613AB;
    D_801164A4[19] = D_801407D0;
    D_801164A4[20] = D_801427A0;
    if (D_801164A4[20] > 8) {
        D_801164A4[20] = 0;
    } else if (D_801164A4[20] < 0) {
        D_801164A4[20] = 8;
    }
    particle_lifetime_set();
    D_803B3090 = sound_control(0, 0, D_803B3068, 1);
    D_803B3094 = 0;
    D_803B3098 = 0;
    InitEgg(D_803B3094);
    D_803B308C = 1;
}

void func_8039C900(void)
{
    if (D_803B3090) {
        sound_stop(D_803B3090);
        D_803B3090 = 0;
    }
    ambient_sounds_clear();
    D_803B308C = 0;
}

/* func_803A5358 = animText@cheatscr.c (GC 0x80067E68), the multiblit callback; printOption@cheatscr.c
 * (GC 0x80067D20) is inlined into it and its deleted body is the stub func_803A5350 (non-static, off keep).
 * MAT3 port with the Dreamcast unit's `int x = 154` (dreamcast src/cheatscr.c animText) and a `name` local for
 * the label: with both named locals the retail frame (136, str1 at +112) comes out with no unused homes.
 * gText D_8017A4E0 (tab2 +0x1C, tab2a +0x20, tab2b +0x24, single +4); HAL_SetTextDepth render_helper,
 * HAL_SetFont object_create, TB_SetTextColor dispatch_handler, TB_CenterText sound_pitch_diff_halved,
 * TB_RightText sound_pitch_diff_calc, HAL_GetFontAscent object_type_byte2_get, TB_DrawText state_utility,
 * TB_Sprintf fcvt_wrapper ("%d" = D_803B8668), TB_GetFontLineHeight object_bytes_sum_global.
 * N64: 21 options; only cheat 18 (gFrameScale D_801613AB) prints a number (GC also 15, gRandomWeapons).
 * Shaping, measured: the value index is written `(u32)tab2a[...] + gCheatState[cheat] + 1` (the unsigned label
 * offset first, as found by the dot_runtime_a_settings_scroll_20261012 packet): MAT3's `state + 1 + tab2a[...]`
 * emits 190 instructions (the label*4 is not shared between the two branches). */

typedef struct TextTables {
    u16 numSingle; u8 **single;
    u16 numTab, numTabA; u16 *tab; u8 **taba;
    u16 numTab2, numTab2a, numTab2b; u16 *tab2; u16 *tab2a; u8 **tab2b;
} TextTables;
extern TextTables D_8017A4E0;   /* gText */
extern char D_803B8668[];       /* "%d" */
extern void render_helper(f32);
extern void *object_create(s32);
extern void dispatch_handler(s32);
extern s16 sound_pitch_diff_halved(void *, s16);
extern s16 sound_pitch_diff_calc(void *, s16);
extern u8 object_type_byte2_get(void);
extern s16 object_bytes_sum_global(void);
extern void state_utility(s16, s16, void *);
extern void fcvt_wrapper(u8 *, const char *, ...);
void func_803A5350(s32 x, s32 y, s32 item, u8 *name, u8 *value);

s32 func_803A5358(Blit *blit)
{
    s32 x;
    s32 i;
    s32 y;
    s32 cheat;
    u8 *str;
    u8 str1[4];
    u8 *name;

    render_helper(0.0f);
    object_create(10);
    dispatch_handler(12);
    state_utility(sound_pitch_diff_halved(D_8017A4E0.single[54], 151), (18 - object_type_byte2_get()) / 2 + 6, D_8017A4E0.single[54]);
    object_create(12);
    x = 154;
    y = 47;
    for (i = 0; i < 13; i++) {
        cheat = i + D_803B3094 - 6;
        if (cheat < 0) {
            cheat += 21;
        } else if (cheat >= 21) {
            cheat -= 21;
        }
        if (cheat == 18) {
            fcvt_wrapper(str1, D_803B8668, D_801613AB);
            str = str1;
        } else {
            str = D_8017A4E0.tab2b[(u32)D_8017A4E0.tab2a[*D_8017A4E0.tab2 + cheat] + D_801164A4[cheat] + 1];
        }
        name = D_8017A4E0.tab2b[D_8017A4E0.tab2a[*D_8017A4E0.tab2 + cheat]];
        func_803A5350(x, y, cheat, name, str);
        y += object_bytes_sum_global();
    }
    render_helper(-1.0f);
    return 1;
}

void func_803A5350(s32 x, s32 y, s32 item, u8 *name, u8 *value)
{
    object_create(D_803B3094 == item ? 11 : 12);
    dispatch_handler(D_803B3094 == item ? 22 : 1);
    state_utility(sound_pitch_diff_calc(name, x - 4), y - (D_803B3094 == item ? 1 : 0), name);
    object_create((D_8011648C[item] && D_803B3094 == item) ? 11 : 12);
    dispatch_handler(D_8011648C[item] ? (D_803B3094 == item ? 11 : 1) : 4);
    state_utility(x + 12, y - ((D_8011648C[item] && D_803B3094 == item) ? 1 : 0), value);
}
