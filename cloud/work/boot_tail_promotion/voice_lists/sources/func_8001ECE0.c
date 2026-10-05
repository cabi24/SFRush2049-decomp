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
extern SequenceNode *D_80050C50, *D_80050C54;
extern u32 func_8001EAEC(void);
u32 func_8001ECE0(VoiceState *state)
{
    SequenceNode *current;
    SequenceNode *previous;
    SequenceNode *node;
    u32 key;
    key = func_8001EAEC();
    previous = 0;
    for (current = D_80050C50; current != 0; current = current->next) {
        if (key < current->key) break;
        if (key == current->key) key = func_8001EAEC();
        previous = current;
    }
    node = D_80050C54;
    if (node == 0) return 0xFFFFFFFFU;
    if ((D_80050C54 = D_80050C54->next) != 0)
        D_80050C54->previous = 0;
    if (previous == 0) D_80050C50 = node;
    else previous->next = node;
    node->previous = previous;
    node->next = current;
    if (current != 0) current->previous = node;
    node->key = key;
    node->value = state->identifier60;
    state->entry18 = node;
    return key;
}
