/**
 * collision.h - Collision detection and response for Rush 2049 N64
 *
 * Based on arcade game/collision.c
 * Handles car-car collisions using bounding sphere then box tests
 */

#ifndef COLLISION_H
#define COLLISION_H

#include "types.h"
#include "game/structs.h"

/* Collision algorithm types */
#define COLL_ALG_SOFTWALL   0   /* Soft wall collision */
#define COLL_ALG_HARDWALL   1   /* Hard wall collision */
#define COLL_ALG_CARBODY    2   /* Car body collision */

/* Collision constants */
#define MAXFORCE        4000.0f     /* Maximum collision force (lbs) */
#define MAX_VEL_SQ      (300*300)   /* Max velocity squared (fps^2) */
#define COL_SPRING_K    (-10000.0f) /* Spring constant for collisions */
#define COL_DAMP_K      (-50.0f)    /* Damping constant for collisions */

/* Collision data stored per-car */
typedef struct CollisionData {
    f32     colrad;             /* Collision radius (bounding sphere) */
    f32     BODYR[4][3];        /* Body corner positions (body coords) */
    f32     CENTERFORCE[3];     /* Center force vector */
    f32     BODYFORCE[4][3];    /* Force at each body corner */
    f32     peak_center_force[2][3]; /* Peak center forces */
    f32     peak_body_force[2][3];   /* Peak body forces */
    s8      collidable;         /* Is this car collidable? */
    s8      in_game;            /* Is this car in the game? */
    u8      pad[2];
} CollisionData;

/* External collision data */
extern CollisionData col_data[];

/* Initialization */
void init_collision(s32 car_index);
void init_all_collisions(void);

/* Main collision detection */
void collision(s32 car_index);
void check_all_collisions(void);

/* Collision tests */
s32 check_sphere_collision(s32 car1, s32 car2, f32 *dist_sq);
s32 check_body_collision(s32 car1, s32 car2);
s32 point_in_body(s32 car_index, f32 point[3]);

/* Collision response */
void set_collision_force(s32 car1, s32 car2, f32 force[3], f32 point[3]);
void set_center_force(s32 car_index, f32 force[3]);
void set_body_force(s32 car_index, s32 corner, f32 force[3]);

/* Coordinate transforms for collision */
void body_to_world(f32 body_pos[3], f32 world_pos[3], f32 uvs[3][3], f32 rwr[3]);
void world_to_body(f32 world_pos[3], f32 body_pos[3], f32 uvs[3][3], f32 rwr[3]);

/* Wall collisions */
void check_wall_collision(s32 car_index);

/* Utility */
f32 vec_dist_sq(f32 a[3], f32 b[3]);
void apply_collision_forces(s32 car_index);
void set_car_collidable(s32 car_index, s32 collidable);
void set_car_in_game(s32 car_index, s32 in_game);
void ForceApart(s32 car1, s32 car2, f32 dir[3]);
void set_collision_damage(s32 car_index);
f32 get_collision_radius(s32 car_index);
void set_collision_radius(s32 car_index, f32 radius);

#endif /* COLLISION_H */
