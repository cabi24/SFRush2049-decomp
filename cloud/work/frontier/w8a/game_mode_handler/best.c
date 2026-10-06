/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * game_mode_handler: N64 mode-switch handshake with the scheduler/graphics thread. Sets D_80035472, jams a
 * 2750 message (a 32-byte message record on the stack, type s16 at +0) at the front of queue D_8002ECF8,
 * clears D_80035471, then drains queue D_8002ECC0 (one non-blocking receive, one blocking receive, then
 * non-blocking receives until empty), sets D_80035471 / clears D_80035470, copies D_801497C8 to D_801497F4
 * and calls viUpdateTime, process_inputs and input_init_flag_get. N64-only; no arcade ancestor.
 *
 * Shaping: the three flag bytes are volatile (lui; addiu; sb 0). The first receive's result is compared with
 * -1 in an empty `if` (compiled-out check): that starts the -1 constant web right after the first call, which
 * is where retail materialises `li s2,-1`. The message record is declared before `msg` (frame slots 48 / 44).
 * Also matches at -O2.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef void *OSMesg;
typedef struct OSMesgQueue OSMesgQueue;

s32 osJamMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
void viUpdateTime(void);
void process_inputs(void);
s32 input_init_flag_get(void);

extern volatile u8 D_80035470;
extern volatile u8 D_80035471;
extern volatile u8 D_80035472;
extern OSMesgQueue D_8002ECC0;
extern OSMesgQueue D_8002ECF8;
extern s32 D_801497C8;
extern s32 D_801497F4;

typedef struct { s16 type; s16 pad; u8 data[28]; } SchedMsg;

void game_mode_handler(void)
{
    SchedMsg sm;
    OSMesg msg;

    D_80035472 = 1;
    sm.type = 2750;
    osJamMesg(&D_8002ECF8, &sm, 1);
    D_80035471 = 0;
    if (osRecvMesg(&D_8002ECC0, &msg, 0) == -1) {}
    osRecvMesg(&D_8002ECC0, &msg, 1);
    while (osRecvMesg(&D_8002ECC0, &msg, 0) != -1) {
    }
    D_80035471 = 1;
    D_80035470 = 0;
    D_801497F4 = D_801497C8;
    viUpdateTime();
    process_inputs();
    input_init_flag_get();
}
