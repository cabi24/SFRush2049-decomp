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
void func_80017470(AudioNode *node)
{
    if (node->next != 0) {
        node->next->previous = node->previous;
    }
    if (node->previous != 0) {
        node->previous->next = node->next;
    } else {
        D_8004BE80->pending = node->next;
    }
    node->next = D_80043EB0;
    if (D_80043EB0 != 0) {
        D_80043EB0->previous = node;
    }
    node->previous = 0;
    D_80043EB0 = node;
}
