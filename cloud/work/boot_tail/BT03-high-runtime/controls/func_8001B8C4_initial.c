/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned int u32;
#pragma pack(1)
typedef struct VoiceState {
    u8 unknown00[16];
    u32 next_identifier;
    u8 unknown14[16];
    u32 flags;
    u8 unknown28[56];
    u32 identifier;
    u8 unknown64[316];
} VoiceState;
#pragma pack(0)
extern VoiceState D_8004BEB8[];
extern u8 D_8002C630;
extern int func_8001EDF4(u32);
int func_8001B8C4(u32 key)
{
    u32 identifier;
    int result;
    result = -1;
    if (D_8002C630) {
        identifier = func_8001EDF4(key);
        while (identifier != 0xFFFFFFFFU) {
            if (D_8004BEB8[identifier & 255].identifier == identifier) {
                D_8004BEB8[identifier & 255].flags |= 8;
                result = 0;
            }
            identifier = D_8004BEB8[identifier & 255].next_identifier;
        }
    }
    return result;
}
