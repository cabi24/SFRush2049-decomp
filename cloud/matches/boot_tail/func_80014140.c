/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
extern unsigned short D_80038292;
extern volatile short D_80038362;
extern void func_80013DEC(void);
extern void (*D_8003800C)(void (*)(void));
void func_80014140(void)
{
    D_80038362 = D_80038292;
    while (D_80038362 > 0) {
    }
    D_8003800C(func_80013DEC);
}
