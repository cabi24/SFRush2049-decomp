/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;typedef float f32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
typedef struct Block {u32 magic;struct Block *next;u32 prev;u32 size;u32 owner;s8 used;} Block;
typedef struct Heap {u32 magic;struct Heap *next;Block *blocks;} Heap;
typedef struct OSMesgQueue OSMesgQueue;
extern Heap *D_801527C8;extern OSMesgQueue D_80152770;
s32 osRecvMesg(OSMesgQueue *,void **,s32);s32 osJamMesg(OSMesgQueue *,void *,s32);
u32 audio_output_setup(Heap *heap) {
 Block *block;u32 sum=0;
 osRecvMesg(&D_80152770,0,1);
 if(heap==0)heap=D_801527C8;
 for(block=heap->blocks;block;block=block->next) if(block->used==0)sum+=block->size;
 osJamMesg(&D_80152770,0,0);
 return sum;
}
