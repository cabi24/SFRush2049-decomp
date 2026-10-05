float fabsf(float);
#pragma intrinsic (fabsf)
float fabsf(float);
#pragma intrinsic (fabsf)
typedef signed char S8;
typedef unsigned char U8;
typedef signed short S16;
typedef unsigned short U16;
typedef signed int S32;
typedef unsigned int U32;
typedef float F32;
typedef S32 BOOL;
typedef struct {
                U8 pad0[0x400];
                F32 catchup;
                U8 pad404[0x7C6 - 0x404];
                S16 slot;
                S16 in_game;
                S16 we_control;
                S8 drone_type;
                U8 pad7CD[0x7E6 - 0x7CD];
                S16 drone_target;
                U8 pad7E8[4];
                F32 drone_scale;
                F32 time_boost;
                U8 pad7F4[0x808 - 0x7F4];
} MODELDAT;
typedef struct {
                U8 pad0[8];
                F32 dr_pos[3];
                U8 pad14[0xEE - 0x14];
                S8 place;
                U8 pad0EF[0x100 - 0xEF];
                F32 distance;
                U8 pad104[0x356 - 0x104];
                S16 weight_index;
                U8 pad358;
                S8 state;
                U8 pad35A[0x3B8 - 0x35A];
} CAR_DATA;
extern MODELDAT D_8014A250[];
extern CAR_DATA player_array[];
extern S8 D_80152744;
extern S32 D_801174B4;
extern F32 D_801543CC;
extern S32 D_8014A110;
extern S8 D_8014978C;
extern S8 D_80152570;
extern S16 D_80154484[][38];
extern S8 D_8013FECB;
extern S8 D_80152718;
extern S8 D_80152015;
extern S8 D_80152030;
void func_800DE860(void);
F32 func_800F92C8(F32 in_bound1, F32 in_bound2, F32 input, F32 out_bound1, F32 out_bound2);
void func_800F9398(void);
F32 func_800F92C8(F32 in_bound1, F32 in_bound2, F32 input, F32 out_bound1, F32 out_bound2)
{
    F32 slope, offset;
    if (in_bound1 < in_bound2) {
        if (input < in_bound1)
            return (out_bound1);
        else if (input > in_bound2)
            return (out_bound2);
    } else {
        if (input < in_bound2)
            return (out_bound2);
        else if (input > in_bound1)
            return (out_bound1);
    }
    if (fabsf(in_bound1 - in_bound2) < 1e-3f)
        return ((out_bound1 + out_bound2) * .5f);
    slope = (out_bound1 - out_bound2) / (in_bound1 - in_bound2);
    offset = out_bound1 - slope * in_bound1;
    return (slope * input + offset);
}
void render_large_objects(void)
{
    S16 i, j, k, indx, index, index2, index3, temp, high_index;
    S16 high_place;
    S16 total_drones[6], humans[6], drones[6];
    S16 car_in_conflict[6], cars_in_order[6], my_place;
    S16 ttype, place[6], num_humans, num_drones;
    F32 scale, delta_dist, delta_dist2, slope, offset, delta[3];
    F32 time_scale, target_dist, car_distance[6][6];
    F32 max_brake, min_brake, max_boost, min_boost, temp1;
    F32 place_scale, diff_scale;
    MODELDAT *m;
    CAR_DATA *gc;
    func_800DE860();
    for (i = 0; i < D_80152744; i++) {
        high_place = D_8014A250[i].slot;
        place[high_place] = player_array[high_place].place;
    }
    num_humans = num_drones = 0;
    for (i = 0; i < D_80152744; i++) {
        index = 0;
        for (j = 0; j < D_80152744; j++) {
            index = D_8014A250[j].slot;
            if (place[index] == i)
                break;
        }
        cars_in_order[i] = index;
        if (D_8014A250[index].drone_type == 2)
            humans[num_humans++] = index;
        else
            drones[num_drones++] = index;
    }
    for (i = 0, j = 0; i < num_drones; i++)
        player_array[drones[i]].weight_index = i;
    if (D_801174B4 & 8) {
        for (i = 0; i < D_80152744; i++) {
            index = D_8014A250[i].slot;
            if (D_8014A250[index].drone_type == 1)
                D_8014A250[index].drone_scale = 1;
        }
        return;
    }
    if (num_drones) {
        if (D_801543CC < 5) {
            for (i = 0; i < num_drones; i++) {
                j = num_drones - i - 1;
                D_8014A250[drones[i]].time_boost = ((F32)(j) * .02f) + .98f;
                D_8014A250[drones[i]].drone_scale = 1;
            }
        } else {
            for (i = 0; i < D_80152744; i++) {
                index = D_8014A250[i].slot;
                car_in_conflict[index] = 0;
                gc = &player_array[index];
                for (j = i; j < D_80152744; j++) {
                    index2 = D_8014A250[j].slot;
                    if (index == index2)
                        car_distance[index][index2] = 99999999;
                    else {
                        for (k = 0; k < 3; k++)
                            delta[k] = gc->dr_pos[k] - player_array[index2].dr_pos[k];
                        delta_dist = delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2];
                        car_distance[index][index2] =
                            car_distance[index2][index] = delta_dist;
                    }
                }
            }
            for (i = 0; i < num_drones; i++)
                D_8014A250[drones[i]].drone_target = drones[i];
            for (i = 0; i < num_humans; i++) {
                for (j = i + 1; j < num_humans; j++) {
                    if (car_distance[humans[i]][humans[j]] < 70 * 70) {
                        car_in_conflict[humans[i]] += 10;
                        car_in_conflict[humans[j]] += 10;
                    }
                }
            }
            for (i = 0; i < num_humans; i++) {
                if (car_in_conflict[humans[i]] < 10) {
                    index = humans[i];
                    my_place = place[index];
                    index2 = index3 = -1;
                    if (my_place > 0) {
                        j = cars_in_order[my_place - 1];
                        if (D_8014A250[j].drone_type == 1 && car_in_conflict[j] == 0)
                            index2 = j;
                    }
                    if (my_place < D_80152744 - 1) {
                        j = cars_in_order[my_place + 1];
                        if (D_8014A250[j].drone_type == 1 && car_in_conflict[j] == 0)
                            index3 = j;
                    }
                    if (index2 != -1 && index3 != -1) {
                        if (car_distance[index3][index] < car_distance[index2][index])
                            index2 = index3;
                    } else if (index2 == -1)
                        index2 = index3;
                    if (index2 != -1) {
                        car_in_conflict[index]++;
                        car_in_conflict[index2]++;
                        D_8014A250[index2].drone_target = index;
                    }
                }
            }
            for (indx = 0; indx < D_80152744; indx++) {
                index = D_8014A250[indx].slot;
                if (D_8014A250[index].we_control) {
                    index2 = D_8014A250[index].drone_target;
                    my_place = place[index];
                    if (D_80152030 == 5)
                        place_scale = 0;
                    else if (my_place == 0)
                        place_scale = .5f / (D_80152744 - 1);
                    else
                        place_scale = (F32)my_place / (D_80152744 - 1);
                    diff_scale = 1.0f - (F32)D_80152030 / 5.0f;
                    if (D_8014A110 == 3) {
                        temp = D_8014978C;
                        if (D_80152570)
                            high_index = 6;
                        else
                            high_index = 0;
                        diff_scale *= (F32)D_80154484[D_8014A250[index].slot][temp + high_index] * .125f + .75f;
                    }
                    if (index2 != index) {
                        delta_dist = player_array[index].distance - player_array[index2].distance;
                        if (delta_dist < -20) {
                            time_scale = func_800F92C8(-300, -60, delta_dist, ((1.0f - 1.1f) * diff_scale + 1.1f - place_scale * ((.15f - .05f) * diff_scale + .05f)), (1.0f - place_scale * ((.06f - .02f) * diff_scale + .02f)));
                            scale = ((.96f - 1.0f) * diff_scale + 1.0f - place_scale * ((.05f - .02f) * diff_scale + .02f));
                        } else {
                            if (car_in_conflict[index2] <= 1) {
                                scale = func_800F92C8(200, 60, delta_dist, (.9f - place_scale * ((.6f - .1f) * diff_scale + .1f)), ((.96f - 1.0f) * diff_scale + 1.0f - place_scale * ((.05f - .02f) * diff_scale + .02f)));
                                time_scale = (1.0f - place_scale * ((.06f - .02f) * diff_scale + .02f));
                            } else {
                                scale = ((.96f - 1.0f) * diff_scale + 1.0f - place_scale * ((.05f - .02f) * diff_scale + .02f));
                                time_scale = (1.0f - place_scale * ((.06f - .02f) * diff_scale + .02f));
                            }
                        }
                    } else {
                        if (my_place == 0) {
                            index2 = cars_in_order[1];
                            delta_dist = player_array[index].distance - player_array[index2].distance;
                            if (delta_dist < 200) {
                                time_scale = func_800F92C8(200, 0, delta_dist, (1.0f - place_scale * ((.06f - .02f) * diff_scale + .02f)), ((1.0f - 1.1f) * diff_scale + 1.1f - place_scale * ((.15f - .05f) * diff_scale + .05f)));
                                scale = ((.96f - 1.0f) * diff_scale + 1.0f - place_scale * ((.05f - .02f) * diff_scale + .02f));
                            } else {
                                scale = func_800F92C8(500, 200, delta_dist, (.9f - place_scale * ((.6f - .1f) * diff_scale + .1f)), ((.96f - 1.0f) * diff_scale + 1.0f - place_scale * ((.05f - .02f) * diff_scale + .02f)));
                                time_scale = (1.0f - place_scale * ((.06f - .02f) * diff_scale + .02f));
                            }
                        } else if (my_place == D_80152744 - 1) {
                            index2 = cars_in_order[D_80152744 - 2];
                            delta_dist = player_array[index].distance - player_array[index2].distance;
                            if (delta_dist < -150) {
                                time_scale = func_800F92C8(-150, -300, delta_dist, (1.0f - place_scale * ((.06f - .02f) * diff_scale + .02f)), ((1.0f - 1.1f) * diff_scale + 1.1f - place_scale * ((.15f - .05f) * diff_scale + .05f)));
                                scale = ((.96f - 1.0f) * diff_scale + 1.0f - place_scale * ((.05f - .02f) * diff_scale + .02f));
                            } else {
                                scale = func_800F92C8(-150, 0, delta_dist, ((.96f - 1.0f) * diff_scale + 1.0f - place_scale * ((.05f - .02f) * diff_scale + .02f)), (.9f - place_scale * ((.6f - .1f) * diff_scale + .1f)));
                                time_scale = (1.0f - place_scale * ((.06f - .02f) * diff_scale + .02f));
                            }
                        } else {
                            index2 = cars_in_order[my_place - 1];
                            delta_dist = player_array[index].distance - player_array[index2].distance;
                            index2 = cars_in_order[my_place + 1];
                            delta_dist2 = player_array[index].distance - player_array[index2].distance;
                            if (delta_dist > -150 && delta_dist2 < 150) {
                                target_dist = (delta_dist + delta_dist2) / 2;
                                if (target_dist < 0) {
                                    time_scale = func_800F92C8(delta_dist - target_dist, -200, delta_dist, (1.0f - place_scale * ((.06f - .02f) * diff_scale + .02f)), ((1.0f - 1.1f) * diff_scale + 1.1f - place_scale * ((.15f - .05f) * diff_scale + .05f)));
                                    scale = ((.96f - 1.0f) * diff_scale + 1.0f - place_scale * ((.05f - .02f) * diff_scale + .02f));
                                } else {
                                    scale = func_800F92C8(delta_dist2 - target_dist, 200, delta_dist2, ((.96f - 1.0f) * diff_scale + 1.0f - place_scale * ((.05f - .02f) * diff_scale + .02f)), (.9f - place_scale * ((.6f - .1f) * diff_scale + .1f)));
                                    time_scale = (1.0f - place_scale * ((.06f - .02f) * diff_scale + .02f));
                                }
                            } else {
                                for (i = 0, j = 0; i < my_place; i++) {
                                    if (D_8014A250[cars_in_order[i]].drone_type == 2)
                                        j++;
                                }
                                for (i = my_place, k = 0; i < D_80152744; i++) {
                                    if (D_8014A250[cars_in_order[i]].drone_type == 2)
                                        k++;
                                }
                                if (j >= k) {
                                    if (delta_dist < -150) {
                                        time_scale = func_800F92C8(-150, -300, delta_dist, (1.0f - place_scale * ((.06f - .02f) * diff_scale + .02f)), ((1.0f - 1.1f) * diff_scale + 1.1f - place_scale * ((.15f - .05f) * diff_scale + .05f)));
                                        scale = ((.96f - 1.0f) * diff_scale + 1.0f - place_scale * ((.05f - .02f) * diff_scale + .02f));
                                    } else {
                                        scale = func_800F92C8(-150, 0, delta_dist, ((.96f - 1.0f) * diff_scale + 1.0f - place_scale * ((.05f - .02f) * diff_scale + .02f)), (.9f - place_scale * ((.6f - .1f) * diff_scale + .1f)));
                                        time_scale = (1.0f - place_scale * ((.06f - .02f) * diff_scale + .02f));
                                    }
                                } else {
                                    if (delta_dist2 < 150) {
                                        time_scale = func_800F92C8(150, 0, delta_dist2, (1.0f - place_scale * ((.06f - .02f) * diff_scale + .02f)), ((1.0f - 1.1f) * diff_scale + 1.1f - place_scale * ((.15f - .05f) * diff_scale + .05f)));
                                        scale = ((.96f - 1.0f) * diff_scale + 1.0f - place_scale * ((.05f - .02f) * diff_scale + .02f));
                                    } else {
                                        scale = func_800F92C8(500, 150, delta_dist2, (.9f - place_scale * ((.6f - .1f) * diff_scale + .1f)), ((.96f - 1.0f) * diff_scale + 1.0f - place_scale * ((.05f - .02f) * diff_scale + .02f)));
                                        time_scale = (1.0f - place_scale * ((.06f - .02f) * diff_scale + .02f));
                                    }
                                }
                            }
                        }
                    }
                    if (D_8013FECB || D_80152718)
                        time_scale = (1.0f - place_scale * ((.06f - .02f) * diff_scale + .02f));
                    if (D_80152015 && player_array[humans[num_humans - 1]].place > place[index]) {
                        scale = ((.96f - 1.0f) * diff_scale + 1.0f - place_scale * ((.05f - .02f) * diff_scale + .02f));
                        time_scale = (1.0f - place_scale * ((.06f - .02f) * diff_scale + .02f));
                    }
                    temp1 = D_8014A250[index].drone_scale;
                    if (temp1 > scale) {
                        temp1 -= .01f;
                        if (temp1 < scale)
                            temp1 = scale;
                    } else {
                        temp1 += .01f;
                        if (temp1 > scale)
                            temp1 = scale;
                    }
                    D_8014A250[index].drone_scale = temp1;
                    temp1 = D_8014A250[index].time_boost;
                    if (temp1 > time_scale) {
                        temp1 -= .01f;
                        if (temp1 < time_scale)
                            temp1 = time_scale;
                    } else {
                        temp1 += .01f;
                        if (temp1 > time_scale)
                            temp1 = time_scale;
                    }
                    D_8014A250[index].time_boost = temp1;
                }
            }
        }
    }
}
