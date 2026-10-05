/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* NEAR-MISS 15/66 in the unit with wheel_setup_initial (28/31). Score: blob_unit score func_80096130 wheel_setup_initial --with best.c --internal heap_release_locked --internal slot_tables_clear --internal wait_idle */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef signed int s32;typedef unsigned int u32;
#define NULL ((void *)0)
typedef struct OSMesgQueue OSMesgQueue;
extern OSMesgQueue D_80152770;
s32 osRecvMesg(OSMesgQueue *,void **,s32);s32 osJamMesg(OSMesgQueue *,void *,s32);
void audio_reverb_update(u32 address,s32 tag);
typedef struct Slot20 {u8 pad0[6];u8 resource;u8 pad7;u32 word8,allocation,word16;} Slot20;
extern Slot20 D_80156D38[64];
extern volatile s16 D_8002EB70;
extern u8 D_801161F4[],D_80151AE8[],D_80138670[];
void *memset(void *,s32,u32);
void heap_release_locked(u32 address) {
    osRecvMesg(&D_80152770,NULL,1);
    audio_reverb_update(address,0);
    osJamMesg(&D_80152770,NULL,0);
}
void slot_tables_clear(s32 index) {
    memset(D_801161F4+index*8,0,8);
    memset(D_80151AE8+index*8,0,8);
    memset(D_80138670+index*8,0,8);
}
void wait_idle(void) {
    while(D_8002EB70!=0){}
}
void func_80096130(s32 index) {
    Slot20 *slot=&D_80156D38[index];
    u32 allocation=slot->allocation;
    if (slot == NULL) {
    }
    if(allocation!=0){
        wait_idle();
        heap_release_locked(allocation);
        slot->allocation=0;
        slot_tables_clear(index);
    }
}
void wheel_setup_initial(s32 resource) {
    s32 i;
    Slot20 *slot=D_80156D38;
    for(i=0;i<64;i++,slot++){
        if(slot->allocation!=0 && slot->resource==resource)func_80096130(i);
    }
}
