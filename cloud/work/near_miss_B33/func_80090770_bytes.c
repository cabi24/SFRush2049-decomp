/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed short s16;
typedef unsigned char u8;
extern u8 D_8012E700[];
void func_80090770(s16 index,s16 value) {
    *(s16*)(D_8012E700+index*0x44+0x14)=value;
}
