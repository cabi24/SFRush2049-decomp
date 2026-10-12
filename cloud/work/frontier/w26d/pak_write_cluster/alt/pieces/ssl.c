void slot_state_lookup(C96Handle *handle, u8 *address, u32 bytes) {
    C96File *file;
    if (!handle) return;
    file = handle->file;
    if (D_80144030[file->channel].slots[file->slot].flag2) {}
    func_800A1DD4(handle, address, bytes, 1);
    D_80144030[file->channel].slots[file->slot].modified = 1;
    D_80144030[file->channel].modified = 1;
}
