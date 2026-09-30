/*@@HDR 0 2171@@*/
extern u8 D_80140BDC[];
/*@@HDR 2172 3086@@*/
u8 *func_800A4770(u8 *arg0, u8 *arg1);
/*@@HDR 3087 3119@@*/

/*@@HDR 3120 3697@@*/
#endif

typedef struct { u8 *ptr; s32 count; } ABBEnt;
f32 func_800ABB58(s32 arg0) {
    s32 idx;
    s32 sub;
    ABBEnt *e;

    idx = arg0 >> 0xA;
    if (idx >= (s32) *(volatile u8 *) D_80140BDC) {
        return 0.0f;
    }
    e = (ABBEnt *) ((u8 *) &D_801161F4 + idx * 8);
    sub = arg0 & 0x3FF;
    if (sub >= e->count) {
        return 0.0f;
    }
    return *(f32 *) (e->ptr + sub * 0x58 + 0x10);
}
