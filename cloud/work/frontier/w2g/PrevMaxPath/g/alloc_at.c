/*
 * NextMaxPath (0x800A0FDC) is a historical label: this is the game heap's
 * "allocate at a fixed address" (N64-only; same block allocator as
 * src/blob/groups/audio_heap).  NextMaxPath(addr, size): under the heap lock
 * (queue D_80152770) find the heap that owns addr (func_80095F8C), round size
 * up to 32, find the block that contains addr, split off the part in front of
 * addr (or give it to the previous block when it is under 64 bytes), split
 * off the tail when 64 or more bytes remain, mark the block used and return
 * addr.  Its only caller is the overlay loader PrevMaxPath (0x800A11E4).
 *
 * Whole-program context: func_80095F8C must be an internal procedure of the
 * same -O3 unit (not in `keep`, with its real callers).  Its register summary
 * is what lets retail keep addr/size in t0/t2 across that call and what
 * narrows the temp ring to t6-t9; with func_80095F8C kept the body is 64
 * words off.  The group is src/blob/groups/codex_heap_release_a25 (group.c
 * unchanged, all members still MATCH) plus this file.  NextMaxPath itself is
 * kept: retail passes its arguments in a0/a1 and did not inline it into its
 * single caller.
 *
 * Shaping quirks (match the bytes, may not be the original spelling):
 *   - the empty `if ((u32) b + b->size + 32 < addr + size) { }` is retail's
 *     compare-and-branch whose two arms are identical (probably a removed
 *     "does not fit" report);
 *   - `prev` is assigned twice around the `b->prev->next = n` store: retail
 *     holds both loads in a0 and reloads b->prev for the store;
 *   - frame: `result` is the eleventh local (slot 28 of a 72-byte frame);
 *     unused1..unused5 stand for five locals whose names and uses are unknown.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

typedef struct Block {
    /* 0x00 */ u32 magic;
    /* 0x04 */ struct Block *next;
    /* 0x08 */ struct Block *prev;
    /* 0x0C */ u32 size;
    /* 0x10 */ void *owner;
    /* 0x14 */ s8 used;
    /* 0x15 */ u8 tag;
    /* 0x16 */ u8 pad16[10];
} Block; /* 0x20 */

typedef struct Heap {
    /* 0x00 */ u32 magic;
    /* 0x04 */ struct Heap *next;
    /* 0x08 */ Block *first;
    /* 0x0C */ Block *last;
    /* 0x10 */ u32 end;
} Heap;

extern s32 D_80152770;

s32 osRecvMesg(void *mq, void **msg, s32 flags);
s32 osJamMesg(void *mq, void *msg, s32 flags);
Heap *func_80095F8C(u32 addr);

void *NextMaxPath(u32 addr, u32 size) {
    Heap *heap;
    Block *b;
    Block *n;
    u32 before;
    Block *prev;
    s32 unused1;
    s32 unused2;
    s32 unused3;
    s32 unused4;
    s32 unused5;
    void *result;

    osRecvMesg(&D_80152770, 0, 1);
    heap = func_80095F8C(addr);
    size = (size + 31) & ~31;
    for (b = heap->first; b != 0; b = b->next) {
        if (addr >= (u32) b && (addr < (u32) b->next || b->next == 0)) {
            if ((u32) b + b->size + 32 < addr + size) {
            }
            break;
        }
    }
    result = (u8 *) b + 32;
    if (addr != (u32) result) {
        n = (Block *) (addr - 32);
        n->next = b->next;
        if (n->next != 0) {
            n->next->prev = n;
        } else {
            heap->last = n;
        }
        n->owner = 0;
        n->used = 1;
        n->tag = 0;
        n->pad16[0] = 0;
        n->magic = 0xFEDCBA98;
        before = addr - (u32) b - 32;
        n->size = b->size - before;
        if (before >= 64) {
            n->prev = b;
            b->next = n;
            b->size = b->size - n->size - 32;
        } else {
            prev = b->prev;
            n->prev = prev;
            b->prev->next = n;
            prev = b->prev;
            prev->size = prev->size + addr - (u32) b - 32;
        }
        b = n;
        result = (u8 *) b + 32;
    }
    if (b->size - size >= 64) {
        n = (Block *) ((u8 *) b + size + 32);
        n->next = b->next;
        if (n->next != 0) {
            n->next->prev = n;
        } else {
            heap->last = n;
        }
        n->prev = b;
        n->size = b->size - size - 32;
        n->owner = 0;
        n->used = 0;
        n->tag = 0;
        n->pad16[0] = 0;
        n->magic = 0xFEDCBA98;
        b->next = n;
        b->size = size;
    }
    b->owner = 0;
    b->used = 1;
    b->tag = 0;
    osJamMesg(&D_80152770, 0, 0);
    return result;
}
