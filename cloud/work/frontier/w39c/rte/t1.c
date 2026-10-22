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

extern OSMesgQueue D_8017A498;
extern OSMesg D_8017A4B8[8];
extern /*@{*//*@|volatile @}*/u16 D_801525F0;
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
    OSScClient client;
    OSMesg msg;
    /*@{v*//*@|s32 t;@|s32 n;@|s32 t; s32 n;@}*/

    msg = 0;
    __setfpcsr(0x01000E00);
    osCreateMesgQueue(&D_8017A498, D_8017A4B8, 8);
    func_800E7710();
    D_801525F0 = 0;
    func_800E762C(0);
    osScAddClient(D_8002E8E8, &client, &D_8017A498);
    /*@{v*//*@|@|n = 1;@|n = 1;@}*/
    /*@{*/while (1)/*@|for (;;)@}*/ {
        /*@{v*//*@|t = (s32)msg == 1;@|@|t = n == 1;@}*/
        osRecvMesg(&D_8017A498, &msg, 1);
        /*@{d*/do {
            /*@{v*//*@|if (t) {} if (t) {}@|n++;@|if (t) {} if (t) {}@}*/
        } while (osRecvMesg(&D_8017A498, &msg, 0) != -1);/*@|while (osRecvMesg(&D_8017A498, &msg, 0) != -1) {
            /*@{v*//*@|if (t) {} if (t) {}@|n++;@|if (t) {} if (t) {}@}*/
        }@}*/
        /*@{c*/if (func_800E73D8() && D_801525F0) {
            func_800E6AF8();
        }/*@|if (func_800E73D8()) { if (D_801525F0) { func_800E6AF8(); } }@|if (func_800E73D8() != 0 && D_801525F0 != 0) { func_800E6AF8(); }@}*/
    }
}

extern s32 __padA, __padB;
void __pad24(void) { __padA = 0; __padB = 0; }
