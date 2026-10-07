/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
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

extern Node *D_801392C8;
extern s16 D_8012E66C;
extern s16 D_8012E678;

Node *func_80090284(void)
{
    Node *node;

    if (!D_801392C8) return 0;
    node = D_801392C8;
    D_801392C8 = node->next;
    D_8012E66C++;
    if (D_8012E66C > D_8012E678) D_8012E678 = D_8012E66C;
    node->next = 0;
    node->field4 = 0;
    node->field6 = -1;
    node->field8 = 0;
    node->fieldC = 0;
    node->field14 = 0;
    node->field10 = 0.0f;
    return node;
}

/*
 * Pop and initialize a 24-byte free-list node, maintaining the signed-half
 * allocation and high-water counters. The empty list returns null.
 * Arcade equivalent: not established; the arcade checkout is unavailable.
 * N64 adaptation: O32 layout and halfword counter narrowing are target-specific.
 * The source reproduces all 132 target bytes at the flags above. Field names
 * other than next remain neutral; original source spellings are not claimed.
 * Statement order (unlink before increment) is required for matching.
 * This is cloud matching evidence only; image and full-ROM gates remain pending.
 */
