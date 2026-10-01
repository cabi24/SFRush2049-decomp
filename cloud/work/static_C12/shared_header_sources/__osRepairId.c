/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
#include "rom_tu.h"
#include "context.h"
s32 __osRepairId(OSPfs* pfs, __OSPackId* badid, __OSPackId* newid) {
    s32 ret = 0;
    u8 temp[BLOCKSIZE];
    u8 comp[BLOCKSIZE];
    u8 mask = 0;
    int i;
    int j;
    u16 index[4];

    j = 0;

    newid->repaired = -1;
    newid->random = osGetCount();
    newid->serial_mid = badid->serial_mid;
    newid->serial_low = badid->serial_low;

    SET_ACTIVEBANK_TO_ZERO();
    do {
        ERRCK(SELECT_BANK(pfs, j));
        ERRCK(__osContRamRead(pfs->queue, pfs->channel, 0, temp));

        temp[0] = j | 0x80;

        for (i = 1; i < BLOCKSIZE; i++) {
            temp[i] = ~temp[i];
        }

        ERRCK(__osContRamWrite(pfs->queue, pfs->channel, 0, temp, FALSE));
        ERRCK(__osContRamRead(pfs->queue, pfs->channel, 0, comp));

        for (i = 0; i < BLOCKSIZE; i++) {
            if (comp[i] != temp[i]) {
                break;
            }
        }

        if (i != BLOCKSIZE) {
            break;
        }

        if (j > 0) {
            ERRCK(SELECT_BANK(pfs, 0));
            ERRCK(__osContRamRead(pfs->queue, pfs->channel, 0, (u8*)temp));

            if (temp[0] != 0x80) {
                break;
            }
        }

        j++;
    } while (j < PFS_MAX_BANKS);

    SET_ACTIVEBANK_TO_ZERO();

    mask = (j > 0) ? 1 : 0;

    newid->deviceid = (badid->deviceid & (u16)~1) | mask;
    newid->banks = j;
    newid->version = badid->version;
    __osIdCheckSum((u16*)newid, &newid->checksum, &newid->inverted_checksum);
    index[0] = PFS_ID_0AREA;
    index[1] = PFS_ID_1AREA;
    index[2] = PFS_ID_2AREA;
    index[3] = PFS_ID_3AREA;

    for (i = 0; i < ARRLEN(index); i++) {
        ERRCK(__osContRamWrite(pfs->queue, pfs->channel, index[i], (u8*)newid, TRUE));
    }

    ERRCK(__osContRamRead(pfs->queue, pfs->channel, PFS_ID_0AREA, (u8*)temp));

    for (i = 0; i < BLOCKSIZE; i++) {
        if (temp[i] != ((u8*)newid)[i]) {
            return PFS_ERR_DEVICE;
        }
    }
    return 0;
}
