/*
 * Largest free block in a selected game heap, under its message-queue lock.
 * func_800E79F8: 0x800E79F8..0x800E7A98, 160 bytes.
 *
 * The two-return heap selector and wrapper/worker structure follow the
 * accepted frontier_heap_free_total group. The immediately preceding
 * callerless stub func_800E79F0 has the same placement as that group's
 * func_800BAF90 worker. Its assignment here is a source-boundary inference,
 * not a recovered original declaration or an arcade ancestry claim.
 *
 * The meaningful worker is defined after the wrapper, as in free-total.
 * IDO O3 inlines it, leaves the real 8-byte stub, and schedules the unlock
 * arguments before the scan. No filler locals or artificial reads are used.
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
} Heap;

typedef struct OSMesgQueue OSMesgQueue;

extern Heap *D_801527C8;
extern OSMesgQueue D_80152770;

s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osJamMesg(OSMesgQueue *, void *, s32);

u32 func_800E79F0(Heap *heap);

static Heap *heap_or_default(Heap *heap)
{
    if (heap != 0) {
        return heap;
    }
    return D_801527C8;
}

u32 func_800E79F8(Heap *heap)
{
    u32 largest;

    osRecvMesg(&D_80152770, 0, 1);
    largest = func_800E79F0(heap);
    osJamMesg(&D_80152770, 0, 0);
    return largest;
}

u32 func_800E79F0(Heap *heap)
{
    u32 largest;
    Block *block;

    block = heap_or_default(heap)->first;
    largest = 0;
    while (block != 0) {
        if (block->used == 0) {
            if (largest < block->size) {
                largest = block->size;
            }
        }
        block = block->next;
    }
    return largest;
}
