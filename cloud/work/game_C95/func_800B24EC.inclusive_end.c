/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;
typedef struct NameRecord { char name[32]; u32 offset; } NameRecord;
typedef struct Directory { NameRecord *records; u32 count; } Directory;
extern u8 D_80140BDC;
extern Directory D_80151AE8[];
extern char *func_800A473C(char *, const char *);
extern s32 func_80092D80(s32);
extern s32 pointer_compare_thunk(const void *, const void *);
extern void *entity_name_copy(const void *, const void *, u32, u32, s32 (*)(const void *, const void *));
NameRecord *func_800B24EC(const char *name, u16 *key, s8 first, s8 last) {
    char query[40];
    s32 index;
    NameRecord *result = 0;
    func_800A473C(query, name);
    if (first < 0) first = 0;
    if (last < 0 || last >= D_80140BDC) last = D_80140BDC - 1;
    for (index = first; index < last + 1; index++) {
        if (!func_80092D80(index)) continue;
        result = entity_name_copy(query, D_80151AE8[index].records, D_80151AE8[index].count, sizeof(NameRecord), pointer_compare_thunk);
        if (result) break;
    }
    if (!result) {
        *key = 0;
        return 0;
    }
    *key = (result - D_80151AE8[index].records) | (index << 10);
    return result;
}
