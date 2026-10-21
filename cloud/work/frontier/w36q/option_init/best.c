/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * replay_save_prompt (0x800DABDC, 1540 B; historical label) = GC initScreen@optionscr.c (MAT3 optionscr.c, GC
 * 0x80074E38) with GC calcOptionMenuMaxSize (GC 0x80075BB0) and initButtons (GC 0x80075D00) written in place.
 * Internal: sole caller func_800DB1E0 = updateOptionScreen (group frontier_optionscr_buttons).
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
typedef struct { volatile s8 attached; u8 pad[15]; } ContData;

extern OSMesgQueue D_801461D0;
extern Button D_8011650C[32];              /* buttonInfo */
extern char *D_801164E8[];                 /* buttonNames */
extern Ent D_8012E700[];
extern volatile u8 gNumModels;
extern f32 D_8011418C[3][3];               /* unitmatrix */
extern const MultiBlit D_80116D4C, D_80116D70;   /* frontEndBlit, pauseBlit */
extern Blit *D_80116D98;                   /* blit */
extern s32 D_80116D0C, D_80116D10;         /* buttonsInited, buttonsJustInited */
extern s32 D_80116D14[14];                 /* validOption */
extern s16 D_80149D9C;                     /* gMenuCount */
extern s16 D_80149B84;                     /* gOptionMenuSize */
extern s16 D_80116D9C;                     /* gSelectedOption */
extern s16 D_80149DA2;                     /* gFirstOption */
extern s32 D_80116DA0;                     /* exitOptionScreen */
extern s8 D_80116DA8;                      /* gControllerRemoved */
extern s32 D_8015698C;                     /* playerInPause */
extern ContData D_80156CF0[];              /* gControllerData */
extern s8 D_80116D94;                      /* inited */
extern s8 D_80117428;
extern u32 state_word_a;                   /* gGameState */

s32 osRecvMesg(OSMesgQueue *, void *, s32);
s32 osSendMesg(OSMesgQueue *, void *, s32);
s32 slot_state_setup(s32);
s16 object_bytes_sum_global(void);         /* TB_GetFontLineHeight */
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
void time_result_display(void);                    /* _TitlePane@optionscr.c */
void *ambient_sound_set(s32, s32, s32, s32, s32, s32, s32, s32);   /* InitPane */
Blit *sound_control(s16, s16, const MultiBlit *, s16);   /* NewMultiBlit */

void op_gfx_lock(void) { osRecvMesg(&D_801461D0, 0, 1); }
void op_gfx_unlock(void) { osSendMesg(&D_801461D0, 0, 0); }
s32 op_font_set(s32 font)
{
    s32 old;
    op_gfx_lock();
    old = slot_state_setup(font);
    op_gfx_unlock();
    return old;
}
void op_SetFont(s32 font)
{
    op_font_set(font);
}
void op_setButtonColor(s16 objnum, Color *col) { D_8012E700[objnum].color = col->rgba; }

void replay_save_prompt(void)
{
    s16 y;

    func_800B8810();
    {
        s32 height;
        s32 i;

        op_SetFont(13);
        height = 210 - object_bytes_sum_global() * 2;
        op_SetFont(11);
        height -= object_bytes_sum_global();
        D_80149D9C = 0;
        for (i = 0; i < 14; i++) {
            if (D_80116D14[i]) D_80149D9C++;
        }
        D_80149B84 = D_80149D9C < height / object_bytes_sum_global() ? D_80149D9C : height / object_bytes_sum_global();
    }
    D_80116D9C = 0;
    while (!D_80116D14[D_80116D9C]) {
        if (++D_80116D9C >= 14) D_80116D9C = 0;
    }
    D_80149DA2 = D_80116D9C;
    D_80116DA0 = 0;
    if (state_word_a & 0x7C03FFFE) {
        particle_lifetime_set();
        {
            Color buttonColor = { { 255, 255, 255, 255 } };
            TexDef *tex;
            u16 texIdx;
            s32 buttonIdx;

            for (buttonIdx = 0; buttonIdx < 32; buttonIdx++) {
                if (D_8011650C[buttonIdx].handle == -1) {
                    memcpy(D_8011650C[buttonIdx].mat, D_8011418C, 36);
                    D_8011650C[buttonIdx].handle = sign_extend_call(
                        string_copy_format(D_801164E8[D_8011650C[buttonIdx].type], 0, gNumModels - 1, 1),
                        &D_8011650C[buttonIdx].mat, -1,
                        (D_8011650C[buttonIdx].type == 0 || D_8011650C[buttonIdx].type == 1 ||
                         D_8011650C[buttonIdx].type == 2) ? 0x42000 : 0);
                    op_setButtonColor(D_8011650C[buttonIdx].handle, &buttonColor);
                }
            }
            tex = func_800B24EC("CURSOR", &texIdx, 0, gNumModels - 1, 1);
            func_8008D870(D_8011650C[28].handle, tex, -1);
            tex = func_800B24EC("B1", &texIdx, 0, gNumModels - 1, 1);
            func_8008D870(D_8011650C[29].handle, tex, -1);
            memcpy(D_8011650C[29].mat, D_8011418C, 36);
            func_800B5898(-1.5707964f, D_8011650C[29].mat);
            D_8011650C[29].pos[0] = -75.1f;
            D_8011650C[29].pos[1] = 55.2f;
            D_8011650C[29].pos[2] = 100.0f;
            func_8008B32C(D_8011650C[29].mat, D_8011650C[29].mat, 0.5f);
            memcpy(D_8011650C[30].mat, D_8011418C, 36);
            func_800B5898(-1.5707964f, D_8011650C[30].mat);
            D_8011650C[30].pos[0] = -87.1f;
            D_8011650C[30].pos[1] = 55.2f;
            D_8011650C[30].pos[2] = 100.0f;
            tex = func_800B24EC("BUTTON_TITLE", &texIdx, 0, gNumModels - 1, 1);
            func_8008D870(D_8011650C[31].handle, tex, -1);
            memcpy(D_8011650C[31].mat, D_8011418C, 36);
            func_800B5898(-1.5707964f, D_8011650C[31].mat);
            D_8011650C[31].pos[0] = 0.0f;
            D_8011650C[31].pos[1] = 115.0f;
            D_8011650C[31].pos[2] = 200.0f;
            D_80116D0C = 1;
            D_80116D10 = 1;
            particle_velocity_set();
        }
        D_80116D98 = sound_control(0, 0, &D_80116D4C, 1);
    } else {
        D_80116DA8 = !D_80156CF0[D_8015698C].attached;
        op_SetFont(13);
        time_result_display();
        y = object_bytes_sum_global() * 2 + 10;
        op_SetFont(11);
        ambient_sound_set(40, y - 4, 280, y + (D_80149B84 * object_bytes_sum_global() + 4), 176, 0, 0, 0);
        D_80116D98 = sound_control(0, 0, &D_80116D70, 1);
    }
    entity_audio_update();
    D_80116D94 = 1;
}
