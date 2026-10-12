/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/*
 * func_80100B8C(s32 arg0): results/attract caption screen. Overlay off (render_helper(0)), text box mode
 * D_80118E20 = {1,1} (inlined func_800B669C), then either two centred captions in font 13 (D_801613E8 == 1:
 * D_8012029C and countdown_object[33]) or one in font 11 (countdown_object[32]) followed by colour 1; box mode
 * {0,3}, overlay -1. Returns 1. font_set = the lock/slot_state_setup/unlock idiom (locked object_create;
 * slot_state_setup takes its argument in s2 by IPA, so this only matches in the unit/group).
 * Base: credits_scroll_grp gr3_d.c context copy (m2c-era); fixed: dispatch_handler(1) is in the else arm only.
 * SHAPING DEVICES (disclosed):
 *  - `char buf[40]` is never used: exact frame residual. Retail frame 144, without it 104 (40 B = 10 words at
 *    the top of the frame; the inlined font_set areas at sp+60/sp+84 already coincide). This is an unused
 *    local ARRAY -- check against the Wave 12 "no pad arrays" rule before installing.
 *  - `D_801613E8 == 1U`: the unsigned constant shares the u32 `1` web of the inlined func_800B669C(1, 1)
 *    stores, giving retail's `bne v0,t6` (signed `1` gets its own `li at,1`).
 *  - D_80118E20 is DEFINED here (`Pair D_80118E20;`): the two tail stores share one `lui at` only when the
 *    symbol is defined in the unit (w15d). Data-symbol change for integration.
 */
typedef signed char s8;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct { u32 opaque[6]; } OSMesgQueue;
extern OSMesgQueue D_801461D0;
s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osSendMesg(OSMesgQueue *, void *, s32);
s32 slot_state_setup(s32 selection);
extern char **countdown_object;
extern s8 D_801613E8;
extern char D_8012029C[];
typedef struct { u32 first, second; } Pair;
Pair D_80118E20;
void func_800B669C(u32 first, u32 second);
void render_helper(f32);
void dispatch_handler(s32);
void camera_auto_follow(s32, s32, s32, s32, s32, s32, char *);

static void gfx_lock(void)
{
    osRecvMesg(&D_801461D0, (void *)0, 1);
}

static void gfx_unlock(void)
{
    osSendMesg(&D_801461D0, (void *)0, 0);
}

static s32 font_set(s32 font)
{
    s32 old;

    gfx_lock();
    old = slot_state_setup(font);
    gfx_unlock();
    return old;
}

s32 func_80100B8C(s32 arg0)
{
    char buf[40];

    render_helper(0.0f);
    func_800B669C(1, 1);
    if (D_801613E8 == 1U) {
        font_set(13);
        dispatch_handler(22);
        camera_auto_follow(160, 10, 320, 110, -1, 0, D_8012029C);
        camera_auto_follow(160, 190, 320, 110, -1, 0, countdown_object[33]);
    } else {
        font_set(11);
        dispatch_handler(22);
        camera_auto_follow(160, 185, 320, 110, -1, 0, countdown_object[32]);
        dispatch_handler(1);
    }
    D_80118E20.second = 3;
    D_80118E20.first = 0;
    render_helper(-1.0f);
    return 1;
}
