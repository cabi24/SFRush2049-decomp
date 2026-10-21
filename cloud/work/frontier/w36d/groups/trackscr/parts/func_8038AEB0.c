/* w36e body (cloud/work/frontier/w36e/groups/best_ts), merged 2026-10-21 */
/* w36e final body (cloud/work/frontier/w36e/groups/best_ts) */
void func_8038AEB0(void)
{
    Gfx *gfx;
    Vtx *vtx;
    s32 i, vertex;
    u32 vtxSize, dlSize;
    s32 trackMBIdx;
    func_8038AEA8();
    if (D_803BAC90 >= 0 && D_803BAC90 <= 5) {
        if (D_803B7CE4[19].handle != -1) {
            vtxSize = (D_803B7CD8 + 1) * 4 * sizeof(Vtx);
            memcpy(D_803BABE8[6], D_803BABE8[D_803BAC90], vtxSize);
            dlSize = (D_803B7CD8 * 5 + 7) * sizeof(Gfx);
            memcpy(D_803BABC8[6], D_803BABC8[D_803BAC90], dlSize);
            gfx = D_803BAB50;
            if ((D_8014A110 == 3 && D_801543D8[D_80154628].mirror == 1)
                || (D_8014A110 != 3 && D_80146108[28])) {
                gSPTexture(gfx++, 0xFFFF, 0xFFFF, 0, 0, 0);
                gDPPipeSync(gfx++);
                gDPSetRenderMode(gfx++, 0xC8112230, 0);
                gDPSetCombine(gfx++, 0xFFFFFF, 0xFFFE7C38);
                for (i = 0, vertex = 0; i < D_803B7CD8; i++, vertex += 4) {
                    if (i + 1 == D_803B7CD8) {
                        if (vertex) {}
                        gSPVertex(gfx++, &((Vtx *)D_803BABE8[6])[vertex], 4, 0);
                        gSPVertex(gfx++, &((Vtx *)D_803BABE8[6])[0], 4, 4);
                    } else {
                        gSPVertex(gfx++, &((Vtx *)D_803BABE8[6])[vertex], 8, 0);
                    }
                    gSP2Triangles(gfx++, 1, 5, 0, 0, 4, 0, 5, 0);
                    gSP2Triangles(gfx++, 0, 4, 3, 0, 7, 3, 4, 0);
                    gSP2Triangles(gfx++, 3, 7, 2, 0, 6, 2, 7, 0);
                    gSP2Triangles(gfx++, 2, 6, 1, 0, 5, 1, 6, 0);
                }
                gSPTexture(gfx++, 0xFFFF, 0xFFFF, 0, 0, 1);
                gSPEndDisplayList(gfx++);
            }
            func_8008B32C((f32 (*)[3])D_8011418C, (f32 (*)[3])D_803B8254, 2.0f);
            func_800B5898(-0.785398185f, (f32 (*)[3])D_803B8254);
            D_803B7CE4[19].position[0] = D_803B832C[0];
            D_803B7CE4[19].position[1] = D_803B832C[1];
            D_803B7CE4[19].position[2] = D_803B832C[2];
        }
    } else if ((D_803BAC90 >= 6 && D_803BAC90 <= 13) || (D_803BAC90 >= 14 && D_803BAC90 <= 17)) {
        if (D_803B8284 == -1) {
            D_803B8284 = sign_extend_call(string_copy_format(D_803B7738[D_803BAC90], 0, D_80140BDC - 1, 1), (u32)D_803BAC30, -1, 0);
            func_8008B32C((f32 (*)[3])D_8011418C, (f32 (*)[3])D_803BAC30, 2.0f);
            func_800B5898(-0.785398185f, (f32 (*)[3])D_803BAC30);
            D_803BAC30[9] = D_803B832C[0];
            D_803BAC30[10] = D_803B832C[1];
            D_803BAC30[11] = D_803B832C[2];
        }
    }
    D_803BAC78 = D_803BAC90;
}
