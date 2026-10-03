/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
extern unsigned short D_80038360;
extern void (*D_80038020)(void);
extern void (*D_80038024)(void);
extern void func_80013DEC(void);
extern void (*D_80038008)(void (*)(void));
void func_800140F8(void)
{
    D_80038360 = 0xFFFF;
    D_80038020 = 0;
    D_80038024 = 0;
    D_80038008(func_80013DEC);
}
