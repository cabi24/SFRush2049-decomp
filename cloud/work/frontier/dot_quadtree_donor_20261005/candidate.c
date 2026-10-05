/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Reconstructed N64 downleaf. Arcade algorithm ancestry: rushtherock
 * game/stree.c at 845329d7b36f5a384c5625ed9a0aef584ab46139.
 * N64 bounds are signed halfwords, midpoint division truncates toward zero,
 * child_mask selects links, and link zero returns NULL without an output store.
 * Genuine midpoint sums are consumed by quadrant selection; no scratch padding. */
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
    s16 q;

    for (;;) {
        mid_x = node->min_x + node->max_x;
        mid_y = node->min_y + node->max_y;
        q = (x < mid_x / 2) ? 0 : 1;
        if (y < mid_y / 2)
            q += 2;
        if (node->child_mask & (1 << q)) {
            node = &D_80124EEC[node->child[q]];
            if (node == D_80124EEC)
                return 0;
            continue;
        } else {
            *quadrant = q;
            return node;
        }
    }
}
