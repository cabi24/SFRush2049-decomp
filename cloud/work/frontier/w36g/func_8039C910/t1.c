/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef int s32; typedef unsigned int u32;
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
extern u32 D_8015694C;          /* gActiveEdges */
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

static void InitEgg(s32 egg)
{
    D_803B4278 = &D_803B3460[egg];
    D_803B427C = 0.0f;
    D_803B4280 = 0;
}

static void initScreen(void)
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

static void uninitScreen(void)
{
    if (D_803B3090) {
        sound_stop(D_803B3090);
        D_803B3090 = 0;
    }
    ambient_sounds_clear();
    D_803B308C = 0;
}

void func_8039C910(void)
{
/*@{dir*//*@|    s32 dir;
@}*/    if (!D_803B308C) {
        initScreen();
    }
    if (D_8015694C & 5) {
/*@{*/        entity_flags_apply((D_8015694C & 1) ? 46 : 38, 0, 1, 0);
/*@|        entity_flags_apply(D_8015694C & 1 ? 46 : 38, 0, 1, 0);
@}*/        D_803B3098 = 2;
    } else if (D_80149784 & 0xc00) {
        if (D_80149784 & 0x400) {
            entity_flags_apply(40, 0, 1, 0);
/*@{*/            if (--D_803B3094 < 0) {
/*@|            --D_803B3094;
            if (D_803B3094 < 0) {
@}*/                D_803B3094 = 20;
            }
        } else {
            entity_flags_apply(39, 0, 1, 0);
/*@{*/            if (++D_803B3094 >= 21) {
/*@|            ++D_803B3094;
            if (D_803B3094 >= 21) {
@}*/                D_803B3094 = 0;
            }
        }
        InitEgg(D_803B3094);
    }
/*@{*/    if ((D_8015694C & 0x3000) && D_8011648C[D_803B3094]) {
/*@|    if (D_8015694C & 0x3000 && D_8011648C[D_803B3094]) {
@| if ((D_8015694C & 0x3000) != 0 && D_8011648C[D_803B3094] != 0) {
@}*//*@{dir*/        s32 dir = (D_8015694C & 0x1000) ? -1 : 1;
/*@|        dir = (D_8015694C & 0x1000) ? -1 : 1;
@}*/
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
        uninitScreen();
        func_800B5570(4);
    } else if (D_803B3098) {
        uninitScreen();
        func_800B5570(16);
    }
}
