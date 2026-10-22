/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* CONTEXT (byte-identical body of the locked frontier_rumble_thread member func_8008A704 = the public pak/SI lock and
 * its deleted static init) so the group compile inlines it into PS_PakServer as the unit does. Not claimed. */
typedef signed char s8;
typedef signed int s32;
typedef void *OSMesg;
typedef struct OSMesgQueue OSMesgQueue;
extern OSMesgQueue D_801497D0;   /* one-slot queue used as the SI lock */
extern OSMesg D_801527E4;
extern s8 D_8011194C;            /* lock initialised */
void osCreateMesgQueue(OSMesgQueue *mq, OSMesg *msg, s32 count);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flags);
s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flags);

static void func_8008A6FC(void) {
    D_8011194C = 1;
    osCreateMesgQueue(&D_801497D0, &D_801527E4, 1);
    osSendMesg(&D_801497D0, 0, 0);
}

void func_8008A704(void) {
    OSMesg msg;

    if (!D_8011194C) {
        func_8008A6FC();
    }
    osRecvMesg(&D_801497D0, &msg, 1);
}
