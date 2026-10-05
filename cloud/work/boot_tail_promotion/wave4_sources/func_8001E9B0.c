/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Adapted from cloud/matches/boot_tail/func_8001E9B0.c to the production
 * src/rom/lib_1f5b0.c SequenceNode record (hoisted in wave 4); code unchanged. */
typedef unsigned int u32;
#pragma pack(1)
typedef struct SequenceNode { struct SequenceNode *next; struct SequenceNode *previous; u32 key; int value; } SequenceNode;
#pragma pack(0)
extern unsigned int D_80050A4C;
extern SequenceNode D_80050A50[32];
extern SequenceNode *D_80050C50;
extern SequenceNode *D_80050C54;
void func_8001E9B0(void)
{
    SequenceNode *previous;
    int i;
    D_80050A4C = 0;
    D_80050C50 = 0;
    D_80050C54 = D_80050A50;
    previous = 0;
    for (i = 0; i < 32; i++) {
        D_80050A50[i].previous = previous;
        if (previous != 0) previous->next = &D_80050A50[i];
        previous = &D_80050A50[i];
    }
    previous->next = 0;
}
