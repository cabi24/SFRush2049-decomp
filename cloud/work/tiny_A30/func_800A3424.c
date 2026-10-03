/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;typedef float f32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
typedef struct Car {u8 bytes[772];} Car;extern Car D_80144030[];
void func_800A3424(void *owner,s32 enabled) {
 void *node;u32 index;Car *car;u8 *slot;s32 offset;
 if(owner) {
 node=FIELD(owner,void *,0);index=FIELD(node,u8,16);car=&D_80144030[index];
 slot=(u8 *)car+FIELD(node,u8,17)*40;
 if(enabled==FIELD(slot,s8,132))return;
 FIELD(slot,s8,132)=enabled;
 if(enabled)FIELD(&D_80144030[index],s8,10)=1;
 else {
 for(offset=0;offset<640;offset+=40)if(FIELD(car,s8,132+offset)!=0)break;
 if(offset>=640) {FIELD(&D_80144030[index],s8,10)=0;if(FIELD(&D_80144030[index],s8,9)==0)FIELD(&D_80144030[index],s8,1)=0;}
 }
 }
}
