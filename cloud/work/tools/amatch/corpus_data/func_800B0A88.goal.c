void func_800B0A88(s16 slot, s16 idx) {
    HudRec *h;
    f32 vec[3];
    s32 k;
    s32 b8;

    if (gameplay_mode == 2 || gameplay_mode == 6 || (h = &((HudRec *) &D_8014A250)[slot], h->s7CA != 0)) {
        ((SndSlot *) ((u32) ((u8 *) &player_array[slot] + 0x290)))[idx].cb = 0;
        return;
    }
    (&((SndSlot *) ((u32) ((u8 *) &player_array[slot] + 0x290)))[idx])->cb = (void *) buffer_swap;
    (&((SndSlot *) ((u32) ((u8 *) &player_array[slot] + 0x290)))[idx])->slot = slot;
    (&((SndSlot *) ((u32) ((u8 *) &player_array[slot] + 0x290)))[idx])->idx = idx;
    b8 = h->b8;
    vec[0] = 0.0f;
    vec[2] = 0.0f;
    vec[1] = *(f32 *) ((u8 *) &D_8011B4B8 + b8 * 0xC);
    k = D_80111299[slot * 13 + b8] * 4 + idx + 0xE0;
    (&((SndSlot *) ((u32) ((u8 *) &player_array[slot] + 0x290)))[idx])->handle = save_slot_valid(D_801427C0[k], 0xF, ((SndCtl *) &D_80139320)[slot].first, 0, slot, -1, 1);
    func_8008D6FC((&((SndSlot *) ((u32) ((u8 *) &player_array[slot] + 0x290)))[idx])->handle, vec, NULL);
    model_data_load((&((SndSlot *) ((u32) ((u8 *) &player_array[slot] + 0x290)))[idx])->handle, 1, 0xF);
    func_80092484(slot, idx);
}