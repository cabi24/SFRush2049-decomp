/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* osPfsIsPlug (static, boot 0x80010160, 396 B, segment 0x10d60 -> src/rom/lib_10d60.c, IDO 5.3)
 * = ultralib src/io/pfsisplug.c osPfsIsPlug verbatim (VERSION_J+: loop bounds __osMaxControllers).
 * Project names: ultralib __osPfsPifRam = project __osPfsBuffer (u8[64], m2c_types.h; read through an OSPifRam
 *   view for pifstatus), __osContLastCmd = __osPfsRequestType (u8), __osMaxControllers = __osPfsRequestType2 (u8),
 *   __osPfsGetInitData = func_800103A0 (locked, w16g). CONT_CMD_* constants written as their values.
 * w23c: no shaping. Matches the retail .s word for word under IDO 5.3 (tools/asmcmp.py: 0 differing of 99);
 * there is NO inventory target for this function yet (see RESULTS.md). */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef void *OSMesg;
typedef struct OSMesgQueue OSMesgQueue;
typedef struct OSContStatus {
    u16 type;
    u8 status;
    u8 errno;
} OSContStatus;
extern u8 __osPfsBuffer[64];
extern u8 __osPfsRequestType2;
extern void __osSiGetAccess(void);
extern void __osSiRelAccess(void);
extern s32 __osSiRawStartDma(s32 direction, void *dramAddr);
extern s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
extern void __osPfsRequestData(u8 cmd);
extern void func_800103A0(u8* pattern, OSContStatus* data);

s32 osPfsIsPlug(OSMesgQueue* mq, u8* pattern) {
    s32 ret = 0;
    OSMesg msg;
    u8 bitpattern;
    OSContStatus contData[4];
    s32 channel;
    u8 bits = 0;
    s32 crcErrorCount = 3;

    __osSiGetAccess();

    do {
        __osPfsRequestData(0);

        ret = __osSiRawStartDma(1, &__osPfsBuffer);
        osRecvMesg(mq, &msg, 1);

        ret = __osSiRawStartDma(0, &__osPfsBuffer);
        osRecvMesg(mq, &msg, 1);

        func_800103A0(&bitpattern, &contData[0]);

        for (channel = 0; channel < __osPfsRequestType2; channel++) {
            if ((contData[channel].status & 4) == 0) {
                crcErrorCount--;
                break;
            }
        }

        if (channel == __osPfsRequestType2) {
            crcErrorCount = 0;
        }
    } while (crcErrorCount > 0);

    for (channel = 0; channel < __osPfsRequestType2; channel++) {
        if ((contData[channel].errno == 0) && ((contData[channel].status & 1) != 0)) {
            bits |= (1 << channel);
        }
    }
    __osSiRelAccess();
    *pattern = bits;
    return ret;
}
