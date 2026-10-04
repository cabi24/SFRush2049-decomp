/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native row strides; overall external array bounds are not inferred. */
typedef unsigned char u8;
typedef unsigned short u16;
extern u8 D_80050D00[][16][134];
extern u8 D_80055000[][134];

u16 func_80020A04(u8 controller, u8 channel, u8 set)
{
    if (set != 255) {
        if (controller < 64) {
            return (D_80050D00[set][channel][controller & 31] << 7) |
                   D_80050D00[set][channel][(controller & 31) + 32];
        } else if (controller == 128 || controller == 129) {
            return (D_80050D00[set][channel][controller & 254] << 7) |
                   D_80050D00[set][channel][(controller & 254) + 1];
        } else if (controller == 132 || controller == 133) {
            return (D_80050D00[set][channel][controller & 254] << 7) |
                   D_80050D00[set][channel][(controller & 254) + 1];
        } else if (controller < 70) {
            return D_80050D00[set][channel][controller] < 64 ? 0 : 16383;
        } else if (controller >= 96 && controller <= 101) {
            return 0;
        } else {
            return D_80050D00[set][channel][controller] << 7;
        }
    } else {
        if (controller < 64) {
            return (D_80055000[channel][controller & 31] << 7) |
                   D_80055000[channel][(controller & 31) + 32];
        } else if (controller == 128 || controller == 129) {
            return (D_80055000[channel][controller & 254] << 7) |
                   D_80055000[channel][(controller & 254) + 1];
        } else if (controller == 132 || controller == 133) {
            return (D_80055000[channel][controller & 254] << 7) |
                   D_80055000[channel][(controller & 254) + 1];
        } else if (controller < 70) {
            return D_80055000[channel][controller] < 64 ? 0 : 16383;
        } else if (controller >= 96 && controller <= 101) {
            return 0;
        } else {
            return D_80055000[channel][controller] << 7;
        }
    }
}
