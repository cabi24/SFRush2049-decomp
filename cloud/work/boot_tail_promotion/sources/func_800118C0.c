/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_800118C0.c: calls renamed to their canonical
 * symbol_addrs names (func_80010110 -> osYieldThread); no other change. */
extern volatile unsigned char D_800382CD;
extern void (*D_80038004)(void);
extern void osYieldThread(void);
void func_800118C0(void)
{
    if (D_800382CD != 0) {
        D_80038004();
        osYieldThread();
        D_800382CD = 0;
    }
}
