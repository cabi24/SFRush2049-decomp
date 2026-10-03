/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native 4088-byte voice-record reconstruction; original field names unknown. */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct VoiceState {
    u8 unknown000[0x528];
    u8 selector528[256];
    u8 unknown628[0x998];
    u8 byteFC0;
    u8 byteFC1;
    u8 unknownFC2[2];
    u8 byteFC4;
    u8 byteFC5;
    u16 valueFC6;
    u8 unknownFC8[0x30];
} VoiceState;
extern VoiceState D_80043EB8[8];
extern u8 D_8002C630;
extern u32 func_80017644(u32);
int func_80018EB4(u32 identifier, u8 enabled)
{
    u32 index;
    if (D_8002C630) {
        index = func_80017644(identifier);
        if (index != 0xFFFFFFFF && (index & 0x80000000) == 0) {
            D_80043EB8[index].byteFC5 = !enabled;
            return 1;
        }
    }
    return 0;
}
