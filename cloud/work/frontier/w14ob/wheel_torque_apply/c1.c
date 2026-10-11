/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

typedef struct NameKey {
    u32 hdr;
    char name[20];
} NameKey;
typedef struct NameEntry {
    struct Node104 *node;
    char name[16];
} NameEntry;
typedef struct Node104 { u8 opaque[104]; } Node104;

extern s16 D_80149D90;
extern Node104 *D_80149B80;
extern u32 *D_801497FC;
extern NameEntry *D_80149818;
extern s32 pointer_offset_wrapper(const void *a, const void *b);
s32 func_800A473C(char *dst, u8 *src);
void differential_output(Node104 *node, s16 parent);
void *entity_name_copy(const void *key, const void *base, u32 n, u32 size,
                       s32 (*compar)(const void *, const void *));
void func_800AB638(void);

static void wheel_find_node(u8 *name, Node104 **out) {
    NameKey key;
    func_800A473C(key.name, name);
    *out = ((NameEntry *) entity_name_copy(&key, D_80149818, *D_801497FC, sizeof(NameEntry),
                                           pointer_offset_wrapper))->node;
}

s16 wheel_torque_apply(u8 *arg0, s16 arg1) {
    D_80149D90 = -1;
    wheel_find_node(arg0, &D_80149B80);
    differential_output(D_80149B80, arg1);
    func_800AB638();
    return D_80149D90;
}
