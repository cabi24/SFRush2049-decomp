/**
 * @file os_pfs_state.c
 * @brief Controller Pak file state functions
 *
 * Decompiled from asm/us/B120.s
 */

#include "types.h"

/* External functions */
extern s32 func_8000F2D0(void *pfs);
extern s32 func_8000E850(void *pfs, u8 fillByte);
extern s32 func_8000E8D0(s32 channel, s32 bank, u16 page, void *buffer);
extern s32 func_8000F3A4(void *pfs, u16 *buffer, s32 start, u8 bank);
extern s32 func_8000E620(s32 channel, s32 bank);
extern void func_8000D2B0(void *src, void *dst, s32 len);

/**
 * Get file state/info from Controller Pak
 * (func_8000A520 - osPfsFileState)
 *
 * Retrieves information about a file on the Controller Pak.
 *
 * @param pfs PFS handle
 * @param fileNo File number
 * @param state Output: file state structure
 * @return 0 on success, error code on failure
 */
s32 osPfsFileState(void *pfs, s32 fileNo, void *state) {
    u8 *p = (u8 *)pfs;
    u8 buffer[0x100];
    u16 pageTable[0x80];
    u8 lastBank;
    u8 bank;
    u8 page;
    s32 pages;
    s32 result;

    /* Validate file number */
    if (fileNo >= *(s32 *)(p + 0x50) || fileNo < 0) {
        return 5;
    }

    /* Check if initialized */
    if ((*(u32 *)p & 1) == 0) {
        return 5;
    }

    /* Check PFS status */
    result = func_8000F2D0(pfs);
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

    /* Check if valid entry */
    if (*(u16 *)(buffer + 4) == 0 && *(u32 *)buffer == 0) {
        return 5;
    }

    /* Count pages used by file */
    bank = buffer[6];
    page = buffer[7];
    lastBank = 0xFF;
    pages = 0;

    while (*(u16 *)(buffer + 0x34) < *(s32 *)(p + 0x60)) {
        if (lastBank != bank) {
            lastBank = bank;
            result = func_8000F3A4(pfs, pageTable, 0, bank);
            if (result != 0) {
                return result;
            }
        }
        pages++;
        page = pageTable[page];
        bank = buffer[0x34];
    }

    /* Check if valid end */
    if (*(u16 *)(buffer + 0x34) != 1) {
        return 3;  /* Inconsistent */
    }

    /* Fill output structure */
    *(s32 *)state = pages << 8;
    *(u16 *)((u8 *)state + 8) = *(u16 *)(buffer + 4);
    *(u32 *)((u8 *)state + 4) = *(u32 *)buffer;
    func_8000D2B0(buffer + 0x10, (u8 *)state + 0xE, 16);
    func_8000D2B0(buffer + 0x0C, (u8 *)state + 0xA, 4);

    return func_8000E620(*(s32 *)(p + 4), *(s32 *)(p + 8));
}
