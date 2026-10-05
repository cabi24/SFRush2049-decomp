/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* NOT A MATCH: complete body, 880 words against 892; opcode-level alignment leaves 85 of 892 unmatched.
 *
 * Tournament setup, no networking: seeds an LCG from the save record (t->seed), picks drone cars without
 * repeats, shuffles drone order and the 12 skill columns, builds the race list (track/mirror/reverse without
 * repeating a track back to back or using one four times), decodes finished-race results (3 bits per race) into
 * finish positions and points, then sorts the six entrants by points. The stub func_800F34D0 directly before
 * this function is taken to be the deleted static random helper (inferred: retail has no jal to any rand here
 * and the inline sequence is rand() followed by `% (max + 1)` with an unsigned divide).
 *
 * Measured: irand() must be two nested static functions (a 2-statement rand and a 1-statement wrapper) for
 * umerge to inline its eight call sites (one function holding both statements is not inlined at any); the fill loops stay un-unrolled only when the bound is the global
 * D_80154640 itself; the column-swap loop unrolls by 2 only when the entrant address is a pointer local.
 * Residual: register allocation regime (see ../RESULTS.md).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

typedef struct {
    /* 0x00 */ u8 pad0[5];
    /* 0x05 */ u8 unk5;
    /* 0x06 */ u8 resume;
    /* 0x07 */ u8 unk7;
    /* 0x08 */ s8 type;
    /* 0x09 */ s8 nraces;
    /* 0x0A */ u8 padA[2];
    /* 0x0C */ u32 seed;
    /* 0x10 */ u8 pad10[4];
    /* 0x14 */ u8 results[6][9];
} Tourney;

typedef struct {
    /* 0x000 */ u8 pad0[0x6F4];
    /* 0x6F4 */ Tourney tourney;
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
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 index;
    /* 0x02 */ u8 pad2[0x46];
    /* 0x48 */ Ref *ref;
} Player76; /* 0x4C */

typedef struct {
    /* 0x00 */ u8 car;
    /* 0x01 */ u8 place;
    /* 0x02 */ u16 points;
    /* 0x04 */ u8 finish[24];
    /* 0x1C */ u8 pts[24];
    /* 0x34 */ s16 skill[12];
} Entrant; /* 0x4C */

typedef struct {
    /* 0x00 */ u8 track;
    /* 0x01 */ u8 mirror;
    /* 0x02 */ u8 reverse;
    /* 0x03 */ u8 unk3;
    /* 0x04 */ u8 unk4;
} Race; /* 5 */

typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ u16 max;
} Range;

extern s32 D_8011176C[];
extern s32 D_80111784[];
extern Range D_80111794[];
extern Range D_801117A4[];
extern s32 D_801117C4[][12];
extern s16 D_8014A108;
extern Player76 D_8014A118[];
extern u8 D_801543D4;
extern Race D_801543D8[];
extern Entrant D_80154450[];
extern s8 D_80154628;
extern s8 D_80154640;
extern u32 D_80154658;
extern u8 D_80154FD0[];
extern u8 D_80155140[];
extern u16 D_80155148[][6];
extern s8 D_80156994;

static u32 trand(void) {
    D_80154658 = D_80154658 * 1103515245 + 12345;
    return (D_80154658 >> 16) & 0x7FFF;
}
static u16 irand(u16 max) {
    return trand() % (max + 1);
}

#define SWAP(a, b) ((a) ^= (b), (b) ^= (a), (a) ^= (b))

void net_session_update(void) {
    s32 order[6];
    s32 i;
    s32 j;
    s32 k;
    s32 bit;
    Tourney *t;
    s32 ntracks;
    s32 ndrones;
    s32 n;
    s32 m;
    s32 cnt;
    u8 *count;
    Entrant *e;
    Player76 *p;
    u16 x;

    p = &D_8014A118[D_801543D4];
    t = &p->ref->car->data->base->tourney;
    D_80154658 = t->seed;
    ntracks = D_80111784[t->type];
    if (D_80156994 == 0 && ntracks >= 5) {
        ntracks = 5;
    }
    if (t->type == 3) {
        D_80154640 = ntracks * 4;
    } else {
        D_80154640 = ntracks * 2;
    }
    if (t->nraces < D_80154640 - 1) {
        D_80154628 = t->nraces;
    } else {
        D_80154628 = D_80154640 - 1;
    }
    for (i = 0; i < ntracks; i++) {
        D_80154FD0[i] = 0;
    }

    ndrones = 6 - D_8014A108;
    for (i = 0; i < ndrones; i++) {
roll:
        D_80154450[D_8014A108 + i].car = irand(54);
        for (j = 0; j < i; j++) {
            if (D_80154450[D_8014A108 + i].car == D_80154450[D_8014A108 + j].car) {
                goto roll;
            }
        }
    }
    for (i = 0; i < ndrones; i++) {
        order[i] = i;
    }
    for (i = 0; i < ndrones; i++) {
        do {
            j = irand(ndrones - 1);
        } while (i == j);
        SWAP(order[i], order[j]);
    }
    for (i = 0; i < ndrones; i++) {
        for (k = 0; k < 12; k++) {
            D_80154450[order[i] + D_8014A108].skill[k] = D_801117C4[i][k];
        }
    }
    for (k = 0; k < 12; k++) {
        do {
            j = irand(11);
        } while (k == j);
        for (n = 0; n < ndrones; n++) {
            e = &D_80154450[order[n]] + D_8014A108;
            SWAP(e->skill[k], e->skill[j]);
        }
    }

    for (i = 0; i <= D_80154628; i++) {
        if (t->type == 3) {
            D_801543D8[i].track = irand(ntracks - 1);
            D_801543D8[i].mirror = irand(1);
            D_801543D8[i].reverse = irand(1);
            if (i != 0) {
                j = 0;
                while ((D_801543D8[i].track == D_801543D8[i - 1].track && j < ntracks) ||
                       D_80154FD0[D_801543D8[i].track] == 4) {
                    D_801543D8[i].track++;
                    if (D_801543D8[i].track >= ntracks) {
                        D_801543D8[i].track = 0;
                    }
                    j++;
                }
                do {
                    for (j = 0; j < i; j++) {
                        if (D_801543D8[i].track == D_801543D8[j].track &&
                            D_801543D8[i].mirror == D_801543D8[j].mirror &&
                            D_801543D8[i].reverse == D_801543D8[j].reverse) {
                            m = ((D_801543D8[i].reverse << 1) | D_801543D8[i].mirror) + 1;
                            m %= 4;
                            D_801543D8[i].mirror = m & 1;
                            D_801543D8[i].reverse = (m >> 1) & 1;
                            break;
                        }
                    }
                } while (j != i);
            }
            count = &D_80154FD0[D_801543D8[i].track];
            (*count)++;
        } else {
            D_801543D8[i].track = i % ntracks;
            D_801543D8[i].mirror = i >= ntracks;
        }
        x = irand(D_80111794[t->type].max);
        D_801543D8[i].unk3 = (x < 4) ? 0 : ((x < 7) ? 1 : ((x < 9) ? 2 : 3));
        D_801543D8[i].unk4 = irand(D_801117A4[t->type].max);
    }

    cnt = 6 - ((D_8014A108 == 1) ? 0 : 2);
    for (i = 0; i < cnt; i++) {
        if (t->resume != 0) {
            D_80154450[i].points = D_80155148[p->index][i];
        } else {
            D_80154450[i].points = 0;
        }
        for (j = 0; j < t->nraces; j++) {
            if (t->resume != 0 && j < D_80155140[p->index]) {
                D_80154450[i].finish[j] = 6;
                D_80154450[i].pts[j] = 0;
            } else {
                D_80154450[i].finish[j] = 0;
                for (k = 0; k < 3; k++) {
                    bit = j * 3 + k;
                    D_80154450[i].finish[j] |= ((t->results[i][bit >> 3] >> (bit & 7)) & 1) << k;
                }
                D_80154450[i].pts[j] = D_8011176C[D_80154450[i].finish[j]];
                D_80154450[i].points += D_80154450[i].pts[j];
            }
        }
        for (; j < D_80154640; j++) {
            D_80154450[i].finish[j] = 6;
            D_80154450[i].pts[j] = 0;
        }
    }
    for (; i < 6; i++) {
        D_80154450[i].points = 0;
        for (j = 0; j < D_80154640; j++) {
            D_80154450[i].finish[j] = 6;
            D_80154450[i].pts[j] = 0;
        }
    }

    for (i = 0; i < 6; i++) {
        order[i] = i;
    }
    for (i = 0; i < 5; i++) {
        for (j = i + 1; j < 6; j++) {
            if (D_80154450[order[i]].points < D_80154450[order[j]].points) {
                SWAP(order[i], order[j]);
            }
        }
    }
    for (i = 0; i < 6; i++) {
        D_80154450[order[i]].place = i;
    }
}
