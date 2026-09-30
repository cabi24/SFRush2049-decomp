typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32;
typedef struct { u8 pad[6]; u8 flag6; u8 pad1; s8 mode; s8 lim; u8 pad2[2]; u32 seed; u8 pad3[4]; u8 bits[6 * 9]; } Info;
typedef struct { u8 pad[0x6F4]; Info info; } Sess;
typedef struct { u8 pad[0x2C]; Sess **s; } A;
typedef struct { u8 id; u8 b1; u16 h2; u8 b4[24]; u8 b28[16]; s16 h[16]; } Rec;
typedef struct { u8 a, b, c, d, e; } Slot;
typedef struct { u16 hi, lo; } Pair;
typedef struct { u8 pad0; u8 b1; u8 pad[0x46]; A **a; } InRec;
extern InRec input_rec0[];
extern Rec D_80154450[];
extern Slot D_801543D8[];
extern u8 D_801543D4, D_80156994;
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
extern u16 D_80155148[][38];
#define RAND() (D_80154658 = D_80154658 * 1103515245u + 12345u, (D_80154658 >> 16) & 0x7FFF)
#define RR(max) ((u16)(RAND() % ((u16)(max) + 1)))
void net_session_update(void) {
    InRec *p = &input_rec0[D_801543D4];
    Sess *v = *((*p->a)->s);
    Info *info = &v->info;
    s32 n, i, k, j, r, cnt, cap, idx[6];
    D_80154658 = info->seed;
    n = D_80111784[info->mode];
    if (D_80156994 == 0 && n >= 5) n = 5;
    D_80154640 = (info->mode == 3) ? n * 4 : n * 2;
    D_80154628 = (info->lim < D_80154640 - 1) ? info->lim : D_80154640 - 1;
    for (i = 0; i < n; i++) D_80154FD0[i] = 0;
    cnt = 6 - active_player_count;
    for (k = 0; k < cnt; k++) {
        do {
            r = RR(54);
            D_80154450[active_player_count + k].id = r;
            for (j = 0; j < k; j++) if (D_80154450[active_player_count + j].id == (u8)r) break;
        } while (j < k);
    }
    for (k = 0; k < cnt; k++) idx[k] = k;
    for (k = 0; k < cnt; k++) {
        do { j = RR(cnt - 1); } while (k == j);
        idx[k] ^= idx[j]; idx[j] ^= idx[k]; idx[k] ^= idx[j];
    }
    for (k = 0; k < cnt; k++)
        for (i = 0; i < 12; i++) D_80154450[active_player_count + idx[k]].h[i] = D_801117C4[k][i];
    for (i = 0; i < 12; i++) {
        do { r = RR(11); } while (r == i);
        for (k = 0; k < cnt; k++) {
            D_80154450[active_player_count + idx[k]].h[4 + i] ^= D_80154450[active_player_count + idx[k]].h[4 + r];
            D_80154450[active_player_count + idx[k]].h[4 + r] ^= D_80154450[active_player_count + idx[k]].h[4 + i];
            D_80154450[active_player_count + idx[k]].h[4 + i] ^= D_80154450[active_player_count + idx[k]].h[4 + r];
        }
    }
    /* phase 5: slot table */
    for (i = 0; i <= D_80154628; i++) {
        if (info->mode == 3) {
            D_801543D8[i].a = RR(n - 1);
            D_801543D8[i].b = RAND() & 1;
            D_801543D8[i].c = RR(1);
            if (i > 0) {
                j = 0;
                while ((D_801543D8[i].a == D_801543D8[i - 1].a && j < n) || D_80154FD0[D_801543D8[i].a] == 4) {
                    D_801543D8[i].a++;
                    if (D_801543D8[i].a >= n) D_801543D8[i].a = 0;
                    j++;
                }
                for (;;) {
                    for (j = 0; j < i; j++) {
                        if (D_801543D8[i].a == D_801543D8[j].a && D_801543D8[i].b == D_801543D8[j].b && D_801543D8[i].c == D_801543D8[j].c) {
                            r = ((D_801543D8[i].c << 1) | D_801543D8[i].b) + 1;
                            r = r % 4;
                            D_801543D8[i].b = r & 1;
                            D_801543D8[i].c = (r >> 1) & 1;
                            break;
                        }
                    }
                    if (j == i) break;
                }
            }
            D_80154FD0[D_801543D8[i].a]++;
        } else {
            D_801543D8[i].a = i % n;
            D_801543D8[i].b = i >= n;
        }
        r = RR(D_80111794[info->mode].lo);
        D_801543D8[i].d = r < 4 ? 0 : r < 7 ? 1 : r < 9 ? 2 : 3;
        D_801543D8[i].e = RR(D_801117A4[info->mode].lo);
    }
    cnt = 6 - (active_player_count == 1 ? 0 : 2);
    cap = D_80154640;
    for (i = 0; i < cnt; i++) {
        Rec *rp = &D_80154450[i];
        rp->h2 = info->flag6 ? D_80155148[p->b1][i] : 0;
        for (j = 0; j < info->lim; j++) {
            Rec *r0 = &D_80154450[i];
            if (info->flag6 && j < D_80155140[p->b1]) {
                r0->b4[j] = 6;
                r0->b28[j] = 0;
            } else {
                r0->b4[j] = 0;
                for (r = 0, k = j * 3; r < 3; r++, k++)
                    r0->b4[j] |= ((info->bits[i * 9 + (k >> 3)] >> (k & 7)) & 1) << r;
                rp->b28[j] = D_8011176C[rp->b4[j]];
                rp->h2 += rp->b28[j];
            }
        }
        for (; j < cap; j++) {
            rp->b4[j] = 6;
            rp->b28[j] = 0;
        }
    }
    for (; i < 6; i++) {
        D_80154450[i].h2 = 0;
        for (j = 0; j < cap; j++) {
            D_80154450[i].b4[j] = 6;
            D_80154450[i].b28[j] = 0;
        }
    }
    for (i = 0; i < 6; i++) idx[i] = i;
    for (i = 0; i < 5; i++)
        for (j = i + 1; j < 6; j++)
            if (D_80154450[idx[i]].h2 < D_80154450[idx[j]].h2) {
                idx[i] ^= idx[j]; idx[j] ^= idx[i]; idx[i] ^= idx[j];
            }
    D_80154450[idx[0]].b1 = 0;
    D_80154450[idx[1]].b1 = 1;
    for (i = 2; i < 6; i++) D_80154450[idx[i]].b1 = i;
}
