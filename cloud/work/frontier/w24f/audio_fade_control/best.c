/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * w24f best (from w20f) for the audio_fade_control unit (NOT installable: gfx_setup_e700 4 rows; unit score with
 *   blob_unit --tag w20f score audio_fade_control gfx_setup_e700 audio_bus_route audio_timing_sync
 *     sound_priority_set audio_stream_control audio_buffer_manage audio_state_save
 *     --with THIS --internal audio_fade_control --internal func_800956BC --neighbours
 * w24f: audio_fade_control now EQUAL: entry assigned inside the condition and ONE DISCLOSED shaping device,
 * the compiled-out `if (node->kind == 2) {}` (gives the constant 2 its second, codeless use -> coloured a0).
 * EQUAL: audio_fade_control, audio_bus_route, audio_timing_sync, sound_priority_set, audio_stream_control,
 *        audio_buffer_manage, audio_state_save.  FAIL: gfx_setup_e700 9 words (4 rows, colour-only: &D_80146138 must be a2).
 * Sound command queue (N64-only; no arcade ancestor found). Free list D_80146138, active list D_80146160,
 * 24-byte sound entries D_80144C48 indexed by handle & 255.
 * Static helpers list_append / node_alloc / entry_alloc are inlined (deleted-procedure stubs in retail:
 * three before audio_fade_control, func_800954A0 before gfx_setup_e700); for packaging they must be renamed
 * to stub names and made non-static internal (w17j lesson).
 */
typedef signed char s8;typedef unsigned char u8;typedef signed int s32;typedef unsigned int u32;typedef float f32;
typedef struct Entry {s8 flags[6];u8 pad6[2];s32 handle;s32 resource;u32 state16;s32 state20;} Entry;
typedef struct Node {struct Node *prev,*next;u8 kind,busy;u8 pad10[2];Entry *entry;f32 value,stream,priority,route;} Node;
typedef struct List {u32 pad0[2];Node *tail,*head;} List;
extern List D_80146138,D_80146160;
extern Entry *D_80144C48;
extern u32 D_801460F4,D_80110288;
void func_8009211C(List *,Node *);
void func_80091FBC(List *,Node *,Node *);
Node *func_800956BC(Entry *key);
static void list_append(List *list,Node *node) {
 func_80091FBC(list,node,list->tail);
}
void audio_fade_control(Node *node) {
 Entry *entry;Node *cur,*next;
 if(node->kind!=4 && node->kind==2 && (entry=node->entry)->flags[1]==0) {
  if(node->kind==2){}
  cur=D_80146160.head;
  while(cur) {
   next=cur->next;
   if(entry==cur->entry) {
    func_8009211C(&D_80146160,cur);
    list_append(&D_80146138,cur);
    entry=node->entry;
   }
   cur=next;
  }
  entry->flags[0]=0;entry->handle=0;
  list_append(&D_80146138,node);
 } else {
  list_append(&D_80146160,node);
 }
}
static Node *node_alloc(void) {
 Node *node=D_80146138.head;
 node->busy=0;
 func_8009211C(&D_80146138,node);
 return node;
}
void gfx_setup_e700(s32 value) {
 Node *node=node_alloc();
 node->kind=4;node->value=(f32)value;node->entry=0;
 audio_fade_control(node);
}
void audio_bus_route(s32 handle,f32 value) {
    Node *node;
    if(handle!=-1) {
        if(handle==D_80144C48[handle&255].handle) {
            node=func_800956BC(&D_80144C48[handle&255]);
            if(node==0) {
                node=node_alloc();
                node->kind=3;
                node->entry=&D_80144C48[handle&255];
                node->value=-2.0f;
                node->stream=-2.0f;
                node->priority=-2.0f;
                node->route=value;
                audio_fade_control(node);
            } else node->route=value;
        }
    }
}
void audio_timing_sync(s32 handle) {
 Node *node;
 if(handle!=-1) {
  if(handle==D_80144C48[handle&255].handle) {
   node=node_alloc();
   node->kind=2;
   node->entry=&D_80144C48[handle&255];
   node->entry->flags[2]=1;node->entry->handle=0;
   audio_fade_control(node);
  }
 }
}
void sound_priority_set(s32 handle,f32 value) {
    Node *node;
    if(handle!=-1) {
        if(handle==D_80144C48[handle&255].handle) {
            node=func_800956BC(&D_80144C48[handle&255]);
            if(node==0) {
                node=node_alloc();
                node->kind=3;
                node->entry=&D_80144C48[handle&255];
                node->value=-2.0f;
                node->stream=-2.0f;
                node->route=-2.0f;
                node->priority=value;
                audio_fade_control(node);
            } else node->priority=value;
        }
    }
}
void audio_stream_control(s32 handle,f32 value) {
    Node *node;
    if(handle!=-1) {
        if(handle==D_80144C48[handle&255].handle) {
            node=func_800956BC(&D_80144C48[handle&255]);
            if(node==0) {
                node=node_alloc();
                node->kind=3;
                node->entry=&D_80144C48[handle&255];
                node->value=-2.0f;
                node->priority=-2.0f;
                node->route=-2.0f;
                node->stream=value;
                audio_fade_control(node);
            } else node->stream=value;
        }
    }
}
void audio_buffer_manage(s32 handle,f32 value) {
    Node *node;
    if(handle!=-1) {
        if(handle==D_80144C48[handle&255].handle) {
            node=func_800956BC(&D_80144C48[handle&255]);
            if(node==0) {
                node=node_alloc();
                node->kind=3;
                node->entry=&D_80144C48[handle&255];
                node->stream=-2.0f;
                node->priority=-2.0f;
                node->route=-2.0f;
                node->value=value;
                audio_fade_control(node);
            } else node->value=value;
        }
    }
}
static s32 entry_alloc(Entry **out)
{
 u32 i;
 *out=0;
 for(i=0;i<D_801460F4;i++) {
  if(D_80144C48[i].flags[0]==0) {
   D_80144C48[i].state16=-1;
   D_80144C48[i].flags[0]=1;
   D_80144C48[i].flags[1]=0;
   D_80144C48[i].flags[2]=0;
   D_80144C48[i].flags[3]=0;
   D_80144C48[i].flags[4]=0;
   D_80144C48[i].flags[5]=0;
   D_80144C48[i].handle=(D_80110288<<8)|i;
   if(D_80110288>=0xFFFFFF)D_80110288=1;
   else D_80110288++;
   *out=&D_80144C48[i];
   return 1;
  }
 }
 return 0;
}
s32 audio_state_save(s32 resource,f32 value,f32 stream)
{
 Entry *entry;
 Node *node;
 if(resource==-1)return -1;

 if(!entry_alloc(&entry))return -1;
 node=node_alloc();
 node->kind=0;
 node->entry=entry;
 entry->resource=resource;
 node->value=value;
 node->stream=stream;
 audio_fade_control(node);
 return entry->handle;
}
