
/* ---- PS_PakServer and its private callees ---- */
typedef struct ControllerData { s8 present; u8 opaque1[15]; } ControllerData;
extern ControllerData D_80156CF0[];     /* gControllerData */
extern s8 D_8011EAE0;                   /* gInited */
extern OSMesgQueue D_80035458;          /* SI message queue */
extern OSMesgQueue D_80152770;          /* heap semaphore */
extern void func_8008A704(void);        /* poll controller/pak status */
extern s32 osPfsInitPak(OSMesgQueue *, OSPfs *, s32);
extern s32 osMotorInit(OSMesgQueue *, OSPfs *, s32);
extern s32 osPfsRepairId(OSPfs *);
extern s32 func_800A1910(u8 *, u8 *, s32);   /* memcmp */
extern s32 PS_FlushPak(s32, s32);
extern void *memcpy(void *, const void *, u32);
extern void audio_reverb_update(u32, s32);  /* DeallocateMemory */

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

/* 0x800A377C: PS_PakServer */
void track_render_process(s32 forceUpdate, s32 rumbleOnly)
{
    s32 i;
    s32 j;
    s32 ret;
    volatile PakRecord *pak;
    OSPfs pfs;
    s8 status;
    s8 retry;
    s8 changed[4];
    s8 dup;
    s8 same;
    PakHandle *fileh;
    tPS_File *psfile;
    volatile PakFile *file;
    volatile PakRecord *other;

    retry = 0;
    status = 0;
    if (!D_8011EAE0) return;
    for (i = 0; i < 4; i++) changed[i] = 0;
    func_8008A704();
    for (i = 0; i < 4; i++) {
        if (D_8011EAE8 >= 0 && i != D_8011EAE8) continue;
        pak = &D_80144030[i];
        if (!D_80156CF0[i].present || !pak->opaque2 || pak->opaque3) {
            pak->opaque3 = 0;
            if (pak->inserted) {
                pak->inserted = 0;
                pak->opaque9 = 0;
                pak->error = 0;
                if (pak->opaque6) {
                    pak->opaque6 = 0;
                    pak->rumble128 = 0;
                    pak->rumble126 = 0;
                    pak->rumble125 = 0;
                    pak->rumble124 = 0;
                    changed[i] = 1;
                    continue;
                }
                if (!pak->opaque10 && !pak->flush && !pak->flushing) {
                    func_800A3640(i);
                    func_800A3724(i);
                }
                pak->damaged = 0;
                changed[i] = 1;
            } else if (pak->opaque4 && !pak->opaque10 && !pak->flush && !pak->flushing) {
                func_800A3640(i);
                func_800A3724(i);
            }
            continue;
        }
        if (pak->inserted) continue;
        changed[i] = 1;
        pak->inserted = 1;
        dup = 0;
        same = 0;
        memcpy(&pfs, (void *)&pak->pfs, sizeof(OSPfs));
init:
        pfs.queue = 0;
        ret = osPfsInitPak(&D_80035458, &pfs, i);
        if (!pfs.queue) ret = 1;
        pak->error = func_800A1E94(ret);
        if (pak->error == 10) {
            if (!osMotorInit(&D_80035458, &pfs, i)) {
                if (!pak->opaque6) pak->opaque6 = 1;
                else changed[i] = 0;
                continue;
            }
            if (rumbleOnly) {
                pak->inserted = 0;
                changed[i] = 0;
                continue;
            }
            paksys_unlock();
prompt:
            D_8011EAE8 = i;
            D_80144008(i, 10, 0, 0, &retry, &status, func_8008A6A4);
            D_8011EAE8 = -1;
            if (!pak->inserted) {
                paksys_lock();
                changed[i] = 0;
                continue;
            }
            if (status) {
                paksys_lock();
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
                paksys_lock();
                pak->damaged = 1;
                pak->opaque9 = 0;
                changed[i] = 0;
                continue;
            } else {
                D_8011EAE8 = i;
                D_80144008(i, 11, 0, 0, &retry, &status, 0);
                D_8011EAE8 = -1;
            }
            paksys_lock();
            goto init;
        }
inited:
        pak->opaque6 = 0;
        pak->rumble128 = 0;
        pak->rumble126 = 0;
        pak->rumble125 = 0;
        pak->rumble124 = 0;
        if (ret == 2) pak->error = 0;
        else if (ret) {
            changed[i] = 0;
            pak->damaged = 1;
            pak->opaque9 = 0;
            continue;
        }
        if (!pak->opaque4) {
            pak->opaque4 = 1;
        } else if (!func_800A1910((u8 *)pak->pfs.id, pfs.id, 32)) {
            changed[i] = 0;
            memcpy((void *)&pak->pfs, &pfs, sizeof(OSPfs));
            osPfsFreeBlocks((OSPfs *)&pak->pfs, (s32 *)&pak->FreeSpace);
            pak->opaque9 = 1;
            if (pak->flushing) continue;
            for (fileh = D_80144D60[i].head; fileh; fileh = fileh->file->next) {
                if (_FileChanged(fileh->file)) {
                    pak->files[fileh->file->fileNo].flag1 = 1;
                    pak->flush = 1;
                }
            }
            same = 1;
            goto flush;
        }
        for (j = 0; j < 4; j++) {
            if (j == i) continue;
            other = &D_80144030[j];
            if (!other->opaque4 || func_800A1910(pfs.id, (u8 *)other->pfs.id, 32)) continue;
            if (pak->opaque4) {
                dup = 1;
                goto locked;
            }
move:
            other = &D_80144030[j];
            memcpy((void *)&pak->pfs, &pfs, sizeof(OSPfs));
            osPfsFreeBlocks((OSPfs *)&pak->pfs, (s32 *)&pak->FreeSpace);
            pak->opaque9 = 1;
            memcpy(&D_80144D60[i], &D_80144D60[j], sizeof(tDLList));
            D_80144D60[j].head = 0;
            D_80144D60[j].tail = 0;
            D_80144D60[j].count = 0;
            for (fileh = D_80144D60[i].head; fileh; fileh = fileh->file->next)
                fileh->file->pak = i;
            memcpy((void *)pak->files, (void *)other->files, sizeof(pak->files));
            changed[i] = 0;
            other->opaque4 = 0;
            other->opaque9 = 0;
            if (other->inserted && !other->damaged) other->opaque3 = 1;
            changed[j] = 1;
            break;
        }
        if (j < 4) continue;
        if (pak->flushing) {
            changed[i] = 0;
            continue;
        }
locked:
        if (pak->opaque10) {
            changed[i] = 0;
            continue;
        }
flush:
        paksys_unlock();
        if (!PS_FlushPak(i, 0)) {
            changed[i] = 0;
            paksys_lock();
            continue;
        }
        paksys_lock();
        if (pak->flushed && !same) {
            dup = 0;
            same = 0;
            goto init;
        }
        if (same) continue;
        func_800A3640(i);
        if (dup) goto move;
        pak->opaque9 = 1;
        pak->opaque10 = 0;
        pak->flush = 0;
        memcpy((void *)&pak->pfs, &pfs, sizeof(OSPfs));
        osPfsFreeBlocks((OSPfs *)&pak->pfs, (s32 *)&pak->FreeSpace);
        for (j = 0; j < 16; j++) {
            file = &pak->files[j];
            ret = osPfsFileState((OSPfs *)&pak->pfs, j, (OSPfsState *)&file->state);
            if (ret) file->state.file_size = 0;
            file->flush = 0;
            file->flag1 = 0;
            file->flag2 = 0;
            file->mapBytes = (((file->state.file_size + 31) >> 5) + 7) >> 3;
            pak->error = func_800A1E94(ret);
            if (!file->state.file_size) continue;
            fileh = pfs_get_node();
            psfile = fileh->file;
            psfile->RemoveFile = 0;
            psfile->MoveFile = 0;
            psfile->pak = i;
            psfile->fileNo = j;
            psfile->size = file->state.file_size;
            psfile->blocks = (file->state.file_size + 255) >> 8;
            func_800A1644(psfile->name, (u8 *)file->state.game_name, 16);
            func_800A1644(psfile->ext, (u8 *)file->state.ext_name, 4);
            if (psfile->ext[0]) psfile->ext[1] = 0;
            psfile->data = 0;
            no_catchup(fileh);
        }
    }
    paksys_unlock();
    for (i = 0; i < 4; i++) D_80144030[i].changed = changed[i];
}
