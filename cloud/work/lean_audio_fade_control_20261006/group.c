/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* audio_fade_control research: native entry snapshot before the kind guard.
 * Keep the post-list-callback reload. Real A158 callers remain context.
 * Observed group residual is four of 53 words; no match claim.
 */
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
void gfx_setup_e700(s32 value) {
 Node *node=D_80146138.head;
 node->busy=0;
 func_8009211C(&D_80146138,node);
 node->kind=4;node->entry=0;node->value=(f32)value;
 audio_fade_control(node);
}
void audio_timing_sync(s32 handle) {
 s32 offset;Node *node;
 if(handle!=-1) {
  offset=(handle&255)*24;
  if(handle==*(s32 *)((u8 *)D_80144C48+offset+8)) {
   node=D_80146138.head;node->busy=0;
   func_8009211C(&D_80146138,node);
   node->kind=2;
   node->entry=(Entry *)((u8 *)D_80144C48+offset);
   node->entry->flags[2]=1;node->entry->handle=0;
   audio_fade_control(node);
  }
 }
}

Node *func_800956BC(Entry *entry) {
    Node *p=D_80146160.head;
    while(p) {
        if(entry==p->entry && p->kind==3) return p;
        p=p->next;
    }
    return 0;
}
void audio_bus_route(s32 handle,f32 value) {
    s32 offset;Node *node;
    if(handle!=-1) {
        offset=(handle&255)*24;
        if(handle==*(s32 *)((u8 *)D_80144C48+offset+8)) {
            node=func_800956BC((Entry *)(offset+(u8 *)D_80144C48));
            if(node==0) {
                node=D_80146138.head;
                node->busy=0;
                func_8009211C(&D_80146138,node);
                node->kind=3;
                node->value=-2.0f;
                node->entry=(Entry *)(offset+(u8 *)D_80144C48);
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
        if(handle==*(s32 *)((u8 *)D_80144C48+offset+8)) {
            node=func_800956BC((Entry *)(offset+(u8 *)D_80144C48));
            if(node==0) {
                node=D_80146138.head;
                node->busy=0;
                func_8009211C(&D_80146138,node);
                node->kind=3;
                node->value=-2.0f;
                node->entry=(Entry *)(offset+(u8 *)D_80144C48);
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
        if(handle==*(s32 *)((u8 *)D_80144C48+offset+8)) {
            node=func_800956BC((Entry *)(offset+(u8 *)D_80144C48));
            if(node==0) {
                node=D_80146138.head;
                node->busy=0;
                func_8009211C(&D_80146138,node);
                node->kind=3;
                node->value=-2.0f;
                node->entry=(Entry *)(offset+(u8 *)D_80144C48);
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
        if(handle==*(s32 *)((u8 *)D_80144C48+offset+8)) {
            node=func_800956BC((Entry *)(offset+(u8 *)D_80144C48));
            if(node==0) {
                node=D_80146138.head;
                node->busy=0;
                func_8009211C(&D_80146138,node);
                node->kind=3;
                node->stream=-2.0f;
                node->entry=(Entry *)(offset+(u8 *)D_80144C48);
                node->priority=-2.0f;
                node->route=-2.0f;
                node->value=value;
                audio_fade_control(node);
            } else node->value=value;
        }
    }
}
