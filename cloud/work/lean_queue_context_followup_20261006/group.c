/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* audio_bus_route and audio_timing_sync follow-up research; other bodies are context. */
typedef signed char s8;typedef unsigned char u8;typedef signed int s32;typedef unsigned int u32;typedef float f32;
typedef struct Entry {s8 flags[3];u8 pad3[5];s32 handle;u8 pad12[12];} Entry;
typedef struct Node {struct Node *prev,*next;u8 kind,busy;u8 pad10[2];Entry *entry;f32 value,stream,priority,route;} Node;
typedef struct List {u32 pad0[2];Node *tail,*head;} List;
extern List D_80146138,D_80146160;
extern Entry *D_80144C48;
void func_8009211C(List *,Node *);
void func_80091FBC(List *,Node *,Node *);
void audio_fade_control(Node *node) {
 Entry *entry=node->entry;Node *cur,*next;
 if(node->kind!=4 && node->kind==2 && entry->flags[1]==0) {
  cur=D_80146160.head;
  while(cur) {
   next=cur->next;
   if(entry==cur->entry) {
    func_8009211C(&D_80146160,cur);
    func_80091FBC(&D_80146138,cur,D_80146138.tail);
    entry=node->entry;
   }
   cur=next;
  }
  entry->flags[0]=0;entry->handle=0;
  func_80091FBC(&D_80146138,node,D_80146138.tail);
 } else func_80091FBC(&D_80146160,node,D_80146160.tail);
}
static Node *take_command(void) {
 Node *node;
 node=D_80146138.head;
 node->busy=0;
 func_8009211C(&D_80146138,node);
 return node;
}
void gfx_setup_e700(s32 value) {
 Node *node=D_80146138.head;
 node->busy=0;
 func_8009211C(&D_80146138,node);
 node->kind=4;node->entry=0;node->value=(f32)value;
 audio_fade_control(node);
}
void audio_timing_sync(s32 handle) {
 u32 slot;Node *node;
 if(handle!=-1) {
  slot=handle&255;
  if(handle==D_80144C48[slot].handle) {
   node=take_command();
   node->kind=2;
   node->entry=&D_80144C48[slot];
   node->entry->flags[2]=1;node->entry->handle=0;
   audio_fade_control(node);
  }
 }
}

Node *func_800956BC(s32);

void audio_bus_route(s32 handle,f32 value) {
    s32 offset;Node *node;
    if(handle!=-1) {
        offset=(handle&255)*24;
        if(handle==D_80144C48[handle&255].handle) {
            node=func_800956BC((s32)(offset+(u8 *)D_80144C48));
            if(node==0) {
                node=take_command();
                node->kind=3;
                node->entry=(Entry *)(offset+(u8 *)D_80144C48);
                node->value=-2.0f;
                node->stream=-2.0f;
                node->priority=-2.0f;
                node->route=value;
                audio_fade_control(node);
            } else node->route=value;
        }
    }
}
void sound_priority_set(s32 handle,f32 value) {
    s32 offset;Node *node;
    if(handle!=-1) {
        offset=(handle&255)*24;
        if(handle==D_80144C48[handle&255].handle) {
            node=func_800956BC((s32)(offset+(u8 *)D_80144C48));
            if(node==0) {
                node=take_command();
                node->kind=3;
                node->entry=(Entry *)(offset+(u8 *)D_80144C48);
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
    s32 offset;Node *node;
    if(handle!=-1) {
        offset=(handle&255)*24;
        if(handle==D_80144C48[handle&255].handle) {
            node=func_800956BC((s32)(offset+(u8 *)D_80144C48));
            if(node==0) {
                node=take_command();
                node->kind=3;
                node->entry=(Entry *)(offset+(u8 *)D_80144C48);
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
    s32 offset;Node *node;
    if(handle!=-1) {
        offset=(handle&255)*24;
        if(handle==D_80144C48[handle&255].handle) {
            node=func_800956BC((s32)(offset+(u8 *)D_80144C48));
            if(node==0) {
                node=take_command();
                node->kind=3;
                node->entry=(Entry *)(offset+(u8 *)D_80144C48);
                node->stream=-2.0f;
                node->priority=-2.0f;
                node->route=-2.0f;
                node->value=value;
                audio_fade_control(node);
            } else node->value=value;
        }
    }
}
