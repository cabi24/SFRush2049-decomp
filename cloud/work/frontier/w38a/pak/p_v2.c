/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * Controller Pak system (MAT3 paksysdc.c, N64 osPfs form), lane w37a (wave 37). One file, GC paksysdc.c names:
 *   PS_CreateFile (0x800A2D4C)        - w36v body, unchanged
 *   no_catchup (0x800A2D0C)           - one-parameter InsertLLE(&gPS_Files[psfile->pak], fileh, head) helper
 *   func_800A3640 = _RemovePak        - w8b body (osSendMesg), private IPA (unsaved s-registers)
 *   func_800A3724 = _ClearPak         - memset of the 772-byte gPaks record keeping .damaged; pak in a3 (IPA)
 *   track_render_process = PS_PakServer (0x800A377C) - NOT MATCHED (genuine reconstruction, see RESULTS.md)
 *   car_lod_select = PS_InitPakSys (0x800A4508)      - NOT MATCHED (19 words: s3/s4 swap + one schedule)
 * The four first functions are EQUAL in the whole-program unit with PS_PakServer and PS_InitPakSys as their REAL
 * callers (no stand-ins). They stay provisional only because those two callers are not matched yet.
 *
 * Provenance: MAT3 decomp/src/GameSource/src/paksysdc.c (GC/PS2 platform layer: names, statement order of
 * PS_PakServer/PS_InitPakSys, ClearLL/InitLL use) + rush_d_dwarf.c locals (i, j, changed[4], dup, same, psfileh);
 * N64 osPfs bodies reconstructed from retail with Astra's research PR #410 candidate.c
 * (dot_mat3_pak_cleanup_research_20261017) as the starting skeleton; w36v (PS_CreateFile), w36w3 (InitPakSys,
 * _ClearPak), w8b (_RemovePak). Rumble field names from frontier_rumble_thread.
 *
 * Shaping in PS_PakServer (all source-level):
 *  - no pak/other pointer locals (volatile gPaks indexed directly, as PS_CreateFile);
 *  - `if (!flushing) { ...; same = 1; goto flush; } continue;` (retail's dead `b next` block), GC-style positive
 *    `if (i != j && inited && !memcmp) { ... move: ... break; }`, `if (file_size) {...}` in the directory loop;
 *  - ClearLL(&gPS_Files[j]) = func_800A370C inlined; the first rumble clear is the one-site inlined helper
 *    _StopRumble (retail's +8 inline area above the first lock), the second is inline code;
 *  - `s8 status = 0; s8 retry = 0;` initialisers; `dup = 0; same = 0; memcpy(...)` on one line (as1 line tie);
 *  - DISCLOSED DEVICE: three compiled-out `if (forceUpdate) {}` at the end. umerge otherwise also inlines retail's
 *    first `jal func_8008A704` (the public pak lock): retail's caller is that much larger at the IR level. The
 *    real source of that size is not known (dead stores do not count; extra branch structure does).
 *  - `s8 X` is an unreferenced s8 between pfs and status: retail's frame has one there (sp+311). Disclosed.
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
void func_800A3640(s32 pak)
{
    PakHandle *fileh;
    PakHandle *next;
    tPS_File *psfile;

    fileh = D_80144D60[pak].head;
    while (fileh != 0) {
        psfile = fileh->file;
        next = psfile->next;
        if (psfile->RemoveFile != 0) {
            psfile->RemoveFile(fileh);
        }
        if (psfile->data != 0) {
            u8 **data = psfile->data;
            osRecvMesg(&D_80152770, 0, 1);
            audio_reverb_update((u32)data, 0);
            osSendMesg(&D_80152770, 0, 0);
            psfile->data = 0;
        }
        func_8009211C(&D_80144D60[fileh->file->pak], fileh);
        func_80091FBC(&D_801460E0, fileh, D_801460E0.head);
        fileh = next;
    }
}

/* 0x800A3724: _ClearPak (private IPA: pak in a3) */
void func_800A3724(u8 pak)
{
    s8 damaged;

    damaged = D_80144030[pak].damaged;
    memset((void *)&D_80144030[pak], 0, sizeof(PakRecord));
    D_80144030[pak].damaged = damaged;
}

/* inlined: does the file's write bitmap have dirty bytes */
s32 _FileChanged(tPS_File *psfile)
{
    u8 *map;
    u32 i;

    if (!psfile->data) return 0;
    map = *psfile->data + psfile->size;
    for (i = 0; i < D_80144030[psfile->pak].files[psfile->fileNo].mapBytes; i++) {
        if (map[i]) return 1;
    }
    return 0;
}

/* inlined: motor off and rumble state cleared */
void _StopRumble(s32 pak)
{
    D_80144030[pak].opaque6 = 0;
    D_80144030[pak].phase = 0;
    D_80144030[pak].level = 0;
    D_80144030[pak].request = 0;
    D_80144030[pak].motorOn = 0;
}

/* 0x800A377C: PS_PakServer */
void track_render_process(s32 forceUpdate, s32 rumbleOnly)
{
    s32 i;
    s32 j;
    s32 ret;
    s32 err;
    OSPfs pfs;
    s8 X;
    s8 status = 0;
    s8 retry = 0;
    s8 changed[4];
    s8 dup;
    s8 same;
    PakHandle *fileh;
    tPS_File *psfile;

    if (!D_8011EAE0) return;
    for (i = 0; i < 4; i++) changed[i] = 0;
    func_8008A704();
    for (i = 0; i < 4; i++) {
        if (D_8011EAE8 >= 0 && i != D_8011EAE8) continue;
        if (!D_80156CF0[i].present || !D_80144030[i].opaque2 || D_80144030[i].opaque3) {
            D_80144030[i].opaque3 = 0;
            if (D_80144030[i].inserted) {
                D_80144030[i].inserted = 0;
                D_80144030[i].opaque9 = 0;
                D_80144030[i].error = 0;
                if (D_80144030[i].opaque6) {
                    _StopRumble(i);
                    changed[i] = 1;
                    continue;
                }
                if (!D_80144030[i].opaque10 && !D_80144030[i].flush && !D_80144030[i].flushing) {
                    func_800A3640(i);
                    func_800A3724(i);
                }
                D_80144030[i].damaged = 0;
                changed[i] = 1;
            } else if (D_80144030[i].opaque4 && !D_80144030[i].opaque10 && !D_80144030[i].flush && !D_80144030[i].flushing) {
                func_800A3640(i);
                func_800A3724(i);
            }
            continue;
        }
        if (D_80144030[i].inserted) continue;
        changed[i] = 1;
        D_80144030[i].inserted = 1;
        dup = 0; same = 0; memcpy(&pfs, (void *)&D_80144030[i].pfs, sizeof(OSPfs));
init:
        pfs.queue = 0;
        ret = osPfsInitPak(&D_80035458, &pfs, i);
        if (!pfs.queue) ret = 1;
        D_80144030[i].error = func_800A1E94(ret);
        if (D_80144030[i].error == 10) {
            if (!osMotorInit(&D_80035458, &pfs, i)) {
                if (!D_80144030[i].opaque6) D_80144030[i].opaque6 = 1;
                else changed[i] = 0;
                continue;
            }
            if (rumbleOnly) {
                D_80144030[i].inserted = 0;
                changed[i] = 0;
                continue;
            }
            paksys_unlock();
prompt:
            D_8011EAE8 = i;
            D_80144008(i, 10, 0, 0, &retry, &status, func_8008A6A4);
            D_8011EAE8 = -1;
            if (!D_80144030[i].inserted) {
                func_8008A704();
                changed[i] = 0;
                continue;
            }
            if (status) {
                func_8008A704();
                ret = osPfsInitPak(&D_80035458, &pfs, i);
                if (!ret) goto inited;
                ret = osPfsRepairId(&pfs);
                paksys_unlock();
                if (ret) {
                    D_8011EAE8 = i;
                    D_80144008(i, 12, 0, 0, &retry, &status, 0);
                    D_8011EAE8 = -1;
                    goto prompt;
                }
                D_8011EAE8 = i;
                D_80144008(i, 13, 0, 0, &retry, &status, 0);
                D_8011EAE8 = -1;
            } else if (retry) {
                func_8008A704();
                D_80144030[i].damaged = 1;
                D_80144030[i].opaque9 = 0;
                changed[i] = 0;
                continue;
            } else {
                D_8011EAE8 = i;
                D_80144008(i, 11, 0, 0, &retry, &status, 0);
                D_8011EAE8 = -1;
            }
            func_8008A704();
            goto init;
        }
inited:
        D_80144030[i].opaque6 = 0;
        D_80144030[i].phase = 0;
        D_80144030[i].level = 0;
        D_80144030[i].request = 0;
        D_80144030[i].motorOn = 0;
        if (ret == 2) D_80144030[i].error = 0;
        else if (ret) {
            changed[i] = 0;
            D_80144030[i].damaged = 1;
            D_80144030[i].opaque9 = 0;
            continue;
        }
        if (!D_80144030[i].opaque4) {
            D_80144030[i].opaque4 = 1;
        } else if (!func_800A1910((u8 *)D_80144030[i].pfs.id, pfs.id, 32)) {
            changed[i] = 0;
            memcpy((void *)&D_80144030[i].pfs, &pfs, sizeof(OSPfs));
            osPfsFreeBlocks((OSPfs *)&D_80144030[i].pfs, (s32 *)&D_80144030[i].FreeSpace);
            D_80144030[i].opaque9 = 1;
            if (!D_80144030[i].flushing) {
                for (fileh = D_80144D60[i].head; fileh; fileh = fileh->file->next) {
                    if (_FileChanged(fileh->file)) {
                        D_80144030[i].files[fileh->file->fileNo].flag1 = 1;
                        D_80144030[i].flush = 1;
                    }
                }
                same = 1;
                goto flush;
            }
            continue;
        }
        for (j = 0; j < 4; j++) {
            if (i != j && D_80144030[j].opaque4 && !func_800A1910(pfs.id, (u8 *)D_80144030[j].pfs.id, 32)) {
                if (D_80144030[i].opaque4) {
                    dup = 1;
                    goto locked;
                }
move:
                memcpy((void *)&D_80144030[i].pfs, &pfs, sizeof(OSPfs));
                osPfsFreeBlocks((OSPfs *)&D_80144030[i].pfs, (s32 *)&D_80144030[i].FreeSpace);
                D_80144030[i].opaque9 = 1;
                memcpy(&D_80144D60[i], &D_80144D60[j], sizeof(tDLList));
                func_800A370C(&D_80144D60[j]);
                for (fileh = D_80144D60[i].head; fileh; fileh = fileh->file->next)
                    fileh->file->pak = i;
                memcpy((void *)D_80144030[i].files, (void *)D_80144030[j].files, sizeof(D_80144030[i].files));
                changed[i] = 0;
                D_80144030[j].opaque4 = 0;
                D_80144030[j].opaque9 = 0;
                if (D_80144030[j].inserted && !D_80144030[j].damaged) D_80144030[j].opaque3 = 1;
                changed[j] = 1;
                break;
            }
        }
        if (j < 4) continue;
        if (D_80144030[i].flushing) {
            changed[i] = 0;
            continue;
        }
locked:
        if (D_80144030[i].opaque10) {
            changed[i] = 0;
            continue;
        }
flush:
        paksys_unlock();
        if (!PS_FlushPak(i, 0)) {
            changed[i] = 0;
            func_8008A704();
            continue;
        }
        func_8008A704();
        if (D_80144030[i].flushed && !same) {
            dup = 0;
            same = 0;
            goto init;
        }
        if (same) continue;
        func_800A3640(i);
        if (dup) goto move;
        D_80144030[i].opaque9 = 1;
        D_80144030[i].opaque10 = 0;
        D_80144030[i].flush = 0;
        memcpy((void *)&D_80144030[i].pfs, &pfs, sizeof(OSPfs));
        osPfsFreeBlocks((OSPfs *)&D_80144030[i].pfs, (s32 *)&D_80144030[i].FreeSpace);
        for (j = 0; j < 16; j++) {
            err = osPfsFileState((OSPfs *)&D_80144030[i].pfs, j, (OSPfsState *)&D_80144030[i].files[j].state);
            if (err) D_80144030[i].files[j].state.file_size = 0;
            D_80144030[i].files[j].flush = 0;
            D_80144030[i].files[j].flag1 = 0;
            D_80144030[i].files[j].flag2 = 0;
            D_80144030[i].files[j].mapBytes = (((D_80144030[i].files[j].state.file_size + 31) >> 5) + 7) >> 3;
            D_80144030[i].error = func_800A1E94(err);
            if (D_80144030[i].files[j].state.file_size) {
                fileh = pfs_get_node();
                psfile = fileh->file;
                psfile->RemoveFile = 0;
                psfile->MoveFile = 0;
                psfile->pak = i;
                psfile->fileNo = j;
                psfile->size = D_80144030[i].files[j].state.file_size;
                psfile->blocks = (psfile->size + 255) >> 8;
                func_800A1644(psfile->name, (u8 *)D_80144030[i].files[j].state.game_name, 16);
                func_800A1644(psfile->ext, (u8 *)D_80144030[i].files[j].state.ext_name, 4);
                if (psfile->ext[0]) psfile->ext[1] = 0;
                psfile->data = 0;
                no_catchup(fileh);
            }
        }
    }
    paksys_unlock();
    for (i = 0; i < 4; i++) D_80144030[i].changed = changed[i];
    if (forceUpdate) {}
    if (forceUpdate) {}
    if (forceUpdate) {}
}

/* ---- PS_InitPakSys ---- */
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
    PakHandle *fileh;
    tPS_File *psfile;

    D_80144008 = errorFunc;
    func_800A44E8(&D_801460E0, 1, 1);
    fileh = D_80144C60;
    psfile = D_80144DC0;
    do {
        fileh->file = psfile;
        func_80091FBC(&D_801460E0, fileh, D_801460E0.head);
        psfile++;
        fileh++;
    } while (psfile < &D_80144DC0[64]);
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
