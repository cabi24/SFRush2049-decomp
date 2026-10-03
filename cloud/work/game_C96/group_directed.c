/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
#include "rom_tu.h"
typedef struct C96File C96File;
typedef struct C96Handle { C96File *file; } C96Handle;
struct C96File {
    C96Handle *next;
    u8 unknown04[12];
    u8 channel, slot;
    u8 unknown12[46];
    u32 data_size;
    u8 unknown44[4];
    u8 **data;
};
typedef struct C96Slot {
    u8 unknown0;
    s8 modified, flag2;
    u8 unknown3;
    u16 map_bytes;
    u8 unknown6[34];
} C96Slot;
typedef struct C96Channel {
    u8 unknown0;
    s8 state;
    u8 unknown2[4];
    s8 flag6, busy, result, flag9;
    u8 unknown10;
    s8 modified;
    OSPfs pfs;
    s32 error;
    u8 unknown120[12];
    C96Slot slots[16];
} C96Channel;
typedef struct C96List { C96Handle *head; u32 unknown[3]; } C96List;
extern C96Channel D_80144030[];
extern C96List D_80144D68[];
extern OSMesgQueue D_801497D0;
extern OSMesg D_801527E4;
extern s8 D_8011194C, D_8011EAE8;
extern void (*D_80144008)(s32,s32,s32,s32,s8 *,s8 *,s32);
extern s32 func_800A1E94(s32);
void func_800A1BB4(s32 channel) {
    C96Handle *node;
    C96Handle *next;
    C96File *file;
    if (!D_80144030[channel].modified) return;
    node = D_80144D68[channel].head;
    while (node) {
        file = node->file;
        next = file->next;
        if (file->data && D_80144030[channel].slots[file->slot].modified) break;
        node = next;
    }
    if (!node) D_80144030[channel].modified = 0;
}
s32 func_800A1C6C(C96Handle *handle, s32 *bytes, s32 *offset, u8 **address) {
    C96File *file = handle->file;
    C96Channel *table = D_80144030;
    u8 *map;
    u8 *scan;
    u32 i, j;
    s32 complete = 0;
    if (!file->data) return 0;
    *bytes = 0;
    map = *file->data + file->data_size;
    scan = map;
    for (i = 0; i < table[file->channel].slots[file->slot].map_bytes; i++, scan++) {
        if (*scan == 0) continue;
        for (j = 0; j < 8; j++) {
            if (*scan & (1 << j)) {
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
    *bytes *= 32;
    if (*bytes == 0) return 0;
    return 1;
}
void func_800A1DD4(C96Handle *handle, u8 *address, u32 bytes, s32 mark) {
    C96File *file = handle->file;
    u8 *base = *file->data;
    u8 *map;
    u32 first, last;
    /* The original compares the allocation bounds, then both paths select
       this same map. Keep no fabricated error side effect or return. */
    if (address >= base && address < base + file->data_size)
        map = base + file->data_size;
    else map = base + file->data_size;
    first = (u32)(address - base) >> 5;
    last = (u32)(address + bytes - base - 1) >> 5;
    for (; first <= last; first++) {
        if (mark) map[first >> 3] |= 1 << (first & 7);
        else map[first >> 3] &= ~(1 << (first & 7));
    }
}
s32 track_data_decompress(C96Handle *handle, s32 mode) {
    C96File *file;
    C96Channel *channel;
    C96Slot *slot;
    s32 index;
    s32 bytes, offset;
    u8 *address;
    s8 retry, callback_value;
    OSMesg message;
    s32 result;
    if (!handle) return 1;
    file = handle->file;
    index = file->channel;
    channel = &D_80144030[index];
    slot = &channel->slots[file->slot];
    if (!slot->modified) return 1;
    osWritebackDCache(*file->data, file->data_size);
    if (func_800A1C6C(handle, &bytes, &offset, &address)) {
        do {
            osWritebackDCache(address, bytes);
write_retry:
            result = osPfsGetFileSize(&channel->pfs, file->slot, 1, offset, bytes, address);
            if (result) {
                channel->error = func_800A1E94(result);
                if (!mode) {
                    if (channel->flag6 || (channel->state && !channel->flag9)) channel->error = 3;
                    else if (!channel->state) channel->error = 2;
                    osJamMesg(&D_801497D0, 0, 0);
                    D_8011EAE8 = index;
                    D_80144008(index, channel->error, 0, 0, &retry, &callback_value, 0);
                    D_8011EAE8 = -1;
                    if (!D_8011194C) {
                        D_8011194C = 1;
                        osCreateMesgQueue(&D_801497D0, &D_801527E4, 1);
                        osJamMesg(&D_801497D0, 0, 0);
                    }
                    osRecvMesg(&D_801497D0, &message, 1);
                    if (retry) goto write_retry;
                }
                if (!mode) {
                    slot->modified = 0;
                    func_800A1BB4(index);
                }
                return 0;
            }
            func_800A1DD4(handle, address, bytes, 0);
        } while (func_800A1C6C(handle, &bytes, &offset, &address));
    }
    slot->modified = 0;
    func_800A1BB4(index);
    return 1;
}
s32 track_collision_setup(s32 index, s32 mode) {
    C96Channel *channel = &D_80144030[index];
    C96Handle *node;
    C96Handle *next;
    C96File *file;
    OSMesg message;
    channel->result = 0;
    if (!channel->modified) return 1;
    if (!D_8011194C) {
        D_8011194C = 1;
        osCreateMesgQueue(&D_801497D0, &D_801527E4, 1);
        osJamMesg(&D_801497D0, 0, 0);
    }
    osRecvMesg(&D_801497D0, &message, 1);
    channel->busy = 1;
    node = D_80144D68[index].head;
    while (node) {
        file = node->file;
        next = file->next;
        if (file->data && channel->slots[file->slot].modified) {
            if (!track_data_decompress(node, mode)) break;
            channel->result = 1;
        }
        node = next;
    }
    channel->busy = 0;
    osJamMesg(&D_801497D0, 0, 0);
    return node == 0;
}
s32 MaxPathZeroControls(C96Handle *handle, s32 mode) {
    OSMesg message;
    if (!D_8011194C) {
        D_8011194C = 1;
        osCreateMesgQueue(&D_801497D0, &D_801527E4, 1);
        osJamMesg(&D_801497D0, 0, 0);
    }
    osRecvMesg(&D_801497D0, &message, 1);
    D_80144030[handle->file->channel].busy = 1;
    D_80144030[handle->file->channel].result = 0;
    D_80144030[handle->file->channel].result = track_data_decompress(handle, mode);
    D_80144030[handle->file->channel].busy = 0;
    osJamMesg(&D_801497D0, 0, 0);
    return D_80144030[handle->file->channel].result;
}
void slot_state_lookup(C96Handle *handle, u8 *address, u32 bytes) {
    C96File *file;
    if (!handle) return;
    file = handle->file;
    func_800A1DD4(handle, address, bytes, 1);
    D_80144030[file->channel].slots[file->slot].modified = 1;
    D_80144030[file->channel].modified = 1;
}
