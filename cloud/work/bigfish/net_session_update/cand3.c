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
extern u16 D_80155148[][38];
#define RAND() (D_80154658 = D_80154658 * 1103515245u + 12345u, (D_80154658 >> 16) & 0x7FFF)
#define RR(max) (r = RAND(), r = (u16)((u32)r % ((u16)(max) + 1)))
void net_session_update(void) {
    s32 n, i, k, j, r, cnt;
    s32 idx[6];
    s32 cap;
    s32 x;
    Sess *v;
    Info *info;
    u8 pad[124];
    v = *((*input_rec0[D_801543D4].a)->s);
    info = &v->info;
    D_80154658 = info->seed;
    n = D_80111784[info->mode];
    if (D_80156994 == 0 && n >= 5) n = 5;
    D_80154640 = (info->mode == 3) ? n * 4 : n * 2;
    D_80154628 = (info->lim < D_80154640 - 1) ? info->lim : D_80154640 - 1;
    for (i = 0; i < n; i++) D_80154FD0[i] = 0;
    cnt = 6 - active_player_count;
    for (x = 0; x < cnt; x++) {
    again:
        D_80154450[active_player_count + x].id = RR(54);
        for (i = 0; i < x; i++) if (D_80154450[active_player_count + i].id == (u8)r) goto again;
    }
    for (x = 0; x < cnt; x++) idx[x] = x;
    for (k = 0; k < cnt; k++) {
        do { j = RR(cnt - 1); } while (k == j);
        idx[k] ^= idx[j]; idx[j] ^= idx[k]; idx[k] ^= idx[j];
    }
    for (i = 0; i < cnt; i++)
        for (x = 0; x < 12; x++) D_80154450[active_player_count + idx[i]].h[x] = D_801117C4[i][x];
    for (j = 0; j < 12; j++) {
        do { k = RR(11); } while (k == j);
        for (i = 0; i < cnt; i++) {
            D_80154450[active_player_count + idx[i]].h[4 + j] ^= D_80154450[active_player_count + idx[i]].h[4 + k];
            D_80154450[active_player_count + idx[i]].h[4 + k] ^= D_80154450[active_player_count + idx[i]].h[4 + j];
            D_80154450[active_player_count + idx[i]].h[4 + j] ^= D_80154450[active_player_count + idx[i]].h[4 + k];
        }
    }
    for (j = 0; j <= D_80154628; j++) {
        if (info->mode == 3) {
            D_801543D8[j].a = RR(n - 1);
            D_801543D8[j].b = RAND() & 1;
            D_801543D8[j].c = RR(1);
            if (j > 0) {
                x = 0;
                while ((D_801543D8[j].a == D_801543D8[j - 1].a && x < n) || D_80154FD0[D_801543D8[j].a] == 4) {
                    D_801543D8[j].a++;
                    if (D_801543D8[j].a >= n) D_801543D8[j].a = 0;
                    x++;
                }
                for (;;) {
                    for (x = 0; x < j; x++) {
                        if (D_801543D8[j].a == D_801543D8[x].a && D_801543D8[j].b == D_801543D8[x].b && D_801543D8[j].c == D_801543D8[x].c) {
                            r = ((D_801543D8[j].c << 1) | D_801543D8[j].b) + 1;
                            r = r % 4;
                            D_801543D8[j].b = r & 1;
                            D_801543D8[j].c = (r >> 1) & 1;
                            break;
                        }
                    }
                    if (x == j) break;
                }
            }
            D_80154FD0[D_801543D8[j].a]++;
        } else {
            D_801543D8[j].a = j % n;
            D_801543D8[j].b = j >= n;
        }
        r = RR(D_80111794[info->mode].lo);
        D_801543D8[j].d = r < 4 ? 0 : r < 7 ? 1 : r < 9 ? 2 : 3;
        D_801543D8[j].e = RR(D_801117A4[info->mode].lo);
    }
    cnt = 6 - (active_player_count == 1 ? 0 : 2);
    cap = D_80154640;
    for (i = 0; i < cnt; i++) {
        D_80154450[i].h2 = info->flag6 ? D_80155148[input_rec0[D_801543D4].b1][i] : 0;
        for (j = 0; j < info->lim; j++) {
            if (info->flag6 && j < D_80155140[input_rec0[D_801543D4].b1]) {
                D_80154450[i].b4[j] = 6;
                D_80154450[i].b28[j] = 0;
            } else {
                D_80154450[i].b4[j] = 0;
                for (r = 0, k = j * 3; r < 3; r++, k++)
                    D_80154450[i].b4[j] |= ((info->bits[i * 9 + (k >> 3)] >> (k & 7)) & 1) << r;
                D_80154450[i].b28[j] = D_8011176C[D_80154450[i].b4[j]];
                D_80154450[i].h2 += D_80154450[i].b28[j];
            }
        }
        for (; j < cap; j++) {
            D_80154450[i].b4[j] = 6;
            D_80154450[i].b28[j] = 0;
        }
    }
    for (; i < 6; i++) {
        D_80154450[i].h2 = 0;
        for (k = 0; k < cap; k++) {
            D_80154450[i].b4[k] = 6;
            D_80154450[i].b28[k] = 0;
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
