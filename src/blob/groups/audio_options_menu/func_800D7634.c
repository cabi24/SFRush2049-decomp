/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* group audio_options_menu (w22e): func_800D690C (stub given a body) + func_800D6914 (1260 B) + func_800D7634 (1804 B),
 * all three EQUAL in the whole-program unit with their real callers (no stand-ins, no implicit int, no false prototype).
 *   func_800D690C  (2 w)   deleted helper "row selectable?" (`v = 1; return v;`), umerge-inlined into func_800D7634
 *                          (the row-wrap do-while: retail's unfolded `li v0,1; beqzl v0`) and func_800D6914
 *                          (`if ((vis = func_800D690C(i)) == 0)`, the sibling func_800B59F0's visibility idiom).
 *                          Its retail remains are the caller-less locked stub at 0x800D690C.
 *   func_800D6914  (315 w) audio-options 3D buttons update (sibling of the locked pause-menu func_800B59F0,
 *                          physics_sym group): 4 rows, selected one spun to pi, slider cursor D_801102B4[8].
 *   func_800D7634  (451 w) N64 audio-options screen input handler (no arcade ancestor): rows 0 sfx volume
 *                          (D_80146108.sfx), 1 music volume (.music), 2 music track (D_80110640 0..13, 12 = random via
 *                          func_800D6160(0), 13 = off), 3 stereo/mono (.mono, func_800D6E00). A/B/Start leave
 *                          (resource_type_select, D_8011063C = 2); A on the track row restarts the track; left/right
 *                          move the row (D_80110638, wrap 0..3); up/down change the value; post_msg_a/post_msg_b send a
 *                          type-1 message to the audio slot queue (func_800C8918's post_type1 idiom).
 * Levers (each measured in the unit; w21h/w22e RESULTS.md):
 *  - every speed_set call is the locked kept wrapper speed_mode0_wrapper/speed_mode1_wrapper and func_800D6160(0) is
 *    the locked physics_init_mode0(), all umerge-inlined (8-byte inline areas = retail frame 256).
 *  - func_800D6914 compares `i == D_80110638` directly; s7 = &D_801105B8 and s8 = 1 come from the inlined
 *    func_800D690C visibility test (the w20e D_80110638 getter was a stand-in).
 * Disclosed shaping:
 *  1. func_800D7634: one compiled-out block end `do {} while (0);` right after post_msg_b() on the track row. Traced:
 *     without it the post block (func_80091B00's precoloured v0 result) shares a uopt block with the D_80110640
 *     address web, so v0 is forbidden to that web and it takes a1 at all four track sites (15 words).
 *  2. Unused locals that are the exact frame residual:
 *     func_800D7634 `padB` (retail 256, without it 248; 1 slot);
 *     func_800D6914 `m[3][3]`, `k` (retail 144, without them 104; 10 slots), `cur` and `d` (1 slot each: removal
 *     moves old/sel_y/tex off retail's homes 76/68/62; the sibling func_800B59F0 has the same `cur`/`d`).
 *  3. Helper identities: func_800D690C = constant-1 row predicate (hypothesis supported by the stub position, the
 *     unfolded `li v0,1` in func_800D7634 and the s7/s8 webs in func_800D6914); post_msg_a/post_msg_b are two
 *     non-static copies of func_800C8918's post_type1 (no retail stubs; one shared helper with two call sites is
 *     not inlined by umerge). Non-static and not on keep, so uld makes them internal and umerge inlines them.
 *  4. Old-style declarations with the right return type: func_800B24EC(), model_data_load(), model_transform_setup().
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct {
    s32 type;            /* 0x00 */
    s32 handle;          /* 0x04 */
    f32 ang;             /* 0x08 */
    f32 mat[3][3];       /* 0x0C */
    f32 pos[3];          /* 0x30 */
    u8 pad3C[4];
} Button; /* 0x40 */

extern s32 D_801105B4;
extern s32 D_801105B8;
extern f32 D_801105BC;
extern Button D_801102B4[];
extern s32 D_80110638;
extern f32 D_8011418C[];
extern volatile f32 D_8002EB94;
extern volatile u8 D_80140BDC;
extern f32 D_801543D0;

f32 fabsf(f32);
#pragma intrinsic(fabsf)
void *memcpy(void *, const void *, u32);
s32 func_800B24EC();
void func_8008D870(s16 handle, s32 tex, s32 c);
void func_800B5898(f32 angle, f32 uv[][3]);
void func_800B5940(f32 angle, f32 uv[][3]);
void func_8008B32C(f32 (*dst)[3], f32 (*src)[3], f32 s);
void model_data_load();
void model_transform_setup();

s32 func_800D690C(s32 i) {
    s32 v;
    v = 1;
    return v;
}

void func_800D6914(void) {
    s32 i;
    s32 name;
    f32 y;
    f32 cur;
    f32 m[3][3];
    f32 k;
    f32 old[3];
    f32 d;
    f32 sel_y;
    s32 vis;
    s16 tex;

    if (D_801105B4 == 0)
        return;
    y = 100.0f;
    for (i = 0; i < 4; i++) {
        if (i == D_80110638) {
            sel_y = y;
            if (D_801102B4[i].ang < 3.1415927f)
                D_801102B4[i].ang += 12.566371f * D_8002EB94;
            if (3.1415927f < D_801102B4[i].ang || D_801105B8 == 1)
                D_801102B4[i].ang = 3.1415927f;
        } else {
            if (0.0f < D_801102B4[i].ang)
                D_801102B4[i].ang -= 12.566371f * D_8002EB94;
            if (D_801102B4[i].ang < 0.0f || D_801105B8 == 1)
                D_801102B4[i].ang = 0.0f;
        }
        memcpy(D_801102B4[i].mat, D_8011418C, 36);
        memcpy(old, D_801102B4[i].pos, 12);
        D_801102B4[i].pos[0] = -90.0f;
        D_801102B4[i].pos[1] = y;
        D_801102B4[i].pos[2] = 300.0f;
        if (D_801105B8 == 0) {
            if (old[1] + 180.0f * D_8002EB94 < D_801102B4[i].pos[1])
                D_801102B4[i].pos[1] = old[1] + 180.0f * D_8002EB94;
            if (D_801102B4[i].pos[1] < old[1] - 180.0f * D_8002EB94)
                D_801102B4[i].pos[1] = old[1] - 180.0f * D_8002EB94;
        }
        D_801102B4[i + 4].ang = D_801102B4[i].ang;
        D_801102B4[i + 4].pos[0] = 150.0f;
        D_801102B4[i + 4].pos[1] = D_801102B4[i].pos[1];
        D_801102B4[i + 4].pos[2] = D_801102B4[i].pos[2];
        func_800B5898(D_801102B4[i].ang - 1.5707964f, D_801102B4[i].mat);
        memcpy(D_801102B4[i + 4].mat, D_801102B4[i].mat, 36);
        if (1.5707964f < D_801102B4[i].ang)
            name = func_800B24EC("BUTTON_SELECT", &tex, 0, (s8)(D_80140BDC - 1), 1);
        else
            name = func_800B24EC("BUTTON", &tex, 0, (s8)(D_80140BDC - 1), 1);
        func_8008D870(D_801102B4[i].handle, name, -1);
        func_8008D870(D_801102B4[i + 4].handle, name, -1);
        if ((vis = func_800D690C(i)) == 0) {
            model_data_load(D_801102B4[i].handle, 1, 15);
            model_data_load(D_801102B4[i + 4].handle, 1, 15);
        } else {
            model_transform_setup(D_801102B4[i].handle, 0, 15);
            model_transform_setup(D_801102B4[i + 4].handle, 0, 15);
            y -= 40.0f;
        }
    }
    memcpy(old, D_801102B4[8].pos, 12);
    D_801102B4[8].ang += 6.2831855f * D_8002EB94;
    if (6.2831855f < D_801102B4[8].ang)
        D_801102B4[8].ang -= 6.2831855f;
    memcpy(D_801102B4[8].mat, D_8011418C, 36);
    D_801102B4[8].pos[0] = -160.0f;
    D_801102B4[8].pos[2] = 225.0f;
    D_801102B4[8].pos[1] = sel_y * 225.0f / 300.0f;
    if (D_801105B8 == 0) {
        y = D_801102B4[8].pos[1];
        if (D_801543D0 < fabsf(y - old[1]) * 60.0f / 10.0f) {
            D_801543D0 = fabsf(y - old[1]) * 60.0f / 10.0f;
            D_801543D0 = (D_801543D0 < 540.0f * D_801105BC) ? D_801543D0 : 540.0f * D_801105BC;
            D_801543D0 = (180.0f * D_801105BC < D_801543D0) ? D_801543D0 : 180.0f * D_801105BC;
        }
        if (old[1] + D_801543D0 * D_8002EB94 < y)
            D_801102B4[8].pos[1] = old[1] + D_801543D0 * D_8002EB94;
        else if (y < old[1] - D_801543D0 * D_8002EB94)
            D_801102B4[8].pos[1] = old[1] - D_801543D0 * D_8002EB94;
        else
            D_801543D0 = 180.0f * D_801105BC;
    }
    func_800B5940(D_801102B4[8].ang, D_801102B4[8].mat);
    func_800B5898(1.5707964f, D_801102B4[8].mat);
    func_8008B32C(D_801102B4[8].mat, D_801102B4[8].mat, 0.4f);
    D_801105B8 = 0;
}


typedef struct { f32 x, y; } Stick;
typedef struct { volatile s8 b0; u8 pad[15]; } Pad16;
typedef struct { u8 pad0[12]; s8 music; s8 sfx; s8 track; s8 mono; } AudioCfg;
typedef struct Msg { s16 id; s8 type, used; u8 payload[20]; } Msg;
typedef struct OSMesgQueue OSMesgQueue;
typedef void *OSMesg;
extern u32 D_801174B4;
extern s32 D_801174B8;
extern Stick D_80156958[4];
extern s32 D_8015694C;
extern s32 D_80156998[4];
extern s32 D_8015698C;
extern s8 D_8011062C;
extern s32 D_8011063C;
extern s32 D_80110638;
extern s32 D_80110640;
extern s32 D_80110644;
extern s8 D_8011064C;
extern AudioCfg D_80146108;
extern Pad16 D_80156CF0[];
extern s8 D_80157244;
extern s32 D_80142728, D_801427A8;
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flags);
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flags);
Msg *func_80091B00(void);
void func_80091AF0(void);
void func_80091AF8(void);
void drone_pathfind_main(void);
void resource_type_select(s32 a);
void audio_doppler(s32 a);
void audio_distance_atten(s32 a);
void speed_set(f32 blend, f32 amount, s32 first, s32 second);
void sync_entry_register(s32 a, s32 b);
void func_800D6160(s32 mode);
void physics_init_mode0(void);
void speed_mode0_wrapper(f32 blend, f32 amount);
void speed_mode1_wrapper(f32 blend, f32 amount);
void func_800D6E00(s32 a);
void credits_scroll(void);
void credits_screen(void);
void func_800B4FB0(s32 a);
void func_800D6914(void);

void post_msg_a(void) {
    Msg *sp1C;
    Msg *temp_v0;

    func_80091AF0();
    temp_v0 = func_80091B00();
    sp1C = temp_v0;
    temp_v0->type = 1; func_80091AF8();
    osSendMesg((OSMesgQueue *) &D_801427A8, sp1C, 0);
}

void post_msg_b(void) {
    Msg *sp1C;
    Msg *temp_v0;

    func_80091AF0();
    temp_v0 = func_80091B00();
    sp1C = temp_v0;
    temp_v0->type = 1; func_80091AF8();
    osSendMesg((OSMesgQueue *) &D_801427A8, sp1C, 0);
}


void func_800D7634(void) {
    Pad16 *p;
    s32 padB;
    s32 btn;
    f32 stick[2];
    s32 ds;
    s32 dz;

    if (D_801174B4 & 0x7C03FFFE) {
        stick[0] = D_80156958[0].x;
        stick[1] = D_80156958[0].y;
        btn = D_8015694C;
    } else {
        btn = D_80156998[D_8015698C];
        stick[0] = D_80156958[D_8015698C].x;
        stick[1] = D_80156958[D_8015698C].y;
    }
    if (D_8011062C == 0)
        drone_pathfind_main();
    if (btn & 5) {
        resource_type_select(btn);
        D_8011063C = 2;
    } else if (btn & 2) {
        switch (D_80110638) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            post_msg_a();
            speed_mode0_wrapper(D_80146108.music / 10.0f, 0.05f);
            if (D_80110640 < 12)
                sync_entry_register(D_80110640, 1);
            else if (D_80110640 == 12) {
                if (D_801174B4 & 0x7C03FFFE)
                    sync_entry_register(6, 1);
                else
                    physics_init_mode0();
            }
            break;
        }
    } else if (btn & 0xC00) {
        ds = (btn & 0x400) ? -1 : 1;
        audio_distance_atten(btn);
        do {
            D_80110638 += ds;
            if (D_80110638 > 3)
                D_80110638 = 0;
            else if (D_80110638 < 0)
                D_80110638 = 3;
        } while (!func_800D690C(D_80110638));
        if (D_80110638 == 1 || D_80110638 == 2) {
            speed_mode0_wrapper(D_80146108.music / 10.0f, 0.05f);
            if (D_80110640 < 12)
                sync_entry_register(D_80110640, 1);
            else if (D_80110640 == 12) {
                if (D_801174B4 & 0x7C03FFFE)
                    sync_entry_register(6, 1);
                else
                    physics_init_mode0();
            }
        }
    } else if (btn & 0x3000) {
        dz = (btn & 0x1000) ? -1 : 1;
        audio_doppler(btn);
        switch (D_80110638) {
        case 0:
            D_80146108.sfx += dz;
            if (D_80146108.sfx > 10)
                D_80146108.sfx = 0;
            else if (D_80146108.sfx < 0)
                D_80146108.sfx = 10;
            speed_mode1_wrapper(D_80146108.sfx / 10.0f, 0.05f);
            break;
        case 1:
            D_80146108.music += dz;
            if (D_80146108.music > 10)
                D_80146108.music = 0;
            else if (D_80146108.music < 0)
                D_80146108.music = 10;
            speed_mode0_wrapper(D_80146108.music / 10.0f, 0.05f);
            break;
        case 2:
            post_msg_b();
            do {} while (0);
            D_80110640 += dz;
            if (D_80110640 > 13)
                D_80110640 = 0;
            else if (D_80110640 < 0)
                D_80110640 = 13;
            D_80110644 = D_80110640;
            D_80146108.track = D_80110640;
            speed_mode0_wrapper(D_80146108.music / 10.0f, 0.05f);
            if (D_80110640 < 12)
                sync_entry_register(D_80110640, 1);
            else if (D_80110640 == 12) {
                if (D_801174B4 & 0x7C03FFFE)
                    sync_entry_register(6, 1);
                else
                    physics_init_mode0();
            }
            break;
        case 3:
            D_80146108.mono = !D_80146108.mono;
            func_800D6E00(D_80146108.mono);
            break;
        }
    }
    func_800D6914();
    if (D_801174B4 & 0x7C0000) {
        p = &D_80156CF0[D_8015698C];
        if (!p->b0 != D_8011064C) {
            D_8011064C = !p->b0;
            credits_scroll();
        }
    }
    if (D_80157244) {
        credits_screen();
        if (D_801174B4 & 0x7C03FFFE)
            D_801174B8 = 4;
        else
            func_800B4FB0(1);
    } else if (D_8011063C) {
        credits_screen();
        if (D_801174B4 & 0x7C03FFFE)
            D_801174B8 = 16;
        else
            func_800B4FB0(1);
    }
}
