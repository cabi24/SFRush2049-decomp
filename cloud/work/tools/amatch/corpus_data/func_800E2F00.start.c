/*@@HDR 1 3086@@*/
u8 *func_800A4770(u8 *arg0, u8 *arg1);
/*@@HDR 3087 3306@@*/

/*@@HDR 3307 3697@@*/
#endif

void func_800E2F00(void *arg0) {
    s32 temp_v0;
    s32 temp_v0_2;

    if (M2C_FIELD(arg0, s8 *, 0x640) != 0) {
        M2C_FIELD(arg0, f32 *, 0x404) = (f32) func_800E2D18(arg0, (s16) (s32) (M2C_FIELD(arg0, f32 *, 0x408) * D_801243E0), 0, M2C_FIELD(arg0, s32 *, 0x40C));
    } else if (M2C_FIELD(arg0, s16 *, 0x3F4) < 0) {
        M2C_FIELD(arg0, f32 *, 0x404) = (f32) func_800E2D18(arg0, (s16) (s32) (M2C_FIELD(arg0, f32 *, 0x408) * D_801243E4), (s16) (s32) (M2C_FIELD(arg0, f32 *, 0x3D0) * 128.0f), M2C_FIELD(arg0, s32 *, 0x40C));
    } else {
        M2C_FIELD(arg0, f32 *, 0x404) = (f32) func_800E2D18(arg0, (s16) (s32) (M2C_FIELD(arg0, f32 *, 0x408) * *(f32 *)0x801243E8), (s16) (s32) (M2C_FIELD(arg0, f32 *, 0x3D0) * 128.0f), M2C_FIELD(arg0, s32 *, 0x40C));
    }
    temp_v0 = M2C_FIELD(arg0, s32 *, 0x614);
    if (((temp_v0 == 1) || (temp_v0 == 2)) && ((temp_v0_2 = M2C_FIELD(arg0, s32 *, 0x618), (temp_v0_2 == 1)) || (temp_v0_2 == 2))) {
        M2C_FIELD(arg0, f32 *, 0x404) = (f32) (M2C_FIELD(arg0, f32 *, 0x404) * 1.5f);
    }
    if ((M2C_FIELD(arg0, s8 *, 0xA) != 0) && (M2C_FIELD(arg0, s16 *, 0x3F4) != 4)) {
        M2C_FIELD(arg0, f32 *, 0x404) = (f32) (M2C_FIELD(arg0, f32 *, 0x404) * D_801243EC);
    }
}
