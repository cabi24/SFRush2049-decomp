/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
#pragma pack(1)
typedef struct VoiceState {
    u32 command00;
    u8 unknown04[12];
    u32 next_identifier;
    u8 unknown14[16];
    u32 flags24;
    u8 unknown28[36];
    u8 external4C;
    u8 unknown4D[19];
    u32 identifier60;
    u8 unknown64[89];
    u8 activeBD;
    u8 unknownBE[226];
} VoiceState;
#pragma pack(0)
extern VoiceState D_8004BEB8[];
extern u8 D_8004FA18;
extern void func_8001F9D0(VoiceState *);
extern void func_80014B3C(int);
void func_8001FAE4(u8 preserve_external)
{
    int i;
    for (i = 0; i < D_8004FA18; i++) {
        if (D_8004BEB8[i].command00 != 0) {
            if (!preserve_external ||
                (preserve_external && !D_8004BEB8[i].external4C)) {
                func_8001F9D0(&D_8004BEB8[i]);
                D_8004BEB8[i].command00 = 0;
                D_8004BEB8[i].flags24 &= ~3U;
            } else {
                continue;
            }
        }
        func_80014B3C(i);
    }
}
