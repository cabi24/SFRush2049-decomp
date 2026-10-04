/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef struct AudioBufferNode {
    struct AudioBufferNode *next;
    struct AudioBufferNode *previous;
    unsigned char *buffer;
    unsigned int unknown0C;
    unsigned int unknown10;
} AudioBufferNode;
extern void *(*D_80038018)(unsigned int, unsigned int);
extern void func_800084E0(void *, int);
extern AudioBufferNode *D_80038338;
extern unsigned char *D_8003833C;
extern unsigned int D_80038340;
extern AudioBufferNode *D_80038344;
extern AudioBufferNode *D_80038348;
void func_80011F60(int count)
{
    int i;
    int bytes;
    D_80038338 = D_80038018(count * sizeof(AudioBufferNode), 0);
    bytes = count * 256;
    D_8003833C = D_80038018(bytes, 0);
    func_800084E0(D_8003833C, bytes);
    D_80038344 = 0;
    D_80038348 = D_80038338;
    D_80038338[0].buffer = D_8003833C;
    D_80038338[0].previous = 0;
    for (i = 1; i != count; i++) {
        D_80038338[i].previous = &D_80038338[i - 1];
        D_80038338[i - 1].next = &D_80038338[i];
        D_80038338[i].buffer = &D_8003833C[i * 256];
    }
    D_80038338[i - 1].next = 0;
    D_80038340 = 0;
}
