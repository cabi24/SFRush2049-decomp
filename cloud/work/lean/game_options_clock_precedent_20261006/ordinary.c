/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Complete native reconstruction of the 14-entry options-menu animation.
 * Unmatched research: no padding, stand-in callers or synthetic operations.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct Button {
    s32 tag;
    s32 handle;
    f32 angle;
    f32 matrix[3][3];
    f32 position[3];
    u8 alpha;
    u8 reserved[3];
} Button;
typedef struct RenderObject {
    u8 prefix[60];
    u32 color;
    u32 suffix;
} RenderObject;
typedef union Color {
    u32 word;
    u8 channel[4];
} Color;
typedef struct TexDef TexDef;
extern Button D_8011650C[29];
extern s32 D_80116D0C, D_80116D10, D_80116D14[14];
extern s16 D_80116D9C, D_80149D9C;
extern u32 D_80116DB0;
extern f32 D_8015273C, D_80152740;
extern f32 D_8002EB94;
extern f32 D_8011418C[3][3];
extern u8 D_80140BDC;
extern RenderObject D_8012E700[];
extern char D_80121048[], D_80121058[];
void *memcpy(void *, const void *, u32);
TexDef *func_800B24EC(char *, s16 *, s8, s8, s32);
void func_800B5898(f32, f32 [3][3]);
void func_800B5940(f32, f32 [3][3]);
void func_8008B32C(f32 [3][3], f32 [3][3], f32);
void func_8008D870(s16, TexDef *, s32);
void model_transform_setup(s32, s32, s32);
void model_data_load(s32, s32, s32);
f32 fabsf(f32);
#pragma intrinsic(fabsf)

static __inline TexDef *MBOX_FindTexture_Err(char *name, s16 *index, s32 err) {
    return func_800B24EC(name, index, 0, D_80140BDC - 1, err);
}

void func_800DA2C0(void) {
    s32 i;
    s32 countIndex;
    s32 enabled;
    s32 y;
    f32 targetY;
    f32 difference;
    f32 position[3];
    TexDef *texture;
    s16 textureIndex;
    s8 settled;
    Color color;
    settled = 1;
    color.word = D_80116DB0;
    if (D_80116D0C == 0) {
        return;
    }
    enabled = 0;
    for (countIndex = 0; countIndex < D_80116D9C; countIndex++) {
        if (D_80116D14[countIndex] != 0) {
            enabled++;
        }
    }
    memcpy(position, D_8011650C[28].position, 12);
    D_8011650C[28].angle += 6.2831855f * D_8002EB94;
    if (D_8011650C[28].angle > 6.2831855f) {
        D_8011650C[28].angle -= 6.2831855f;
    }
    memcpy(D_8011650C[28].matrix, D_8011418C, 36);
    targetY = 75.0f - enabled * 165.0f / (D_80149D9C - 1);
    D_8011650C[28].position[0] = -160.0f;
    D_8011650C[28].position[1] = targetY;
    D_8011650C[28].position[2] = 225.0f;
    if (D_80116D10 == 0) {
        difference = fabsf(D_8011650C[28].position[1] - position[1]);
        if (D_8015273C < difference * 60.0f / 10.0f) {
            D_8015273C = (difference * 60.0f / 10.0f < 540.0f ? difference * 60.0f / 10.0f : 540.0f);
            D_8015273C = (180.0f < D_8015273C ? D_8015273C : 180.0f);
        }
        if (position[1] + D_8015273C * D_8002EB94 < D_8011650C[28].position[1]) {
            D_8011650C[28].position[1] = position[1] + D_8015273C * D_8002EB94;
        } else if (D_8011650C[28].position[1] < position[1] - D_8015273C * D_8002EB94) {
            D_8011650C[28].position[1] = position[1] - D_8015273C * D_8002EB94;
        } else {
            D_8015273C = 180.0f;
        }
    }
    func_800B5940(D_8011650C[28].angle, D_8011650C[28].matrix);
    func_800B5898(1.5707964f, D_8011650C[28].matrix);
    func_8008B32C(D_8011650C[28].matrix, D_8011650C[28].matrix, 0.4f);
    y = targetY * 1.33f + enabled * 40;
    for (i = 0; i < 14; i++) {
        if (i == D_80116D9C) {
            if (D_8011650C[i].angle < 3.1415927f) {
                D_8011650C[i].angle += 12.566371f * D_8002EB94;
            }
            if (D_8011650C[i].angle > 3.1415927f) {
                D_8011650C[i].angle = 3.1415927f;
            }
            if (D_80116D10 != 0) {
                D_8011650C[i].angle = 3.1415927f;
            }
        } else {
            if (D_8011650C[i].angle > 0.0f) {
                D_8011650C[i].angle -= 12.566371f * D_8002EB94;
            }
            if (D_8011650C[i].angle < 0.0f) {
                D_8011650C[i].angle = 0.0f;
            }
            if (D_80116D10 != 0) {
                D_8011650C[i].angle = 0.0f;
            }
        }
        D_8011650C[i + 14].angle = D_8011650C[i].angle;
        memcpy(D_8011650C[i].matrix, D_8011418C, 36);
        memcpy(D_8011650C[i + 14].matrix, D_8011418C, 36);
        memcpy(position, D_8011650C[i].position, 12);
        D_8011650C[i].position[0] = -90.0f;
        D_8011650C[i].position[1] = y;
        D_8011650C[i].position[2] = 300.0f;
        if (D_80116D10 == 0) {
            difference = fabsf(D_8011650C[i].position[1] - position[1]);
            if (D_80152740 < difference * 60.0f / 10.0f) {
                D_80152740 = (difference * 60.0f / 10.0f < 540.0f ? difference * 60.0f / 10.0f : 540.0f);
            }
            if (position[1] + D_80152740 * D_8002EB94 < D_8011650C[i].position[1]) {
                D_8011650C[i].position[1] = position[1] + D_80152740 * D_8002EB94;
                settled = 0;
            } else if (D_8011650C[i].position[1] < position[1] - D_80152740 * D_8002EB94) {
                D_8011650C[i].position[1] = position[1] - D_80152740 * D_8002EB94;
                settled = 0;
            }
        }
        D_8011650C[i + 14].position[0] = 150.0f;
        D_8011650C[i + 14].position[1] = D_8011650C[i].position[1] * 31.0f / 30.0f;
        D_8011650C[i + 14].position[2] = 310.0f;
        func_800B5898(D_8011650C[i].angle - 1.5707964f, D_8011650C[i].matrix);
        func_800B5898(D_8011650C[i + 14].angle - 1.5707964f, D_8011650C[i + 14].matrix);
        if (D_8011650C[i].angle > 1.5707964f) {
            texture = MBOX_FindTexture_Err(D_80121048, &textureIndex, 1);
        } else {
            texture = MBOX_FindTexture_Err(D_80121058, &textureIndex, 1);
        }
        func_8008D870(D_8011650C[i].handle, texture, -1);
        func_8008D870(D_8011650C[i + 14].handle, texture, -1);
        if (D_8011650C[i].position[1] > 140.0f || D_8011650C[i].position[1] < -160.0f) {
            D_8011650C[i].alpha = 0;
        } else if (D_8011650C[i].position[1] > 100.0f) {
            D_8011650C[i].alpha = (u32)(255.0f - (D_8011650C[i].position[1] - 100.0f) * 255.0f / 40.0f);
        } else if (D_8011650C[i].position[1] < -120.0f) {
            D_8011650C[i].alpha = (u32)(255.0f - (-120.0f - D_8011650C[i].position[1]) * 255.0f / 40.0f);
        } else {
            D_8011650C[i].alpha = 255;
        }
        color.channel[3] = D_8011650C[i].alpha;
        D_8011650C[i + 14].alpha = D_8011650C[i].alpha;
        D_8012E700[(s16)D_8011650C[i].handle].color = color.word;
        D_8012E700[(s16)D_8011650C[i + 14].handle].color = color.word;
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
            y -= 40.0f;
        }
    }
    if (settled == 1) {
        D_80152740 = 0.0f;
    }
    D_80116D10 = 0;
}
