/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* osPfsRepairId at the slot labelled osMotorStop (static, boot 0x8000A2F0, 96 B, segment 0xab20 ->
 * src/rom/lib_ab20.c, converted). The C function is named osMotorStop because promote.py splices
 * by slot name; the code is ultralib osPfsRepairId (see RESULTS.md "labels").
 * Origin: reference/repos/ultralib (decompals e24c836) src/io/pfsrepairid.c, BUILD_VERSION >= VERSION_J body.
 * Project names: __osGetId (0x8000F124) is the callee, as in ultralib. */
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct OSPfs {
    s32 status;
    void *queue;
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
