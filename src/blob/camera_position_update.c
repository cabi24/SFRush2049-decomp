/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * camera_position_update (0x800AEB54, 452 B): historical label. N64-only player-pair visibility matrix
 * (no arcade counterpart found). Clears the 4x4 byte matrix D_801613C0, then, when the enable flag
 * D_801613A8 is set and gameplay_mode is 6, casts a segment between every player i < count-1 and every
 * player j >= 1 (the inner loop restarts at 1 for each i, including j == i and repeated pairs), both
 * endpoints raised 5 units in y, through input_deadzone_apply (historical label: a segment/collision
 * test taking start, end, a 9-float result, 0.5f, 0, 0xFFFF). A nonnull result marks [i][j] and [j][i].
 *
 * Source-form notes (no shaping devices, no unused locals):
 * - D_801613C0 is s8 (lb/sb in all users): the stored 1 shares the s32 1 web, so j = 1 is `move s1,s3`.
 * - The y offset is `v[1] += 5.0f` after the three copies (retail adds in $f0, a coloured web).
 * - The clear loop uses its own index k declared after i, j (all three take frame slots: 12 B above
 *   first[]), written on three lines (one line changes as1's delay-slot fill of the unrolled stores).
 * w27a.
 */
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef float f32;

typedef struct Player {        /* 952 B player/car record */
    u8 pad000[8];
    f32 position[3];           /* 0x08 */
    u8 pad014[932];
} Player;

extern s8 D_801613C0[4][4];    /* visibility matrix */
extern s8 D_801613A8;          /* enable flag */
extern s32 gameplay_mode;
extern s16 active_player_count;
extern Player player_array[];

u16 *input_deadzone_apply(void *arg0, void *arg1, f32 *arg2, f32 arg3, s32 arg4, s32 arg5);

void camera_position_update(void) {
    s32 i, j, k;
    f32 first[3], second[3];
    f32 result[9];

    for (i = 0; i < 4; i++)
        for (k = 0; k < 4; k++)
            D_801613C0[i][k] = 0;
    if (D_801613A8 && gameplay_mode == 6) {
        for (i = 0; i < active_player_count - 1; i++) {
            first[0] = player_array[i].position[0];
            first[1] = player_array[i].position[1];
            first[2] = player_array[i].position[2];
            first[1] += 5.0f;
            for (j = 1; j < active_player_count; j++) {
                second[0] = player_array[j].position[0];
                second[1] = player_array[j].position[1];
                second[2] = player_array[j].position[2];
                second[1] += 5.0f;
                if (input_deadzone_apply(first, second, result, 0.5f, 0, 0xFFFF)) {
                    D_801613C0[i][j] = 1;
                    D_801613C0[j][i] = 1;
                }
            }
        }
    }
}
