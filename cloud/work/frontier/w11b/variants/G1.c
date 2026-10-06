#define FLOORF(v) (((v) < 0.0f && (f32)(s32)(v) != (v)) ? (v) - 1.0f : (v))

/*
 * input_deadzone_apply (0x800ADD58, 895 words) -- w10c near-miss: 895 words, opcode-identical
 * except 6 load-placement rows (opdiff.py); 365 positional words differ because of (1) frame
 * 408 vs 400 (one extra unused temp pair at the bottom; every named local is at its retail
 * offset + 8) and (2) FP temp choice from the first instruction on (retail start[0] -> $f6,
 * ours $f8; a permutation of f4/f6/f8/f10 that follows through the function).
 * Use with hdr_ipcorder.h + w1g_part_ipcorder.c (input_process_controller declared as
 * (poly, p1, p2, out, vcOut, mat, flag, outIdx, rad2): retail sets up a3, f24, s1, s3, s7, s8, s6,
 * s5; rad2 first or last both reproduce it because as1 hoists the mul).
 * Segment sweep through the quadtree (historical label): cur = start; walk cells from the cell
 * of cur towards the cell of end; in each cell, every polygon (skip type 15 and skipType) is
 * tested with input_process_controller (segment cur->end, radius^2); a hit inside the cell's box
 * [minx,maxx) x [minz,maxz) shortens end to the hit point, mat = basis, best = poly.  After a cell
 * with a hit: optional snap (steering_sensitivity / traction_control + 3x func_8008E0B8), return
 * poly.  Otherwise step to the neighbouring cell where cur->end leaves the box (x faces first,
 * then z faces, corner cases by the sign of the other delta) and continue; 0 at the end cell.
 * Levers that moved it: u8 n / u32 i / list[i] / n = *p++ (camera_trigger_check's lesson; 887 ->
 * 895 words); mid-points as `mx = x0 + x1; if (mx < 0) mx++; ... mx >>= 1;` (retail's +1 then
 * shift, s16); box bounds as four ternaries (two `quad & 1` tests); declarations to the
 * retail layout (m 364, i 360, quad 358, endQuad 356, {t, mx, mz, n} 344, cur 332, d 320, np 308,
 * vc 296, s16 W 294, idx 292, bestIdx 290, list[35] 220, Y 216, minx..maxz 214..208, x 204,
 * z 200, node 196, endNode 192, poly 188, best 184, off 180, p 176).
 * Next: the extra bottom temp and the FP LRU start are probably the same cause (an expression
 * that gets a temp in ours); try the camera_trigger_check-style statement orders in the four
 * face sections and the `t` computation, and check whether retail has an inlined helper.
 */
Poly *input_deadzone_apply(f32 *start, f32 *end, f32 (*mat)[3], f32 radius, s32 snap, s32 skipType) {
    f32 m[3][3];
    u32 i;
    s16 quad;
    s16 endQuad;
    f32 t;
    s16 mx;
    s16 mz;
    u8 n;
    f32 cur[3];
    f32 d[3];
    f32 np[3];
    f32 vc[3];
    s16 W;
    u16 idx;
    u16 bestIdx;
    u16 list[35];
    u32 off;
    s16 minx;
    s16 maxx;
    s16 minz;
    s16 maxz;
    f32 x;
    f32 z;
    QNode *node;
    QNode *endNode;
    Poly *poly;
    Poly *best;
    u8 *p;

    best = NULL;
    cur[0] = start[0];
    cur[2] = start[2];
    cur[1] = start[1];
    x = FLOORF(cur[0]);
    z = FLOORF(cur[2]);
    node = handbrake_apply(D_80124EEC, (s32)x, (s32)z, &quad);
    if (node == NULL) {
        return NULL;
    }
    if (end[0] < 0.0f && (f32)(s32)end[0] != end[0]) {
        x = end[0] - 1.0f;
    } else {
        x = end[0];
    }
    if (end[2] < 0.0f && (f32)(s32)end[2] != end[2]) {
        z = end[2] - 1.0f;
    } else {
        z = end[2];
    }
    endNode = handbrake_apply(D_80124EEC, (s32)x, (s32)z, &endQuad);
    for (;;) {
        mx = node->x0 + node->x1;
        if (mx < 0) {
            mx++;
        }
        mz = node->y0 + node->y1;
        if (mz < 0) {
            mz++;
        }
        mx >>= 1;
        mz >>= 1;
        minx = (quad & 1) ? mx : node->x0;
        maxx = (quad & 1) ? node->x1 : mx;
        minz = (quad & 2) ? node->y0 : mz;
        maxz = (quad & 2) ? mz : node->y1;
        off = node->child[quad];
        if (node->mask & (16 << quad)) {
            off |= 0x10000;
        }
        if (off != 0) {
            p = D_80152460 + off;
            n = *p++;
            func_800ADCE0(p, n, list, -1);
            for (i = 0; i < n; i++) {
                poly = &D_801497F8[list[i]];
                if ((poly->type & 0xF) == 15 || (poly->type & 0xF) == skipType) {
                    continue;
                }
                if (input_process_controller(poly, cur, end, np, vc, (f32 *)m, 0, (s16 *)&idx, radius * radius) != 0) {
                    if (np[0] < minx || maxx <= np[0] || np[2] < minz || maxz <= np[2]) {
                        continue;
                    }
                    end[0] = np[0];
                    end[1] = np[1];
                    end[2] = np[2];
                    math_utility(m, mat);
                    best = poly;
                    bestIdx = idx;
                }
            }
        }
        if (best != NULL) {
            if (snap != 0) {
                if (best->type & 0x2000) {
                    steering_sensitivity((s32)best, bestIdx, end, vc, mat, 250.0f);
                } else if (best->type & 0x1000) {
                    traction_control((s32)best, bestIdx, end, vc, mat);
                    func_8008E0B8(mat[0]);
                    func_8008E0B8(mat[1]);
                    func_8008E0B8(mat[2]);
                }
            }
            return best;
        }
        if (node == endNode && quad == endQuad) {
            return NULL;
        }
        d[0] = end[0] - cur[0];
        d[1] = end[1] - cur[1];
        d[2] = end[2] - cur[2];
        if (d[0] < 0.0f) {
            np[0] = minx;
            t = (np[0] - cur[0]) / d[0];
            np[1] = d[1] * t + cur[1];
            np[2] = d[2] * t + cur[2];
            if (np[2] == minz && d[2] < 0.0f) {
                node = handbrake_apply(D_80124EEC, minx - 1, minz - 1, &quad);
                goto moved;
            }
            if (np[2] == maxz && d[2] < 0.0f) {
                node = handbrake_apply(D_80124EEC, minx - 1, maxz - 1, &quad);
                goto moved;
            }
            if (minz <= np[2] && np[2] <= maxz) {
                node = handbrake_apply(D_80124EEC, minx - 1, (s32)FLOORF(np[2]), &quad);
                goto moved;
            }
        } else if (d[0] > 0.0f) {
            np[0] = maxx;
            t = (np[0] - cur[0]) / d[0];
            np[1] = d[1] * t + cur[1];
            np[2] = d[2] * t + cur[2];
            if (np[2] == minz && d[2] < 0.0f) {
                node = handbrake_apply(D_80124EEC, maxx, minz - 1, &quad);
                goto moved;
            }
            if (np[2] == maxz && d[2] < 0.0f) {
                node = handbrake_apply(D_80124EEC, maxx, maxz - 1, &quad);
                goto moved;
            }
            if (minz <= np[2] && np[2] <= maxz) {
                node = handbrake_apply(D_80124EEC, maxx, (s32)FLOORF(np[2]), &quad);
                goto moved;
            }
        }
        if (d[2] < 0.0f) {
            np[2] = minz;
            t = (np[2] - cur[2]) / d[2];
            np[1] = d[1] * t + cur[1];
            np[0] = d[0] * t + cur[0];
            if (np[0] == minx && d[0] < 0.0f) {
                node = handbrake_apply(D_80124EEC, minx - 1, minz - 1, &quad);
                goto moved;
            }
            if (np[0] == maxx && d[0] < 0.0f) {
                node = handbrake_apply(D_80124EEC, maxx - 1, minz - 1, &quad);
                goto moved;
            }
            if (minx <= np[0] && np[0] <= maxx) {
                node = handbrake_apply(D_80124EEC, (s32)FLOORF(np[0]), minz - 1, &quad);
                goto moved;
            }
        } else if (d[2] > 0.0f) {
            np[2] = maxz;
            t = (np[2] - cur[2]) / d[2];
            np[1] = d[1] * t + cur[1];
            np[0] = d[0] * t + cur[0];
            if (np[0] == minx && d[0] < 0.0f) {
                node = handbrake_apply(D_80124EEC, minx - 1, maxz, &quad);
                goto moved;
            }
            if (np[0] == maxx && d[0] < 0.0f) {
                node = handbrake_apply(D_80124EEC, maxx - 1, maxz, &quad);
                goto moved;
            }
            if (minx <= np[0] && np[0] <= maxx) {
                node = handbrake_apply(D_80124EEC, (s32)FLOORF(np[0]), maxz, &quad);
                goto moved;
            }
        }
moved:
        cur[0] = np[0];
        cur[1] = np[1];
        cur[2] = np[2];
        if (node == NULL) {
            return NULL;
        }
    }
}
