/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * sound_play_menu (0x800CC3C0; the name is a historical label): allocate from the END of a game
 * heap under the heap lock: round size to 32, walk heap->last backwards for the first free block
 * that fits (tracking total/largest free like audio_helper), split its tail off as the new block
 * when the remainder is >= 64 bytes, mark used (owner 0, tag 0), return the data pointer.
 * Mirror image of audio_helper in src/blob/groups/audio_heap (same Block/Heap layout).
 *
 * Shaping quirk (w9d): the dead reassignment `bs = b->size;` right after computing n. It kills the
 * availability of n's defining expression (b + bs - size), so uopt does not copy-propagate it into
 * the two link blocks; n stays one web (a0), the post-loop load keeps v1, and both osJamMesg zero
 * arguments are hoisted as in retail. `bs -= size;` in the same place also matches. Found with the
 * traced uopt (force n=a0 / load=v1 on a variant reproduced retail first). Earlier w2e/w4a residual
 * (14/83, ~300 variants) was exactly this copy-propagation split of n.
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
} Heap;
typedef struct OSMesgQueue OSMesgQueue;
extern Heap *D_801527C8;
extern OSMesgQueue D_80152770;
s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osJamMesg(OSMesgQueue *, void *, s32);

static Heap *heap_or_default(Heap *heap)
{
    if (heap != 0) {
        return heap;
    }
    return D_801527C8;
}

void *sound_play_menu(Heap *heap, u32 size)
{
    Block *n;
    Block *b;
    u32 total;
    u32 largest;
    u32 bs;

    osRecvMesg(&D_80152770, 0, 1);
    total = 0;
    largest = 0;
    heap = heap_or_default(heap);
    size = (size + 31) & ~31;
    for (b = heap->last; b != 0; b = b->prev) {
        if (b->used == 0) {
            bs = b->size;
            total += bs;
            if (largest < bs) {
                largest = bs;
            }
            if (bs >= size) {
                break;
            }
        }
    }
    bs = b->size;
    if (bs - size >= 64) {
        n = (Block *)((u8 *)b + bs - size);
        bs = b->size;
        n->next = b->next;
        if (n->next != 0) {
            n->next->prev = n;
        } else {
            heap->last = n;
        }
        n->prev = b;
        n->size = size;
        n->owner = 0;
        n->used = 1;
        n->tag = 0;
        n->pad16[0] = 0;
        n->magic = 0xFEDCBA98;
        b->next = n;
        b->size = b->size - size - 32;
        b = n;
    } else {
        b->owner = 0;
        b->used = 1;
        b->tag = 0;
    }
    osJamMesg(&D_80152770, 0, 0);
    return (u8 *)b + 32;
}
