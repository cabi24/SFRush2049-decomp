s32 func_8001467C(s32);                             /* extern */
? func_80014AF0(s32);                               /* extern */
? func_8001F6EC(void *);                            /* extern */
? func_80020F4C(u8, u8, s32);                       /* extern */
s8 func_80020F98(s32, s32);                         /* extern */
? func_8002106C(void *);                            /* extern */
? func_800217E4(void *);                            /* extern */
s32 func_8002193C(void *, ? *);                     /* extern */
s32 func_80021B9C(void *, ? *);                     /* extern */
s32 func_80021BC0(void *, ? *);                     /* extern */
s32 func_80021BF0(void *, ? *);                     /* extern */
s32 func_80021C58(void *, ? *);                     /* extern */
s32 func_80021CDC(void *, ? *);                     /* extern */
s32 func_80021D5C(void *, ? *);                     /* extern */
s32 func_80021DF8(void *, ? *);                     /* extern */
s32 func_80021E74(void *, ? *);                     /* extern */
s32 func_80021F08(void *, ? *);                     /* extern */
s32 func_80021F68(void *, ? *);                     /* extern */
s32 func_80021F84(void *, ? *);                     /* extern */
s32 func_80022160(void *, ? *);                     /* extern */
s32 func_80022324(void *, ? *);                     /* extern */
s32 func_800223D0(void *, ? *);                     /* extern */
s32 func_8002243C(void *, ? *);                     /* extern */
s32 func_8002245C(void *, ? *);                     /* extern */
s32 func_800224B0(void *, ? *);                     /* extern */
s32 func_80022514(void *, ? *);                     /* extern */
s32 func_80022580(void *, ? *);                     /* extern */
s32 func_800225AC(void *, ? *);                     /* extern */
s32 func_800225DC(void *, ? *);                     /* extern */
s32 func_800225FC(void *, ? *);                     /* extern */
s32 func_80022678(void *, ? *);                     /* extern */
s32 func_8002279C(void *, ? *);                     /* extern */
s32 func_80022874(void *, ? *);                     /* extern */
s32 func_80022A40(void *, ? *);                     /* extern */
s32 func_80022A78(void *, ? *);                     /* extern */
s32 func_80022A98(void *, ? *);                     /* extern */
s32 func_80022C58(void *, ? *);                     /* extern */
s32 func_80022CD4(void *, ? *);                     /* extern */
s32 func_80022F24(void *, ? *);                     /* extern */
s32 func_8002300C(void *, ? *, ?);                  /* extern */
s32 func_800230B0(void *, ? *);                     /* extern */
s32 func_80023190(void *, ? *);                     /* extern */
s32 func_800232A4(void *, ? *);                     /* extern */
s32 func_80023520(void *, ? *);                     /* extern */
s32 func_80023544(void *, ? *);                     /* extern */
s32 func_80023564(void *, ? *);                     /* extern */
s32 func_80023710(void *, ? *);                     /* extern */
s32 func_80023734(void *, ? *);                     /* extern */
s32 func_80023818(void *, ? *);                     /* extern */
s32 func_80023844(void *, ? *);                     /* extern */
s32 func_80023870(void *, ? *);                     /* extern */
s32 func_8002389C(void *, ? *);                     /* extern */
s32 func_800238C8(void *, ? *);                     /* extern */
s32 func_800238F4(void *, ? *);                     /* extern */
s32 func_80023920(void *, ? *);                     /* extern */
s32 func_8002394C(void *, ? *);                     /* extern */
s32 func_80023978(void *, ? *);                     /* extern */
s32 func_800239A4(void *, ? *);                     /* extern */
s32 func_80023BDC(void *, ? *, ?);                  /* extern */
s32 func_80023D70(void *, ? *);                     /* extern */
s32 func_80023DB8(void *, ? *, ?);                  /* extern */
extern s32 D_8004BE90;
extern u8 D_80056200;
extern ? D_80056228;

void func_80023E9C(void *arg0) {
    ? temp_t6;
    s32 temp_s0;
    s32 temp_s0_2;
    s32 temp_t2_2;
    s32 temp_v0;
    s32 temp_v0_3;
    s32 var_s0;
    s32 var_v0;
    s32 var_v1_2;
    s8 temp_v0_2;
    s8 var_v1;
    u32 temp_at;
    u32 temp_t8;
    u32 temp_v0_4;
    u8 temp_t2;
    u8 temp_t3;
    u8 temp_t9;

    var_s0 = (unaligned s32) arg0->unk24;
    if (var_s0 & 3) {
        if (var_s0 & 1) {
            arg0->unk24 = (unaligned s32) (var_s0 & ~1);
            func_80014AF0((unaligned s32) arg0->unk60 & 0xFF);
            var_s0 = (unaligned s32) arg0->unk24;
        }
        if (!(var_s0 & 0x40)) {
            func_8002106C(arg0);
            temp_t2 = arg0->unk54;
            temp_t3 = arg0->unk55;
            temp_v0 = arg0->unk52 << 0x10;
            arg0->unk38 = (unaligned s32) (arg0->unk53 << 0x10);
            arg0->unk30 = (unaligned s32) temp_v0;
            arg0->unk34 = (unaligned s32) temp_v0;
            arg0->unk4A = temp_t2;
            arg0->unk4B = temp_t3;
            arg0->unk2F = (u8) arg0->unk56;
            temp_v0_2 = func_80020F98(temp_t2 & 0xFF, temp_t3 & 0xFF);
            if (temp_v0_2 != 0xFF) {
                arg0->unkC1 = temp_v0_2;
                var_v1 = arg0->unk4F | (arg0->unk4E << 8);
            } else {
                var_v1 = arg0->unk4F | (arg0->unk4E << 8);
                arg0->unkC1 = var_v1;
            }
            func_80020F4C(arg0->unk4A, arg0->unk4B, var_v1 & 0xFF);
            func_800217E4(arg0);
            temp_at = 0U >> 8;
            arg0->unk68 = (s8) temp_at;
            arg0->unk170 = (s8) temp_at;
            arg0->unk17C = (s8) temp_at;
            arg0->unkB8 = (s8) (0x2000U >> 8);
            arg0->unk8C = (unaligned s32) 0;
            arg0->unk90 = (unaligned s32) 0x64;
            arg0->unk6A = 2;
            arg0->unk6B = 2;
            arg0->unk16C = (unaligned s32) 0;
            arg0->unk178 = (unaligned s32) 0;
            arg0->unk1C = (unaligned s32) 0;
            arg0->unk20 = (unaligned s32) 0;
            arg0->unk5C = (unaligned s32) 0x4000AC44;
            arg0->unk24 = (unaligned s32) ((unaligned s32) arg0->unk24 & 8);
            arg0->unk98 = 0;
            arg0->unk99 = 0x80;
            arg0->unk9A = 0;
            arg0->unk69 = 0;
            arg0->unk48 = 0;
            arg0->unk49 = 0;
            arg0->unk40 = 0;
            arg0->unk44 = 0;
            arg0->unk171 = 0;
            arg0->unk17D = 0;
            arg0->unkB9 = 0x2000;
            arg0->unkC2 = (s8) (0U >> 8);
            arg0->unkC3 = 0;
            var_v0 = 0;
            arg0->unkBE = (u8) arg0->unkBF;
            do {
                temp_t2_2 = var_v0 * 2;
                var_v0 = (var_v0 + 1) & 0xFF;
                (arg0 + temp_t2_2)->unk180 = 0;
            } while (var_v0 < 0x10);
            arg0->unkA0 = (unaligned s32) 0;
            arg0->unkA4 = (unaligned s32) 0;
            goto block_11;
        }
        arg0->unk24 = (unaligned s32) 0;
        func_8001F6EC(arg0);
        return;
    }
block_11:
    temp_v0_3 = (unaligned s32) arg0->unk20;
    if (temp_v0_3 != 0) {
        temp_s0 = (unaligned s32) arg0->unk24;
        if (!(temp_s0 & 0x40000000) && (temp_s0 & 8)) {
            temp_t6 = (unaligned s32) arg0->unk1C;
            arg0->unk4 = temp_v0_3;
            arg0->unk9C = (unaligned s32) 0;
            arg0->unk1C = (unaligned s32) 0;
            arg0->unk20 = (unaligned s32) 0;
            arg0->unk0 = (unaligned s32) temp_t6;
        }
    }
    if ((unaligned s32) arg0->unk9C == 0) {
        D_80056200 = 0;
loop_17:
        temp_t9 = D_80056200 + 1;
        D_80056200 = temp_t9;
        if ((temp_t9 & 0xFF) < 0x21) {
            D_80056228.unk0 = (s32) (unaligned s32) arg0->unk4->unk0;
            D_80056228.unk4 = (s32) (unaligned s32) arg0->unk4->unk4;
            arg0->unk4 = (unaligned s32) ((unaligned s32) arg0->unk4 + 8);
            temp_t8 = D_80056228.unk0 & 0x7F;
            switch (temp_t8) {
            case 0x0:
                var_v1_2 = func_80021BC0(arg0, &D_80056228) & 0xFF;
                break;
            case 0x1:
                var_v1_2 = func_80021BF0(arg0, &D_80056228) & 0xFF;
                break;
            case 0x2:
                var_v1_2 = func_80021C58(arg0, &D_80056228) & 0xFF;
                break;
            case 0x3:
                var_v1_2 = func_80021CDC(arg0, &D_80056228) & 0xFF;
                break;
            case 0x4:
                var_v1_2 = func_8002193C(arg0, &D_80056228) & 0xFF;
                break;
            case 0x5:
                var_v1_2 = func_80021F84(arg0, &D_80056228) & 0xFF;
                break;
            case 0x6:
                var_v1_2 = func_80021E74(arg0, &D_80056228) & 0xFF;
                break;
            case 0x7:
                var_v1_2 = func_80021B9C(arg0, &D_80056228) & 0xFF;
                break;
            case 0x8:
                var_v1_2 = func_80022160(arg0, &D_80056228) & 0xFF;
                break;
            case 0x9:
                var_v1_2 = func_80022324(arg0, &D_80056228) & 0xFF;
                break;
            case 0xA:
                var_v1_2 = func_80021D5C(arg0, &D_80056228) & 0xFF;
                break;
            case 0xB:
                var_v1_2 = func_80023190(arg0, &D_80056228) & 0xFF;
                break;
            case 0xC:
                var_v1_2 = func_80022F24(arg0, &D_80056228) & 0xFF;
                break;
            case 0xD:
                var_v1_2 = func_800232A4(arg0, &D_80056228) & 0xFF;
                break;
            case 0xE:
                var_v1_2 = func_800230B0(arg0, &D_80056228) & 0xFF;
                break;
            case 0xF:
                var_v1_2 = func_80023520(arg0, &D_80056228) & 0xFF;
                break;
            case 0x10:
                var_v1_2 = func_80022874(arg0, &D_80056228) & 0xFF;
                break;
            case 0x11:
                var_v1_2 = func_80022A40(arg0, &D_80056228) & 0xFF;
                break;
            case 0x12:
                var_v1_2 = func_80022A78(arg0, &D_80056228) & 0xFF;
                break;
            case 0x13:
                var_v1_2 = func_80021DF8(arg0, &D_80056228) & 0xFF;
                break;
            case 0x14:
                var_v1_2 = func_80023544(arg0, &D_80056228) & 0xFF;
                break;
            case 0x17:
                var_v1_2 = func_80023564(arg0, &D_80056228) & 0xFF;
                break;
            case 0x18:
                var_v1_2 = func_80022678(arg0, &D_80056228) & 0xFF;
                break;
            case 0x19:
                var_v1_2 = func_800225FC(arg0, &D_80056228) & 0xFF;
                break;
            case 0x1A:
                var_v1_2 = func_8002279C(arg0, &D_80056228) & 0xFF;
                break;
            case 0x1B:
                var_v1_2 = func_800239A4(arg0, &D_80056228) & 0xFF;
                break;
            case 0x1C:
                var_v1_2 = func_80022A98(arg0, &D_80056228) & 0xFF;
                break;
            case 0x1D:
                var_v1_2 = func_8002300C(arg0, &D_80056228, 0) & 0xFF;
                break;
            case 0x1E:
                var_v1_2 = func_8002300C(arg0, &D_80056228, 1) & 0xFF;
                break;
            case 0x1F:
                var_v1_2 = func_80022CD4(arg0, &D_80056228) & 0xFF;
                break;
            case 0x28:
                var_v1_2 = func_80021F08(arg0, &D_80056228) & 0xFF;
                break;
            case 0x29:
                var_v1_2 = func_80021F68(arg0, &D_80056228) & 0xFF;
                break;
            case 0x30:
                var_v1_2 = func_800223D0(arg0, &D_80056228) & 0xFF;
                break;
            case 0x31:
                var_v1_2 = func_8002243C(arg0, &D_80056228) & 0xFF;
                break;
            case 0x32:
                var_v1_2 = func_800225AC(arg0, &D_80056228) & 0xFF;
                break;
            case 0x33:
                var_v1_2 = func_800225DC(arg0, &D_80056228) & 0xFF;
                break;
            case 0x34:
                var_v1_2 = func_80023734(arg0, &D_80056228) & 0xFF;
                break;
            case 0x35:
                var_v1_2 = func_80023710(arg0, &D_80056228) & 0xFF;
                break;
            case 0x36:
                var_v1_2 = func_80022580(arg0, &D_80056228) & 0xFF;
                break;
            case 0x37:
                var_v1_2 = func_80022514(arg0, &D_80056228) & 0xFF;
                break;
            case 0x38:
                var_v1_2 = func_8002245C(arg0, &D_80056228) & 0xFF;
                break;
            case 0x39:
                var_v1_2 = func_800224B0(arg0, &D_80056228) & 0xFF;
                break;
            case 0x40:
                var_v1_2 = func_80023818(arg0, &D_80056228) & 0xFF;
                break;
            case 0x41:
                var_v1_2 = func_80023844(arg0, &D_80056228) & 0xFF;
                break;
            case 0x42:
                var_v1_2 = func_80023870(arg0, &D_80056228) & 0xFF;
                break;
            case 0x43:
                var_v1_2 = func_8002389C(arg0, &D_80056228) & 0xFF;
                break;
            case 0x44:
                var_v1_2 = func_800238C8(arg0, &D_80056228) & 0xFF;
                break;
            case 0x45:
                var_v1_2 = func_800238F4(arg0, &D_80056228) & 0xFF;
                break;
            case 0x46:
                var_v1_2 = func_80023920(arg0, &D_80056228) & 0xFF;
                break;
            case 0x47:
                var_v1_2 = func_8002394C(arg0, &D_80056228) & 0xFF;
                break;
            case 0x48:
                var_v1_2 = func_80023978(arg0, &D_80056228) & 0xFF;
                break;
            case 0x50:
                var_v1_2 = func_80022C58(arg0, &D_80056228) & 0xFF;
                break;
            case 0x60:
                var_v1_2 = func_80023BDC(arg0, &D_80056228, 0) & 0xFF;
                break;
            case 0x61:
                var_v1_2 = func_80023BDC(arg0, &D_80056228, 1) & 0xFF;
                break;
            case 0x62:
                var_v1_2 = func_80023BDC(arg0, &D_80056228, 2) & 0xFF;
                break;
            case 0x63:
                var_v1_2 = func_80023BDC(arg0, &D_80056228, 3) & 0xFF;
                break;
            case 0x64:
                var_v1_2 = func_80023BDC(arg0, &D_80056228, 4) & 0xFF;
                break;
            case 0x65:
                var_v1_2 = func_80023D70(arg0, &D_80056228) & 0xFF;
                break;
            case 0x70:
                var_v1_2 = func_80023DB8(arg0, &D_80056228, 0) & 0xFF;
                break;
            case 0x71:
                var_v1_2 = func_80023DB8(arg0, &D_80056228, 1) & 0xFF;
                break;
            default:
                var_v1_2 = 0;
                break;
            }
            if (var_v1_2 == 0) {
                goto loop_17;
            }
        }
        arg0->unkA4 = (unaligned s32) ((unaligned s32) arg0->unkA4 + D_8004BE90);
        return;
    }
    temp_s0_2 = (unaligned s32) arg0->unk24;
    arg0->unkA4 = (unaligned s32) ((unaligned s32) arg0->unkA4 + D_8004BE90);
    if ((temp_s0_2 & 4) && !(temp_s0_2 & 0x40000000) && (temp_s0_2 & 8)) {
        arg0->unk24 = (unaligned s32) (temp_s0_2 & ~4);
        arg0->unk9C = (unaligned s32) 0;
        arg0->unkA0 = (unaligned s32) arg0->unkA4;
    }
    if (((unaligned s32) arg0->unk24 & 0x100000) && (func_8001467C((unaligned s32) arg0->unk60 & 0xFF) == 0)) {
        arg0->unk24 = (unaligned s32) ((unaligned s32) arg0->unk24 & 0xFFEFFFFF);
        arg0->unk9C = (unaligned s32) 0;
        arg0->unkA0 = (unaligned s32) arg0->unkA4;
    }
    temp_v0_4 = (unaligned s32) arg0->unk9C;
    if ((temp_v0_4 != -1U) && ((u32) (unaligned s32) arg0->unkA4 >= temp_v0_4)) {
        arg0->unkA0 = temp_v0_4;
        arg0->unk9C = (unaligned s32) 0;
    }
}
