/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* N64 downleaf adaptation of rushtherock game/stree.c. */
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned char u8;
typedef struct QNode {
    s16 parent;
    u8 unknown02;
    u8 child_mask;
    s16 min_x, max_x, min_y, max_y;
    u16 child[4];
} QNode;
extern QNode *D_80124EEC;

QNode *func_800AC9BC(QNode *node, s16 x, s16 y, s16 *quadrant)
{
    s32 mid_x, mid_y;
    s32 child;
    s16 q;

    mid_x = (node->min_x + node->max_x) / 2;
    mid_y = (node->min_y + node->max_y) / 2;
    q = 1;
    if (x < mid_x)
        q = 0;
    if (y < mid_y)
        q += 2;
    if (node->child_mask & (1 << q)) {
        child = node->child[q];
        node = &D_80124EEC[child];
        if (node == D_80124EEC)
            return 0;
        return func_800AC9BC(node, x, y, quadrant);
    } else {
        *quadrant = q;
        return node;
    }
}
