typedef int s32;
typedef struct Node { struct Node *child; struct Node *next; } Node;
void camera_update_a(s32 arg0, Node *n, s32 *count, void (*fn)(Node *, s32)) {
    Node *child;
    while (n != 0) {
        child = n->child;
        camera_update_a(arg0, child, count, fn);
        fn(n, *count);
        (*count)++;
        n = n->next;
    }
}
