/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80023DB8 - lib_22300 (IDO 7.1). Macro "compare and branch": read two macro variables
 * (func_80023AD4: controller flag + index from word0 bytes 1/2 and 3/word1 byte 0), compare them
 * (COMPARISON 0: ==, 1: <), invert the result when word1 byte 1 is nonzero, and on true jump the
 * voice's program counter to program + (word1 >> 16) commands. Returns 0 (continue).
 * w18c: (1) the second index is passed as `word1 & 0xFF` (retail loads the word and narrows it in
 * a2; the bare u32 argument folds to `lbu 7` under 7.1: shaping, a mask, disclosed); (2) the jump
 * target is written `index + program` (retail computes the index first; `program + index` loads
 * program first, true=40). COMPARISON values other than 0/1 leave CONDITION unset, as in D10
 * (the dispatcher only passes 0 and 1).
 * Types: private packed program view (MacroVoice_80023DB8: program, current); MacroCommand is the
 * TU's typedef; func_80023AD4's prototype and VoiceVariables_80023AD4 view as w18c's
 * func_80023AD4 (same TU, earlier): the voice is passed with a cast (type view only).
 * Prior: D10 BT05-macro-five (5.3, 49 words; 7.1 true=350). */
typedef unsigned char u8;
typedef short s16;
typedef unsigned int u32;
typedef struct MacroCommand { u32 word[2]; } MacroCommand;
#pragma pack(1)
typedef struct VoiceVariables_80023AD4 { u8 unknown000[0x180]; s16 variables180[16]; } VoiceVariables_80023AD4;
typedef struct MacroVoice_80023DB8 { MacroCommand *program; MacroCommand *current; } MacroVoice_80023DB8;
#pragma pack(0)
extern s16 func_80023AD4(VoiceVariables_80023AD4 *voice, u8 isController, u8 index);

u8 func_80023DB8(MacroVoice_80023DB8 *voice, MacroCommand *command, u8 comparison)
{
    s16 left;
    s16 right;
    u8 condition;
    left = func_80023AD4((VoiceVariables_80023AD4 *)voice, command->word[0] >> 8, command->word[0] >> 16);
    right = func_80023AD4((VoiceVariables_80023AD4 *)voice, command->word[0] >> 24, command->word[1] & 0xFF);
    switch (comparison) {
    case 0: condition = left == right; break;
    case 1: condition = left < right; break;
    }
    if ((command->word[1] >> 8) & 255) {
        condition = !condition;
    }
    if (condition) {
        voice->current = ((command->word[1] >> 16) & 65535) + voice->program;
    }
    return 0;
}
