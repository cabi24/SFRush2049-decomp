s32 track_data_decompress(C96Handle *handle, s32 mode) {
    s32 result;
    s32 index, slot_index;
    s8 retry, callback_value;
    u8 *address;
    s32 bytes, offset;
    u8 **allocation;
    if (!handle) return 1;
    index = handle->file->channel;
    slot_index = handle->file->slot;
    allocation = handle->file->data;
    if (!D_80144030[index].slots[slot_index].modified) return 1;
    osWritebackDCache(*allocation, handle->file->data_size);
    if (func_800A1C6C(handle, &address, &bytes, &offset)) {
        do {
            osWritebackDCache(address, bytes);
write_retry:
            result = osPfsReadWriteFile(&D_80144030[index].pfs, slot_index, 1, offset, bytes, address);
            if (result) {
                D_80144030[index].error = func_800A1E94(result);
                if (!mode) {
                    if (D_80144030[index].flag6) D_80144030[index].error = 3;
                    else if (D_80144030[index].state && !D_80144030[index].flag9) D_80144030[index].error = 3;
                    else if (!D_80144030[index].state) D_80144030[index].error = 2;
                    pfs_unlock();
                    D_8011EAE8 = index;
                    D_80144008(index, D_80144030[index].error, 0, 0, &retry, &callback_value, 0);
                    D_8011EAE8 = -1;
                    pfs_lock();
                    if (retry) goto write_retry;
                }
                if (!mode) {
                    D_80144030[index].slots[slot_index].modified = 0;
                    func_800A1BB4(index);
                }
                return 0;
            }
            func_800A1DD4(handle, address, bytes, 0);
        } while (func_800A1C6C(handle, &address, &bytes, &offset));
    }
    D_80144030[index].slots[slot_index].modified = 0;
    func_800A1BB4(index);
    return 1;
}
