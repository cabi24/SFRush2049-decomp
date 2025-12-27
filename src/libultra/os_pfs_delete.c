/**
 * @file os_pfs_delete.c
 * @brief Controller Pak file delete functions
 *
 * Decompiled from asm/us/B300.s
 */

#include "types.h"

/* External functions */
extern s32 osPfsFindFile(void *pfs, u16 companyCode, u32 gameCode,
                         u8 *gameName, u8 *extName, s32 *fileNo);
extern s32 func_8000E850(void *pfs, u8 fillByte);
extern s32 func_8000E8D0(s32 channel, s32 bank, u16 page, void *buffer);
extern s32 func_8000F3A4(void *pfs, u16 *buffer, s32 start, u8 bank);
extern s32 func_8000F680(s32 channel, s32 bank, u32 size, u8 *buffer, s32 flags);
extern void func_80008590(void *ptr, s32 size);

/* Forward declaration */
static s32 func_8000A8D8(void *pfs, u16 *buffer, u8 startPage, u8 bank, u16 *nextPage);

/**
 * Delete a file from Controller Pak
 * (func_8000A700 - osPfsDeleteFile)
 *
 * Deletes a file from the Controller Pak filesystem.
 *
 * @param pfs PFS handle
 * @param companyCode Company code of file
 * @param gameCode Game code of file
 * @param gameName Game name (can be NULL)
 * @param extName Extension name (can be NULL)
 * @return 0 on success, error code on failure
 */
s32 osPfsDeleteFile(void *pfs, u16 companyCode, u32 gameCode,
                    u8 *gameName, u8 *extName) {
    u8 *p = (u8 *)pfs;
    u8 buffer[0x28];
    u16 pageTable[0x80];
    u8 bank, page, nextBank, nextPage;
    s32 fileNo;
    s32 result;

    /* Validate parameters */
    if (companyCode == 0 || gameCode == 0) {
        return 5;
    }

    /* Find the file */
    result = osPfsFindFile(pfs, companyCode, gameCode, gameName, extName, &fileNo);
    if (result != 0) {
        return result;
    }

    /* Initialize if needed */
    if (*(u8 *)(p + 0x65) != 0) {
        result = func_8000E850(pfs, 0);
        if (result != 0) {
            return result;
        }
    }

    /* Read file directory entry */
    result = func_8000E8D0(
        *(s32 *)(p + 4),
        *(s32 *)(p + 8),
        *(s32 *)(p + 0x5C) + fileNo,
        buffer
    );
    if (result != 0) {
        return result;
    }

    /* Get starting page */
    bank = buffer[6];
    page = buffer[7];

    /* Free all pages in the file chain */
    while (bank < *(u8 *)(p + 0x64)) {
        /* Read page table for this bank */
        result = func_8000F3A4(pfs, pageTable, 0, bank);
        if (result != 0) {
            return result;
        }

        /* Mark page as free and get next */
        result = func_8000A8D8(pfs, pageTable, page, bank, (u16 *)(buffer + 4));
        if (result != 0) {
            return result;
        }

        /* Write updated page table */
        result = func_8000F3A4(pfs, pageTable, 1, bank);
        if (result != 0) {
            return result;
        }

        /* Check if end of chain */
        if (*(u16 *)(buffer + 4) == 1) {
            break;
        }

        /* Get next page location */
        bank = *(u8 *)(buffer + 4);
        page = *(u8 *)(buffer + 5);
    }

    /* Check for incomplete chain */
    if (bank >= *(u8 *)(p + 0x64)) {
        return 3;  /* Inconsistent */
    }

    /* Clear directory entry */
    func_80008590(buffer, 0x20);

    /* Write cleared entry back */
    return func_8000F680(
        *(s32 *)(p + 4),
        *(s32 *)(p + 8),
        *(s32 *)(p + 0x5C) + fileNo,
        buffer,
        0
    );
}

/**
 * Free a page in the page table
 * (func_8000A8D8)
 *
 * Helper function to mark a page as free and return the next page.
 */
static s32 func_8000A8D8(void *pfs, u16 *pageTable, u8 startPage, u8 bank, u16 *nextPage) {
    u8 *p = (u8 *)pfs;
    u16 currentPage;
    u16 temp;

    /* Build page index */
    currentPage = ((u16)bank << 8) | startPage;

    /* Follow chain until we reach boundary or return to start */
    do {
        temp = currentPage;
        currentPage = pageTable[currentPage & 0xFF];
        pageTable[temp & 0xFF] = 3;  /* Mark as free */
    } while (currentPage < *(s32 *)(p + 0x60) && bank != (currentPage & 0xFF));

    *nextPage = currentPage;
    return 0;
}
