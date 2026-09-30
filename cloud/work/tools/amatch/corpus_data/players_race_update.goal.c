/*@@HDR 0 3086@@*/
u8 *func_800A4770(u8 *arg0, u8 *arg1);
/*@@HDR 3087 3507@@*/

/*@@HDR 3508 3697@@*/
#endif

void players_race_update(void) {
    D_8014A250_Record *r;
    s32 i;
    for (i = 0, r = &D_8014A250; i != 6; i++, r = (D_8014A250_Record *) ((s8 *) r + 0x808)) {
        if ((*(s16 *) ((s8 *) r + 0x7C8) != 0) && ((s8) player_array[i].pad0EC[0x26D] < 2)) {
            func_800D4DFC(r);
        }
    }
}
