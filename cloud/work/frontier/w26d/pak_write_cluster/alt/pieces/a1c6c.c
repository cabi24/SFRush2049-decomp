s32 func_800A1C6C(C96Handle *handle, u8 **address, s32 *bytes, s32 *offset) {
    u8 complete = 0;
    C96File *file = handle->file;
    u8 *map;
    u8 *scan;
    u32 i, j;
    if (!file->data) return 0;
    *bytes = 0;
    map = *file->data + file->data_size;
    for (i = 0; i < D_80144030[file->channel].slots[file->slot].map_bytes; i++) {
        if (map[i] != 0) {
        for (j = 0; j < 8; j++) {
            if (map[i] & (1 << j)) {
                if (*bytes == 0) {
                    *offset = (i * 8 + j) * 32;
                    *address = *file->data + *offset;
                    *bytes = 1;
                } else (*bytes)++;
            } else if (*bytes) {
                complete = 1;
                break;
            }
        }
        if (complete) break;
        }
    }
    *bytes *= 32;
    return (*bytes == 0) ^ 1;
}
