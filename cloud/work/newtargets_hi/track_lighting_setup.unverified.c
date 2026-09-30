/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed int s32;
extern s32 D_8014A110;
void func_800F56E0(void);
void place_cars_in_order(void);
void assign_drones(void);
void audio_update_a(void);
void graphics_chunk_b(void);
void graphics_chunk(void);
void track_lighting_setup(void) {
    switch (D_8014A110) {
    case 0: func_800F56E0(); break;
    case 1: place_cars_in_order(); break;
    case 2: assign_drones(); break;
    case 3: audio_update_a(); break;
    case 4: graphics_chunk_b(); break;
    case 5: graphics_chunk(); break;
    case 6: break;
    }
}
