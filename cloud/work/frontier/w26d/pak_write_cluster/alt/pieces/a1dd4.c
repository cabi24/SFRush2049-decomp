void func_800A1DD4(C96Handle *handle, u8 *address, u32 bytes, s32 mark) {
    C96File *file = handle->file;
    u8 *map;
    u32 first, last;
    if (address < *file->data || address >= *file->data + file->data_size) {
    }
    last = (bytes = (u32)(address + bytes)) - (u32)*file->data - 1 >> 5;
    first = (bytes = (u32)(address - *file->data)) >> 5;
    map = *file->data + file->data_size;
    for (; first <= last; first++) {
        if (mark) map[first >> 3] |= 1 << (first & 7);
        else map[first >> 3] &= ~(1 << (first & 7));
    }
}
