/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Complete reconstruction; matching status is recorded in the packet receipt. */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
extern void func_80020610(u8 controller, u8 channel, u8 set, u8 value);

void func_800206AC(u8 controller, u8 channel, u8 set, u16 value)
{
    u32 index;
    if (controller < 64) {
        index = controller & 31;
        func_80020610(index, channel, set, value >> 7);
        func_80020610(index + 32, channel, set, value & 127);
    } else if (controller == 128 || controller == 129) {
        index = controller & 254;
        func_80020610(index, channel, set, value >> 7);
        func_80020610(index + 1, channel, set, value & 127);
    } else if (controller == 132 || controller == 133) {
        index = controller & 254;
        func_80020610(index, channel, set, value >> 7);
        func_80020610(index + 1, channel, set, value & 127);
    } else {
        func_80020610(controller, channel, set, value >> 7);
    }
}
