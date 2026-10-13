/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * save_settings (0x800DE20C, 584 B) -- modal settings dialog runner (name is a historical label).
 * Starts a menu sound, draws the dialog frame (display_settings), saves each viewport's fog
 * parameters (func_800A7428 reads D_8017A510[i] +0x40..+0x47) and sets fog off (1000/999,
 * black, alpha 255), then runs control_settings(arg0, arg4, arg5, &done, arg6) until `done`,
 * calling game_mode_handler on thread 7 and func_800C813C(0, 0) except for modes 10-13;
 * finally stops the sound and restores every viewport's fog. N64-specific UI (no arcade
 * counterpart). Locals are declared in retail stack order (done at 151, snd at 144, arrays 136..112).
 * No shaping devices. Unit: blob_unit score save_settings --neighbours -> EQUAL, 0 locked differ.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef float f32;
typedef s32 OSId;
typedef struct OSThread OSThread;

extern s16 gNumWindows;
extern s32 D_80116DBC;

extern s32 sound_control(s16 arg0, s16 arg1, void *arg2, s16 arg3);
extern void sound_stop(s32 sound_id);
extern void game_mode_handler(void);
extern OSId osGetThreadId(OSThread *thread);
s32 control_settings(s32 arg0, s32 arg1, s32 arg2, s8 *done, s32 (*arg4)(void));
void display_settings(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void func_800A7428(s16 *arg0, s16 *arg1, u8 *arg2, u8 *arg3, u8 *arg4, u8 *arg5, s32 arg6);
void func_800A7480(s16 arg0, s16 arg1, u8 arg2, u8 arg3, u8 arg4, u8 arg5, s32 arg6);
void SaveServer(s32 arg0, s32 arg1);

void save_settings(s32 arg0, s32 mode, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 (*arg6)(void)) {
    s8 done;
    s32 snd;
    s16 a[4];
    s16 b[4];
    u8 c[4];
    u8 d[4];
    u8 e[4];
    u8 f[4];
    s32 i;

    done = 0;
    snd = sound_control(0, 0, &D_80116DBC, 1);
    display_settings(arg0, mode, 10, 10, 300, 180);
    for (i = 0; i < gNumWindows; i++) {
        func_800A7428(&a[i], &b[i], &c[i], &d[i], &e[i], &f[i], i);
        func_800A7480(1000, 999, 0, 0, 0, 255, i);
    }
    while (!done) {
        if (osGetThreadId(0) == 7) {
            game_mode_handler();
        }
        if (mode != 11 && mode != 12 && mode != 13 && mode != 10) {
            SaveServer(0, 0);
        }
        control_settings(arg0, arg4, arg5, &done, arg6);
    }
    sound_stop(snd);
    for (i = 0; i < gNumWindows; i++) {
        func_800A7480(a[i], b[i], c[i], d[i], e[i], f[i], i);
    }
}
