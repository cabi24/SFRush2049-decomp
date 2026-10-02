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

void *func_800A47C0(void *,const void *,u32);
u32 format_string_parse(u8 *data,u32 size)
{
    u8 *cursor=data;
    u32 result=0,value;
    while(size!=0) {
        value=*cursor;
        switch(value&7) {
        case 0:result-=value;cursor++;break;
        case 1:result|=value;cursor++;break;
        case 2:result&=value;cursor++;break;
        case 3:result^=value;cursor++;break;
        case 4:result*=value;cursor++;break;
        case 5:result/=value;cursor++;break;
        default:result+=value;cursor++;break;
        }
        size--;
    }
    return result;
}


typedef float f32;
typedef struct State88 {u32 hash;u8 code,pad5;s8 field6;u8 field7;u8 pad8[14];u8 name[13];u8 pad35;signed short id;u8 pad38[2];u32 word40,word44;u8 pad48[4];f32 float52,float56;u32 compressedBytes,length,checksum68,checksum72;u8 *allocation;u32 word80,extent;} State88;
typedef struct Handle {State88 *data;} Handle;
typedef struct Metadata44 {u8 pad0[8];s8 field8;u8 field9;u8 name[13];u8 pad23;u32 word24,word28;f32 value32;u32 word36;Handle *descriptor;} Metadata44;
typedef struct MetadataHandle {Metadata44 *data;} MetadataHandle;
extern s8 D_8014978C;
extern f32 D_80111754[],D_8002AFB4;
u8 *sound_play_menu(s32,u32);
s32 car_angular_velocity_clamp(u8 *,u32,u8 *,u32,s32,s32,s32);
MetadataHandle *func_800C7200(void);
Handle *audio_task_complete(s32,s32);
void *memset(void *,s32,u32);
void *memcpy(void *,const void *,u32);
MetadataHandle *func_800CC50C(State88 *input,s8 *units) {
    s32 shift,compressedBytes;
    u32 length,checksum,extent;
    u8 *original,*buffer,*allocation;
    MetadataHandle *handle;
    Metadata44 *meta;
    shift=(s32)((f32)(D_80111754[D_8014978C]+10.0)*D_8002AFB4);
    allocation=input->allocation;
    original=allocation;
    extent=input->extent;
    func_800A47C0(allocation+extent,allocation+shift,extent);
    extent=input->extent;
    allocation=input->allocation;
    func_800A47C0(allocation+2*extent,allocation+2*shift,extent);
    length=3*input->extent;
    checksum=format_string_parse(original,length);
    input->id=0;
    buffer=sound_play_menu(0,length);
    compressedBytes=car_angular_velocity_clamp(original,length,buffer,length,9,10,4);
    allocation=input->allocation;
    osRecvMesg((OSMesgQueue *)&D_80152770,NULL,1);
    audio_reverb_update((u32)allocation,0);
    osJamMesg((OSMesgQueue *)&D_80152770,NULL,0);
    input->allocation=NULL;
    handle=func_800C7200();
    meta=handle->data;
    memset(meta,0,44);
    meta->descriptor=audio_task_complete(0,compressedBytes+88);
    memcpy(meta->descriptor->data,input,88);
    meta->descriptor->data->code=17;
    meta->descriptor->data->allocation=NULL;
    meta->descriptor->data->word80=0;
    meta->descriptor->data->extent=0;
    meta->descriptor->data->compressedBytes=compressedBytes;
    meta->descriptor->data->length=length;
    meta->descriptor->data->hash=format_string_parse((u8 *)meta->descriptor->data+4,(u8 *)&meta->descriptor->data->length-(u8 *)meta->descriptor->data);
    meta->descriptor->data->checksum68=format_string_parse(buffer,compressedBytes);
    meta->descriptor->data->checksum72=checksum;
    memcpy((u8 *)meta->descriptor->data+88,buffer,compressedBytes);
    osRecvMesg((OSMesgQueue *)&D_80152770,NULL,1);
    audio_reverb_update((u32)buffer,0);
    osJamMesg((OSMesgQueue *)&D_80152770,NULL,0);
    meta->field8=meta->descriptor->data->field6;
    meta->field9=meta->descriptor->data->field7;
    memcpy(meta->name,meta->descriptor->data->name,13);
    meta->word24=meta->descriptor->data->word40;
    meta->word28=meta->descriptor->data->word44;
    meta->value32=meta->descriptor->data->float56;
    *units=((s32)audio_buffer_sync((u32)meta->descriptor)+255)>>8;
    return handle;
}
