/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native packed sorted-list lookup; original full object types are unknown. */
typedef unsigned int u32;
#pragma pack(1)
typedef struct SequenceNode {
    struct SequenceNode *next;
    u32 unknown04;
    u32 key;
    int value;
} SequenceNode;
#pragma pack(0)
extern SequenceNode *D_80050C50;
SequenceNode *func_8001EAA0(u32 key)
{
    SequenceNode *node;
    node = D_80050C50;
    while (node != 0) {
        if (key == node->key) return node;
        if (key < node->key) break;
        node = node->next;
    }
    return 0;
}
