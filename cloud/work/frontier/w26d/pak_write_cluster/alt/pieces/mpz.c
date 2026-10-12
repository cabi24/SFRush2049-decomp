s32 MaxPathZeroControls(C96Handle *handle, s32 mode) {
    pfs_lock();
    D_80144030[handle->file->channel].busy = 1;
    D_80144030[handle->file->channel].result = 0;
    D_80144030[handle->file->channel].result = track_data_decompress(handle, mode);
    D_80144030[handle->file->channel].busy = 0;
    pfs_unlock();
    return D_80144030[handle->file->channel].result;
}
