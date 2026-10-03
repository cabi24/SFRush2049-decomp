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
extern SequenceNode *func_8001EAA0(u32 key);
int func_8001EDF4(u32 key)
{
    SequenceNode *node;
    if (key != 0xFFFFFFFF) {
        node = func_8001EAA0(key);
        if (node != 0) return node->value;
    }
    return -1;
}
