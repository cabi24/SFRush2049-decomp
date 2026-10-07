/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Whole native options-menu setup; real font wrapper context retained. */
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
typedef struct Button {
    s32 tag, handle;
    f32 angle, matrix[3][3], position[3];
    u8 alpha, reserved[3];
} Button;
typedef struct RenderObject { u8 prefix[60]; u32 color, suffix; } RenderObject;
extern Button D_8011650C[32];
extern RenderObject D_8012E700[];
extern s8 D_80117428, D_80116D94, D_80116DA8;
extern s16 D_80149D9C, D_80149B84, D_80116D9C, D_80149DA2;
extern s32 D_80116D14[14], D_80116DA0, D_80116D0C, D_80116D10, D_8015698C;
extern u32 state_word_a, D_80116DAC;
extern s8 D_80156CF0[][16];
extern u8 D_80140BDC;
extern char *D_801164E8[];
extern char D_8012102C[], D_80121034[], D_80121038[];
extern f32 D_8011418C[3][3];
extern OSMesgQueue D_801461D0;
extern const MultiBlit D_80116D4C, D_80116D70;
extern Blit *D_80116D98;
extern s32 osRecvMesg(OSMesgQueue *, void **, s32);
extern s32 osJamMesg(OSMesgQueue *, void *, s32);
extern s32 slot_state_setup(s32);
extern s16 object_bytes_sum_global(void);
extern void particle_lifetime_set(void);
extern void particle_velocity_set(void);
extern void entity_audio_update(void);
extern void time_result_display(void);
extern void *memcpy(void *, const void *, u32);
extern s32 string_copy_format(char *, s8, s8, s8);
extern s32 func_8008E26C(s32, void *, s16, s32);
extern TexDef *func_800B24EC(char *, s16 *, s8, s8, s32);
extern void func_8008D870(s16, TexDef *, s32);
extern void func_800B5898(f32, f32 [3][3]);
extern void func_8008B32C(f32 [3][3], f32 [3][3], f32);
extern Blit *sound_control(s16, s16, const MultiBlit *, s16);
extern FiveParts *ambient_sound_set(s32, s32, s32, s32, s32, s32, s32, s32);

/* Existing genuine wrapper: cloud/work/s20261004/E/src/helper.h. */
static void gfx_lock(void) { osRecvMesg(&D_801461D0, 0, 1); }
static void gfx_unlock(void) { osJamMesg(&D_801461D0, 0, 0); }
static s32 font_set(s32 font)
{
    s32 old;
    gfx_lock();
    old = slot_state_setup(font);
    gfx_unlock();
    return old;
}

void replay_save_prompt(void)
{
    s32 available;
    s32 *option;
    Button *button;
    f32 (*matrix)[3];
    s32 model, flags, handle;
    u32 color;
    s16 textureIndex;
    s16 y;
    f32 angle, height;
    D_80117428 = 1;
    font_set(13);
    available = 210 - object_bytes_sum_global() * 2;
    font_set(11);
    available -= object_bytes_sum_global();
    D_80149D9C = 0;
    for (option = D_80116D14; option != D_80116D14 + 14; ++option) {
        if (*option != 0) ++D_80149D9C;
    }
    if (D_80149D9C < available / object_bytes_sum_global()) {
        D_80149B84 = D_80149D9C;
    } else {
        D_80149B84 = available / object_bytes_sum_global();
    }
    D_80116D9C = 0;
    while (D_80116D14[D_80116D9C] == 0) {
        ++D_80116D9C;
        if (D_80116D9C >= 14) D_80116D9C = 0;
    }
    D_80149DA2 = D_80116D9C;
    D_80116DA0 = 0;
    if (state_word_a & 0x7C03FFFE) {
        particle_lifetime_set();
        color = D_80116DAC;
        for (button = D_8011650C; button != D_8011650C + 32; ++button) {
            matrix = button->matrix;
            if (button->handle == -1) {
                memcpy(matrix, D_8011418C, 36);
                model = string_copy_format(D_801164E8[button->tag], 0,
                                           (s8)(D_80140BDC - 1), 1);
                flags = button->tag == 0 || button->tag == 1 || button->tag == 2 ? 0x42000 : 0;
                handle = func_8008E26C(model, matrix, -1, flags);
                button->handle = handle;
                D_8012E700[handle].color = color;
            }
        }
        func_8008D870((s16)D_8011650C[28].handle,
            func_800B24EC(D_8012102C, &textureIndex, 0, (s8)(D_80140BDC - 1), 1), -1);
        func_8008D870((s16)D_8011650C[29].handle,
            func_800B24EC(D_80121034, &textureIndex, 0, (s8)(D_80140BDC - 1), 1), -1);
        matrix = D_8011650C[29].matrix;
        memcpy(matrix, D_8011418C, 36);
        angle = -1.5707964f;
        func_800B5898(angle, matrix);
        height = 55.2f;
        D_8011650C[29].position[1] = height;
        D_8011650C[29].position[2] = 100.0f;
        D_8011650C[29].position[0] = -75.1f;
        func_8008B32C(matrix, matrix, 0.5f);
        matrix = D_8011650C[30].matrix;
        memcpy(matrix, D_8011418C, 36);
        func_800B5898(angle, matrix);
        D_8011650C[30].position[1] = height;
        D_8011650C[30].position[2] = 100.0f;
        D_8011650C[30].position[0] = -87.1f;
        func_8008D870((s16)D_8011650C[31].handle,
            func_800B24EC(D_80121038, &textureIndex, 0, (s8)(D_80140BDC - 1), 1), -1);
        matrix = D_8011650C[31].matrix;
        memcpy(matrix, D_8011418C, 36);
        func_800B5898(angle, matrix);
        D_8011650C[31].position[1] = 115.0f;
        D_8011650C[31].position[0] = 0.0f;
        D_8011650C[31].position[2] = 200.0f;
        D_80116D0C = 1;
        D_80116D10 = 1;
        particle_velocity_set();
        D_80116D98 = sound_control(0, 0, &D_80116D4C, 1);
    } else {
        D_80116DA8 = D_80156CF0[D_8015698C][0] == 0;
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
