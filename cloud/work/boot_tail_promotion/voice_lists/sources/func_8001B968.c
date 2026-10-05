/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Promotion adaptation: use lib_1a660.c's existing voice-array view;
 * typed channel+0x55/value+0xC2 and signed lookup failure contract. */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char s8;
#pragma pack(1)
typedef struct VoiceState_80019C8C { u8 unknown00[16];u32 child,parent;u8 unknown18[12]; u32 flags;u8 unknown28[34];u8 channel,set;u8 unknown4C[2]; u16 base_key,key;u8 unknown52[3];u8 channel55;u8 unknown56[10];u32 identifier; u8 unknown64[40];u32 glide;u8 unknown90[4];u32 pitch; u8 unknown98[40];s8 cents;u8 original_key;u16 valueC2;u8 unknownC4[220]; } VoiceState_80019C8C;
#pragma pack(0)
extern VoiceState_80019C8C D_8004BEB8[];
extern int func_8001EDF4(u32);
u16 func_8001B968(u32 key)
{
    int identifier;
    identifier = func_8001EDF4(key);
    if (identifier != -1) {
        if (D_8004BEB8[identifier & 255].identifier == (u32)identifier &&
            !(D_8004BEB8[identifier & 255].flags & 2)) {
            return D_8004BEB8[identifier & 255].valueC2;
        }
    }
    return 0;
}
