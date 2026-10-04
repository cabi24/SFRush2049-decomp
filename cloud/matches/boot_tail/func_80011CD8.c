/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct OSMesgQueue_s OSMesgQueue;
extern int osRecvMesg(OSMesgQueue *, void **, int);
extern unsigned char D_800382B0[];
extern volatile unsigned char D_800382CC;
extern void func_80010110(void);
void func_80011CD8(void)
{
    if (D_800382CC != 0) {
        osRecvMesg((OSMesgQueue *)D_800382B0, 0, 1);
        D_800382CC = 0;
        func_80010110();
    }
}
