/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* Required SDK context: include/types.h, include/PR/os_message.h,
 * include/PR/os_pfs.h, include/PR/os.h and src/rom/rom_tu.h at the stated base. */
typedef signed char s8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef struct OSThread_s OSThread;
typedef void *OSMesg;
typedef struct OSMesgQueue_s {
    OSThread *mtqueue, *fullqueue;
    s32 validCount, first, msgCount;
    OSMesg *msg;
} OSMesgQueue;
typedef struct OSPfs {
    s32 status;
    OSMesgQueue *queue;
    s32 channel;
    u8 id[32], label[32];
    s32 version, dir_size, inode_table, minode_table, dir_table;
    s32 inode_start_page;
    u8 banks, activebank;
} OSPfs;
extern void osCreateMesgQueue(OSMesgQueue *, OSMesg *, s32);
extern s32 osRecvMesg(OSMesgQueue *, OSMesg *, s32);
extern s32 osSendMesg(OSMesgQueue *, OSMesg, s32);
extern void osWritebackDCache(void *, s32);
extern s32 osPfsReadWriteFile(OSPfs *, s32, u8, int, int, u8 *);

typedef struct C96File C96File;
typedef struct C96Handle { C96File *file; } C96Handle;
struct C96File {
    C96Handle *next;
    u8 unknown04[12];
    u8 channel, slot;
    u8 unknown12[46];
    u32 data_size;
    u8 unknown44[4];
    u8 **data;
};
typedef struct C96Slot {
    u8 unknown0;
    s8 modified;
    s8 flag2;
    u8 unknown3;
    u16 map_bytes;
    u16 unknown6;
    u32 allocation;
    u8 unknown12[28];
} C96Slot;
typedef struct C96Channel {
    u8 unknown0;
    s8 state;
    u8 unknown2[4];
    s8 flag6, busy, result, flag9;
    u8 unknown10;
    s8 modified;
    OSPfs pfs;
    s32 error;
    u8 unknown120[12];
    C96Slot slots[16];
} C96Channel;
typedef struct C96List { C96Handle *head; u32 unknown[3]; } C96List;
extern volatile C96Channel D_80144030[];
extern C96List D_80144D68[];
extern OSMesgQueue D_801497D0;
extern OSMesg D_801527E4;
extern s8 D_8011194C, D_8011EAE8;
extern void (*D_80144008)(s32,s32,s32,s32,s8 *,s8 *,s32);
extern s32 func_800A1E94(s32);
void pfs_queue_init(void) {
    if (!D_8011194C) {
        D_8011194C = 1; osCreateMesgQueue(&D_801497D0, &D_801527E4, 1);
        osSendMesg(&D_801497D0, 0, 0);
    }
}
void pfs_lock(void) {
    OSMesg message;
    pfs_queue_init();
    osRecvMesg(&D_801497D0, &message, 1);
}
void pfs_unlock(void) {
    osSendMesg(&D_801497D0, 0, 0);
}
