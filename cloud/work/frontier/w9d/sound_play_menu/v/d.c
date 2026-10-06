/* flags: -g0 -O3 -mips2 -G 0 -non_shared -- NOT A MATCH: 14/83 words (score.py fn; same 14 in blob_unit). */
/*
 * sound_play_menu (0x800CC3C0; the name is a historical label): allocate from the END of a game
 * heap under the heap lock: round size to 32, walk heap->last backwards for the first free block
 * that fits (tracking total/largest free like audio_helper), split its tail off as the new block
 * when the remainder is >= 64 bytes, mark used (owner 0, tag 0), return the data pointer.
 * Mirror image of audio_helper in src/blob/groups/audio_heap (same Block/Heap layout).
 *
 * Residual (workbench diagnose: pool lane, webs a0->a1 x4 and a1->a2 x1): retail colours the new
 * block n into a0 and hoists both osJamMesg zero arguments (move a1,zero; move a2,zero) above
 * the split test; here n takes a1 (then a copy to a0, +1 word) and only a2's zero is hoisted.
 * Tried without effect (~300 variants): all 120 declaration orders x 2 forms; n/link/return
 * spellings (32); osJamMesg prototypes/casts/unprototyped; inlined lock/unlock helpers; tail
 * worker helpers (static / kept, before / after the caller, with/without owner/tag params:
 * all worse); result variable r; loop over n; heap copy local; #line probes (line order is not
 * the lever here, unlike audio_output_setup); -O2 (80 words).
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
        n = (Block *)((u8 *)b + bs); n = (Block *)((u8 *)n - size);
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
