/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct AudioNode {
    struct AudioNode *next;
    struct AudioNode *previous;
    unsigned char unknown08[16];
} AudioNode;
typedef struct AudioState {
    unsigned char unknown00[3960];
    AudioNode *active;
    AudioNode *pending;
} AudioState;
extern AudioNode *D_80043EB0;
extern AudioState *D_8004BE80;
AudioNode *func_800173B4(void)
{
    AudioNode *node;
    AudioNode *head;
    node = D_80043EB0;
    if (node != 0) {
        D_80043EB0 = node->next;
        if (D_80043EB0 != 0) {
            D_80043EB0->previous = 0;
        }
        node->previous = 0;
        head = D_8004BE80->active;
        node->next = head;
        if (head != 0) {
            D_8004BE80->active->previous = node;
        }
        D_8004BE80->active = node;
    }
    return node;
}
