/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Macro variable arithmetic; native ABI and source-family evidence in README.
 * w20c: declarations aligned with lib_22300 (verbatim TU lines: the packed VoiceVariables_80023AD4
 * view of func_80023AD4/func_80023B50, MacroCommand { u32 word[2]; }). Type views only. */
typedef unsigned char u8;
typedef short s16;
typedef unsigned int u32;
typedef int s32;
#pragma pack(1)
typedef struct VoiceVariables_80023AD4 { u8 unknown000[0x180]; s16 variables180[16]; } VoiceVariables_80023AD4;
#pragma pack(0)
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
extern s16 func_80023AD4(VoiceVariables_80023AD4 *voice, u8 isController, u8 index);
extern void func_80023B50(VoiceVariables_80023AD4 *voice, u8 isController, u8 index, s16 value);

/* The native dispatcher's five call sites supply operation 0 through 4.
 * Other byte values read an uninitialized native result and are outside the
 * defined command contract. Do not invent a default arithmetic result. */
u8 func_80023BDC(VoiceVariables_80023AD4 *voice, MacroCommand *command, u8 operation)
{
    s16 left;
    s16 right;
    s32 result;

    left = func_80023AD4(voice, (command->word[0] >> 24) & 255, command->word[1] & 255);
    if (operation == 4) {
        right = (s16)(command->word[1] >> 8);
    } else {
        right = func_80023AD4(voice, (command->word[1] >> 8) & 255, (command->word[1] >> 16) & 255);
    }
    switch (operation) {
    case 4:
    case 0: result = left + right; break;
    case 1: result = left - right; break;
    case 2: result = left * right; break;
    case 3: result = right != 0 ? left / right : 0; break;
    }
    func_80023B50(voice, (command->word[0] >> 8) & 255, (command->word[0] >> 16) & 255,
                 result < -32768 ? -32768 : result > 32767 ? 32767 : result);
    return 0;
}
