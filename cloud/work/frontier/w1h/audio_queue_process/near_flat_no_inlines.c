/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
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
    char pad0[6];
    s8 unk6;
    char pad7[5];
    char pfs[0x70];   /* 0x0C */
    s8 unk7C;
    s8 unk7D;
    volatile s8 unk7E;
    s8 unk7F;
    volatile s16 unk80;
    char pad82[0x282];
} Player; /* 0x304 */

extern OSMesgQueue D_80154378;
extern OSMesg D_8015439C;
extern OSMesgQueue D_801497D0;
extern OSMesg D_801527E4;
extern s32 D_8002E8E8;
extern OSMesgQueue D_80035458;
extern s8 D_8011194C;
extern s8 D_8011EAE4;
extern u16 D_8011ECEC[];
extern Player D_80144030[4];

void osCreateMesgQueue(OSMesgQueue *mq, OSMesg *msg, s32 count);
s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flags);
s32 osJamMesg(OSMesgQueue *mq, OSMesg msg, s32 flags);
void osScAddClient(void *sc, OSScClient *c, OSMesgQueue *mq);
s32 osMotorInit(void *pfs, s32 flag);
s32 osMotorStart(OSMesgQueue *mq, void *pfs, s32 channel);

void audio_queue_process(void *arg) {
    OSScClient client;
    OSMesg msg;
    OSMesg msg2[8];
    s32 i;
    s32 tries;
    s32 level;

    osCreateMesgQueue(&D_80154378, &D_8015439C, 1);
    osScAddClient(&D_8002E8E8, &client, &D_80154378);
    for (;;) {
        osRecvMesg(&D_80154378, &msg, 1);
        if (!D_8011194C) {
            D_8011194C = 1;
            osCreateMesgQueue(&D_801497D0, &D_801527E4, 1);
            osJamMesg(&D_801497D0, 0, 0);
        }
        osRecvMesg(&D_801497D0, msg2, 1);
        for (i = 0; i < 4; i++) {
            if (!D_80144030[i].unk6) {
                continue;
            }
            tries = 0;
            if (++D_80144030[i].unk80 >= 10) {
                D_80144030[i].unk80 = 0;
            }
            level = D_80144030[i].unk7D;
            if (level > 40) {
                level = 40;
            }
            if (level < D_80144030[i].unk7E) {
                level = D_80144030[i].unk7E - 1;
            }
            D_80144030[i].unk7E = level;
            if (D_8011EAE4 == 0) goto off;
            if (level > 0) goto check;
off:
            if (D_80144030[i].unk7C) {
                if (!D_8011EAE4) {
                    osMotorStart(&D_80035458, D_80144030[i].pfs, i);
                }
                while (1) {
                    if (osMotorInit(D_80144030[i].pfs, 0) == 0) {
                        D_80144030[i].unk7C = 0;
                        break;
                    }
                    if (tries) break;
                    if (osMotorStart(&D_80035458, D_80144030[i].pfs, i) != 0) break;
                    tries = 1;
                }
            }
            continue;
check:
            if (level < 10) goto check2;
on:
            if (!D_80144030[i].unk7C) {
                while (1) {
                    if (osMotorInit(D_80144030[i].pfs, 1) == 0) {
                        D_80144030[i].unk7C = 1;
                        break;
                    }
                    if (tries) break;
                    if (osMotorStart(&D_80035458, D_80144030[i].pfs, i) != 0) break;
                    tries = 1;
                }
            }
            continue;
check2:
            if (D_8011ECEC[level] & (1 << D_80144030[i].unk80)) goto on;
            goto off;
        }
        osJamMesg(&D_801497D0, 0, 0);
    }
}
