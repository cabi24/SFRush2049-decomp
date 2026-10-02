/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
extern int D_8014A110;
extern void func_800F56E0(void);
extern void place_cars_in_order(void);
extern void assign_drones(void);
extern void audio_update_a(void);
extern void graphics_chunk_b(void);
extern void graphics_chunk(void);
void track_lighting_setup(void)
{
    switch(D_8014A110) {
    case 0:case 3:func_800F56E0();break;
    case 1:place_cars_in_order();break;
    case 2:assign_drones();break;
    case 4:audio_update_a();break;
    case 5:graphics_chunk_b();break;
    case 6:graphics_chunk();break;
    }
}
