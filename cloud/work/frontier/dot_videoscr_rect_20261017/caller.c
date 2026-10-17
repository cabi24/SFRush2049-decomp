/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* N64 videoscr initScreen and its operational initButtons helper.
 * Reconstructed from authenticated target control flow and the existing matched button layout.
 * Research only: no match claim. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef union { u32 word; struct { u8 r,g,b,a; } rgba; } VideoColor;
typedef struct { s32 type, handle; f32 ang; f32 mat[3][3]; f32 pos[3]; u8 pad3c[4]; } VideoButton;
typedef struct { u8 pad0[60]; u32 color; u8 pad40[4]; } VideoObject;
extern VideoButton D_8011A994[12];
extern char *D_8011A970[];
extern VideoObject D_8012E700[];
extern VideoColor D_8011AD88;
extern f32 D_8011418C[];
extern volatile u8 gNumModels;
extern s8 D_80117428;
extern s32 state_word_a;
extern s32 D_8011AC94, D_8011AC98;
extern s32 D_8011AD34, D_8011AD3C, D_8011AD44, D_8011AD4C, D_8011AD50;
extern s8 D_8011AD30, D_8011AD54, D_8011AD58, D_8011AD5C;
extern s8 D_8011AD40[];
extern s8 D_80146108[];
extern char D_8011ACA0[], D_8011ACE8[];
extern char D_8012268C[], D_80122694[], D_80122698[];
extern const f32 D_80123DE4, D_80123DE8, D_80123DEC, D_80123DF0;
void *memcpy(void *, const void *, u32);
void *string_copy_format(char *, void *, s8, s32);
s32 func_8008E26C(void *, f32 (*)[3], s16, u32);
s32 func_800B24EC(char *, u16 *, void *, s8, s32);
void func_8008D870(s16, s32, s32);
void func_800B5898(f32, f32 (*)[3]);
void func_8008B32C(f32 (*)[3], f32 (*)[3], f32);
void particle_lifetime_set(void);
void particle_velocity_set(void);
void particle_render(void);
void set_race_state(void);
s32 sound_control(s32, s32, void *, s32);

void video_setColor(s32 object, VideoColor *color)
{
    D_8012E700[object].color = color->word;
}

s8 video_validOption(s32 i) { s8 valid; valid = D_8011AD40[i]; return valid; }

static void video_initButtons(void)
{
    VideoColor buttonColor;
    s32 tex;
    u16 texIdx;
    s32 buttonIdx;
    buttonColor = D_8011AD88;
    for (buttonIdx = 0; buttonIdx < 12; buttonIdx++) {
        if (D_8011A994[buttonIdx].handle == -1) {
            memcpy(D_8011A994[buttonIdx].mat, D_8011418C, 36);
            D_8011A994[buttonIdx].handle = func_8008E26C(
                string_copy_format(D_8011A970[D_8011A994[buttonIdx].type], 0, gNumModels - 1, 1),
                D_8011A994[buttonIdx].mat, -1,
                D_8011A994[buttonIdx].type == 0 || D_8011A994[buttonIdx].type == 1 || D_8011A994[buttonIdx].type == 2 ? 0x42000 : 0);
            video_setColor(D_8011A994[buttonIdx].handle, &buttonColor);
        }
    }
    tex = func_800B24EC(D_8012268C, &texIdx, 0, gNumModels - 1, 1);
    func_8008D870(D_8011A994[8].handle, tex, -1);
    tex = func_800B24EC(D_80122694, &texIdx, 0, gNumModels - 1, 1);
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
    tex = func_800B24EC(D_80122698, &texIdx, 0, gNumModels - 1, 1);
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

void particle_collision(void)
{
    D_80117428 = 1;
    particle_render();
    if (state_word_a & 0x7C03FFFE) {
        particle_lifetime_set();
        video_initButtons();
        D_8011AD34 = sound_control(0, 0, D_8011ACA0, 2);
    } else {
        D_8011AD34 = sound_control(0, 0, D_8011ACE8, 2);
    }
    D_8011AD4C = D_80146108[20];
    D_8011AD50 = D_80146108[19];
    set_race_state();
    while (!video_validOption(D_8011AD44)) {
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
