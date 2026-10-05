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
extern void func_80018D40(u32);
void func_80018E6C(void)
{
    int i;
    for (i = 0; i < 7; i++) {
        func_80018D40(D_80043EB8[i].identifier);
    }
}
