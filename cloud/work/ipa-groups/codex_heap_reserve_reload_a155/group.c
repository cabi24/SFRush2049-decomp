/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
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
void audio_effect_process(u32 address) {
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

void *func_800A47C0(void *dst, const void *src, u32 n) {
    char *d = (char *)dst;
    const char *s = (const char *)src;
    if (s < d && d < s + n) {
        d += n;
        s += n;
        if (((u32)d & 3) == 0 && ((u32)s & 3) == 0) {
            while (n >= 4) {
                *(u32 *)(d -= 4) = *(const u32 *)(s - 4);
                n -= 4; s -= 4;
            }
        }
        while (n--) { *--d = *--s; }
    } else {
        if (((u32)d & 3) == 0 && ((u32)s & 3) == 0) {
            while (n >= 4) {
                *(u32 *)d = *(const u32 *)s;
                d += 4; s += 4; n -= 4;
            }
        }
        while (n--) { *d++ = *s++; }
    }
    return dst;
}
void *NextMaxPath(u32 address,u32 size)
{
 Heap *heap;Block *block,*created,*following;u32 before;void *result;
 osRecvMesg((OSMesgQueue *)&D_80152770,NULL,1);
 heap=func_80095F8C(address);
 size=(size+31)&~31U;
 block=heap->blocks;
 while(block) {
  if(address>=(u32)block && (block->next==0 || address<(u32)block->next)) {
   break;
  }
  block=block->next;
 }
 result=(u8 *)block+32;
 if(address!=(u32)result) {
  created=(Block *)(address-32);
  following=block->next;
  created->next=following;
  if(following)following->pad8[0]=(u32)created;
  else heap->padc[0]=(u32)created;
  created->pad8[2]=0;created->pad14=1;created->tag=0;created->counter=0;
  created->pad0=0xFEDCBA98;
  before=address-(u32)block-32;
  created->pad8[1]=block->pad8[1]-before;
  if(before>=64) {
   created->pad8[0]=(u32)block;block->next=created;
   block->pad8[1]=block->pad8[1]-created->pad8[1]-32;
  } else {
   created->pad8[0]=block->pad8[0];
   ((Block *)block->pad8[0])->next=created;
   ((Block *)block->pad8[0])->pad8[1]=((Block *)block->pad8[0])->pad8[1]+address-(u32)block-32;
  }
  block=created;result=(u8 *)block+32;
 }
 if(block->pad8[1]-size>=64) {
  created=(Block *)((u8 *)block+size+32);
  created->next=block->next;
  if(created->next)created->next->pad8[0]=(u32)created;
  else heap->padc[0]=(u32)created;
  created->pad8[0]=(u32)block;
  created->pad8[1]=block->pad8[1]-size-32;
  created->pad8[2]=0;created->pad14=0;created->tag=0;created->counter=0;
  created->pad0=0xFEDCBA98;
  block->next=created;block->pad8[1]=size;
 }
 block->pad8[2]=0;block->pad14=1;block->tag=0;
 osJamMesg((OSMesgQueue *)&D_80152770,NULL,0);
 return result;
}
