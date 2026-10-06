typedef int s32;
typedef struct Node { struct Node *child; struct Node *next; } Node;
void camera_update_a(s32 arg0, Node *n, s32 *count, void (*fn)(Node *, s32)) {
    while (n != 0) {
        camera_update_a(arg0, n->child, count, fn);
        fn(n, *count);
        (*count)++;
        if (fn) { }
        n = n->next;
    }
}
