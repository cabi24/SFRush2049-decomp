/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* __osPfsRWInode (static, boot 0x8000F3A4, 732 B, segment 0xf700 -> src/rom/lib_f700.c)
 * Origin: reference/repos/ultralib (decompals e24c836) src/io/contpfs.c,
 *         BUILD_VERSION >= VERSION_J branch (inode cache fast path, single re-sum).
 * Project names: the J+ inode-cache globals are this project's
 *   __osContPifInode  (0x80037CB0, ultralib __osPfsInodeCache)
 *   __osSiChannelMask (0x8002C4D4, ultralib __osPfsInodeCacheBank)
 *   __osSiLastChannel (0x8002C4D0, ultralib __osPfsInodeCacheChannel)
 * They are extern here: the TU must not define data (ROM layout). */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef void *OSMesg;
typedef struct OSMesgQueue_s {
    void *mtqueue;
    void *fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    OSMesg *msg;
} OSMesgQueue;
typedef struct OSPfs {
    s32 status;
    OSMesgQueue *queue;
    s32 channel;
    u8 id[32];
    u8 label[32];
    s32 version;
    s32 dir_size;
    s32 inode_table;
    s32 minode_table;
    s32 dir_table;
    s32 inode_start_page;
    u8 banks;
    u8 activebank;
} OSPfs;
typedef union __OSInodeUnit {
    struct {
        u8 bank;
        u8 page;
    } inode_t;
    u16 ipage;
} __OSInodeUnit;
typedef struct __OSInode {
    __OSInodeUnit inode_page[128];
} __OSInode;

#define FALSE 0
#define PFS_READ 0
#define PFS_WRITE 1
#define PFS_ONE_PAGE 8
#define BLOCKSIZE 32
#define PFS_INODE_SIZE_PER_PAGE 128
#define PFS_ERR_INCONSISTENT 3
#define ERRCK(fn) ret=fn; if(ret!=0) return ret
#define SET_ACTIVEBANK_TO_ZERO() if(pfs->activebank!=0) { ERRCK(__osPfsSelectBank(pfs,0)); } (void)0

extern void bcopy(const void *src, void *dst, s32 len);
extern u16 __osSumcalc(u8 *ptr, int length);
extern s32 __osPfsSelectBank(OSPfs *pfs, u8 bank);
extern s32 __osContRamWrite(OSMesgQueue *mq, s32 channel, u16 address, u8 *buffer, s32 force);
extern s32 __osContRamRead(OSMesgQueue *mq, s32 channel, u16 address, u8 *buffer);
extern __OSInode __osContPifInode;
extern s32 __osSiLastChannel;
extern u8 __osSiChannelMask;

s32 __osPfsRWInode(OSPfs* pfs, __OSInode* inode, u8 flag, u8 bank) {
    u8 sum;
    int j;
    s32 ret;
    int offset;
    u8* addr;

    if (flag == PFS_READ && bank == __osSiChannelMask && (pfs->channel == __osSiLastChannel)) {
        bcopy(&__osContPifInode, inode, sizeof(__OSInode));
        return 0;
    }

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

            sum = __osSumcalc((u8*)&inode->inode_page[offset], (PFS_INODE_SIZE_PER_PAGE - offset) * 2);

            if (sum != inode->inode_page[0].inode_t.page) {
                return PFS_ERR_INCONSISTENT;
            }

            for (j = 0; j < PFS_ONE_PAGE; j++) {
                addr = ((u8*)inode->inode_page + j * BLOCKSIZE);
                ret =
                    __osContRamWrite(pfs->queue, pfs->channel, pfs->inode_table + bank * PFS_ONE_PAGE + j, addr, FALSE);
            }
        }
    }

    __osSiChannelMask = bank;
    bcopy(inode, &__osContPifInode, sizeof(__OSInode));
    __osSiLastChannel = pfs->channel;

    return 0;
}
