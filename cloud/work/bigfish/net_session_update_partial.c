typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32;
typedef struct { u8 pad[8]; s8 mode; s8 lim; u8 pad2[2]; u32 seed; } Info;
typedef struct { u8 pad[0xC]; Info info; } Sess0;   /* placeholder; real base is Sess+0x6F4 */
typedef struct { u8 pad[0x6F4]; Info info; } Sess;
typedef struct { u8 pad[0x2C]; Sess **s; } A;
typedef struct { u8 id; u8 pad[0x2B]; s16 h[16]; } Rec;
typedef struct { u8 pad[0x48]; A **a; } InRec;
extern InRec input_rec0[];
extern Rec D_80154450[];
extern u8 D_801543D4, D_80156994;
extern s16 active_player_count;
extern s32 D_80111784[];
extern s32 D_801117C4[][12];
extern u32 D_80154658;
extern s8 D_80154640, D_80154628;
extern u8 D_80154FD0[];
#define RAND() (D_80154658 = D_80154658 * 1103515245u + 12345u, (D_80154658 >> 16) & 0x7FFF)
void net_session_update(void) {
    InRec *p = &input_rec0[D_801543D4];
    Sess *v = *((*p->a)->s);
    Info *info = &v->info;
    s32 n, i, k, j, r, cnt, idx[6];
    D_80154658 = info->seed;
    n = D_80111784[info->mode];
    if (D_80156994 == 0 && n >= 5) n = 5;
    D_80154640 = (info->mode == 3) ? n * 4 : n * 2;
    D_80154628 = (info->lim < D_80154640 - 1) ? info->lim : D_80154640 - 1;
    for (i = 0; i < n; i++) D_80154FD0[i] = 0;
    cnt = 6 - active_player_count;
    for (k = 0; k < cnt; k++) {
        do {
            r = RAND() % 55;
            D_80154450[active_player_count + k].id = r;
            for (j = 0; j < k; j++) if (D_80154450[active_player_count + j].id == (u8)r) break;
        } while (j < k);
    }
    for (k = 0; k < cnt; k++) idx[k] = k;
    for (k = 0; k < cnt; k++) {
        j = RAND() % cnt;
        if (k != j) { idx[k] ^= idx[j]; idx[j] ^= idx[k]; idx[k] ^= idx[j]; }
    }
    for (k = 0; k < cnt; k++)
        for (i = 0; i < 12; i++) D_80154450[active_player_count + idx[k]].h[i] = D_801117C4[k][i];
    for (i = 0; i < 12; i++) {
        do { r = RAND() % 12; } while (r == i);
        for (k = 0; k < cnt; k++) {
            s16 *q = D_80154450[active_player_count + idx[k]].h + 4;
            q[i] ^= q[r]; q[r] ^= q[i]; q[i] ^= q[r];
        }
    }
}
