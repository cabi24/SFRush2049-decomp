typedef struct Node { struct Node *left; struct Node *right; } Node;
typedef struct { s32 (*cmp)(Node *, Node *); s32 count; } Tree;
void camera_update_b(Tree *t, Node **pp, Node *n) {
    while (1) {
        if (*pp == 0) {
            *pp = n;
            n->left = 0;
            (*pp)->right = 0;
            t->count++;
            return;
        }
        if (t->cmp(n, *pp) < 0) {
            pp = &(*pp)->left;
        } else {
            pp = &(*pp)->right;
        }
    }
}
