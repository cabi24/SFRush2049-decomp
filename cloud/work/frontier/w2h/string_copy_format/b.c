/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
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

s32 string_copy_format(char *name, s8 first, s8 last, s8 unused) {
    s32 i;
    Rec key;
    Rec *found;
    s32 pad;

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
        return -1;
    }
    return (found - D_801161F4[i].base) | (i << 10);
}
