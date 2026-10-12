void func_800A1DD4(C96Handle *handle, u8 *address, u32 bytes, s32 mark) {
    C96File *file = handle->file;
    u8 *map;
    u32 first, last, i;
    u8 *end;
    if (address < *file->data || address >= *file->data + file->data_size) {
    }
    end = address + bytes;
    bytes = (u32)(address - *file->data) >> 5;
    last = (u32)(end - *file->data - 1) >> 5;
    map = *file->data + file->data_size;
    for (i = bytes; i <= last; i++) {
        if (mark) map[i >> 3] |= 1 << (i & 7);
        else map[i >> 3] &= ~(1 << (i & 7));
    }
}
