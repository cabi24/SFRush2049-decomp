/* GENERATED ROM-aligned TU — segment 0xa810 (rom/lib_a810)
 * layout map d0906279f3a363c7867864e8022f124d57c55d485c021b2accf340401d72ea36; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-01 — osPfsInitPak
 * Source:   cloud/work/static_C9/osPfsInitPak.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C9/osPfsInitPak.c:osPfsInitPak (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 osPfsInitPak(OSMesgQueue* queue, OSPfs* pfs, int channel) {
    s32 ret = 0;
    u16 sum;
    u16 isum;
    u8 temp[BLOCKSIZE];
    __OSPackId* id;
    __OSPackId newid;

    __osSiGetAccess();

    ret = osContStartReadData(queue, channel);

    __osSiRelAccess();

    if (ret != 0) {
        return ret;
    }

    pfs->queue = queue;
    pfs->channel = channel;
    pfs->status = 0;

    ERRCK(__osTimerInterrupt(pfs));
    ERRCK(SELECT_BANK(pfs, 0));
    ERRCK(__osContRamRead(pfs->queue, pfs->channel, PFS_ID_0AREA, temp));

    __osIdCheckSum((u16*)temp, &sum, &isum);
    id = (__OSPackId*)temp;

    if ((id->checksum != sum) || (id->inverted_checksum != isum)) {
        ret = __osCheckId(pfs, id);

        if (ret != 0) {
            pfs->status |= PFS_ID_BROKEN;
            return ret;
        }
        
    }

    if (!(id->deviceid & 1)) {
        ret = __osRepairId(pfs, id, &newid);

        if (ret != 0) {
            if (ret == PFS_ERR_ID_FATAL) {
                pfs->status |= PFS_ID_BROKEN;
            }
            return ret;
        }

        id = &newid;

        if (!(id->deviceid & 1)) {
            return PFS_ERR_DEVICE;
        }
    }

    bcopy(id, pfs->id, BLOCKSIZE);

    pfs->version = id->version;
    pfs->banks = id->banks;
    pfs->inode_start_page = 1 + DEF_DIR_PAGES + (2 * pfs->banks);
    pfs->dir_size = DEF_DIR_PAGES * PFS_ONE_PAGE;
    pfs->inode_table = 1 * PFS_ONE_PAGE;
    pfs->minode_table = (1 + pfs->banks) * PFS_ONE_PAGE;
    pfs->dir_table = pfs->minode_table + (pfs->banks * PFS_ONE_PAGE);

    ERRCK(__osContRamRead(pfs->queue, pfs->channel, PFS_LABEL_AREA, pfs->label));

    ret = osPfsChecker(pfs);
    pfs->status |= PFS_INITIALIZED;

    return ret;
}

/* PROMOTED 2026-10-01 — __osTimerInterrupt
 * Source:   cloud/work/static_C10/__osTimerInterrupt.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C10/__osTimerInterrupt.c:__osTimerInterrupt (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 __osTimerInterrupt(OSPfs* pfs) {
    s32 i;
    s32 ret = 0;
    u8 temp1[BLOCKSIZE];
    u8 temp2[BLOCKSIZE];
    u8 save[BLOCKSIZE];

    ERRCK(SELECT_BANK(pfs, PFS_ID_BANK_256K));
    ERRCK(__osContRamRead(pfs->queue, pfs->channel, 0, save));

    for (i = 0; i < BLOCKSIZE; i++) {
        temp1[i] = i;
    }

    ERRCK(__osContRamWrite(pfs->queue, pfs->channel, 0, temp1, FALSE));
    ERRCK(__osContRamRead(pfs->queue, pfs->channel, 0, temp2));

    if (bcmp(temp1, temp2, BLOCKSIZE) != 0) {
        return PFS_ERR_DEVICE;
    }

    ret = __osContRamWrite(pfs->queue, pfs->channel, 0, save, FALSE);
    return ret;
}

