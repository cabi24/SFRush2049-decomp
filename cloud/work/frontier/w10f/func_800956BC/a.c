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
extern List D_80146160;
Node *func_800956BC(s32 key) {
    Node *node;
    for (node = D_80146160.head; node != 0; node = node->next) {
        if (node->key == key && node->kind == 3) return node;
    }
    return 0;
}
