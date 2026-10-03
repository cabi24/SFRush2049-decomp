/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct TransferRequest {
    void *destination;
    void *source;
    unsigned int size;
} TransferRequest;
typedef struct OSMesgQueue_s OSMesgQueue;
extern volatile unsigned char D_80037FA0;
extern TransferRequest D_80037FA8[];
extern unsigned char D_80037FE0[];
extern void func_80014594(void);
extern void func_800145DC(void);
extern void func_800105C4(void);
extern void func_80010110(void);
extern void osInvalDCache(void *, int);
extern int osRecvMesg(OSMesgQueue *, void **, int);
void func_80010714(void *destination, void *source, unsigned int size)
{
    func_80014594();
    func_800105C4();
    if (D_80037FA0 < 4) {
        D_80037FA8[D_80037FA0].destination = destination;
        D_80037FA8[D_80037FA0].source = source;
        D_80037FA8[D_80037FA0].size = (size + 15) & ~15U;
        D_80037FA0++;
        osInvalDCache(destination, size);
    }
    func_800145DC();
}
