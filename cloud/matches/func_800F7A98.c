/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;typedef signed char s8;typedef unsigned short u16;typedef unsigned int u32;typedef int s32;
typedef struct Data {u8 *base;} Data;
typedef struct Car {u8 pad0[44];Data *data;} Car;
typedef struct Ref {Car *car;} Ref;
typedef struct Player76 {u8 pad0;u8 index;u8 pad2[70];Ref *ref;} Player76;
extern u32 D_801174B4;extern s8 D_8014978C;extern Player76 D_8014A118[];extern Car *D_80146150[];
u32 func_800F7A98(s32 player,s32 bit) {
 s32 selector;Player76 *p;Ref *ref;Data *data;
 if(D_801174B4&8)return 1;
 selector=D_8014978C;
 if(selector>=0&&selector<6) {
  p=&D_8014A118[player];ref=p->ref;
  if(!ref){return *(u16 *)(D_80146150[p->index]->data->base+232+selector*96)&(1u<<bit);}
  data=ref->car->data;if(!data)return 1;
  return *(u16 *)(data->base+232+selector*96)&(1u<<bit);
 }
 if(selector>=14&&selector<18) {
  p=&D_8014A118[player];ref=p->ref;
  if(!ref){return *(u16 *)(D_80146150[p->index]->data->base+456+selector*64)&(1u<<bit);}
  return *(u16 *)(ref->car->data->base+456+selector*64)&(1u<<bit);
 }
 return 1;
}
