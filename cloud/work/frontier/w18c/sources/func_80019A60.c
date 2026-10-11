/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80019A60 - lib_1a660 (IDO 7.1). Set the tick rate of sequence slot INDEX (255 = the
 * global slot 8) from a tempo VALUE: D_8004FA20[index] = (value << 3) * 1536 / 240.
 * Callers: func_80018A30 (lib_17dc0, rate124 change, index D_8004BE78) and lib_1a660
 * `func_80019A60(120, 255)` (default tempo 120), both locked with prototype (u32, u8).
 * w18c: `value << 3` keeps retail's two-step multiply (sll 3, then *1536 as (x<<2)-x <<9);
 * `value * 8` folds into one *12288 (true=160). No shaping devices.
 * Prior: D10 BT03-high-medium (5.3, 6 words; 7.1 true=160). */
extern unsigned int D_8004FA20[];

void func_80019A60(unsigned int value, unsigned char index)
{
    if (index == 255) {
        index = 8;
    }
    D_8004FA20[index] = (value << 3) * 1536 / 240;
}
