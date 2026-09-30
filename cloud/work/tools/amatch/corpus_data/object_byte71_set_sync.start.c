/*@@HDR 1 3086@@*/
u8 *func_800A4770(u8 *arg0, u8 *arg1);
/*@@HDR 3087 3463@@*/

/*@@HDR 3464 3697@@*/
#endif

void object_byte71_set_sync(void **arg0, u8 arg1) {
    void *temp_v0;
    void *temp_v1;

    temp_v1 = *M2C_FIELD(*arg0, void ***, 0x2C);
    if (arg1 != M2C_FIELD(temp_v1, u8 *, 0x47)) {
        M2C_FIELD(temp_v1, u8 *, 0x47) = arg1;
        temp_v0 = *arg0;
        slot_state_lookup(M2C_FIELD(temp_v0, void ***, 8), (u8 *) *M2C_FIELD(temp_v0, void ***, 0x2C) + 0x47, 1);
    }
}
