/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * Video-options screen setup (GC videoscr.c), for the physics_sym group (updateVideoScreen = physics_sym):
 *  particle_render    (0x800B8650, 448 B) = GC calcMenuRect@videoscr.c (MAT3 0x8006519C)
 *  particle_collision (0x800B8820, 1012 B) = GC initScreen@videoscr.c (MAT3 0x8006425C) with GC initButtons@videoscr.c
 *                       (MAT3 0x80065338) written in place.
 * w36q (wave 36). Pattern from the audio pair (groups/audio_options_menu/init_w36q.c; Astra PR #411 for audioscr).
 * Helpers vi_gfx_lock / vi_gfx_unlock / vi_SetFont (GC HAL_SetFont: no old-font value is kept, retail has no
 * `move s0,v0`) and vi_setButtonColor (GC ZOID_SetColor) are non-static and not on keep (w17j stub rule).
 * DISCLOSED DEVICE: unused locals u0..u3 in particle_collision = the exact frame residual: retail frame 136, without
 * them 120 (4 slots, 16 B at the frame top; every sp-relative access is identical with and without them). The same
 * 16 B appear in drone_pathfind_main (GC initScreen@audioscr.c, also with initButtons written in place): most likely
 * retail's umerge inline area for initButtons, which the shadow unit cannot inline (umerge's knapsack is global).
 * N64 differences from GC: calcMenuRect uses font 11 and text tables 41/42 (GC 10, 42/43); validOption is the
 * group's getter func_800B59E8 (optionValid[i] only); NewMultiBlit gets 2 entries for both defs; B1/arrow y is 55.2.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct OSMesgQueue OSMesgQueue;
typedef struct TexDef TexDef;
typedef struct Blit Blit;
typedef struct MultiBlit MultiBlit;
typedef struct { s32 type, handle; f32 ang; f32 mat[3][3]; f32 pos[3]; u8 pad3C[4]; } Button;   /* 0x40 */
typedef struct { u8 pad0[60]; u32 color; u8 pad40[4]; } Ent;                                 /* 0x44 */
typedef union Color { u8 c[4]; u32 rgba; } Color;
typedef struct TextState { void *word0; char **single; void *word8; u16 *tab; char **taba; } TextState;
typedef struct { s32 x, w, h, cx; } MenuRect;

extern OSMesgQueue D_801461D0;
extern TextState countdown_state;          /* gText */
extern MenuRect D_8011AD70;                /* menuRect */
extern Button D_8011A994[12];              /* buttonInfo */
extern char *D_8011A970[];                 /* buttonNames */
extern Ent D_8012E700[];
extern volatile u8 gNumModels;
extern f32 D_8011418C[3][3];               /* unitmatrix */
extern const MultiBlit D_8011ACA0, D_8011ACE8;   /* frontEndBlit, pauseBlit */
extern Blit *D_8011AD34;                   /* blit */
extern s32 D_8011AC94, D_8011AC98;         /* buttonsInited, buttonsJustInited */
extern s8 D_80146108[];                    /* gOptions */
extern s32 D_8011AD4C, D_8011AD50;         /* gVertCenter, gHorzCenter */
extern s32 D_8011AD44;                     /* gVideoMenuSelect */
extern s8 D_8011AD54, D_8011AD58, D_8011AD5C;   /* gTestFrequency, gConfirmFrequency, gYesFrequency */
extern s32 D_8011AD3C;                     /* exitVideoScreen */
extern s8 D_8011AD30;                      /* inited */
extern u32 state_word_a;                   /* gGameState */

s32 osRecvMesg(OSMesgQueue *, void *, s32);
s32 osSendMesg(OSMesgQueue *, void *, s32);
s32 slot_state_setup(s32);
u32 object_manager_update(void *, s16);    /* HAL_GetTextWidth */
s16 object_bytes_sum_global(void);         /* TB_GetFontLineHeight */
s32 fcvt_wrapper(char *, char *, ...);     /* TB_Sprintf */
s32 func_800B59E8(s32 i);                  /* validOption (physics_sym getter) */
void *memcpy(void *, const void *, u32);
s32 string_copy_format(char *, s8, s8, s32);       /* MBOX_FindObject_WORK */
s16 sign_extend_call(s32, void *, s32, u32);       /* MBOX_NewObject wrapper */
TexDef *func_800B24EC(char *, u16 *, s8, s8, s32); /* MBOX_FindTexture_WORK */
void func_8008D870(s16, TexDef *, s32);            /* ZOID_SetTexture */
void func_800B5898(f32, f32 [3][3]);               /* WPitchUV */
void func_8008B32C(f32 [3][3], f32 [3][3], f32);   /* ScaleUV */
void func_800B8810(void);                          /* SaveGlobals */
void particle_lifetime_set(void);                  /* createBG */
void particle_velocity_set(void);                  /* ZeroCameras */
void entity_audio_update(void);                    /* ClearRushControls */
Blit *sound_control(s16, s16, const MultiBlit *, s16);   /* NewMultiBlit */

void vi_gfx_lock(void) { osRecvMesg(&D_801461D0, 0, 1); }
void vi_gfx_unlock(void) { osSendMesg(&D_801461D0, 0, 0); }
void vi_SetFont(s32 font)
{
    vi_gfx_lock();
    slot_state_setup(font);
    vi_gfx_unlock();
}

void particle_render(void)
{
    s32 menuSize;
    s32 optionWidth;
    s32 valueWidth;
    s32 menuIdx;
    u8 str1[40];

    vi_SetFont(11);
    menuSize = 0;
    optionWidth = 0;
    valueWidth = 0;
    for (menuIdx = 0; menuIdx < 4; menuIdx++) {
        if (func_800B59E8(menuIdx) == 1) {
            menuSize++;
            optionWidth = object_manager_update(countdown_state.taba[countdown_state.tab[41] + menuIdx], -1) > optionWidth ?
                object_manager_update(countdown_state.taba[countdown_state.tab[41] + menuIdx], -1) : optionWidth;
            if (menuIdx == 2) {
                fcvt_wrapper((char *)str1, "%s  %s", (countdown_state.taba + countdown_state.tab[42])[0],
                             (countdown_state.taba + countdown_state.tab[42])[1]);
                valueWidth = object_manager_update(str1, -1) > valueWidth ? object_manager_update(str1, -1) : valueWidth;
            } else {
                valueWidth = object_manager_update("000", -1) > valueWidth ? object_manager_update("000", -1) : valueWidth;
            }
        }
    }
    D_8011AD70.w = optionWidth + valueWidth + 10;
    D_8011AD70.h = menuSize * object_bytes_sum_global();
    D_8011AD70.x = 160 - D_8011AD70.w / 2;
    D_8011AD70.cx = D_8011AD70.x + optionWidth + 5;
}

void vi_setButtonColor(s16 objnum, Color *col) { D_8012E700[objnum].color = col->rgba; }

void particle_collision(void)
{
    s32 u0, u1, u2, u3;

    func_800B8810();
    particle_render();
    if (state_word_a & 0x7C03FFFE) {
        particle_lifetime_set();
        {
            Color buttonColor = { { 255, 255, 255, 255 } };
            TexDef *tex;
            u16 texIdx;
            s32 buttonIdx;

            for (buttonIdx = 0; buttonIdx < 12; buttonIdx++) {
                if (D_8011A994[buttonIdx].handle == -1) {
                    memcpy(D_8011A994[buttonIdx].mat, D_8011418C, 36);
                    D_8011A994[buttonIdx].handle = sign_extend_call(
                        string_copy_format(D_8011A970[D_8011A994[buttonIdx].type], 0, gNumModels - 1, 1),
                        &D_8011A994[buttonIdx].mat, -1,
                        (D_8011A994[buttonIdx].type == 0 || D_8011A994[buttonIdx].type == 1 ||
                         D_8011A994[buttonIdx].type == 2) ? 0x42000 : 0);
                    vi_setButtonColor(D_8011A994[buttonIdx].handle, &buttonColor);
                }
            }
            tex = func_800B24EC("CURSOR", &texIdx, 0, gNumModels - 1, 1);
            func_8008D870(D_8011A994[8].handle, tex, -1);
            tex = func_800B24EC("B1", &texIdx, 0, gNumModels - 1, 1);
            func_8008D870(D_8011A994[9].handle, tex, -1);
            memcpy(D_8011A994[9].mat, D_8011418C, 36);
            func_800B5898(-1.5707964f, D_8011A994[9].mat);
            D_8011A994[9].pos[0] = -75.1f;
            D_8011A994[9].pos[1] = 55.2f;
            D_8011A994[9].pos[2] = 100.0f;
            func_8008B32C(D_8011A994[9].mat, D_8011A994[9].mat, 0.5f);
            memcpy(D_8011A994[10].mat, D_8011418C, 36);
            func_800B5898(-1.5707964f, D_8011A994[10].mat);
            D_8011A994[10].pos[0] = -87.1f;
            D_8011A994[10].pos[1] = 55.2f;
            D_8011A994[10].pos[2] = 100.0f;
            tex = func_800B24EC("BUTTON_TITLE", &texIdx, 0, gNumModels - 1, 1);
            func_8008D870(D_8011A994[11].handle, tex, -1);
            memcpy(D_8011A994[11].mat, D_8011418C, 36);
            func_800B5898(-1.5707964f, D_8011A994[11].mat);
            D_8011A994[11].pos[0] = 0.0f;
            D_8011A994[11].pos[1] = 115.0f;
            D_8011A994[11].pos[2] = 200.0f;
            D_8011AC94 = 1;
            D_8011AC98 = 1;
            particle_velocity_set();
        }
        D_8011AD34 = sound_control(0, 0, &D_8011ACA0, 2);
    } else {
        D_8011AD34 = sound_control(0, 0, &D_8011ACE8, 2);
    }
    D_8011AD4C = D_80146108[20];
    D_8011AD50 = D_80146108[19];
    entity_audio_update();
    while (!func_800B59E8(D_8011AD44)) {
        if (++D_8011AD44 >= 4) {
            D_8011AD44 = 0;
            break;
        }
    }
    D_8011AD54 = 0;
    D_8011AD58 = 0;
    D_8011AD5C = 0;
    D_8011AD3C = 0;
    D_8011AD30 = 1;
}
