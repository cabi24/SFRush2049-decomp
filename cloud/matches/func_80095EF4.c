/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned int u32;
typedef int s32;
typedef signed char s8;

typedef struct Block {
    u32 pad0;
    struct Block *next;
    u32 pad8[3];
    s8 pad14;
    s8 tag;
} Block;
typedef struct Pool {
    s32 count;
    u32 *base;
    struct Pool *next;
} Pool;
typedef struct Heap {
    u32 pad0;
    u32 pad4;
    Block *blocks;
    u32 padc[3];
    Pool pool;
} Heap;
Block *func_80095EF4(Heap *heap, u32 addr, s32 tag) {
    Pool *p;
    Block *b;
    p = &heap->pool;
    while (1) {
        if (p == 0 || p->base == 0) break;
        if (addr >= (u32)p->base && addr < (u32)(p->base + p->count)) {
            addr = *(u32 *)addr;
            break;
        }
        p = p->next;
    }
    b = heap->blocks;
    while (b != 0) {
        if (tag != b->tag || addr < (u32)b || (b->next != 0 && addr >= (u32)b->next)) {
            b = b->next;
        } else break;
    }
    return b;
}
