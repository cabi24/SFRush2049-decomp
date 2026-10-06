/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* NOT a match: 892 words, 200 strict-equal, 584 aligned-exact (65%), 837 shape-aligned (94%). See ../net_session_update.md */
typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32;
typedef struct { u8 pad[6]; u8 flag6; u8 pad1; s8 mode; s8 lim; u8 pad2[2]; u32 seed; u8 pad3[4]; u8 bits[6 * 9]; } Info;
typedef struct { u8 pad[0x6F4]; Info info; } Sess;
typedef struct { u8 pad[0x2C]; Sess **s; } A;
typedef struct { u8 id; u8 b1; u16 h2; u8 b4[24]; u8 b28[24]; s16 skill[12]; } Rec;
typedef struct { u8 a, b, c, d, e; } Slot;
typedef struct { u16 hi, lo; } Pair;
typedef struct { u8 pad0; u8 b1; u8 pad[0x46]; A **a; } InRec;
extern InRec input_rec0[];
extern Rec D_80154450[];
extern Slot D_801543D8[];
extern u8 D_801543D4;
extern s8 D_80156994;
extern s16 active_player_count;
extern s32 D_80111784[];
extern s32 D_8011176C[];
extern Pair D_80111794[];
extern Pair D_801117A4[];
extern s32 D_801117C4[][12];
extern u32 D_80154658;
extern s8 D_80154640, D_80154628;
extern u8 D_80154FD0[];
extern u8 D_80155140[];
extern u16 D_80155148[][6];
#define RI(e) D_80154450[(e) + active_player_count]
s32 func_800D50E4(u32 *seed) {
    *seed = *seed * 1103515245 + 12345;
    return (*seed >> 16) & 0x7fff;
}
static u16 irand(u16 max) {
    return (u32)func_800D50E4(&D_80154658) % (max + 1);
}
void net_session_update(void) {
    s32 n, i, k, j, r, cnt;
    s32 idx[6];
    s32 m;
    s32 x;
    Sess *v;
    Info *info;
    u8 pad[8];
    v = *((*input_rec0[D_801543D4].a)->s);
    info = &v->info;
    D_80154658 = info->seed;
    n = D_80111784[info->mode];
    if (D_80156994 == 0 && n >= 5) n = 5;
    if (info->mode == 3) D_80154640 = n * 4;
    else D_80154640 = n * 2;
    D_80154628 = (info->lim < D_80154640 - 1) ? info->lim : D_80154640 - 1;
    for (i = 0; i < n; i++)
        D_80154FD0[i] = 0;
    cnt = 6 - active_player_count;
    for (i = 0; i < cnt; i++) {
    again:
        RI(i).id = irand(54);
        for (j = 0; j < i; j++)
            if (RI(i).id == RI(j).id) goto again;
    }
    for (i = 0; i < cnt; i++)
        idx[i] = i;
    for (i = 0; i < cnt; i++) {
        do { k = irand(cnt - 1); } while (i == k);
        idx[i] ^= idx[k]; idx[k] ^= idx[i]; idx[i] ^= idx[k];
    }
    for (i = 0; i < cnt; i++)
        for (k = 0; k != 12; k++)
            RI(idx[i]).skill[k] = D_801117C4[i][k];
    for (k = 0; k < 12; k++) {
        do { x = irand(11); } while (k == x);
        for (i = 0; i < cnt; i++) {
            RI(idx[i]).skill[k] ^= RI(idx[i]).skill[x];
            RI(idx[i]).skill[x] ^= RI(idx[i]).skill[k];
            RI(idx[i]).skill[k] ^= RI(idx[i]).skill[x];
        }
    }
    for (i = 0; i <= D_80154628; i++) {
        if (info->mode == 3) {
            D_801543D8[i].a = irand(n - 1);
            D_801543D8[i].b = (u16)(func_800D50E4(&D_80154658) & 1);
            D_801543D8[i].c = irand(1);
            if (i != 0) {
                k = 0;
                while ((D_801543D8[i].a == D_801543D8[i - 1].a && k < n) || D_80154FD0[D_801543D8[i].a] == 4) {
                    D_801543D8[i].a++;
                    if (D_801543D8[i].a >= n) D_801543D8[i].a = 0;
                    k++;
                }
                for (;;) {
                    for (k = 0; k < i; k++) {
                        if (D_801543D8[i].a == D_801543D8[k].a && D_801543D8[i].b == D_801543D8[k].b && D_801543D8[i].c == D_801543D8[k].c) {
                            r = (D_801543D8[i].b | (D_801543D8[i].c << 1)) + 1;
                            r = r % 4;
                            D_801543D8[i].b = r & 1;
                            D_801543D8[i].c = (r >> 1) & 1;
                            break;
                        }
                    }
                    if (k == i) break;
                }
            }
            D_80154FD0[D_801543D8[i].a]++;
        } else {
            D_801543D8[i].a = i % n;
            D_801543D8[i].b = i >= n;
        }
        r = irand(D_80111794[info->mode].lo);
        D_801543D8[i].d = r < 4 ? 0 : r < 7 ? 1 : r < 9 ? 2 : 3;
        D_801543D8[i].e = irand(D_801117A4[info->mode].lo);
    }
    m = 6 - (active_player_count == 1 ? 0 : 2);
    for (i = 0; i < m; i++) {
        D_80154450[i].h2 = info->flag6 ? D_80155148[input_rec0[D_801543D4].b1][i] : 0;
        for (j = 0; j < info->lim; j++) {
            if (info->flag6 && j < D_80155140[input_rec0[D_801543D4].b1]) {
                D_80154450[i].b4[j] = 6;
                D_80154450[i].b28[j] = 0;
            } else {
                D_80154450[i].b4[j] = 0;
                for (r = 0, x = j * 3; r < 3; r++, x++)
                    D_80154450[i].b4[j] |= ((info->bits[i * 9 + (x >> 3)] >> (x & 7)) & 1) << r;
                D_80154450[i].b28[j] = D_8011176C[D_80154450[i].b4[j]];
                D_80154450[i].h2 += D_80154450[i].b28[j];
            }
        }
        for (; j < D_80154640; j++) {
            D_80154450[i].b4[j] = 6;
            D_80154450[i].b28[j] = 0;
        }
    }
    for (; i < 6; i++) {
        D_80154450[i].h2 = 0;
        for (j = 0; j < D_80154640; j++) {
            D_80154450[i].b4[j] = 6;
            D_80154450[i].b28[j] = 0;
        }
    }
    for (i = 0; i < 6; i++)
        idx[i] = i;
    for (i = 0; i < 5; i++)
        for (k = i + 1; k < 6; k++)
            if (D_80154450[idx[i]].h2 < D_80154450[idx[k]].h2) {
                idx[i] ^= idx[k]; idx[k] ^= idx[i]; idx[i] ^= idx[k];
            }
    for (i = 0; i < 6; i++)
        D_80154450[idx[i]].b1 = i;
}
