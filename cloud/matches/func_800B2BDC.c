/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Initialize the 100-node free list and clear the active head and allocation count.
 * Native layout and ownership are supported by the initializer and the real
 * func_80090284 consumer. No whole-function arcade donor is established.
 * Defining the actual pool array allows IDO to combine its symbol references;
 * the indexed traversal and next-pointer-before-id order follow native effects.
 * BSS address/extent and compatible consumer context are checked separately.
 */
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned int u32;
typedef float f32;

typedef struct Node {
    struct Node *next;
    s16 field4;
    s16 field6;
    s16 field8;
    u8 padA[2];
    void *fieldC;
    f32 field10;
    u32 field14;
} Node;

Node D_80138880[100];
extern Node *D_801392C8;
extern Node *D_801391F0;
extern s16 D_8012E66C;

void func_800B2BDC(void)
{
    int i;

    D_801392C8 = D_80138880;
    for (i = 0; i < 99; i++) {
        D_80138880[i].next = &D_80138880[i + 1];
        D_80138880[i].field6 = -1;
    }
    D_80138880[i].next = 0;
    D_80138880[i].field6 = -1;
    D_801391F0 = 0;
    D_8012E66C = 0;
}
