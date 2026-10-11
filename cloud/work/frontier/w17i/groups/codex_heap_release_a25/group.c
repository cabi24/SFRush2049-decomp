typedef signed char s8;typedef unsigned char u8;typedef signed int s32;typedef unsigned int u32;
#define NULL ((void *)0)
#define M2C_FIELD(expr,type,offset) (*(type)((s8 *)(expr)+(offset)))
typedef struct Block {u32 pad0;struct Block *next;u32 pad8[3];s8 pad14; s8 tag; u8 counter;u8 pad23[9];} Block;
typedef struct Pool {s32 count;u32 *base;struct Pool *next;} Pool;
typedef struct Heap {u32 pad0;u32 pad4;Block *blocks;u32 padc[3];Pool pool;} Heap;
typedef struct OSMesgQueue OSMesgQueue;
extern s32 D_801527C8,D_80152770;
extern s8 D_8011ED04,D_8011ED00;
extern u8 D_8038A400[];
typedef struct RaceEntry {u32 allocation;u8 pad4[20];} RaceEntry;
extern RaceEntry D_8013FEF4[];
s32 osRecvMesg(OSMesgQueue *,void **,s32);s32 osJamMesg(OSMesgQueue *,void *,s32);
void func_80095EC0(void *,u32);
void *func_80095F8C(u32 arg0) {
    s32 var_v0;
    s32 var_v1;

    var_v0 = D_801527C8;
    var_v1 = 0;
    if (var_v0 != 0) {
        do {
            if ((arg0 >= (u32) M2C_FIELD(var_v0, u32 *, 8)) && (arg0 < (u32) M2C_FIELD(var_v0, u32 *, 0x10))) {
                var_v1 = var_v0;
            }
            var_v0 = M2C_FIELD(var_v0, s32 *, 4);
        } while (var_v0 != 0);
    }
    return (void *) var_v1;
}
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
void audio_reverb_update(u32 address,s32 tag) {
 Heap *heap; Block *block,*next,*prev;
 heap=func_80095F8C(address);
 block=func_80095EF4(heap,address,tag);
 func_80095EC0((u8 *)block+32,block->pad8[1]);
 if(block->pad8[2]) *(u32 *)block->pad8[2]=0;
 next=block->next;
 if(next && next->pad14==0) {
  block->next=next->next;
  if(block->next) block->next->pad8[0]=(u32)block;
  else heap->padc[0]=(u32)block;
  block->pad8[1]+=next->pad8[1]+32;
  func_80095EC0(next,32);
 }
 prev=(Block *)block->pad8[0];
 block->pad8[2]=0;block->pad14=0;block->tag=0;block->counter=0;
 if(prev && prev->pad14==0) {
  prev->next=block->next;
  if(prev->next) prev->next->pad8[0]=(u32)prev;
  else heap->padc[0]=(u32)prev;
  prev->pad8[1]+=block->pad8[1]+32;
  func_80095EC0(block,32);
 }
}
/* w8a: __inline + four unused locals (w6c): umerge inlines it into func_800C885C below, and each inlined
 * instance reserves its callee's locals in the caller's frame (retail spill homes 56 / 32, frame 64). */
__inline void audio_effect_process(u32 address) {
 s32 u0, u1, u2, u3;
 osRecvMesg((OSMesgQueue *)&D_80152770,NULL,1);
 audio_reverb_update(address,0);
 osJamMesg((OSMesgQueue *)&D_80152770,NULL,0);
}
void synced_model_render(u32 address) {
 osRecvMesg((OSMesgQueue *)&D_80152770,NULL,1);
 audio_reverb_update(address,0);
 osJamMesg((OSMesgQueue *)&D_80152770,NULL,0);
}
void MP_TargetSpeed(void) {
 if(D_8011ED04) {
 osRecvMesg((OSMesgQueue *)&D_80152770,NULL,1);
 audio_reverb_update((u32)D_8038A400,0);
 osJamMesg((OSMesgQueue *)&D_80152770,NULL,0);
 D_8011ED04=0;
 }
}
void assign_default_paths(void) {
 if(D_8011ED00) {
 osRecvMesg((OSMesgQueue *)&D_80152770,NULL,1);
 audio_reverb_update((u32)D_8038A400,0);
 osJamMesg((OSMesgQueue *)&D_80152770,NULL,0);
 D_8011ED00=0;
 }
}
void stat_race_end(s32 index) {
 RaceEntry *entry=&D_8013FEF4[index];
 u32 address=entry->allocation;
 osRecvMesg((OSMesgQueue *)&D_80152770,NULL,1);
 audio_reverb_update(address,0);
 osJamMesg((OSMesgQueue *)&D_80152770,NULL,0);
}
void *audio_buffer_sync(u32 address) {
 void *owner,*block,*result;
 osRecvMesg((OSMesgQueue *)&D_80152770,NULL,1);
 owner=func_80095F8C(address);
 block=func_80095EF4(owner,address,0);
 result=M2C_FIELD(block,void **,12);
 osJamMesg((OSMesgQueue *)&D_80152770,NULL,0);
 return result;
}
void object_counter_decrement(u32 address) {
 void *owner,*block; s32 count;
 osRecvMesg((OSMesgQueue *)&D_80152770,NULL,1);
 owner=func_80095F8C(address);
 block=func_80095EF4(owner,address,0);
 count=M2C_FIELD(block,u8 *,22);
 if(count>0)M2C_FIELD(block,u8 *,22)=count-1;
 osJamMesg((OSMesgQueue *)&D_80152770,NULL,0);
}
void object_counter_increment(u32 address) {
 void *owner,*block; s32 count;
 osRecvMesg((OSMesgQueue *)&D_80152770,NULL,1);
 owner=func_80095F8C(address);
 block=func_80095EF4(owner,address,0);
 count=M2C_FIELD(block,u8 *,22);
 if(count<255)M2C_FIELD(block,u8 *,22)=count+1;
 osJamMesg((OSMesgQueue *)&D_80152770,NULL,0);
}

/*
 * menu_item_select (historical label, misleading): heap block resize in place.
 * (address, desired) -> takes the heap message-queue lock (D_80152770), rounds
 * desired up to 32, finds the owning heap (func_80095F8C) and block header
 * (func_80095EF4, tag 0), then
 *   shrink, successor absent or in use: split off a free block when at least
 *     64 bytes remain (new size = old - desired - 32);
 *   shrink, successor free: when at least 32 bytes remain, move the
 *     successor's header down (new size = old - desired + successor);
 *   grow: successor is taken without a null/used test; when
 *     successor - desired + old < 32 it is absorbed whole (size = old +
 *     successor + 32), else its header moves up (old + successor - desired).
 * Block header: magic 0xFEDCBA98, next, prev, size, owner, used, tag, count.
 * Heap +0xC is the tail block. No arcade ancestor identified (N64 allocator).
 *
 * Whole-program member: func_80095F8C preserves a1/a3 and func_80095EF4
 * preserves a3/t0 for this caller, so it only reproduces with both real
 * lookups (internal) in the unit. Everything above this comment is the locked
 * codex_heap_release_a25 group source, unchanged.
 *
 * Shaping notes: `block->next == NULL` / `moved->next = block->next` use the
 * field expression (v1) while `next` is its own variable (a1); the grow path
 * reads the successor size once into a local for the test only.
 */
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

/* w8a: func_800C885C -- frees the two cached audio heap handles D_8011025C / D_80110260 (the first half of
 * func_800C8918's shutdown): for each non-zero handle, audio_effect_process(handle) (inlined), then zero it.
 * audio_reverb_update is internal and clobbers s0/s1, which is why this caller saves them unused. */
extern u32 D_8011025C;
extern u32 D_80110260;

void func_800C885C(void) {
    if (D_8011025C) {
        audio_effect_process(D_8011025C);
        D_8011025C = 0;
    }
    if (D_80110260) {
        audio_effect_process(D_80110260);
        D_80110260 = 0;
    }
}
