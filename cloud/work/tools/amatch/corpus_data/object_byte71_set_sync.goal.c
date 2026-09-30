/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*@@HDR 1 3086@@*/
u8 *func_800A4770(u8 *arg0, u8 *arg1);
/*@@HDR 3087 3463@@*/

/*@@HDR 3464 3697@@*/
#endif

void object_byte71_set_sync(u8 ***arg0, s32 arg1) {
    u8 *p;

    p = *(u8 **) *(u8 ***) ((u8 *) *arg0 + 0x2C);
    if (arg1 != p[0x47]) {
        p[0x47] = arg1;
        slot_state_lookup(*(void ***) ((u8 *) *arg0 + 8), (u32) (*(u8 **) *(u8 ***) ((u8 *) *arg0 + 0x2C) + 0x47), 1);
    }
}
