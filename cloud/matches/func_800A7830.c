/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned int u32;
typedef struct RecordSlot {
    unsigned char unknown_00[52];
    u32 value;
    unsigned char unknown_38[12];
} RecordSlot;
extern RecordSlot D_8012E700[];

/* The third incoming word is homed but not read by the native body. */
void func_800A7830(short index, u32 value, u32 unused)
{
    D_8012E700[index].value = value;
}
