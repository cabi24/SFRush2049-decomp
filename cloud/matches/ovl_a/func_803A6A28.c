/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Standalone match: no callers, inlined helpers, or deleted-static stubs required in this unit. */
/* Image A menu text callback. N64-specific rendering; no arcade donor found.
 * Complete native extent: [0x803A6A28, 0x803A6B50), 296 bytes.
 * Typed callback and draw-state sequence follow the witnessed A:803AE51C
 * sibling. The unused argument is part of the externally addressed callback.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct MenuText {
    u8 pad0[920];
    u8 *text920;
    u8 pad924[12];
    u8 *text936;
} MenuText;
extern MenuText *D_8017A4E4;
extern u32 D_80156978;
extern s8 D_80156994;
extern u8 D_803B87E8[];
extern s32 D_8002E440, D_8002E444;
void render_helper(f32);
void *object_create(s32);
s8 object_byte9_set(s8);
void mode_byte_set(s16);
void func_800B669C(u32, u32);
void dispatch_handler(s32);
void camera_auto_follow(s16, s16, s16, s16, s16, s16, u8 *);
void music_tempo_adjust(s16, s16, u8 *, ...);
void state_utility(s16, s16, void *);

s32 func_803A6A28(s32 arg0)
{
    render_helper(0.0f);
    object_create(0);
    object_byte9_set(0);
    mode_byte_set(2);
    func_800B669C(1, 1);
    dispatch_handler(1);
    camera_auto_follow(160, 165, 288, 110, -1, 0, D_8017A4E4->text936);
    if (D_80156978 == 0x3C000) {
        dispatch_handler(14);
        music_tempo_adjust(210, 30, D_803B87E8, D_8002E440, D_8002E444);
    }
    if (D_80156994 != 0) {
        dispatch_handler(14);
        state_utility(160, 10, D_8017A4E4->text920);
    }
    func_800B669C(0, 3);
    object_byte9_set(1);
    mode_byte_set(-1);
    render_helper(-1.0f);
    return 1;
}
