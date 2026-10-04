/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Native state-service reconstruction; actual input and data accesses audited. */
extern unsigned char D_8002C630;
extern void func_80014594(void);
extern void func_800145DC(void);
typedef struct LinkNode {
    struct LinkNode *next;
    struct LinkNode *previous;
} LinkNode;
extern LinkNode *D_8004FD54;
int func_8001D8B0(LinkNode *node)
{
    if (D_8002C630) {
        func_80014594();
        if (node->next != 0) node->next->previous = node->previous;
        if (node->previous != 0) node->previous->next = node->next;
        else D_8004FD54 = node->next;
        func_800145DC();
        return 1;
    }
    return 0;
}
