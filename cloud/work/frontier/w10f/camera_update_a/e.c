typedef int s32;
typedef struct Node { struct Node *child; struct Node *next; } Node;
void camera_update_a(s32 arg0, Node *n, s32 *count, void (*fn)(Node *, s32)) {
    if (n == 0) return;
    do {
        camera_update_a(arg0, n->child, count, fn);
        fn(n, *count);
        (*count)++;
        n = n->next;
    } while (n != 0);
}
