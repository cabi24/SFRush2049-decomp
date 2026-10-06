/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800E7710 (historical label): controller subsystem init. Creates the SI event queue D_80035458
 * (8-entry buffer D_80150F18), registers it for OS_EVENT_SI (5) with a message pointing at a static s16
 * holding 5 (osSetEventMesg, 0x80006E10, historical label osSetEventMesgAlt), then under the SI/pak lock
 * (queue D_801497D0, created once with D_8011194C as the init flag; same helpers as the matched
 * func_800A1A60 / check_mpath_save) calls osContInit(&D_80035458, &D_80111950 [bit pattern],
 * D_80149440 [OSContStatus[]]) (0x80009450, historical label __osContBuildPacket), releases the lock and
 * clears the first byte of the four 16-byte rumble records D_80156CF0. N64-only, no arcade ancestor.
 *
 * Shaping (w8d): the message is a FUNCTION-LOCAL static (own .bss at 0x80150F58, nothing else in the image
 * references that address). As an extern, uopt CSEs the store address with the argument address into one
 * register (`la a2; sh t6,0(a2)`); as a local static ugen keeps `sh t6,%lo(sym)(at)` and builds the
 * argument separately, as retail. (An extern struct at D_80150F18 with the message at +0x40 also gives
 * identical code but no such struct exists: D_80150F38/F40 belong to other code.)
 */
typedef unsigned char u8;typedef signed char s8;typedef unsigned short u16;typedef short s16;
typedef unsigned int u32;typedef int s32;
typedef void *OSMesg;
typedef struct OSThread OSThread;
typedef struct OSMesgQueue {OSThread *mtqueue,*fullqueue;s32 validCount,first,msgCount;OSMesg *msg;} OSMesgQueue;
typedef struct OSContStatus {u16 type;u8 status,errno;} OSContStatus;
typedef struct Rumble16 {u8 flag;u8 pad[15];} Rumble16;
extern void osCreateMesgQueue(OSMesgQueue *,OSMesg *,s32);
extern s32 osJamMesg(OSMesgQueue *,OSMesg,s32),osRecvMesg(OSMesgQueue *,OSMesg *,s32);
extern void osSetEventMesgAlt(s32,OSMesgQueue *,OSMesg);  /* SDK osSetEventMesg */
extern s32 __osContBuildPacket(OSMesgQueue *,u8 *,OSContStatus *);  /* SDK osContInit */
extern s8 D_8011194C;
extern OSMesgQueue D_801497D0;
extern OSMesg D_801527E4;
extern OSMesgQueue D_80035458;
extern OSMesg D_80150F18[8];
extern u8 D_80111950;
extern OSContStatus D_80149440[];
extern Rumble16 D_80156CF0[4];

static void pak_queue_init(void)
{
    if (!D_8011194C) {
        D_8011194C = 1; osCreateMesgQueue(&D_801497D0,&D_801527E4,1);
        osJamMesg(&D_801497D0,0,0);
    }
}

static void pak_lock(void)
{
    OSMesg message;

    pak_queue_init();
    osRecvMesg(&D_801497D0,&message,1);
}

static void pak_unlock(void)
{
    osJamMesg(&D_801497D0,0,0);
}

void func_800E7710(void)
{
    static s16 si_message;  /* 0x80150F58, only referenced here */
    s32 i;

    osCreateMesgQueue(&D_80035458, D_80150F18, 8);
    si_message = 5;
    osSetEventMesgAlt(5, &D_80035458, &si_message);
    pak_lock();
    __osContBuildPacket(&D_80035458, &D_80111950, D_80149440);
    pak_unlock();
    for (i = 0; i < 4; i++) {
        D_80156CF0[i].flag = 0;
    }
}
