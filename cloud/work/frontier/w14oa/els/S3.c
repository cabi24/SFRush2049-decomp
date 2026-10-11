/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;

void func_800966D8(s32 arg0, s32 arg1);

void entity_lod_select(u32 *dl, s32 base, s32 seg, s32 flag, u32 mask) {
    u32 *cmd;
    u32 *g;
    u32 val;
    u32 saved;
    s32 first;
    s32 was;
    u8 op;
    u8 op2;
    u32 *stk[10];
    u32 *start;
    u32 **slot;
    u32 *nx;
    u32 sb2;
    u32 snib;
    u32 res;
    s32 w1;
    s32 depth;
    s32 type;
    s32 idx;
    u32 lo16;
    u32 one;
    u32 bit;
    u32 hit;
    s32 isone;
    u32 sum;
    u32 hseg;
    u32 sum4;
    u32 snib2;
    u32 sb;
    u32 dlcmd;
    u32 *tgt;
    s32 nd;
    u32 vtx;
    s32 adv;

    first = 1;
    depth = 0;
    start = dl;
    stk[0] = start;

    while (depth >= 0 && depth < 10) {
        g = stk[depth];
        slot = &stk[depth];
        cmd = *slot;
        op = (g[0] & 0xFF000000) >> 24;
        if ((op & 0xC0) == 0x40 || (op & 0xC0) == 0x80 || (op >= 9 && op < 0x40) || (op >= 0xC0 && op < 0xD6)) {
            goto next;
        }
        nx = cmd + 2;
        op2 = (nx[0] & 0xFF000000) >> 24;
        if (op == 1 || op == 0xDD || op == 0xDE || op == 0xDA || op == 0xDC || (op == 0xE1 && (op2 == 4 || op2 == 0xDD)) || op >= 0xFD) {
            if (seg < 0) {
                val = ((cmd[1] + base) & 0x00FFFFFF) | (cmd[1] & 0x0F000000);
            } else {
                snib = seg & 0xF;
                sb2 = snib << 24;
                val = ((cmd[1] + base) & 0x00FFFFFF) | sb2;
            }
            res = val;
            cmd[1] = res;
        }
        switch (op) {
            case 0xE0:
                type = (cmd[0] & 0x00FF0000) >> 16;
                w1 = cmd[1];
                lo16 = cmd[0];
                idx = lo16 & 0xFFFF;
                one = 1;
                bit = one << idx;
                isone = type == 1;
                if (isone && (hit = bit & mask) == 0) {
                    if (seg < 0) {
                        sum = w1 + (u32) base;
                        hseg = w1 & 0x0F000000;
                        val = (sum & 0x00FFFFFF) | hseg;
                    } else {
                        sum4 = w1 + (u32) base;
                        snib2 = seg & 0xF;
                        sb = snib2 << 24;
                        val = (sum4 & 0x00FFFFFF) | sb;
                    }
                    dlcmd = 0xDE010000;
                    cmd[0] = dlcmd;
                    cmd[1] = val;
                }
                break;
            case 1:
                if (cmd[0] & 0x00FF0000) {}
                was = !!first;
                first = 0;
                if (was) {
                    vtx = val;
                    func_800966D8(vtx, flag);
                }
                break;
            case 0xE1:
                saved = cmd[1];
                break;
            case 4:
                tgt = (u32 *) saved;
                stk[depth] = tgt;
                break;
            case 0xDE:
                adv = 2;
                stk[depth] += adv;
                break;
            case 0xDF:
                nd = depth - 1;
                depth = nd;
                break;
            case 0xDB:
            case 0xDC:
            case 0xDD:
                break;
        }
next:
        if (op != 0xDE) {
            if (depth >= 0) {
                stk[depth] += 2;
            }
        }
    }
}
