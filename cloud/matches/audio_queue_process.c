/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* one file: func_8008A704, its deleted static func_8008A6FC, the deleted static func_8008A774 and sync_release_video must be defined here; see below */
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
extern s32 D_8002E8E8;           /* scheduler */
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
