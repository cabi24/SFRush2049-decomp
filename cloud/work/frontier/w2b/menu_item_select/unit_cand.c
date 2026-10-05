typedef signed char s8;typedef unsigned char u8;typedef signed int s32;typedef unsigned int u32;
#define NULL ((void *)0)
typedef struct OSMesgQueue OSMesgQueue;
typedef struct Heap Heap;
typedef struct Block Block;
extern u8 D_80152770[];
s32 osRecvMesg(OSMesgQueue *,void **,s32);s32 osJamMesg(OSMesgQueue *,void *,s32);
void *func_80095F8C(u32 arg0);
Block *func_80095EF4(Heap *heap, u32 addr, s32 tag);
typedef struct MBlock {
    u32 magic;
    struct MBlock *next;
    struct MBlock *prev;
    u32 size;
    void *owner;
    s8 used;
    s8 tag;
    u8 count;
} MBlock;
typedef struct MHeap {
    u32 pad0[3];
    MBlock *tail;
} MHeap;

void menu_item_select(u32 address, u32 desired) {
    MHeap *heap;
    MBlock *block;
    MBlock *next;
    MBlock *moved;
    u32 size;
    u32 nsize;

    osRecvMesg((OSMesgQueue *)&D_80152770, NULL, 1);
    desired = (desired + 31) & ~31U;
    heap = (MHeap *)func_80095F8C(address);
    block = (MBlock *)func_80095EF4((Heap *)heap, address, 0);
    size = block->size;
    if (size >= desired) {
        next = block->next;
        if (block->next == NULL || next->used != 0) {
            if (size - desired >= 64) {
                moved = (MBlock *)((u8 *)block + desired + 32);
                moved->next = block->next;
                if (moved->next != NULL) {
                    moved->next->prev = moved;
                } else {
                    heap->tail = moved;
                }
                moved->prev = block;
                moved->size = block->size - desired - 32;
                moved->owner = NULL;
                moved->used = 0;
                moved->tag = 0;
                moved->count = 0;
                moved->magic = 0xFEDCBA98;
                block->next = moved;
                block->size = desired;
            }
        } else {
            if (size - desired >= 32) {
                moved = (MBlock *)((u8 *)block + desired + 32);
                moved->next = next->next;
                if (moved->next != NULL) {
                    moved->next->prev = moved;
                } else {
                    heap->tail = moved;
                }
                moved->prev = block;
                moved->size = block->size - desired + next->size;
                moved->owner = NULL;
                moved->used = 0;
                moved->tag = 0;
                moved->count = 0;
                moved->magic = 0xFEDCBA98;
                block->next = moved;
                block->size = desired;
            }
        }
    } else {
        next = block->next;
        nsize = next->size;
        if (nsize - desired + size < 32) {
            block->next = next->next;
            if (block->next != NULL) {
                block->next->prev = block;
            } else {
                heap->tail = block;
            }
            desired = block->size + next->size + 32;
        } else {
            moved = (MBlock *)((u8 *)block + desired + 32);
            moved->next = next->next;
            if (moved->next != NULL) {
                moved->next->prev = moved;
            } else {
                heap->tail = moved;
            }
            moved->prev = block;
            moved->size = block->size + next->size - desired;
            moved->owner = NULL;
            moved->used = 0;
            moved->tag = 0;
            moved->count = 0;
            moved->magic = 0xFEDCBA98;
            block->next = moved;
        }
        block->size = desired;
    }
    osJamMesg((OSMesgQueue *)&D_80152770, NULL, 0);
}
