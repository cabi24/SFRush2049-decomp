/* Compile the actual submitted source unchanged and expose a normalized host ABI. */
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include "../../../matches/sound_control.c"
#define MAX 8
#define ROW 14
static Blit objects[MAX];
static MultiBlit records[MAX];
static int used, event_count, mutate_records;
static uint32_t events[64][8];
static int token_names[MAX], token_images[MAX], token_infos[MAX], token_data[MAX];
static s32 animate_yes(Blit *b);
static s32 animate_no(Blit *b);
static uint32_t name_token(const char *p) {
 int i;if(!p)return 0;if(p==(char *)-1)return 0xffffffffU;
 for(i=0;i<MAX;i++)if(p==(char *)&token_names[i])return 0x100U+i;
 assert(0);return 0;
}
static uint32_t pointer_token(const void *p) {
 int i;if(!p)return 0;
 for(i=0;i<MAX;i++) {
  if(p==&objects[i])return 0x200U+i;
  if(p==&token_images[i])return 0x300U+i;
  if(p==&token_infos[i])return 0x400U+i;
  if(p==&token_data[i])return 0x500U+i;
 }
 /* The submitted N64 sentinel branch intentionally stores a function address. */
 if(p==(void *)animate_yes)return 0x600U;
 if(p==(void *)animate_no)return 0x601U;
 assert(0);return 0;
}
static uint32_t callback_token(s32 (*p)(Blit *)) {
 if(!p)return 0;
 if(p==animate_yes)return 0x600;
 if(p==animate_no)return 0x601;
 assert(0);return 0;
}
static void snapshot(Blit *b,uint32_t *v) {
 v[0]=name_token(b->Name);v[1]=pointer_token(b->Image);v[2]=pointer_token(b->Info);
 v[3]=b->TexIndex;v[4]=(uint32_t)b->X;v[5]=(uint32_t)b->Y;v[6]=b->Z;
 v[7]=(uint32_t)b->Width;v[8]=(uint32_t)b->Height;v[9]=b->Alpha;v[10]=b->Flip;
 v[11]=(uint32_t)b->Hide;v[12]=(uint32_t)b->Init;v[13]=(uint32_t)b->Top;
 v[14]=(uint32_t)b->Bot;v[15]=(uint32_t)b->Left;v[16]=(uint32_t)b->Right;
 v[17]=(uint32_t)b->color;v[18]=callback_token(b->AnimFunc);v[19]=b->AnimID;
 v[20]=(uint32_t)b->AnimDTA;v[21]=b->BLIdx;v[22]=pointer_token(b->data);
 v[23]=pointer_token(b->child);
}
static uint32_t hash_object(Blit *b) {
 uint32_t v[24],h=2166136261U;int k;snapshot(b,v);
 for(k=0;k<24;k++)h=(h^v[k])*16777619U;
 return h;
}
static void event(int kind,Blit *b,uint32_t a,uint32_t c,uint32_t d,uint32_t e,uint32_t result) {
 uint32_t *row;assert(event_count<64);row=events[event_count++];
 row[0]=kind;row[1]=(uint32_t)(b-objects);row[2]=a;row[3]=c;row[4]=d;row[5]=e;
 row[6]=hash_object(b);row[7]=result;
}
Blit *func_800B3704(const char *name,int x,int y,u32 flags) {
 Blit *b;int i=used++;assert(i<MAX);b=&objects[i];
 b->Name=name;b->Image=&token_images[i];b->Info=&token_infos[i];b->TexIndex=100+i;
 b->X=x;b->Y=y;b->Z=200+i;b->Width=300+i;b->Height=400+i;b->Alpha=17+i;
 b->Flip=1;b->Hide=-1;b->Init=1;b->Top=-2;b->Bot=-3;b->Left=-4;b->Right=-5;b->color=-6;
 b->AnimFunc=0;b->AnimID=0xffffffffU;b->AnimDTA=-1;b->BLIdx=500+i;
 b->data=&token_data[i];b->child=0;
 event(1,b,name_token(name),(uint32_t)x,(uint32_t)y,flags,0);
 if(mutate_records) { records[i].width=123+i;records[i].height=234+i;records[i].alpha^=0x5aU; }
 return b;
}
void Input_ApplyPadConfig(Blit *b) {
 int i=(int)(b-objects);event(2,b,0,0,0,0,0);
 /* External callbacks may update the underlying non-const descriptor storage. */
 if(mutate_records)records[i].animid^=0x40000000U;
}
static s32 animate(Blit *b,int result) {
 int i=(int)(b-objects);event(3,b,0,0,0,0,(uint32_t)result);
 b->AnimDTA=700+i;b->Alpha^=0x33U;
 if(mutate_records && i+1<MAX)records[i+1].dulx=-1234;
 return result;
}
static s32 animate_yes(Blit *b){return animate(b,mutate_records?-7:1);}
static s32 animate_no(Blit *b){return animate(b,0);}
void sound_stop(Blit *b){event(4,b,0,0,0,0,0);b->Init=0;}
int run_case(const int32_t *input,uint32_t *out) {
 int i,k,pos=0;Blit *result;used=event_count=0;mutate_records=input[3];
 memset(objects,0,sizeof(objects));memset(records,0,sizeof(records));
 for(i=0;i<MAX;i++) {
  const int32_t *r=input+4+i*ROW;MultiBlit *m=&records[i];
  m->texname=r[0]==-1?(char *)-1:(r[0]==0?0:(char *)&token_names[i]);
  m->dulx=r[1];m->duly=r[2];m->width=r[3];m->height=r[4];m->top=r[5];m->bot=r[6];m->left=r[7];m->right=r[8];
  m->zdepth=(uint32_t)r[9];m->alpha=(uint32_t)r[10];m->animid=(uint32_t)r[11];
  m->animfunc=r[12]==0?0:(r[12]==1?animate_yes:animate_no);
 }
 result=sound_control((s16)input[1],(s16)input[2],records,(s16)input[0]);
 out[pos++]=pointer_token(result);out[pos++]=(uint32_t)used;out[pos++]=(uint32_t)event_count;
 for(i=0;i<used;i++){snapshot(&objects[i],out+pos);pos+=24;}
 for(i=0;i<event_count;i++)for(k=0;k<8;k++)out[pos++]=events[i][k];
 return pos;
}
