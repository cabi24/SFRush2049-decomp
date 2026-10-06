typedef unsigned char u8;

typedef struct Node {
    int pad0, pad4, pad8;
    void *data;     /* 12 */
} Node;

typedef struct Pool {
    u8 flag;
    int count;
    int size;
    void *mem;
    Node *head;     /* 16 */
} Pool;

extern Pool D_80155220;
extern char D_80155B30[];
extern char D_80155290[];
void struct_fields_init(Pool *pool, void *mem, int size, int count, u8 flag);
void func_8008D0C0(void *data);
void func_800AFA84(Pool *pool, Node *node);
void *memset(void *, int, unsigned int);

void func_800B0580(void)
{
    Node *n;

    while (D_80155220.head != 0) {
        n = D_80155220.head;
        func_8008D0C0(n->data);
        func_800AFA84(&D_80155220, n);
    }
    struct_fields_init(&D_80155220, D_80155B30, 36, 100, 1);
    memset(D_80155290, 0, 2208);
}
