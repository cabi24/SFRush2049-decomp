/* Genuine full caller from cloud/work/ipa-groups/billboard_render/br.c.
 * Player-table/count symbols and native-backed pointer interfaces normalized.
 * This caller is NONMATCH;
 * F1210 and every other unavailable helper remain external declarations.
 */
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
    s32 state;
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
    state = D_80140B20;
    switch (state) {
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
            } else if (D_80149418[i] != 0) {
                sound_handles_array_clear(D_80149418[i]);
                D_80149418[i] = 0;
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
