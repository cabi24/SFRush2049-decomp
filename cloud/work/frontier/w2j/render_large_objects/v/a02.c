/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
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

#define MAX_LINKS 6
#define DRONE 1
#define HUMAN 2

typedef struct {
    /* 0x000 */ U8 pad0[0x400];
    /* 0x400 */ F32 catchup;
    /* 0x404 */ U8 pad404[0x7C6 - 0x404];
    /* 0x7C6 */ S16 slot;
    /* 0x7C8 */ S16 in_game;
    /* 0x7CA */ S16 we_control;
    /* 0x7CC */ S8 drone_type;
    /* 0x7CD */ U8 pad7CD[0x7E6 - 0x7CD];
    /* 0x7E6 */ S16 drone_target;
    /* 0x7E8 */ U8 pad7E8[4];
    /* 0x7EC */ F32 drone_scale;
    /* 0x7F0 */ F32 time_boost;
    /* 0x7F4 */ U8 pad7F4[0x808 - 0x7F4];
} MODELDAT;

typedef struct {
    /* 0x000 */ U8 pad0[8];
    /* 0x008 */ F32 dr_pos[3];
    /* 0x014 */ U8 pad14[0xEE - 0x14];
    /* 0x0EE */ S8 place;
    /* 0x0EF */ U8 pad0EF[0x100 - 0xEF];
    /* 0x100 */ F32 distance;
    /* 0x104 */ U8 pad104[0x356 - 0x104];
    /* 0x356 */ S16 weight_index;
    /* 0x358 */ U8 pad358;
    /* 0x359 */ S8 state;
    /* 0x35A */ U8 pad35A[0x3B8 - 0x35A];
} CAR_DATA;

extern MODELDAT D_8014A250[];
extern CAR_DATA player_array[];
#define model D_8014A250
#define game_car player_array
extern S8 D_80152744;
#define num_active_cars D_80152744
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
#define coast_flag D_8013FECB
#define end_game_flag D_80152718
#define lap_flag D_80152015

void func_800DE860(void);
F32 func_800F92C8(F32 in_bound1, F32 in_bound2, F32 input, F32 out_bound1, F32 out_bound2);
#define linear_interp func_800F92C8
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

#ifdef PCIO_FN
void place_cars_in_order(S16 *cars_in_order, S16 *humans, S16 *drones, S16 *num_humans, S16 *num_drones, S16 *place)
{
    S16 i, j, index;

    *num_humans = *num_drones = 0;

    for (i = 0; i < num_active_cars; i++) {
        index = 0;
        for (j = 0; j < num_active_cars; j++) {
            index = model[j].slot;

            if (place[index] == i)
                break;
        }

        cars_in_order[i] = index;

        if (model[index].drone_type == HUMAN)
            humans[(*num_humans)++] = index;
        else
            drones[(*num_drones)++] = index;
    }
}
#endif

#define MAX_BOOST ((1.0f - 1.1f) * diff_scale + 1.1f - place_scale * ((.15f - .05f) * diff_scale + .05f))
#define MIN_BOOST (1.0f - place_scale * ((.06f - .02f) * diff_scale + .02f))
#define MAX_BRAKE (.9f - place_scale * ((.6f - .1f) * diff_scale + .1f))
#define MIN_BRAKE ((.96f - 1.0f) * diff_scale + 1.0f - place_scale * ((.05f - .02f) * diff_scale + .02f))

#ifdef AD_FN
void func_800F9398(void)
#else
void render_large_objects(void)
#endif
{
    S16 i, j, k, indx, index, index2, index3, temp, high_index;
    S16 high_place;
    S16 total_drones[MAX_LINKS], humans[MAX_LINKS], drones[MAX_LINKS];
    S16 car_in_conflict[MAX_LINKS], cars_in_order[MAX_LINKS], my_place;
    S16 ttype, place[MAX_LINKS], num_humans, num_drones;
    F32 scale, delta_dist, delta_dist2, slope, offset, delta[3];
    F32 time_scale, target_dist, car_distance[MAX_LINKS][MAX_LINKS];
    F32 max_brake, min_brake, max_boost, min_boost, temp1;
    F32 place_scale, diff_scale;
    S32 mode;
    S32 off;
    MODELDAT *m;
    CAR_DATA *gc;

#ifndef AD_FN
    func_800DE860();
#endif

    for (i = 0; i < num_active_cars; i++) {
        index = model[i].slot;
        place[index] = game_car[index].place;
    }

#ifdef PCIO_FN
    place_cars_in_order(cars_in_order, humans, drones, &num_humans, &num_drones, place);
#else
    num_humans = num_drones = 0;

    for (i = 0; i < num_active_cars; i++) {
        index = 0;
        for (j = 0; j < num_active_cars; j++) {
            index = model[j].slot;

            if (place[index] == i)
                break;
        }

        cars_in_order[i] = index;

        if (model[index].drone_type == HUMAN)
            humans[num_humans++] = index;
        else
            drones[num_drones++] = index;
    }
#endif

    for (i = 0, j = 0; i < num_drones; i++)
        game_car[drones[i]].weight_index = i;

    if (D_801174B4 & 8) {
        for (i = 0; i < num_active_cars; i++) {
            index = model[i].slot;

            if (model[index].drone_type == DRONE)
                model[index].drone_scale = 1;
        }

        return;
    }

    if (num_drones) {
        if (D_801543CC < 5) {
            for (i = 0; i < num_drones; i++) {
                j = num_drones - i - 1;
                model[drones[i]].time_boost = ((F32)(j) * .02f) + .98f;
                model[drones[i]].drone_scale = 1;
            }
        } else {
            for (i = 0; i < num_active_cars; i++) {
                index = model[i].slot;
                car_in_conflict[index] = 0;
                gc = &game_car[index];

                for (j = i; j < num_active_cars; j++) {
                    index2 = model[j].slot;
                    if (index == index2)
                        car_distance[index][index2] = 99999999;
                    else {
                        for (k = 0; k < 3; k++)
                            delta[k] = gc->dr_pos[k] - game_car[index2].dr_pos[k];

                        delta_dist = delta[0] * delta[0] + delta[1] * delta[1] + delta[2] * delta[2];

                        car_distance[index][index2] =
                            car_distance[index2][index] = delta_dist;
                    }
                }
            }

            for (i = 0; i < num_drones; i++)
                model[drones[i]].drone_target = drones[i];

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
                        if (model[j].drone_type == DRONE && car_in_conflict[j] == 0)
                            index2 = j;
                    }

                    if (my_place < num_active_cars - 1) {
                        j = cars_in_order[my_place + 1];
                        if (model[j].drone_type == DRONE && car_in_conflict[j] == 0)
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
                        model[index2].drone_target = index;
                    }
                }
            }

            for (indx = 0; indx < num_active_cars; indx++) {
                index = model[indx].slot;

                if (model[index].we_control) {
                    index2 = model[index].drone_target;
                    my_place = place[index];

                    diff_scale = 1.0f - (F32)D_80152030 / 5.0f;

                    if (D_80152030 == 5)
                        place_scale = 0;
                    else if (my_place == 0)
                        place_scale = .5f / (num_active_cars - 1);
                    else
                        place_scale = (F32)my_place / (num_active_cars - 1);

                    if (D_8014A110 == 3) {
                        mode = D_8014978C;
                        off = 0;
                        if (D_80152570)
                            off = 6;
                        diff_scale *= (F32)D_80154484[model[index].slot][mode + off] * .125f + .75f;
                    }

#ifdef VARS
                    max_boost = (1.0f - 1.1f) * diff_scale + 1.1f - place_scale * ((.15f - .05f) * diff_scale + .05f);
                    min_boost = 1.0f - place_scale * ((.06f - .02f) * diff_scale + .02f);
                    max_brake = .9f - place_scale * ((.6f - .1f) * diff_scale + .1f);
                    min_brake = (.96f - 1.0f) * diff_scale + 1.0f - place_scale * ((.05f - .02f) * diff_scale + .02f);
#else
#define max_boost MAX_BOOST
#define min_boost MIN_BOOST
#define max_brake MAX_BRAKE
#define min_brake MIN_BRAKE
#endif

                    if (index2 != index) {
                        delta_dist = game_car[index].distance - game_car[index2].distance;

                        if (delta_dist < -20) {
                            time_scale = linear_interp(-300, -60, delta_dist, max_boost, min_boost);
                            scale = min_brake;
                        } else {
                            if (car_in_conflict[index2] <= 1) {
                                scale = linear_interp(200, 60, delta_dist, max_brake, min_brake);
                                time_scale = min_boost;
                            } else {
                                scale = min_brake;
                                time_scale = min_boost;
                            }
                        }
                    } else {
                        if (my_place == 0) {
                            index2 = cars_in_order[1];
                            delta_dist = game_car[index].distance - game_car[index2].distance;

                            if (delta_dist < 200) {
                                time_scale = linear_interp(200, 0, delta_dist, min_boost, max_boost);
                                scale = min_brake;
                            } else {
                                scale = linear_interp(500, 200, delta_dist, max_brake, min_brake);
                                time_scale = min_boost;
                            }
                        } else if (my_place == num_active_cars - 1) {
                            index2 = cars_in_order[num_active_cars - 2];

                            delta_dist = game_car[index].distance - game_car[index2].distance;

                            if (delta_dist < -150) {
                                time_scale = linear_interp(-150, -300, delta_dist, min_boost, max_boost);
                                scale = min_brake;
                            } else {
                                scale = linear_interp(-150, 0, delta_dist, min_brake, max_brake);
                                time_scale = min_boost;
                            }
                        } else {
                            index2 = cars_in_order[my_place - 1];
                            delta_dist = game_car[index].distance - game_car[index2].distance;

                            index2 = cars_in_order[my_place + 1];
                            delta_dist2 = game_car[index].distance - game_car[index2].distance;

                            if (delta_dist > -150 && delta_dist2 < 150) {
                                target_dist = (delta_dist + delta_dist2) / 2;

                                if (target_dist < 0) {
                                    time_scale = linear_interp(delta_dist - target_dist, -200, delta_dist, min_boost, max_boost);
                                    scale = min_brake;
                                } else {
                                    scale = linear_interp(delta_dist2 - target_dist, 200, delta_dist2, min_brake, max_brake);
                                    time_scale = min_boost;
                                }
                            } else {
                                for (i = 0, j = 0; i < my_place; i++) {
                                    if (model[cars_in_order[i]].drone_type == HUMAN)
                                        j++;
                                }

                                for (i = my_place, k = 0; i < num_active_cars; i++) {
                                    if (model[cars_in_order[i]].drone_type == HUMAN)
                                        k++;
                                }

                                if (j >= k) {
                                    if (delta_dist < -150) {
                                        time_scale = linear_interp(-150, -300, delta_dist, min_boost, max_boost);
                                        scale = min_brake;
                                    } else {
                                        scale = linear_interp(-150, 0, delta_dist, min_brake, max_brake);
                                        time_scale = min_boost;
                                    }
                                } else {
                                    if (delta_dist2 < 150) {
                                        time_scale = linear_interp(150, 0, delta_dist2, min_boost, max_boost);
                                        scale = min_brake;
                                    } else {
                                        scale = linear_interp(500, 150, delta_dist2, max_brake, min_brake);
                                        time_scale = min_boost;
                                    }
                                }
                            }
                        }
                    }

                    if (coast_flag || end_game_flag)
                        time_scale = min_boost;

                    if (lap_flag && game_car[humans[num_humans - 1]].place > place[index]) {
                        scale = min_brake;
                        time_scale = min_boost;
                    }

                    temp1 = model[index].drone_scale;

                    if (temp1 > scale) {
                        temp1 -= .01f;
                        if (temp1 < scale)
                            temp1 = scale;
                    } else {
                        temp1 += .01f;
                        if (temp1 > scale)
                            temp1 = scale;
                    }

                    model[index].drone_scale = temp1;

                    temp1 = model[index].time_boost;

                    if (temp1 > time_scale) {
                        temp1 -= .01f;
                        if (temp1 < time_scale)
                            temp1 = time_scale;
                    } else {
                        temp1 += .01f;
                        if (temp1 > time_scale)
                            temp1 = time_scale;
                    }

                    model[index].time_boost = temp1;
                }
            }
        }
    }
}

#ifdef AD_FN
void render_large_objects(void)
{
    func_800DE860();
    func_800F9398();
}
#endif
