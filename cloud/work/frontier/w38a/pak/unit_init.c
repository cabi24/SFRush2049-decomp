/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
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
    u8 motorOn, request, level, opaque127;   /* rumble thread state (frontier_rumble_thread) */
    s16 phase;
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
void func_800A3724(u8 pak);
/* ---- PS_PakServer and its private callees ---- */
typedef struct ControllerData { s8 present; u8 opaque1[15]; } ControllerData;
extern ControllerData D_80156CF0[];     /* gControllerData */
extern s8 D_8011EAE0;                   /* gInited */
extern OSMesgQueue D_80035458;          /* SI message queue */
extern OSMesgQueue D_80152770;          /* heap semaphore */
extern void func_8008A704(void);        /* pak lock (public; the first call stays a jal: umerge budget) */
extern s32 osPfsInitPak(OSMesgQueue *, OSPfs *, s32);
extern s32 osMotorInit(OSMesgQueue *, OSPfs *, s32);
extern s32 osPfsRepairId(OSPfs *);
extern s32 func_800A1910(u8 *, u8 *, s32);   /* memcmp */
extern s32 PS_FlushPak(s32, s32);
extern void *memcpy(void *, const void *, u32);
extern void audio_reverb_update(u32, s32);  /* DeallocateMemory */
extern void func_800A370C(tDLList *);       /* ClearLL */

/* 0x800A3640: _RemovePak (private IPA: unsaved s-registers) */
extern PakHandle D_80144C60[];          /* the 64 tPS_File handles */
extern tPS_File D_80144DC0[];           /* the 64 tPS_File records (76 bytes) */
extern tPS_File D_801460C0[];           /* end of the records */
extern u8 D_800349F0[];                 /* pak thread */
extern u8 D_800329F0[];                 /* pak thread stack top */
extern void audio_queue_process(void *);    /* rumble/pak thread entry */
extern void osCreateThread(void *, s32, void (*)(void *), void *, void *, s32);
extern void osStartThread(void *);
extern void func_800A43FC(void);        /* poll pak presence (_CompleteCallback slot) */
extern void func_800A44E8(tDLList *, u8, u8);   /* InitLL (MAT3 char = unsigned) */

/* 0x800A4508: PS_InitPakSys */
void car_lod_select(PakErrorFunc errorFunc)
{
    s32 i;

    D_80144008 = errorFunc;
    func_800A44E8(&D_801460E0, 1, 1);
    for (i = 0; i < 64; i++) {
        D_80144C60[i].file = &D_80144DC0[i];
        func_80091FBC(&D_801460E0, &D_80144C60[i], D_801460E0.head);
    }
    for (i = 0; i < 4; i++) {
        func_800A44E8(&D_80144D60[i], 1, 1);
        D_80144030[i].damaged = 0;
        func_800A3724(i);
    }
    osCreateThread(D_800349F0, 2, audio_queue_process, 0, D_800329F0, 6);
    osStartThread(D_800349F0);
    D_8011EAE0 = 1;
    func_800A43FC();
}
