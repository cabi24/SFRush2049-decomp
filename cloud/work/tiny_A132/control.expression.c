/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed short s16;
typedef int s32;
extern void particle_position_set(s16 mask);
extern void Effects_UpdateEmitters(void);
void particle_velocity_set(void)
{
    s32 i;
    for(i=0;i<4;i++) {
        particle_position_set((s16)(i==0 ? 1 : i==1 ? 2 : i==2 ? 4 : 8));
    }
    Effects_UpdateEmitters();
}
