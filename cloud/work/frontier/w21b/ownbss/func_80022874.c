/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80022874 - lib_22300 (IDO 7.1). Macro command "start sample": looks the sample id (word[0] bits 8..23)
 * up into the record (func_80016CF0 = sample-info lookup, 0 on success); the start offset is word[1] (mode 0),
 * scaled by (127 - velocity) / 127 (mode 1) or velocity / 127 (mode 2, velocity = volume30 >> 16), else 0,
 * clamped below the sample length; starts it on the voice (func_800146B4: voice index, record, 1 unless flag
 * 0x200) and stores the sample info/address into the voice, setting flag 0x20. Returns 0.
 * w21b (from w20b best, true=1710): the start call is written as two calls in an if/else on the flag
 * (`if (!(flags24 & 0x200)) f(index, &rec, 1); else f(index, &rec, 0);`, retail's hoisted `move a2,zero`
 * and two jals), with the voice index computed into a local just before it (one `lwl/lwr; andi` before the
 * branch). That also moves &rec's materialisation to after the lookup call (PRE to the common dominator of
 * the two calls) and keeps `state` in its home slot (true=1710 -> 0).
 * w20b lever kept: the sample record is a FUNCTION-LOCAL STATIC (retail addresses each field `lui; sw/lw`
 * and never CSEs the base with the call argument; an extern struct does). It is the 28-byte .bss record at
 * 0x80056208 (+8 offset = label D_80056210, +12 length = D_80056214, +4 = D_8005620C): code identical
 * (true=0), objdump rows differ only by `.bss+N` vs the splat labels. Needs an owned .bss storage block in
 * lib_22300 at 0x80056208 (see RESULTS.md). The static's name carries its address (asm-processor needs
 * unique static names in a ROM TU). No devices, no unused locals. Types: private MacroState_80022874 view
 * (the TU's MacroState has other field names), SampleInfo_80022874 (MusyX-like sample info record). */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
typedef struct SampleInfo_80022874 { u32 info; u32 address; u32 offset; u32 length; u32 loopStart; u32 loopLength; u8 type; } SampleInfo_80022874;
#pragma pack(1)
typedef struct MacroState_80022874 { u8 unknown00[0x24]; u32 flags24; u8 unknown28[8]; u32 volume30; u8 unknown34[0x24]; u32 sampleAddress58; u32 sampleInfo5C; u32 identifier60; } MacroState_80022874;
#pragma pack(0)
extern int func_80016CF0(u16, SampleInfo_80022874 *);
extern void func_800146B4(u32, SampleInfo_80022874 *, u8);
u8 func_80022874(MacroState_80022874 *state, MacroCommand *command)
{
    static SampleInfo_80022874 sample_80056208;
    u32 index;
    if (func_80016CF0(command->word[0] >> 8, &sample_80056208) == 0) {
        switch ((u8)(command->word[0] >> 24)) {
        case 0:
            sample_80056208.offset = command->word[1];
            break;
        case 1:
            sample_80056208.offset = (command->word[1] * (127U - (state->volume30 >> 16))) / 127U;
            break;
        case 2:
            sample_80056208.offset = (command->word[1] * (state->volume30 >> 16)) / 127U;
            break;
        default:
            sample_80056208.offset = 0;
        }
        if (sample_80056208.offset >= sample_80056208.length) {
            sample_80056208.offset = sample_80056208.length - 1;
        }
        index = state->identifier60 & 255;
        if (!(state->flags24 & 0x200)) {
            func_800146B4(index, &sample_80056208, 1);
        } else {
            func_800146B4(index, &sample_80056208, 0);
        }
        state->sampleInfo5C = sample_80056208.info;
        state->sampleAddress58 = sample_80056208.address;
        state->flags24 |= 0x20;
    }
    return 0;
}
