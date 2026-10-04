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
void func_8001729C(AudioState *state)
{
    AudioNode *node;
    if (state->active != 0) {
        node = state->active;
        while (node->next != 0) {
            node = node->next;
        }
        if (D_80043EB0 != 0) {
            node->next = D_80043EB0;
            D_80043EB0->previous = node;
        }
        D_80043EB0 = state->active;
        state->active = 0;
    }
    if (state->pending != 0) {
        node = state->pending;
        while (node->next != 0) {
            node = node->next;
        }
        if (D_80043EB0 != 0) {
            node->next = D_80043EB0;
            D_80043EB0->previous = node;
        }
        D_80043EB0 = state->pending;
        state->pending = 0;
    }
}
