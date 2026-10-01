/* GENERATED ROM-aligned TU — segment 0xf700 (rom/lib_f700)
 * layout map 4f66863fb5ebae8c13b1a88ee16b3303b8bf374da66df1a42ffd8d8012d9048c; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"

/* PROMOTED 2026-10-01 — __osSumcalc
 * Source:   cloud/work/static_C5/__osSumcalc.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C5/__osSumcalc.c:__osSumcalc (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
u16 __osSumcalc(u8 *ptr, int length) { int i; u32 sum = 0; u8 *tmp = ptr; for(i=0;i<length;i++) { sum += *tmp++; } return sum & 0xFFFF; }

/* PROMOTED 2026-07-11 — __osIdCheckSum
 * Source:   src/util/checksum.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:src/util/checksum.c:__osIdCheckSum (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 __osIdCheckSum(u16 *ptr, u16 *csum, u16 *icsum) {
    u16 data = 0;
    u32 j;

    *csum = *icsum = 0;

    for (j = 0; j < 28; j += 2) {
        data = *(u16 *)((u32)ptr + j);
        *csum += data;
        *icsum += ~data;
    }

    return 0;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_f700/__osRepairId.s")
/* PROMOTED 2026-10-01 — __osCheckId
 * Source:   cloud/work/static_C9/__osCheckId.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C9/__osCheckId.c:__osCheckId (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 __osCheckId(OSPfs* pfs, __OSPackId* temp) {
    u16 index[4];
    s32 ret = 0;
    u16 sum;
    u16 isum;
    int i;
    int j;

    SET_ACTIVEBANK_TO_ZERO();
    index[0] = PFS_ID_0AREA;
    index[1] = PFS_ID_1AREA;
    index[2] = PFS_ID_2AREA;
    index[3] = PFS_ID_3AREA;
    for (i = 1; i < ARRLEN(index); i++) {
        ERRCK(__osContRamRead(pfs->queue, pfs->channel, index[i], (u8*)temp));
        __osIdCheckSum((u16*)temp, &sum, &isum);
        if (temp->checksum == sum && temp->inverted_checksum == isum) {
            break;
        }
    }

    if (i == ARRLEN(index)) {
        return PFS_ERR_ID_FATAL;
    }

    for (j = 0; j < ARRLEN(index); j++) {
        if (j != i) {
            ERRCK(__osContRamWrite(pfs->queue, pfs->channel, index[j], (u8*)temp, TRUE));
        }
    }

    return 0;
}

/* PROMOTED 2026-10-01 — __osGetId
 * Source:   cloud/work/static_C9/__osGetId.c (in-repo, locked)
 * Flags:    -g0 -O2 -mips2 -G 0 -non_shared
 * Evidence: lock:cloud/work/static_C9/__osGetId.c:__osGetId (score0)
 * Gate:     full-ROM SHA-1 (promotion transaction)
 */
s32 __osGetId(OSPfs* pfs) {
    u16 sum;
    u16 isum;
    u8 temp[BLOCKSIZE];
    __OSPackId newid;
    s32 ret;
    __OSPackId* id;

    SET_ACTIVEBANK_TO_ZERO();
    ERRCK(__osContRamRead(pfs->queue, pfs->channel, PFS_ID_0AREA, (u8*)temp));
    __osIdCheckSum((u16*)temp, &sum, &isum);
    id = (__OSPackId*)temp;

    if (id->checksum != sum || id->inverted_checksum != isum) {
        ret = __osCheckId(pfs, id);

        if (ret == PFS_ERR_ID_FATAL) {
            ERRCK(__osRepairId(pfs, id, &newid));
            id = &newid;
        } else if (ret != 0) {
            return ret;
        }
    }

    if ((id->deviceid & 1) == 0) {
        ERRCK(__osRepairId(pfs, id, &newid));
        id = &newid;

        if ((id->deviceid & 1) == 0) {
            return PFS_ERR_DEVICE;
        }
    }

    bcopy(id, pfs->id, BLOCKSIZE);
    pfs->version = id->version;
    pfs->banks = id->banks;
    pfs->inode_start_page = 1 + DEF_DIR_PAGES + (2 * pfs->banks);
    pfs->dir_size = 16;
    pfs->inode_table = PFS_ONE_PAGE;
    pfs->minode_table = (1 + pfs->banks) * PFS_ONE_PAGE;
    pfs->dir_table = pfs->minode_table + pfs->banks * PFS_ONE_PAGE;
    ERRCK(__osContRamRead(pfs->queue, pfs->channel, PFS_LABEL_AREA, pfs->label));
    return 0;
}

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_f700/osPfsReadWriteFile_pages.s")
#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_f700/__osPfsRWInode.s")
