/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Promotion adaptation: use lib_1f5b0.c's live voice and sequence-node
 * contracts. Node+4 is previous; node value/lookup result is signed int. */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char s8;
#pragma pack(1)
typedef struct SequenceNode { struct SequenceNode *next; struct SequenceNode *previous; u32 key; int value; } SequenceNode;
typedef struct VoiceState { u32 command00; u8 unknown04[12]; u32 next_identifier; u32 parent_identifier; SequenceNode *entry18; u8 unknown1C[8]; u32 flags24; u32 value28; u8 unknown2C[32]; u8 external4C; u8 unknown4D[19]; u32 identifier60; u8 unknown64[89]; u8 activeBD; u8 unknownBE[226]; } VoiceState;
#pragma pack(0)
extern void func_8001EB10(VoiceState *);
extern void func_8001F6EC(VoiceState *);
void func_8001F9D0(VoiceState *state)
{
    func_8001EB10(state);
    state->flags24 &= ~3U;
    state->value28 = 0;
    func_8001F6EC(state);
}
