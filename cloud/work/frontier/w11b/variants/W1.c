/*
 * entity_update (0x800C6AA0, 391 words) -- w10c near-miss: 27/391 words, all of them
 * temp-slot numbers (+4: ours 112/116/120, retail 108/112/116) and one v0/v1 swap (*surf reload
 * vs. the D_80152818 slot pointer).  Opcode sequence identical (opdiff.py: 0 rows).
 * Per-wheel surface probe (sibling of camera_trigger_check; historical label):
 *   floor(pos) -> quadtree cell starting from car->node[wheel] (+928, cached back on success);
 *   polygon list (func_800ADCE0, marker = car->hint[wheel] +1440); func_800C3AD0 with zmin -5 and
 *   bound = best; keep -5 < h < best; out = hit point, mat = basis, hint[wheel] = poly index.
 *   Winner: (out.y < 1 && cnt & 0x30) -> car->p1608 = poly; 0x2000 -> steering_sensitivity
 *   (miss below -5), 0x1000 -> traction_control.  *surf = type & 0xF, kind/slope/attr[wheel]
 *   from type/cnt; surface 3/4/7 with out.y < 0.25 and the car slot not flagged (857/856) and
 *   s1732 == -1: 4 -> effect_cleanup(id, id, -1) + b1600 = 1, 7 -> b1741 = 1 + func_800C54F0,
 *   3 -> same + func_800B61A8(21, 0, 1, 2) (the locked SOUND wrapper, inlined).
 *   Miss: *surf = kind = slope = attr = 0, out = pos + (0, 200, 0), mat = identity (D_8011418C).
 * Levers found: u8 n / u32 i / list[i] / n = *p++ (as camera_trigger_check); calling
 *   func_800B61A8 instead of open-coding `if (D_8010FFC0) entity_flags_apply(...)` (it also
 *   stopped -5.0f being hoisted into $f26 -- the constant web's savings 20 vs cost 19.75 were
 *   marginal); `if (off != 0) { ... if (bestAbs != 250.0f) { ... return; } }` falling into miss:
 *   (off == 0 and "no hit" share retail's recompute block); `attr &= 0x7FF` (stored value forwarded).
 * Residual: one more uopt temp word than retail at the bottom of the frame (named locals and
 *   the inline-reservation area line up: best 164, poly 168, list 180, quad 278).  Tried: x/z
 *   named or not, unused pads, n = *p; p+1, attr spelling, s16 idx, -200 spelling, an extra
 *   inlined helper (adds 8 bytes per call site).  Next: find which expression owns the extra
 *   temp (ugen spill-slot numbering), e.g. the D_80152818[car->id] address or the inlined
 *   effect_cleanup arguments.
 */
static void eu_snd(void) { func_800B61A8(21, 0, 1, 2); }
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
    if ((*surf == 3 || *surf == 4 || *surf == 7) && out[1] < 0.25f) {
        if (D_80152818[car->id].b857 == 0 && D_80152818[car->id].b856 == 0 && car->s1732 == -1) {
            if (*surf == 4) {
                effect_cleanup(car->id, car->id, -1);
                car->b1600 = 1;
            } else if (*surf == 7) {
                car->b1741 = 1;
                func_800C54F0(car->id, 1);
            } else {
                car->b1741 = 1;
                func_800C54F0(car->id, 1);
                eu_snd();
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
