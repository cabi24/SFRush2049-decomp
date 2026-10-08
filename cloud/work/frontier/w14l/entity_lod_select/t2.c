/* flags: -g0 -O3 -mips2 -G 0 -non_shared  (NOT a match: vbatch round 2 template) */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;

void func_800966D8(s32 arg0, s32 arg1);

void entity_lod_select(u32 *dl, s32 base, s32 seg, s32 flag, u32 mask) {
    /*@{decl*/s32 pad0;
    s32 pad1;
    u32 val;
    u32 saved;
    s32 first;
    s32 was;
    u8 op;
    u8 op2;
    u32 *stk[10];
    u32 *cmd;
    s32 w1;
    s32 depth;
    s32 type;
    s32 idx;
    u32 pad[20];/*@| u32 pad[20];
    s32 idx;
    s32 type;
    s32 depth;
    s32 w1;
    u32 *cmd;
    u32 *stk[10];
    u8 op2;
    u8 op;
    s32 was;
    s32 first;
    u32 saved;
    u32 val;
    s32 pad1;
    s32 pad0; @}*/

    first = 1;
    depth = 0;
    stk[0] = dl;
    /*@{ef*/if (first) {}/*@| if (depth) {} @| if (op2) {} @| if (w1) {} @| if (seg) {} @| @}*/
    /*@{wh*/while (depth >= 0 && depth < 10)/*@| while (depth < 10 && depth >= 0) @| while ((u32) depth < 10) @}*/ {
        cmd = stk[depth];
        op = (cmd[0] & 0xFF000000) >> 24;
        /*@{o2*//*@| op2 = (cmd[2] & 0xFF000000) >> 24; @}*/
        /*@{ecc*//*@| if (cmd[0] & 0x00FF0000) {} @| if (op2) {} @}*/
        if (/*@{g1*/(op & 0xC0) == 0x40/*@| 0x40 == (op & 0xC0) @}*/ || (op & 0xC0) == 0x80 || (op >= 9 && op < 0x40) || (op >= 0xC0 && op < 0xD6)) {
            goto next;
        }
        /*@{o2*/op2 = (cmd[2] & 0xFF000000) >> 24;/*@| @}*/
        if (op == 1 || op == 0xDD || op == 0xDE || op == 0xDA || op == 0xDC || (op == 0xE1 && (op2 == 4 || op2 == 0xDD)) || op >= 0xFD) {
            if (seg < 0) {
                /*@{vs*/val = ((cmd[1] + base) & 0x00FFFFFF) | (cmd[1] & 0x0F000000);/*@| val = (cmd[1] & 0x0F000000) | ((cmd[1] + base) & 0x00FFFFFF); @}*/
            } else {
                val = ((cmd[1] + base) & 0x00FFFFFF) | ((seg & 0xF) << 24);
            }
            cmd[1] = val;
        }
        switch (op) {
            case 0xE0:
                /*@{ty*/type = (cmd[0] & 0x00FF0000) >> 16;/*@| type = (u8) (cmd[0] >> 16); @| type = cmd[0] >> 16 & 0xFF; @}*/
                w1 = cmd[1];
                /*@{ix*/idx = cmd[0] & 0xFFFF;/*@| idx = (u16) cmd[0]; @| idx = cmd[0]; idx &= 0xFFFF; @}*/
                if (type == 1 && /*@{mk*/!((1 << idx) & mask)/*@| ((1u << idx) & mask) == 0 @| !((mask >> idx) & 1) @}*/) {
                    if (seg < 0) {
                        val = ((w1 + (u32) base) & 0x00FFFFFF) | (w1 & 0x0F000000);
                    } else {
                        val = ((w1 + (u32) base) & 0x00FFFFFF) | ((seg & 0xF) << 24);
                    }
                    cmd[0] = 0xDE010000;
                    cmd[1] = val;
                }
                break;
            case 1:
                /*@{ec1*/if (cmd[0] & 0x00FF0000) {
                }/*@| if (first) {} @| if (depth) {} @| @}*/
                /*@{wa*/was = first;/*@| was = first != 0; @| was = !!first; @}*/
                first = 0;
                if (was) {
                    func_800966D8(val, flag);
                }
                break;
            case 0xE1:
                saved = cmd[1];
                break;
            case 4:
                stk[depth] = (u32 *) saved;
                break;
            case 0xDE:
                /*@{de*/stk[depth] += 2;/*@| stk[depth] = stk[depth] + 2; @}*/
                break;
            case 0xDF:
                /*@{dd*/depth--;/*@| depth = depth - 1; @}*/
                break;
            case 0xDB:
            case 0xDC:
            case 0xDD:
                break;
            }
next:
        /*@{nx*/if (op != 0xDE && depth >= 0) { stk[depth] += 2; }/*@| if (op != 0xDE) { if (depth >= 0) { stk[depth] += 2; } } @}*/
    }
}
