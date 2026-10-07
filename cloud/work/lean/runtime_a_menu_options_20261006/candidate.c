/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Image A: draw a seven-item menu with optional projected positions. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct Color4 { u32 word; } Color4;
typedef struct Language { s32 unknown0; char **text; s32 unknown8; u16 *indices; char **indexed; } Language;
typedef struct Slot { f32 x,y,angle; u8 unknown0C[36]; f32 position[3]; u8 unknown3C[4]; } Slot;
typedef struct Camera { f32 uv[3][3], position[3]; u8 unknown30[104]; } Camera;
extern Language countdown_state;
extern s32 D_803B4634, D_803B469C;
extern f32 D_803B4624[3];
extern Camera D_80150B70[];
extern s16 D_8014A108;
extern Slot D_803B42F4[];
extern char D_803B8854[], D_803B885C[];
extern Color4 D_801146BC, D_801146C0;
extern void render_helper(f32);
extern s32 object_create(s32);
extern void dispatch_handler(s32);
extern void func_800B669C(u32,u32);
extern void brake_light_update(s32,f32 *,Camera *,void *,s16 *);
extern s16 sound_pitch_diff_halved(void *,s16);
extern s16 object_bytes_sum_global(void);
extern void state_utility(s16,s16,void *);
extern void fcvt_wrapper(char *,char *,...);
extern s32 func_80399394(s32);
extern void func_800ED66C(f32);
extern void func_800BEA3C(Color4,Color4);

s32 func_803A6B50(void *callback_context)
{
    char text[50];
    s16 point[2];
    s16 x, y;
    s32 i;
    char *title;
    Slot *slot;
    render_helper(0.0f);
    object_create(13);
    dispatch_handler(1);
    x = 160;
    y = 10;
    if (D_803B4634 == 1) {
        func_800B669C(0, 1);
        brake_light_update(0, D_803B4624, &D_80150B70[0], 0, point);
        x = point[0];
        y = point[1];
    }
    title = countdown_state.text[40];
    state_utility(sound_pitch_diff_halved(title, x), y, title);
    y += object_bytes_sum_global() * 2;
    if (D_8014A108 == 1) {
        fcvt_wrapper(text, D_803B8854, D_8014A108, countdown_state.text[29]);
    } else {
        fcvt_wrapper(text, D_803B885C, D_8014A108, countdown_state.indexed[countdown_state.indices[20]]);
    }
    dispatch_handler(10);
    y += object_bytes_sum_global() * 2;
    object_create(10);
    for (i = 0; i < 7; i++) {
        if (func_80399394(i)) {
            dispatch_handler(i == D_803B469C ? 22 : 1);
            if (D_803B4634 == 1) {
                slot = &D_803B42F4[i];
                if (slot->angle > 0.6981317f && slot->angle < 2.443461f) continue;
                if (slot->angle > 0.34906584f && slot->angle < 2.7925267219543457f) {
                    func_800ED66C(128.0f);
                } else {
                    func_800ED66C(-1.0f);
                }
                if (slot->angle < 1.5707964f) {
                    func_800BEA3C(D_801146BC, D_801146C0);
                    dispatch_handler(1);
                } else {
                    func_800BEA3C(D_801146BC, D_801146C0);
                    dispatch_handler(22);
                }
                brake_light_update(0, slot->position, &D_80150B70[0], 0, point);
                x = point[0];
                y = point[1];
                x -= 45;
            }
            state_utility(x, y, countdown_state.indexed[countdown_state.indices[22] + i]);
            y += object_bytes_sum_global();
        }
    }
    func_800B669C(0, 3);
    func_800ED66C(-1.0f);
    func_800BEA3C(D_801146BC, D_801146C0);
    render_helper(-1.0f);
    return 1;
}
