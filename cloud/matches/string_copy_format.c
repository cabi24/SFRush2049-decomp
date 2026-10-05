/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * string_copy_format (historical label): look an 88-byte record up by name across a range of sorted
 * tables and return its packed handle (index | table << 10), or -1.
 * The name is copied into a local key with func_80092DCC (strncpy, n = 16), or "AAANULLOBJ" when the
 * name is NULL/empty (strcpy intrinsic: retail's lwl/lwr copy of the literal at 0x80120E68). Then the
 * same clamp/loop/bsearch as the matched sibling func_800B24EC: D_801161F4[i] = { base, count },
 * element size 88, entity_name_copy = bsearch, validate_and_call = comparator.
 *
 * STRICT MATCH (frontier wave 3, w3a). Shaping quirk: the fourth parameter `complain` (callers pass 1)
 * is read only by a compiled-out diagnostic in the not-found branch (`if (complain) DEBUG_PRINT(...);`
 * with an empty macro). That read keeps its a3 web alive from entry, so uopt cannot colour the copy
 * of `name` into a3 and picks t0 (retail `move t0,a0`); it also explains the a3 home store that is
 * never reloaded. Without it `name` lands in a3 (36 words). Found with the instrumented uopt
 * globalcolor trace (forbidden mask of the name web lacked a3). Matches in the whole-program unit too.
 */
#define DEBUG_PRINT(args)
typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

extern char *strcpy(char *, const char *);
#pragma intrinsic (strcpy)

typedef struct Rec {
    char name[88];
} Rec;

typedef struct RecTable {
    Rec *base;
    u32 count;
} RecTable;

extern RecTable D_801161F4[];
extern volatile u8 D_80140BDC;

s8 func_80092D80(s32 arg0);
char *func_80092DCC(char *dst, char *src, s32 n);
void *entity_name_copy(void *key, void *base, u32 n, u32 size, s32 (*compar)(void *, void *));
s32 validate_and_call(void *, void *);

s32 string_copy_format(char *name, s8 first, s8 last, s8 complain) {
    s32 i;
    Rec key;
    Rec *found;

    found = 0;
    if (name == 0 || *name == 0) {
        strcpy(key.name, "AAANULLOBJ");
    } else {
        func_80092DCC(key.name, name, 16);
    }
    if (first < 0) {
        first = 0;
    }
    if (last < 0 || last >= D_80140BDC) {
        last = D_80140BDC - 1;
    }
    for (i = first; i <= last; i++) {
        if (func_80092D80(i) == 0) {
            continue;
        }
        found = entity_name_copy(&key, D_801161F4[i].base, D_801161F4[i].count, sizeof(Rec), validate_and_call);
        if (found != 0) {
            break;
        }
    }
    if (found == 0) {
        if (complain) DEBUG_PRINT(("string_copy_format: %s not found\n", key.name));
        return -1;
    }
    return (found - D_801161F4[i].base) | (i << 10);
}
