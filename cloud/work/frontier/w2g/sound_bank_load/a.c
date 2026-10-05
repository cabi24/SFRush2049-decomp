/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

typedef struct {
    char *base;
    u32 count;
} Table;

extern Table D_80138670[];
extern u8 D_80140BDC;

char *func_800A473C(char *dst, char *src);
s8 func_80092D80(s32 idx);
void *entity_name_copy(void *key, void *base, u32 n, u32 size, s32 (*compar)(void *, void *));
s32 func_8009508C(void *, void *);

char *sound_bank_load(char *name, u16 *out, s8 lo, s8 hi) {
    char key[28];
    s32 i;
    char *found = 0;

    func_800A473C(key, name);
    if (lo < 0) {
        lo = 0;
    }
    if (hi < 0 || hi >= D_80140BDC) {
        hi = D_80140BDC - 1;
    }
    for (i = lo; i <= hi; i++) {
        if (func_80092D80(i) == 0) {
            continue;
        }
        found = entity_name_copy(key, D_80138670[i].base, D_80138670[i].count, 24, func_8009508C);
        if (found != 0) {
            break;
        }
    }
    if (found == 0) {
        *out = 0;
        return 0;
    }
    *out = ((found - D_80138670[i].base) / 24) | (i << 10);
    return found;
}
