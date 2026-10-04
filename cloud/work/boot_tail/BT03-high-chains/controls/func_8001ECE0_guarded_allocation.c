/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned int u32;
#pragma pack(1)
typedef struct SequenceNode {
    struct SequenceNode *next;
    struct SequenceNode *previous;
    u32 key;
    u32 value;
} SequenceNode;
typedef struct VoicePrefix {
    u8 unknown00[24];
    SequenceNode *entry18;
    u8 unknown1C[68];
    u32 identifier60;
} VoicePrefix;
#pragma pack(0)
extern SequenceNode *D_80050C50, *D_80050C54;
extern u32 func_8001EAEC(void);
u32 func_8001ECE0(VoicePrefix *state)
{
    SequenceNode *current;
    SequenceNode *previous;
    SequenceNode *node;
    u32 key;
    key = func_8001EAEC();
    current = D_80050C50;
    previous = 0;
    while (current != 0) {
        if (key < current->key) break;
        if (key == current->key) key = func_8001EAEC();
        previous = current;
        current = current->next;
    }
    node = D_80050C54;
    if (node != 0) {
    D_80050C54 = node->next;
    if (D_80050C54 != 0) D_80050C54->previous = 0;
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
    return 0xFFFFFFFFU;
}
