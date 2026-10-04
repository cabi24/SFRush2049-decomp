/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Macro variable arithmetic; native ABI and source-family evidence in README. */
typedef unsigned char u8;
typedef short s16;
typedef unsigned int u32;
typedef int s32;
typedef struct VoiceState VoiceState;
typedef struct MacroCommand { u32 word0; u32 word1; } MacroCommand;
extern s16 func_80023AD4(VoiceState *voice, u8 controller, u8 index);
extern void func_80023B50(VoiceState *voice, u8 controller, u8 index, s16 value);

/* The native dispatcher's five call sites supply operation 0 through 4.
 * Other byte values read an uninitialized native result and are outside the
 * defined command contract. Do not invent a default arithmetic result. */
u8 func_80023BDC(VoiceState *voice, MacroCommand *command, u8 operation)
{
    s16 left;
    s16 right;
    s32 result;

    left = func_80023AD4(voice, command->word0 >> 24, command->word1);
    if (operation == 4) {
        right = command->word1 >> 8;
    } else {
        right = func_80023AD4(voice, command->word1 >> 8, command->word1 >> 16);
    }
    switch (operation) {
    case 4:
    case 0: result = left + right; break;
    case 1: result = left - right; break;
    case 2: result = left * right; break;
    case 3: result = right != 0 ? left / right : 0; break;
    }
    func_80023B50(voice, command->word0 >> 8, command->word0 >> 16,
                 result < -32768 ? -32768 : result > 32767 ? 32767 : result);
    return 0;
}
