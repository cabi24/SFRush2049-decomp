/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * camera_update_a -- recursive in-order walk of a child/next tree: for each node, visit its children
 * first, then call fn(node, *count) and bump the shared counter, then continue with the next sibling.
 * arg0 is only passed down. (The name is a historical label.)
 *
 * Shaping (w11a): written as plain double recursion. uopt turns the tail call on n->next into the loop
 * itself, which gives retail's colouring (count s0, n s1, entry test on s1 after the moves). The
 * hand-written while/for/do loops (w10f, 12 rows) colour n before count (n save 25.5 vs 20) instead.
 * Also MATCH at -O2.
 */
typedef int s32;
typedef struct Node { struct Node *child; struct Node *next; } Node;

void camera_update_a(s32 arg0, Node *n, s32 *count, void (*fn)(Node *, s32)) {
    if (n != 0) {
        camera_update_a(arg0, n->child, count, fn);
        fn(n, *count);
        (*count)++;
        camera_update_a(arg0, n->next, count, fn);
    }
}
