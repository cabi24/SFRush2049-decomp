/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned int u32;
extern u8 func_80021028(u8, u8);
extern u32 func_80019C8C(u8, u8, u8);
u32 func_80019ED0(u8 key, u8 channel, u8 set)
{
    u32 identifier;
    if (channel != 255 && func_80021028(channel, set) != 0) {
        key &= 127;
        identifier = func_80019C8C(key, channel, set);
        if (identifier != 0xFFFFFFFFU) {
            return identifier;
        }
    }
    return 0xFFFFFFFFU;
}
