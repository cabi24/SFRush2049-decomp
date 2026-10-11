/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned char u8;typedef signed int s32;typedef unsigned int u32;
typedef struct Slot20 {u8 pad0[6];u8 resource;u8 pad7;u32 word8,allocation,word16;} Slot20;
extern Slot20 D_80156D38[64];
void func_80096130(s32 index);
void wheel_setup_initial(s32 resource) {
    s32 i;
    Slot20 *slot=D_80156D38;
    for(i=0;i<64;i++,slot++){
        if(resource) {}
        if(slot->allocation!=0 && slot->resource==resource)func_80096130(i);
    }
}
