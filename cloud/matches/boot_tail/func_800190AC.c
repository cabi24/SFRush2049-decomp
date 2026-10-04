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
extern u32 func_80017644(u32);
void func_800190AC(u32 identifier, u32 first, u32 second)
{
    u32 index;
    index = func_80017644(identifier);
    if (index != 0xFFFFFFFFU) {
        if ((index & 0x80000000U) == 0) {
            D_80043EB8[index].first110 = first;
            D_80043EB8[index].second114 = second;
        } else {
            index &= 0x7FFFFFFFU;
            D_80043EB8[index].firstFE4 = first;
            D_80043EB8[index].secondFE8 = second;
            D_80043EB8[index].flagsFEE |= 0x10;
        }
    }
}
