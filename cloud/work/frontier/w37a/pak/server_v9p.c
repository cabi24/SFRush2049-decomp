
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
    D_80144030[pak].rumble128 = 0;
    D_80144030[pak].rumble126 = 0;
    D_80144030[pak].rumble125 = 0;
    D_80144030[pak].rumble124 = 0;
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
    s8 status;
    s8 retry;
    s8 changed[4];
    s8 dup;
    s8 same;
    PakHandle *fileh;
    tPS_File *psfile;

    retry = 0;
    status = 0;
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
        dup = 0;
        same = 0;
        memcpy(&pfs, (void *)&D_80144030[i].pfs, sizeof(OSPfs));
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
        D_80144030[i].rumble128 = 0;
        D_80144030[i].rumble126 = 0;
        D_80144030[i].rumble125 = 0;
        D_80144030[i].rumble124 = 0;
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
    if (forceUpdate) {}
}
