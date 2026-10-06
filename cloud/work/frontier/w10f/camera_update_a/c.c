typedef int s32;
typedef struct Node { struct Node *child; struct Node *next; } Node;
void camera_update_a(s32 arg0, Node *n, s32 *count, void (*fn)(Node *, s32)) {
    Node *p;
    for (p = n; p != 0; p = p->next) {
        camera_update_a(arg0, p->child, count, fn);
        fn(p, *count);
        (*count)++;
    }
}
