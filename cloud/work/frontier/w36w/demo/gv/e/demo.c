/* Native demo/attract research. One original formal is preserved.
 * The original legacy source passes an additional actual argument through
 * an unprototyped declaration. This is compiler-specific source evidence,
 * not a portable ISO C arity contract or an invented second formal.
 * Native stub identities are inferred from the exact adjacent code layout
 * and the genuine donor call boundaries; their eight-byte bodies are locked.
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

static s8 D_80114154 = 0;
extern s16 D_80151AD0;
extern u32 D_801174B4;
extern u32 D_801174B8;
extern s32 D_801174BC;
extern s32 D_8014A110;
static s16 D_80114158 = -1;
extern s8 D_80152744;
extern MODELDAT D_8014A250[];
typedef struct DemoVisual DemoVisual;
extern DemoVisual *D_801391F0;
extern Player D_8014A118[4];
extern s8 D_8014978C;
static s8 D_8011414C = 0;
typedef struct DemoBlit DemoBlit;
static DemoBlit *D_80114150 = 0;
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
void entity_transform_apply(DemoVisual *node, s32 arg1);
void func_800B0580(void);
void func_800B1F30(s16 arg0);
void func_800B912C(s32 arg0);
DemoBlit *sound_control(s16 arg0, s16 arg1, void *arg2, s16 arg3);
void sound_stop(DemoBlit *sound);
void viScheduleTick(f32 arg0);
s32 viDeadlinePassed(void);
void entity_audio_update(void);

static void setGameState(u32 state);
void debug_stats(s32 on, s32 x);
void func_800FD01C(void);
void func_800FD00C(void);
void func_800FD014(void);
void func_800FCDF0(void);
static void RemoveEnvirons(void);

void debug_collision(void) {
    s32 x;

    if (!D_8011414C) {
        func_800FD01C();
    }
    debug_stats(1, 0);
    if (D_80157244 && D_80114154) {
        func_800FD00C();
        setGameState(4);
        return;
    }
    if ((D_8015694C & 7) && D_80114154 || viDeadlinePassed()) {
        x = (D_8015694C & 1) != 0;
        func_800FD00C();
        if (D_80114158 == 5 && viDeadlinePassed()) {
            setGameState(2);
            return;
        }
        setGameState(4);
        if (x) {
            D_801174BC = 16;
        }
    }
}

void func_800FD01C(void) {

        D_80114150 = sound_control(0, 0, &D_80114128, 1);
        viScheduleTick(30.0f);
        func_800FD014();
        D_8011414C = 1;
        entity_audio_update();
    }

void func_800FD00C(void) {
        if (D_80114150) {
            sound_stop(D_80114150);
            D_80114150 = 0;
        }
        debug_stats(0, 0);
        D_8011414C = 0;
}

void debug_stats(s32 on, s32 x) {
    static s8 D_8011415C = 0;

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
        } else if (D_80114154) {
            render_viewport_init();
        }
    } else {
        if (x) {}
        D_8011415C = 0;
        D_80114154 = 0;
        func_800FCDF0();
        if (!(D_801174B4 & 0x02000000)) {
            func_800B912C(D_8014978C);
        }
    }
}

void func_800FCDF0(void) {
    s16 i;
        for (i = 0; i < D_80152744; i++) {
            func_800D5828(D_8014A250[i].slot);
        }
        players_frame_update();
        func_800D5374();
        records_screen();
        RemoveEnvirons();
        for (i = 0; i < D_80152744; i++) {
            func_800B1F30(i);
        }
        for (i = 0; i < 4; i++) {
            D_8014A118[i].node = i;
        }
}

static void RemoveEnvirons(void)
{
    while (D_801391F0) entity_transform_apply(D_801391F0, 1);
    func_800B0580();
}

void func_800FD014(void) {
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
}

static void setGameState(u32 state) { D_801174B8 = state; }
