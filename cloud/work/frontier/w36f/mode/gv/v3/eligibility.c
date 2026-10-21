/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct Slot64 {
    s32 unknown0,id;
    f32 angle,matrix[3][3],position[3];
    s32 unknown3C;
} Slot64;
typedef struct Resource { u8 unknown0[16]; u8 player; } Resource;
typedef struct Owner { u32 unknown0,unknown4; Resource **resource; } Owner;
typedef struct Player76 { u8 unknown0[72]; Owner **owner; } Player76;
typedef struct Blit Blit;
extern s16 D_8014A108;
extern s8 D_803B4679[][7],D_803B4678,D_80157244;
extern s32 D_803B4634,D_803B4638,D_803B469C,D_803B46A0,D_8014A110;
extern u32 D_801174BC,D_801174B4,D_8015694C;
extern f32 D_803B463C,D_803B4640,D_803B4644,D_803B4648,D_803B464C,D_803B4650;
extern f32 D_803BAC18,D_8011418C[3][3];
extern volatile f32 D_8002EB94;
extern Slot64 D_803B42F4[13];
extern volatile u8 D_80140BDC;
extern char D_803B8884[],D_803B8894[];
extern Player76 D_8014A118[];
extern Blit *D_803B467C;
extern s32 func_800F7644(s32);
extern s8 func_800A361C(s32);
extern void *memcpy(void *,const void *,u32);
extern void math_utility(void *,void *);
extern void func_800B5898(f32,f32 (*)[3]);
extern void func_800B5940(f32,f32 (*)[3]);
extern void func_8008B32C(f32 (*)[3],f32 (*)[3],f32);
extern void func_8008D870(s16,s32,s32);
extern void *func_800B24EC(char *,s16 *,s32,s8,s32);
extern void model_data_load(s32,s32,u32),model_transform_setup(s32,s32,u32);
extern void func_800B5570(s32),func_803998C0(void);
extern void resource_type_select(u32),audio_distance_atten(u32);
extern void sound_call_minimal(s16),sound_stop(Blit *),ambient_sounds_clear(void);
extern f32 fabsf(f32);
#pragma intrinsic (fabsf)

s32 func_80399394(s32 index)
{
    s32 value=D_803B4679[D_8014A108][index];
    if(index==5 && !func_800F7644(18))value=0;
    return value;
}

void func_803993EC(void)
{
    s32 buttonIdx;
    f32 prevPos[3];
    f32 buttonY;
    f32 selectY;
    void *tex;
    u16 texIdx;
    if (!D_803B4634) return;
    buttonY = D_803B4648;
    for (buttonIdx = 0; buttonIdx < 7; buttonIdx++) {
        if (buttonIdx == D_803B469C) {
            selectY = buttonY;
            if (D_803B42F4[buttonIdx].angle < 3.1415927f) D_803B42F4[buttonIdx].angle += 12.566371f * D_8002EB94;
            if (D_803B42F4[buttonIdx].angle > 3.1415927f || D_803B4638 == 1) D_803B42F4[buttonIdx].angle = 3.1415927f;
        } else {
            if (D_803B42F4[buttonIdx].angle > 0.0f) D_803B42F4[buttonIdx].angle -= 12.566371f * D_8002EB94;
            if (D_803B42F4[buttonIdx].angle < 0.0f || D_803B4638 == 1) D_803B42F4[buttonIdx].angle = 0.0f;
        }
        memcpy(D_803B42F4[buttonIdx].matrix, D_8011418C, 36);
        memcpy(prevPos, D_803B42F4[buttonIdx].position, 12);
        D_803B42F4[buttonIdx].position[0] = D_803B4644;
        D_803B42F4[buttonIdx].position[1] = buttonY;
        D_803B42F4[buttonIdx].position[2] = D_803B464C;
        if (!D_803B4638) {
            if (D_803B42F4[buttonIdx].position[1] > prevPos[1] + 180.0f * D_8002EB94) D_803B42F4[buttonIdx].position[1] = prevPos[1] + 180.0f * D_8002EB94;
            if (D_803B42F4[buttonIdx].position[1] < prevPos[1] - 180.0f * D_8002EB94) D_803B42F4[buttonIdx].position[1] = prevPos[1] - 180.0f * D_8002EB94;
        }
        func_800B5898(D_803B42F4[buttonIdx].angle - 1.5707964f, D_803B42F4[buttonIdx].matrix);
        if (D_803B42F4[buttonIdx].angle > 1.5707964f) tex = func_800B24EC(D_803B8884, (s16 *)&texIdx, 0, D_80140BDC - 1, 1);
        else tex = func_800B24EC(D_803B8894, (s16 *)&texIdx, 0, D_80140BDC - 1, 1);
        func_8008D870((s16)D_803B42F4[buttonIdx].id, (s32)tex, -1);
        if (!func_80399394(buttonIdx)) model_data_load(D_803B42F4[buttonIdx].id, 0, 15);
        else {
            model_transform_setup(D_803B42F4[buttonIdx].id, 0, 15);
            buttonY -= D_803B4640 * D_803B463C;
        }
    }
    memcpy(prevPos, D_803B42F4[7].position, 12);
    D_803B42F4[7].angle += 6.2831855f * D_8002EB94;
    if (D_803B42F4[7].angle > 6.2831855f) D_803B42F4[7].angle -= 6.2831855f;
    math_utility(D_8011418C, D_803B42F4[7].matrix);
    D_803B42F4[7].position[0] = D_803B4644 - 80.0f;
    D_803B42F4[7].position[1] = selectY * D_803B4650 / D_803B464C;
    D_803B42F4[7].position[2] = D_803B4650;
    if (!D_803B4638) {
        if (D_803BAC18 < fabsf(D_803B42F4[7].position[1] - prevPos[1]) * 60.0f / 10.0f) {
            D_803BAC18 = fabsf(D_803B42F4[7].position[1] - prevPos[1]) * 60.0f / 10.0f;
            D_803BAC18 = D_803BAC18 < 540.0f * D_803B463C ? D_803BAC18 : 540.0f * D_803B463C;
            D_803BAC18 = D_803BAC18 > 180.0f * D_803B463C ? D_803BAC18 : 180.0f * D_803B463C;
        }
        if (D_803B42F4[7].position[1] > prevPos[1] + D_803BAC18 * D_8002EB94) D_803B42F4[7].position[1] = prevPos[1] + D_803BAC18 * D_8002EB94;
        else if (D_803B42F4[7].position[1] < prevPos[1] - D_803BAC18 * D_8002EB94) D_803B42F4[7].position[1] = prevPos[1] - D_803BAC18 * D_8002EB94;
        else D_803BAC18 = 180.0f * D_803B463C;
    }
    func_800B5940(D_803B42F4[7].angle, D_803B42F4[7].matrix);
    func_800B5898(1.5707964f, D_803B42F4[7].matrix);
    func_8008B32C(D_803B42F4[7].matrix, D_803B42F4[7].matrix, 0.4f);
    D_803B4638 = 0;
}

void uninitScreen(void);

void func_80399D10(void)
{
    s32 i;
    if (D_801174BC != 1) {
        if (D_801174BC != D_801174B4) {
            if (D_801174BC == 0x4000) func_800B5570(0x4000);
            else func_800B5570(0x80);
            return;
        }
        D_801174BC = 1;
    }
    if (!D_803B4678) func_803998C0();
    while (!func_80399394(D_803B469C)) {
        if (++D_803B469C >= 7) D_803B469C = 0;
    }
    func_803993EC();
    if (D_8015694C & 3) { resource_type_select(D_8015694C); D_803B46A0 = 1; }
    else if (D_8015694C & 4) { resource_type_select(D_8015694C); D_803B46A0 = 2; }
    else if (D_8015694C & 0x400) {
        audio_distance_atten(D_8015694C);
        do { if (--D_803B469C < 0) D_803B469C = 6; } while (!func_80399394(D_803B469C));
    } else if (D_8015694C & 0x800) {
        audio_distance_atten(D_8015694C);
        do { if (++D_803B469C >= 7) D_803B469C = 0; } while (!func_80399394(D_803B469C));
    }
    for (i = 0; i < D_8014A108; i++) {
        Resource **fileh = (*D_8014A118[i].owner)->resource;
        if (fileh && func_800A361C((*fileh)->player)) {
            uninitScreen(); func_800B5570(0x20); return;
        }
    }
    if (D_80157244) { uninitScreen(); func_800B5570(4); return; }
    if (D_803B46A0) {
        uninitScreen();
        switch (D_803B46A0) {
        case 1:
            switch (D_803B469C) {
            case 0: D_8014A110 = 0; func_800B5570(0x80); break;
            case 1: D_8014A110 = 1; func_800B5570(0x80); break;
            case 2: D_8014A110 = 2; func_800B5570(0x80); break;
            case 3: D_8014A110 = 3; func_800B5570(0x4000); break;
            case 4: D_8014A110 = 4; func_800B5570(0x80); break;
            case 5: D_8014A110 = 5; func_800B5570(0x80); break;
            case 6: D_8014A110 = 6; func_800B5570(0x80); break;
            }
            break;
        case 2: func_800B5570(0x20); break;
        }
        D_803B46A0 = 0;
    }
}

void uninitButtons(void);

void uninitScreen(void)
{
    uninitButtons();
    if (D_803B467C) { sound_stop(D_803B467C); D_803B467C = 0; }
    ambient_sounds_clear();
    D_803B4678 = 0;
}

void uninitButtons(void)
{
    s32 buttonIdx;
    for (buttonIdx = 0; buttonIdx < 13; buttonIdx++) {
        if (D_803B42F4[buttonIdx].id != -1) {
            sound_call_minimal(D_803B42F4[buttonIdx].id);
            D_803B42F4[buttonIdx].id = -1;
        }
    }
    D_803B4634 = 0;
}
