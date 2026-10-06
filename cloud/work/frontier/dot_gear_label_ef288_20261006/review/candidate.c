/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Native gear-label HUD callback. Field names describe observed use; no
 * original N64 type/TU claim. The unused first ABI slot is genuinely homed
 * by the native callback. Unknown arrays represent native record storage.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

typedef struct GearModel {
    u8 unknown000[0xA];
    s8 hidden;
    u8 unknown00B[0x725];
    s8 gear;
    u8 unknown731[0x95];
    s16 car_index;
    u8 unknown7C8[0x40];
} GearModel;
typedef struct GearCar {
    u8 unknown000[0xEF];
    s8 kind;
    u8 unknown0F0[0x2C8];
} GearCar;
typedef struct GearPosition {
    s32 x;
    s16 unknown04;
    s16 y;
} GearPosition;
typedef struct GearAssets {
    void *unknown00;
    const char **labels;
} GearAssets;

extern s16 D_80151AD0;
extern s8 D_80161394;
extern GearModel D_8014A250[];
extern GearCar D_80152818[];
extern GearPosition D_80115BE8[][4];
extern GearAssets D_8017A4E0;
extern u8 D_801461D0[];
extern void render_helper(float);
extern s32 osRecvMesg(void *, void *, s32);
extern s32 osJamMesg(void *, void *, s32);
extern s32 slot_state_setup(s32);
extern void dispatch_handler(s32);
extern u32 object_utility(const char *, s32);
extern void state_utility(s16, s16, const char *);

s32 func_800EF288(u32 callback_argument)
{
    GearModel *model;
    GearPosition *position;
    s32 i;
    s8 gear;
    s16 x;
    s16 y;
    u32 width;
    char text[2];

    render_helper(0.0f);
    if (D_80151AD0 == 1) {
        osRecvMesg(D_801461D0, 0, 1);
        slot_state_setup(10);
        osJamMesg(D_801461D0, 0, 0);
    } else if (D_80151AD0 == 2) {
        osRecvMesg(D_801461D0, 0, 1);
        slot_state_setup(11);
        osJamMesg(D_801461D0, 0, 0);
    } else {
        osRecvMesg(D_801461D0, 0, 1);
        slot_state_setup(12);
        osJamMesg(D_801461D0, 0, 0);
    }
    model = D_8014A250;
    for (i = 0; i < D_80151AD0; i++, model++) {
        gear = model->gear;
        if (D_80161394 && !model->hidden &&
            D_80152818[model->car_index].kind != 1) {
            position = &D_80115BE8[D_80151AD0 - 1][i];
            x = (s16)((u32)position->x - 4U);
            y = position->y;
            dispatch_handler(0);
            width = object_utility(D_8017A4E0.labels[204], -1);
            state_utility((s16)((u32)x + 1U - width),
                          (s16)(y + 1), D_8017A4E0.labels[204]);
            dispatch_handler(1);
            width = object_utility(D_8017A4E0.labels[204], -1);
            state_utility((s16)((u32)x - width),
                          y, D_8017A4E0.labels[204]);
            position = &D_80115BE8[D_80151AD0 - 1][i];
            x = (s16)((u32)position->x + 4U);
            y = position->y;
            if (gear == -1) {
                text[0] = 'R';
            } else if (gear == 0) {
                text[0] = 'N';
            } else {
                text[0] = (char)(gear + '0');
            }
            text[1] = '\0';
            dispatch_handler(0);
            state_utility((s16)(x + 1), (s16)(y + 1), text);
            dispatch_handler(14);
            state_utility(x, y, text);
        }
    }
    render_helper(-1.0f);
    return 1;
}
