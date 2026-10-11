#include "PRinternal/macros.h"
#include "PR/os_internal.h"
#include "PR/rcp.h"
#include "PRinternal/controller.h"
#include "PRinternal/siint.h"

extern s32 __osContLastChannel;
extern OSPifRam __osPfsBuffer; extern u8 __osPfsRequestType; s32 osContStartReadData(OSMesgQueue*, int);

#define READFORMAT(ptr) ((__OSContRamReadFormat*)(ptr))

#if BUILD_VERSION >= VERSION_J
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
        ptr = (u8*)__osPfsBuffer.ramarray;

        if (__osPfsRequestType != CONT_CMD_WRITE_PAK || (u32)__osContLastChannel != channel) {
            __osPfsRequestType = CONT_CMD_WRITE_PAK;
            __osContLastChannel = channel;

            for (i = 0; i < channel; i++) { *ptr++ = CONT_CMD_REQUEST_STATUS; }

            __osPfsBuffer.pifstatus = CONT_CMD_EXE;

            READFORMAT(ptr)->dummy = CONT_CMD_NOP;
            READFORMAT(ptr)->txsize = CONT_CMD_WRITE_PAK_TX;
            READFORMAT(ptr)->rxsize = CONT_CMD_WRITE_PAK_RX;
            READFORMAT(ptr)->cmd = CONT_CMD_WRITE_PAK;
            READFORMAT(ptr)->datacrc = 0xFF;

            ptr[sizeof(__OSContRamReadFormat)] = CONT_CMD_END;
        } else {
            ptr += channel;
        }

#if BUILD_VERSION >= VERSION_J
        READFORMAT(ptr)->addrh = address >> 3;
        READFORMAT(ptr)->addrl = ((address << 5) | __osContAddressCrc(address));
#else
        READFORMAT(ptr)->address = (address << 0x5) | __osContAddressCrc(address);
#endif

        bcopy(buffer, READFORMAT(ptr)->data, BLOCKSIZE);

        ret = __osSiRawStartDma(OS_WRITE, &__osPfsBuffer);
        crc = __osContDataCrc(buffer);
        osRecvMesg(mq, NULL, OS_MESG_BLOCK);

        ret = __osSiRawStartDma(OS_READ, &__osPfsBuffer);
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
#else

static void __osPackRamWriteData(int channel, u16 address, u8 *buffer);

s32 __osContRamWrite(OSMesgQueue* mq, int channel, u16 address, u8* buffer, int force) {
    s32 ret = 0;
    s32 i;
    u8* ptr = (u8*)&__osPfsBuffer;
    __OSContRamReadFormat ramreadformat;
    s32 retry = 2;
    u8 crc;

    if ((force != TRUE) && (address < PFS_LABEL_AREA) && (address != 0)) {
        return 0;
    }

    __osSiGetAccess();
    __osPfsRequestType = CONT_CMD_WRITE_PAK;
    __osPackRamWriteData(channel, address, buffer);
    ret = __osSiRawStartDma(OS_WRITE, &__osPfsBuffer);
    osRecvMesg(mq, NULL, OS_MESG_BLOCK);

    do {
        ret = __osSiRawStartDma(OS_READ, &__osPfsBuffer);
        osRecvMesg(mq, NULL, OS_MESG_BLOCK);
        ptr = (u8*)&__osPfsBuffer;

        if (channel != 0) {
            for (i = 0; i < channel; i++) {
                ptr++;
            }
        }

        ramreadformat = *READFORMAT(ptr);

        ret = CHNL_ERR(ramreadformat);
        if (ret == 0) {
            if (__osContDataCrc(buffer) != ramreadformat.datacrc) {
                ret = osContStartReadData(mq, channel);
                
                if (ret != 0) {
                    __osSiRelAccess();
                    return ret;
                }
                
                ret = PFS_ERR_CONTRFAIL;
            }
        } else {
            ret = PFS_ERR_NOPACK;
        }
    } while ((ret == PFS_ERR_CONTRFAIL) && retry-- >= 0);

    __osSiRelAccess();

    return ret;
}

static void __osPackRamWriteData(int channel, u16 address, u8 *buffer) {
    u8 *ptr;
    __OSContRamReadFormat ramreadformat;
    int i;

    ptr = (u8 *)__osPfsBuffer.ramarray;
    __osPfsBuffer.pifstatus = CONT_CMD_EXE;
    ramreadformat.dummy = CONT_CMD_NOP;
    ramreadformat.txsize = CONT_CMD_WRITE_PAK_TX;
    ramreadformat.rxsize = CONT_CMD_WRITE_PAK_RX;
    ramreadformat.cmd = CONT_CMD_WRITE_PAK;
    ramreadformat.address = (address << 0x5) | __osContAddressCrc(address);
    ramreadformat.datacrc = CONT_CMD_NOP;
    
    for (i = 0; i < ARRLEN(ramreadformat.data); i++) {
        ramreadformat.data[i] = *buffer++;
    }
    
    if (channel != 0) {
        for (i = 0; i < channel; i++) { *ptr++ = CONT_CMD_REQUEST_STATUS; }
    }
    
    *(__OSContRamReadFormat *)ptr = ramreadformat;
    ptr += sizeof(__OSContRamReadFormat);
    ptr[0] = CONT_CMD_END;
}

#endif
