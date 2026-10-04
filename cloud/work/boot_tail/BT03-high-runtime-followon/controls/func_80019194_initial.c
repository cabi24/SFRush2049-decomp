/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char s8;
typedef signed short s16;
typedef struct SequenceContext {
    u8 unknown000[1320];
    u8 channels528[64];
    u8 unknown568[2652];
    u8 channelFC4;
    u8 unknownFC5[27];
    u8 valueFE0;
    u8 unknownFE1[13];
    u8 flagsFEE,unknownFEF;
    u32 pendingFF0;
    u8 unknownFF4[4];
} SequenceContext;
extern SequenceContext D_80043EB8[8];
extern u32 func_80017644(u32);
extern void func_8001B9F8(u8,u16,u8,u8,u32);
void func_80019194(u8 channel,u16 duration,u32 identifier,u8 flags)
{
    u32 index;
    int i;
    index=func_80017644(identifier);
    if (index != 0xFFFFFFFFU) {
        if (!(index & 0x80000000U)) {
            func_8001B9F8(channel,duration,D_80043EB8[index].channelFC4,flags,identifier);
            for (i=0;i<64;i++) {
                if (D_80043EB8[index].channels528[i] != D_80043EB8[index].channelFC4) {
                    func_8001B9F8(channel,duration,D_80043EB8[index].channels528[i],0,0);
                }
            }
        } else {
            index &= 0x7FFFFFFFU;
            switch (flags & 15) {
            case 0: D_80043EB8[index].valueFE0=channel; break;
            case 1: D_80043EB8[index].pendingFF0=0; break;
            case 2:
                D_80043EB8[index].valueFE0=channel;
                D_80043EB8[index].flagsFEE |= 8;
                break;
            case 3:
                D_80043EB8[index].valueFE0=channel;
                D_80043EB8[index].flagsFEE |= 128;
                break;
            }
        }
    }
}
