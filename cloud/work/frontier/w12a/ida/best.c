#define FLOORF(v) (((v) < 0.0f && (f32)(s32)(v) != (v)) ? (v) - 1.0f : (v))

/*
 * input_deadzone_apply (0x800ADD58, 895 words) -- w11b near-miss: 122/895 words (w10c: 365).
 * Frame now 400 as retail; every stored local at its retail offset.  Semantics: see w10c's best.c.
 * w11b lever (365 -> 122): the extra 8 frame bytes were the cfe temp of the float ?: in FLOORF
 *   (cfe Udef 228 -> 224 once one 4-byte named local is gone).  The unused pad `Y` was
 *   standing where `off` belongs: declaring `u32 off` in Y's place (and dropping the separate
 *   `off` at the bottom; equally `u8 *p` there) gives cfe 224 = retail.  FLOORF is then used for all
 *   six floors (top two as well; byte-identical to the if/else spelling).  Replacing the ternary by
 *   if/else into x/z or a named float also gives 224 and the same 122 words.
 * Residual (122 rows, structure identical except 12 load-placement rows in face sections 2-4):
 *   (1) uopt FP colours: z = $f2 (retail $f0) and end[2]'s web; forcing z=c24 + w66=c25
 *       (CDX proc 419) fixes those rows only (115 left);
 *   (2) ugen's FP ring (f4 f6 f8 f10) starts one event off from the first FP instruction
 *       (retail start[0] -> $f6, ours $f8) and stays permuted; the face-section load placement
 *       follows from it.  Not moved by: copy order, `best = NULL` placement, loop/comma copies,
 *       FLOORF vs if/else at the top, a float-using procedure before/after it in the same file.
 *   Next: find the one extra FP alloc/free event in the entry block (ucode: lod a3 -> f26 for
 *   radius, rldc 0.0 -> f28, three ilod/str copies, lod cur[0]); compare with retail-shaped
 *   alternatives of the radius parameter use.
 * Use with hdr_ipcorder.h + w1g_part_ipcorder.c (input_process_controller (poly, p1, p2, out,
 * vcOut, mat, flag, outIdx, rad2)).
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
    cur[1] = start[1];
    cur[2] = start[2];
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
