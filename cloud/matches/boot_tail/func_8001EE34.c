/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct ChannelValue { u16 unknown; u16 value; } ChannelValue;
extern u8 D_8004FA18;
extern ChannelValue D_800504C8[];
extern u8 D_80050548[256];
extern u16 D_80050A48;

void func_8001EE34(void)
{
    u32 i;
    for (i = 0; i < D_8004FA18; i++) {
        D_800504C8[i].value = 0;
    }
    for (i = 0; i < 256; i++) {
        D_80050548[i] = 255;
    }
    D_80050A48 = 65535;
}
