typedef struct Str11 { char c[11]; } Str11;
typedef struct NameTbl { char *base; int count; } NameTbl;
extern Str11 D_80120E68;
extern unsigned char D_80140BDC;
extern NameTbl D_801161F4[];
extern int func_80095120();
extern void func_80092DCC();
extern int func_80092D80();
extern char *entity_name_copy();
int string_copy_format(char *name, signed char lo, signed char hi, int unused) {
    union { Str11 s; char c[92]; } buf;
    char *found = 0;
    int i;
    if (name != 0 && *name != 0) {
        func_80092DCC(buf.c, name, sizeof(buf.c) > 16 ? 16 : 16);
    } else {
        buf.s = *(Str11 *)"abcdefghij";
    }
    if (lo < 0) lo = 0;
    if (hi < 0 || hi >= D_80140BDC) hi = D_80140BDC - 1;
    for (i = lo; i <= hi; i++) {
        if (func_80092D80(i)) {
            found = entity_name_copy(buf.c, D_801161F4[i].base, D_801161F4[i].count, 88, func_80095120);
            if (found) break;
        }
    }
    if (found == 0) return -1;
    return (((int)found - (int)D_801161F4[i].base) / 88) | (i << 10);
}
