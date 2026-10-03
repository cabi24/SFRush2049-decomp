/* Reconstructed region of setup_state_main, 800FB378 through 800FB5BC.
 * Include within the genuine body's first-entry, flags&0x02000000 branch.
 * Requires local int i; SetupModel952 *model; SetupVehicle2056 *vehicle;
 * and u8 player. This is NOT a standalone replacement or matching root.
 */
D_8015274C = 0;
D_80152768 = D_8015274C;
D_80153FD2 = D_8015274C;
for (i = 0; i < D_8014A108; i++) {
    player = D_8014A118[i].player;
    D_80153E88[player].mode -= 6;
    model = &player_array[player];
    vehicle = &D_8014A250[player];
    model->byte_359 = 0;
    model->byte_ef = 0;
    model->word_380 = 0;
    vehicle->byte_7cc = 1;
    vehicle->half_7ca = 1;
    vehicle->byte_a = 1;
    vehicle->word_7d4 = 0;
    model->flags_e8 = 0;
    music_tempo_set((s16)player, vehicle->mode, 1);
    D_801527D8[D_80152768] = (s16)i;
    D_80152768++;
    D_80152808[D_8015274C] = (s16)i;
    D_8015274C++;
}
for (; i < D_80152744; i++) {
    model = &player_array[i];
    vehicle = &D_8014A250[i];
    model->byte_359 = 0;
    model->byte_ef = 0;
    model->flags_e8 = 0;
    vehicle->word_7d4 = 0;
    music_tempo_set((s16)i, vehicle->mode, 1);
    D_801527D8[D_80152768] = (s16)i;
    D_80152768++;
    D_80152808[D_8015274C] = (s16)i;
    D_8015274C++;
}
D_80153E84 = D_8015274C;
D_80153F08 = D_80153E84;
D_80153F24 = -1;
D_80153F40 = 0;
i = 0;
init_state_continue();
/* Native saves the live zero i at sp+0xFC across the non-O32 FAF6C call.
 * Do not insert a synthetic spill in C. Remaining parent body is not recovered
 * here, and this fragment cannot establish the required IPA partition. */
