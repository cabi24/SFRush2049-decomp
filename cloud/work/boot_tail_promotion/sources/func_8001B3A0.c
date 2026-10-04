/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_8001B3A0.c: file-local type names VoiceState suffixed _8001B3A0 so several bodies share one ROM TU; no other change. */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
#pragma pack(1)
typedef struct VoiceState_8001B3A0 {
    u32 command00;
    u8 unknown04[12];
    u32 next10;
    u8 unknown14[16];
    u32 flags24;
    u8 unknown28[3];
    u8 channel2B;
    u8 unknown2C[2];
    u8 channel2E;
    u8 unknown2F[28];
    u8 channel4B;
    u8 external4C;
    u8 unknown4D[8];
    u8 channel55;
    u8 unknown56[10];
    u32 identifier60;
    u8 unknown64[316];
} VoiceState_8001B3A0;
#pragma pack(0)
extern VoiceState_8001B3A0 D_8004BEB8[];
extern u32 func_8001EDF4(u32);
extern void func_80020610(u8, u8, u8, u8);
int func_8001B3A0(u32 identifier, u8 value)
{
    int result;
    u32 index;
    result = -1;
    identifier = func_8001EDF4(identifier);
    while (identifier != 0xFFFFFFFFU) {
        index = identifier & 255;
        if (D_8004BEB8[index].identifier60 == identifier) {
            result = 0;
            if (D_8004BEB8[index].flags24 & 2) {
                func_80020610(131, index, D_8004BEB8[index].channel55, value);
            } else {
                func_80020610(131, index, D_8004BEB8[index].channel4B, value);
            }
            identifier = D_8004BEB8[index].next10;
        } else {
            return result;
        }
    }
    return result;
}
