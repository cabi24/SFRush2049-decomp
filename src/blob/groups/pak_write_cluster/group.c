/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * Controller-pak write cluster (w26d -> w27e -> w28b): 7/7 EQUAL in the whole-program unit, score.py group MATCH.
 * N64-only (no arcade ancestor; GC paksysdc.c is a different platform layer). Members: func_800A1BB4 (clear the
 * channel's modified flag when no open file is still dirty), func_800A1C6C (find the next dirty 32-byte run),
 * func_800A1DD4 (mark/clear the dirty bitmap for a byte range), track_data_decompress (write dirty runs with
 * osPfsReadWriteFile, retry via the error callback), track_collision_setup / MaxPathZeroControls (locked flush
 * wrappers), slot_state_lookup (mark a range dirty).
 * Unit command (repo root):
 *   python3 -m tools.conveyor.pipeline.blob_unit --tag w28b score func_800A1BB4 func_800A1C6C func_800A1DD4 \
 *     track_data_decompress track_collision_setup MaxPathZeroControls slot_state_lookup --with THIS.c \
 *     --internal func_800A1BB4 --internal func_800A1C6C --internal func_800A1DD4 --internal track_data_decompress \
 *     --internal pfs_queue_init --internal pfs_lock --internal pfs_unlock --neighbours
 * Shaping devices (disclosed):
 *  - `extern volatile C96Channel D_80144030[]` (w8b/w26d: the pak records are volatile).
 *  - func_800A1DD4: the empty range check `if (address < base || address >= base + size) {}` (from the research base;
 *    its loads survive and are retail's).
 *  - slot_state_lookup: `if (...slots[slot].flag2) {}` (compiled-out test of a volatile byte; emits only retail's lb).
 *  - pfs_queue_init/pfs_lock/pfs_unlock are non-static internal helpers (static ones leave deleted-procedure stubs).
 *  - func_800A1BB4: dead stores `next = p = 0` (early-return block, reached by `goto out`) and `next = 0` (found
 *    block), kept alive to ugen by the compiled-out holder `out: if (next) {}`; as1 deletes them as dead, which
 *    leaves retail's tail-duplicated `jr ra; nop` early return and the `b exit; nop` found-path jump-around (w28b).
 *  - func_800A1DD4: `first = bytes;` (the byte offset) is never used except by the compiled-out holder
 *    `if (first) {}` after the loop: a codeless web that occupies a3 (retail's skipped register); `bytes >>= 5`
 *    pops the ugen temp that retail pops (dest == operand). Frame 0, so no frame cost (w28b).
 * Base: dot_pak_write_research_20261006 / game_C96 (types), w25e/w26d/w27e bodies.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef struct OSThread_s OSThread;
typedef void *OSMesg;
typedef struct OSMesgQueue_s {
    OSThread *mtqueue, *fullqueue;
    s32 validCount, first, msgCount;
    OSMesg *msg;
} OSMesgQueue;
typedef struct OSPfs {
    s32 status;
    OSMesgQueue *queue;
    s32 channel;
    u8 id[32], label[32];
    s32 version, dir_size, inode_table, minode_table, dir_table;
    s32 inode_start_page;
    u8 banks, activebank;
} OSPfs;
extern void osCreateMesgQueue(OSMesgQueue *, OSMesg *, s32);
extern s32 osRecvMesg(OSMesgQueue *, OSMesg *, s32);
extern s32 osSendMesg(OSMesgQueue *, OSMesg, s32);
extern void osWritebackDCache(void *, s32);
extern s32 osPfsReadWriteFile(OSPfs *, s32, u8, int, int, u8 *);

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
    s8 modified;
    s8 flag2;
    u8 unknown3;
    u16 map_bytes;
    u16 unknown6;
    u32 allocation;
    u8 unknown12[28];
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
extern volatile C96Channel D_80144030[];
extern C96List D_80144D68[];
extern OSMesgQueue D_801497D0;
extern OSMesg D_801527E4;
extern s8 D_8011194C, D_8011EAE8;
extern void (*D_80144008)(s32,s32,s32,s32,s8 *,s8 *,s32);
extern s32 func_800A1E94(s32);
void pfs_queue_init(void) {
    if (!D_8011194C) {
        D_8011194C = 1; osCreateMesgQueue(&D_801497D0, &D_801527E4, 1);
        osSendMesg(&D_801497D0, 0, 0);
    }
}
void pfs_lock(void) {
    OSMesg message;
    pfs_queue_init();
    osRecvMesg(&D_801497D0, &message, 1);
}
void pfs_unlock(void) {
    osSendMesg(&D_801497D0, 0, 0);
}
void func_800A1BB4(s32 index) {
    C96Handle *p, *next;
    C96File *data;

    if (D_80144030[index].modified == 0) { next = p = 0; goto out; }
    next = p = D_80144D68[index].head;
    while (p != 0) {
        data = p->file;
        next = data->next;
        if (data->data != 0) {
            if (D_80144030[index].slots[data->slot].modified != 0) { next = 0; break; }
        }
        p = next;
    }
    if (p == 0) D_80144030[index].modified = 0;
out:
    if (next) {}
}
s32 func_800A1C6C(C96Handle *handle, u8 **address, s32 *bytes, s32 *offset) {
    u8 complete = 0;
    C96File *file = handle->file;
    u8 *map;
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
void func_800A1DD4(C96Handle *handle, u8 *address, u32 bytes, s32 mark) {
    C96File *file = handle->file;
    u8 *map;
    u32 first, last, i;
    u8 *end;
    if (address < *file->data || address >= *file->data + file->data_size) {
    }
    end = address + bytes;
    bytes = (u32)(address - *file->data);
    first = bytes;
    bytes >>= 5;
    last = (u32)(end - *file->data - 1) >> 5;
    map = *file->data + file->data_size;
    for (i = bytes; i <= last; i++) {
        if (mark) map[i >> 3] |= 1 << (i & 7);
        else map[i >> 3] &= ~(1 << (i & 7));
    }
    if (first) {}
}
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
s32 PS_FlushPak(s32 index, s32 mode) {
    C96Handle *node;
    C96Handle *next;
    D_80144030[index].result = 0;
    if (!D_80144030[index].modified) return 1;
    pfs_lock();
    D_80144030[index].busy = 1;
    node = D_80144D68[index].head;
    while (node) {
        next = node->file->next;
        if (node->file->data && D_80144030[index].slots[node->file->slot].modified) {
            if (!track_data_decompress(node, mode)) break;
            D_80144030[index].result = 1;
        }
        node = next;
    }
    D_80144030[index].busy = 0;
    pfs_unlock();
    return node == 0;
}
s32 PS_FlushFile(C96Handle *handle, s32 mode) {
    pfs_lock();
    D_80144030[handle->file->channel].busy = 1;
    D_80144030[handle->file->channel].result = 0;
    D_80144030[handle->file->channel].result = track_data_decompress(handle, mode);
    D_80144030[handle->file->channel].busy = 0;
    pfs_unlock();
    return D_80144030[handle->file->channel].result;
}
void PS_WriteFile(C96Handle *handle, u8 *address, u32 bytes) {
    C96File *file;
    if (!handle) return;
    file = handle->file;
    if (D_80144030[file->channel].slots[file->slot].flag2) {}
    func_800A1DD4(handle, address, bytes, 1);
    D_80144030[file->channel].slots[file->slot].modified = 1;
    D_80144030[file->channel].modified = 1;
}
