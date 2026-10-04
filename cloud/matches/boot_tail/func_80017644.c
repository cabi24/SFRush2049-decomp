/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct VoiceRecord {
    u32 identifier;
    u8 unknown004[0x10C];
    u32 first110;
    u32 second114;
    u8 unknown118[0xEA8];
    u8 activeFC0;
    u8 inactiveFC1;
    u16 valueFC2;
    u8 unknownFC4[0x20];
    u32 firstFE4;
    u32 secondFE8;
    u16 valueFEC;
    u8 flagsFEE;
    u8 unknownFEF[9];
} VoiceRecord;
extern VoiceRecord D_80043EB8[8];
u32 func_80017644(u32 identifier)
{
    int i;
    for (i = 0; i < 8; ++i) {
        if (D_80043EB8[i].inactiveFC1 == 0 &&
            D_80043EB8[i].identifier == (identifier & 0x7FFFFFFFU)) {
            return (identifier & 0x80000000U) | i;
        }
    }
    return 0xFFFFFFFFU;
}
