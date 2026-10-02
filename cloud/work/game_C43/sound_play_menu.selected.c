/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned int u32;
typedef int s32;
typedef signed char s8;
typedef unsigned char u8;
typedef struct Block { u32 magic; struct Block *previous, *next; u32 size, value; s8 used; u8 flag21, flag22, pad23[9]; } Block;
typedef struct Arena { u32 field0,field4,field8; Block *first; } Arena;
extern u8 D_80152770[];
extern Arena *D_801527C8;
extern s32 osRecvMesg(void *, void *, s32);
extern s32 osJamMesg(void *, void *, s32);
void *sound_play_menu(Arena *arena, u32 size) {
    Block *block;
    Block *split;
    Arena *selected;
    u32 total;
    u32 largest;
    osRecvMesg(D_80152770,0,1);
    total=0;
    largest=0;
    if(arena) selected=arena; else selected=D_801527C8;
    block=selected->first;
    size=(size+31)&~31;
    while(block) {
        if(!block->used) {
            total+=block->size;
            if(largest<block->size) largest=block->size;
            if(block->size>=size) break;
        }
        block=block->next;
    }
    if(block->size-size>=64) {
        split=(Block *)((u8 *)block+block->size-size);
        split->previous=block->previous;
        if(block->previous) block->previous->next=split;
        else selected->first=split;
        split->next=block;
        split->size=size;
        split->value=0;
        split->used=1;
        split->flag21=0;
        split->flag22=0;
        split->magic=0xFEDCBA98;
        block->previous=split;
        block->size-=size+32;
        block=split;
    } else {
        block->value=0;
        block->used=1;
        block->flag21=0;
    }
    osJamMesg(D_80152770,0,0);
    return (u8 *)block+32;
}
