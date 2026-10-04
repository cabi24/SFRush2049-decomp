/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
extern unsigned char D_8002C630;
extern volatile unsigned char D_8004BE94;
extern void func_80014E10(void);
extern void func_80017108(void);
extern void func_800199F4(void);
extern void func_8001C1D8(unsigned int);
extern void func_8001C390(void);
extern void func_8001E0C0(void);

int func_800107E0(unsigned int frequency)
{
    func_80014E10();
    func_80017108();
    func_800199F4();
    D_8004BE94 = 0;
    func_8001C1D8(frequency);
    func_8001C390();
    func_8001E0C0();
    D_8002C630 = 1;
    return 0;
}
