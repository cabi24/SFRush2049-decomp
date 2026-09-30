/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/*@@HDR 1 3086@@*/
u8 *func_800A4770(u8 *arg0, u8 *arg1);
/*@@HDR 3087 3297@@*/

/*@@HDR 3298 3697@@*/
#endif

void func_800E1540(f32 *arg0) {
    f32 t;

    t = D_80142764 / arg0[0x5BC / 4];
    arg0[0x28 / 4] = arg0[0x10 / 4] * t;
    arg0[0x2C / 4] = arg0[0x14 / 4] * t;
    arg0[0x30 / 4] = arg0[0x18 / 4] * t;
    func_800E1500(arg0, arg0 + 7, arg0 + 0xD);
}
