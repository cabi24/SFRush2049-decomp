/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * car_damage_visual (historical label) @ 0x800A51E0, 680 bytes: heap compaction.
 * Under the heap message-queue lock (D_80152770): (1) releases every used block that has no
 * owner pointer, no tag and a zero lock count; (2) for each remaining movable block (used, owner
 * pointer set, lock count zero) looks for an earlier free block that is large enough, or the free
 * block directly in front of it, moves the payload down with memmove (func_800A47C0), repoints
 * the owner (*ref = new payload), splits off the remainder as a new header (when adjacent, or
 * when at least 64 bytes remain) and releases the old block (audio_reverb_update = heap release).
 * No arcade ancestor found; N64 allocator module (0xFEDCBA98 block marker).
 *
 * func_800A51D8 is the retail `jr ra; nop` stub directly in front of this function: a deleted
 * static, the "heap or default heap" getter, inlined here. It must not be on the keep list.
 * audio_reverb_update is internal in retail (arguments arrive in a1/a2), so this body only
 * reproduces with the real release module in the unit (group.c and alloc_at.c, unchanged
 * copies of src/blob/groups/codex_heap_release_a25/).
 *
 * Shaping facts (each needed):
 * - the scan loop keeps `next` as a variable set on both paths (not `cursor = cursor->next`);
 * - no variable for the payload address: `(u8 *)block + 32` is written at both uses;
 * - `size` is assigned, then the memmove argument re-reads `block->size`;
 * - the remainder's size is stored before its ref/used/tag/lock fields;
 * - `if (block->magic != MAGIC) after = previous->next; else after = block->next;`.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;
#define NULL ((void *)0)
#define BLOCK_MAGIC 0xFEDCBA98

typedef struct Block {
    /* 0x00 */ u32 magic;
    /* 0x04 */ struct Block *next;
    /* 0x08 */ struct Block *prev;
    /* 0x0C */ u32 size;           /* payload bytes, header excluded */
    /* 0x10 */ u32 *ref;           /* owner's pointer to the payload, or NULL */
    /* 0x14 */ s8 used;
    /* 0x15 */ s8 tag;
    /* 0x16 */ u8 lock;
    /* 0x17 */ u8 pad17[9];
} Block;                           /* 0x20 */

typedef struct Heap {
    /* 0x00 */ u32 unk0;
    /* 0x04 */ struct Heap *next;
    /* 0x08 */ Block *first;
    /* 0x0C */ Block *last;
} Heap;

typedef struct OSMesgQueue OSMesgQueue;
extern Heap *D_801527C8;
extern s32 D_80152770;
s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osJamMesg(OSMesgQueue *, void *, s32);
void audio_reverb_update(u32 address, s32 tag);
void *func_800A47C0(void *dst, const void *src, u32 n);

Heap *func_800A51D8(Heap *heap) {
    if (heap) return heap;
    return D_801527C8;
}

void car_damage_visual(Heap *hint) {
    Heap *heap;
    Block *block, *cursor, *after, *tail, *previous;
    Block *next;
    s32 adjacent;
    u32 size, difference;

    osRecvMesg((OSMesgQueue *)&D_80152770, NULL, 1);
    heap = func_800A51D8(hint);
    block = heap->first;
    while (block != NULL) {
        if (block->used && !block->lock && !block->ref && !block->tag) {
            audio_reverb_update((u32)((u8 *)block + 32), 0);
            block = heap->first;
        }
        block = block->next;
    }
    block = heap->first;
    while (block != NULL) {
        after = block->next;
        if (block->used && block->ref && !block->lock) {
            cursor = heap->first;
            adjacent = 0;
            while (cursor != NULL) {
                if (cursor == block) {
                    cursor = NULL;
                    break;
                }
                if (cursor->used) {
                    next = cursor->next;
                } else {
                    if (cursor->size >= block->size) break;
                    next = cursor->next;
                    if (block == next) {
                        adjacent = 1;
                        break;
                    }
                }
                cursor = next;
            }
            if (cursor != NULL) {
                cursor->ref = block->ref;
                *cursor->ref = (u32)((u8 *)cursor + 32);
                cursor->used = 1;
                cursor->tag = block->tag;
                cursor->lock = block->lock;
                size = block->size;
                block->ref = 0;
                func_800A47C0((u8 *)cursor + 32, (u8 *)block + 32, block->size);
                if (adjacent || cursor->size - size >= 64) {
                    tail = (Block *)((u8 *)cursor + size + 32);
                    if (adjacent) tail->next = after;
                    else tail->next = cursor->next;
                    if (tail->next != NULL) tail->next->prev = tail;
                    else heap->last = tail;
                    tail->prev = cursor;
                    if (adjacent) difference = 0; else difference = size + 32;
                    tail->size = cursor->size - difference;
                    tail->ref = 0;
                    tail->used = adjacent;
                    tail->tag = 0;
                    tail->lock = 0;
                    tail->magic = BLOCK_MAGIC;
                    cursor->next = tail;
                    cursor->size = size;
                    if (adjacent) block = tail;
                }
                previous = block->prev;
                audio_reverb_update((u32)((u8 *)block + 32), 0);
                if (after->magic != BLOCK_MAGIC) {
                    if (block->magic != BLOCK_MAGIC) after = previous->next;
                    else after = block->next;
                }
            }
        }
        block = after;
    }
    osJamMesg((OSMesgQueue *)&D_80152770, NULL, 0);
}
