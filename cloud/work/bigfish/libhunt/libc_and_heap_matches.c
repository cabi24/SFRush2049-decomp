/* Verified by: python3 tools/cloud/score.py fn cloud/work/bigfish/libhunt/libc_and_heap_matches.c <name> --flags "-g0 -O2 -mips2 -G 0 -non_shared"
 * Each function below reports MATCH on its own (one function scored per run).
 * Found by the round-3 "bigfish" scouting pass (see LIBRARY_HUNT.md). Not spliced. */
typedef unsigned int u32;
typedef int s32;
typedef signed char s8;

/* libc ANSI rand(): seed at D_8011735C. No callers (all inlined elsewhere). */
extern int D_8011735C;
int func_8008B2B4(void) {
    D_8011735C = D_8011735C * 1103515245 + 12345;
    return (D_8011735C >> 16) & 0x7fff;
}

/* libc strncmp */
int func_800950AC(const unsigned char *s1, const unsigned char *s2, unsigned int n) {
    if (n == 0) return 0;
    while (n-- != 0 && *s1 == *s2) {
        if (n == 0 || *s1 == 0 || *s2 == 0) break;
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

/* heap region lookup (range check against a region list at D_801527C8) */
typedef struct HeapRegion {
    u32 pad0;
    struct HeapRegion *next;
    u32 start;
    u32 pad0c;
    u32 end;
} HeapRegion;
extern HeapRegion *D_801527C8;
HeapRegion *func_80095F8C(u32 addr) {
    HeapRegion *r = D_801527C8;
    HeapRegion *found = 0;
    while (r != 0) {
        if (addr >= r->start && addr < r->end) {
            found = r;
        }
        r = r->next;
    }
    return found;
}

/* heap block lookup: pool list at heap+0x18, block list at heap+8 */
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

/* generic list container: init (see func_80091FBC insert-after, func_8009211C remove).
 * flag0 = "indirect" (items are handles), flag1 = doubly linked. Never called directly. */
typedef unsigned char u8;
typedef struct List { u8 indirect; u8 doubly; u8 pad[2]; int count; void *head; void *tail; } List;
void func_800A44E8(List *l, u8 indirect, u8 doubly) {
    l->indirect = indirect;
    l->doubly = doubly;
    l->count = 0;
    l->tail = 0;
    l->head = 0;
}
