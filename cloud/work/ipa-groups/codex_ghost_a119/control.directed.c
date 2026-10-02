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
typedef signed short s16;
typedef float f32;
typedef struct GhostState {
    u8 pad0[5];s8 state;u8 pad6[50];f32 value;
    u8 pad60[16];u32 allocation;u32 word80,word84;
} GhostState;
typedef struct GhostDescriptor {GhostState *state;} GhostDescriptor;
typedef struct GhostHandle GhostHandle;
typedef struct GhostData {
    u32 word0;GhostHandle *adjust;u8 pad8[28];GhostDescriptor *allocation;
    GhostDescriptor *descriptor;u8 pad44[20];u32 count,word68;GhostDescriptor *resource;
} GhostData;
struct GhostHandle {GhostData *data;};
typedef struct GhostPlayer {u8 pad0[239];s8 ready;f32 value;u8 tail[708];} GhostPlayer;
typedef struct GhostObject {u8 pad0[1820];s16 reset;u8 tail[234];} GhostObject;
extern GhostHandle *D_80152698[];
extern GhostPlayer D_80152818[];
extern GhostObject D_8014A250[];
extern GhostState *D_80140800;
extern s16 D_8014A108;
extern s32 D_8014A110;
extern s8 D_80114738;
void AdjustSteer(void *);
void menu_transition(GhostHandle *handle) {
    GhostData *data=handle->data;
    GhostDescriptor *allocation;
    if((allocation=data->allocation)!=0){
        osRecvMesg((OSMesgQueue *)&D_80152770,NULL,1);
        audio_reverb_update((u32)allocation,0);
        osJamMesg((OSMesgQueue *)&D_80152770,NULL,0);
        data->allocation=0;
        data->descriptor->state->allocation=0;
        if(data->adjust!=0){
            AdjustSteer(data->adjust);
            data->descriptor=0;
        }
    }
}
void ghost_race_setup(void) {
    int i;
    GhostHandle *handle;
    GhostState *state;
    GhostPlayer *player;
    D_80140800=0;
    if(D_8014A110==2){
        D_80114738=1;
        for(i=0;i<D_8014A108;i++){
            handle=D_80152698[i];
            if(handle!=0){
                D_80152698[i]=0;
                D_8014A250[i].reset=0;
                state=handle->data->descriptor->state;
                player=&D_80152818[i];
                if(state->state<0){
                    state->state=0;
                    if(player->ready!=0){
                        state->value=player->value;
                        state->word84=state->word80;
                        D_80140800=state;
                    }else{
                        osRecvMesg((OSMesgQueue *)&D_80152770,NULL,1);
                        audio_reverb_update(state->allocation,0);
                        osJamMesg((OSMesgQueue *)&D_80152770,NULL,0);
                        state->allocation=0;
                    }
                }else if(state->state>0)menu_transition(handle);
            }
        }
        D_8014A108=1;
        D_80114738=0;
    }
}
