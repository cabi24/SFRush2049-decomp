/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef struct OSMesgQueue OSMesgQueue;
typedef struct OSIoMesg OSIoMesg;

typedef struct {
    u32 size;
    u32 devAddr;
    void *vAddr;
} DmaReq;

typedef struct {
    u16 pad0;
    u16 count;
    u16 next;
    u16 pad6;
    DmaReq *reqs;
} DmaList;

extern DmaList D_80153F10;
extern OSIoMesg D_80161438;
extern OSMesgQueue D_80153E68;
s32 __osPiRawStartDma(OSIoMesg *mb, s32 priority, s32 direction, u32 devAddr, void *vAddr, u32 nbytes, OSMesgQueue *mq);

s32 func_8008ABE4(void)
{
    DmaReq *r;

    if (D_80153F10.next < D_80153F10.count) {
        D_80153F10.next++;
        r = &D_80153F10.reqs[D_80153F10.next - 1];
        __osPiRawStartDma(&D_80161438, 1, 0, r->devAddr, r->vAddr, r->size, &D_80153E68);
        return 1;
    }
    return 0;
}
