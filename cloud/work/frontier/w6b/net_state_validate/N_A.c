/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* NOT A MATCH: 27/682 words differ (14 aligned rows; mnemonics differ in 3 rows).
 *
 * Unlock-table refresh, no networking: for each of D_8014A108 players it fills five s8 tables from the save
 * record reached through D_8014A118[i].ref->car->data->base (medal bit counts per track/cup, summed track
 * values / 10 against thresholds, summed cup values and stunt counts against thresholds, three mode records),
 * with three cheat words (D_801164C0/C2/C4 == 1) that set everything. No arcade ancestor found.
 *
 * Shaping that the code depends on (each one measured, see ../RESULTS.md):
 *  - `if (data == 0) continue;` rather than `if (data != 0) { ... }`: gives uopt the empty block where it places the
 *    reload of D_8014A108 after the calls (retail reloads only on the call path).
 *  - the two call loops compare with != (li at,576; bne) and use two pointer locals.
 *  - one accumulator per section (sum, total, total2), `data` shared by sections 2 and 3 only, separate loop
 *    variables k and q for the 4-trip and 8-trip loops (a loop whose variable is reused by a later loop is not
 *    unrolled), cup2/stunt pointers assigned before both loops.
 * Residual: the row pointer of the two 19-wide loops is v1 (retail a1); in the last loop `flag = D_80156994` is
 * hoisted out of the loop here, retail reloads it per iteration after the data test (data in a3, flag in a1).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

typedef struct {
    /* 0x00 */ u8 pad0[0x58];
    /* 0x58 */ s32 unk58;
    /* 0x5C */ u16 unk5C;
    /* 0x5E */ u16 unk5E;
} TrackRec; /* 0x60 */

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ u8 pad10[0x2C];
    /* 0x3C */ u16 unk3C;
    /* 0x3E */ u16 unk3E;
} CupRec; /* 0x40 */

typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ u16 unk8;
    /* 0x0A */ u16 unkA;
} StuntRec; /* 0x0C */

typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ u16 unk2;
    /* 0x04 */ u16 unk4;
    /* 0x06 */ u16 unk6;
    /* 0x08 */ u8 pad8[0x14];
} ModeRec; /* 0x1C */

typedef struct {
    /* 0x000 */ u8 pad0[0x8C];
    /* 0x08C */ TrackRec tracks[12];
    /* 0x50C */ CupRec cups[4];
    /* 0x60C */ StuntRec stunts[8];
    /* 0x66C */ ModeRec modes[4];
} SaveBase;

typedef struct {
    /* 0x00 */ SaveBase *base;
} Data;

typedef struct {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ Data *data;
} Car;

typedef struct {
    /* 0x00 */ Car *car;
} Ref;

typedef struct {
    /* 0x00 */ u8 pad0[0x48];
    /* 0x48 */ Ref *ref;
} Player76; /* 0x4C */

extern s16 D_801164C0;
extern s16 D_801164C2;
extern s16 D_801164C4;
extern s16 D_8014A108;
extern Player76 D_8014A118[];
extern s8 D_80150DD8[][19];
extern s8 D_80150E30[][13];
extern s8 D_80150E88[][4];
extern s8 D_80150EB8[][8];
extern s8 D_80150ED8[][9];
extern s8 D_80150F00[][5];
extern s8 D_80150F40[][6];
extern s8 D_80150F7C[];
extern s8 D_80156994;

extern s32 func_800B78A4(u32 x, u8 n);

void net_state_validate(void) {
    s16 i;
    s16 j;
    s32 k;
    s32 n0;
    s32 n1;
    s32 n2;
    s32 n3;
    Player76 *p;
    TrackRec *track;
    CupRec *cup;
    s32 sum;
    Data *data;
    s32 total;
    s32 total2;
    CupRec *cup2;
    StuntRec *stunt;
    s32 q;
    Data *data4;
    SaveBase *base;
    s8 flag;

    if (D_801164C0 == 1) {
        for (i = 0; i < D_8014A108; i++) {
            for (j = 0; j < 13; j++) {
                D_80150E30[i][j] = 1;
            }
        }
    } else {
        for (i = 0; i < D_8014A108; i++) {
            p = &D_8014A118[i];
            n0 = 0;
            n1 = 0;
            n2 = 0;
            n3 = 0;
            if (p->ref->car->data == 0) {
                continue;
            }
            for (j = 0; j < 13; j++) {
                if (j < 6) {
                    D_80150E30[i][j] = 1;
                } else {
                    D_80150E30[i][j] = 0;
                }
            }
            for (k = 0; k != 6; k++) {
                track = &p->ref->car->data->base->tracks[k];
                n0 += func_800B78A4(track->unk5C & 0xFF, 16);
                n1 += func_800B78A4(track->unk5C & 0xFF00, 16);
            }
            for (k = 0; k != 4; k++) {
                cup = &p->ref->car->data->base->cups[k];
                n2 += func_800B78A4(cup->unk3C & 0xFF, 16);
                n3 += func_800B78A4(cup->unk3C & 0xFF00, 16);
            }
            if (n0 >= 48) {
                D_80150E30[i][6] = 1;
            }
            if (n1 >= 24) {
                D_80150E30[i][7] = 1;
            }
            if (n1 >= 36) {
                D_80150E30[i][8] = 1;
            }
            if (n2 >= 32) {
                D_80150E30[i][9] = 1;
            }
            if (n3 >= 16) {
                D_80150E30[i][10] = 1;
            }
            if (n3 >= 24) {
                D_80150E30[i][11] = 1;
            }
            if (n0 >= 48 && n1 >= 48 && n2 >= 32 && n3 >= 32) {
                D_80150E30[i][12] = 1;
            }
        }
    }

    if (D_801164C4 == 1) {
        for (i = 0; i < D_8014A108; i++) {
            data = D_8014A118[i].ref->car->data;
            if (data == 0) {
                continue;
            }
            for (j = 0; j < 8; j++) {
                if (j == 6 || j == 7) {
                    D_80150EB8[i][j] = 0;
                } else {
                    D_80150EB8[i][j] = 1;
                }
            }
            for (j = 0; j < 9; j++) {
                D_80150ED8[i][j] = 1;
            }
            for (j = 0; j < 5; j++) {
                D_80150F00[i][j] = 1;
            }
            for (j = 0; j < 6; j++) {
                D_80150F40[i][j] = 1;
            }
        }
    } else {
        for (i = 0; i < D_8014A108; i++) {
            sum = 0;
            data = D_8014A118[i].ref->car->data;
            if (data == 0) {
                continue;
            }
            for (k = 0; k < 12; k++) {
                sum += data->base->tracks[k].unk58;
            }
            sum /= 10;
            D_80150EB8[i][0] = 1;
            D_80150EB8[i][1] = 1;
            D_80150EB8[i][2] = sum >= 200;
            D_80150EB8[i][3] = sum >= 200;
            D_80150EB8[i][4] = sum >= 500;
            D_80150EB8[i][5] = sum >= 500;
            D_80150EB8[i][6] = 0;
            D_80150EB8[i][7] = 0;
            D_80150ED8[i][0] = 1;
            D_80150ED8[i][1] = 1;
            D_80150ED8[i][2] = 1;
            D_80150ED8[i][3] = sum >= 250;
            D_80150ED8[i][4] = sum >= 500;
            D_80150ED8[i][5] = sum >= 800;
            D_80150ED8[i][6] = sum >= 1200;
            D_80150ED8[i][7] = sum >= 1600;
            D_80150ED8[i][8] = sum >= 2000;
            D_80150F00[i][0] = 1;
            D_80150F00[i][1] = sum >= 300;
            D_80150F00[i][2] = sum >= 1200;
            D_80150F00[i][3] = sum >= 100;
            D_80150F00[i][4] = sum >= 600;
            D_80150F40[i][0] = 1;
            D_80150F40[i][1] = 1;
            D_80150F40[i][2] = 1;
            D_80150F40[i][3] = sum >= 150;
            D_80150F40[i][4] = sum >= 400;
            D_80150F40[i][5] = sum >= 700;
        }
    }

    if (D_801164C2 == 1) {
        for (i = 0; i < D_8014A108; i++) {
            for (j = 0; j < 19; j++) {
                D_80150DD8[i][j] = 1;
            }
        }
    } else {
        for (i = 0; i < D_8014A108; i++) {
            total2 = 0;
            total = 0;
            for (j = 0; j < 19; j++) {
                D_80150DD8[i][j] = 0;
            }
            D_80150DD8[i][0] = 1;
            D_80150DD8[i][1] = 1;
            D_80150DD8[i][2] = 1;
            D_80150DD8[i][3] = 1;
            D_80150DD8[i][14] = 1;
            D_80150DD8[i][6] = 1;
            D_80150DD8[i][7] = 1;
            D_80150DD8[i][8] = 1;
            D_80150DD8[i][9] = 1;
            data = D_8014A118[i].ref->car->data;
            if (data == 0) {
                continue;
            }
            cup2 = data->base->cups;
            stunt = data->base->stunts;
            for (k = 0; k < 4; k++) {
                total += cup2[k].unk0C;
            }
            for (q = 0; q < 8; q++) {
                total2 += stunt[q].unk8;
            }
            D_80150DD8[i][15] = total >= 100000;
            D_80150DD8[i][16] = total >= 250000;
            D_80150DD8[i][17] = total >= 500000;
            D_80150DD8[i][18] = total >= 1000000;
            D_80150DD8[i][10] = total2 >= 100;
            D_80150DD8[i][11] = total2 >= 250;
            D_80150DD8[i][12] = total2 >= 500;
            D_80150DD8[i][13] = total2 >= 1000;
        }
    }

    if (D_801164C2 == 1) {
        for (i = 0; i < D_8014A108; i++) {
            for (j = 0; j < 4; j++) {
                D_80150E88[i][j] = 1;
            }
        }
        if (D_80156994 == 0) {
            D_80150E88[i][2] = 0;
        }
    } else {
        for (i = 0; i < D_8014A108; i++) {
            data = D_8014A118[i].ref->car->data;
            if (data == 0) {
                continue;
            }
            base = data->base;
            for (j = 0; j < 4; j++) {
                D_80150E88[i][j] = 0;
            }
            D_80150E88[i][0] = 1;
            if (base->modes[1].unk2 != 0 || base->modes[1].unk4 != 0 || base->modes[1].unk6 != 0 || D_80150F7C[1] != 0) {
                D_80150E88[i][1] = 1;
                D_80150DD8[i][4] = 1;
            }
            if (D_80156994 == 0) {
                if (base->modes[2].unk2 != 0 || base->modes[2].unk4 != 0 || base->modes[2].unk6 != 0 || D_80150F7C[3] != 0) {
                    D_80150E88[i][3] = 1;
                }
            } else {
                if (base->modes[2].unk2 != 0 || base->modes[2].unk4 != 0 || base->modes[2].unk6 != 0 || D_80150F7C[2] != 0) {
                    D_80150E88[i][2] = 1;
                    D_80150DD8[i][5] = 1;
                }
                if (base->modes[3].unk2 != 0 || base->modes[3].unk4 != 0 || base->modes[3].unk6 != 0 || D_80150F7C[3] != 0) {
                    D_80150E88[i][3] = 1;
                }
            }
        }
    }
}
