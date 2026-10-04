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
extern AudioNode D_800426B0[256];
void func_800171C0(void)
{
    AudioNode *previous;
    int i;
    previous = 0;
    D_80043EB0 = D_800426B0;
    for (i = 0; i < 256; i++) {
        D_800426B0[i].previous = previous;
        if (previous != 0) {
            previous->next = &D_800426B0[i];
        }
        previous = &D_800426B0[i];
    }
    previous->next = 0;
}
