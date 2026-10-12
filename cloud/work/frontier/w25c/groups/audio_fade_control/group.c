/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * audio_fade_control unit (sound command queue; N64-only, no arcade ancestor found).
 * Free list D_80146138, active list D_80146160, 24-byte sound entries D_80144C48 indexed by handle & 255.
 * audio_fade_control is internal (IPA: node in s4, s0-s3 left to the callers); func_800956BC (locked single,
 * byte-identical copy in func_800956BC.c) is internal too (its callers keep a2 live across the call).
 * Deleted inlined helpers, non-static and named after their retail stubs so each stub lands in its own slot
 * (w17j): func_800953B4 = list_append, func_800953BC = entry_alloc (stub identity among the three stubs before
 * audio_fade_control is a HYPOTHESIS: the stubs are identical `jr ra; nop`), func_800954A0 = node_alloc (the stub
 * right before gfx_setup_e700, its first caller). func_800953C4 stays the locked empty stub (unclaimed).
 * DISCLOSED SHAPING DEVICES:
 *  1. audio_fade_control: compiled-out `if (node->kind == 2) {}` first in the then-arm (w24f): gives the constant
 *     2 a second, codeless use so it is a coloured web (retail `li a0,2`).
 *  2. node_alloc (func_800954A0): compiled-out `do {} while (0);` after the head load (w25c), e.g. a compiled-out
 *     ASSERT(node). It ends the basic block before the unlink call; the &D_80146138 web then spans two blocks and
 *     a0 (precoloured for the call's argument in the second block) is forbidden to it, so it takes a2 as in retail
 *     gfx_setup_e700 (`lui a2; addiu a0,a2,...`). Inert for the other five node_alloc callers.
 * Provenance: w20f (6/8), w24f (7/8), w25c (8/8). cloud/work/frontier/w25c/RESULTS.md.
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
void func_800953B4(List *list,Node *node) {
 func_80091FBC(list,node,list->tail);
}
s32 func_800953BC(Entry **out)
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
void audio_fade_control(Node *node) {
 Entry *entry;Node *cur,*next;
 if(node->kind!=4 && node->kind==2 && (entry=node->entry)->flags[1]==0) {
  if(node->kind==2){}
  cur=D_80146160.head;
  while(cur) {
   next=cur->next;
   if(entry==cur->entry) {
    func_8009211C(&D_80146160,cur);
    func_800953B4(&D_80146138,cur);
    entry=node->entry;
   }
   cur=next;
  }
  entry->flags[0]=0;entry->handle=0;
  func_800953B4(&D_80146138,node);
 } else {
  func_800953B4(&D_80146160,node);
 }
}
Node *func_800954A0(void) {
 Node *node=D_80146138.head;
 do{}while(0);
 node->busy=0;
 func_8009211C(&D_80146138,node);
 return node;
}
void gfx_setup_e700(s32 value) {
 Node *node=func_800954A0();
 node->kind=4;node->value=(f32)value;node->entry=0;
 audio_fade_control(node);
}
void audio_bus_route(s32 handle,f32 value) {
    Node *node;
    if(handle!=-1) {
        if(handle==D_80144C48[handle&255].handle) {
            node=func_800956BC(&D_80144C48[handle&255]);
            if(node==0) {
                node=func_800954A0();
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
   node=func_800954A0();
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
                node=func_800954A0();
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
                node=func_800954A0();
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
                node=func_800954A0();
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
s32 audio_state_save(s32 resource,f32 value,f32 stream)
{
 Entry *entry;
 Node *node;
 if(resource==-1)return -1;

 if(!func_800953BC(&entry))return -1;
 node=func_800954A0();
 node->kind=0;
 node->entry=entry;
 entry->resource=resource;
 node->value=value;
 node->stream=stream;
 audio_fade_control(node);
 return entry->handle;
}
