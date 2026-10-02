/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct Link {struct Link *next, *prev; u8 enabled;} Link;
typedef struct List {u8 indirect, doubly, pad[2]; u32 count; Link *head, *tail;} List;
extern void func_8009211c(List *list, Link *object);
Link *object_init_cleared(List *list, Link *object) {
    if (!object) return 0;
    func_8009211c(list, object);
    object->enabled = 0;
    return object;
}
