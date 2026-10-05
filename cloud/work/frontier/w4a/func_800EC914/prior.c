/* flags: -g0 -O3 -mips2 -G 0 -non_shared  (same result at -O2) */
/*
 * NOT a match: 3/149 words differ, one as1 ordering (see RESULTS.md):
 *   retail  lui t4; addiu t4,10088; lui t5; addiu t5,16338; sh a3,0(t4)
 *   this    lui t4; lui t5; addiu t5,16338; addiu t4,10088; sh a3,0(t4)
 *
 * N64 version of arcade munge_gLink_data() (reference/repos/rushtherock/game/
 * mdrive.c:463): from the per-slot link records build model[].slot (the list
 * of active cars), we_control / in_game / drone_type per model, and the
 * humans[] / drones[] / our_drones[] index lists.  N64 differences: only the
 * first D_801543CA slots are examined (the rest get in_game 0), `place` is the
 * slot index, a byte from the link record is copied to game_car[i]+0xEE, the
 * difficulty/marker/track_len block is gone, D_80143FF4 is cleared at the end.
 *
 * What mattered:
 *   - D_801543CA (number of cars) is volatile: its address is kept in t3 and
 *     it is re-read through 0(t3);
 *   - loops 1 and 2 go through the arcade's `m`/`gc` pointer locals, loop 3
 *     indexes model[] directly (a named pointer there loses `.noalias` and
 *     as1 leaves the we_control load behind the count store);
 *   - the counts in loop 3 and the chained zeroing share the local `j`
 *     (j = n; arr[j] = index; n = j + 1).  That keeps j out of a2 (still
 *     owned by `i`), so the model pointer and the our_drones count get a2.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;

#define MAX_LINKS 6
#define LINK_ACTIVE 0x80
#define DRONE 1
#define HUMAN 2

typedef struct {
    /* 0x000 */ u8 pad0[0x7C6];
    /* 0x7C6 */ s16 slot;
    /* 0x7C8 */ s16 in_game;
    /* 0x7CA */ s16 we_control;
    /* 0x7CC */ s8 drone_type;
    /* 0x7CD */ u8 pad7CD[0x3B];
} MODELDAT; /* 0x808 */

typedef struct {
    /* 0x000 */ u8 pad0[0xEE];
    /* 0x0EE */ u8 unkEE;
    /* 0x0EF */ u8 padEF[0x26C];
    /* 0x35B */ s8 place;
    /* 0x35C */ u8 pad35C[0x5C];
} CAR_DATA; /* 0x3B8 */

typedef struct {
    /* 0x0 */ u8 pad0[5];
    /* 0x5 */ u8 unk5;
    /* 0x6 */ u8 flags;
    /* 0x7 */ u8 owner;
} Link; /* 0x8 */

extern volatile s16 D_801543CA;        /* number of cars */
extern s8 D_80152744;         /* num_active_cars */
extern MODELDAT D_8014A250[]; /* model */
extern CAR_DATA D_80152818[]; /* game_car */
extern Link D_80153E88[];     /* gLink */
extern s16 D_8015274C;        /* num_our_drones */
extern s16 D_80152768;        /* num_drones */
extern s16 D_80153FD2;        /* num_humans */
extern s16 D_80143A40[];      /* humans */
extern s16 D_801527D8[];      /* drones */
extern s16 D_80152808[];      /* our_drones */
extern s32 D_80143FF4;

void func_800EC914(void) {
    s32 i;
    s32 index;
    s32 j;
    MODELDAT *m;
    CAR_DATA *gc;

    D_80152744 = 0;
    for (i = 0; i < D_801543CA; i++) {
        m = &D_8014A250[i];
        gc = &D_80152818[i];
        m->we_control = (D_80153E88[i].owner == 0);
        m->drone_type = 0;
        m->in_game = (D_80153E88[i].flags & LINK_ACTIVE) != 0;
        if (m->in_game) {
            D_8014A250[D_80152744++].slot = i;
            gc->place = i;
            gc->unkEE = D_80153E88[i].unk5;
            m->drone_type = (D_80153E88[i].owner < MAX_LINKS) ? DRONE : HUMAN;
        }
    }
    for (i = D_801543CA; i < MAX_LINKS; i++) {
        m = &D_8014A250[i];
        m->we_control = (D_80153E88[i].owner == 0);
        m->drone_type = 0;
        m->in_game = 0;
    }
    D_8015274C = 0;
    j = D_8015274C;
    D_80152768 = j;
    D_80153FD2 = j;
    for (i = 0; i < D_80152744; i++) {
        index = D_8014A250[i].slot;
        if (D_8014A250[index].drone_type == HUMAN) {
            j = D_80153FD2;
            D_80143A40[j] = index;
            D_80153FD2 = j + 1;
        } else {
            j = D_80152768;
            D_801527D8[j] = index;
            D_80152768 = j + 1;
            if (D_8014A250[index].we_control) {
                D_80152808[D_8015274C++] = index;
            }
        }
    }
    D_80143FF4 = 0;
}
