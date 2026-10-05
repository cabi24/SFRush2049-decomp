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
    u32 largest;
    u32 total;
    Block *n;
    u32 bs;
    Block *b;

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
    if (b->size - size >= 64) {
        n = (Block *)((u8 *)b + b->size - size);
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
