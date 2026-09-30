/*@@HDR 1 3086@@*/
u8 *func_800A4770(u8 *arg0, u8 *arg1);
/*@@HDR 3087 3507@@*/

/*@@HDR 3508 3697@@*/
#endif

void players_race_update(void) {
    s32 var_s0;

    var_s0 = 0;
    do {
        if ((D_8014AA18 != 0) && ((s8) player_array[var_s0].pad0EC[0x26D] < 2)) {
            func_800D4DFC(&D_8014AA18);
        }
        var_s0 += 1;
    } while (var_s0 != 6);
}
