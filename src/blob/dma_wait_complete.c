/* flags: -g0 -O3 -mips2 -G 0 -non_shared  (one file: controller_rumble_thunk is inlined here; the audio_queue_process.c bodies are in the file only for the `.align 5` position, see below) */
/*
 * Historical label dma_wait_complete is misleading: this is a scheduler-client
 * thread entry (N64-only, no arcade ancestor).  It registers a client on the
 * scheduler D_8002E8E8 with its own 8-slot queue D_80152788 and loops forever
 * on the queue.  Message type 1 (retrace): if the callback D_8011EAA8 is set,
 * spin while the scheduler's current task (+0x274) is D_8002AFA0, then call
 * it; then advance the clock D_80152748 by D_8002AFB8 and wrap it at 14400.
 * Message type 4 (pre-NMI): controller_rumble_thunk(), i.e. func_800205E4().
 *
 * Whole-program context this body needs:
 *   - controller_rumble_thunk (0x8008AA20, the function in front of it) is
 *     inlined by umerge for `case 4`; the inlined call is the 8 frame bytes
 *     that a direct func_800205E4() call leaves missing (96 instead of 104).
 *   - the unreachable epilogue after the endless loop is `.align 5`, so the
 *     nop count depends on this function's offset in the object.  Retail has
 *     it 0x10 past a 32-byte boundary (the whole-program object starts at
 *     0x80086A50).  Here the real neighbours sync_release_video,
 *     func_8008A704 and audio_queue_process (0x350 bytes, bodies unchanged
 *     from cloud/matches/audio_queue_process.c) and the thunk (0x20) are
 *     emitted in front of it: the definitions of this file's two functions
 *     come first in the source because umerge emits uncalled roots in
 *     reverse definition order.  In tools.conveyor.pipeline.blob_unit the
 *     body is 103 words instead of 105 for the same reason (two nops) and
 *     needs an `align` entry in src/blob/unit_overrides.json, like
 *     audio_queue_process and task_complete_signal.
 *
 * Shaping quirks (match the bytes, may not be the original spelling):
 *   - the callback is called with itself as argument, `D_8011EAA8(D_8011EAA8)`:
 *     retail keeps the pointer in a0 (`lw a0; beqzl a0; ...; jalr a0`); a
 *     plain `D_8011EAA8()` colours it v1 (4 words).  INFERRED: the real
 *     callback may take some other value that IDO folded onto the same web.
 *   - `void *volatile cur` on the scheduler field +0x274 (retail reloads it in
 *     the spin loop while D_8002AFA0 stays in v0); operand order
 *     `D_8002AFA0 == D_8002E8E8.cur`.
 *   - the scheduler extern is typed here (struct Sched) instead of the `s32`
 *     audio_queue_process.c uses; that file only takes its address.
 */
typedef float f32;
/*
 * Historical label audio_queue_process is misleading: this is the Rumble Pak
 * thread entry (N64-only, no arcade ancestor).  It registers a scheduler
 * client, and on every retrace message walks the four 0x304-byte controller
 * records at D_80144030 under the SI lock (queue D_801497D0):
 *   - phase (+0x80) counts 0..9;
 *   - current level (+0x7E) follows the requested level (+0x7D, capped at 40)
 *     upward at once and downward by one per frame;
 *   - the motor is on when rumble is enabled (D_8011EAE4), level > 0 and
 *     either level >= 10 or bit `phase` of the duty table D_8011ECEC[level]
 *     is set (PWM for weak levels); otherwise off;
 *   - motor on/off goes through 0x80009F20(pfs, on) (label osMotorInit: really
 *     the motor access call) and, on failure, one re-init through
 *     0x8000A194(siQueue, pfs, port) (label osMotorStart: really the pak init)
 *     followed by a single retry.
 *
 * Whole-program context this body needs (all are real neighbours):
 *   - func_8008A704 (0x8008A704, the lazy-initialising lock) is inlined here by
 *     umerge.  It only reproduces (and itself scores MATCH from this file)
 *     when its one-time initialisation is a separate static, inlined and
 *     deleted: that is the `jr ra; nop` stub func_8008A6FC in front of it.
 *   - the stub func_8008A774 between the lock and this function is a second
 *     deleted static.  Each inlined call costs 8 bytes of this frame, and the
 *     frame has room for exactly three (lock, lock init, one more), so the
 *     unlock at the bottom is that static, with a direct osJamMesg.  Its body
 *     is INFERRED (calling the matched sync_release_video instead gives the
 *     same code with a frame 8 bytes too large).
 *   - the epilogue after the endless loop is `.align 5`: the nop count
 *     depends on this function's offset in the object.  Retail has
 *     sync_release_video, stub, lock, stub in front of it (0xAC bytes from a
 *     32-byte boundary); sync_release_video is defined last here because
 *     umerge emits uncalled roots in reverse definition order, which puts it
 *     first in the object.
 *
 * Shaping quirks (match the bytes, may not be the original spelling):
 *   - `volatile` on fields +0x7E and +0x80 (retail reloads them);
 *   - `switch (present) { case 0: skip: continue; }` with `goto skip` exits:
 *     retail keeps an empty block after the presence test that every exit of
 *     the body joins (bnezl/b pair); a plain `if (...) continue;` folds it;
 *   - goto layout of the on/off decision (off block first, duty-table test
 *     last), matching retail block order;
 *   - unused local `p` supplies 4 bytes of the frame; operand order
 *     `table & (1 << phase)`.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;

typedef struct { s32 w[6]; } OSMesgQueue;
typedef void *OSMesg;
typedef struct { void *next; OSMesgQueue *msgQ; } OSScClient;

typedef struct {
    /* 0x000 */ char pad0[6];
    /* 0x006 */ s8 present;
    /* 0x007 */ char pad7[5];
    /* 0x00C */ char pfs[0x70];
    /* 0x07C */ s8 motorOn;
    /* 0x07D */ s8 request;
    /* 0x07E */ volatile s8 level;
    /* 0x07F */ s8 unk7F;
    /* 0x080 */ volatile s16 phase;
    /* 0x082 */ char pad82[0x282];
} Controller; /* 0x304 */

extern OSMesgQueue D_80154378;   /* retrace queue of this thread */
extern OSMesg D_8015439C;
extern OSMesgQueue D_801497D0;   /* one-slot queue used as the SI lock */
extern OSMesg D_801527E4;
typedef struct {
    char pad[0x274];
    void *volatile cur;
} Sched;
extern Sched D_8002E8E8;         /* scheduler */
extern OSMesgQueue D_80035458;   /* SI message queue */
extern s8 D_8011194C;            /* lock initialised */
extern s8 D_8011EAE4;            /* rumble enabled */
extern u16 D_8011ECEC[];         /* duty masks for levels 0..9 */
extern Controller D_80144030[4];

void osCreateMesgQueue(OSMesgQueue *mq, OSMesg *msg, s32 count);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flags);
s32 osJamMesg(OSMesgQueue *mq, OSMesg msg, s32 flags);
void osScAddClient(void *sc, OSScClient *c, OSMesgQueue *mq);
s32 osMotorInit(void *pfs, s32 on);
s32 osMotorStart(OSMesgQueue *mq, void *pfs, s32 port);
void sync_release_video(void);


/* ---- this file's claim: controller_rumble_thunk (context, already locked) and dma_wait_complete ---- */
extern OSMesgQueue D_80152788;   /* this thread's queue */
extern OSMesg D_801527A8[8];
extern f32 D_80152748;           /* accumulator, wraps at 14400 */
extern void (*D_8011EAA8)();     /* per-retrace callback, may be null */
extern void *D_8002AFA0;         /* task the callback must not race */
extern f32 D_8002AFB8;           /* increment per retrace */

void func_800205E4(void);

void controller_rumble_thunk(void) {
    func_800205E4();
}

void dma_wait_complete(void *arg) {
    OSScClient client;
    s16 *msg = 0;

    osCreateMesgQueue(&D_80152788, D_801527A8, 8);
    osScAddClient(&D_8002E8E8, &client, &D_80152788);
    D_80152748 = 0.0f;
    while (1) {
        osRecvMesg(&D_80152788, (OSMesg *) &msg, 1);
        switch (*msg) {
        case 1:
            if (D_8011EAA8 != 0) {
                while (D_8002AFA0 != 0 && D_8002AFA0 == D_8002E8E8.cur) {
                }
                D_8011EAA8(D_8011EAA8);
            }
            D_80152748 += D_8002AFB8;
            if (D_80152748 > 14400.0f) {
                D_80152748 -= 14400.0f;
            }
            break;
        case 4:
            controller_rumble_thunk();
            break;
        }
    }
}

/* ---- context: cloud/matches/audio_queue_process.c, bodies unchanged (see header) ---- */
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

static void func_8008A774(void) {
    osJamMesg(&D_801497D0, 0, 0);
}

void audio_queue_process(void *arg) {
    OSScClient client;
    OSMesg msg;
    s32 i;
    s32 retried;
    s32 level;
    Controller *p;

    osCreateMesgQueue(&D_80154378, &D_8015439C, 1);
    osScAddClient(&D_8002E8E8, &client, &D_80154378);
    for (;;) {
        osRecvMesg(&D_80154378, &msg, 1);
        func_8008A704();
        for (i = 0; i < 4; i++) {
            switch (D_80144030[i].present) {
            case 0:
skip:
                continue;
            }
            retried = 0;
            if (++D_80144030[i].phase >= 10) {
                D_80144030[i].phase = 0;
            }
            level = D_80144030[i].request;
            if (level > 40) {
                level = 40;
            }
            if (level < D_80144030[i].level) {
                level = D_80144030[i].level - 1;
            }
            D_80144030[i].level = level;
            if (D_8011EAE4 == 0) goto off;
            if (level > 0) goto check;
off:
            if (D_80144030[i].motorOn) {
                if (!D_8011EAE4) {
                    osMotorStart(&D_80035458, D_80144030[i].pfs, i);
                }
                while (1) {
                    if (osMotorInit(D_80144030[i].pfs, 0) == 0) {
                        D_80144030[i].motorOn = 0;
                        break;
                    }
                    if (retried) break;
                    if (osMotorStart(&D_80035458, D_80144030[i].pfs, i) != 0) break;
                    retried = 1;
                }
            }
            goto skip;
check:
            if (level < 10) goto check2;
on:
            if (!D_80144030[i].motorOn) {
                while (1) {
                    if (osMotorInit(D_80144030[i].pfs, 1) == 0) {
                        D_80144030[i].motorOn = 1;
                        break;
                    }
                    if (retried) break;
                    if (osMotorStart(&D_80035458, D_80144030[i].pfs, i) != 0) break;
                    retried = 1;
                }
            }
            goto skip;
check2:
            if (D_8011ECEC[level] & (1 << D_80144030[i].phase)) goto on;
            goto off;
        }
        func_8008A774();
    }
}

void sync_release_video(void) {
    osJamMesg(&D_801497D0, 0, 0);
}

