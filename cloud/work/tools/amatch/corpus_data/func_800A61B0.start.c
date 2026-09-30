/*@@HDR 1 3086@@*/
u8 *func_800A4770(u8 *arg0, u8 *arg1);
/*@@HDR 3087 3096@@*/

/*@@HDR 3097 3697@@*/
#endif

void func_800A61B0(void *arg0, void *arg1, void *arg2) {
    M2C_FIELD(arg1, f32 *, 0) = (f32) ((M2C_FIELD(arg2, f32 *, 8) * M2C_FIELD(arg0, f32 *, 8)) + ((M2C_FIELD(arg0, f32 *, 0) * M2C_FIELD(arg2, f32 *, 0)) + (M2C_FIELD(arg0, f32 *, 4) * M2C_FIELD(arg2, f32 *, 4))));
    M2C_FIELD(arg1, f32 *, 4) = (f32) ((M2C_FIELD(arg2, f32 *, 0x14) * M2C_FIELD(arg0, f32 *, 8)) + ((M2C_FIELD(arg0, f32 *, 0) * M2C_FIELD(arg2, f32 *, 0xC)) + (M2C_FIELD(arg0, f32 *, 4) * M2C_FIELD(arg2, f32 *, 0x10))));
    M2C_FIELD(arg1, f32 *, 8) = (f32) ((M2C_FIELD(arg2, f32 *, 0x20) * M2C_FIELD(arg0, f32 *, 8)) + ((M2C_FIELD(arg0, f32 *, 0) * M2C_FIELD(arg2, f32 *, 0x18)) + (M2C_FIELD(arg0, f32 *, 4) * M2C_FIELD(arg2, f32 *, 0x1C))));
}
