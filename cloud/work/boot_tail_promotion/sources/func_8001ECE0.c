/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_8001ECE0.c: file-local type names SequenceNode, VoicePrefix suffixed _8001ECE0 so several bodies share one ROM TU; no other change. */
typedef unsigned char u8;
typedef unsigned int u32;
#pragma pack(1)
typedef struct SequenceNode_8001ECE0 {
    struct SequenceNode_8001ECE0 *next;
    struct SequenceNode_8001ECE0 *previous;
    u32 key;
    u32 value;
} SequenceNode_8001ECE0;
typedef struct VoicePrefix_8001ECE0 {
    u8 unknown00[24];
    SequenceNode_8001ECE0 *entry18;
    u8 unknown1C[68];
    u32 identifier60;
} VoicePrefix_8001ECE0;
#pragma pack(0)
extern SequenceNode_8001ECE0 *D_80050C50, *D_80050C54;
extern u32 func_8001EAEC(void);
u32 func_8001ECE0(VoicePrefix_8001ECE0 *state)
{
    SequenceNode_8001ECE0 *current;
    SequenceNode_8001ECE0 *previous;
    SequenceNode_8001ECE0 *node;
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
