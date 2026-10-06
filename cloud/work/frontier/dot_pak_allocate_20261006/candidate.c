/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Research NONMATCH at 0x800A2D4C (1,752 native bytes).
 * Baseline: PR #163, commit 1bd09c5eb3bde8f803c46d0d657e9264d4223a38,
 * tree 277d344e5d9a0edf6eea7732fe7d770cb04aed25,
 * cloud/matches/pak_reset_a3724_group/group.c. This is an incremental change
 * to its complete Controller Pak reconstruction, not newly discovered code.
 * Keep the real shared helpers/callers and their existing interfaces via repro.py.
 * Existing local declarations follow native address-taken object order.
 * Consume the file size through its actual request field, removing the extra
 * file_size temporary. No filler locals, synthetic callers, or qualifiers.
 * 348/438 -> 335/438 differing words; frame 288 -> native 280; still 6 extra
 * nonzero words. Original source spelling and full behavior are not proven.
 */

/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
PakNode *track_process_main(s32 port, u8 *name, u8 *extension,
                           u32 size, s32 company, s32 game_code)
{
    s32 error, file;
    u8 game_name[16], ext_name[4];
    s8 retry, status;
    Pak772 *pak;
    PakNode *node;
    PakRequest *request;
    OSPfsState *state;

    pak_lock();
    func_800A150C(game_name, name, 16);
    func_800A150C(ext_name, extension, 4);
    pak = &D_80144030[port];
    for (;;) {
        error = osPfsReadWriteFile(&pak->pfs, (u16)company, (u32)game_code,
                                  game_name, ext_name, (size + 255) & ~255U, &file);
        if (error == 0) break;
        pak->error = func_800A1E94(error);
        pak_unlock();
        D_8011EAE8 = port;
        D_80144008(port, pak->error, (size + 255) >> 8, 0, &retry, &status,
                    pak->error == 3 ? 0 : func_8008A6A4);
        D_8011EAE8 = -1;
        if (pak->error != 3 && !pak->enabled) return 0;
        pak_lock();
        if (pak->error == 8 && retry) {
            for (file = 0; file < 16; file++) {
                state = &D_80144030[port].files[file].state;
                if (func_8008AD04(state->game_name, game_name) == 0 &&
                    func_8008AD04(state->ext_name, ext_name) == 0 &&
                    state->game_code == (u32)game_code &&
                    company == state->company_code) break;
            }
            if (file == 16) continue;
            pak_unlock();
            for (node = D_80144D60[port].head; node != 0;
                 node = node->request->next) {
                if (node->request->file == file) {
                    AdjustSpeed(node);
                    break;
                }
            }
            pak_lock();
        }
        if (pak->error == 6 && retry) retry = 0;
        if (retry) continue;
        pak_unlock();
        return 0;
    }
    osPfsFreeBlocks(&pak->pfs, &pak->free_bytes);
    for (;;) {
        error = osPfsDeleteFile(&pak->pfs, file, &pak->files[file].state);
        if (!error && D_80144030[port].files[file].state.file_size != 0) break;
        pak->error = func_800A1E94(error);
        pak_unlock();
        D_8011EAE8 = port;
        D_80144008(port, pak->error, (size + 255) >> 8, 0,
                    &retry, &status, func_8008A6A4);
        D_8011EAE8 = -1;
        if (!pak->enabled) {
            D_80144030[port].files[file].state.file_size = 0;
            return 0;
        }
        pak_lock();
        if (!retry) {
            pak_unlock();
            D_80144030[port].files[file].state.file_size = 0;
            return 0;
        }
    }
    node = D_801460E0.head;
    func_8009211C(&D_801460E0, node);
    request = node->request;
    request->done = 0;
    request->opaque12 = 0;
    request->port = port;
    request->file = file;
    request->file_size = D_80144030[port].files[file].state.file_size;
    request->pages = (request->file_size + 255) >> 8;
    D_80144030[port].files[file].bitmap_bytes = (((request->file_size + 31) >> 5) + 7) >> 3;
    request->buffer = audio_task_complete(0,
        D_80144030[port].files[file].bitmap_bytes + request->file_size);
    memset(*request->buffer, 0, request->file_size);
    memset(*request->buffer + request->file_size, 255,
        D_80144030[port].files[file].bitmap_bytes);
    D_80144030[port].files[file].active = 0;
    D_80144030[port].files[file].flag1 = 0;
    D_80144030[port].files[file].flag2 = 0;
    func_800A1644(request->name, D_80144030[port].files[file].state.game_name, 16);
    func_800A1644(request->extension, D_80144030[port].files[file].state.ext_name, 4);
    no_catchup(node);
    pak_unlock();
    return node;
}
