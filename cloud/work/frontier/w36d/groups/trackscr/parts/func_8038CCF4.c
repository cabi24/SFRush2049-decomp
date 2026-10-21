/* buildRaceTrackMaps (inlined). N64: the GC PS2 packet code is an N64 display list per map: kNumMapSects
 * sections of 4 vertices (8 for the last, which wraps to the first) and four G_TRI2 commands each. */
void func_8038CCF4(void)
{
    s32 saveTrackNum;
    s32 pointIdx;
    s32 vertIdx;
    s32 prevPt;
    s32 currStartPt;
    s32 currEndPt;
    s32 nextPt;
    f32 x0, y0, x1, y1, x2, y2, x3, y3;
    f32 dx0, dy0, len0, dx1, dy1, len1, dx2, dy2, len2;
    f32 u0;
    f32 xoffset, yoffset, zoffset;
    s32 u1, u2;
    s32 trackMBIdx;
    RomObjDef *trackRomObjDef;
    Gfx *dl;
    Vtx_t *vtx;
    s32 vtxSize, dlSize;
    s32 i;
    s32 u3;
    saveTrackNum = D_8014978C;
    for (D_8014978C = 0; D_8014978C <= 5; D_8014978C++) {
        if (!func_800F7644(D_8014978C)) continue;
        {
            func_8038CCEC();
            xoffset = -(D_801407D4[0] + D_801407B4[0]) / 2;
            yoffset = -(D_801407D4[2] + D_801407B4[2]) / 2;
            zoffset = -(D_801407D4[1] + D_801407B4[1]) / 2;
            D_803B7CE4[D_8014978C].scale_to = 5000.0f / ((D_801407B4[0] - D_801407D4[0] > D_801407B4[2] - D_801407D4[2]) ? D_801407B4[0] - D_801407D4[0] : D_801407B4[2] - D_801407D4[2]);
            trackMBIdx = string_copy_format(D_803B7738[D_8014978C], 0, D_80140BDC - 1, 1);
            trackRomObjDef = &D_801161F4[trackMBIdx >> 10].romObjDefs[trackMBIdx & 1023];
            D_803BAB58[D_8014978C] = trackRomObjDef->lod[0];
            vtxSize = D_803B7CD8 * 64 + 64;
            D_803BABE8[D_8014978C] = audio_dma_sync(0, vtxSize + 7);
            D_803BAAE8[D_8014978C].verts = vtx = (Vtx_t *)(((u32)D_803BABE8[D_8014978C] + 7) & ~7);
            dlSize = D_803B7CD8 * 40 + 56;
            D_803BABC8[D_8014978C] = audio_dma_sync(0, dlSize + 7);
            D_803BAAE8[D_8014978C].displayList = (u32 *)(dl = (Gfx *)(((u32)D_803BABC8[D_8014978C] + 7) & ~7));
            gSPTexture(dl++, 0xFFFF, 0xFFFF, 0, 0, 0);
            gDPPipeSync(dl++);
            gDPSetRenderMode(dl++, 0xC8112230, 0);
            gDPSetCombine(dl++, 0xFFFFFF, 0xFFFE7C38);
            for (pointIdx = 0; pointIdx < D_803B7CD8; pointIdx++) {
                f32 sectVerts[8][3];
                if (pointIdx > 0) prevPt = (D_801407F0.numPts * (pointIdx - 1)) / D_803B7CD8; else prevPt = -1;
                currStartPt = D_801407F0.numPts * pointIdx / D_803B7CD8;
                currEndPt = pointIdx < D_803B7CD8 - 1 ? (D_801407F0.numPts * (pointIdx + 1)) / D_803B7CD8 : func_800BA61C(D_80151CE8.loopPt);
                if (pointIdx < D_803B7CD8 - 2) nextPt = (D_801407F0.numPts * (pointIdx + 2)) / D_803B7CD8; else nextPt = -1;
                x1 = xoffset + D_801407F0.pts[currStartPt][0];
                y1 = yoffset + D_801407F0.pts[currStartPt][2];
                x2 = xoffset + D_801407F0.pts[currEndPt][0];
                y2 = yoffset + D_801407F0.pts[currEndPt][2];
                if (prevPt != -1) {
                    x0 = xoffset + D_801407F0.pts[prevPt][0];
                    y0 = yoffset + D_801407F0.pts[prevPt][2];
                } else {
                    x0 = x1 - (x2 - x1);
                    y0 = y1 - (y2 - y1);
                }
                if (nextPt != -1) {
                    x3 = xoffset + D_801407F0.pts[nextPt][0];
                    y3 = yoffset + D_801407F0.pts[nextPt][2];
                } else {
                    x3 = x2 + (x2 - x1);
                    y3 = y2 + (y2 - y1);
                }
                dx0 = x0 - x1; dy0 = y1 - y0;
                dx1 = x1 - x2; dy1 = y2 - y1;
                dx2 = x2 - x3; dy2 = y3 - y2;
                len0 = sqrtf(dx0 * dx0 + dy0 * dy0);
                len1 = sqrtf(dx1 * dx1 + dy1 * dy1);
                len2 = sqrtf(dx2 * dx2 + dy2 * dy2);
                dx0 *= 100.0f; dy0 *= 100.0f; dx1 *= 100.0f; dy1 *= 100.0f; dx2 *= 100.0f; dy2 *= 100.0f;
                dx0 /= len0; dy0 /= len0; dx1 /= len1; dy1 /= len1; dx2 /= len2; dy2 /= len2;
                sectVerts[0][0] = x1 + (dy0 + dy1) / 2;
                sectVerts[0][2] = y1 + (dx0 + dx1) / 2;
                sectVerts[0][1] = zoffset + D_801407F0.pts[currStartPt][1] + 50.0f;
                sectVerts[1][0] = x1 - (dy0 + dy1) / 2;
                sectVerts[1][2] = y1 - (dx0 + dx1) / 2;
                sectVerts[1][1] = zoffset + D_801407F0.pts[currStartPt][1] + 50.0f;
                sectVerts[2][0] = x1 - (dy0 + dy1) / 2;
                sectVerts[2][2] = y1 - (dx0 + dx1) / 2;
                sectVerts[2][1] = D_801407D4[1] + zoffset - 50.0f;
                sectVerts[3][0] = x1 + (dy0 + dy1) / 2;
                sectVerts[3][2] = y1 + (dx0 + dx1) / 2;
                sectVerts[3][1] = D_801407D4[1] + zoffset - 50.0f;
                sectVerts[4][0] = x2 + (dy1 + dy2) / 2;
                sectVerts[4][2] = y2 + (dx1 + dx2) / 2;
                sectVerts[4][1] = zoffset + D_801407F0.pts[currEndPt][1] + 50.0f;
                sectVerts[5][0] = x2 - (dy1 + dy2) / 2;
                sectVerts[5][2] = y2 - (dx1 + dx2) / 2;
                sectVerts[5][1] = zoffset + D_801407F0.pts[currEndPt][1] + 50.0f;
                sectVerts[6][0] = x2 - (dy1 + dy2) / 2;
                sectVerts[6][2] = y2 - (dx1 + dx2) / 2;
                sectVerts[6][1] = D_801407D4[1] + zoffset - 50.0f;
                sectVerts[7][0] = x2 + (dy1 + dy2) / 2;
                sectVerts[7][2] = y2 + (dx1 + dx2) / 2;
                sectVerts[7][1] = D_801407D4[1] + zoffset - 50.0f;
                if (pointIdx == D_803B7CD8 - 1) {
                    sectVerts[0][1] += 0.1f; sectVerts[1][1] += 0.1f;
                    sectVerts[2][1] += 0.1f; sectVerts[3][1] += 0.1f;
                }
                sectVerts[0][0] *= 0.01f; sectVerts[0][1] *= 0.01f; sectVerts[0][2] *= 0.01f;
                sectVerts[1][0] *= 0.01f; sectVerts[1][1] *= 0.01f; sectVerts[1][2] *= 0.01f;
                sectVerts[2][0] *= 0.01f; sectVerts[2][1] *= 0.01f; sectVerts[2][2] *= 0.01f;
                sectVerts[3][0] *= 0.01f; sectVerts[3][1] *= 0.01f; sectVerts[3][2] *= 0.01f;
                sectVerts[4][0] *= 0.01f; sectVerts[4][1] *= 0.01f; sectVerts[4][2] *= 0.01f;
                sectVerts[5][0] *= 0.01f; sectVerts[5][1] *= 0.01f; sectVerts[5][2] *= 0.01f;
                sectVerts[6][0] *= 0.01f; sectVerts[6][1] *= 0.01f; sectVerts[6][2] *= 0.01f;
                sectVerts[7][0] *= 0.01f; sectVerts[7][1] *= 0.01f; sectVerts[7][2] *= 0.01f;
                for (i = 0; i < (pointIdx == D_803B7CD8 - 1 ? 8 : 4); i++) {
                    vtx->ob[0] = sectVerts[i][0] * 16.0f;
                    vtx->ob[1] = sectVerts[i][1] * 16.0f;
                    vtx->ob[2] = sectVerts[i][2] * 16.0f;
                    vtx->flag = 0x8000;
                    vtx->tc[0] = 0;
                    vtx->tc[1] = 0;
                    vtx->cn[0] = D_803B8354.channel[0];
                    vtx->cn[1] = D_803B8354.channel[1];
                    vtx->cn[2] = D_803B8354.channel[2];
                    vtx->cn[3] = D_803B8354.channel[3];
                    vtx++;
                }
                if (pointIdx == D_803B7CD8 - 1) {
                    gSPVertex(dl++, (Vtx_t *)D_803BABE8[D_8014978C] + pointIdx * 4, 4, 0);
                    gSPVertex(dl++, D_803BABE8[D_8014978C], 4, 4);
                } else {
                    gSPVertex(dl++, (Vtx_t *)D_803BABE8[D_8014978C] + pointIdx * 4, 8, 0);
                }
                gSP2Triangles(dl++, 0, 4, 1, 0, 5, 1, 4, 0);
                gSP2Triangles(dl++, 1, 5, 2, 0, 6, 2, 5, 0);
                gSP2Triangles(dl++, 2, 6, 3, 0, 7, 3, 6, 0);
                gSP2Triangles(dl++, 3, 7, 0, 0, 4, 0, 7, 0);
            }
            gSPTexture(dl++, 0xFFFF, 0xFFFF, 0, 0, 1);
            gSPEndDisplayList(dl++);
            D_803BAAE8[D_8014978C].flags = 0;
            D_803BAAE8[D_8014978C].texidx = 0;
            D_803BAAE8[D_8014978C].lodRad = 0.0f;
            trackRomObjDef->lod[0] = D_803BAAE8[D_8014978C];
            if (D_803B7CE4[D_8014978C].handle == -1) {
                D_803B7CE4[D_8014978C].handle = sign_extend_call(trackMBIdx, (u32)D_803B7CE4[D_8014978C].matrix, -1, 0);
                func_8008B32C((f32 (*)[3])D_8011418C, D_803B7CE4[D_8014978C].matrix, 1.0f);
                func_800B5898(-1.5707963705062866f, D_803B7CE4[D_8014978C].matrix);
                D_803B7CE4[D_8014978C].position[0] = D_803B8358[0];
                D_803B7CE4[D_8014978C].position[1] = D_803B8358[1];
                D_803B7CE4[D_8014978C].position[2] = D_803B8358[2];
            }
            func_8038CCE4();
        }
    }
    D_8014978C = saveTrackNum;
    trackMBIdx = string_copy_format(D_803B7738[19], 0, D_80140BDC - 1, 1);
    trackRomObjDef = &D_801161F4[trackMBIdx >> 10].romObjDefs[trackMBIdx & 1023];
    D_803BAB58[6] = trackRomObjDef->lod[0];
    D_803BABE8[6] = audio_dma_sync(0, D_803B7CD8 * 64 + 64 + 7);
    D_803BAAE8[6].verts = (void *)(((u32)D_803BABE8[6] + 7) & ~7);
    D_803BABC8[6] = audio_dma_sync(0, D_803B7CD8 * 40 + 56 + 7);
    D_803BAAE8[6].displayList = (u32 *)(((u32)D_803BABC8[6] + 7) & ~7);
    D_803BAAE8[6].flags = 0;
    D_803BAAE8[6].texidx = 0;
    D_803BAAE8[6].lodRad = 0.0f;
    trackRomObjDef->lod[0] = D_803BAAE8[6];
    if (D_803B7CE4[19].handle == -1) {
        D_803B7CE4[19].handle = sign_extend_call(trackMBIdx, (u32)D_803B7CE4[19].matrix, -1, 0);
        func_8008B32C((f32 (*)[3])D_8011418C, D_803B7CE4[19].matrix, 1.0f);
        func_800B5898(-1.5707963705062866f, D_803B7CE4[19].matrix);
        D_803B7CE4[19].position[0] = D_803B8364[0];
        D_803B7CE4[19].position[1] = D_803B8364[1];
        D_803B7CE4[19].position[2] = D_803B8364[2];
    }
}
