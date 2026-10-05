/*
 * audio_output_setup (0x800BAF98; the name is a historical label): total free
 * bytes in a game heap, taken under the heap lock (message queue D_80152770).
 * A null heap means the default heap D_801527C8.
 *
 * func_800BAF90 (the caller-less `jr ra; nop` stub at 0x800BAF90) is the
 * deleted worker: one call site, so umerge inlines it and leaves the stub.
 * heap_or_default is the same two-return deleted static as in
 * src/blob/groups/audio_heap (its v0 result temp is the retail `move v0,v1`).
 *
 * Shaping facts, each checked by removing it:
 *  - the worker must be DEFINED AFTER its caller in the file. as1 pulls the
 *    osJamMesg argument set-up (la a0; move a1; move a2) up over the inlined
 *    loop, and places `move a2,zero` before the inlined `sum = 0` only when
 *    the call's source line is lower than the worker's lines. With the worker
 *    defined first the two moves swap (2 words).
 *  - heap_or_default as a function (ternary / if-assignment: 18-29 words).
 *  - `block = ...; sum = 0;` in that order.
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

u32 func_800BAF90(Heap *heap);

static Heap *heap_or_default(Heap *heap)
{
    if (heap != 0) {
        return heap;
    }
    return D_801527C8;
}

u32 audio_output_setup(Heap *heap)
{
    u32 sum;

    osRecvMesg(&D_80152770, 0, 1);
    sum = func_800BAF90(heap);
    osJamMesg(&D_80152770, 0, 0);
    return sum;
}

u32 func_800BAF90(Heap *heap)
{
    u32 sum;
    Block *block;

    block = heap_or_default(heap)->first;
    sum = 0;
    while (block != 0) {
        if (block->used == 0) {
            sum += block->size;
        }
        block = block->next;
    }
    return sum;
}
