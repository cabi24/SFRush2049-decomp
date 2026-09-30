/*@@HDR 0 3086@@*/
u8 *func_800A4770(u8 *arg0, u8 *arg1);
/*@@HDR 3087 3237@@*/

/*@@HDR 3238 3697@@*/
#endif

f32 func_800D0A34(void *arg0) {
    f32 var_f2;
    s8 temp_v0;

    var_f2 = ((f32 (*)[3]) &D_801111E0)[M2C_FIELD(arg0, s8 *, 0xD)][M2C_FIELD(arg0, s8 *, 0xB)];
    temp_v0 = (&D_8011156C)[M2C_FIELD(arg0, u8 *, 8)];
    if (temp_v0 == 0) {
        var_f2 += D_80124148;
    } else if (temp_v0 == 2) {
        var_f2 -= D_8012414C;
    }
    return var_f2;
}
