/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001BDB8 - lib_1a660 (IDO 7.1). Does channel INDEX have a pending command? True when its
 * state (D_8004F300[index], 0x28-byte ChannelState) is not of kind 4, has a pending word and
 * its flags word has bit 31 set.
 * Base: w17g lockonly/func_8001BDB8.c (D10 body; LOCK-exact under 7.1, TU-blocked: the TU
 * defines ChannelState only after this slot). w18c: the TU's own ChannelState and D_8004F300
 * declarations (func_8001C1D8's block, verbatim), hoisted above this slot by a TU edit
 * (RESULTS.md "TU edits"); the TU types flags08 as u32, so the sign test is written
 * `(int)state->flags08 < 0` (same code as D10's int field). No other shaping devices. */
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct ChannelState {u32 word00,word04,flags08,pending0C,unknown10;u8 kind14;u8 unknown15[3];u32 value18;u8 unknown1C[12];} ChannelState;
extern ChannelState D_8004F300[32];

int func_8001BDB8(u8 index)
{
    ChannelState *state;
    state = &D_8004F300[index];
    if (state->kind14 != 4 && state->pending0C != 0 && (int)state->flags08 < 0) return 1;
    return 0;
}
