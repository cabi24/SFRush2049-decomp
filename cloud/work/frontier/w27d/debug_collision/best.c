/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * debug_collision (0x800FD024, 532 B) + debug_stats (0x800FCDF8, 532 B) -- DRAFT pair (names are
 * historical labels). debug_collision runs the race-start sequence: on first entry start a sound,
 * viScheduleTick(30.0f), advance the attract counter D_80114158 (wrap at 6, or 5 without
 * D_80156994), then each frame debug_stats(1) (set up / render the race viewport once
 * display_list_flush says ready); when done (D_80157244 && D_80114154, or a frame/deadline test)
 * stop the sound, debug_stats(0) (tear down cars, entities, players) and pick the next state
 * (D_801174B8 = 4, or 2 on counter 5 with viDeadlinePassed; D_801174BC = 16 when bit 0 set).
 * debug_stats is internal: its second parameter (an s16 reused as the loop counter) arrives in s0
 * (retail callers do `move s0,zero` before each jal); it saves no s-registers (the caller does).
 * Unit (--internal debug_stats): debug_collision 3 words (the three `move s0,zero` come out as
 * `sw zero,4(sp)`: our callee does not give `i` s0), debug_stats 117/133 words, ops 18: we colour
 * &D_8011415C (v0) and the constant 1 (s0); retail keeps both as temporaries.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct {
    u8 pad0[0x7C6];
    s16 slot;
    u8 pad7C8[0x808 - 0x7C8];
} MODELDAT;

typedef struct {
    u8 node;
    u8 pad1[0x4B];
} Player;

extern s8 D_8011415C;
extern s8 D_80114154;
extern s16 D_80151AD0;
extern u32 D_801174B4;
extern s32 D_801174B8;
extern s32 D_801174BC;
extern s32 D_8014A110;
extern s16 D_80114158;
extern s8 D_80152744;
extern MODELDAT D_8014A250[];
extern void *D_801391F0;
extern Player D_8014A118[4];
extern s8 D_8014978C;
extern s8 D_8011414C;
extern void *D_80114150;
extern s32 D_80114128;
extern s32 D_80138660;
extern s8 D_80156994;
extern s8 D_80157244;
extern s32 D_8015694C;

void func_800EC190(s16 arg0);
s32 display_list_flush(s32 arg0, s32 arg1);
void camera_race_setup(void);
void race_init_helper(void);
void func_800FBE60(void);
void render_viewport_init(void);
void func_800D5828(s16 index);
void players_frame_update(void);
void func_800D5374(void);
void records_screen(void);
void entity_transform_apply(void *node, s32 arg1);
void func_800B0580(void);
void func_800B1F30(s16 arg0);
void func_800B912C(s32 arg0);
void *sound_control(s16 arg0, s16 arg1, void *arg2, s16 arg3);
void sound_stop(void *sound);
void viScheduleTick(f32 arg0);
s32 viDeadlinePassed(void);
void entity_audio_update(void);

void debug_stats(s32 on, s16 i) {
    void *node;

    if (on) {
        if (!D_8011415C) {
            D_8011415C = 1;
            D_80151AD0 = 1;
            if (!(D_801174B4 & 0x02000000)) {
                D_8014A110 = 0;
                func_800EC190(D_80114158);
            }
        }
        if (!D_80114154 && display_list_flush(D_801174B4 & 8, 0)) {
            camera_race_setup();
            race_init_helper();
            func_800FBE60();
            D_80114154 = 1;
            return;
        }
        if (D_80114154) {
            render_viewport_init();
        }
    } else {
        D_8011415C = 0;
        D_80114154 = 0;
        for (i = 0; i < D_80152744; i++) {
            func_800D5828(D_8014A250[i].slot);
        }
        players_frame_update();
        func_800D5374();
        records_screen();
        while ((node = D_801391F0) != 0) {
            entity_transform_apply(node, 1);
        }
        func_800B0580();
        for (i = 0; i < D_80152744; i++) {
            func_800B1F30(i);
        }
        for (i = 0; i < 4; i++) {
            D_8014A118[i].node = i;
        }
        if (!(D_801174B4 & 0x02000000)) {
            func_800B912C(D_8014978C);
        }
    }
}

void debug_collision(void) {
    s32 x;

    if (!D_8011414C) {
        D_80114150 = sound_control(0, 0, &D_80114128, 1);
        viScheduleTick(30.0f);
        D_80114158++;
        D_80138660 = 0;
        if (D_80114158 >= 6) {
            D_80138660 = 1;
            D_80114158 = 0;
        }
        if (!D_80156994 && D_80114158 >= 5) {
            D_80138660 = 1;
            D_80114158 = 0;
        }
        D_8011414C = 1;
        entity_audio_update();
    }
    debug_stats(1, 0);
    if (D_80157244 && D_80114154) {
        if (D_80114150) {
            sound_stop(D_80114150);
            D_80114150 = 0;
        }
        debug_stats(0, 0);
        D_8011414C = 0;
        D_801174B8 = 4;
        return;
    }
    if ((D_8015694C & 7) && D_80114154 || viDeadlinePassed()) {
        x = (D_8015694C & 1) != 0;
        if (D_80114150) {
            sound_stop(D_80114150);
            D_80114150 = 0;
        }
        debug_stats(0, 0);
        D_8011414C = 0;
        if (D_80114158 == 5 && viDeadlinePassed()) {
            D_801174B8 = 2;
            return;
        }
        D_801174B8 = 4;
        if (x) {
            D_801174BC = 16;
        }
    }
}
