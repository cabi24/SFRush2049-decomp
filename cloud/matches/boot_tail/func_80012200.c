/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct AudioNode {
    struct AudioNode *next;
    unsigned char unknown04[12];
    unsigned short countdown;
} AudioNode;
extern AudioNode *D_80038344;
void func_80012200(void)
{
    AudioNode *node;
    for (node = D_80038344; node != 0; node = node->next) {
        if (node->countdown != 0) {
            --node->countdown;
        }
    }
}
