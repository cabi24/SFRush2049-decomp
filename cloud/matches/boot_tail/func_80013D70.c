/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
extern unsigned char D_8002C630;
extern unsigned int D_800382A0;
extern unsigned int D_800382A4;
extern void *D_80038228[];
extern void func_800198C8(void);
extern void func_8001B154(void);
extern void func_800139D4(void *, unsigned short);

int func_80013D70(unsigned short index, unsigned short count)
{
    if (D_8002C630 != 0) {
        if (--D_800382A0 == 0) {
            D_800382A0 = D_800382A4;
            func_800198C8();
            func_8001B154();
        }
    }
    func_800139D4(D_80038228[index], count);
    return 1;
}
