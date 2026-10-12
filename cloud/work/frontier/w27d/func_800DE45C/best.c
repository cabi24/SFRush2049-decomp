/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800DE45C (0x800DE45C, 1020 B) -- DRAFT, logic complete. Settings/race reset: four player
 * records D_8014A118[i] (node = i, unk1 = 5, unk42 = 0, unk44 = -1), drone_target_update(1), the 41
 * option bytes D_80146108[i] = func_800CCB40(i), two speed_set calls (IPA: blend f20, amount f22,
 * first s1, second s2; amount literal 0.05f, two rodata words 0x80124308/0x8012430C), then the
 * option bytes are copied to their globals and ~25 flags cleared.
 * Unit: 220/255 words, ops 80 (v2 form with D_80146108[120..123]: ops 72).
 * Residual: (1) the player block: retail is a FULLY UNROLLED form (per store `lui at` + fresh `li`
 * from the t6-t9 ring, no base or constant CSE, as if uopt unrolled a 4-iteration loop after PRE);
 * explicit statements CSE the base/constants, loops (s32/s16/s8/u8 index, do-while, !=, volatile
 * array) are not unrolled. (2) the 41-entry loop keeps `slti at,s0,41` (ours: `li s2,41; bne`).
 * (3) retail ends with `li v0,1` for the two `= 1` stores (a return value, or a v0 tie).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef float f32;

typedef struct {
    u8 node;
    s8 unk1;
    u8 pad2[0x42 - 2];
    s8 unk42;
    u8 pad43;
    s32 unk44;
    u8 pad48[4];
} Player;

typedef struct {
    u8 pad0[6];
    s16 unk06;
    u8 pad8[0x1A - 8];
    s16 unk1A;
    s16 unk1C;
    s16 unk1E;
    s16 unk20;
} Rec164A4;

extern Player D_8014A118[4];
extern s8 D_80146108[];
extern s8 D_80146180[4];
extern Rec164A4 D_801164A4;
extern s16 D_80151AD0;
extern s16 D_8014A108;
extern s8 D_8013F1D8;
extern s8 D_8013F1D9;
extern s8 D_8013F2FC;
extern s16 D_8013FEC8;
extern s8 D_8013FECC;
extern s8 D_8013FECD;
extern s8 D_801403D0;
extern s8 D_80140418;
extern s8 D_8014061A;
extern s8 D_801407B0;
extern s32 D_801407BC;
extern s8 D_801407D0;
extern s32 D_801407DC;
extern s32 D_80140804;
extern s32 D_80140A00;
extern s8 D_80140A04;
extern s32 D_80140AD8;
extern s32 D_80140B08;
extern s32 D_80140BD8;
extern s32 D_80142510;
extern s8 D_801426EC;
extern s16 D_80142724;
extern s8 D_80142726;
extern s8 D_80142760;
extern s8 D_801427A0;
extern s8 D_801427A1;
extern s8 D_80142B00;
extern s8 D_80142DB0;
extern s8 D_80143F1C;
extern s8 D_80150EFC;
extern s8 D_80151AD8;
extern s8 D_80152030;
extern s8 D_80152570;
extern s16 D_80152734;
extern s8 D_801543C8;
extern s8 D_80156BDC;
extern s8 D_80156CE8;
extern s8 D_8015723C;
extern s8 D_8015B24C;
extern s8 D_8015B25C;
extern s8 D_8015F72C;
extern s8 D_8015F734;
extern s8 D_8016137C;
extern s8 D_80161394;
extern s8 D_801613A0;
extern s8 D_801613A8;
extern s8 D_801613A9;
extern s8 D_801613AA;
extern s8 D_801613AB;
extern s8 D_8017A4D8;
extern s8 D_8017A634;

void drone_target_update(s32 arg0);
s8 func_800CCB40(s32 i);
void speed_set(f32 blend, f32 amount, s32 first, s32 second);
void func_800D6E00(s8 arg0);

void func_800DE45C(void) {
    s32 i;

    D_8014A118[0].unk1 = 5;
    D_8014A118[0].unk42 = 0;
    D_8014A118[0].unk44 = -1;
    D_8014A118[0].node = 0;
    D_8014A118[1].unk1 = 5;
    D_8014A118[1].unk42 = 0;
    D_8014A118[1].unk44 = -1;
    D_8014A118[1].node = 1;
    D_8014A118[2].unk1 = 5;
    D_8014A118[2].unk42 = 0;
    D_8014A118[2].unk44 = -1;
    D_8014A118[2].node = 2;
    D_8014A118[3].unk1 = 5;
    D_8014A118[3].unk42 = 0;
    D_8014A118[3].unk44 = -1;
    D_8014A118[3].node = 3;
    drone_target_update(1);
    for (i = 0; i < 41; i++) {
        D_80146108[i] = func_800CCB40(i);
    }
    speed_set(D_80146108[13] / 10.0f, 0.05f, 0, 1);
    speed_set(D_80146108[12] / 10.0f, 0.05f, 1, 0);
    func_800D6E00(D_80146108[15]);
    D_80152734 = D_80146108[21];
    D_80142724 = D_80146108[22];
    D_80156BDC = D_80146108[0];
    D_801543C8 = D_80146108[23];
    D_80156CE8 = D_80146108[1];
    D_8015723C = D_80146108[2];
    D_8015B24C = D_80146108[3];
    D_8015B25C = D_80146108[4];
    D_8015F72C = D_80146108[5];
    D_8015F734 = D_80146108[6];
    D_8016137C = D_80146108[7];
    D_80161394 = D_80146108[8];
    D_80152030 = D_80146108[24];
    D_801426EC = D_80146108[25];
    D_80150EFC = D_80146108[26];
    D_80152570 = D_80146108[27];
    D_80140A04 = D_80146108[28] > 0;
    D_8013F1D9 = D_80146108[28] >= 2;
    D_8013F1D8 = D_80146108[29];
    D_80142726 = D_80146108[30];
    D_801613AA = D_80146108[9];
    D_80142760 = D_80146108[31];
    D_801613A8 = D_80146108[10];
    D_801613A9 = D_80146108[11];
    D_8013FEC8 = D_80146108[16];
    D_801613A0 = D_80146108[18];
    D_801407BC = D_80146108[32];
    D_801407DC = D_80146108[33];
    D_80140804 = D_80146108[35];
    D_80140A00 = D_80146108[34];
    D_80140AD8 = D_80146108[36];
    D_80140B08 = D_80146108[37];
    D_80140BD8 = D_80146108[39];
    D_80142510 = D_80146108[38];
    D_8017A4D8 = 0;
    D_801427A0 = 0;
    D_8013FECC = 0;
    D_8013FECD = 0;
    D_801164A4.unk06 = 0;
    D_80151AD8 = 0;
    D_801403D0 = 0;
    D_80142B00 = 0;
    D_80140418 = 0;
    D_8014061A = 0;
    D_8013F2FC = 0;
    D_80142DB0 = 0;
    D_80143F1C = 0;
    D_8017A634 = 0;
    D_801164A4.unk1A = 0;
    D_801164A4.unk1C = 0;
    D_801164A4.unk1E = 0;
    D_801164A4.unk20 = 0;
    D_801427A1 = 0;
    D_801407B0 = 0;
    D_801613AB = 0;
    D_801407D0 = 0;
    D_80146180[1] = 0;
    D_80146180[2] = 0;
    D_80146180[3] = 0;
    D_80146180[0] = 0;
    D_80151AD0 = 1;
    D_8014A108 = 1;
}
