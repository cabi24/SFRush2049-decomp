typedef int s32;
typedef struct Node { struct Node *child; struct Node *next; } Node;
void camera_update_a(s32 arg0, Node *n, s32 *count, void (*fn)(Node *, s32)) {
    s32 *c = count;
    while (n != 0) {
        camera_update_a(arg0, n->child, c, fn);
        fn(n, *c);
        (*c)++;
        n = n->next;
    }
}
