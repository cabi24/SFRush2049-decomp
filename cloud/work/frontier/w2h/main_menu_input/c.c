/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed int s32;
typedef float f32;

typedef struct Node {
    /* 0x00 */ s32 unk0[3];
    /* 0x0C */ f32 a[3];
    /* 0x18 */ f32 b[3];
    /* 0x24 */ f32 c[3];
    /* 0x30 */ f32 d[3];
} Node;

typedef struct OSMesgQueue OSMesgQueue;
extern OSMesgQueue D_80142728;
s32 osRecvMesg(OSMesgQueue *mq, void *msg, s32 flag);
s32 osJamMesg(OSMesgQueue *mq, void *msg, s32 flag);
void func_800D52CC(Node *node) {
    Node *p; p = node;
}

void main_menu_input(Node *node, f32 *a, f32 *b, f32 *c, f32 *d) {
    if (node != (Node *) -1) {
        osRecvMesg(&D_80142728, 0, 1);
        func_800D52CC(node);
        node->a[0] = a[0];
        node->a[1] = a[1];
        node->a[2] = a[2];
        node->b[0] = b[0];
        node->b[1] = b[1];
        node->b[2] = b[2];
        node->c[0] = c[0];
        node->c[1] = c[1];
        node->c[2] = c[2];
        node->d[0] = d[0];
        node->d[1] = d[1];
        node->d[2] = d[2];
        osJamMesg(&D_80142728, 0, 0);
    }
}
