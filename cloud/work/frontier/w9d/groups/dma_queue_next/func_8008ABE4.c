/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_8008ABE4 (0x8008ABE4): start the next queued PI DMA. D_80153F10 is a request list
 * {u16 count @2; u16 next @4; DmaReq *reqs @8} of 12-byte {size, devAddr, vAddr} requests; if
 * next < count, advance next and start the DMA of the request just taken with the 7-argument
 * PI start call at 0x80008630 (historical label __osPiRawStartDma; the argument list is
 * osPiStartDma's: io message D_80161438, priority 1, direction 0 = read, done queue D_80153E68).
 * Returns 1 if a DMA was started, else 0.
 *
 * Internal function (IPA): only reproduces with its real callers struct_init_and_call and
 * task_complete_signal in the -O3 unit (four-wide t6-t9 temp ring). Shaping: the request is
 * indexed as reqs[next - 1] after the increment and read field by field with no named pointer
 * (a `DmaReq *r` local adds an `addiu v0,v0,-12`); early returns, not a result variable.
 */
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
    if (D_80153F10.next < D_80153F10.count) {
        D_80153F10.next++;
        __osPiRawStartDma(&D_80161438, 1, 0, D_80153F10.reqs[D_80153F10.next - 1].devAddr,
                          D_80153F10.reqs[D_80153F10.next - 1].vAddr, D_80153F10.reqs[D_80153F10.next - 1].size,
                          &D_80153E68);
        return 1;
    }
    return 0;
}
