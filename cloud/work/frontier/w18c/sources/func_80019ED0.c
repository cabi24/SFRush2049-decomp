/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80019ED0 - lib_1a660 (IDO 7.1). Find the active voice for KEY on CHANNEL/SET: when the
 * channel is valid (not 255) and func_80021028(channel, set) reports it enabled (lib_21210,
 * locked), return func_80019C8C(key & 127, channel, set) (lib_1a660, locked: voice identifier
 * or 0xFFFFFFFF); otherwise 0xFFFFFFFF.
 * w18c: the key mask is written in the argument (`key & 127`); D10's `key &= 127;` statement
 * moves the key reload ahead of the call's delay-slot load (true=210). No shaping devices.
 * Prior: D10 BT03-high-params (5.3, 7 words). */
typedef unsigned char u8;
typedef unsigned int u32;
extern u8 func_80021028(u8, u8);
extern u32 func_80019C8C(u8, u8, u8);

u32 func_80019ED0(u8 key, u8 channel, u8 set)
{
    u32 identifier;
    if (channel != 255 && func_80021028(channel, set) != 0) {
        identifier = func_80019C8C(key & 127, channel, set);
        if (identifier != 0xFFFFFFFFU) {
            return identifier;
        }
    }
    return 0xFFFFFFFFU;
}
