/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

typedef struct NameEntry {
    char name[36];
} NameEntry;

typedef struct NameTable {
    NameEntry *base;
    u32 count;
} NameTable;

extern volatile u8 D_80140BDC;
extern NameTable D_80151AE8[];
extern char *func_800A473C(char *dst, char *src);
extern s8 func_80092D80(s32 arg0);
extern void *entity_name_copy(void *key, void *base, u32 n, u32 size, s32 (*compar)(void *, void *));
extern s32 pointer_compare_thunk(void *, void *);

NameEntry *func_800B24EC(char *name, s16 *out, s8 lo, s8 hi) {
    NameEntry *found;
    char buf[36];
    s32 i;

    found = 0;
    func_800A473C(buf, name);
    if (lo < 0) {
        lo = 0;
    }
    if (hi < 0 || hi >= D_80140BDC) {
        hi = D_80140BDC - 1;
    }
    for (i = lo; i <= hi; i++) {
        if (!func_80092D80(i)) {
            continue;
        }
        found = entity_name_copy(buf, D_80151AE8[i].base, D_80151AE8[i].count, 36, pointer_compare_thunk);
        if (found != 0) {
            break;
        }
    }
    if (found == 0) {
        *out = 0;
        return 0;
    }
    *out = (found - D_80151AE8[i].base) | (i << 10);
    return found;
}
