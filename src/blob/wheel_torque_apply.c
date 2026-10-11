/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * wheel_torque_apply (historical label; 0x800AC668, 140 bytes): look a node up by name and place it.
 * Clears the "first registered id" D_80149D90 to -1, copies the name into a search key (strcpy-style
 * func_800A473C), bsearches (entity_name_copy) the 20-byte name table D_80149818 (count *D_801497FC,
 * compared by pointer_offset_wrapper on the names at +4), stores the entry's node pointer in the pool
 * pointer D_80149B80, runs differential_output(D_80149B80, parent) (the recursive node placement), then
 * func_800AB638 (table pointer rebuild) and returns D_80149D90. No arcade ancestor identified.
 *
 * w14ob: the lookup is a static helper that umerge inlines (no stub is emitted for it):
 *  - its key local lives in the inlined-call area (24-byte key + the name parameter = 32 bytes, frame 64);
 *  - its statements carry the helper's (earlier) source lines, so as1 schedules the key address
 *    `addiu a0,sp,36` before the caller's `sh` to D_80149D90 and the sh fills the jal delay slot;
 *  - the store goes through the out parameter (*out = ...), so uopt keeps it a separate address from the
 *    caller's D_80149B80 load: no shared `la`, no store-to-load forwarding (retail's lui at / lui a0 pair).
 * No shaping constructs (no volatile, no compiled-out reads, no unused locals).
 */
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

typedef struct Node104 { u8 opaque[104]; } Node104;
typedef struct NameKey {
    u32 hdr;
    char name[20];
} NameKey;
typedef struct NameEntry {
    Node104 *node;
    char name[16];
} NameEntry;

extern s16 D_80149D90;
extern Node104 *D_80149B80;
extern u32 *D_801497FC;
extern NameEntry *D_80149818;
extern void pointer_offset_wrapper(s32 arg0, s32 arg1);
extern u8 *func_800A473C(u8 *arg0, u8 *arg1);
extern void *entity_name_copy(void *key, void *base, u32 n, u32 size, s32 (*compar)(void *, void *));
extern void differential_output(Node104 *node, s16 parent);
extern void func_800AB638(void);

static void wheel_find_node(u8 *name, Node104 **out) {
    NameKey key;
    func_800A473C((u8 *) key.name, name);
    *out = ((NameEntry *) entity_name_copy(&key, D_80149818, *D_801497FC, sizeof(NameEntry),
                                           (s32 (*)(void *, void *)) pointer_offset_wrapper))->node;
}

s16 wheel_torque_apply(u8 *arg0, s16 arg1) {
    D_80149D90 = -1;
    wheel_find_node(arg0, &D_80149B80);
    differential_output(D_80149B80, arg1);
    func_800AB638();
    return D_80149D90;
}
