/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed int s32;typedef unsigned int u32;typedef float f32;
typedef struct Entry {s8 flags[3];u8 pad3[5];s32 handle;u8 pad12[12];} Entry;
typedef struct Node {struct Node *prev,*next;u8 kind,busy;u8 pad10[2];Entry *entry;f32 value;} Node;
typedef struct List {u32 pad0[2];Node *tail,*head;} List;
extern List D_80146138,D_80146160;
extern Entry *D_80144C48;
void func_8009211C(List *,Node *);
void func_80091FBC(List *,Node *,Node *);
typedef struct ResourceEntry24 {s8 flags[6];u8 reserved6[2];s32 handle,resource;u32 state16,state20;} ResourceEntry24;
typedef struct Submission24 {Node prefix;f32 stream;} Submission24;
extern u32 D_801460F4,D_80110288;
void audio_fade_control(Node *);
static s32 allocate_entry(ResourceEntry24 **out)
{
 u32 i;
 *out=0;
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
   *out=&((ResourceEntry24 *)D_80144C48)[i];
   return 1;
  }
 }
 return 0;
}
s32 audio_state_save(s32 resource,f32 value,f32 stream)
{
 ResourceEntry24 *entry;
 Submission24 *submission;
 if(resource==-1)return -1;
 if(!allocate_entry(&entry))return -1;
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
