/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * game_late_init (0x800EEA7C, 820 B): late boot/game init. Sets the display (waits for osTvType = D_80000300,
 * viewport_setup), allocates three 0x22620-byte heaps (D_8002EBB0 via audio_dma_sync, D_80156CEC and D_80157240
 * via sound_play_menu; each cleared with audio_loop_control, then 64-byte aligned (+32 & ~63)), fills the
 * segment table D_80156BE0 (buffers D_800586D0 / D_8006A090, counts 2), D_8015B250/D_8015B260 and
 * D_801497C8 = D_801497F4 = D_800586D0 + 0x9CC0, runs the module inits, hud_setup(32,16,16,5,64,1.0,200.0,0),
 * frees D_8011025C / D_80110260 under the D_80152770 heap lock when D_80156994 is clear, then effect_spawn,
 * player_state_set(-1, 0), func_800DE45C, viTickStart, creates the D_801461D0 lock once, D_80149DA0 = -1.
 * STATE: draft. unit 184/205 words (rows words 69 ops 30 norm 30), frame 128/56.
 * Residual class: logic mostly known, call conventions/frame unknown: retail calls audio_reverb_update with the
 * pointer in a1 and 0 in a2 and leaves a0 unset (an uninitialised first argument?), spills the two pointers
 * to 72/48(sp), saves unused s0/s1 (an unmatched internal callee -- func_800DE45C (w27d) or hud_setup --
 * clobbers them), and has a 128-byte frame (72 bytes of unexplained locals).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct { u32 opaque[6]; } OSMesgQueue;
typedef void *OSMesg;
typedef struct {
    u8 other0[88];
    void *buf88;        /* 88 */
    s16 n92;            /* 92 */
    u8 other94[30];
    s32 f124;           /* 124 */
    u8 other128[88];
    void *buf216;       /* 216 */
    s16 n220;           /* 220 */
    u8 other222[30];
    s32 f252;           /* 252 */
} Segs;
extern OSMesgQueue D_801461D0;
extern OSMesg D_801461FC[];
extern OSMesgQueue D_80152770;
extern s32 D_8015F738, D_80161380, D_80161398, D_801613A4;
extern f32 D_80154188;
extern s32 D_80000300;
extern s32 D_8002AFC0, D_8002AFC4;
extern s32 D_8002EBB0, D_80156CEC, D_80157240;
extern Segs D_80156BE0;
extern u8 D_800586D0[], D_8006A090[];
extern u8 *D_8015B250, *D_8015B260;
extern u8 *D_801497C8, *D_801497F4;
extern s8 D_80156994;
extern void *D_8011025C, *D_80110260;
extern s8 D_801147C0;
extern s8 D_80149DA0;
extern s32 osRecvMesg(OSMesgQueue *, OSMesg *, s32);
extern s32 osSendMesg(OSMesgQueue *, OSMesg, s32);
extern void osCreateMesgQueue(OSMesgQueue *, OSMesg *, s32);
extern void func_800E7D0C(u16 arg0, u16 arg1);
extern void menu_save_options(void);
extern s32 get_tv_offset(void);
extern void viewport_setup(s32, s32, s32, s32);
extern void *audio_dma_sync(s32 arg0, s32 arg1);
extern void audio_loop_control(u32 arg0, s32 arg1);
extern void *sound_play_menu(s32 arg0, s32 arg1);
extern void func_800A5B3C(void);
extern void func_800A5488(void);
extern void particle_velocity_set(void);
extern void hud_setup(s32, s32, s32, s32, s32, f32, f32, s32);
extern void audio_reverb_update(u32 arg0, s32 arg1);
extern void effect_spawn(void);
extern void player_state_set(s32, s32);
extern void func_800DE45C();
extern void viTickStart(void);
void func_800960CC(u32 address);   /* DeallocateMemory (codex_heap_release_a25/wheel_params_set.c), inlined */

void _InitSemaphore_Init(void);
void HAL_InitText_Init(void);

void game_late_init(void)
{
    func_800E7D0C(50, 10);
    menu_save_options();
    D_8015F738 = 0;
    D_80161380 = 0;
    D_80161398 = 0;
    D_801613A4 = 0;
    D_80154188 = 90.0f;
    while (D_80000300 == 0) {
    }
    viewport_setup(D_80000300, get_tv_offset(), D_8002AFC0, D_8002AFC4);
    D_8002EBB0 = (s32)audio_dma_sync(0, 0x22620);
    audio_loop_control(D_8002EBB0, 0);
    D_80156CEC = (s32)sound_play_menu(0, 0x22620);
    audio_loop_control(D_80156CEC, 0);
    D_80157240 = (s32)sound_play_menu(0, 0x22620);
    audio_loop_control(D_80157240, 0);
    D_8002EBB0 = (D_8002EBB0 + 32) & ~63;
    D_80156CEC = (D_80156CEC + 32) & ~63;
    D_80157240 = (D_80157240 + 32) & ~63;
    D_80156BE0.f252 = D_80157240;
    D_80156BE0.buf216 = D_8006A090;
    D_80156BE0.buf88 = D_800586D0;
    D_80156BE0.n92 = 2;
    D_80156BE0.n220 = 2;
    D_80156BE0.f124 = D_80156CEC;
    D_8015B250 = D_800586D0;
    D_8015B260 = D_800586D0 + 1728;
    D_801497C8 = D_801497F4 = D_800586D0 + 0x9CC0;
    func_800A5B3C();
    func_800A5488();
    particle_velocity_set();
    hud_setup(32, 16, 16, 5, 64, 1.0f, 200.0f, 0);
    if (!D_80156994) {
        if (D_8011025C) {
            func_800960CC((u32)D_8011025C);
            D_8011025C = 0;
        }
        if (D_80110260) {
            func_800960CC((u32)D_80110260);
            D_80110260 = 0;
        }
    }
    effect_spawn();
    player_state_set(-1, 0);
    func_800DE45C();
    viTickStart();
    HAL_InitText_Init();
}

void _InitSemaphore_Init(void)
{
    if (!D_801147C0) {
        D_801147C0 = 1;
        osCreateMesgQueue(&D_801461D0, D_801461FC, 1);
        osSendMesg(&D_801461D0, (OSMesg)0, 0);
    }
}

void HAL_InitText_Init(void)
{
    _InitSemaphore_Init();
    D_80149DA0 = -1;
}
