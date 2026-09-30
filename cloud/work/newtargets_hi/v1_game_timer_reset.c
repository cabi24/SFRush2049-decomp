typedef signed short s16;
typedef signed int s32;
typedef struct Node {
    char pad[14];
    s16 x;
    s16 y;
    char pad2[0x2A];
    struct Node *next;
} Node;
void func_80094EC8(Node *n);
void game_timer_reset(Node *n, s16 dx, s16 dy) {
    s16 a = dx;
    s16 b = dy;
    while (n != 0) {
        n->x += a;
        n->y += b;
        func_80094EC8(n);
        n = n->next;
    }
}
