typedef signed char s8;typedef unsigned char u8;typedef short s16;typedef unsigned short u16;
typedef signed int s32;typedef unsigned int u32;typedef float f32;
#define NULL ((void *)0)
#define M2C_FIELD(expr,type,offset) (*(type)((s8 *)(expr)+(offset)))
typedef struct OSMesgQueue OSMesgQueue;
s32 osRecvMesg(OSMesgQueue *,void **,s32);s32 osJamMesg(OSMesgQueue *,void *,s32);
extern s32 D_801527C8;
extern OSMesgQueue D_80152770;

/* ---- heap release (copied from src/blob/groups/codex_heap_release_a25, matched there) ---- */
typedef struct Block {u32 pad0;struct Block *next;u32 pad8[3];s8 pad14; s8 tag; u8 counter;u8 pad23[9];} Block;
typedef struct Pool {s32 count;u32 *base;struct Pool *next;} Pool;
typedef struct Heap {u32 pad0;u32 pad4;Block *blocks;u32 padc[3];Pool pool;} Heap;
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

void *audio_buffer_sync(u32 address) {
 void *owner,*block,*result;
 osRecvMesg((OSMesgQueue *)&D_80152770,NULL,1);
 owner=func_80095F8C(address);
 block=func_80095EF4(owner,address,0);
 result=M2C_FIELD(block,void **,12);
 osJamMesg((OSMesgQueue *)&D_80152770,NULL,0);
 return result;
}

/* ---- menu objects ---- */
typedef struct MenuData {
    u8 pad0[5];
    s8 loaded;        /* 5 */
    s8 index;         /* 6 */
    u8 variant;       /* 7 */
    u8 pad8[14];
    char name[18];    /* 22 */
    s32 key0;         /* 40 */
    s32 key1;         /* 44 */
    u8 pad48[12];
    s32 size;         /* 60 */
    s32 tag;          /* 64 */
    u8 pad68[8];
    s32 buffer;       /* 76 */
    u8 pad80[4];
    s32 count;        /* 84 */
    u8 data[4];       /* 88 */
} MenuData;
typedef struct Obj Obj;
typedef Obj **Handle;
struct Obj {
    Handle next;      /* 0 */
    Handle slot;      /* 4 */
    s8 index;         /* 8 */
    u8 variant;       /* 9 */
    char name[14];    /* 10 */
    s32 key0;         /* 24 */
    s32 key1;         /* 28 */
    f32 order;        /* 32 */
    void **h36;       /* 36 */
    void **h40;       /* 40 */
};
typedef struct Slot {
    u8 pad0[8];
    void *update;     /* 8 */
    void *render;     /* 12 */
    u8 id;            /* 16 */
    u8 pad17[47];
    s32 catchup;      /* 64 */
    u8 pad68[4];
    void **data;      /* 72 */
    s32 f76;          /* 76 */
} Slot;
typedef struct List { s32 pad0; s32 pad4; Handle head; } List;

extern List D_80152020;
extern Handle D_80152698[4];
extern char D_801211E4[], D_801211EC[], D_801211F0[], D_801211F4[];
void audio_volume_pan(void);
void func_800949D4(void);
s32 sprintf(char *, const char *, ...);
void *memcpy(void *, const void *, u32);
s32 func_8008AD04(char *, char *);
void func_800CB748(void *, void *);
void AdjustSpeed(void *);
void AdjustSteer(void *);
Handle track_process_main(s32, char *, char *, u32, s32, s32);
void slot_state_lookup(void *, void *, u32);
s32 MaxPathZeroControls(void *, s32);
s32 track_collision_setup(s32, s32);
void func_80091FBC(List *, Handle, Handle);
void drone_set_catchup(void *, s32, s32);
s32 audio_task_complete(void *, s32);
s32 inflate_entry_alt(void *, s32, s32);
s32 format_string_parse(s32, s32);
s32 draw_ui_element(Handle, s32, s32);
void menu_item_select(void *, s32);
void func_800CB9D0(void *);

void menu_transition(Handle h) {
    Obj *o = *h;
    void **old = o->h36;

    if (old != NULL) {
        osRecvMesg(&D_80152770, NULL, 1);
        audio_reverb_update((u32)old, 0);
        osJamMesg(&D_80152770, NULL, 0);
        o->h36 = NULL;
        ((Slot *)*o->h40)->f76 = 0;
        if (o->slot != NULL) {
            AdjustSteer(o->slot);
            o->h40 = NULL;
        }
    }
}

void menu_back(Handle h) {
    MenuData *m = *(MenuData **)(*h)->h40;
    s32 n;

    if (m->buffer == 0) {
        (*h)->h36 = (void **)audio_task_complete(NULL, m->tag);
        m->buffer = *(s32 *)(*h)->h36;
        n = inflate_entry_alt(m->data, m->size, m->buffer);
        m->count = n / 3;
        format_string_parse(m->buffer, n);
        m->loaded = 1;
    }
}

s32 func_800CBF2C(Handle h, s32 back, s32 select) {
    Obj *o = *h;
    void **data;

    if (o->h40 != NULL) {
        if (o->slot == NULL) {
            if (back) {
                menu_back(h);
            }
            return 1;
        }
        menu_transition(h);
    }
    drone_set_catchup(o->slot, 0, ((Slot *)*o->slot)->catchup);
    data = ((Slot *)*o->slot)->data;
    if (data == NULL) {
        return 0;
    }
    o->h40 = data;
    ((Slot *)*data)->f76 = 0;
    if (draw_ui_element(h, 0, 1) && !draw_ui_element(h, 1, 1)) {
        return 0;
    }
    if (back) {
        menu_back(h);
    }
    if (select) {
        menu_item_select(o->h40, 88);
        func_800CB9D0(o->h40);
        func_800CB9D0(o->h36);
        ((Slot *)*o->h40)->f76 = *(s32 *)*o->h36;
    }
    return 1;
}

s32 func_800CC040(s32 id, Handle owner, Handle self, s32 release) {
    MenuData *m;
    Obj *obj;
    Handle node;
    Handle *slot;
    Handle created;
    u32 size;
    void **old;
    void **gone;
    s32 i;
    char buf[32];

    m = *(MenuData **)(*self)->h40;
    if (release) {
        func_800CB748(m, m->data);
        gone = (*self)->h40;
        osRecvMesg(&D_80152770, NULL, 1);
        audio_reverb_update((u32)gone, 0);
        osJamMesg(&D_80152770, NULL, 0);
        *self = NULL;
        return 1;
    }
    for (node = D_80152020.head; node != NULL; node = obj->next) {
        obj = *node;
        if (id == ((Slot *)*obj->slot)->id && func_8008AD04(obj->name, m->name) == 0 &&
            m->key0 == obj->key0 && m->key1 == obj->key1 &&
            obj->index == m->index && obj->variant == m->variant) {
            for (i = 0; i < 4; i++) {
                if (D_80152698[i] == node) {
                    D_80152698[i] = NULL;
                    break;
                }
            }
            if (func_800CBF2C(node, 0, 0)) {
                func_800CB748(*obj->h40, ((MenuData *)*obj->h40)->data);
                if ((*node)->slot != NULL) {
                    AdjustSpeed((*node)->slot);
                }
            }
            break;
        }
    }
    obj = *self;
    size = (u32)audio_buffer_sync((u32)obj->h40);
    sprintf(buf, D_801211E4, (char *)*M2C_FIELD(*owner, void ***, 8) + 18, m->index + 1,
            m->variant ? D_801211EC : D_801211F0);
    created = track_process_main(id, buf, D_801211F4, size, 0x3544, 0x4E525545);
    if (created == NULL) {
        return 0;
    }
    obj->slot = created;
    ((Slot *)*created)->update = audio_volume_pan;
    ((Slot *)*obj->slot)->render = func_800949D4;
    memcpy(*((Slot *)*created)->data, *obj->h40, size);
    old = obj->h40;
    osRecvMesg(&D_80152770, NULL, 1);
    audio_reverb_update((u32)old, 0);
    osJamMesg(&D_80152770, NULL, 0);
    obj->h40 = ((Slot *)*created)->data;
    slot_state_lookup(created, *obj->h40, size);
    MaxPathZeroControls(created, 0);
    track_collision_setup(id, 1);
    AdjustSteer(created);
    obj->h40 = NULL;
    obj->h36 = NULL;
    for (node = D_80152020.head; node != NULL; node = (*node)->next) {
        if (obj->order < (*node)->order) {
            break;
        }
    }
    func_80091FBC(&D_80152020, self, node);
    return 1;
}

s32 menu_item_value_get(Handle h) {
    return func_800CBF2C(h, 1, 1);
}
