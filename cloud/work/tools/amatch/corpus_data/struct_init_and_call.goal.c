/*@@HDR 0 2615@@*/
typedef struct { s8 active; s8 pad1; s16 id; s16 count; s16 pad6; s32 arg; s32 aux; } CallSlot;
extern CallSlot D_80153F10;
/*@@HDR 2616 3086@@*/
u8 *func_800A4770(u8 *arg0, u8 *arg1);
/*@@HDR 3087 3591@@*/

/*@@HDR 3592 3697@@*/
#endif

s32 struct_init_and_call(s32 arg0, s32 arg1, s32 arg2) {
    if (arg1 != 0) {
        D_80153F10.arg = arg0;
        D_80153F10.id = arg1;
        D_80153F10.aux = arg2;
        D_80153F10.count = 0;
        D_80153F10.active = 1;
        func_8008ABE4();
    }
    return 1;
}
