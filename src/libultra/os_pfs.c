/**
 * @file os_pfs.c
 * @brief Controller Pak (PFS) filesystem functions
 *
 * Decompiled from asm/us/A810.s (0x80009C10 - 0x8000B918)
 * Handles Controller Pak initialization, reading, and writing.
 *
 * The Controller Pak is a 32KB memory card with:
 * - 128 pages of 256 bytes each
 * - FAT-style file allocation table
 * - Inode table for directory entries
 */

#include "types.h"
#include "PR/os_pfs.h"

/* External libultra functions */
extern void __osSiGetAccess(void);
extern void __osSiRelAccess(void);
extern s32 osContStartReadData(OSMesgQueue *mq, s32 channel);
extern s32 __osContRamRead(OSMesgQueue *mq, s32 channel, u16 addr, u8 *data);
extern s32 __osContRamWrite(OSMesgQueue *mq, s32 channel, u16 addr, u8 *data, u8 flag);
extern void __osPfsGetLabel(u8 *label, u16 *companyCode, u16 *gameCode);
extern s32 __osPfsGetId(OSPfs *pfs, u8 *label, u8 *id);
extern s32 __osPfsLabelCheck(OSPfs *pfs, u8 *label);
extern void bcopy(void *src, void *dst, s32 len);

/* Forward declarations */
static s32 __osPfsGetStatus(OSPfs *pfs);

/**
 * osPfsInitPak - Initialize controller pak
 * (0x80009C10)
 *
 * @param mq      Message queue for SI operations
 * @param pfs     Pointer to OSPfs structure to initialize
 * @param channel Controller channel (0-3)
 * @return        0 on success, error code on failure
 */
s32 osPfsInitPak(OSMesgQueue *mq, OSPfs *pfs, s32 channel) {
    s32 ret;
    u16 companyCode;
    u16 gameCode;
    u8 labelBuf[32];
    u8 idBuf[32];
    u8 *labelPtr;

    /* Acquire SI access */
    __osSiGetAccess();
    ret = osContStartReadData(mq, channel);
    __osSiRelAccess();

    if (ret != 0) {
        return ret;
    }

    /* Initialize pfs structure */
    pfs->queue = mq;
    pfs->channel = channel;
    pfs->status = 0;
    pfs->activebank = 0;

    /* Get pak status */
    ret = __osPfsGetStatus(pfs);
    if (ret != 0) {
        return ret;
    }

    /* Select bank 0 */
    ret = __osPfsSelectBank(pfs, 0);
    if (ret != 0) {
        return ret;
    }

    /* Read label from page 1 */
    ret = __osContRamRead(pfs->queue, pfs->channel, 1, labelBuf);
    if (ret != 0) {
        return ret;
    }

    /* Parse label for company and game codes */
    __osPfsGetLabel(labelBuf, &companyCode, &gameCode);

    labelPtr = labelBuf;

    /* Check if label matches expected values */
    if (companyCode != *(u16 *)(labelBuf + 0x24) ||
        gameCode != *(u16 *)(labelBuf + 0x26)) {
        /* Label mismatch - try to read ID */
        ret = __osPfsLabelCheck(pfs, labelBuf);
        if (ret != 0) {
            pfs->status |= PFS_CORRUPTED;
            goto done;
        }
    }

    /* Check if initialized bit is set */
    if (!(labelBuf[0x20] & 1)) {
        /* Not initialized - get ID */
        ret = __osPfsGetId(pfs, labelBuf, idBuf);
        if (ret != 0) {
            if (ret == PFS_ERR_FULL) {
                pfs->status |= PFS_CORRUPTED;
            }
            return ret;
        }
        labelPtr = idBuf;

        if (!(idBuf[0x18] & 1)) {
            return PFS_ERR_NOPACK;
        }
    }

    /* Copy label to pfs structure */
    bcopy(labelPtr, pfs->label, 32);

    /* Parse label fields */
    {
        u8 numBanks = labelPtr[0x1A];
        s32 temp = numBanks * 8;
        s32 inodeStart = temp + 8;

        pfs->dir_size = (numBanks * 2) + 3;
        pfs->inode_table = inodeStart;
        pfs->minode_table = inodeStart + temp;
        pfs->banks = numBanks;
    }

    /* Read inode from page 7 */
    ret = __osContRamRead(pfs->queue, pfs->channel, 7, pfs->id);
    if (ret != 0) {
        return ret;
    }

    /* Run filesystem integrity check */
    ret = osPfsChecker(pfs);
    pfs->status |= PFS_INITIALIZED;

done:
    return ret;
}

/**
 * __osPfsGetStatus - Get controller pak status
 * (0x80009E18)
 *
 * @param pfs  PFS structure
 * @return     0 on success, error code on failure
 */
static s32 __osPfsGetStatus(OSPfs *pfs) {
    u8 statusBuf[32];
    s32 ret;
    s32 i;

    /* Read status page */
    ret = __osContRamRead(pfs->queue, pfs->channel, 0, statusBuf);
    if (ret != 0) {
        return ret;
    }

    /* Check for valid pak signature */
    for (i = 0; i < 32; i++) {
        if (statusBuf[i] != 0) {
            break;
        }
    }

    if (i == 32) {
        return PFS_ERR_NOPACK;
    }

    return 0;
}

/**
 * __osPfsSelectBank - Select controller pak bank
 * (0x8000E850)
 *
 * @param pfs  PFS structure
 * @param bank Bank number to select
 * @return     0 on success, error code on failure
 */
s32 __osPfsSelectBank(OSPfs *pfs, u8 bank) {
    u8 bankCmd[32];
    s32 ret;
    s32 i;

    if (pfs->activebank == bank) {
        return 0;
    }

    /* Build bank select command */
    for (i = 0; i < 32; i++) {
        bankCmd[i] = bank;
    }

    /* Write to bank select register (address 0x8000) */
    ret = __osContRamWrite(pfs->queue, pfs->channel, 0x8000 >> 5, bankCmd, 1);
    if (ret != 0) {
        return ret;
    }

    pfs->activebank = bank;
    return 0;
}

/**
 * osPfsReadWriteFile - Read or write a file on controller pak
 * (0x8000A970)
 *
 * @param pfs      PFS structure
 * @param fileNo   File number (0-15)
 * @param flag     0 = read, 1 = write
 * @param offset   Byte offset within file
 * @param size     Number of bytes to read/write
 * @param data     Data buffer
 * @return         0 on success, error code on failure
 */
s32 osPfsReadWriteFile(OSPfs *pfs, s32 fileNo, u8 flag, s32 offset, s32 size, u8 *data) {
    s32 ret;
    s32 page;
    s32 pageOffset;
    s32 bytesToCopy;
    u8 pageBuf[PFS_PAGE_SIZE];

    if (pfs == NULL || data == NULL) {
        return PFS_ERR_INVALID;
    }

    if (!(pfs->status & PFS_INITIALIZED)) {
        return PFS_ERR_INVALID;
    }

    if (fileNo < 0 || fileNo > 15) {
        return PFS_ERR_INVALID;
    }

    if (size <= 0) {
        return PFS_ERR_INVALID;
    }

    /* Calculate starting page and offset */
    page = pfs->minode_table + (offset / PFS_PAGE_SIZE);
    pageOffset = offset % PFS_PAGE_SIZE;

    while (size > 0) {
        /* Select appropriate bank */
        ret = __osPfsSelectBank(pfs, (u8)(page / 128));
        if (ret != 0) {
            return ret;
        }

        /* Read current page */
        ret = __osContRamRead(pfs->queue, pfs->channel, (u16)(page % 128), pageBuf);
        if (ret != 0) {
            return ret;
        }

        bytesToCopy = PFS_PAGE_SIZE - pageOffset;
        if (bytesToCopy > size) {
            bytesToCopy = size;
        }

        if (flag == PFS_READ) {
            /* Read operation */
            bcopy(pageBuf + pageOffset, data, bytesToCopy);
        } else {
            /* Write operation */
            bcopy(data, pageBuf + pageOffset, bytesToCopy);
            ret = __osContRamWrite(pfs->queue, pfs->channel, (u16)(page % 128), pageBuf, 0);
            if (ret != 0) {
                return ret;
            }
        }

        data += bytesToCopy;
        size -= bytesToCopy;
        page++;
        pageOffset = 0;
    }

    return 0;
}
