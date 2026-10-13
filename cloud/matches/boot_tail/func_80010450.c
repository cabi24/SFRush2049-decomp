/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80010450: acknowledge the previous audio DMA batch, then append pending
 * transfer requests to the shared twelve-byte DMA record queue. Static
 * lib_11050, IDO 7.1. No proven arcade ancestor. Request layout and volatile
 * pending count follow accepted func_80010628/func_80010714; the record and
 * two-argument command writer follow accepted func_80012730. The source field
 * is pointer-typed like the request; it is one native word. Valid request count
 * is zero through four with sufficient shared queue capacity. No shaping devices
 * or new volatile assumptions. Independent checker owns installation and merging. */
typedef struct TransferRequest { void *destination; void *source; unsigned int size; } TransferRequest;
typedef struct DmaEntry_80010450 { int length; void *source; unsigned char *destination; } DmaEntry_80010450;
extern TransferRequest D_80037FA8[];
extern volatile unsigned char D_80037FA0;
extern unsigned char D_80037FD8;
extern unsigned char D_80037FE0[];
extern void *D_8003802C;
extern unsigned int D_80038038;
extern unsigned int D_8003803C;
extern unsigned short D_80038028;
typedef struct OSMesgQueue_s OSMesgQueue;
extern int osSendMesg(OSMesgQueue *, void *, int);
extern unsigned int func_8002C5E0(unsigned int, unsigned int);
void func_80010450(void)
{
    unsigned char i;
    if (D_80037FD8 != 0) {
        osSendMesg((OSMesgQueue *)D_80037FE0, 0, 0);
    }
    if (D_80037FA0 != 0) {
        for (i = 0; i < D_80037FA0; i++) {
            D_80038038 += 12;
            D_8003803C = func_8002C5E0(D_8003803C, D_80038038);
            ((DmaEntry_80010450 *)D_8003802C)[D_80038028].source = D_80037FA8[i].source;
            ((DmaEntry_80010450 *)D_8003802C)[D_80038028].length = D_80037FA8[i].size;
            ((DmaEntry_80010450 *)D_8003802C)[D_80038028].destination = D_80037FA8[i].destination;
            D_80038028++;
        }
        D_80037FA0 = 0;
        D_80037FD8 = 1;
    } else {
        D_80037FD8 = 0;
    }
}
