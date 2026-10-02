/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/*
 * Initialize a one-message synchronization queue once, then reset a state byte.
 * N64-specific adaptation using libultra queues. No direct arcade equivalent is
 * established; the Rush The Rock reference checkout is unavailable here.
 *
 * The named initialized value is the actual byte written to the guard. Its
 * local lifetime preserves the native IDO O2 address/constant scheduling.
 * No extra operation, argument, helper, volatile access or stack pad is used.
 */
typedef signed char s8;
typedef int s32;
typedef void *OSMesg;
typedef struct {
    void *mtqueue;
    void *fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    OSMesg *msg;
} OSMesgQueue;

void osCreateMesgQueue(OSMesgQueue *, OSMesg *, s32);
s32 osJamMesg(OSMesgQueue *, OSMesg, s32);
extern s8 D_801147C0;
extern s8 D_80149DA0;
extern OSMesgQueue D_801461D0;
extern OSMesg D_801461FC;

void sync_init_conditional(void)
{
    const s8 initialized = 1;

    if (!D_801147C0) {
        D_801147C0 = initialized;
        osCreateMesgQueue(&D_801461D0, &D_801461FC, 1);
        osJamMesg(&D_801461D0, (OSMesg)0, 0);
    }
    D_80149DA0 = -1;
}
