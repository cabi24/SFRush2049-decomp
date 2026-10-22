/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * world_trigger_activate (0x800EE5DC, 464 B; historical label) = the N64 main (graphics) thread's message step, called
 * forever by the static game_init (no GC/DC counterpart: GC/DC flattened the threads). One scheduler message:
 *  - 4 (pre-NMI): func_800205E4, apply_display_mode, osViBlack(1), modelrun = 0, check_mpath_save, pak gInited
 *    (D_8011EAE0) = 0, hang;
 *  - 1 (retrace): when the next framebuffer is the current one, no frame is in flight (D_80035471/70/72), the
 *    scheduler's frame count moved on (+636 != D_8002EB98) and fewer than two tasks are pending (D_80114730):
 *    retrace count D_80151AE0 = 0, StartGraphics (gfx_frame_begin), D_80035470 = 1, wake the game thread
 *    (osSendMesg D_8002ECC0); then count++;
 *  - 2750 (frame-end request from the game thread's UpdateFrame): D_80035472 = 0, EndGraphics
 *    (world_collision_response), toggle the frame record D_8015F72D, pending++;
 *  - 2 (task done): toggle D_801161CC (function-local static, own .data), pending--.
 * Base: w36w3's draft (case order 4, 1, 2750, 2 = retail body layout; volatile flags; function-local static).
 * w39c levers (vbatch b1/b2 + trace, cloud/work/frontier/w39c/RESULTS.md): D_80114730 NOT volatile (address webs v1 in
 * every case); the pre-NMI and frame-end bodies are inlined helpers (MainPreNMI / MainEndFrame: invented names; with
 * both bodies written in place the frame is 48, retail 56); the switch value is the named local `type` (the 4-byte
 * local above msg, retail sp+52, replacing w36w3's disclosed unused `x`); the retrace counter goes through the one
 * scratch variable `cur` that also held the framebuffer (`cur = D_80151AE0; D_80151AE0 = cur + 1;` gives retail's
 * v1 load: a separate local takes v0, force.sh p1:w40=c2 proved it is the only residual). No unused locals, no
 * compiled-out holders. The framebuffer pointers are compared as s32 through `cur`.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef void *OSMesg;
typedef struct { u32 opaque[6]; } OSMesgQueue;
typedef struct { u8 pad[636]; s32 frameCount; } Sched;

extern OSMesgQueue D_8002ECF8;
extern OSMesgQueue D_8002ECC0;
extern u8 D_80142D90[];
extern volatile s8 D_80035470, D_80035471, D_80035472;
extern volatile Sched D_8002E8E8;
extern volatile s32 D_8002EB98;
extern u32 D_80114730;
extern volatile u16 D_801525F0;
extern s8 D_8011EAE0;
extern volatile s8 D_8015F72D;
extern s32 D_80151AE0;

s32 osRecvMesg(OSMesgQueue *, OSMesg *, s32);
s32 osSendMesg(OSMesgQueue *, OSMesg, s32);
void *osViGetCurrentFramebuffer(void);
void *osViGetNextFramebuffer(void);
void osViBlack(u8);
void func_800205E4(void);
void apply_display_mode(void);
void check_mpath_save(void);
void gfx_frame_begin(void);
void world_collision_response(void);

void MainPreNMI(void)
{
    func_800205E4();
    apply_display_mode();
    osViBlack(1);
    D_801525F0 = 0;
    check_mpath_save();
    D_8011EAE0 = 0;
    for (;;) {
    }
}

void MainEndFrame(void)
{
    D_80035472 = 0;
    world_collision_response();
    D_8015F72D ^= 1;
    D_80114730++;
}


void world_trigger_activate(void)
{
    static s32 D_801161CC = 0;
    s16 type;
    OSMesg msg = 0;
    s32 cur;

    osRecvMesg(&D_8002ECF8, &msg, 1);
    type = *(s16 *)msg;
    switch (type) {
    case 4:
        MainPreNMI();
        break;
    case 1:
        cur = (s32)osViGetCurrentFramebuffer();
        if ((s32)osViGetNextFramebuffer() == cur && !D_80035471 && !D_80035470 && !D_80035472 &&
            D_8002EB98 != D_8002E8E8.frameCount && D_80114730 < 2) {
            D_80151AE0 = 0;
            gfx_frame_begin();
            D_80035470 = 1;
            osSendMesg(&D_8002ECC0, D_80142D90, 1);
        }
        cur = D_80151AE0;
        D_80151AE0 = cur + 1;
        break;
    case 2750:
        MainEndFrame();
        break;
    case 2:
        D_801161CC ^= 1;
        D_80114730--;
        break;
    }
}
