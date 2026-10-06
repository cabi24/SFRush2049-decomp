/*
 * Game heap ("audio_*" names are historical): a first-fit block allocator
 * (32-byte block headers, magic 0xFEDCBA98) with a per-heap table of handles.
 * Hand-written from the assembly.
 *
 * Frontier wave 1 (w1a) changes against src/blob/groups/audio_heap/group.c:
 *  - audio_helper's real parameter order is (heap, size, owner, tag). IPA still
 *    colours size->a0 and heap->a2, so the callee's words are unchanged, but
 *    the callers evaluate the heap argument first.
 *  - heap_or_default() is a deleted static with two returns, inlined at its
 *    three call sites (probably the jr-ra stub func_80097468). Its inlined
 *    result temp is the v0 that retail moves into a2/s2; a ternary or an
 *    if-assignment at the call site is coloured straight into a2 instead.
 *  - audio_task_complete: declaration order h, slot, t, i gives the 48-byte frame.
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

typedef struct HandleTable {
    /* 0x0 */ u32 count;
    /* 0x4 */ u32 *slots;
    /* 0x8 */ struct HandleTable *next;
} HandleTable;

typedef struct Heap {
    /* 0x00 */ u32 magic;
    /* 0x04 */ struct Heap *next;
    /* 0x08 */ Block *first;
    /* 0x0C */ Block *last;
    /* 0x10 */ u32 end;
    /* 0x14 */ u16 maxHandles;
    /* 0x16 */ u16 pad16;
    /* 0x18 */ HandleTable tbl;
    /* 0x24 */ u8 pad24[4];
} Heap;

extern u8 D_8017A640[];
extern Heap *D_801527C8;
extern s32 D_80152770[];
extern s32 D_801527A0;
extern s8 D_80116488;
extern s8 D_80156994;
extern u32 D_80000318;

s32 osRecvMesg();
s32 osJamMesg();
void osCreateMesgQueue();
void *memset();

void *audio_helper(Heap *heap, u32 size, void *owner, s32 tag);
void func_800E7B44(Heap *heap, u16 count, u16 a);
void *audio_dma_sync(Heap *heap, u32 size);
void *audio_task_complete(Heap *heap, u32 size);
void *func_800E7C2C(Heap *heap, u32 size, u16 count, u16 a);
void func_800E7D0C(u16 count, u16 a);

static Heap *heap_or_default(Heap *heap)
{
    if (heap != 0) {
        return heap;
    }
    return D_801527C8;
}

void *audio_helper(Heap *heap, u32 size, void *owner, s32 tag)
{
    Block *b;
    Block *n;
    u32 total = 0;
    u32 largest = 0;
    u32 bs;

    size = (size + 31) & ~31;
    for (b = heap->first; b != 0; b = b->next) {
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
        n = (Block *)((u8 *)b + size + 32);
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
    b->used = 1;
    b->owner = owner;
    b->tag = tag;
    return (u8 *)b + 32;
}

void func_800E7B44(Heap *heap, u16 count, u16 a)
{

    if (a == 0) {
    }
    heap->magic = 0xFEDCBA98;
    heap->next = 0;
    heap->first = (Block *)(((u32)heap + 67) & ~31);
    heap->first->magic = 0xFEDCBA98;
    heap->first->next = 0;
    heap->first->prev = 0;
    heap->first->size = heap->end - (u32)heap->first - 32;
    heap->first->owner = 0;
    heap->first->used = 0;
    heap->first->tag = 0;
    heap->first->pad16[0] = 0;
    heap->last = heap->first;
    heap->tbl.count = count;
    heap->maxHandles = a;
    heap->tbl.next = 0;
    if (count > 0) {
        heap->tbl.slots = audio_helper(heap, count * 4, 0, 1);
        memset(heap->tbl.slots, 0, count * 4);
    } else {
        heap->tbl.slots = 0;
    }
}

void *audio_dma_sync(Heap *heap, u32 size)
{
    void *p;

    osRecvMesg(D_80152770, 0, 1);
    p = audio_helper(heap_or_default(heap), size, 0, 0);
    osJamMesg(D_80152770, 0, 0);
    return p;
}

void *audio_task_complete(Heap *heap, u32 size)
{
    Heap *h;
    u32 *slot;
    HandleTable *t;
    u32 i;

    osRecvMesg(D_80152770, 0, 1);
    h = heap_or_default(heap);
    t = &h->tbl;
    if (t->slots == 0) {
        t->count = h->maxHandles;
        t->slots = audio_helper(h, h->maxHandles * 4, 0, 1);
        memset(t->slots, 0, h->maxHandles * 4);
        t->next = 0;
    }
    i = 0;
    for (;;) {
        slot = t->slots;
        for (; i < t->count; i++, slot++) {
            if (*slot == 0) {
                break;
            }
        }
        if (i < t->count) {
            break;
        }
        i = 0;
        if (t->next == 0) {
            t->next = audio_helper(h, 12, 0, 1);
            t->next->count = h->maxHandles;
            t->next->slots = audio_helper(h, h->maxHandles * 4, 0, 1);
            memset(t->next->slots, 0, h->maxHandles * 4);
            t->next->next = 0;
        }
        t = t->next;
    }
    *slot = (u32)audio_helper(h, size, slot, 0);
    osJamMesg(D_80152770, 0, 0);
    return slot;
}
void *func_800E7C2C(Heap *heap, u32 size, u16 count, u16 a)
{
    Heap *h;
    Heap *p;

    osRecvMesg(D_80152770, 0, 1);
    h = audio_helper(heap_or_default(heap), size + 64, 0, 1);
    h->end = (u32)h + ((size + 95) & ~31);
    func_800E7B44(h, count, a);
    for (p = D_801527C8; p != 0; p = p->next) {
        if (p->next == 0) {
            p->next = h;
            break;
        }
    }
    osJamMesg(D_80152770, 0, 0);
    return h;
}

void func_800E7D04(s8 *flag)
{
    if (*flag == 0) {
        *flag = 1;
        osCreateMesgQueue(D_80152770, &D_801527A0, 1);
        osJamMesg(D_80152770, 0, 0);
    }
}

void func_800E7D0C(u16 count, u16 a)
{
    Heap *h;

    func_800E7D04(&D_80116488);
    {
    h = (Heap *)(((u32)D_8017A640 + 31) & ~31);
    D_801527C8 = h;
    h->end = D_80000318 | 0x80000000;
    if (h->end >= 0x80400001) {
        D_80156994 = 1;
    }
    }
    func_800E7B44(D_801527C8, count, a);
}
