/*
 * entity_update (0x800C6AA0, 391 words) -- w11b near-miss: 10/391 words, all one v0/v1 swap
 * (the *surf reload vs. the D_80152818[car->id] slot address in the surface-effect tail).
 * Arcade ancestor: game/stree.c tiresurf(m, ipos, opos, roadcode, uvs, whl) (quadtree leaf from
 * m->lasttp[whl], slist scan, best surface, `*roadcode = flags & SURF_MASK`, miss: opos = ipos
 * with the default plane 200 below, uvs = identity).  N64 adds the hint (+1440), the 0x2000/0x1000
 * snaps and the 3/4/7 surface effects.  Semantics otherwise as w10c (see its best.c header).
 * w11b lever (27 -> 10): the temp-slot +4 was the merged frame, not a coloured web.
 *   umerge places inlined-callee areas in call-site order and 8-aligns each area start:
 *   cfe 172 -> align8(172+4)=176, effect_cleanup +8, func_800B61A8 +20 = 204 (ours, 4 mod 8);
 *   retail's temps need 208.  Any inlined call AFTER the func_800B61A8 call aligns 204 -> 208;
 *   a 0-area helper (no params, or pointer params only) adds nothing else.  eu_nop() is that
 *   helper (a stubbed-out N64 call is the likely original; it leaves no stub in the unit).
 *   A float parameter costs 8 (eu_off: 216); a wrapper around func_800B61A8 gives 212.
 * Residual: uopt p1 colours the *surf reload web (w177: save 1.5, nocs 4) before the slot
 *   address PRE web (w188: save 1.5, nocs 2) -- equal priority, lower web number wins -- so
 *   *surf takes v0 and the slot v1; retail is the other way.  Forcing either w177=c2 or
 *   (w177=c2, w188=c1) gives 0 differing rows (tools: ctr.sh / force.sh, CDX proc 618).
 *   ~45 source variants on the tail (condition shapes, early returns, slot spellings, *surf
 *   casts, slot pointer local, empty-if probes) did not move it.
 */
static void eu_nop(void) {}
void entity_update(EUCar *car, f32 *pos, f32 *out, s32 *surf, f32 (*mat)[3], s32 wheel) {
    f32 m[3][3];
    u32 i;
    u8 *p;
    u16 idx;
    u16 bestIdx;
    s16 quad;
    u8 n;
    QNode *node;
    f32 h;
    u32 off;
    f32 pt[3];
    u16 list[36];
    f32 bestAbs;
    f32 bestH;
    Poly *poly;
    Poly *best;
    f32 x;
    f32 z;

    best = NULL;
    bestAbs = 250.0f;
    if (pos[0] < 0.0f && (f32)(s32)pos[0] != pos[0]) {
        x = pos[0] - 1.0f;
    } else {
        x = pos[0];
    }
    if (pos[2] < 0.0f && (f32)(s32)pos[2] != pos[2]) {
        z = pos[2] - 1.0f;
    } else {
        z = pos[2];
    }
    node = handbrake_apply(car->node[wheel], (s32)x, (s32)z, &quad);
    if (node == NULL) {
        goto miss;
    }
    car->node[wheel] = node;
    off = node->child[quad];
    if (node->mask & (16 << quad)) {
        off |= 0x10000;
    }
    if (off != 0) {
    p = D_80152460 + off;
    n = *p++;
    func_800ADCE0(p, n, list, car->hint[wheel]);
    for (i = 0; i < n; i++) {
        poly = &D_801497F8[list[i]];
        if ((poly->type & 0xF) == 5 || (poly->type & 0xF) == 6 || (poly->type & 0xF) == 15) {
            continue;
        }
        h = bestAbs;
        if (func_800C3AD0(poly, pos, NULL, pt, &h, (s16 *)&idx, (f32 *)m, -5.0f) != 0) {
            if (-5.0f < h && h < bestAbs) {
                best = poly;
                bestIdx = idx;
                bestAbs = h;
                out[0] = pt[0];
                out[1] = pt[1];
                out[2] = pt[2];
                math_utility(m, mat);
                car->hint[wheel] = list[i];
            }
        }
    }
    if (bestAbs != 250.0f) {
    if (out[1] < 1.0f && (best->cnt & 0x30)) {
        car->p1608 = best;
    }
    if (best->type & 0x2000) {
        steering_sensitivity((s32)best, bestIdx, pos, out, mat, 250.0f);
        if (out[1] < -5.0f) {
            goto miss;
        }
    } else if (best->type & 0x1000) {
        traction_control((s32)best, bestIdx, pos, out, mat);
    }
    *surf = best->type & 0xF;
    car->kind[wheel] = (best->type & 0xF0) >> 4;
    car->slope[wheel] = (best->type & 0xF00) >> 8;
    if (best->type & 0x8000) {
        car->slope[wheel] = -car->slope[wheel];
    }
    car->attr[wheel] = best->cnt;
    if (best->cnt & 0x30) {
        car->attr[wheel] &= 0x7FF;
    }
    quad = *surf;
    if ((quad == 3 || quad == 4 || quad == 7) && out[1] < 0.25f) {
        if (D_80152818[car->id].b857 == 0 && D_80152818[car->id].b856 == 0 && car->s1732 == -1) {
            if (quad == 4) {
                effect_cleanup(car->id, car->id, -1);
                car->b1600 = 1;
            } else if (quad == 7) {
                car->b1741 = 1;
                func_800C54F0(car->id, 1);
            } else {
                car->b1741 = 1;
                func_800C54F0(car->id, 1);
                func_800B61A8(21, 0, 1, 2);
                eu_nop();
            }
        }
    }
    return;
    }
    }
miss:
    *surf = 0;
    car->kind[wheel] = 0;
    car->slope[wheel] = 0;
    car->attr[wheel] = 0;
    out[0] = pos[0];
    out[1] = pos[1] - -200.0f;
    out[2] = pos[2];
    math_utility(D_8011418C, mat);
}
