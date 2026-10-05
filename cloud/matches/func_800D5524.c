/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* the file must also define player_conditional_check: -O3 inlines it at all six call sites */
/*
 * Stops and clears every looping sound handle owned by one car (N64 sound
 * bookkeeping; no arcade ancestor identified -- the arcade keeps these in
 * the MODELDAT/sound tables differently).   is the car's MODELDAT
 * (slot at +0x7C6, s8 mode at +0x7CC).  Runs once per armed flag
 * D_8010FFC4[slot]; D_8010FFCC[slot] is set to 1 for even slots.
 *   - D_80140420[slot] (0x54 bytes: s32 + four 0x14-byte handle records):
 *     records 0, 1 and 3 are released if their handle is not -1;
 *   - D_80140640[slot] is always released;
 *   - only when mode == 2: the three records D_801406C0[slot][0..2] and the
 *     bare handles D_80140AE0/D_801407E0/D_801407C0[slot] (set to -1), with
 *     D_80140A08[slot] and D_80140B10[slot] zeroed.
 * A record is released by player_conditional_check(rec, flag) (0x800D54E0,
 * already matched; reproduced unchanged here): flag != 0 calls
 * results_screen_update(handle), flag == 0 calls scheduler_recv(handle), then
 * player_conditional_call resets the record (handle -1, four -2.0f).
 * The historical labels of all four callees are misleading.
 *
 * Whole-program fact: player_conditional_check is inlined by umerge at every
 * call (frame +8, 'li at,40' loop bound left unhoisted).  Written out by hand
 * the same statements keep 40 in s7.
 * Shaping quirk: the code-free dead read 'if (i) {}' in the first loop.  It
 * raises the counter web above the pointer web (s1/s2) and with it m above
 * the constant 2 (s3/s4); without it 19/157 words differ, all register names.
 */
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

void player_conditional_check(SndRec *rec, s32 arg1) {
    if (arg1 != 0) {
        results_screen_update(rec->handle);
    } else {
        scheduler_recv(rec->handle);
    }
    player_conditional_call(rec);
}

void func_800D5524(MODELDAT *m) {
    s32 slot = m->slot;
    s32 i;

    if (D_8010FFC4[slot]) {
        D_8010FFC4[slot] = 0;
        D_8010FFCC[slot] = (slot & 1) == 0;
        for (i = 0; i < 2; i++) {
            if (i) {}
            if (D_80140420[slot].rec[i].handle != -1) {
                player_conditional_check(&D_80140420[slot].rec[i], m->unk7CC != 2);
            }
        }
        if (D_80140420[slot].rec[3].handle != -1) {
            player_conditional_check(&D_80140420[slot].rec[3], m->unk7CC != 2);
        }
        player_conditional_check(&D_80140640[slot], m->unk7CC != 2);
        if (m->unk7CC == 2) {
            player_conditional_check(&D_801406C0[slot][0], 0);
            player_conditional_check(&D_801406C0[slot][1], 0);
            player_conditional_check(&D_801406C0[slot][2], 0);
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
