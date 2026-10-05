void camera_scene_manager(void) {
    Ply *ply;
    PCar *car;
    HudRec *hud;
    Stunt *st;
    s32 i;
    s32 ok;
    s32 fl;
    s32 n;
    f32 *pdt;
    f32 r;
    f32 x;
    f32 y;
    f32 z;

    pdt = (f32 *)(u32)&D_8002EB94;
    if (D_8013FECB != 0) {
        ok = 1;
        if (active_player_count > 0) {
            ply = D_801569B8;
            do {
                if (ply->flags & 0x1F7) {
                    D_8016139C = 0.0f;
                    ok = 0;
                }
                ply++;
            } while (ply < &D_801569B8[active_player_count]);
        }
        if (ok != 0) {
            D_8016139C += *pdt;
            if (2.69f <= D_8016139C) {
                D_80152738 = 1;
            }
        }
    }
    for (i = 0, ply = D_801569B8, car = player_array; i < active_player_count; i++, ply++, car++) {
        hud = &((HudRec *) &D_8014A250)[i];
        fl = ply->flags;
        if ((fl & 0x1F0) && hud->f3F8 != 0) {
            st = &D_80152038[i];
            st->f60 += *pdt;
        }
        if ((fl & 0xC08FFFFF) && (car->f358 != 0 || hud->f6C4 != -1)) {
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
            D_80161360 = D_80157238 < 3 && car->f358 == 0 && (hud->f6C4 + 1) == 0;
            D_80161388[0] = fabsf(car->vx);
            D_80161388[1] = fabsf(car->vy);
            D_80161388[2] = fabsf(car->vz);
            D_80156BC8 = D_80156BD8 = D_80156CE4 = 0;
            fl = ply->flags;
            if ((fl & 0x01000000) && car->vx < 0.0f) {
                ply->flags = fl & 0xFEFFFFFF;
                *(u32 *)&ply->flags |= 0x02000000;
                D_80156BC8 = 1;
            } else if (fl & 0x02000000) {
                if (car->vx >= 0.0f) {
                    ply->flags = fl & 0xFDFFFFFF;
                    *(u32 *)&ply->flags |= 0x01000000;
                    D_80156BC8 = 1;
                }
            }
            fl = ply->flags;
            if ((fl & 0x04000000) && car->vy < 0.0f) {
                ply->flags = fl & 0xFBFFFFFF;
                *(u32 *)&ply->flags |= 0x08000000;
                D_80156BD8 = 1;
            } else if (fl & 0x08000000) {
                if (car->vy >= 0.0f) {
                    ply->flags = fl & 0xF7FFFFFF;
                    *(u32 *)&ply->flags |= 0x04000000;
                    D_80156BD8 = 1;
                }
            }
            fl = ply->flags;
            if ((fl & 0x10000000) && car->vz < 0.0f) {
                ply->flags = fl & 0xEFFFFFFF;
                *(u32 *)&ply->flags |= 0x20000000;
                D_80156CE4 = 1;
            } else if (fl & 0x20000000) {
                if (car->vz >= 0.0f) {
                    ply->flags = fl & 0xDFFFFFFF;
                    *(u32 *)&ply->flags |= 0x10000000;
                    D_80156CE4 = 1;
                }
            }
            func_800C2944(i);
            func_800C26C4(i);
            func_800C2430(i);
            fl = ply->flags;
            if (fl & 0x80) {
                n = 0;
                if (D_80157238 >= 3) {
                    fl &= ~0x80;
                    ply->flags = fl;
                } else if (D_80157238 == 0) {
                    ply->a28 += fabsf(car->vx);
                    ply->a2C += fabsf(car->vy);
                    ply->a30 += fabsf(car->vz);
                    fl = ply->flags;
                }
                x = ply->a28;
                if (0.4712389f < x) {
                    n = 1;
                }
                y = ply->a2C;
                if (0.4712389f < y) {
                    n++;
                }
                z = ply->a30;
                if (0.4712389f < z) {
                    n++;
                }
                if (n >= 2 && 3.1415927f <= z + (x + y)) {
                    ply->flags = fl & ~0x70;
                    func_800C1B60(i, 5);
                    ply->a28 = 0.0f;
                    ply->a2C = 0.0f;
                    ply->a30 = 0.0f;
                    fl = ply->flags;
                }
            } else if (D_80161360 != 0) {
                fl = ply->flags = fl | 0x80;
                ply->a28 = fabsf(car->vx);
                ply->a2C = fabsf(car->vy);
                ply->a30 = fabsf(car->vz);
            }
            if (fl & 0x100) {
                if (D_80157238 > 0 || (car->fE8 & 0x100000)) {
                    ply->t38 = D_801543CC;
                    r = ply->t38 - ply->t34;
                    ply->flags = fl & ~0x100;
                    while (r > 5.0f) {
                        func_800C1B60(i, 9);
                        r -= 1.0f;
                    }
                }
                fl = ply->flags;
            } else if (D_80157238 == 0) {
                fl = ply->flags = fl | 0x100;
                ply->t38 = -1.0f;
                ply->t34 = D_801543CC;
            }
            if (fl & 1) {
                if (D_80157238 >= 3 || D_80157238 < 2) {
                    ply->t40 = D_801543CC;
                    r = ply->t40 - ply->t3C;
                    ply->flags = fl & ~1;
                    if (r > 0.25f) {
                        r -= 0.25f;
                        while (r > 0.0f) {
                            func_800C1B60(i, 6);
                            r -= 0.15f;
                        }
                    }
                }
            } else if (D_80157238 == 2 && (D_8015B248 == 2 || D_8015B258 == 2)) {
                ply->flags = fl | 1;
                ply->t40 = -1.0f;
                ply->t3C = D_801543CC;
            }
            func_800C220C(i);
            func_800C2004(i);
            if (ply->flags & 0x1F7) {
                ply->t54 = -1.0f;
            } else if (D_80157238 >= 3) {
                st = &D_80152038[i];
                if (st->count == 0) {
                    st->f60 = 0.0f;
                }
                if (ply->t54 == -1.0f && st->count != 0) {
                    ply->t54 = D_801543CC;
                }
            }
            if (D_80157238 >= 3 && D_80152038[i].count != 0) {
                x = ply->t38;
                if (x != -1.0f && (D_801543CC - x) > 1) {
                    ply->t38 = -1.0f;
                    func_800C1B60(i, 11);
                }
            }
            car->fE8 &= 0xFFEFFFFF;
        }
    }
}

