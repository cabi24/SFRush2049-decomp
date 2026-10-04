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
extern u32 D_8004BE84;
u32 func_800175B4(u32 slot)
{
    u32 identifier;
    int i;
    do {
        identifier = D_8004BE84++;
        D_8004BE84 &= 0x7FFFFFFFU;
        for (i = 0; i < 8; i++) {
            if (D_80043EB8[i].inactiveFC1 == 0 &&
                D_80043EB8[i].identifier == identifier) {
                identifier = 0xFFFFFFFFU;
                break;
            }
        }
    } while (identifier == 0xFFFFFFFFU);
    D_80043EB8[slot].identifier = identifier;
    return identifier;
}
