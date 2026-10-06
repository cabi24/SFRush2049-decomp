/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef signed short s16;
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct CleanupRow { u32 unknown00; int identifier; u8 unknown08[56]; } CleanupRow;
extern CleanupRow D_8011650C[32];
extern u32 D_801174B4;
extern int D_80116DA4, D_80116D98, D_80116D0C;
extern s8 D_80116D94;
extern void sound_handles_clear(int);
extern void sound_stop(int);
extern void ambient_sounds_clear(void);
extern void entity_spawn_callback(s16, int, int);
void func_800DA0BC(void)
{
    CleanupRow *row;
    int identifier;
    if (!(D_801174B4 & 0x7C03FFFE)) {
        sound_handles_clear(0);
        D_80116DA4 = 0;
    }
    if (D_80116D98 != 0) {
        sound_stop(D_80116D98);
        D_80116D98 = 0;
    }
    ambient_sounds_clear();
    for (row = D_8011650C; row != D_8011650C + 32; ++row) {
        identifier = row->identifier;
        if (identifier != -1) {
            entity_spawn_callback((s16)identifier, 0, 0);
            row->identifier = -1;
        }
    }
    D_80116D0C = 0;
    D_80116D94 = 0;
}
