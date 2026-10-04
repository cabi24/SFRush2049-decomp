/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef signed short s16;
extern s16 D_8004BE98[16];
extern void func_80014594(void);
extern void func_800145DC(void);
s16 func_8002021C(u8 channel, s16 value)
{
    s16 previous;
    channel &= 15;
    func_80014594();
    previous = D_8004BE98[channel];
    D_8004BE98[channel] = value;
    func_800145DC();
    return previous;
}
