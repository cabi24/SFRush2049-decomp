/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * render_thread_entry (0x800E7808, 268 B; historical label) = ModelThread@model.c, N64 thread form (Dreamcast
 * model.c ModelThread is exact for the per-tick body; MAT3 GC model.c 0x8002CA78). Created by the static game_init
 * as the scheduler-client thread (id 5, pri 7). The GC per-call sInited block runs once before the loop:
 * modelrun (D_801525F0) = 0; init_model_task(0) (func_800E762C). Each tick: block on the client queue, drain the
 * queued messages, then `if (ControllerUpdate() && modelrun) model_iteration();` (func_800E73D8 / model_iteration).
 * SHAPING (disclosed, compiles to no code): the drain loop tests each message's scheduler type in a compiled-out
 * `if` (OS_SC_RETRACE_MSG 1 / OS_SC_PRE_NMI_MSG 4 of the SDK sched.h; any second value gives the same code). uopt
 * keeps the type load as a coloured web (retail's never-used s2 save) and the constant 1 as a register candidate
 * in the drain loop (retail's `li s1,1` / `move a2,s1`); ugen drops the empty branch and as1 deletes the dead loads
 * (retail's lhu t7: one ring pop for the deleted msg load). Without it: the constant 1 is no candidate and two
 * s-register webs are missing (w39c: 30-33 rows, colour only). An `if (...) {}` around a debug print is the likely
 * original. No unused locals, no volatile.
 * Group packaging (layout only): func_800E7710.c and func_800E6AF0.c are byte copies of locked singles that put
 * render_thread_entry at its retail offset modulo 32 for the `.align 5` pad of the unreachable epilogue.
 */
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef void *OSMesg;
typedef struct OSMesgQueue { u8 opaque[0x18]; } OSMesgQueue;
typedef struct OSScClient { struct OSScClient *next; OSMesgQueue *msgQ; } OSScClient;
typedef struct OSScMsg { s16 type; s8 misc[30]; } OSScMsg;

#define OS_SC_RETRACE_MSG 1
#define OS_SC_PRE_NMI_MSG 4

extern OSMesgQueue D_8017A498;      /* model thread client queue */
extern OSMesg D_8017A4B8[8];        /* its messages */
extern u16 D_801525F0;              /* modelrun */
extern u8 D_8002E8E8[];             /* scheduler */

extern void __setfpcsr(u32 value);
void osCreateMesgQueue(OSMesgQueue *mq, OSMesg *msg, s32 count);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flags);
void osScAddClient(void *sc, OSScClient *c, OSMesgQueue *msgQ);
void func_800E7710(void);           /* controller init (osSetEventMesg SI) */
void func_800E762C(s32 arg0);       /* init_model_task */
s32 func_800E73D8(void);            /* ControllerUpdate */
void model_iteration(void);

void render_thread_entry(void *arg)
{
    OSScClient client;
    OSMesg msg;

    msg = 0;
    __setfpcsr(0x01000E00);
    osCreateMesgQueue(&D_8017A498, D_8017A4B8, 8);
    func_800E7710();
    D_801525F0 = 0;
    func_800E762C(0);
    osScAddClient(D_8002E8E8, &client, &D_8017A498);
    while (1) {
        osRecvMesg(&D_8017A498, &msg, 1);
        do {
            if (((OSScMsg *)msg)->type == OS_SC_RETRACE_MSG || ((OSScMsg *)msg)->type == OS_SC_PRE_NMI_MSG) {}
        } while (osRecvMesg(&D_8017A498, &msg, 0) != -1);
        if (func_800E73D8() && D_801525F0) {
            model_iteration();
        }
    }
}
