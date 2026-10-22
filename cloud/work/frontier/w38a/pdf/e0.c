/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * pak_delete_file_retry (0x800A2680, 784 B) = paksysdc.c PS_DeleteFile (N64 osPfs body) -- NOT a match (lane w36v).
 * Unit (`--internal func_800A1E94 paksys_queue_init paksys_lock paksys_unlock pdf_heap_lock pdf_heap_unlock
 * pdf_buffer_free pdf_free_blocks pdf_release`): FAIL 154 of 196 words; rows 63, ops 10; frame 232/224
 * (prior best w28b/w31c: 98-99 rows). Levers kept from PS_CreateFile: no record pointer local (direct
 * D_80144030[pak] indexing), retail declaration order (psfile last after retry/status), func_800A1E94 internal,
 * inlined helpers for the two retail stubs 0x800A2670 (osPfsFreeBlocks wrapper) and 0x800A2678 (handle back to the
 * free list) and the nested heap-lock free. Residual: retail has 8 spill-temp webs vs 6 here (frame -8) and colours
 * pfs (s8) where ours colours ext; without the `state` pointer local (v/u3.c) the frame is exact (232) but the state
 * address is CSE'd with the record address (80 rows). See RESULTS.md.
 */
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
    u8 opaque124[8];
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

extern s32 osPfsDeleteFile(OSPfs *, u16, u32, u8 *, u8 *);
extern OSMesgQueue D_80152770;                 /* heap lock queue */
extern void audio_reverb_update(u32, s32);     /* heap free (IPA: a1, a2) */
extern s32 func_800A1E94(s32);
void paksys_lock(void);     /* installed w37a_paksys helper */
void paksys_unlock(void);
void pdf_heap_lock(void) {
    osRecvMesg(&D_80152770, 0, 1);
}
void pdf_heap_unlock(void) {
    osSendMesg(&D_80152770, 0, 0);
}
void pdf_free_blocks(s32 pak) {
    osPfsFreeBlocks(&D_80144030[pak].pfs, &D_80144030[pak].FreeSpace);
}
void pdf_release(PakHandle *fileh) {
    func_8009211C(&D_80144D60[fileh->file->pak], fileh);
    func_80091FBC(&D_801460E0, fileh, D_801460E0.head);
}
void pdf_buffer_free(void **address) {
    pdf_heap_lock();
    audio_reverb_update((u32)address, 0);
    pdf_heap_unlock();
}

void pak_delete_file_retry(PakHandle *fileh)
{
    s32 pak;
    OSPfsState *state;
    PakHandle *cur;
    s32 error;
    s8 retry;
    s8 status;
    tPS_File *psfile;

    paksys_lock();
    psfile = fileh->file;
    pak = psfile->pak;
    state = &D_80144030[psfile->pak].files[psfile->fileNo].state;
    for (;;) {
        error = osPfsDeleteFile(&D_80144030[pak].pfs, state->company_code, state->game_code,
                                state->game_name, state->ext_name);
        if (error) {
            D_80144030[pak].error = func_800A1E94(error);
            paksys_unlock();
            D_8011EAE8 = pak;
            D_80144008(pak, D_80144030[pak].error, 0, 0, &retry, &status, func_8008A6A4);
            D_8011EAE8 = -1;
            for (cur = D_80144D60[pak].head; cur != 0; cur = cur->file->next) {
                if (fileh == cur) break;
            }
            if (cur == 0) return;
            paksys_lock();
            if (!retry) {
                paksys_unlock();
                return;
            }
        } else break;
    }
    pdf_free_blocks(pak);
    if (psfile->RemoveFile) psfile->RemoveFile(fileh);
    state->file_size = 0;
    if (psfile->data) {
        pdf_buffer_free((void **)psfile->data);
        psfile->data = 0;
    }
    pdf_release(fileh);
    paksys_unlock();
}

