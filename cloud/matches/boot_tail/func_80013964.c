/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
extern unsigned char D_8003829E;
extern unsigned short D_800382E0[];
extern unsigned short *D_800382D8[];
extern unsigned short *D_800382D4;
extern unsigned short *D_800382D0;
extern unsigned int D_80038034;
extern unsigned int D_80038030;
void func_80013964(void)
{
    unsigned int cursor;
    D_800382D4 = &D_800382E0[D_8003829E];
    D_800382D0 = D_800382D8[D_8003829E];
    cursor = (unsigned int)D_800382D0 + 16;
    D_80038034 = cursor & ~15U;
    D_80038030 = cursor;
    *D_800382D4 = 0;
    *D_800382D0 = 0;
}
