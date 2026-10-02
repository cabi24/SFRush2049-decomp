/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;typedef float f32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
typedef struct Car {u8 bytes[772];} Car;typedef struct List {void *node;u8 pad4[12];} List;
extern Car D_80144030[];extern List D_80144D68[];
void func_800A1BB4(s32 index) {
 void *head,*node,*next;Car *car=&D_80144030[index];
 if(FIELD(car,s8,11)==0)return;
 head=D_80144D68[index].node;
 while(head) {
 node=FIELD(head,void *,0);next=FIELD(node,void *,0);
 if(FIELD(node,u32,72)!=0 && FIELD(&D_80144030[index],s8,133+FIELD(node,u8,17)*40)!=0)break;
 head=next;
 }
 if(head==0)FIELD(car,s8,11)=0;
}
