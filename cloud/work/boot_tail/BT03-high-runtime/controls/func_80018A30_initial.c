/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct TimedValue { u32 time; u32 value; } TimedValue;
typedef struct Context {
    u8 unknown000[0x120];
    u32 half120;
    u32 rate124;
    u8 unknown128[0xE40];
    u32 activeF68;
    TimedValue *currentF6C;
    u32 lowF70;
    u32 highF74;
} Context;
extern Context *D_8004BE80;
extern u32 D_8004BE78;
extern void func_80019A60(u32, u8);
void func_80018A30(void)
{
    if (D_8004BE80->activeF68) {
        while (D_8004BE80->currentF6C->time != 0xFFFFFFFFU) {
            if (D_8004BE80->highF74 + D_8004BE80->half120 <
                D_8004BE80->currentF6C->time) break;
            D_8004BE80->rate124 = D_8004BE80->currentF6C->value;
            func_80019A60(D_8004BE80->rate124, D_8004BE78);
            D_8004BE80->currentF6C++;
        }
    }
}
