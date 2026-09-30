/*@@HDR 0 3086@@*/
u8 *func_800A4770(u8 *arg0, u8 *arg1);
/*@@HDR 3087 3199@@*/

/*@@HDR 3200 3697@@*/
#endif

void func_800C4C9C(void *arg0, s16 arg1) {
 u8 *temp_v0;
 temp_v0 = &player_array[M2C_FIELD(arg0, s16 *, 0x7C6)].pad0EC[0x228];
M2C_FIELD(temp_v0, s16 *, 0x24) = arg1;
M2C_FIELD(temp_v0, f32 *, 0x20) = (f32) D_801543CC;
M2C_FIELD(temp_v0, s16 *, 0x26) = -1;
M2C_FIELD(temp_v0, f32 *, 0) = 20.0f;
M2C_FIELD(temp_v0, f32 *, 4) = 0.0f;
}
