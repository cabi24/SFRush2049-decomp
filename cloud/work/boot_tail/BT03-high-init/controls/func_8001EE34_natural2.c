/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct ChannelValue { u16 unknown; u16 value; } ChannelValue;
typedef struct ChannelMap { u8 a; u8 b; u8 c; u8 d; } ChannelMap;
extern u8 D_8004FA18;
extern ChannelValue D_800504C8[];
extern ChannelMap D_80050548[64];
extern u16 D_80050A48;

void func_8001EE34(void)
{
    u32 i;
    for (i = 0; i < D_8004FA18; i++) {
        D_800504C8[i].value = 0;
    }
    for (i = 0; i != 64; i++) {
        D_80050548[i].a = 255;
        D_80050548[i].b = 255;
        D_80050548[i].c = 255;
        D_80050548[i].d = 255;
    }
    D_80050A48 = 65535;
}
