#include "PRinternal/macros.h"
#include "PR/os_internal.h"
#include "PR/os_version.h"
#include "PRinternal/controller.h"
#include "PR/rmon.h"

#if BUILD_VERSION >= VERSION_J
__OSInode __osContPifInode ALIGNED(0x8);
s32 __osSiLastChannel = -1;
u8 __osSiChannelMask = 250;
#endif







s32 __osPfsRWInode(OSPfs* pfs, __OSInode* inode, u8 flag, u8 bank) {
    u8 sum;
    int j;
    s32 ret;
    int offset;
    u8* addr;

#if BUILD_VERSION >= VERSION_J
    if (flag == PFS_READ && bank == __osSiChannelMask && (pfs->channel == __osSiLastChannel)) {
        bcopy(&__osContPifInode, inode, sizeof(__OSInode));
        return 0;
    }
#endif

    SET_ACTIVEBANK_TO_ZERO();

    offset = (bank > 0) ? 1 : pfs->inode_start_page;

    if (flag == PFS_WRITE) {
        inode->inode_page[0].inode_t.page =
            __osSumcalc((u8*)&inode->inode_page[offset], (PFS_INODE_SIZE_PER_PAGE - offset) * 2);
    }

    for (j = 0; j < PFS_ONE_PAGE; j++) {
        addr = ((u8*)inode->inode_page + j * BLOCKSIZE);

        if (flag == PFS_WRITE) {
            ret = __osContRamWrite(pfs->queue, pfs->channel, pfs->inode_table + bank * PFS_ONE_PAGE + j, addr, FALSE);
            ret = __osContRamWrite(pfs->queue, pfs->channel, pfs->minode_table + bank * PFS_ONE_PAGE + j, addr, FALSE);
        } else {
            ret = __osContRamRead(pfs->queue, pfs->channel, pfs->inode_table + bank * PFS_ONE_PAGE + j, addr);
        }

        if (ret != 0) {
            return ret;
        }
    }

    if (flag == PFS_READ) {
        sum = __osSumcalc((u8*)&inode->inode_page[offset], (PFS_INODE_SIZE_PER_PAGE - offset) * 2);
        if (sum != inode->inode_page[0].inode_t.page) {
            for (j = 0; j < PFS_ONE_PAGE; j++) {
                addr = ((u8*)inode->inode_page + j * BLOCKSIZE);
                ret = __osContRamRead(pfs->queue, pfs->channel, pfs->minode_table + bank * PFS_ONE_PAGE + j, addr);
            }

#if BUILD_VERSION >= VERSION_J
            sum = __osSumcalc((u8*)&inode->inode_page[offset], (PFS_INODE_SIZE_PER_PAGE - offset) * 2);
#endif

            if (sum != inode->inode_page[0].inode_t.page) {
                return PFS_ERR_INCONSISTENT;
            }

            for (j = 0; j < PFS_ONE_PAGE; j++) {
                addr = ((u8*)inode->inode_page + j * BLOCKSIZE);
                ret =
                    __osContRamWrite(pfs->queue, pfs->channel, pfs->inode_table + bank * PFS_ONE_PAGE + j, addr, FALSE);
            }
        }
#if BUILD_VERSION < VERSION_J
        else
        {
            for (j = 0; j < PFS_ONE_PAGE; j++)
            {
                addr = ((u8 *)inode->inode_page + j * 32);
                ret = __osContRamWrite(pfs->queue, pfs->channel, pfs->minode_table + bank * PFS_ONE_PAGE + j, addr, FALSE);
            }
        }
#endif
    }

#if BUILD_VERSION >= VERSION_J
    __osSiChannelMask = bank;
    bcopy(inode, &__osContPifInode, sizeof(__OSInode));
    __osSiLastChannel = pfs->channel;
#endif

    return 0;
}

// This was moved into it's own file in 2.0J
#if BUILD_VERSION < VERSION_J
#endif

#ifdef _DEBUG
#endif
