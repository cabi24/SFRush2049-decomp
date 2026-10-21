/* animTrackMaps stand-in: the GC transform loop and pathPos advance (the N64 vertex-colour fade is omitted). */
/* w36e final body (cloud/work/frontier/w36e/groups/best_ts) */
typedef union MapColor { u8 c[4]; u32 w; } MapColor;
typedef struct MapVtx { s16 ob[3]; u16 flag; s16 tc[2]; u8 cn[4]; } MapVtx;
void func_8038A820(void)
{
    static f32 mapPos[3] = { -100.0f, 30.0f, 200.0f };
    s32 trackIdx;
    s32 pointIdx;
    MapVtx *pStrip;
    s32 fade;
    MapColor col;
    s32 i;
    s32 mapIdx;
    if (D_803BAAE0) {
        for (trackIdx = 0; trackIdx <= 19; trackIdx++) {
            if (D_803B7CE4[trackIdx].handle != -1) {
                if (trackIdx < 19) {
                    func_8008B32C((f32 (*)[3])D_8011418C, D_803B7CE4[trackIdx].matrix, D_803B7CE4[trackIdx].scale_from);
                    func_800B5898(-1.5707963705062866f, D_803B7CE4[trackIdx].matrix);
                    func_800B5940(-D_803B7CE4[trackIdx].angle, D_803B7CE4[trackIdx].matrix);
                    D_803B7CE4[trackIdx].position[0] = mapPos[0];
                    D_803B7CE4[trackIdx].position[1] = mapPos[1];
                    D_803B7CE4[trackIdx].position[2] = mapPos[2];
                    D_803B7CE4[trackIdx].position[0] += D_803B7CE4[trackIdx].offset_x;
                    D_803B7CE4[trackIdx].position[1] += D_803B7CE4[trackIdx].offset_y;
                    if (trackIdx == D_8014978C) {
                        func_8008B32C(D_803B7CE4[trackIdx].matrix, D_803B7CE4[trackIdx].matrix, 0.8f);
                        D_803B7CE4[trackIdx].position[0] = D_803B7CE4[trackIdx].position[0] * 0.8f;
                        D_803B7CE4[trackIdx].position[1] = D_803B7CE4[trackIdx].position[1] * 0.8f;
                        D_803B7CE4[trackIdx].position[2] = D_803B7CE4[trackIdx].position[2] * 0.8f;
                    }
                }
                mapIdx = trackIdx == 19 ? 6 : trackIdx;
                if (trackIdx < 6 || trackIdx == 19) {
                    if (D_803BAAE8[mapIdx].displayList) {
                        for (pointIdx = 0; pointIdx < D_803B7CD8; pointIdx++) {
                            pStrip = (MapVtx *)D_803BAAE8[mapIdx].verts;
                            pStrip += pointIdx * 4;
                            if (D_80146108[27] == 0) fade = D_803B7CE0 * D_803B7CD8 - pointIdx;
                            else fade = D_803B7CE0 * D_803B7CD8 + pointIdx;
                            if (fade < 0) fade += D_803B7CD8;
                            if (fade > D_803B7CD8 - 1) fade -= D_803B7CD8;
                            fade *= 2;
                            if (mapIdx != D_8014978C && mapIdx < 6) {
                                col.c[0] = (mapIdx + 1) & 1 ? 127 : 31;
                                col.c[1] = (mapIdx + 1) & 2 ? 127 : 31;
                                if ((mapIdx + 1) & 4) col.c[2] = 127; else col.c[2] = 31;
                            } else if (mapIdx < 6) {
                                if (fade > 63) fade = 64;
                                if ((D_8014978C + 1) & 1) col.c[0] = 255 - fade; else col.c[0] = 255 - fade * 3;
                                if ((D_8014978C + 1) & 2) col.c[1] = 255 - fade; else col.c[1] = 255 - fade * 3;
                                if ((D_8014978C + 1) & 4) col.c[2] = 255 - fade; else col.c[2] = 255 - fade * 3;
                            } else {
                                if (fade > 63) fade = 64;
                                if ((D_803BAC78 + 1) & 1) col.c[0] = 255 - fade; else col.c[0] = 255 - fade * 3;
                                if ((D_803BAC78 + 1) & 2) col.c[1] = 255 - fade; else col.c[1] = 255 - fade * 3;
                                if ((D_803BAC78 + 1) & 4) col.c[2] = 255 - fade; else col.c[2] = 255 - fade * 3;
                            }
                            if (pointIdx == D_803B7CD8 - 1) col.w = 0xFFFFFFFF;
                            for (i = 0; i < (pointIdx == D_803B7CD8 - 1 ? 8 : 4); i++) {
                                if (i & 2) col.w = 255;
                                pStrip->cn[0] = col.c[2];
                                pStrip->cn[1] = col.c[1];
                                pStrip->cn[2] = col.c[0];
                                pStrip->cn[3] = col.c[3];
                                pStrip++;
                            }
                        }
                    }
                }
            }
        }
        D_803B7CE0 += D_8002EB94 / 5.0f;
        while (D_803B7CE0 > 1.0f) D_803B7CE0 -= 1.0f;
    }
}
