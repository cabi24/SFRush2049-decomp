/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
#pragma pack(1)
typedef struct VoiceState {
    u32 command00;
    u8 unknown04[32];
    u32 flags24,age28;
    u8 unknown2C[2];
    u8 channel2E,unknown2F;
    u32 volume30;
    u8 unknown34[4];
    u32 panning38;
    u8 unknown3C[4];
    u32 words40[2];
    u8 status48,status49,channel4A;
    u8 unknown4B[21];
    u32 identifier60;
    u8 unknown64[4];
    u16 loop68;
    u8 unknown6A[34];
    u32 word8C;
    u8 unknown90[8];
    u8 state98,depth99,flag9A;
    u8 unknown9B[34];
    u8 activeBD,groupBE;
    u8 unknownBF[173];
    u32 word16C;
    u16 half170;
    u8 unknown172[6];
    u32 word178;
    u16 half17C;
    u8 unknown17E[34];
} VoiceState;
#pragma pack(0)
typedef struct ChannelState {u32 word00,word04,flags08,pending0C,unknown10;u8 kind14;u8 unknown15[3];u32 value18;u8 unknown1C[12];} ChannelState;
extern VoiceState D_8004BEB8[32];
extern ChannelState D_8004F300[32];
extern short D_8004BE98[16];
extern u32 D_8004F800,D_8004F804;
extern u8 D_8004F2F8;
extern void func_80019A60(u32,u8);
extern void func_8001E9B0(void);
extern void func_8001F864(void);
extern void func_800218CC(void);
void func_8001C1D8(u32 configuration)
{
    int i;
    D_8004F800 = configuration;
    D_8004F804 = 10240;
    func_80019A60(120,255);
    D_8004F2F8 = 0;
    for (i=0;i<32;i++) {
        D_8004BEB8[i].identifier60 = 0xFFFFFFFFU;
        D_8004BEB8[i].command00 = 0;
        D_8004BEB8[i].flags24 = 0;
        D_8004BEB8[i].age28 = 0;
        D_8004BEB8[i].channel2E = 0;
        D_8004BEB8[i].loop68 = 0;
        D_8004BEB8[i].channel4A = 255;
        D_8004BEB8[i].volume30 = 0;
        D_8004BEB8[i].depth99 = 128;
        D_8004BEB8[i].flag9A = 0;
        D_8004BEB8[i].panning38 = 0x3F0000;
        D_8004BEB8[i].words40[0] = 0;
        D_8004BEB8[i].words40[1] = 0;
        D_8004BEB8[i].status48 = 0;
        D_8004BEB8[i].status49 = 0;
        D_8004BEB8[i].activeBD = 0;
        D_8004BEB8[i].groupBE = 23;
        D_8004BEB8[i].word16C = 0;
        D_8004BEB8[i].half170 = 0;
        D_8004BEB8[i].word178 = 0;
        D_8004BEB8[i].half17C = 0;
        D_8004BEB8[i].word8C = 100;
        D_8004BEB8[i].state98 = 0;
    }
    for (i=0;i<32;i++) {
        D_8004F300[i].word00 = 0;
        D_8004F300[i].word04 = 0;
        D_8004F300[i].pending0C = 0;
        D_8004F300[i].kind14 = 4;
        D_8004F300[i].value18 = 0x7F0000;
    }
    D_8004F300[31].kind14 = 1;
    for (i=0;i<8;i++) D_8004F300[i+23].kind14 = 0;
    D_8004F300[21].word00 = 0x7F0000;
    D_8004F300[22].word00 = 0x7F0000;
    func_8001E9B0();
    func_8001F864();
    for (i=0;i<16;i++) {
        D_8004BE98[i] = 0;
    }
    func_800218CC();
}
