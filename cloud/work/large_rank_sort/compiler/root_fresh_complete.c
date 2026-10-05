/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef struct { u8 pad0[0x7C6]; s16 idx; u8 pad1[0x808 - 0x7C8]; } Rec;
typedef struct { u8 pad0[20]; s32 key; u8 pad1[0x78 - 24]; } Ent;
typedef struct { u8 pad0[238]; s8 b238; u8 pad1[931 - 239]; s8 b931; u8 pad2[952 - 932]; } Car;
extern s16 D_80151AD0;
extern s8 D_80143F54[];
extern s32 gameplay_mode;
extern s16 active_player_count;
extern Rec D_8014A250[];
extern Ent D_80152038[];
extern Car player_array[];
extern s8 D_8012E67C[];
extern s8 D_8015256C[];
extern s32 D_80150B60;
extern s8 D_80150B68[];

extern s8 D_80143F55[];
void func_800F7F3C(void);
typedef unsigned int u32;
void func_800F7F3C(void) {
    s16 temp_t3;
    s32 temp_t2;
    s32 temp_t2_2;
    s32 temp_t2_3;
    s32 var_t1_2;
    s32 var_t1_3;
    s32 var_t1_4;
    s8 *var_v0;
    s8 *var_v0_2;
    s8 *var_v0_3;
    s8 *var_v0_4;
    s8 *var_v1;
    s8 *var_v1_2;
    s8 *var_v1_3;
    s8 *var_v1_4;
    s8 temp_a0;
    s8 temp_a0_2;
    s8 temp_a0_3;
    s8 temp_a0_4;
    s8 temp_v1;
    s8 temp_v1_2;
    s8 temp_v1_3;
    s8 temp_v1_4;
    s32 var_t1;

    var_t1 = 0;
    if (D_80151AD0 > 0) {
        var_v1 = D_80143F54;
        do {
            *var_v1 = var_t1;
            var_t1 += 1;
            var_v1 += 1;
        } while (var_t1 < D_80151AD0);
        var_t1 = 0;
    }
    if (gameplay_mode == 4) {
        temp_t2 = active_player_count - 1;
        if (temp_t2 > 0) {
            do {
                var_t1 += 1;
                if (temp_t2 > 0) {
                    var_v0 = D_80143F54;
                    do {
                        temp_v1 = var_v0[0];
                        temp_a0 = var_v0[1];
                        if (D_80152038[D_8014A250[temp_v1].idx].key < D_80152038[D_8014A250[temp_a0].idx].key) {
                            var_v0[1] = temp_v1;
                            var_v0[0] = temp_a0;
                        }
                        var_v0 += 1;
                    } while ((u32) var_v0 < (u32) (D_80143F54 + active_player_count - 1));
                }
            } while (var_t1 < temp_t2);
        }
        var_t1_2 = 1;
        if (active_player_count >= 2) {
            var_v1_2 = D_80143F55;
            do {
                if (D_80152038[D_8014A250[*D_80143F54].idx].key == D_80152038[D_8014A250[*var_v1_2].idx].key) {
                    D_80150B68[var_t1_2] = 1;
                    D_80150B60 += 1;
                }
                var_t1_2 += 1;
                var_v1_2 += 1;
            } while (var_t1_2 < active_player_count);
        }
    } else if (gameplay_mode == 6) {
        temp_t2_2 = active_player_count - 1;
        if (temp_t2_2 > 0) {
            do {
                var_t1 += 1;
                if (temp_t2_2 > 0) {
                    var_v0_2 = D_80143F54;
                    do {
                        temp_v1_2 = var_v0_2[0];
                        temp_a0_2 = var_v0_2[1];
                        if (player_array[D_8014A250[temp_v1_2].idx].b931 < player_array[D_8014A250[temp_a0_2].idx].b931) {
                            var_v0_2[1] = temp_v1_2;
                            var_v0_2[0] = temp_a0_2;
                        }
                        var_v0_2 += 1;
                    } while ((u32) var_v0_2 < (u32) (D_80143F54 + active_player_count - 1));
                }
            } while (var_t1 < temp_t2_2);
            var_t1 = 0;
        }
        if (temp_t2_2 > 0) {
            do {
                var_t1 += 1;
                if (temp_t2_2 > 0) {
                    var_v0_3 = D_80143F54;
                    do {
                        temp_v1_3 = var_v0_3[0];
                        temp_a0_3 = var_v0_3[1];
                        if (D_8015256C[D_8012E67C[D_8014A250[temp_v1_3].idx]] < D_8015256C[D_8012E67C[D_8014A250[temp_a0_3].idx]]) {
                            var_v0_3[1] = temp_v1_3;
                            var_v0_3[0] = temp_a0_3;
                        }
                        var_v0_3 += 1;
                    } while ((u32) var_v0_3 < (u32) (D_80143F54 + active_player_count - 1));
                }
            } while (var_t1 < temp_t2_2);
        }
        var_t1_3 = 1;
        if (active_player_count >= 2) {
            var_v1_3 = D_80143F55;
            do {
                if (player_array[D_8014A250[*D_80143F54].idx].b931 == player_array[D_8014A250[*var_v1_3].idx].b931) {
                    D_80150B68[var_t1_3] = 1;
                    D_80150B60 += 1;
                }
                var_t1_3 += 1;
                var_v1_3 += 1;
            } while (var_t1_3 < active_player_count);
        }
    } else {
        temp_t3 = active_player_count;
        temp_t2_3 = temp_t3 - 1;
        if (temp_t2_3 > 0) {
            do {
                var_t1 += 1;
                if (temp_t2_3 > 0) {
                    var_v0_4 = D_80143F54;
                    do {
                        temp_v1_4 = var_v0_4[0];
                        temp_a0_4 = var_v0_4[1];
                        if (player_array[D_8014A250[temp_a0_4].idx].b238 < player_array[D_8014A250[temp_v1_4].idx].b238) {
                            var_v0_4[1] = temp_v1_4;
                            var_v0_4[0] = temp_a0_4;
                        }
                        var_v0_4 += 1;
                    } while ((u32) var_v0_4 < (u32) (D_80143F54 + temp_t3 - 1));
                }
            } while (var_t1 < temp_t2_3);
        }
        var_t1_4 = 1;
        if (temp_t3 >= 2) {
            var_v1_4 = D_80143F55;
            do {
                if (player_array[D_8014A250[*D_80143F54].idx].b238 == player_array[D_8014A250[*var_v1_4].idx].b238) {
                    D_80150B68[var_t1_4] = 1;
                    D_80150B60 += 1;
                }
                var_t1_4 += 1;
                var_v1_4 += 1;
            } while (var_t1_4 < temp_t3);
        }
    }
}
