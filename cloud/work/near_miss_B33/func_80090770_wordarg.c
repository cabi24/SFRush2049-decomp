/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed short s16; typedef int s32;
typedef unsigned char u8;
extern struct Slot {u8 prefix[0x14];s16 value;u8 suffix[0x2E];} D_8012E700[];
void func_80090770(s16 index,s32 value) {
    D_8012E700[index].value=value;
}
