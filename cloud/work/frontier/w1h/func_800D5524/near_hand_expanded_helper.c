/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef int s32;
typedef float f32;

typedef struct {
    s32 handle;
    f32 unk4[4];
} SndRec; /* 0x14 */

typedef struct {
    s32 unk0;
    SndRec rec[4];
} SndSet; /* 0x54 */

typedef struct {
    u8 pad0[0x7C6];
    s16 slot;
    u8 pad7C8[4];
    s8 unk7CC;
} MODELDAT;

extern s8 D_8010FFC4[];
extern s8 D_8010FFCC[];
extern SndSet D_80140420[];
extern SndRec D_80140640[];
extern SndRec D_801406C0[][3];
extern s32 D_80140AE0[];
extern s16 D_80140A08[];
extern f32 D_80140B10[];
extern s32 D_801407E0[];
extern s32 D_801407C0[];

void results_screen_update(s32 handle);
void scheduler_recv(s32 handle);
void player_conditional_call(SndRec *rec);

void func_800D5524(MODELDAT *m) {
    s32 slot = m->slot;
    s32 i;
    SndRec *p;

    if (D_8010FFC4[slot]) {
        D_8010FFC4[slot] = 0;
        D_8010FFCC[slot] = (slot & 1) == 0;
        for (i = 0; i < 2; i++) {
            if (D_80140420[slot].rec[i].handle != -1) {
                p = &D_80140420[slot].rec[i];
                if (m->unk7CC != 2) {
                    results_screen_update(p->handle);
                } else {
                    scheduler_recv(p->handle);
                }
                player_conditional_call(p);
            }
        }
        if (D_80140420[slot].rec[3].handle != -1) {
            p = &D_80140420[slot].rec[3];
            if (m->unk7CC != 2) {
                results_screen_update(p->handle);
            } else {
                scheduler_recv(p->handle);
            }
            player_conditional_call(p);
        }
        if (m->unk7CC != 2) {
            results_screen_update(D_80140640[slot].handle);
        } else {
            scheduler_recv(D_80140640[slot].handle);
        }
        player_conditional_call(&D_80140640[slot]);
        if (m->unk7CC == 2) {
            p = &D_801406C0[slot][0];
            scheduler_recv(p->handle);
            player_conditional_call(p);
            p = &D_801406C0[slot][1];
            scheduler_recv(p->handle);
            player_conditional_call(p);
            p = &D_801406C0[slot][2];
            scheduler_recv(p->handle);
            player_conditional_call(p);
            scheduler_recv(D_80140AE0[slot]);
            D_80140AE0[slot] = -1;
            D_80140A08[slot] = 0;
            D_80140B10[slot] = 0.0f;
            scheduler_recv(D_801407E0[slot]);
            D_801407E0[slot] = -1;
            scheduler_recv(D_801407C0[slot]);
            D_801407C0[slot] = -1;
        }
    }
}
