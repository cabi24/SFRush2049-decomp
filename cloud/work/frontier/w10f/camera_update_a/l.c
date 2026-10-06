typedef int s32;
typedef struct Node { struct Node *child; struct Node *next; } Node;
void camera_update_a(s32 arg0, Node *n, s32 *count, void (*fn)(Node *, s32)) {
    Node *p;
    if (n == 0) return;
    p = n;
    do {
        camera_update_a(arg0, p->child, count, fn);
        fn(p, *count);
        (*count)++;
    } while ((p = p->next) != 0);
}
