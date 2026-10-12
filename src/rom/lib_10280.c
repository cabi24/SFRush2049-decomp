/* GENERATED ROM-aligned TU — segment 0x10280 (rom/lib_10280)
 * layout map a7e16243b207e227e8df23c31a3396c01164d159748332ea08293dd2d2e25d2d; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-11 — __osContRamWrite
 * Source:   cloud/work/frontier/w16g/sources/__osContRamWrite.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w16g/sources/__osContRamWrite.c:__osContRamWrite (score0); rom_tu.h context: cloud/work/frontier/w16g/ctx/ctx_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
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
extern s32 __osPfsGetStatus(OSMesgQueue *mq, int channel);
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
                if ((ret = __osPfsGetStatus(mq, channel))) {
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

