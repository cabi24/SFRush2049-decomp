typedef signed char s8;
typedef unsigned int u32;
typedef int s32;
typedef struct Node {
    u32 pad0;
    struct Node *next;
    struct Node *first;
    u32 size;
    u32 pad1[1];
    s8 used;
} Node;
typedef struct Head { u32 pad0; u32 pad1; Node *first; } Head;
typedef struct { int x; } Q;
extern Q D_80152770;
extern Head *D_801527C8;
s32 osRecvMesg(Q *q, void *msg, s32 flags);
s32 osJamMesg(Q *q, void *msg, s32 flags);
u32 func_800E79F8(Head *h) {
    u32 max;
    Node *n;
    osRecvMesg(&D_80152770, 0, 1);
    max = 0;
    for (n = (h ? h : D_801527C8)->first; n != 0; n = n->next) {
        if (n->used == 0 && max < n->size) max = n->size;
    }
    osJamMesg(&D_80152770, 0, 0);
    return max;
}
