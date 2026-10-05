/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/*
 * NOT matchable alone: retail uses s0 and s1 without saving them (whole-program
 * -O3 register convention), so any standalone compile adds sw/lw s0,s1 (+4
 * words, 137/140 positional). In an -O3 group with two stand-in callers
 * (standin_group/) this body scores strict MATCH, but a stand-in group cannot
 * be spliced: it needs the real callers car_setup_confirm and func_800F8EC8.
 *
 * N64 descendant of the place-ranking tail of arcade CheckCPs
 * (reference/repos/rushtherock/game/scp.c:921-954): collect unlocked cars,
 * selection-sort by distance, write game_car[].place. N64 adds the
 * "already finished" count and swaps with XOR.
 * high_index is s32 (s16 adds two sign extensions); place[]/dist_tab[] sizes
 * and local order only fix the 72-byte frame.
 */
typedef float f32;
typedef short s16;
typedef int s32;
typedef signed char s8;

typedef struct {
    char pad0[0x7C6];
    s16 slot;
    char pad7C8[0x20];
    s8 unk7E8;
    char pad7E9[0x1F];
} Model; /* 0x808 */

typedef struct {
    char pad0[0xEE];
    s8 place;
    s8 unkEF;
    char padF0[0x10];
    f32 distance;
    char pad104[0x255];
    s8 unk359;
    char pad35A[0x5E];
} GameCar; /* 0x3B8 */

extern s8 D_80152744;
extern s16 D_80152734;
extern Model D_8014A250[];
extern GameCar D_80152818[];

void func_800D1AB0(void) {
    s16 i;
    s16 j;
    s16 num_left;
    s16 num_done;
    s32 high_index;
    s16 place[6];
    s32 dist_tab[6];
    s16 index;

    num_done = 0;
    num_left = 0;
    for (i = 0; i < D_80152744; i++) {
        if (D_80152818[D_8014A250[i].slot].unk359 > 0 && D_8014A250[i].unk7E8 < D_80152734) {
            num_done++;
        } else if (D_80152818[D_8014A250[i].slot].unkEF != 1) {
            place[num_left++] = D_8014A250[i].slot;
        }
    }
    if (num_left != 0) {
        for (i = 0; i < num_left; i++) {
            dist_tab[place[i]] = D_80152818[place[i]].distance;
        }
        for (i = 0; i < num_left; i++) {
            high_index = i;
            for (j = i + 1; j < num_left; j++) {
                index = place[j];
                if (dist_tab[index] > dist_tab[place[high_index]]) {
                    high_index = j;
                }
            }
            D_80152818[place[high_index]].place = i + D_80152744 - num_left - num_done;
            place[high_index] ^= place[i];
            place[i] ^= place[high_index];
            place[high_index] ^= place[i];
        }
    }
}
