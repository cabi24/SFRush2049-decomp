/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* Image A formatted menu callback, [0x803AF744, 0x803AF978).
 * N64-specific renderer; witnessed sibling: A:803AE51C.
 * The callback argument is unused but homed by the original O32 body.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct MenuText {
    u8 pad0[344];
    char *text344;
    u8 pad348[4];
    char *text352;
    u8 pad356[4];
    char *text360;
    u8 pad364[244];
    char *text608;
    char *text612;
    char *text616;
    u8 pad620[4];
    char *text624;
    u8 pad628[316];
    char *text944;
} MenuText;
extern MenuText *D_8017A4E4;
extern u8 D_803B83A0;
extern s8 D_803B839C;
extern char D_803B92B4[];
void render_helper(f32);
void func_800B669C(u32, u32);
void *object_create(s32);
void dispatch_handler(s32);
s32 func_800A3508(s32);
s32 sprintf(char *, const char *, ...);
void camera_auto_follow(s16, s16, s16, s16, s16, s16, void *);
s16 object_bytes_sum_global(void);
void state_utility(s16, s16, void *);

s32 func_803AF744(s32 arg0)
{
    char buffer[256];
    s32 y;
    s32 height;
    render_helper(0.0f);
    func_800B669C(1, 1);
    object_create(11);
    dispatch_handler(1);
    sprintf(buffer, D_803B92B4,
            D_803B83A0 > 0 ? D_8017A4E4->text612 : D_8017A4E4->text608, D_8017A4E4->text616,
            D_8017A4E4->text352, D_8017A4E4->text360,
            func_800A3508(0x810), D_8017A4E4->text344);
    camera_auto_follow(160, 90, 300, 220, -1, 0, buffer);
    height = object_bytes_sum_global();
    y = 200 - height * 3;
    object_create(D_803B839C ? 11 : 10);
    dispatch_handler(D_803B839C ? 1 : 22);
    state_utility(160, y - (D_803B839C ? 0 : 1), D_8017A4E4->text944);
    if (D_803B83A0 > 0) {
        object_create(D_803B839C ? 10 : 11);
        dispatch_handler(D_803B839C ? 22 : 1);
        state_utility(160, y - (D_803B839C ? 1 : 0) + height, D_8017A4E4->text624);
    }
    func_800B669C(0, 3);
    render_helper(-1.0f);
    return 1;
}
