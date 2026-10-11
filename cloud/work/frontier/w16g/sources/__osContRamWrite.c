/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* __osContRamWrite (static, boot 0x8000F680, 592 B, segment 0x10280 -> src/rom/lib_10280.c, unconverted)
 * Origin: reference/repos/ultralib (decompals e24c836) src/io/contramwrite.c,
 *         BUILD_VERSION >= VERSION_J body (cached PIF command, addrh/addrl split).
 * Spelling follows the promoted sibling __osContRamRead (src/rom/lib_f4d0.c).
 * Project names for ultralib symbols:
 *   __osPfsBuffer       (u8[64] in m2c_types.h; ultralib OSPifRam __osPfsPifRam)
 *   __osPfsRequestType  (ultralib __osContLastCmd)
 *   __osContLastChannel (ultralib __osPfsLastChannel)
 *   osContStartReadData (0x8000E620; ultralib __osPfsGetStatus)
 *   __osContDataCrc     (0x8000FAC0, current symbol_addrs name; the alias
 *                        __osPfsDataChecksum would relocate differently) */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef void *OSMesg;
typedef struct OSMesgQueue_s {
    void *mtqueue;
    void *fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    OSMesg *msg;
} OSMesgQueue;
typedef struct {
    u32 ramarray[15];
    u32 pifstatus;
} OSPifRam;
typedef struct {
    u8 dummy, txsize, rxsize, cmd, addrh, addrl, data[32], datacrc;
} __OSContRamReadFormat;

#define NULL ((void *)0)
#define TRUE 1
#define OS_READ 0
#define OS_WRITE 1
#define OS_MESG_BLOCK 1
#define BLOCKSIZE 32
#define PFS_LABEL_AREA 7
#define PFS_ERR_NOPACK 1
#define PFS_ERR_CONTRFAIL 4
#define CONT_CMD_REQUEST_STATUS 0
#define CONT_CMD_WRITE_PAK 3
#define CONT_CMD_WRITE_PAK_TX 35
#define CONT_CMD_WRITE_PAK_RX 1
#define CONT_CMD_EXE 1
#define CONT_CMD_NOP 0xFF
#define CONT_CMD_END 0xFE
#define CHNL_ERR(format) (((format).rxsize & 0xC0) >> 4)
#define READFORMAT(ptr) ((__OSContRamReadFormat*)(ptr))

extern void bcopy(const void *src, void *dst, s32 len);
extern s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flags);
extern void __osSiGetAccess(void);
extern void __osSiRelAccess(void);
extern s32 __osSiRawStartDma(s32 direction, void *dramAddr);
extern u8 __osContAddressCrc(u16);
extern u8 __osContDataCrc(u8 *);
extern s32 osContStartReadData(OSMesgQueue *mq, int channel);
extern u8 __osPfsBuffer[64];
extern u8 __osPfsRequestType;
extern s32 __osContLastChannel;

s32 __osContRamWrite(OSMesgQueue* mq, int channel, u16 address, u8* buffer, int force) {
    s32 ret = 0;
    s32 i;
    u8* ptr;
    s32 retry = 2;
    u8 crc;

    if ((force != TRUE) && (address < PFS_LABEL_AREA) && (address != 0)) {
        return 0;
    }

    __osSiGetAccess();

    do {
        ptr = (u8*)&(*(OSPifRam *)__osPfsBuffer);

        if (__osPfsRequestType != CONT_CMD_WRITE_PAK || (u32)__osContLastChannel != channel) {
            __osPfsRequestType = CONT_CMD_WRITE_PAK;
            __osContLastChannel = channel;

            for (i = 0; i < channel; i++) { *ptr++ = CONT_CMD_REQUEST_STATUS; }

            (*(OSPifRam *)__osPfsBuffer).pifstatus = CONT_CMD_EXE;

            READFORMAT(ptr)->dummy = CONT_CMD_NOP;
            READFORMAT(ptr)->txsize = CONT_CMD_WRITE_PAK_TX;
            READFORMAT(ptr)->rxsize = CONT_CMD_WRITE_PAK_RX;
            READFORMAT(ptr)->cmd = CONT_CMD_WRITE_PAK;
            READFORMAT(ptr)->datacrc = 0xFF;

            ptr[sizeof(__OSContRamReadFormat)] = CONT_CMD_END;
        } else {
            ptr += channel;
        }

        READFORMAT(ptr)->addrh = address >> 3;
        READFORMAT(ptr)->addrl = ((address << 5) | __osContAddressCrc(address));

        bcopy(buffer, READFORMAT(ptr)->data, BLOCKSIZE);

        ret = __osSiRawStartDma(OS_WRITE, &(*(OSPifRam *)__osPfsBuffer));
        crc = __osContDataCrc(buffer);
        osRecvMesg(mq, NULL, OS_MESG_BLOCK);

        ret = __osSiRawStartDma(OS_READ, &(*(OSPifRam *)__osPfsBuffer));
        osRecvMesg(mq, NULL, OS_MESG_BLOCK);

        ret = CHNL_ERR(*READFORMAT(ptr));

        if (!ret) {
            if (crc != READFORMAT(ptr)->datacrc) {
                if ((ret = osContStartReadData(mq, channel))) {
                    break;
                } else {
                    ret = PFS_ERR_CONTRFAIL;
                }
            }
        } else {
            ret = PFS_ERR_NOPACK;
        }
    } while ((ret == PFS_ERR_CONTRFAIL) && (retry-- >= 0));

    __osSiRelAccess();

    return ret;
}
