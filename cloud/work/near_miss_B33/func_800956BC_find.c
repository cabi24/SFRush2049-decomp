/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;
typedef int s32;
typedef struct Node {
    s32 word0;
    struct Node *next;
    u8 kind;
    u8 pad[3];
    s32 key;
} Node;
extern struct {s32 words[3];Node *head;} D_80146160;
Node *func_800956BC(s32 key) {
    Node *node=D_80146160.head;
    while(node!=0) {
        if(node->key==key && node->kind==3) return node;
        node=node->next;
    }
    return 0;
}
