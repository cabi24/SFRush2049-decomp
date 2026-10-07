/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800956BC: walk the singly linked node list whose head is word 3 of
 * D_80146160 and return the first node with key == `key` and kind byte == 3,
 * or NULL.  Also matches at -O2.
 * Shaping quirk: the list header is read through `volatile` -- retail builds
 * the full address (lui/addiu) and loads head with offset 12, which only a
 * volatile (address-form) read produces here.
 */
typedef unsigned char u8;
typedef int s32;
typedef struct Node {
    s32 word0;
    struct Node *next;
    u8 kind;
    u8 pad[3];
    s32 key;
} Node;
typedef struct { s32 words[3]; Node *head; } List;
extern volatile List D_80146160;
Node *func_800956BC(s32 key) {
    Node *node;
    for (node = (Node *)D_80146160.head; node != 0; node = node->next) {
        if (node->key == key && node->kind == 3) return node;
    }
    return 0;
}
