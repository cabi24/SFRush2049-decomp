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
extern VoiceState D_8004BEB8[];
extern SequenceNode *D_80050C50, *D_80050C54;
extern void func_80021844(VoiceState *);
void func_8001EB10(VoiceState *state)
{
    func_80021844(state);
    if (state->identifier60 != 0xFFFFFFFFU) {
        if (state->parent_identifier != 0xFFFFFFFFU) {
            D_8004BEB8[(state->parent_identifier & 255)].next_identifier=state->next_identifier;
            if (state->next_identifier != 0xFFFFFFFFU) {
                D_8004BEB8[(state->next_identifier & 255)].parent_identifier=state->parent_identifier;
            }
        } else if (state->next_identifier != 0xFFFFFFFFU) {
            state->entry18->value=state->next_identifier;
            D_8004BEB8[(state->next_identifier & 255)].parent_identifier=0xFFFFFFFFU;
            D_8004BEB8[(state->next_identifier & 255)].entry18=state->entry18;
        } else {
            if (state->entry18->previous != 0) {
                state->entry18->previous->next=state->entry18->next;
            } else {
                D_80050C50=state->entry18->next;
            }
            if (state->entry18->next != 0) {
                state->entry18->next->previous=state->entry18->previous;
            }
            state->entry18->next=D_80050C54;
            if (D_80050C54 != 0) D_80050C54->previous=state->entry18;
            state->entry18->previous=0;
            D_80050C54=state->entry18;
        }
    }
}
