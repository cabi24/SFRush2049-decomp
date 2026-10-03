/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
extern unsigned char D_8002C630;
extern unsigned char D_8004F810[];
void *func_80010A14(void)
{
    if (D_8002C630 != 0) {
        return D_8004F810;
    }
    return 0;
}
