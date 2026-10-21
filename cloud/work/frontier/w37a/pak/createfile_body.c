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

