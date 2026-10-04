/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct ChannelLink { u8 previous; u8 next; u16 flags; } ChannelLink;
extern u8 D_8004FA18;
extern ChannelLink D_80050440[32];
extern u8 D_800504C0;
extern u8 D_800504C1;

void func_8001F7EC(void)
{
    u32 i;
    u32 count;
    count = D_8004FA18;
    for (i = 0; i < count; i++) {
        D_80050440[i].previous = i - 1;
        D_80050440[i].next = i + 1;
        D_80050440[i].flags = 1;
    }
    D_80050440[0].previous = 255;
    D_80050440[count - 1].next = 255;
    D_800504C0 = 0;
    D_800504C1 = count - 1;
}
