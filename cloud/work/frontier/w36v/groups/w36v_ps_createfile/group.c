/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * PS_CreateFile (0x800A2D4C) = paksysdc.c PS_CreateFile, N64 Controller Pak version, and no_catchup (0x800A2D0C) =
 * the paksysdc.c "add file to gPS_Files[pak]" helper, ONE parameter (the second caller is track_render_process =
 * PS_PakServer, lane w36p). Lane w36v (wave 36).
 *
 * Provenance: the MAT3 GC body (decomp/src/GameSource/src/paksysdc.c PS_CreateFile @0x8002fb30) is the PS2 memory-
 * card platform layer (AllocateHandleHeap + PS2MCSetupMcDirectory), so only names/shape transfer (tPS_File fields
 * RemoveFile/MoveFile/pak/size/blocks/data, InsertLLE into gPS_Files[pak], gPaks[pak].error/FreeSpace). The N64 osPfs
 * body was reconstructed from retail with Astra's research packet PR #410 (dot_mat3_pak_cleanup_research_20261017
 * candidate.c: types, call sequence; 348/438 there) as the starting draft, and the pak_write_cluster group's house
 * style (pfs_queue_init/pfs_lock/pfs_unlock = the inlined lock helpers, volatile pak records).
 *
 * Shaping found by w36v (all source-level, no unused locals, no pad):
 *  - Named locals in retail order: error, file, game_name[16], ext_name[4], retry, status, node, request, state;
 *    no `pak` pointer local (every record access is D_80144030[port].x); the frame (280) and every home then match.
 *  - The allocate loop is `for (;;) { error = alloc(); if (error) { ... } else break; }`: a `goto`/`break` after an
 *    `if (error == 0)` test gives PRE a separate exit-edge block for &D_80144030[port] (10 rows).
 *  - pfs_get_node is the inlined static of retail stub 0x800A2D44 (here non-static/internal so the group object has
 *    no stub of its own); `node = head; RemoveLLE(&free, head)` (head read twice) loads into a1 and copies, as retail.
 *  - IPA: func_800A1E94 (status conversion) must be internal: retail keeps the callback constant in t0 across it.
 *    no_catchup must be internal and one-parameter: IPA then passes node in a1 and retail's callers never set a0.
 *    With a single call site umerge inlines it, so its second retail caller must be present (stand-in below).
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
void no_catchup(PakHandle *fileh);

/* context: byte-identical copy of the locked codex_pfs_status_c90 body (internal here: IPA register facts) */
s32 func_800A1E94(s32 error) {
    switch (error) {
        case 0: return 0;
        case 1: return 2;
        case 2: return 3;
        case 3: return 4;
        case 4: return 5;
        case 5: return 7;
        case 7: return 6;
        case 8: return 6;
        case 9: return 8;
        case 6: return 9;
        case 10:
        case 11: return 10;
        default: return 15;
    }
}

/* the inlined pak lock helpers (bodies as group pak_write_cluster pfs_*; own names: the unit renames duplicated internal names) */
void paksys_queue_init(void) {
    if (!D_8011194C) {
        D_8011194C = 1; osCreateMesgQueue(&D_801497D0, &D_801527E4, 1);
        osSendMesg(&D_801497D0, 0, 0);
    }
}
void paksys_lock(void) {
    OSMesg message;
    paksys_queue_init();
    osRecvMesg(&D_801497D0, &message, 1);
}
void paksys_unlock(void) {
    osSendMesg(&D_801497D0, 0, 0);
}

/* 0x800A2D0C: InsertLLE(&gPS_Files[file->pak], fileh, head) */
void no_catchup(PakHandle *fileh)
{
    func_80091FBC(&D_80144D60[fileh->file->pak], fileh, D_80144D60[fileh->file->pak].head);
}

/* retail stub 0x800A2D44: take a tPS_File handle off the free list */
PakHandle *pfs_get_node(void)
{
    PakHandle *fileh;
    fileh = D_801460E0.head;
    func_8009211C(&D_801460E0, D_801460E0.head);
    return fileh;
}

/* 0x800A2D4C */
PakHandle *PS_CreateFile(s32 pak, u8 *name, u8 *ext, u32 numBytes, s32 company, s32 gameCode)
{
    s32 error;
    s32 file;
    u8 game_name[16];
    u8 ext_name[4];
    s8 retry;
    s8 status;
    PakHandle *fileh;
    tPS_File *psfile;
    OSPfsState *state;

    paksys_lock();
    func_800A150C(game_name, name, 16);
    func_800A150C(ext_name, ext, 4);
    for (;;) {
        error = osPfsAllocateFile(&D_80144030[pak].pfs, (u16)company, (u32)gameCode,
                                  game_name, ext_name, (numBytes + 255) & ~255U, &file);
        if (error) {
            D_80144030[pak].error = func_800A1E94(error);
            paksys_unlock();
            D_8011EAE8 = pak;
            D_80144008(pak, D_80144030[pak].error, (numBytes + 255) >> 8, 0, &retry, &status,
                       D_80144030[pak].error == 3 ? 0 : func_8008A6A4);
            D_8011EAE8 = -1;
            if (D_80144030[pak].error != 3 && !D_80144030[pak].inserted) return 0;
            paksys_lock();
            if (D_80144030[pak].error == 8 && retry) {
                /* the file exists: find its slot and delete our handle for it */
                for (file = 0; file < 16; file++) {
                    state = &D_80144030[pak].files[file].state;
                    if (func_8008AD04(state->game_name, game_name) == 0 &&
                        func_8008AD04(state->ext_name, ext_name) == 0 &&
                        state->game_code == (u32)gameCode &&
                        company == state->company_code) break;
                }
                if (file == 16) continue;
                paksys_unlock();
                for (fileh = D_80144D60[pak].head; fileh != 0; fileh = fileh->file->next) {
                    if (fileh->file->fileNo == file) {
                        pak_delete_file_retry(fileh);
                        break;
                    }
                }
                paksys_lock();
            }
            if (D_80144030[pak].error == 6 && retry) retry = 0;
            if (!retry) {
                paksys_unlock();
                return 0;
            }
        } else break;
    }
    osPfsFreeBlocks(&D_80144030[pak].pfs, &D_80144030[pak].FreeSpace);
    for (;;) {
        error = osPfsFileState(&D_80144030[pak].pfs, file, &D_80144030[pak].files[file].state);
        if (!error && D_80144030[pak].files[file].state.file_size != 0) break;
        D_80144030[pak].error = func_800A1E94(error);
        paksys_unlock();
        D_8011EAE8 = pak;
        D_80144008(pak, D_80144030[pak].error, (numBytes + 255) >> 8, 0,
                   &retry, &status, func_8008A6A4);
        D_8011EAE8 = -1;
        if (!D_80144030[pak].inserted) {
            D_80144030[pak].files[file].state.file_size = 0;
            return 0;
        }
        paksys_lock();
        if (!retry) {
            paksys_unlock();
            D_80144030[pak].files[file].state.file_size = 0;
            return 0;
        }
    }
    fileh = pfs_get_node();
    psfile = fileh->file;
    psfile->RemoveFile = 0;
    psfile->MoveFile = 0;
    psfile->pak = pak;
    psfile->fileNo = file;
    psfile->size = D_80144030[pak].files[file].state.file_size;
    psfile->blocks = (psfile->size + 255) >> 8;
    D_80144030[pak].files[file].mapBytes = (((psfile->size + 31) >> 5) + 7) >> 3;
    psfile->data = audio_task_complete(0, D_80144030[pak].files[file].mapBytes + psfile->size);
    memset(*psfile->data, 0, psfile->size);
    memset(*psfile->data + psfile->size, 255, D_80144030[pak].files[file].mapBytes);
    D_80144030[pak].files[file].flush = 0;
    D_80144030[pak].files[file].flag1 = 0;
    D_80144030[pak].files[file].flag2 = 0;
    func_800A1644(psfile->name, D_80144030[pak].files[file].state.game_name, 16);
    func_800A1644(psfile->ext, D_80144030[pak].files[file].state.ext_name, 4);
    no_catchup(fileh);
    paksys_unlock();
    return fileh;
}

/* STAND-IN (not retail): the second caller of no_catchup. Retail's is track_render_process (PS_PakServer,
 * 0x800A377C, lane w36p, unmatched). Without a second call site umerge inlines no_catchup into PS_CreateFile. */
void w36v_standin_caller(PakHandle *fileh)
{
    no_catchup(fileh);
}
