/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;typedef float f32;
typedef struct Block {u32 magic;struct Block *next;u32 prev;u32 size;u32 owner;s8 used;} Block;
typedef struct Heap {u32 magic;struct Heap *next;Block *blocks;} Heap;
typedef struct OSMesgQueue OSMesgQueue;
extern Heap *D_801527C8;extern OSMesgQueue D_80152770;
s32 osRecvMesg(OSMesgQueue *,void **,s32);s32 osJamMesg(OSMesgQueue *,void *,s32);

u32 func_800BAF90(Heap *heap) {
    Block *block;
    u32 sum;

    if (heap == 0) {
        heap = D_801527C8;
    }
    sum = 0;
    for (block = heap->blocks; block != 0; block = block->next) {
        if (block->used == 0) {
            sum += block->size;
        }
    }
    return sum;
}

u32 audio_output_setup(Heap *heap) {
    u32 sum;

    osRecvMesg(&D_80152770, 0, 1);
    sum = func_800BAF90(heap);
    osJamMesg(&D_80152770, 0, 0);
    return sum;
}
