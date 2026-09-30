typedef unsigned char u8; typedef signed char s8; typedef short s16; typedef int s32; typedef float f32;
typedef struct Car { u8 p0[0x7C6]; s16 order; s16 p1; s16 alive; s8 kind; u8 p2[0x19]; s16 target; u8 p3[4]; f32 sx; f32 sy; u8 p4[0x14]; } Car;
typedef struct Rec { u8 p0[8]; f32 pos[3]; u8 p1[0xE2]; s8 place; u8 p2[0x267]; s16 slot; u8 p3[0x60]; } Rec;
extern Car gCars[]; extern Rec gRecs[]; extern s8 gNumCars; extern s32 gFlags; extern f32 gTimer; extern f32 gK1, gK2;
extern void func_800DE860(void);
void render_large_objects(void) {
    s16 rank[6]; s16 sorted[6]; s16 cnt[6]; s16 hum[6]; s16 bot[6];
    f32 d2[6][6]; f32 diff[3];
    s32 i, j, k, nh, nb, n;
    func_800DE860();
    n = gNumCars;
    nh = 0; nb = 0;
    for (i = 0; i < n; i++) {
        rank[gCars[i].order] = gRecs[gCars[i].order].place;
    }
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            k = gCars[j].order;
            if (rank[k] == i) break;
        }
        sorted[i] = k;
        if (gCars[k].kind == 2) { bot[nb++] = k; } else { hum[nh++] = k; }
    }
    for (i = 0; i < nh; i++) {
        gRecs[hum[i]].slot = i;
    }
    if (gFlags & 8) {
        for (i = 0; i < n; i++) {
            if (gCars[gCars[i].order].kind == 1) gCars[gCars[i].order].sx = 1.0f;
        }
    } else if (nh != 0) {
        if (gTimer < 5.0f) {
            for (i = 0; i < nh; i++) {
                gCars[hum[i]].sx = 1.0f;
                gCars[hum[i]].sy = (f32)(s16)(nh - i - 1) * gK2 + gK1;
            }
        } else {
            for (i = 0; i < n; i++) {
                s32 a = gCars[i].order;
                cnt[a] = 0;
                for (j = i; j < n; j++) {
                    s32 b = gCars[j].order;
                    if (a == b) { d2[a][b] = 0.0f; }
                    else {
                        for (k = 0; k < 3; k++) diff[k] = gRecs[a].pos[k] - gRecs[b].pos[k];
                        d2[a][b] = d2[b][a] = diff[0]*diff[0] + diff[1]*diff[1] + diff[2]*diff[2];
                    }
                }
            }
            (void)cnt;
        }
    }
}
