/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned short u16;
extern u8 D_8002C630;
extern void func_80014594(void);
extern void func_800145DC(void);
extern void func_8001B9F8(u8, u16, u8, u8, unsigned int);
void func_80020494(u8 channel, u16 duration, u8 first, u8 second)
{
    if (D_8002C630) {
        func_80014594();
        if (first) func_8001B9F8(channel, duration, 21, 0, 0);
        if (second) func_8001B9F8(channel, duration, 22, 0, 0);
        func_800145DC();
    }
}
