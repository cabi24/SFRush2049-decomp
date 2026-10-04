/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native macro helper reconstruction; only observed state layout is modeled. */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
extern u8 *func_80016E68(u16);
u32 func_8002321C(u32 volume, u16 curve)
{
    u8 *table;
    u32 high;
    u32 low;
    u32 difference;
    if (curve != 0) {
        if ((table = func_80016E68(curve)) != 0) {
            high = volume >> 16;
            low = volume & 0xFFFF;
            if (high < 127) {
                difference = low * (table[high + 1] - table[high]);
                volume = difference + (table[high] << 16);
            } else {
                volume = table[high] << 16;
            }
        }
    }
    return volume;
}
