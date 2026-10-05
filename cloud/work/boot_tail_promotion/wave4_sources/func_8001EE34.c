/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_8001EE34.c to the production
 * src/rom/lib_1f5b0.c channel-link record (hoisted in wave 4); code unchanged. */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct ChannelLink_8001EE9C { u8 previous, next; u16 active; } ChannelLink_8001EE9C;
extern u8 D_8004FA18;
extern ChannelLink_8001EE9C D_800504C8[32];
extern u8 D_80050548[256];
extern u16 D_80050A48;

void func_8001EE34(void)
{
    u32 i;
    for (i = 0; i < D_8004FA18; i++) {
        D_800504C8[i].active = 0;
    }
    for (i = 0; i < 256; i++) {
        D_80050548[i] = 255;
    }
    D_80050A48 = 65535;
}
