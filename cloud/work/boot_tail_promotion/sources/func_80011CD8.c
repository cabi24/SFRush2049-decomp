/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_80011CD8.c: calls renamed to their canonical
 * symbol_addrs names (func_80010110 -> osYieldThread); no other change. */
typedef struct OSMesgQueue_s OSMesgQueue;
extern int osRecvMesg(OSMesgQueue *, void **, int);
extern unsigned char D_800382B0[];
extern volatile unsigned char D_800382CC;
extern void osYieldThread(void);
void func_80011CD8(void)
{
    if (D_800382CC != 0) {
        osRecvMesg((OSMesgQueue *)D_800382B0, 0, 1);
        D_800382CC = 0;
        osYieldThread();
    }
}
