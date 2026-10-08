/* flags: -g0 -O3 -mips2 -G 0 -non_shared  (NOT a match: vbatch round 2 template) */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;

void func_800966D8(s32 arg0, s32 arg1);

void entity_lod_select(u32 *dl, s32 base, s32 seg, s32 flag, u32 mask) {
/*@{dstk*/s32 pad0;
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
    u32 pad[20];

/*@| u32 *stk[10];
    s32 pad0;
    s32 pad1;
    u32 val;
    u32 saved;
    s32 first;
    s32 was;
    u8 op;
    u8 op2;
    u32 *cmd;
    s32 w1;
    s32 depth;
    s32 type;
    s32 idx;
    u32 pad[20];

 @| s32 pad0;
    s32 pad1;
    u32 val;
    u32 saved;
    s32 first;
    s32 was;
    u8 op;
    u8 op2;
    u32 *cmd;
    u32 *stk[10];
    s32 w1;
    s32 depth;
    s32 type;
    s32 idx;
    u32 pad[20];

 @}*/    first = 1;
    depth = 0;
    /*@{s0*/stk[0] = dl;/*@| stk[0] = dl; if (stk) {} @| if (stk) {} stk[0] = dl; @| stk[0] = dl; if (dl) {} @}*/
     if (op2) {} 
    while (depth >= 0 && depth < 10) {
        /*@{ck*/cmd = stk[depth];/*@| cmd = *(stk + depth); @| cmd = stk[depth + 0]; @}*/
        /*@{opx*/op = (cmd[0] & 0xFF000000) >> 24;/*@| op = cmd[0] >> 24; @| op = (u8) (cmd[0] >> 24); @| op = (cmd[0] >> 24) & 0xFF; @}*/
        
        
        if (/*@{g1*/(op & 0xC0) == 0x40/*@| 0x40 == (op & 0xC0) @| ((op & 0xC0) ^ 0x40) == 0 @}*/ || (op & 0xC0) == 0x80 || (op >= 9 && op < 0x40) || (op >= 0xC0 && op < 0xD6)) {
            goto next;
        }
        op2 = (cmd[2] & 0xFF000000) >> 24;
        /*@{stb*//*@| if (stk) {} @| if (depth) {} @}*/
        if (op == 1 || op == 0xDD || op == 0xDE || op == 0xDA || op == 0xDC || (op == 0xE1 && (op2 == 4 || op2 == 0xDD)) || op >= 0xFD) {
            /*@{stc*//*@| if (stk) {} @| if (stk[depth]) {} @}*/
            if (seg < 0) {
                val = ((cmd[1] + base) & 0x00FFFFFF) | (cmd[1] & 0x0F000000);
            } else {
                val = ((cmd[1] + base) & 0x00FFFFFF) | ((seg & 0xF) << 24);
            }
            cmd[1] = val;
        }
        switch (op) {
            case 0xE0:
                type = (cmd[0] & 0x00FF0000) >> 16;
                w1 = cmd[1];
                idx = cmd[0] & 0xFFFF;
                if (type == 1 &&  ((1u << idx) & mask) == 0 ) {
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
                if (cmd[0] & 0x00FF0000) {
                }
                 was = !!first; 
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
                /*@{de8*/stk[depth] += 2;/*@| stk[depth] = 2 + stk[depth]; @}*/
                break;
            case 0xDF:
                 depth = depth - 1; 
                break;
            case 0xDB:
            case 0xDC:
            case 0xDD:
                break;
            }
next:
         if (op != 0xDE) { if (depth >= 0) { stk[depth] += 2; } } 
    }
}
