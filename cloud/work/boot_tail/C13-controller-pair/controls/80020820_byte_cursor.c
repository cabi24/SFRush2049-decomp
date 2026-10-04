/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned int u32;
extern u8 D_80050D00[][16][134];
extern u8 D_80055000[][134];
extern void func_80020610(u8, u8, u8, u8);
extern void func_80020F4C(u8, u8, u8);
extern void func_80020FDC(u8, u8, u8);
void func_80020820(u8 channel, u8 set)
{
    u32 i;
    u8 *data;
    if (set != 255) {
        data = D_80050D00[set][channel];
        for (i = 0; i < 134; i++) *data++ = 0;
    } else {
        data = D_80055000[channel];
        for (i = 0; i < 134; i++) *data++ = 0;
    }
    func_80020610(7, channel, set, 127);
    func_80020610(10, channel, set, 64);
    func_80020610(128, channel, set, 64);
    func_80020610(129, channel, set, 0);
    func_80020610(64, channel, set, 0);
    func_80020610(65, channel, set, 0);
    func_80020610(91, channel, set, 0);
    func_80020610(131, channel, set, 0);
    func_80020610(132, channel, set, 64);
    func_80020610(133, channel, set, 0);
    func_80020F4C(channel, set, 255);
    func_80020FDC(channel, set, 0);
}
