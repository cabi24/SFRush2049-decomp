/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800E7038 (0x800E7038, 252 B), w36w3 (wave 36): controller-subsystem (re)initialisation, N64-only (no arcade
 * or MAT3 GameCube ancestor). Called by func_800E7134 (the pad reader) while D_80111954 is clear and by
 * effect_spawn. The first time it enables every controller (player_state_set(-1, 1), the per-controller connected
 * flags) and turns stick-as-dpad on for all (player_mode_set(-1, 1)); it then resets the per-axis stick
 * calibration of the four controllers (gStickCal, D_80149AF8) to 70, creates the SI lock queue D_801497A8
 * (one message, D_80152730) once and primes it, waits while the osContInit bit-pattern byte D_80111950 reads 0x80,
 * and reads the pads (controller_poll).
 * Shaping (each verified necessary):
 *  - the queue's created-once flag is a FUNCTION-LOCAL static (own .data 0x80111968; no other function references
 *    that address). As a global, uopt keeps its address and the shared constant 1 of both flag stores in
 *    registers (v0/v1, 46 words); as a local static the stores take a fresh `li t7,1` each, as retail does;
 *  - the calibration reset is a 4-trip loop that IDO unrolls completely (8 separate `li tN,70`; straight-line
 *    stores share one 70 register), with [1] stored before [0] (ugen temp ring t8,t9,t0..t5 as retail);
 *  - gStickCal is DEFINED here, so as1 shares one `lui at` over the 8 stores (with an extern each store gets its
 *    own lui);
 *  - the two calibration stores are on ONE line: on two lines as1 swaps the last pair of stores (2 words).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
typedef void *OSMesg;
typedef struct OSThread OSThread;
typedef struct OSMesgQueue {OSThread *mtqueue,*fullqueue;s32 validCount,first,msgCount;OSMesg *msg;} OSMesgQueue;

extern s8 D_80111954;              /* controller subsystem initialised (also read by func_800E7134) */
extern u8 D_80111950;              /* osContInit bit pattern */
s8 D_80149AF8[4][2];               /* gStickCal: per-controller x/y stick calibration */
extern OSMesgQueue D_801497A8;     /* SI lock queue */
extern OSMesg D_80152730;          /* its one-message buffer */
void player_state_set(s32 player, s32 value);
void player_mode_set(s32 player, s32 value);
void controller_poll(void);
extern void osCreateMesgQueue(OSMesgQueue *, OSMesg *, s32);
extern s32 osSendMesg(OSMesgQueue *, OSMesg, s32);

void func_800E7038(void)
{
    static s8 queueCreated = 0;    /* 0x80111968 */
    s32 i;

    if (D_80111954 == 0) {
        player_state_set(-1, 1);
        player_mode_set(-1, 1);
    }
    D_80111954 = 1;
    for (i = 0; i < 4; i++) {
        D_80149AF8[i][1] = 70; D_80149AF8[i][0] = 70;
    }
    if (queueCreated == 0) {
        queueCreated = 1;
        osCreateMesgQueue(&D_801497A8, &D_80152730, 1);
        osSendMesg(&D_801497A8, 0, 0);
    }
    while (*(volatile u8 *)&D_80111950 == 0x80) {
    }
    controller_poll();
}
