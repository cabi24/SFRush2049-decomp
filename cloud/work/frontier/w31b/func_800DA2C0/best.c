/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * PAIR DRAFT (w31b, from w30b's pair file): func_800DB1E0 = GC updateOptionScreen@optionscr.c and its internal
 * callee func_800DA2C0 = GC updateButtons@optionscr.c (unsaved s0-s8/f20-f30: the two land together).
 * func_800DB1E0: unchanged from w30b (unit EQUAL 350 words with this DA2C0 internal).
 * func_800DA2C0 (w31b rewrite, NO pad arrays): unit 489/579 ops 78 frame 152/160 (w30b, with w10e's pads) ->
 *   202/579, words 88, ops 5, frame 152/120. Levers, in order (rows from `us.sh --internal func_800DA2C0`):
 *   - GC cursorSpeed$472 / scrollSpeed$473 are FUNCTION-LOCAL STATICS = D_8015273C / D_80152740: written as
 *     `static f32` inside the function and used directly (no `speed` local), uopt promotes each to one FP register
 *     with a store at every exit arm, exactly retail's `lwc1 $f2,10044(at)` ... `swc1 $f2,10044(at)` (ops 78 -> 54).
 *   - sel_y computed BEFORE the -160/225 stores (GC order) (ops 54 -> 38).
 *   - two spellings of 255 in the alpha ramps (`255.0f - (...) * 255 / 40.0f`): one constant fewer to hoist, so
 *     12.566371f is hoisted into $f30 as in retail instead of 255.0f (ops 38 -> 28; 255.f/255 int all equal).
 *   - `[i+14].alpha = [i].alpha` and `if ([i].alpha == 0)` (retail keeps the loaded alpha in v1) (ops 28 -> 26).
 *   - DISCLOSED DEVICE: compiled-out holder `if (yi) {}` after `yi -= 40.0f` (ctrace: yi web save 1.50 vs the
 *     &D_80116D10 address web 1.55 for the last callee-saved register s8; with the holder yi is coloured and the
 *     D_80116D10 loads become per-use `lui/lw` as retail) (ops 26 -> 5, words 229 -> 88).
 *   Residual: (1) s6/s7/s8 permutation (yi s6 vs retail s8; &B[i].pos / &validOption[i] one up) = the holder raises
 *   yi a little too much; (2) the colour word is reloaded from the stack after the first D_8012E700 store (retail
 *   loads it once into v0); (3) frame 120 vs 152: retail has 32 more bytes below `color` (no stack stores there: 8
 *   more spill homes or block-scoped locals; not padded).
 * GC-derived names: see w30b's list (gActiveEdges ... uninitScreen). GC updateButtons locals: buttonIdx, i,
 * prevPos[3] (= old, sp+124), selectedPos (= sel_y), clearScrollSpeed (s8, = flag at sp+119), whichItem, buttonY
 * (= yi, int), tex, texIdx (u16, sp+102), buttonColor (union, sp+96). `u0` below is unused (part of the named-local
 * block that puts old/flag/tex/color at retail's offsets relative to each other; frame still short, see above).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef union {
    u8 c[4];
    u32 rgba;
} Color;

typedef struct {
    s32 type;            /* 0x00 */
    s32 handle;          /* 0x04 */
    f32 ang;             /* 0x08 */
    f32 mat[3][3];       /* 0x0C */
    f32 pos[3];          /* 0x30 */
    u8 alpha;            /* 0x3C */
    u8 pad3D[3];
} Button; /* 0x40 */

typedef struct {
    u8 pad0[60];
    Color color;         /* 0x3C */
    u8 pad40[4];
} Model68;

extern s32 D_80116D0C;
extern s32 D_80116D10;
extern s32 D_80116D14[];
extern s16 D_80116D9C;
extern Color D_80116DB0;
extern Button D_8011650C[];
extern s16 D_80149D9C;
extern f32 D_8011418C[];
extern volatile f32 D_8002EB94;
extern volatile u8 gNumModels;
extern Model68 D_8012E700[];

f32 fabsf(f32);
#pragma intrinsic(fabsf)
void *memcpy(void *, const void *, u32);
s32 func_800B24EC();
void func_8008D870(s16 handle, s32 tex, s32 c);
void func_800B5898(f32 angle, f32 uv[][3]);
void func_800B5940(f32 angle, f32 uv[][3]);
void func_8008B32C(f32 (*dst)[3], f32 (*src)[3], f32 s);
void model_data_load();
void model_transform_setup();

void func_800DA2C0(void) {
    static f32 D_8015273C;
    static f32 D_80152740;
    s32 i;
    s32 j;
    s32 n;
    f32 y;
    f32 old[3];
    f32 sel_y;
    s8 flag;
    s32 name;
    s32 yi;
    s32 u0;
    s16 tex;
    Color color;

    flag = 1;
    color.rgba = D_80116DB0.rgba;
    if (D_80116D0C == 0)
        return;
    n = 0;
    for (j = 0; j < D_80116D9C; j++) {
        if (D_80116D14[j] != 0)
            n++;
    }
    memcpy(old, D_8011650C[28].pos, 12);
    D_8011650C[28].ang += 6.2831855f * D_8002EB94;
    if (6.2831855f < D_8011650C[28].ang)
        D_8011650C[28].ang -= 6.2831855f;
    memcpy(D_8011650C[28].mat, D_8011418C, 36);
    sel_y = 75.0f - n * 165.0f / (D_80149D9C - 1);
    D_8011650C[28].pos[0] = -160.0f;
    D_8011650C[28].pos[2] = 225.0f;
    D_8011650C[28].pos[1] = sel_y;
    if (D_80116D10 == 0) {
        y = D_8011650C[28].pos[1];
        if (D_8015273C < fabsf(y - old[1]) * 60.0f / 10.0f) {
            D_8015273C = fabsf(y - old[1]) * 60.0f / 10.0f;
            if (!(D_8015273C < 540.0f))
                D_8015273C = 540.0f;
            if (!(180.0f < D_8015273C))
                D_8015273C = 180.0f;
        }
        if (old[1] + D_8015273C * D_8002EB94 < y)
            D_8011650C[28].pos[1] = old[1] + D_8015273C * D_8002EB94;
        else if (y < old[1] - D_8015273C * D_8002EB94)
            D_8011650C[28].pos[1] = old[1] - D_8015273C * D_8002EB94;
        else
            D_8015273C = 180.0f;
    }
    func_800B5940(D_8011650C[28].ang, D_8011650C[28].mat);
    func_800B5898(1.5707964f, D_8011650C[28].mat);
    func_8008B32C(D_8011650C[28].mat, D_8011650C[28].mat, 0.4f);
    yi = sel_y * 1.33f + n * 40;
    for (i = 0; i < 14; i++) {
        if (i == D_80116D9C) {
            if (D_8011650C[i].ang < 3.1415927f)
                D_8011650C[i].ang += 12.566371f * D_8002EB94;
            if (3.1415927f < D_8011650C[i].ang)
                D_8011650C[i].ang = 3.1415927f;
            if (D_80116D10 != 0)
                D_8011650C[i].ang = 3.1415927f;
        } else {
            if (0.0f < D_8011650C[i].ang)
                D_8011650C[i].ang -= 12.566371f * D_8002EB94;
            if (D_8011650C[i].ang < 0.0f)
                D_8011650C[i].ang = 0.0f;
            if (D_80116D10 != 0)
                D_8011650C[i].ang = 0.0f;
        }
        D_8011650C[i + 14].ang = D_8011650C[i].ang;
        memcpy(D_8011650C[i].mat, D_8011418C, 36);
        memcpy(D_8011650C[i + 14].mat, D_8011418C, 36);
        memcpy(old, D_8011650C[i].pos, 12);
        D_8011650C[i].pos[0] = -90.0f;
        D_8011650C[i].pos[1] = yi;
        D_8011650C[i].pos[2] = 300.0f;
        if (D_80116D10 == 0) {
            if (D_80152740 < fabsf(D_8011650C[i].pos[1] - old[1]) * 60.0f / 10.0f) {
                D_80152740 = fabsf(D_8011650C[i].pos[1] - old[1]) * 60.0f / 10.0f;
                if (!(D_80152740 < 540.0f))
                    D_80152740 = 540.0f;
            }
            if (old[1] + D_80152740 * D_8002EB94 < D_8011650C[i].pos[1]) {
                flag = 0;
                D_8011650C[i].pos[1] = old[1] + D_80152740 * D_8002EB94;
            } else if (D_8011650C[i].pos[1] < old[1] - D_80152740 * D_8002EB94) {
                flag = 0;
                D_8011650C[i].pos[1] = old[1] - D_80152740 * D_8002EB94;
            }
        }
        D_8011650C[i + 14].pos[0] = 150.0f;
        D_8011650C[i + 14].pos[1] = D_8011650C[i].pos[1] * 31.0f / 30.0f;
        D_8011650C[i + 14].pos[2] = 310.0f;
        func_800B5898(D_8011650C[i].ang - 1.5707964f, D_8011650C[i].mat);
        func_800B5898(D_8011650C[i + 14].ang - 1.5707964f, D_8011650C[i + 14].mat);
        if (1.5707964f < D_8011650C[i].ang)
            name = func_800B24EC("BUTTON_SELECT", &tex, 0, (s8)(gNumModels - 1), 1);
        else
            name = func_800B24EC("BUTTON", &tex, 0, (s8)(gNumModels - 1), 1);
        func_8008D870(D_8011650C[i].handle, name, -1);
        func_8008D870(D_8011650C[i + 14].handle, name, -1);
        if (140.0f < D_8011650C[i].pos[1] || D_8011650C[i].pos[1] < -160.0f)
            D_8011650C[i].alpha = 0;
        else if (100.0f < D_8011650C[i].pos[1])
            D_8011650C[i].alpha = 255.0f - (D_8011650C[i].pos[1] - 100.0f) * 255 / 40.0f;
        else if (D_8011650C[i].pos[1] < -120.0f)
            D_8011650C[i].alpha = 255.0f - (-120.0f - D_8011650C[i].pos[1]) * 255 / 40.0f;
        else
            D_8011650C[i].alpha = 255;
        color.c[3] = D_8011650C[i].alpha;
        D_8011650C[i + 14].alpha = D_8011650C[i].alpha;
        D_8012E700[(s16)D_8011650C[i].handle].color.rgba = color.rgba;
        D_8012E700[(s16)D_8011650C[i + 14].handle].color.rgba = color.rgba;
        if (D_80116D14[i] == 0) {
            model_data_load(D_8011650C[i].handle, 0, 15);
            model_data_load(D_8011650C[i + 14].handle, 0, 15);
            D_8011650C[i].alpha = 0;
        } else {
            if (D_8011650C[i].alpha == 0) {
                model_data_load(D_8011650C[i].handle, 0, 15);
                model_data_load(D_8011650C[i + 14].handle, 0, 15);
            } else {
                model_transform_setup(D_8011650C[i].handle, 0, 15);
                model_transform_setup(D_8011650C[i + 14].handle, 0, 15);
            }
            yi -= 40.0f;
            if (yi) {}
        }
    }
    if (flag == 1)
        D_80152740 = 0.0f;
    D_80116D10 = 0;
}


typedef struct {
    volatile s8 b0;
    u8 pad[15];
} Pad16;

extern u32 D_801174B4;
extern s32 D_801174B8;
extern s32 D_8015698C;
extern s32 D_8015694C;
extern s32 D_80149784;
extern s32 D_80156998[];
extern s32 D_80143A00[];
extern s8 D_80116D94;
extern s32 D_80116DA0;
extern s16 D_80149B84;
extern s16 D_80149DA2;
extern s8 D_80146108[];
extern s16 D_8013FEC8;
extern Pad16 D_80156CF0[];
extern s8 D_80116DA8;
extern s8 D_80157244;

void replay_save_prompt(void);
void resource_type_select(s32);
void audio_distance_atten(s32);
void audio_doppler(s32);
void func_800A510C(void);
void init_state_begin(void);
void time_result_display(void);
void func_800DA0BC(void);
void func_800B4FB0(s32);

void func_800DB1E0(void) {
    Pad16 *p;
    s32 btn;
    s32 rep;
    s32 dir;
    s32 x;

    if (D_801174B4 & 0x7C03FFFE) {
        btn = D_8015694C;
        rep = D_80149784;
    } else {
        btn = D_80156998[D_8015698C];
        rep = D_80143A00[D_8015698C];
    }
    if (D_80116D94 == 0)
        replay_save_prompt();
    if (btn & 5) {
        resource_type_select(btn);
        D_80116DA0 = 2;
    } else if (rep & 0x400) {
        audio_distance_atten(rep);
        do {
            D_80116D9C--;
            if (D_80116D9C < 0)
                D_80116D9C = 13;
        } while (!D_80116D14[D_80116D9C]);
    } else if (rep & 0x800) {
        audio_distance_atten(rep);
        do {
            D_80116D9C++;
            if (D_80116D9C >= 14)
                D_80116D9C = 0;
        } while (!D_80116D14[D_80116D9C]);
    }
    x = D_80116D9C - D_80149B84 / 2;
    if (D_80149DA2 < x)
        D_80149DA2 = x;
    if (x < D_80149DA2)
        D_80149DA2 = x;
    if (14 - D_80149B84 < D_80149DA2)
        D_80149DA2 = 14 - D_80149B84;
    if (D_80149DA2 < 0)
        D_80149DA2 = 0;
    if (btn & 0x3000) {
        dir = (btn & 0x1000) ? -1 : 1;
        audio_doppler(btn);
        switch (D_80116D9C) {
        case 1:
            D_80146108[10] = !D_80146108[10];
            break;
        case 2:
            D_80146108[0] ^= 1;
            break;
        case 3:
            D_80146108[1] ^= 1;
            break;
        case 4:
            D_80146108[2] ^= 1;
            break;
        case 5:
            D_80146108[3] ^= 1;
            break;
        case 6:
            D_80146108[4] ^= 1;
            break;
        case 7:
            D_80146108[5] ^= 1;
            break;
        case 8:
            D_80146108[6] ^= 1;
            break;
        case 9:
            D_80146108[7] ^= 1;
            break;
        case 10:
            D_80146108[8] ^= 1;
            break;
        case 11:
            D_80146108[9] ^= 1;
            break;
        case 12:
            D_80146108[18] ^= 1;
            break;
        case 13:
            D_80146108[11] ^= 1;
            break;
        case 0:
            D_8013FEC8 += dir;
            if (D_8013FEC8 < 0)
                D_8013FEC8 = 5;
            else if (D_8013FEC8 >= 6)
                D_8013FEC8 = 0;
            D_80146108[16] = D_8013FEC8;
            D_80146108[17] = 1;
            func_800A510C();
            break;
        }
        if (!(D_801174B4 & 0x7C03FFFE))
            init_state_begin();
    }
    func_800DA2C0();
    if (D_801174B4 & 0x7C0000) {
        p = &D_80156CF0[D_8015698C];
        if (!p->b0 != D_80116DA8) {
            D_80116DA8 = !p->b0;
            time_result_display();
        }
    }
    if (D_80157244) {
        func_800DA0BC();
        if (D_801174B4 & 0x7C03FFFE)
            D_801174B8 = 4;
        else
            func_800B4FB0(1);
    } else if (D_80116DA0) {
        func_800DA0BC();
        if (D_801174B4 & 0x7C03FFFE)
            D_801174B8 = 16;
        else
            func_800B4FB0(1);
    }
}
