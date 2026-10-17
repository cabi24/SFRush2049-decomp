/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* DABDC optionscr initScreen research. Donor-local order and API boundaries
 * are retained; N64 dimensions, layouts, indexes and queue calls stay native.
 * func_800B8810 = SaveGlobals; sign_extend_call = MBOX_NewObject.
 * Definition-after-use ordering for sign_extend_call is disclosed as an as1
 * source-location scheduling lever. No new padding or unused local. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct OSMesgQueue OSMesgQueue;
typedef struct TexDef TexDef;
typedef struct Blit Blit;
typedef struct MultiBlit MultiBlit;
typedef struct FiveParts FiveParts;
typedef struct Matrix { f32 uvs[3][3]; f32 pos[3]; } Matrix;
typedef struct Button {
    s32 tag, handle;
    f32 angle; Matrix mat;
    u8 alpha, reserved[3];
} Button;
typedef struct RenderObject { u8 prefix[60]; u32 color, suffix; } RenderObject;
extern Button D_8011650C[32];
extern RenderObject D_8012E700[];
extern s8 D_80117428, D_80116D94, D_80116DA8;
extern s16 D_80149D9C, D_80149B84, D_80116D9C, D_80149DA2;
extern s32 D_80116D14[14], D_80116DA0, D_80116D0C, D_80116D10, D_8015698C;
extern u32 state_word_a, D_80116DAC;
typedef struct { volatile s8 attached; u8 pad[15]; } Pad16;
extern Pad16 D_80156CF0[];
extern volatile u8 D_80140BDC;
extern char *D_801164E8[];
extern char D_8012102C[], D_80121034[], D_80121038[];
extern f32 D_8011418C[3][3];
extern OSMesgQueue D_801461D0;
extern const MultiBlit D_80116D4C, D_80116D70;
extern Blit *D_80116D98;
extern s32 osRecvMesg(OSMesgQueue *, void **, s32);
extern s32 osSendMesg(OSMesgQueue *, void *, s32);
extern s32 slot_state_setup(s32);
extern s16 object_bytes_sum_global(void);
extern void particle_lifetime_set(void);
extern void particle_velocity_set(void);
extern void entity_audio_update(void);
extern void time_result_display(void);
extern void *memcpy(void *, const void *, u32);
extern s32 string_copy_format(char *, s8, s8, s8);
extern s16 func_8008E26C(s32, void *, s16, s32);
extern TexDef *func_800B24EC(char *, unsigned short *, s8, s8, s32);
extern void func_8008D870(s16, TexDef *, s32);
extern void func_800B5898(f32, f32 [3][3]);
extern void func_8008B32C(f32 [3][3], f32 [3][3], f32);
extern Blit *sound_control(s16, s16, const MultiBlit *, s16);
extern FiveParts *ambient_sound_set(s32, s32, s32, s32, s32, s32, s32, s32);

/* Existing genuine wrapper: cloud/work/s20261004/E/src/helper.h. */
void gfx_lock(void) { osRecvMesg(&D_801461D0, 0, 1); }
void gfx_unlock(void) { osSendMesg(&D_801461D0, 0, 0); }
s32 font_set(s32 font)
{
    s32 old;
    gfx_lock();
    old = slot_state_setup(font);
    gfx_unlock();
    return old;
}

typedef union Color { u8 c[4]; u32 rgba; } Color;
void setButtonColor(s16 objnum, Color *col) { D_8012E700[objnum].color = col->rgba; }
void calcOptionMenuMaxSize(void)
{
    s32 height;
    s32 i;
    font_set(13);
    height = 210 - object_bytes_sum_global() * 2;
    font_set(11);
    height -= object_bytes_sum_global();
    D_80149D9C = 0;
    for (i = 0; i < 14; i++) {
        if (D_80116D14[i]) D_80149D9C++;
    }
    D_80149B84 = D_80149D9C < height / object_bytes_sum_global()
        ? D_80149D9C : height / object_bytes_sum_global();
}
s16 sign_extend_call(s32 obj, Matrix *mat, s32 parent, u32 flags);
void initButtons(void)
{
    Color buttonColor = {255, 255, 255, 255};
    TexDef *tex;
    unsigned short texIdx;
    s32 buttonIdx;
    for (buttonIdx = 0; buttonIdx < 32; buttonIdx++) {
        if (D_8011650C[buttonIdx].handle == -1) {
            memcpy(D_8011650C[buttonIdx].mat.uvs, D_8011418C, 36);
            D_8011650C[buttonIdx].handle = sign_extend_call(
                string_copy_format(D_801164E8[D_8011650C[buttonIdx].tag], 0,
                                   (s8)(D_80140BDC - 1), 1),
                &D_8011650C[buttonIdx].mat, -1,
                (D_8011650C[buttonIdx].tag == 0 ||
                 D_8011650C[buttonIdx].tag == 1 ||
                 D_8011650C[buttonIdx].tag == 2) ? 0x42000 : 0);
            setButtonColor(D_8011650C[buttonIdx].handle, &buttonColor);
        }
    }
    tex = func_800B24EC(D_8012102C, &texIdx, 0, (s8)(D_80140BDC - 1), 1);
    func_8008D870(D_8011650C[28].handle, tex, -1);
    tex = func_800B24EC(D_80121034, &texIdx, 0, (s8)(D_80140BDC - 1), 1);
    func_8008D870(D_8011650C[29].handle, tex, -1);
    memcpy(D_8011650C[29].mat.uvs, D_8011418C, 36);
    func_800B5898(-1.5707964f, D_8011650C[29].mat.uvs);
    D_8011650C[29].mat.pos[0] = -75.1f;
    D_8011650C[29].mat.pos[1] = 55.2f;
    D_8011650C[29].mat.pos[2] = 100.0f;
    func_8008B32C(D_8011650C[29].mat.uvs, D_8011650C[29].mat.uvs, 0.5f);
    memcpy(D_8011650C[30].mat.uvs, D_8011418C, 36);
    func_800B5898(-1.5707964f, D_8011650C[30].mat.uvs);
    D_8011650C[30].mat.pos[0] = -87.1f;
    D_8011650C[30].mat.pos[1] = 55.2f;
    D_8011650C[30].mat.pos[2] = 100.0f;
    tex = func_800B24EC(D_80121038, &texIdx, 0, (s8)(D_80140BDC - 1), 1);
    func_8008D870(D_8011650C[31].handle, tex, -1);
    memcpy(D_8011650C[31].mat.uvs, D_8011418C, 36);
    func_800B5898(-1.5707964f, D_8011650C[31].mat.uvs);
    D_8011650C[31].mat.pos[0] = 0.0f;
    D_8011650C[31].mat.pos[1] = 115.0f;
    D_8011650C[31].mat.pos[2] = 200.0f;
    D_80116D0C = 1;
    D_80116D10 = 1;
    particle_velocity_set();
}
extern void func_800B8810(void);
void replay_save_prompt(void)
{
    s16 y;
    func_800B8810();
    calcOptionMenuMaxSize();
    D_80116D9C = 0;
    while (D_80116D14[D_80116D9C] == 0) {
        ++D_80116D9C;
        if (D_80116D9C >= 14) D_80116D9C = 0;
    }
    D_80149DA2 = D_80116D9C;
    D_80116DA0 = 0;
    if (state_word_a & 0x7C03FFFE) {
        particle_lifetime_set();
        initButtons();
        D_80116D98 = sound_control(0, 0, &D_80116D4C, 1);
    } else {
        D_80116DA8 = !D_80156CF0[D_8015698C].attached;
        font_set(13);
        time_result_display();
        y = object_bytes_sum_global() * 2 + 10;
        font_set(11);
        ambient_sound_set(40, y - 4, 280, object_bytes_sum_global() * D_80149B84 + y + 4,
                          176, 0, 0, 0);
        D_80116D98 = sound_control(0, 0, &D_80116D70, 1);
    }
    entity_audio_update();
    D_80116D94 = 1;
}
s16 sign_extend_call(s32 obj, Matrix *mat, s32 parent, u32 flags)
{
    s16 dlidx;
    dlidx = func_8008E26C(obj, mat, parent, flags);
    return dlidx;
}
