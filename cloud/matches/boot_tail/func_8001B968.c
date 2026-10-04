/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
#pragma pack(1)
typedef struct VoiceState {
    u8 unknown00[36];
    u32 flags;
    u8 unknown28[56];
    u32 identifier;
    u8 unknown64[94];
    u16 valueC2;
    u8 unknownC4[220];
} VoiceState;
#pragma pack(0)
extern VoiceState D_8004BEB8[];
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
