/* camera_scene_manager (0x800C2BE0, 614 words) -- strict MATCH in this real -O3 unit (frontier wave 9, w9h).
 * Per-frame stunt/trick driver over all players (nothing here is a camera; the label is historical):
 * idle-timer bookkeeping (D_8016139C, 2.69 s), wheel-contact counters from the HUD record, axis-flip
 * detection on the car velocity, then the per-axis trackers (func_800C2944/26C4/2430, called with idx in
 * $t1 by IPA), three more trackers that umerge inlined (below), the side trackers func_800C220C/2004
 * (idx in $s1), and the landing check that banks the combo (func_800C1B60(i, 11)).
 *
 * Source structure that the match depends on (each verified by compile; see w9h/RESULTS.md):
 *  - func_800C2418 / func_800C2420 / func_800C2428 are real functions inlined by umerge (one call site,
 *    not kept): they leave the three caller-less `jr ra; nop` stubs at 0x800C2418/2420/2428, and retail
 *    calls run in reverse address order 2944, 26C4, 2430, 2428, 2420, 2418, 220C, 2004.  They are written
 *    like their siblings but WITHOUT a cached `fl` local (p->flags directly; uopt's CSE gives retail's
 *    reloads).  The 0x80 tracker needs the sum in a named float (`s`), assigned inside the condition.
 *  - Both player loops index by `i` (D_801569B8[i], player_array[i]); uopt strength-reduces them into
 *    $s2/$s4 in the preheader, and the first loop's induction pointer shares $s2 with the second.
 *  - The stunt record is addressed as D_80152038[i] (no pointer local): a store through a Stunt* local
 *    kills the PRE web of D_80157238 in the tail and rotates v0/v1/a1 through the whole body.
 *  - No `fl` local at all for ply->flags; the flip pairs are two stores (`&= ~A; |= B`) as in retail.
 *  - `*(f32 *)(u32)&D_8016139C += *pdt` (address-laundered) makes the store go through the hoisted
 *    address register like the loads; `pdt` is the laundered &D_8002EB94 (frame delta).
 *  - Chained zeroing assignments, `x++` counters, int 1 in `(D_801543CC - t38) > 1` (w1g findings).
 * Literals (2.69f, 0.15f, 0.4712389f, 3.1415927f) are natural; own .rodata verified at 0x80123F38..0x80123F54.
 */
void func_800C2418(s32 idx) {
    Ply *p;
    f32 r;

    p = &D_801569B8[idx];
    if (p->flags & 1) {
        if (D_80157238 >= 3 || D_80157238 < 2) {
            p->t40 = D_801543CC;
            r = p->t40 - p->t3C;
            p->flags &= ~1;
            if (r > 0.25f) {
                r -= 0.25f;
                while (r > 0.0f) {
                    func_800C1B60(idx, 6);
                    r -= 0.15f;
                }
            }
        }
    } else if (D_80157238 == 2 && (D_8015B248 == 2 || D_8015B258 == 2)) {
        p->flags |= 1;
        p->t40 = -1.0f;
        p->t3C = D_801543CC;
    }
}

void func_800C2420(s32 idx) {
    Ply *p;
    f32 r;

    p = &D_801569B8[idx];
    if (p->flags & 0x100) {
        if (D_80157238 > 0 || (player_array[idx].fE8 & 0x100000)) {
            p->t38 = D_801543CC;
            r = p->t38 - p->t34;
            p->flags &= ~0x100;
            while (r > 5.0f) {
                func_800C1B60(idx, 9);
                r -= 1.0f;
            }
        }
    } else if (D_80157238 == 0) {
        p->flags |= 0x100;
        p->t38 = -1.0f;
        p->t34 = D_801543CC;
    }
}

void func_800C2428(s32 idx) {
    Ply *p;
    PCar *c;
    s32 n;
    f32 s;

    p = &D_801569B8[idx];
    c = &player_array[idx];
    if (p->flags & 0x80) {
        n = 0;
        if (D_80157238 >= 3) {
            p->flags &= ~0x80;
        } else if (D_80157238 == 0) {
            p->a28 += fabsf(c->vx);
            p->a2C += fabsf(c->vy);
            p->a30 += fabsf(c->vz);
        }
        if (0.4712389f < p->a28) {
            n = 1;
        }
        if (0.4712389f < p->a2C) {
            n++;
        }
        if (0.4712389f < p->a30) {
            n++;
        }
        if (n >= 2 && 3.1415927f <= (s = p->a30 + (p->a28 + p->a2C))) {
            p->flags &= ~0x70;
            func_800C1B60(idx, 5);
            p->a28 = 0.0f;
            p->a2C = 0.0f;
            p->a30 = 0.0f;
        }
    } else if (D_80161360 != 0) {
        p->flags |= 0x80;
        p->a28 = fabsf(c->vx);
        p->a2C = fabsf(c->vy);
        p->a30 = fabsf(c->vz);
    }
}

void camera_scene_manager(void) {
    Ply *ply;
    PCar *car;
    HudRec *hud;
    s32 i;
    s32 ok;
    f32 *pdt;

    pdt = (f32 *)(u32)&D_8002EB94;
    if (D_8013FECB != 0) {
        ok = 1;
        for (i = 0; i < active_player_count; i++) {
            if (D_801569B8[i].flags & 0x1F7) {
                D_8016139C = 0.0f;
                ok = 0;
            }
        }
        if (ok != 0) {
            *(f32 *)(u32)&D_8016139C += *pdt;
            if (2.69f <= D_8016139C) {
                D_80152738 = 1;
            }
        }
    }
    for (i = 0; i < active_player_count; i++) {
        ply = &D_801569B8[i];
        car = &player_array[i];
        hud = &((HudRec *) &D_8014A250)[i];
        if ((ply->flags & 0x1F0) && hud->f3F8 != 0) {
            D_80152038[i].f60 += *pdt;
        }
        if ((ply->flags & 0xC08FFFFF) && (car->f358 != 0 || hud->f6C4 != -1)) {
            ply->flags = 0x15000000;
        } else {
            D_80157238 = D_8015B248 = D_8015B258 = D_8015F730 = D_8015F728 = 0;
            if (hud->s61C != 8) {
                D_80157238++;
                D_8015B248++;
                D_8015F730++;
            }
            if (hud->s61E != 8) {
                D_80157238++;
                D_8015B258++;
                D_8015F730++;
            }
            if (hud->s620 != 8) {
                D_80157238++;
                D_8015B248++;
                D_8015F728++;
            }
            if (hud->s622 != 8) {
                D_80157238++;
                D_8015B258++;
                D_8015F728++;
            }
            D_80161360 = D_80157238 < 3 && car->f358 == 0 && hud->f6C4 == -1;
            D_80161388[0] = fabsf(car->vx);
            D_80161388[1] = fabsf(car->vy);
            D_80161388[2] = fabsf(car->vz);
            D_80156BC8 = D_80156BD8 = D_80156CE4 = 0;
            if ((ply->flags & 0x01000000) && car->vx < 0.0f) {
                ply->flags &= 0xFEFFFFFF;
                ply->flags |= 0x02000000;
                D_80156BC8 = 1;
            } else if (ply->flags & 0x02000000) {
                if (car->vx >= 0.0f) {
                    ply->flags &= 0xFDFFFFFF;
                    ply->flags |= 0x01000000;
                    D_80156BC8 = 1;
                }
            }
            if ((ply->flags & 0x04000000) && car->vy < 0.0f) {
                ply->flags &= 0xFBFFFFFF;
                ply->flags |= 0x08000000;
                D_80156BD8 = 1;
            } else if (ply->flags & 0x08000000) {
                if (car->vy >= 0.0f) {
                    ply->flags &= 0xF7FFFFFF;
                    ply->flags |= 0x04000000;
                    D_80156BD8 = 1;
                }
            }
            if ((ply->flags & 0x10000000) && car->vz < 0.0f) {
                ply->flags &= 0xEFFFFFFF;
                ply->flags |= 0x20000000;
                D_80156CE4 = 1;
            } else if (ply->flags & 0x20000000) {
                if (car->vz >= 0.0f) {
                    ply->flags &= 0xDFFFFFFF;
                    ply->flags |= 0x10000000;
                    D_80156CE4 = 1;
                }
            }
            func_800C2944(i);
            func_800C26C4(i);
            func_800C2430(i);
            func_800C2428(i);
            func_800C2420(i);
            func_800C2418(i);
            func_800C220C(i);
            func_800C2004(i);
            if (ply->flags & 0x1F7) {
                ply->t54 = -1.0f;
            } else if (D_80157238 >= 3) {
                if (D_80152038[i].count == 0) {
                    D_80152038[i].f60 = 0.0f;
                }
                if (ply->t54 == -1.0f && D_80152038[i].count != 0) {
                    ply->t54 = D_801543CC;
                }
            }
            if (D_80157238 >= 3 && D_80152038[i].count != 0) {
                if (ply->t38 != -1.0f && (D_801543CC - ply->t38) > 1) {
                    ply->t38 = -1.0f;
                    func_800C1B60(i, 11);
                }
            }
            car->fE8 &= 0xFFEFFFFF;
        }
    }
}

