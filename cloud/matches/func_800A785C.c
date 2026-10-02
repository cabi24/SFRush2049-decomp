/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed short s16;typedef unsigned char u8;typedef unsigned int u32;
typedef struct Slot {u32 value;u8 pad4[64];} Slot;
extern Slot D_8012E738[];
void func_800A785C(s16 index,u32 value) {
 D_8012E738[index].value=value;
}
