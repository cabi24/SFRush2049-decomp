/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
extern void *D_800382D0;
extern unsigned short *D_800382D4;
extern void func_80011910(void *, unsigned short);
void func_800119E0(void)
{
    func_80011910(D_800382D0, *D_800382D4);
}
