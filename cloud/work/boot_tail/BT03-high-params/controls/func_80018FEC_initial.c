/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct VoiceRecord {
    u32 identifier;
    u8 unknown004[0xFBC];
    u8 activeFC0;
    u8 unknownFC1[0x2D];
    u8 flagsFEE;
    u8 unknownFEF[9];
} VoiceRecord;
extern VoiceRecord D_80043EB8[8];
extern u32 func_80017644(u32);
void func_80018FEC(u32 identifier)
{
    u32 index;
    index = func_80017644(identifier);
    if (index != 0xFFFFFFFFU) {
        if ((index & 0x80000000U) == 0) {
            D_80043EB8[index].activeFC0 = 1;
        } else {
            index &= 0x7FFFFFFFU;
            D_80043EB8[index].flagsFEE &= ~8;
        }
    }
}
