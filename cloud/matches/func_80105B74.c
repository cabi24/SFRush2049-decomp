/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/*
 * func_80105B74: builds the end-of-race results tables. No callers by jal
 * (reached through a pointer); the argument is unused (homed only).
 *   1. Formats four cars' times into the 8-byte digit buffers at D_801439D8
 *      with func_800D2128(time, buf, 'f'): the car's own time (+0xF0) when its
 *      finished flag (+0xEF) is 1, else the global D_801543CC.
 *   2. Clears the six-entry slot table D_80142DB4 to -1.
 *   3. order[place] = slot (model record +0x7C6) of the car whose place byte
 *      (car +0xEE) equals that place.
 *   4. Fills D_80142DB4 from the top down (index num-1-j): first the slot
 *      bytes of the D_80153FD2 0x4C-byte records at D_8014A118, then, in place
 *      order, every slot whose model record has +0x7CC == 1, until full.
 *   5. Sets D_80142D70 = 1 and returns 1.
 * No arcade ancestor identified.
 *
 * Shaping notes:
 *  - D_80152744 (car count) is copied to a local once; D_80153FD2 is re-read
 *    through a hoisted address after every byte store (`lh 0(t2)`), which is
 *    reproduced by declaring it volatile here (QUIRK: matches the bytes, the
 *    original declaration is unknown).
 *  - The first loop uses its own s32 index; the others share s16 counters.
 *  - `D_80142DB4[num - j++ - 1] = idx` (post-increment inside the subscript)
 *    gives retail's store-after-increment order.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef float f32;

typedef struct {
    char pad0[0xEE];
    s8 place;
    s8 finished;
    f32 time;
    char padF4[0x3B8 - 0xF4];
} Car;

typedef struct {
    char pad0[0x7C6];
    s16 slot;
    char pad7C8[4];
    s8 active;
    char pad7CD[0x808 - 0x7CD];
} Model;

typedef struct {
    u8 slot;
    char pad1[0x4B];
} Pad;

extern Car D_80152818[];
extern Model D_8014A250[];
extern Pad D_8014A118[];
extern u8 D_801439D8[6][8];
extern f32 D_801543CC;
extern s8 D_80142DB4[];
extern s8 D_80152744;
extern volatile s16 D_80153FD2;
extern s16 D_80142D70;

s32 func_800D2128(f32 value, u8 *digits, u8 format);

s32 func_80105B74(s32 arg0) {
    s16 order[8];
    s32 k;
    s16 i;
    s16 j;
    s16 idx;
    s16 num;

    for (k = 0; k < 4; k++) {
        func_800D2128((D_80152818[k].finished == 1) ? D_80152818[k].time : D_801543CC, D_801439D8[k], 'f');
    }
    for (i = 0; i < 6; i++) {
        D_80142DB4[i] = -1;
    }
    num = D_80152744;
    for (i = 0; i < num; i++) {
        idx = 0;
        for (j = 0; j < num; j++) {
            idx = D_8014A250[j].slot;
            if (i == D_80152818[idx].place) {
                break;
            }
        }
        order[i] = idx;
    }
    i = 0;
    for (j = 0; j < D_80153FD2; j++) {
        D_80142DB4[num - j - 1] = D_8014A118[j].slot;
    }
    for (; i < num; i++) {
        idx = order[i];
        if (D_8014A250[idx].active == 1) {
            D_80142DB4[num - j++ - 1] = idx;
        }
        if (j == num) {
            break;
        }
    }
    D_80142D70 = 1;
    return 1;
}
