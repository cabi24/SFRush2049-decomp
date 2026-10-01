/* GENERATED ROM-aligned TU — segment 0xf4d0 (rom/lib_f4d0)
 * layout map dd46c0970e0903e72dea725fec854d3ffd8b9d44768859c6597dc35487453467; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-01 — __osContRamRead
 * Source:   cloud/work/static_C10/__osContRamRead.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C10/__osContRamRead.c:__osContRamRead (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 __osContRamRead(OSMesgQueue* mq, int channel, u16 address, u8* buffer) {
    s32 ret = 0;
    s32 i;
    u8* ptr;
    s32 retry = 2;

    __osSiGetAccess();

    do {
        ptr = (u8*)&(*(OSPifRam *)__osPfsBuffer);

        if (__osPfsRequestType != CONT_CMD_READ_PAK || (u32)__osContLastChannel != channel) {
            __osPfsRequestType = CONT_CMD_READ_PAK;
            __osContLastChannel = channel;

            for (i = 0; i < channel; i++) { *ptr++ = CONT_CMD_REQUEST_STATUS; }

            (*(OSPifRam *)__osPfsBuffer).pifstatus = CONT_CMD_EXE;

            READFORMAT(ptr)->dummy = CONT_CMD_NOP;
            READFORMAT(ptr)->txsize = CONT_CMD_READ_PAK_TX;
            READFORMAT(ptr)->rxsize = CONT_CMD_READ_PAK_RX;
            READFORMAT(ptr)->cmd = CONT_CMD_READ_PAK;
            READFORMAT(ptr)->datacrc = 0xFF;

            ptr[sizeof(__OSContRamReadFormat)] = CONT_CMD_END;
        } else {
            ptr += channel;
        }

        READFORMAT(ptr)->addrh = address >> 3;
        READFORMAT(ptr)->addrl = (u8)((address << 5) | __osContAddressCrc(address));


        ret = __osSiRawStartDma(OS_WRITE, &(*(OSPifRam *)__osPfsBuffer));
        osRecvMesg(mq, NULL, OS_MESG_BLOCK);

        ret = __osSiRawStartDma(OS_READ, &(*(OSPifRam *)__osPfsBuffer));
        osRecvMesg(mq, NULL, OS_MESG_BLOCK);

        ret = CHNL_ERR(*READFORMAT(ptr));

        if (!ret) {
            if (__osPfsDataChecksum(READFORMAT(ptr)->data) != READFORMAT(ptr)->datacrc) {
                ret = osContStartReadData(mq, channel);

                if (ret) {
                    break;
                } else {
                    ret = PFS_ERR_CONTRFAIL;
                }
            } else {
                bcopy(READFORMAT(ptr)->data, buffer, BLOCKSIZE);
            }
        } else {
            ret = PFS_ERR_NOPACK;
        }
    } while ((ret == PFS_ERR_CONTRFAIL) && (retry-- >= 0));
    __osSiRelAccess();
    return ret;
}

