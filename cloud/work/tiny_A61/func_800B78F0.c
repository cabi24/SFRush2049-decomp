/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;typedef signed char s8;typedef unsigned short u16;typedef unsigned int u32;typedef int s32;
typedef struct Data {u8 *base;} Data;
typedef struct Car {u8 pad0[44];Data *data;} Car;
typedef struct Ref {Car *car;} Ref;
typedef struct Player76 {u8 pad0;u8 index;u8 pad2[70];Ref *ref;} Player76;
typedef struct Path96 {u8 pad0[92];u16 flags;u8 pad94[2];} Path96;
typedef struct Path64 {u8 pad0[60];u16 flags;u8 pad62[2];} Path64;
extern s8 D_8014978C;extern Player76 D_8014A118[];extern Car *D_80146150[];
extern u32 func_800B78A4(u32,u8);
u8 func_800B78F0(s32 player,s32 mode) {
 s32 selector=D_8014978C;Player76 *p;Ref *ref;Data *data;u32 word,mask;Path96 *path96;Path64 *path64;
 if(selector>=0&&selector<6) {
  p=&D_8014A118[player];ref=p->ref;
  if(!ref){ref=(Ref *)&D_80146150[p->index];p->ref=ref;}
  data=ref->car->data;if(!data)return 16;
  path96=(Path96 *)data->base;path96+=selector;path96=(Path96 *)((u8 *)path96+140);word=path96->flags;
 }else if(selector>=14&&selector<18) {
  p=&D_8014A118[player];ref=p->ref;
  if(!ref){ref=(Ref *)&D_80146150[p->index];p->ref=ref;}
  path64=(Path64 *)ref->car->data->base;path64+=selector;path64=(Path64 *)((u8 *)path64+396);word=path64->flags;
 }else word=0;
 if(mode){if(mode==1)mask=0xFF00;else mask=0xFF;word&=mask;}
 return func_800B78A4(word,16);
}
