/**
 * collision.c - Collision detection and response for Rush 2049 N64
 *
 * Based on arcade game/collision.c
 * Handles car-car collisions using bounding sphere then box tests
 *
 * Algorithm:
 * 1. Check bounding sphere overlap (fast rejection)
 * 2. Check body corners of car1 against car2's bounding box
 * 3. If penetration detected, apply spring/damping forces
 *
 * Key functions mapped from arcade:
 * - init_collision -> collision structure initialization
 * - collision -> per-car collision detection
 * - setCollisionForce -> collision response calculation
 */

#include "types.h"
#include "game/collision.h"
#include "game/structs.h"

/* External OS functions */
extern u32 osGetCount(void);
extern u32 frame_counter;

/* External game state */
extern u8 gstate;
extern CarData car_array[];
extern GameStruct game_struct;
extern s32 num_active_cars;

/* External vector math */
extern f32 vec_dot(const f32 a[3], const f32 b[3]);
extern void vec_sub(const f32 a[3], const f32 b[3], f32 result[3]);
extern void vec_add(const f32 a[3], const f32 b[3], f32 result[3]);
extern void vec_scale(const f32 a[3], f32 scale, f32 result[3]);
extern void vec_copy(const f32 src[3], f32 dst[3]);

/* Collision data for all cars */
CollisionData col_data[MAX_CARS];

/* Default collision box dimensions (half-sizes in feet) */
static const f32 default_body_corners[4][3] = {
    {  2.0f, 0.0f,  4.5f },  /* Front right */
    { -2.0f, 0.0f,  4.5f },  /* Front left */
    { -2.0f, 0.0f, -4.5f },  /* Rear left */
    {  2.0f, 0.0f, -4.5f }   /* Rear right */
};

/* Default collision radius (bounding sphere) */
static const f32 default_colrad = 6.0f;

/**
 * init_collision - Initialize collision data for one car
 * Based on arcade: collision.c:init_collision()
 *
 * @param car_index Car index to initialize
 */
void init_collision(s32 car_index) {
    CollisionData *col;
    s32 i, j;

    if (car_index < 0 || car_index >= MAX_CARS) {
        return;
    }

    col = &col_data[car_index];

    /* Clear force vectors */
    for (i = 0; i < 3; i++) {
        col->CENTERFORCE[i] = 0.0f;
        col->peak_center_force[0][i] = 0.0f;
        col->peak_center_force[1][i] = 0.0f;
        col->peak_body_force[0][i] = 0.0f;
        col->peak_body_force[1][i] = 0.0f;
    }

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            col->BODYFORCE[i][j] = 0.0f;
            col->BODYR[i][j] = default_body_corners[i][j];
        }
    }

    col->colrad = default_colrad;
    col->collidable = 1;
    col->in_game = 1;
}

/**
 * init_all_collisions - Initialize collision for all cars
 */
void init_all_collisions(void) {
    s32 i;

    for (i = 0; i < MAX_CARS; i++) {
        init_collision(i);
    }
}

/**
 * vec_dist_sq - Calculate squared distance between two points
 *
 * @param a First point
 * @param b Second point
 * @return Squared distance
 */
f32 vec_dist_sq(f32 a[3], f32 b[3]) {
    f32 dx = a[0] - b[0];
    f32 dy = a[1] - b[1];
    f32 dz = a[2] - b[2];
    return dx*dx + dy*dy + dz*dz;
}

/**
 * body_to_world - Transform point from body coords to world coords
 *
 * @param body_pos Position in body coordinates
 * @param world_pos Output position in world coordinates
 * @param uvs Rotation matrix (3x3)
 * @param rwr World position of body origin
 */
void body_to_world(f32 body_pos[3], f32 world_pos[3], f32 uvs[3][3], f32 rwr[3]) {
    s32 i;

    for (i = 0; i < 3; i++) {
        world_pos[i] = rwr[i] +
                       body_pos[0] * uvs[0][i] +
                       body_pos[1] * uvs[1][i] +
                       body_pos[2] * uvs[2][i];
    }
}

/**
 * world_to_body - Transform point from world coords to body coords
 *
 * @param world_pos Position in world coordinates
 * @param body_pos Output position in body coordinates
 * @param uvs Rotation matrix (3x3)
 * @param rwr World position of body origin
 */
void world_to_body(f32 world_pos[3], f32 body_pos[3], f32 uvs[3][3], f32 rwr[3]) {
    f32 diff[3];
    s32 i;

    /* Get offset from body origin */
    for (i = 0; i < 3; i++) {
        diff[i] = world_pos[i] - rwr[i];
    }

    /* Dot with each unit vector row */
    body_pos[0] = diff[0] * uvs[0][0] + diff[1] * uvs[0][1] + diff[2] * uvs[0][2];
    body_pos[1] = diff[0] * uvs[1][0] + diff[1] * uvs[1][1] + diff[2] * uvs[1][2];
    body_pos[2] = diff[0] * uvs[2][0] + diff[1] * uvs[2][1] + diff[2] * uvs[2][2];
}

/**
 * check_sphere_collision - Check if two cars' bounding spheres overlap
 * Based on arcade: first check in collision()
 *
 * @param car1 First car index
 * @param car2 Second car index
 * @param dist_sq Output squared distance between centers
 * @return 1 if spheres overlap, 0 otherwise
 */
s32 check_sphere_collision(s32 car1, s32 car2, f32 *dist_sq) {
    CollisionData *c1, *c2;
    f32 dsq;
    f32 combined_rad;

    c1 = &col_data[car1];
    c2 = &col_data[car2];

    /* Calculate squared distance between car centers */
    dsq = vec_dist_sq(car_array[car1].RWR, car_array[car2].RWR);

    if (dist_sq) {
        *dist_sq = dsq;
    }

    /* Check if spheres overlap */
    combined_rad = c1->colrad + c2->colrad;
    if (dsq > combined_rad * combined_rad) {
        return 0;  /* No overlap */
    }

    return 1;  /* Spheres overlap */
}

/**
 * point_in_body - Check if point is inside car's bounding box
 * Based on arcade: PointInBody()
 *
 * @param car_index Car to test against
 * @param point World position to test
 * @return 1 if point is inside, 0 otherwise
 */
s32 point_in_body(s32 car_index, f32 point[3]) {
    CollisionData *col;
    CarData *car;
    f32 body_point[3];
    f32 half_x, half_y, half_z;

    col = &col_data[car_index];
    car = &car_array[car_index];

    /* Transform point to body coordinates */
    world_to_body(point, body_point, car->dr_uvs, car->RWR);

    /* Get half-sizes from body corners */
    half_x = col->BODYR[0][0];  /* Right side = positive X */
    half_y = 2.0f;              /* Fixed height */
    half_z = col->BODYR[0][2];  /* Front = positive Z */

    /* Check if inside box */
    if (body_point[0] < -half_x || body_point[0] > half_x) {
        return 0;
    }
    if (body_point[1] < -half_y || body_point[1] > half_y) {
        return 0;
    }
    if (body_point[2] < -half_z || body_point[2] > half_z) {
        return 0;
    }

    return 1;
}

/**
 * set_collision_force - Calculate and apply collision response
 * Based on arcade: setFBCollisionForce()
 *
 * @param car1 First car index
 * @param car2 Second car index
 * @param vec Direction vector (from car2 to car1)
 * @param point Collision point
 */
void set_collision_force(s32 car1, s32 car2, f32 force[3], f32 point[3]) {
    CollisionData *c1, *c2;
    CarData *d1, *d2;
    f32 rel_vel[3];
    f32 vel_along_normal;
    f32 force_mag;
    f32 normal[3];
    f32 dist;
    s32 i;

    c1 = &col_data[car1];
    c2 = &col_data[car2];
    d1 = &car_array[car1];
    d2 = &car_array[car2];

    /* Calculate relative velocity */
    for (i = 0; i < 3; i++) {
        rel_vel[i] = d1->RWV[i] - d2->RWV[i];
    }

    /* Normalize direction vector */
    dist = force[0]*force[0] + force[1]*force[1] + force[2]*force[2];
    if (dist > 0.001f) {
        dist = 1.0f / sqrtf(dist);
        for (i = 0; i < 3; i++) {
            normal[i] = force[i] * dist;
        }
    } else {
        normal[0] = 0.0f;
        normal[1] = 0.0f;
        normal[2] = 1.0f;
    }

    /* Calculate velocity component along collision normal */
    vel_along_normal = rel_vel[0] * normal[0] +
                       rel_vel[1] * normal[1] +
                       rel_vel[2] * normal[2];

    /* Spring + damping force */
    /* F = Ks * penetration + Kd * relative_velocity */
    force_mag = COL_SPRING_K * 0.5f + COL_DAMP_K * vel_along_normal;

    /* Clamp force magnitude */
    if (force_mag > MAXFORCE) {
        force_mag = MAXFORCE;
    }
    if (force_mag < -MAXFORCE) {
        force_mag = -MAXFORCE;
    }

    /* Apply force to center */
    for (i = 0; i < 3; i++) {
        c1->CENTERFORCE[i] += normal[i] * force_mag;
    }
}

/**
 * collision - Main collision detection for one car
 * Based on arcade: collision.c:collision()
 *
 * @param car_index Car to check collisions for
 */
void collision(s32 car_index) {
    CollisionData *col, *col2;
    CarData *car, *car2;
    f32 vec[3], pos[3], posr[3];
    f32 dsq;
    s32 other, i;

    col = &col_data[car_index];
    car = &car_array[car_index];

    /* Check if this car is collidable */
    if (!col->collidable) {
        return;
    }

    /* Velocity sanity check */
    dsq = car->RWV[0]*car->RWV[0] + car->RWV[1]*car->RWV[1] + car->RWV[2]*car->RWV[2];
    if (dsq > MAX_VEL_SQ) {
        return;
    }

    /* Check against all other cars */
    for (other = 0; other < num_active_cars; other++) {
        if (other == car_index) {
            continue;  /* Don't collide with self */
        }

        col2 = &col_data[other];
        car2 = &car_array[other];

        /* Check if other car is in game and collidable */
        if (!col2->in_game || !col2->collidable) {
            continue;
        }

        /* Velocity sanity check for other car */
        dsq = car2->RWV[0]*car2->RWV[0] + car2->RWV[1]*car2->RWV[1] + car2->RWV[2]*car2->RWV[2];
        if (dsq > MAX_VEL_SQ) {
            continue;
        }

        /* Phase 1: Bounding sphere test */
        if (!check_sphere_collision(car_index, other, &dsq)) {
            continue;  /* Spheres don't overlap */
        }

        /* Calculate vector from other car to this car */
        for (i = 0; i < 3; i++) {
            vec[i] = car->RWR[i] - car2->RWR[i];
        }

        /* Phase 2: Check body corners */
        for (i = 0; i < 4; i++) {
            /* Transform body corner to world coords */
            body_to_world(col->BODYR[i], pos, car->dr_uvs, car->RWR);

            /* Check if corner is inside other car's body */
            if (point_in_body(other, pos)) {
                /* Collision detected! Apply forces */
                set_collision_force(car_index, other, vec, pos);
                return;
            }
        }
    }
}

/**
 * check_all_collisions - Check collisions for all active cars
 */
void check_all_collisions(void) {
    s32 i;

    /* Clear all forces first */
    for (i = 0; i < num_active_cars; i++) {
        col_data[i].CENTERFORCE[0] = 0.0f;
        col_data[i].CENTERFORCE[1] = 0.0f;
        col_data[i].CENTERFORCE[2] = 0.0f;
    }

    /* Check each car for collisions */
    for (i = 0; i < num_active_cars; i++) {
        collision(i);
    }
}

/**
 * apply_collision_forces - Apply accumulated collision forces to car
 *
 * @param car_index Car to apply forces to
 */
void apply_collision_forces(s32 car_index) {
    CollisionData *col;
    CarData *car;
    f32 dt = 1.0f / 60.0f;  /* Fixed timestep */
    f32 mass = 100.0f;      /* Car mass in slugs (placeholder) */
    f32 accel[3];
    s32 i;

    col = &col_data[car_index];
    car = &car_array[car_index];

    /* F = ma, so a = F/m */
    for (i = 0; i < 3; i++) {
        accel[i] = col->CENTERFORCE[i] / mass;
        car->RWV[i] += accel[i] * dt;
    }
}

/**
 * set_car_collidable - Set car's collision state
 *
 * @param car_index Car index
 * @param collidable 1 for collidable, 0 for not
 */
void set_car_collidable(s32 car_index, s32 collidable) {
    if (car_index >= 0 && car_index < MAX_CARS) {
        col_data[car_index].collidable = collidable ? 1 : 0;
    }
}

/**
 * set_car_in_game - Set car's in-game state
 *
 * @param car_index Car index
 * @param in_game 1 if in game, 0 if not
 */
void set_car_in_game(s32 car_index, s32 in_game) {
    if (car_index >= 0 && car_index < MAX_CARS) {
        col_data[car_index].in_game = in_game ? 1 : 0;
    }
}

/**
 * ForceApart - Force two overlapping cars apart
 * Based on arcade: collision.c:ForceApart()
 *
 * Called when cars are stuck inside each other.
 * Applies a large separation force.
 *
 * @param car1 First car index
 * @param car2 Second car index
 * @param dir Direction vector from car2 to car1
 */
void ForceApart(s32 car1, s32 car2, f32 dir[3]) {
    CollisionData *c1;
    CarData *d1;
    f32 invdist, force;
    f32 temp[3];
    s32 i;

    c1 = &col_data[car1];
    d1 = &car_array[car1];

    /* Normalize direction vector */
    invdist = dir[0]*dir[0] + dir[1]*dir[1] + dir[2]*dir[2];
    if (invdist < 0.0001f) {
        /* If coincident centers, force in Y direction based on slot order */
        dir[0] = 0.0f;
        dir[1] = (car1 > car2) ? 1.0f : -1.0f;
        dir[2] = 0.0f;
        invdist = 1.0f;
    } else {
        invdist = 1.0f / sqrtf(invdist);
        for (i = 0; i < 3; i++) {
            dir[i] = dir[i] * invdist;
        }
    }

    /* Large separation force */
    force = 100000.0f;

    for (i = 0; i < 3; i++) {
        temp[i] = force * dir[i];
    }

    /* Transform to body coordinates and apply */
    world_to_body(temp, c1->CENTERFORCE, d1->dr_uvs, d1->RWR);
}

/**
 * set_center_force - Set center force for a car
 *
 * @param car_index Car index
 * @param force Force vector to set
 */
void set_center_force(s32 car_index, f32 force[3]) {
    if (car_index >= 0 && car_index < MAX_CARS) {
        col_data[car_index].CENTERFORCE[0] = force[0];
        col_data[car_index].CENTERFORCE[1] = force[1];
        col_data[car_index].CENTERFORCE[2] = force[2];
    }
}

/**
 * set_body_force - Set force at a body corner
 *
 * @param car_index Car index
 * @param corner Corner index (0-3)
 * @param force Force vector to set
 */
void set_body_force(s32 car_index, s32 corner, f32 force[3]) {
    if (car_index >= 0 && car_index < MAX_CARS && corner >= 0 && corner < 4) {
        col_data[car_index].BODYFORCE[corner][0] = force[0];
        col_data[car_index].BODYFORCE[corner][1] = force[1];
        col_data[car_index].BODYFORCE[corner][2] = force[2];
    }
}

/**
 * set_collision_damage - Calculate and apply damage from collision
 * Based on arcade: collision.c:setCollisionDamage()
 *
 * @param car_index Car that was hit
 */
void set_collision_damage(s32 car_index) {
    CollisionData *col;
    f32 force_mag;

    col = &col_data[car_index];

    /* Calculate magnitude of collision force */
    force_mag = sqrtf(col->CENTERFORCE[0] * col->CENTERFORCE[0] +
                      col->CENTERFORCE[1] * col->CENTERFORCE[1] +
                      col->CENTERFORCE[2] * col->CENTERFORCE[2]);

    /* Update peak force tracking */
    if (force_mag > col->peak_center_force[0][0]) {
        col->peak_center_force[1][0] = col->peak_center_force[0][0];
        col->peak_center_force[0][0] = force_mag;
    } else if (force_mag > col->peak_center_force[1][0]) {
        col->peak_center_force[1][0] = force_mag;
    }

    /* Damage would be applied to car state here based on force magnitude */
}

/**
 * get_collision_radius - Get collision radius for a car
 *
 * @param car_index Car index
 * @return Collision radius
 */
f32 get_collision_radius(s32 car_index) {
    if (car_index >= 0 && car_index < MAX_CARS) {
        return col_data[car_index].colrad;
    }
    return 0.0f;
}

/**
 * set_collision_radius - Set collision radius for a car
 *
 * @param car_index Car index
 * @param radius New collision radius
 */
void set_collision_radius(s32 car_index, f32 radius) {
    if (car_index >= 0 && car_index < MAX_CARS) {
        col_data[car_index].colrad = radius;
    }
}
