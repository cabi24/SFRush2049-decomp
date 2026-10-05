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

extern s32 D_80152770[];

s32 osRecvMesg();
s32 osJamMesg();
extern Heap *D_801527C8;
Heap *func_80095F8C(u32 addr) {
    Heap *h;
    Heap *r = 0;
    for (h = D_801527C8; h != 0; h = h->next) {
        if (addr >= (u32) h->first && addr < h->end) {
            r = h;
        }
    }
    return r;
}

void *NextMaxPath(u32 addr, u32 size) {
    Heap *heap;
    Block *b;
    Block *n;
    void *result;
    u32 before;
    Block *prev;

    osRecvMesg(D_80152770, 0, 1);
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
            b->prev->size = b->prev->size + addr - (u32) b - 32;
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
    osJamMesg(D_80152770, 0, 0);
    return result;
}


/* STAND-IN second caller of func_80095F8C (experiment only) */
Heap *standin_heap(u32 a) {
    return func_80095F8C(a);
}
