/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80023D70 - lib_22300 (IDO 7.1). Macro command handler: forwards the command's byte
 * operands (word0 bits 8-15 and 16-23) and its signed 16-bit operand (low half of word1) to
 * func_80023B50 (unmatched here; the "set variable" helper), and returns 0 (continue).
 * w18c: the low half is taken as `(short)(word[1] & 0xFFFF)` (a LOWORD-style macro):
 * retail loads the whole word and sign-extends with sll/sra; a bare `(short)word[1]` folds to
 * `lh 6(a1)` under 7.1 (true=400). The mask is the shaping device, disclosed.
 * Typedefs copied verbatim from the TU (func_80021B9C's block); func_80023B50's prototype and its
 * VoiceVariables_80023AD4 view as in w18c's func_80023B50 (same TU, earlier): the state pointer is
 * passed with a cast (type view only). Prior: D10 BT05-medium-next. */
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned int u32;
typedef struct MacroState { u8 unknown00[0x30]; u32 field30; u8 unknown34[0x2C]; u32 field60; } MacroState;
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
#pragma pack(1)
typedef struct VoiceVariables_80023AD4 { u8 unknown000[0x180]; s16 variables180[16]; } VoiceVariables_80023AD4;
#pragma pack(0)
extern void func_80023B50(VoiceVariables_80023AD4 *voice, u8 isController, u8 index, s16 value);

u8 func_80023D70(MacroState *state, MacroCommand *command)
{
    func_80023B50((VoiceVariables_80023AD4 *)state, command->word[0] >> 8, command->word[0] >> 16,
                  (s16)(command->word[1] & 0xFFFF));
    return 0;
}
