typedef signed char s8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef void *OSMesg;
typedef struct OSThread OSThread;
typedef struct OSMesgQueue {
    OSThread *mtqueue, *fullqueue;
    s32 validCount, first, msgCount;
    OSMesg *msg;
} OSMesgQueue;
typedef struct OSPfs {
    s32 status;
    OSMesgQueue *queue;
    s32 channel;
    u8 id[32], label[32];
    s32 version, dir_size, inode_table, minode_table, dir_table, inode_start_page;
    u8 banks, activebank;
} OSPfs;
typedef struct OSPfsState {
    u32 file_size, game_code;
    u16 company_code;
    u8 ext_name[4], game_name[16];
} OSPfsState;
/* one Controller Pak file slot (40 bytes; offsets from retail) */
typedef struct PakFile {
    s8 flush, flag1, flag2, opaque3;
    u16 mapBytes, opaque6;
    OSPfsState state;
} PakFile;
/* gPaks[pak] (GC anon_changed_inserted_25c; N64 772 bytes) */
typedef struct PakRecord {
    u8 changed;
    s8 inserted, opaque2, opaque3, opaque4, damaged, opaque6, flushing, flushed, opaque9, opaque10, flush;
    OSPfs pfs;
    s32 error, FreeSpace;
    u8 rumble124, rumble125, rumble126, opaque127;
    s16 rumble128;
    u8 opaque130[2];
    PakFile files[16];
} PakRecord;
typedef struct tPS_File tPS_File;
typedef struct PakHandle { tPS_File *file; } PakHandle;
/* GC tPS_File (N64 76 bytes): ll, RemoveFile, MoveFile, pak, file number, name/ext, size, blocks, data */
struct tPS_File {
    PakHandle *next;
    u32 prev;
    void (*RemoveFile)(PakHandle *);
    void (*MoveFile)(PakHandle *, PakHandle *);
    u8 pak, fileNo;
    u8 name[35], ext[11];
    u32 size, blocks;
    u8 **data;
};
typedef struct tDLList {
    u8 indirect, doubly, opaque2[2];
    u32 count;
    PakHandle *head, *tail;
} tDLList;
typedef s32 (*PakPredicate)(s32);
typedef void (*PakErrorFunc)(s32, s32, u32, s32, s8 *, s8 *, PakPredicate);

extern volatile PakRecord D_80144030[];   /* gPaks */
extern tDLList D_80144D60[];               /* gPS_Files */
extern tDLList D_801460E0;                 /* free tPS_File handles */
extern s8 D_8011194C, D_8011EAE8;           /* lock queue initialised; gErrPak */
extern OSMesgQueue D_801497D0;
extern OSMesg D_801527E4;
extern PakErrorFunc D_80144008;            /* gErrorFunc */
extern void osCreateMesgQueue(OSMesgQueue *, OSMesg *, s32);
extern s32 osSendMesg(OSMesgQueue *, OSMesg, s32);
extern s32 osRecvMesg(OSMesgQueue *, OSMesg *, s32);
extern s32 osPfsAllocateFile(OSPfs *, u16, u32, u8 *, u8 *, s32, s32 *);
extern s32 osPfsFileState(OSPfs *, s32, OSPfsState *);
extern s32 osPfsFreeBlocks(OSPfs *, s32 *);
extern void func_800A150C(u8 *, u8 *, u8);          /* ASCII -> pak font */
extern void func_800A1644(u8 *, u8 *, u8);          /* pak font -> ASCII */
extern s32 func_8008AD04(u8 *, u8 *);               /* strcmp */
extern s32 func_8008A6A4(s32);                      /* !gPaks[pak].inserted */
extern void pak_delete_file_retry(PakHandle *);     /* PS_DeleteFile */
extern void func_8009211C(tDLList *, PakHandle *);  /* RemoveLLE */
extern void func_80091FBC(tDLList *, PakHandle *, PakHandle *);  /* InsertLLE */
extern void *audio_task_complete(void *, u32);      /* AllocateHandleHeap */
extern void *memset(void *, s32, u32);
void no_catchup(PakHandle *fileh);
