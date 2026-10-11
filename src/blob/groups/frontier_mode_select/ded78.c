/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* func_800DED78: N64 per-player get_force_and_peak.
 * Arcade donor: historicalsource/rushtherock 845329d7, game/carsnd.c.
 * N64 uses float seconds, five 24-byte impact records per player.
 */
typedef signed short s16;
typedef int s32;
typedef float f32;
typedef struct Impact24 { f32 force, vector[3], timestamp; s32 flag; } Impact24;
typedef struct Impact120 { Impact24 point[5]; } Impact120;
extern Impact120 D_80140808[];
extern volatile f32 D_8002EB90;
f32 sqrtf(f32);
#pragma intrinsic(sqrtf)

void func_800DED78(s16 index, s32 player, f32 vec[3], f32 threshold)
{
    f32 force;
    if (D_80140808[player].point[index].flag == 0) {
        if (D_80140808[player].point[index].timestamp && D_80140808[player].point[index].force == 0) {
            if (vec[0] == 0 && vec[1] == 0 && vec[2] == 0) {
                if (D_8002EB90 - D_80140808[player].point[index].timestamp > 0.5f)
                    D_80140808[player].point[index].timestamp = 0;
            } else
                D_80140808[player].point[index].timestamp = D_8002EB90;
        } else {
            if (vec[0] != 0 || vec[1] != 0 || vec[2] != 0) {
                force = sqrtf(vec[0]*vec[0] + vec[1]*vec[1] + vec[2]*vec[2]);
                if (threshold < force) {
                    if (force > D_80140808[player].point[index].force) {
                        D_80140808[player].point[index].force = force;
                        D_80140808[player].point[index].vector[0] = vec[0];
                        D_80140808[player].point[index].vector[1] = vec[1];
                        D_80140808[player].point[index].vector[2] = vec[2];
                        D_80140808[player].point[index].timestamp = D_8002EB90;
                    }
                }
            }
            if (D_80140808[player].point[index].timestamp && D_8002EB90 - D_80140808[player].point[index].timestamp > 0.1f) {
                D_80140808[player].point[index].flag = 1;
                D_80140808[player].point[index].timestamp = D_8002EB90;
            }
        }
    }
}
