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

void *NextMaxPath(u32 addr, u32 size);

void *NextMaxPath(u32 addr, u32 size) {
    Heap *heap;
    Block *b;
    Block *n;
    void *result;
    u32 before;

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
            n->prev = b->prev;
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

extern s8 D_8011ED00;
extern s8 D_8011ED04;
extern u8 *D_8002B020;
extern u8 *D_8002B024;
extern s32 D_801174C0;

void audio_loop_control(void *ptr, s32 flag);
void inflate_decompress(u8 *src, u8 *dst, s32 flag);
void bzero(void *p, u32 n);
void audio_reverb_update();

void PrevMaxPath(u8 *dst, char *name, u8 *end, u8 *bssEnd, u32 romStart, u8 *bssStart, s8 *loaded, u8 *rom) {
    void *p;

    if (*loaded != 0) {
        return;
    }
    p = NextMaxPath((u32) dst, end - dst);
    audio_loop_control(p, 0);
    inflate_decompress(rom, dst, 1);
    bzero(bssStart, bssEnd - bssStart);
    *loaded = 1;
    if (romStart) {
    }
    if (name) {
    }
    if (name[0]) {
    }
}

/* deleted static (INFERRED second call site; see notes) */
void func_800A0F64(u32 addr, u32 size) {
    NextMaxPath(addr, size);
}

void InitMaxPath(void) {
    PrevMaxPath((u8 *) 0x8038A400, "Extra", (u8 *) 0x803BB380, (u8 *) 0x8039B440, 0xBE4C70, (u8 *) 0x80394F70, &D_8011ED04, D_8002B024);
}

void sync_maxpath_to_checkpoint(void) {
    PrevMaxPath((u8 *) 0x8038A400, "Start", (u8 *) 0x803BB380, (u8 *) 0x803BAE20, 0xBDA100, (u8 *) 0x803B9A50, &D_8011ED00, D_8002B020);
}
