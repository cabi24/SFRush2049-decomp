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
typedef struct Node104 { u8 b[104]; } Node104;

extern s16 D_80149D90;
extern /*@{vo*/volatile /*@|@}*/s32 D_80149B80;
extern s32 D_801497FC;
extern s32 D_80149818;
extern void pointer_offset_wrapper(s32 arg0, s32 arg1);
s32 func_800A473C(s32 arg0, u8 *arg1);
void differential_output(s32 arg0, s16 arg1);
void *entity_name_copy(void *key, void *base, u32 n, u32 size, s32 (*compar)(void *, void *));
void func_800AB638(void);

static void wheel_find_node(u8 *name, /*@{pv*/volatile /*@|@}*/s32 *out) {
    NameKey key;
    func_800A473C((s32) key.name, name);
    *out = *(s32 *) entity_name_copy(&key, (void *) D_80149818, *(u32 *) D_801497FC, 20,
                                           (s32 (*)(void *, void *)) pointer_offset_wrapper);
}

s16 wheel_torque_apply(u8 *arg0, s16 arg1) {
    D_80149D90 = -1;
    wheel_find_node(arg0, /*@{c*//*@| (s32 *) @}*/&D_80149B80);
    differential_output(D_80149B80, arg1);
    func_800AB638();
    return D_80149D90;
}
