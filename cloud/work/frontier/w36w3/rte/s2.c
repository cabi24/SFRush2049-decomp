/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef void *OSMesg;
typedef struct OSMesgQueue { u8 opaque[0x18]; } OSMesgQueue;
typedef struct OSScClient { struct OSScClient *next; OSMesgQueue *msgQ; } OSScClient;

extern u16 D_801525F0;
extern u8 D_8002E8E8[];

extern void __setfpcsr(u32 value);
void osCreateMesgQueue(OSMesgQueue *mq, OSMesg *msg, s32 count);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flags);
void osScAddClient(void *sc, OSScClient *c, OSMesgQueue *msgQ);
void func_800E7710(void);
void func_800E762C(s32 arg0);
s32 func_800E73D8(void);
void func_800E6AF8(void);

void render_thread_entry(void *arg) {
    static OSMesgQueue queue;     /* 0x8017A498 */
    static OSMesg msgs[8];        /* 0x8017A4B8 */
    OSScClient client;
    OSMesg msg;

    msg = 0;
    __setfpcsr(0x01000E00);
    osCreateMesgQueue(&queue, msgs, 8);
    func_800E7710();
    D_801525F0 = 0;
    func_800E762C(0);
    osScAddClient(D_8002E8E8, &client, &queue);
    while (1) {
        osRecvMesg(&queue, &msg, 1);
        do {
        } while (osRecvMesg(&queue, &msg, 0) != -1);
        if (func_800E73D8() && D_801525F0) {
            func_800E6AF8();
        }
    }
}
