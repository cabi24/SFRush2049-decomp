/* GENERATED ROM-aligned TU — segment 0xab20 (rom/lib_ab20)
 * layout map 89270b21cd87ba7878fc8bf7d36850499567d9ed2989229b5e196a9dcdf1e0e0; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-01 — osMotorInit
 * Source:   cloud/work/static_C9/osMotorInit.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C9/osMotorInit.c:osMotorInit (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 osMotorInit(OSPfs* pfs, s32 flag) {
    int i;
    s32 ret;
    u8* ptr = (u8*)&((OSPifRam *)__osMotorPifBuf)[pfs->channel];

    if (!(pfs->status & PFS_MOTOR_INITIALIZED)) {
        return 5;
    }

    __osSiGetAccess();
    ((OSPifRam *)__osMotorPifBuf)[pfs->channel].pifstatus = CONT_CMD_EXE;
    ptr += pfs->channel;

    for (i = 0; i < BLOCKSIZE; i++) {
        READFORMAT(ptr)->data[i] = flag;
    }

    __osPfsRequestType = CONT_CMD_END;
    __osSiRawStartDma(OS_WRITE, &((OSPifRam *)__osMotorPifBuf)[pfs->channel]);
    osRecvMesg(pfs->queue, NULL, OS_MESG_BLOCK);
    ret = __osSiRawStartDma(OS_READ, &((OSPifRam *)__osMotorPifBuf)[pfs->channel]);
    osRecvMesg(pfs->queue, NULL, OS_MESG_BLOCK);

    ret = READFORMAT(ptr)->rxsize & CHNL_ERR_MASK;
    if (!ret) {
        if (!flag) {
            if (READFORMAT(ptr)->datacrc != 0) {
                ret = PFS_ERR_CONTRFAIL;
            }
        } else {
            if (READFORMAT(ptr)->datacrc != 0xEB) {
                ret = PFS_ERR_CONTRFAIL;
            }
        }
    }

    __osSiRelAccess();

    return ret;
}

/* PROMOTED 2026-10-01 — __osMotorAccess
 * Source:   cloud/work/static_C9/__osMotorAccess.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C9/__osMotorAccess.c:__osMotorAccess (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
void __osMotorAccess(int channel, OSPifRam* mdata) {
    u8* ptr = (u8*)mdata->ramarray;
    __OSContRamReadFormat ramreadformat;
    int i;

    ramreadformat.dummy = CONT_CMD_NOP;
    ramreadformat.txsize = CONT_CMD_WRITE_PAK_TX;
    ramreadformat.rxsize = CONT_CMD_WRITE_PAK_RX;
    ramreadformat.cmd = CONT_CMD_WRITE_PAK;
    ramreadformat.addrh = CONT_BLOCK_RUMBLE >> 3;
    ramreadformat.addrl = (u8)(__osContAddressCrc(CONT_BLOCK_RUMBLE) | (CONT_BLOCK_RUMBLE << 5));

    if (channel != 0) {
        for (i = 0; i < channel; i++) {
            *ptr++ = CONT_CMD_REQUEST_STATUS;
        }
    }

    *READFORMAT(ptr) = ramreadformat;
    ptr += sizeof(__OSContRamReadFormat);
    ptr[0] = CONT_CMD_END;
}

/* PROMOTED 2026-10-01 — osMotorStart
 * Source:   cloud/work/static_C10/osMotorStart.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C10/osMotorStart.c:osMotorStart (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 osMotorStart(OSMesgQueue* mq, OSPfs* pfs, int channel) {
    s32 ret;
    u8 temp[32];

    pfs->queue = mq;
    pfs->channel = channel;
    pfs->activebank = 0xFF;
    pfs->status = 0;

    ret = SELECT_BANK(pfs, 0xFE);

    if (ret == PFS_ERR_NEW_PACK) {
        ret = SELECT_BANK(pfs, 0x80);
    }

    if (ret != 0) {
        return ret;
    }

    ret = __osContRamRead(mq, channel, CONT_BLOCK_DETECT, temp);

    if (ret == PFS_ERR_NEW_PACK) {
        ret = PFS_ERR_CONTRFAIL;
    }

    if (ret != 0) {
        return ret;
    } else if (temp[31] == 254) {
        return PFS_ERR_DEVICE;
    }

    ret = SELECT_BANK(pfs, 0x80);
    if (ret == PFS_ERR_NEW_PACK) {
        ret = PFS_ERR_CONTRFAIL;
    }

    if (ret != 0) {
        return ret;
    }

    ret = __osContRamRead(mq, channel, CONT_BLOCK_DETECT, temp);
    if (ret == PFS_ERR_NEW_PACK) {
        ret = PFS_ERR_CONTRFAIL;
    }

    if (ret != 0) {
        return ret;
    } else if (temp[31] != 0x80) {
        return PFS_ERR_DEVICE;
    }

    if (!(pfs->status & PFS_MOTOR_INITIALIZED)) {
        __osMotorAccess(channel, &((OSPifRam *)__osMotorPifBuf)[channel]);
    }

    pfs->status = PFS_MOTOR_INITIALIZED;
    return 0;
}

/* PROMOTED 2026-10-11 — osMotorStop
 * Source:   cloud/work/frontier/w16g/sources/osMotorStop.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/frontier/w16g/sources/osMotorStop.c:osMotorStop (score0); rom_tu.h context: cloud/work/frontier/w16g/ctx/ctx_O2.jsonl
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
#define PFS_INITIALIZED 0x1
#define PFS_ID_BROKEN 0x4
#define PFS_ERR_INVALID 5
extern s32 __osGetId(OSPfs *);
s32 osMotorStop(OSPfs* pfs) {
    s32 ret = 0;

    if ((pfs->status & (PFS_INITIALIZED | PFS_ID_BROKEN))) {
        ret = __osGetId(pfs);

        if (ret == 0) {
            pfs->status &= ~(PFS_ID_BROKEN);
        }
    } else {
        ret = PFS_ERR_INVALID;
    }

    return ret;
}

