/* billboard_render (0x800F64D4, 976 B): results-screen / save-ghost state machine (the label is
 * historical). If the screen is not yet set up, loads it; a pending abort returns to state 0. Then
 * switch on D_80140B20: 1 func_800F2718; 2 per-player "press to continue" (func_800F1210(3));
 * 3 per-player name entry (func_800F207C) and slot cleanup, then either the save prompt
 * (func_800F1210(4)) or the records table (5); 4 save input (func_800F1930); 5 records table:
 * toggle the shown list (D_801148D0) and redraw it (func_800F0914), or continue / leave.
 * State changes go through func_800F1210, whose new state is a register parameter ($s2).
 *
 * Source: cloud/work/frontier/dot_billboard_context_20261005 (Astra's genuine caller), with three
 * w15f changes:
 *  - the state-2 loop uses its own counter `j` (retail gives the two loops different webs:
 *    state 2 counter s7 / record pointer s6, state 3 counter s6 / slot pointer s7);
 *  - state 3 clears a slot through `h = &D_80149418[i]` (same code as the indexed form);
 *  - SHAPING (disclosed): eight unused s32 locals u1..u8. They are the exact frame residual:
 *    retail's frame is 144 bytes with named-local slots down to sp+96 (flag at sp+112, the
 *    state-2 counter's home at sp+100); every function-level named local takes one slot, used or
 *    not, and these eight put flag and j at retail's offsets. Without them the code is identical
 *    but the frame is 104 bytes (13 rows: frame, homes).
 * Its frame and register choice also depend on the IPA clobber sets of its internal callees: the
 * group's context func_800F0914 (clobbers s0-s8 and $f20/$f22, as retail) and func_800F1210. */
typedef signed int s32;
typedef signed short s16;
typedef signed char s8;
typedef unsigned int u32;

/* 76-byte records at input_rec0 */
typedef struct Rec76 { s32 w0; u32 w4; s32 rest[17]; } Rec76;

extern s8 D_801148A4;
extern s8 D_80157244;
extern s32 D_80140B20;              /* state, dispatched as state-1 (5 cases) */
extern s16 active_player_count;              /* record count */
extern Rec76 input_rec0[];
extern s8 D_80143F18[];
extern s8 *D_80149418[];
extern s32 D_8014A110;
extern s32 D_80140800;
extern s8 D_8014978C;
extern void *D_801461A8;
extern s8 D_801148E4;
extern s8 D_801460C0;
extern void *D_8014A160;
extern u32 D_8015694C;
extern s8 D_801148D0;

extern void track_texture_load(void);
extern void func_800C813C(s32, s32);
extern void visual_objects_update(s32);
extern void func_800F1210(s32);
extern void func_800F2718(void);
extern s32 viDeadlinePassed(void);
extern void resource_type_select(u32);
extern void func_800F207C(s32);
extern void sound_handles_array_clear(s8 *);
extern void wheel_setup_initial(s32);
extern char D_803BA7EB[];
extern void *func_800CC50C(s32, char *);
extern s32 func_800CC040(s32, void *, void *, s32);
extern void func_800F1930(void);
extern void audio_doppler(u32);
extern void func_800F0914(void);
extern void viScheduleTick(float);

void billboard_render(void)
{
    s8 **h;
    s32 u1, u2, u3, u4, u5, u6;
    s32 flag;
    s32 u7, u8;
    s32 j;
    s32 i;

    if (D_801148A4 == 0) {
        track_texture_load();
        func_800C813C(1, 0);
        visual_objects_update(0);
    }
    if (D_80157244 != 0) {
        func_800F1210(0);
        return;
    }
    switch (D_80140B20) {
    case 1:
        func_800F2718();
        break;
    case 2:
        for (j = 0; j < active_player_count; j++) {
            if ((input_rec0[j].w4 & 3) != 0 || viDeadlinePassed() != 0) {
                resource_type_select(input_rec0[j].w4);
                func_800F1210(3);
            }
        }
        break;
    case 3:
        flag = 1;
        for (i = 0; i < active_player_count; i++) {
            if (D_80143F18[i] == 1) {
                func_800F207C(i);
                flag = 0;
            } else {
                h = &D_80149418[i];
                if (*h != 0) {
                    sound_handles_array_clear(*h);
                    *h = 0;
                }
            }
        }
        if (flag == 1) {
            if (D_8014A110 == 2 && D_80140800 != 0) {
                wheel_setup_initial(D_8014978C + 139);
                D_801461A8 = func_800CC50C(D_80140800, D_803BA7EB);
                D_80140800 = 0;
                if (D_801148E4 != 0) {
                    func_800F1210(4);
                } else {
                    func_800CC040(D_801460C0, D_8014A160, D_801461A8, 1);
                    D_801461A8 = 0;
                    func_800F1210(5);
                }
            } else {
                func_800F1210(5);
            }
        }
        break;
    case 4:
        func_800C813C(0, 0);
        func_800F1930();
        break;
    case 5:
        if (D_8014A110 != 2 && (D_8015694C & 0x3000) != 0) {
            audio_doppler(D_8015694C);
            if (D_801148D0 == 0) D_801148D0 = 1; else D_801148D0 = 0;
            func_800F0914();
            viScheduleTick(15.0f);
        } else if ((D_8015694C & 3) != 0 || viDeadlinePassed() != 0) {
            if (D_8014A110 != 2 && viDeadlinePassed() != 0 && D_801148D0 == 0) {
                D_801148D0 = 1;
                func_800F0914();
                viScheduleTick(15.0f);
            } else {
                resource_type_select(D_8015694C);
                func_800F1210(0);
            }
        }
        break;
    }
}
