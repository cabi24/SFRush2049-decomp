/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80017824 - lib_17dc0 (IDO 7.1). Sets MIDI controller 7 for a channel in the current set:
 * func_80020610(controller, channel, set, value) (lib_21210, locked) with set = D_8004BE7B
 * (the low byte of sequence_context.h's u32 D_8004BE78; the target relocates to D_8004BE7B itself).
 * w20b lever: line layout only. With the call on the same source line as the opening brace
 * (w18c h5.c, one-line body) as1 schedules the entry narrowing of channel (andi a1) into the jal
 * delay slot (true=120, 4 rows); with the call on its own line retail's order comes out (andi a1
 * between lui/lbu, li a0 in the delay slot). No devices, no unused locals. */
typedef unsigned char u8;
extern u8 D_8004BE7B;
extern void func_80020610(u8 controller, u8 channel, u8 set, u8 value);

void func_80017824(u8 value, u8 channel)
{
    func_80020610(7, channel, D_8004BE7B, value);
}
