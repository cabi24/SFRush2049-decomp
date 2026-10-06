/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned char u8;typedef signed char s8;typedef unsigned short u16;typedef short s16;
typedef unsigned int u32;typedef int s32;
typedef void *OSMesg;
typedef struct OSThread OSThread;
typedef struct OSMesgQueue {OSThread *mtqueue,*fullqueue;s32 validCount,first,msgCount;OSMesg *msg;} OSMesgQueue;
typedef struct OSContStatus {u16 type;u8 status,errno;} OSContStatus;
typedef struct Rumble16 {u8 flag;u8 pad[15];} Rumble16;
extern void osCreateMesgQueue(OSMesgQueue *,OSMesg *,s32);
extern s32 osJamMesg(OSMesgQueue *,OSMesg,s32),osRecvMesg(OSMesgQueue *,OSMesg *,s32);
extern void osSetEventMesgAlt(s32,OSMesgQueue *,OSMesg);
extern s32 __osContBuildPacket(OSMesgQueue *,u8 *,OSContStatus *);
extern s8 D_8011194C;
extern OSMesgQueue D_801497D0;
extern OSMesg D_801527E4;
extern OSMesgQueue D_80035458;
extern OSMesg D_80150F18[8];
extern s16 D_80150F58;
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
    s32 i;

    osCreateMesgQueue(&D_80035458, D_80150F18, 8);
    D_80150F58 = 5;
    osSetEventMesgAlt(5, &D_80035458, &D_80150F58);
    pak_lock();
    __osContBuildPacket(&D_80035458, &D_80111950, D_80149440);
    pak_unlock();
    for (i = 0; i < 4; i++) {
        D_80156CF0[i].flag = 0;
    }
}
