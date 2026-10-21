/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef void *OSMesg;
typedef struct OSThread OSThread;
typedef struct OSMesgQueue {OSThread *mtqueue,*fullqueue;s32 validCount,first,msgCount;OSMesg *msg;} OSMesgQueue;

typedef struct {
    u8 bitpattern; u8 p0[3];
    s8 inited; u8 p1[3];
    s32 tick;
    s8 resetTick; u8 p2[3];
    u8 skip; u8 p3[3];
    u8 cnt; u8 p4[3];
    s8 mqInit;
} CtlVars;
/*@{*/extern /*@|@}*/CtlVars D_80111950;
s8 D_80149AF8[4][2];
extern OSMesgQueue D_801497A8;
extern OSMesg D_80152730;
void player_state_set(s32 player, s32 value);
void player_mode_set(s32 player, s32 value);
void controller_poll(void);
extern void osCreateMesgQueue(OSMesgQueue *,OSMesg *,s32);
extern s32 osSendMesg(OSMesgQueue *,OSMesg,s32);

void func_800E7038(void)
{
    s32 i;

    if (D_80111950.inited == 0) {
        player_state_set(-1, 1);
        player_mode_set(-1, 1);
    }
    D_80111950.inited = 1;
    for (i = 0; i < 4; i++) {
        D_80149AF8[i][1] = 70;
        D_80149AF8[i][0] = 70;
    }
    if (D_80111950.mqInit == 0) {
        D_80111950.mqInit = 1;
        osCreateMesgQueue(&D_801497A8, &D_80152730, 1);
        osSendMesg(&D_801497A8, 0, 0);
    }
    while (/*@{*/*(volatile u8 *)&D_80111950.bitpattern/*@| ((volatile CtlVars *)&D_80111950)->bitpattern @}*/ == 0x80) {
    }
    controller_poll();
}
