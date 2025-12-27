/**
 * @file os_pfs.h
 * @brief N64 Controller Pak (PFS) filesystem definitions
 *
 * Based on Nintendo SDK headers and reference decomp projects.
 */

#ifndef _OS_PFS_H_
#define _OS_PFS_H_

#include "types.h"
#include "PR/os_message.h"

/**
 * PFS File System Structure
 *
 * Represents a Controller Pak filesystem instance.
 * Offsets verified against assembly:
 *   0x00: status
 *   0x04: queue
 *   0x08: channel
 *   0x0C-0x2B: id[32]
 *   0x2C-0x4B: label[32]
 *   0x4C: version
 *   0x50: dir_size
 *   0x54: inode_table
 *   0x58: minode_table
 *   0x5C: dir_table
 *   0x60: inode_start_page
 *   0x64: banks
 *   0x65: activebank
 */
typedef struct OSPfs {
    s32 status;              /* 0x00: Initialization status flags */
    OSMesgQueue *queue;      /* 0x04: Message queue for SI operations */
    s32 channel;             /* 0x08: Controller port (0-3) */
    u8 id[32];               /* 0x0C: Pack ID */
    u8 label[32];            /* 0x2C: User label */
    s32 version;             /* 0x4C: PFS version */
    s32 dir_size;            /* 0x50: Directory size */
    s32 inode_table;         /* 0x54: Inode table block location */
    s32 minode_table;        /* 0x58: Mirror inode table location */
    s32 dir_table;           /* 0x5C: Directory table block location */
    s32 inode_start_page;    /* 0x60: Starting page for inodes */
    u8 banks;                /* 0x64: Number of banks */
    u8 activebank;           /* 0x65: Currently active bank */
} OSPfs;

/**
 * PFS File State Structure
 *
 * Information about a file on the Controller Pak.
 */
typedef struct OSPfsState {
    u32 file_size;           /* File size in bytes */
    u32 game_code;           /* Game code */
    u16 company_code;        /* Company code */
    char ext_name[4];        /* File extension */
    char game_name[16];      /* Game name */
} OSPfsState;

/**
 * PFS Inode Unit
 *
 * Single inode entry in the inode table.
 */
typedef union __OSInodeUnit {
    struct {
        u8 bank;
        u8 page;
    } inode_t;
    u16 ipage;
} __OSInodeUnit;

/**
 * PFS Inode Table
 *
 * Full inode table for a bank (128 entries).
 */
typedef struct __OSInode {
    __OSInodeUnit inode_page[128];
} __OSInode;

/**
 * PFS Directory Entry
 *
 * Single directory entry on the Controller Pak.
 */
typedef struct __OSDir {
    u32 game_code;
    u16 company_code;
    __OSInodeUnit start_page;
    u8 status;
    u8 reserved;
    char game_name[16];
    char ext_name[4];
    u16 data_sum;
} __OSDir;

/* File System Constants */
#define OS_PFS_VERSION      0x0200
#define OS_PFS_VERSION_HI   (OS_PFS_VERSION >> 8)
#define OS_PFS_VERSION_LO   (OS_PFS_VERSION & 255)

#define PFS_FILE_NAME_LEN   16
#define PFS_FILE_EXT_LEN    4
#define PFS_BLOCKSIZE       32      /* bytes per block */
#define PFS_ONE_PAGE        8       /* blocks per page */
#define PFS_PAGE_SIZE       32      /* bytes per page (same as block) */
#define PFS_MAX_BANKS       62

/* Inode special values */
#define PFS_EOF             1       /* End of file chain */
#define PFS_PAGE_FREE       3       /* Free page marker */

/* Controller Pak Commands */
#define PFS_CMD_READ        2       /* Read page command */
#define PFS_CMD_WRITE       3       /* Write page command */

/* File System Flags */
#define PFS_READ            0
#define PFS_WRITE           1
#define PFS_CREATE          2

/* File System Status Flags */
#define PFS_INITIALIZED         0x01
#define PFS_CORRUPTED           0x02
#define PFS_ID_BROKEN           0x04
#define PFS_MOTOR_INITIALIZED   0x08
#define PFS_GBPAK_INITIALIZED   0x10

/* File System Error Codes */
#define PFS_ERR_SUCCESS         0   /* No error */
#define PFS_ERR_NOPACK          1   /* No memory card plugged in */
#define PFS_ERR_NEW_PACK        2   /* Memory card changed */
#define PFS_ERR_INCONSISTENT    3   /* Need to run checker */
#define PFS_ERR_CONTRFAIL       4   /* Controller overrun error */
#define PFS_ERR_INVALID         5   /* Invalid parameter or file not found */
#define PFS_ERR_BAD_DATA        6   /* Bad data read from pack */
#define PFS_ERR_FULL            7   /* No free pages */
#define PFS_ERR_DIR_FULL        8   /* No free directory entries */
#define PFS_ERR_EXIST           9   /* File already exists */
#define PFS_ERR_ID_FATAL        10  /* Dead memory card */
#define PFS_ERR_DEVICE          11  /* Wrong device type */

/* Function Prototypes */
s32 osPfsInitPak(OSMesgQueue *queue, OSPfs *pfs, s32 channel);
s32 osPfsChecker(OSPfs *pfs);
s32 osPfsRepairId(OSPfs *pfs);
s32 osPfsAllocateFile(OSPfs *pfs, u16 companyCode, u32 gameCode,
                      u8 *gameName, u8 *extName, s32 size, s32 *fileNo);
s32 osPfsFindFile(OSPfs *pfs, u16 companyCode, u32 gameCode,
                  u8 *gameName, u8 *extName, s32 *fileNo);
s32 osPfsDeleteFile(OSPfs *pfs, u16 companyCode, u32 gameCode,
                    u8 *gameName, u8 *extName);
s32 osPfsReadWriteFile(OSPfs *pfs, s32 fileNo, u8 flag, s32 offset,
                       s32 size, u8 *data);
s32 osPfsFileState(OSPfs *pfs, s32 fileNo, OSPfsState *state);
s32 osPfsGetLabel(OSPfs *pfs, u8 *label, s32 *length);
s32 osPfsSetLabel(OSPfs *pfs, u8 *label);
s32 osPfsFreeBlocks(OSPfs *pfs, s32 *freeBytes);
s32 osPfsNumFiles(OSPfs *pfs, s32 *maxFiles, s32 *usedFiles);

/* Internal PFS Functions */
s32 __osPfsSelectBank(OSPfs *pfs, u8 bank);
s32 __osPfsRWInode(OSPfs *pfs, __OSInode *inode, u8 flag, u8 bank);
s32 osPfsAllocate(OSPfs *pfs, s32 pages);
s32 osPfsReAllocate(OSPfs *pfs, s32 pages);

#endif /* _OS_PFS_H_ */
