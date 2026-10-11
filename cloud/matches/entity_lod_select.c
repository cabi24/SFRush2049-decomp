/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* entity_lod_select (0x80096734, 716 B): display-list walker with a 10-entry DL stack (lane w14oa).
 * For every command that is not a vertex/triangle-class word ((op & 0xC0) == 0x40/0x80, 9..0x3F, 0xC0..0xD5):
 *  - relocate the segment address in w1 for G_VTX (0x01), 0xDA, 0xDC, 0xDD, G_DL (0xDE), 0xE1 followed by
 *    0x04/0xDD, and >= 0xFD (image pointers): addr = ((w1 + base) & 0xFFFFFF) | segment nibble (kept from w1 when
 *    seg < 0, else (seg & 0xF) << 24);
 *  - 0xE0 with type byte 1 is a LOD marker: when bit idx (w0 & 0xFFFF) is clear in mask it is rewritten into a
 *    G_DL branch (0xDE010000) to the relocated w1;
 *  - the first G_VTX calls func_800966D8(vtx, flag); 0xE1 latches w1, 0x04 jumps to it; 0xDF pops the stack.
 * N64-only code (GBI walker); no arcade ancestor.
 *
 * Shaping, disclosed:
 *  - `head = stk[depth]` beside `cmd = *slot`: a second pointer local for the current command, used only for the
 *    opcode read. Its code-free web takes v0 over the opcode block, which puts the and-temp in v1 and the
 *    `op & 0xC0` temp in a0 as in retail (w9d-w14n residual). Removing it gives 5 words.
 *  - The named temporaries (start, slot, nextcmd, segnib/segbits, newaddr, w0, one, bit, hit, islod, sum,
 *    hiaddr, sum2, segnib2, segbits2, branch, target, up, vtx, step) are all used and compile to retail's
 *    instructions; together with the declaration order they give the 232-byte frame (every declared local owns
 *    a home). The retail local set is unknown; this set is frame-equivalent, not recovered.
 *  - `if (cmd[0] & 0x00FF0000) {}` in the G_VTX case is a compiled-out read (load-bearing: 179 words without it;
 *    it keeps 0x00FF0000 in s3).
 * Own rodata: the 7-entry jump table at 0x80123A70 (score.py verifies it).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;

void func_800966D8(s32 arg0, s32 arg1);

void entity_lod_select(u32 *dl, s32 base, s32 seg, s32 flag, u32 mask) {
    u32 *cmd;
    u32 *head;
    u32 val;
    u32 saved;
    s32 first;
    s32 was;
    u8 op;
    u8 op2;
    u32 *stk[10];
    u32 *start;
    u32 **slot;
    u32 *nextcmd;
    u32 segbits;
    u32 segnib;
    u32 newaddr;
    s32 w1;
    s32 depth;
    s32 type;
    s32 idx;
    u32 w0;
    u32 one;
    u32 bit;
    u32 hit;
    s32 islod;
    u32 sum;
    u32 hiaddr;
    u32 sum2;
    u32 segnib2;
    u32 segbits2;
    u32 branch;
    u32 *target;
    s32 up;
    u32 vtx;
    s32 step;

    first = 1;
    depth = 0;
    start = dl;
    stk[0] = start;

    while (depth >= 0 && depth < 10) {
        head = stk[depth];
        slot = &stk[depth];
        cmd = *slot;
        op = (head[0] & 0xFF000000) >> 24;
        if ((op & 0xC0) == 0x40 || (op & 0xC0) == 0x80 || (op >= 9 && op < 0x40) || (op >= 0xC0 && op < 0xD6)) {
            goto next;
        }
        nextcmd = cmd + 2;
        op2 = (nextcmd[0] & 0xFF000000) >> 24;
        if (op == 1 || op == 0xDD || op == 0xDE || op == 0xDA || op == 0xDC || (op == 0xE1 && (op2 == 4 || op2 == 0xDD)) || op >= 0xFD) {
            if (seg < 0) {
                val = ((cmd[1] + base) & 0x00FFFFFF) | (cmd[1] & 0x0F000000);
            } else {
                segnib = seg & 0xF;
                segbits = segnib << 24;
                val = ((cmd[1] + base) & 0x00FFFFFF) | segbits;
            }
            newaddr = val;
            cmd[1] = newaddr;
        }
        switch (op) {
            case 0xE0:
                type = (cmd[0] & 0x00FF0000) >> 16;
                w1 = cmd[1];
                w0 = cmd[0];
                idx = w0 & 0xFFFF;
                one = 1;
                bit = one << idx;
                islod = type == 1;
                if (islod && (hit = bit & mask) == 0) {
                    if (seg < 0) {
                        sum = w1 + (u32) base;
                        hiaddr = w1 & 0x0F000000;
                        val = (sum & 0x00FFFFFF) | hiaddr;
                    } else {
                        sum2 = w1 + (u32) base;
                        segnib2 = seg & 0xF;
                        segbits2 = segnib2 << 24;
                        val = (sum2 & 0x00FFFFFF) | segbits2;
                    }
                    branch = 0xDE010000;
                    cmd[0] = branch;
                    cmd[1] = val;
                }
                break;
            case 1:
                if (cmd[0] & 0x00FF0000) {}
                was = first != 0;
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
                target = (u32 *) saved;
                stk[depth] = target;
                break;
            case 0xDE:
                step = 2;
                stk[depth] += step;
                break;
            case 0xDF:
                up = depth - 1;
                depth = up;
                break;
            case 0xDB:
            case 0xDC:
            case 0xDD:
                break;
        }
next:
        if (op != 0xDE && depth >= 0) {
            stk[depth] += 2;
        }
    }
}
