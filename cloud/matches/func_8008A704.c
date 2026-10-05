/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* needs -O3: the static below must be inlined */
/*
 * SI/controller-pak lock acquire with lazy initialisation (N64-only): on the
 * first call it creates the one-slot queue D_801497D0 and primes it with one
 * message; then it takes the message (blocking).  sync_release_video
 * (0x8008A6D0) is the matching release (osJamMesg on the same queue).
 *
 * Shaping fact, not a quirk: the one-time initialisation is a separate static
 * function.  umerge inlines it and deletes it, leaving the `jr ra; nop` stub
 * that retail has directly in front of this function (func_8008A6FC,
 * 0x8008A6FC).  Only that form puts `li t7,1` in the block before the branch
 * (`sw ra` in the delay slot); the flat body is 2/28 words off under every
 * layout (see cloud/work/workbench_pilot_C1.md).
 * The same file also scores MATCH for audio_queue_process, which inlines this
 * function (cloud/matches/audio_queue_process.c).
 */
typedef signed char s8;
typedef int s32;

typedef struct { s32 w[6]; } OSMesgQueue;
typedef void *OSMesg;

extern OSMesgQueue D_801497D0;   /* one-slot queue used as the lock */
extern OSMesg D_801527E4;
extern s8 D_8011194C;            /* lock initialised */

void osCreateMesgQueue(OSMesgQueue *mq, OSMesg *msg, s32 count);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flags);
s32 osJamMesg(OSMesgQueue *mq, OSMesg msg, s32 flags);

static void func_8008A6FC(void) {
    D_8011194C = 1;
    osCreateMesgQueue(&D_801497D0, &D_801527E4, 1);
    osJamMesg(&D_801497D0, 0, 0);
}

void func_8008A704(void) {
    OSMesg msg;

    if (!D_8011194C) {
        func_8008A6FC();
    }
    osRecvMesg(&D_801497D0, &msg, 1);
}
