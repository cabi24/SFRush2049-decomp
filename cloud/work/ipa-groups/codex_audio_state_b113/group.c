/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed int s32;typedef unsigned int u32;typedef float f32;
typedef struct Entry {s8 flags[3];u8 pad3[5];s32 handle;u8 pad12[12];} Entry;
typedef struct Node {struct Node *prev,*next;u8 kind,busy;u8 pad10[2];Entry *entry;f32 value;} Node;
typedef struct List {u32 pad0[2];Node *tail,*head;} List;
extern List D_80146138,D_80146160;
extern Entry *D_80144C48;
void func_8009211C(List *,Node *);
void func_80091FBC(List *,Node *,Node *);
void audio_fade_control(Node *node) {
 Entry *entry;Node *cur,*next;
 if(node->kind!=4 && node->kind==2 && node->entry->flags[1]==0) {
  entry=node->entry;
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

typedef struct ResourceEntry24 {s8 flags[6];u8 reserved6[2];s32 handle,resource,state16,state20;} ResourceEntry24;
typedef struct Submission24 {Node prefix;f32 stream;} Submission24;
extern u32 D_801460F4,D_80110288;
s32 audio_state_save(s32 resource,f32 value,f32 stream)
{
 ResourceEntry24 *entry=0;
 Submission24 *submission;
 u32 i;
 if(resource==-1)return -1;
 for(i=0;i<D_801460F4;i++) {
  if(((ResourceEntry24 *)D_80144C48)[i].flags[0]==0) {
   ((ResourceEntry24 *)D_80144C48)[i].state16=-1;
   ((ResourceEntry24 *)D_80144C48)[i].flags[0]=1;
   ((ResourceEntry24 *)D_80144C48)[i].flags[1]=0;
   ((ResourceEntry24 *)D_80144C48)[i].flags[2]=0;
   ((ResourceEntry24 *)D_80144C48)[i].flags[3]=0;
   ((ResourceEntry24 *)D_80144C48)[i].flags[4]=0;
   ((ResourceEntry24 *)D_80144C48)[i].flags[5]=0;
   ((ResourceEntry24 *)D_80144C48)[i].handle=(D_80110288<<8)|i;
   if(D_80110288>=0xFFFFFF)D_80110288=1;
   else D_80110288++;
   entry=&((ResourceEntry24 *)D_80144C48)[i];
   break;
  }
 }
 if(!entry)return -1;
 submission=(Submission24 *)D_80146138.head;
 submission->prefix.busy=0;
 func_8009211C(&D_80146138,&submission->prefix);
 submission->prefix.kind=0;
 submission->prefix.entry=(Entry *)entry;
 entry->resource=resource;
 submission->prefix.value=value;
 submission->stream=stream;
 audio_fade_control(&submission->prefix);
 return entry->handle;
}
