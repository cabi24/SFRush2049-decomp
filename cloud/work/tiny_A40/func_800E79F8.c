/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;typedef float f32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
typedef struct Block {u32 pad0;struct Block *next;u32 prev,size,owner;s8 used;u8 pad21[11];} Block;
typedef struct Heap {u32 pad0,pad4;Block *blocks;} Heap;
typedef struct OSMesgQueue OSMesgQueue;
extern Heap *D_801527C8;extern OSMesgQueue D_80152770;
s32 osRecvMesg(OSMesgQueue *,void **,s32);s32 osJamMesg(OSMesgQueue *,void *,s32);
u32 func_800E79F8(Heap *heap) {
 Block *block;Heap *selected;u32 largest;
 osRecvMesg(&D_80152770,0,1);
 if(heap)selected=heap;else selected=D_801527C8;
 block=selected->blocks;largest=0;
 while(block) {
  if(block->used==0 && largest<block->size)largest=block->size;
  block=block->next;
 }
 osJamMesg(&D_80152770,0,0);
 return largest;
}
