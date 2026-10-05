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
    /* 0x10 */ void **owner;
    /* 0x14 */ s8 used;
    /* 0x15 */ s8 tag;
    /* 0x16 */ u8 count;
    /* 0x17 */ u8 pad17[9];
} Block; /* 0x20 */

typedef struct Heap {
    /* 0x00 */ u32 magic;
    /* 0x04 */ struct Heap *next;
    /* 0x08 */ Block *first;
    /* 0x0C */ Block *last;
} Heap;

typedef struct OSMesgQueue OSMesgQueue;
extern OSMesgQueue D_80152770;

s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osJamMesg(OSMesgQueue *, void *, s32);
Heap *func_80095F8C(void *addr);
Block *func_80095EF4(Heap *heap, void *addr, s32 tag);
void audio_reverb_update(void *addr, s32 tag);
void *func_800A47C0(void *dst, void *src, u32 n);

void func_800CB9D0(void *addr)
{
    Heap *heap;
    Block *old;
    Block *after;
    Block *n;
    Block *b;
    Block *next;
    u32 used;
    s32 adjacent;
    u32 size;
    s32 pad[7];
    void *src;

    osRecvMesg(&D_80152770, 0, 1);
    heap = func_80095F8C(addr);
    old = func_80095EF4(heap, addr, 0);
    after = old->next;
    adjacent = 0;
    for (b = heap->first; b != 0; b = next) {
        if (b == old) {
            b = 0;
            break;
        }
        if (b->used != 0) {
            next = b->next;
        } else {
            if (b->size >= old->size) {
                break;
            }
            next = b->next;
            if (old == next) {
                adjacent = 1;
                break;
            }
        }
    }
    if (b == 0) {
        osJamMesg(&D_80152770, 0, 0);
        return;
    }
    b->owner = old->owner;
    *b->owner = (u8 *)b + 32;
    b->used = 1;
    b->tag = old->tag;
    b->count = old->count;
    size = old->size;
    old->owner = 0;
    func_800A47C0((u8 *)b + 32, (u8 *)old + 32, old->size);
    src = (u8 *)old + 32;
    if (adjacent || b->size - size >= 64) {
        n = (Block *)((u8 *)b + size + 32);
        if (adjacent) {
            n->next = after;
        } else {
            n->next = b->next;
        }
        if (n->next != 0) {
            n->next->prev = n;
        } else {
            heap->last = n;
        }
        n->prev = b;
        if (adjacent) {
            used = 0;
        } else {
            used = size + 32;
        }
        n->owner = 0;
        n->used = adjacent;
        n->size = b->size - used;
        n->tag = 0;
        n->count = 0;
        n->magic = 0xFEDCBA98;
        b->next = n;
        b->size = size;
        if (adjacent) {
            src = (u8 *)n + 32;
        }
    }
    audio_reverb_update(src, 0);
    osJamMesg(&D_80152770, 0, 0);
}
