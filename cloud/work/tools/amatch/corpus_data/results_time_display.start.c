/*@@HDR 1 3086@@*/
u8 *func_800A4770(u8 *arg0, u8 *arg1);
/*@@HDR 3087 3538@@*/

/*@@HDR 3539 3697@@*/
#endif

s32 results_time_display(s32 arg0) {
    s32 sp1C;
    s32 temp_v0;
    s32 temp_v1;
    s32 var_v1;

    osRecvMesg((OSMesgQueue *) &D_80142728, NULL, 1);
    temp_v0 = func_80091BA8(arg0);
    if (temp_v0 != 0) {
        temp_v1 = M2C_FIELD(temp_v0, s32 *, 0x10);
        if ((temp_v1 == 1) || (temp_v1 == 3)) {
            var_v1 = 1;
        } else {
            if (temp_v1 != 2) {
                goto block_7;
            }
            var_v1 = entity_state_check(M2C_FIELD(temp_v0, s32 *, 0x3C));
        }
    } else {
block_7:
        var_v1 = 0;
    }
    sp1C = var_v1;
    osJamMesg((OSMesgQueue *) &D_80142728, NULL, 0);
    return sp1C;
}
/* Warning: struct __OSThreadprofile_s is not defined (only forward-declared) */
