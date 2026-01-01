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
#include "game/vecmath.h"
#include <math.h>

/* External OS functions */
extern u32 osGetCount(void);
extern u32 frame_counter;

/* External game state */
extern u8 gstate;
extern CarData car_array[];
extern GameStruct game_struct;
extern s32 num_active_cars;

/* External vector math (local declarations for existing functions) */
extern f32 vec_dot(const f32 a[3], const f32 b[3]);
extern void vec_sub(const f32 a[3], const f32 b[3], f32 result[3]);
extern void vec_add(const f32 a[3], const f32 b[3], f32 result[3]);
extern void vec_scale(const f32 a[3], f32 scale, f32 result[3]);
extern void vec_copy(const f32 src[3], f32 dst[3]);

/* Collision data for all cars */
CollisionData col_data[MAX_CARS];

/* Model array (physics state for all cars) - arcade name for car_physics */
CarPhysics model[MAX_LINKS];

/* Damage appearance masks and shifts (arcade collision.c)
 * Damage level is stored in the appearance field as 2 bits per quadrant.
 * Quads: 0=FR, 1=FL, 2=RR, 3=RL, 4=Top
 */
const u32 gDamageMask[5] = {
    0x0003,     /* Quad 0 (front-right): bits 0-1 */
    0x000C,     /* Quad 1 (front-left):  bits 2-3 */
    0x0030,     /* Quad 2 (rear-right):  bits 4-5 */
    0x00C0,     /* Quad 3 (rear-left):   bits 6-7 */
    0x0300      /* Quad 4 (top):         bits 8-9 */
};

const s16 gDamageShift[5] = {
    0,          /* Quad 0: shift 0 */
    2,          /* Quad 1: shift 2 */
    4,          /* Quad 2: shift 4 */
    6,          /* Quad 3: shift 6 */
    8           /* Quad 4: shift 8 */
};

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
f32 vec_dist_sq(const f32 a[3], const f32 b[3]) {
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

/* ========================================================================
 * Arcade-compatible function aliases (collision.c)
 * ======================================================================== */

/**
 * PointInBody - Check if point is inside car's bounding box (arcade name)
 * Wrapper for point_in_body()
 */
s32 PointInBody(s32 car_index, f32 pt[3]) {
    return point_in_body(car_index, pt);
}

/**
 * setFBCollisionForce - Set front/back collision force (arcade name)
 * Based on arcade: collision.c:setFBCollisionForce()
 */
void setFBCollisionForce(s32 m, s32 m1, s32 m2, f32 dir[3], f32 pos[3]) {
    set_collision_force(m, m2, dir, pos);
}

/**
 * setCollisionDamage - Set collision damage (arcade name)
 * Wrapper for set_collision_damage()
 */
void setCollisionDamage(s32 car_index) {
    set_collision_damage(car_index);
}

/**
 * simpleCollForce - Simple collision force calculation (arcade name)
 * Based on arcade: collision.c:simpleCollForce()
 *
 * @param car1 Car to apply force to
 * @param car2 Other car
 * @param dir Direction vector from car1 to car2
 */
void simpleCollForce(s32 car1, s32 car2, f32 dir[3]) {
    CollisionData *c1;
    CarData *d1, *d2;
    f32 invdist, vin, force;
    f32 rvec[3];
    s32 i;

    c1 = &col_data[car1];
    d1 = &car_array[car1];
    d2 = &car_array[car2];

    /* Normalize direction vector */
    invdist = dir[0]*dir[0] + dir[1]*dir[1] + dir[2]*dir[2];
    if (invdist < 0.0001f) {
        /* Coincident centers, bounce up */
        dir[0] = 0.0f;
        dir[1] = 0.0f;
        dir[2] = -1.0f;
        invdist = 1.0f;
    } else {
        invdist = 1.0f / sqrtf(invdist);
        for (i = 0; i < 3; i++) {
            dir[i] = dir[i] * invdist;
        }
    }

    /* Get relative velocity between car centers */
    for (i = 0; i < 3; i++) {
        rvec[i] = d2->RWV[i] - d1->RWV[i];
    }
    vin = rvec[0]*dir[0] + rvec[1]*dir[1] + rvec[2]*dir[2];

    /* Calculate force */
    force = (vin + 5.0f) * 80000.0f;
    if (force < 0.0f) {
        force = 0.0f;
    } else if (force > 100000.0f) {
        force = 100000.0f;
    }

    /* Apply force to center */
    for (i = 0; i < 3; i++) {
        rvec[i] = force * dir[i];
    }
    world_to_body(rvec, c1->CENTERFORCE, d1->dr_uvs, d1->RWR);
}

/**
 * setCenterForce - Set center collision force (arcade name)
 * Based on arcade: collision.c:setCenterForce()
 */
void setCenterForce(s32 m, s32 m1, s32 m2, f32 dir[3], f32 pos[3]) {
    CollisionData *c;
    CarData *d, *d2;
    f32 del[6], rvec[3], dist, invdist, v1, force;
    s32 i, side, mul, comp;

    c = &col_data[m];
    d = &car_array[m];
    d2 = &car_array[m2];

    /* Find closest side (inside = negative) */
    del[0] = pos[0] - c->BODYR[0][0];
    del[1] = pos[1] - c->BODYR[0][1];
    del[2] = -10.0f;
    del[3] = c->BODYR[3][0] - pos[0];
    del[4] = c->BODYR[3][1] - pos[1];
    del[5] = -10.0f;

    /* Find closest side */
    dist = -10.0f;
    side = 0;
    for (i = 0; i < 6; i++) {
        if ((del[i] < 0.0f) && (del[i] > dist)) {
            dist = del[i];
            side = i;
        }
    }

    mul = (side < 3) ? 1 : -1;
    comp = (side < 3) ? side : (side - 3);

    /* Normalize direction vector */
    invdist = dir[0]*dir[0] + dir[1]*dir[1] + dir[2]*dir[2];
    if (invdist < 0.0001f) {
        dir[0] = 0.0f;
        dir[1] = 0.0f;
        dir[2] = -1.0f;
        invdist = 1.0f;
    } else {
        invdist = 1.0f / sqrtf(invdist);
        for (i = 0; i < 3; i++) {
            dir[i] = dir[i] * invdist;
        }
    }

    /* Get relative velocity */
    for (i = 0; i < 3; i++) {
        rvec[i] = d2->RWV[i] - d->RWV[i];
    }
    v1 = rvec[0]*dir[0] + rvec[1]*dir[1] + rvec[2]*dir[2];

    /* Calculate force */
    v1 += 5.0f;
    force = (v1 > 0.0f) ? v1 * 100.0f * 50.0f : 0.0f;  /* mass * 50 */

    for (i = 0; i < 3; i++) {
        pos[i] = force * dir[i];
    }
    world_to_body(pos, c->CENTERFORCE, d->dr_uvs, d->RWR);
}

/**
 * collForceNormal - Calculate collision force based on algorithm (arcade name)
 * Based on arcade: collision.c:collForceNormal()
 */
f32 collForceNormal(s32 car_index, u32 coll_alg, f32 din, f32 vin) {
    f32 force, vout;
    f32 mass = 100.0f;  /* Car mass in slugs */
    f32 idt = 60.0f;    /* Inverse dt (1/60 fps) */

    switch (coll_alg) {
        case COLL_ALG_SOFTWALL:
            if (vin < 0.0f) {
                /* Compression */
                force = (din * g_Kcs) - (vin * vin * g_Kcd);
            } else {
                /* Rebound */
                force = (din * g_Krs) + (vin * vin * g_Krd);
            }
            force *= 2.0f;
            if (force < 0.0f) {
                force = 0.0f;
            }
            break;

        case COLL_ALG_HARDWALL:
            vout = (din * 4.0f) + 1.0f;  /* 5fps @ 1 foot */
            force = (vin + vout) * mass * 0.5f * idt;
            if (force < 0.0f) {
                force = 0.0f;
            }
            break;

        case COLL_ALG_CARBODY:
            force = 50000.0f;
            if (force < 0.0f) {
                force = 0.0f;
            }
            break;

        default:
            force = 0.0f;
            break;
    }

    return force;
}

/**
 * distributeForce - Distribute force between two corners (arcade name)
 * Based on arcade: collision.c:distributeForce()
 */
void distributeForce(f32 force, f32 offset, f32 width, f32 *fpos, f32 *fneg) {
    f32 interp_val;

    interp_val = (width + offset) / (width * 2.0f);
    if (interp_val > 1.0f) {
        interp_val = 1.0f;
    } else if (interp_val < 0.0f) {
        interp_val = 0.0f;
    }
    *fpos = interp_val * force;
    interp_val = 1.0f - interp_val;
    *fneg = interp_val * force;
}

/**
 * CollForceMineIn - Force when my corner is in other car (arcade name)
 * Based on arcade: collision.c:CollForceMineIn()
 */
void CollForceMineIn(s32 car1, s32 car2, f32 vec[3], s32 corner) {
    CollisionData *c1, *c2;
    CarData *d1, *d2;
    f32 del[6], rvec[3], dist, mul, v1, force;
    s32 i, side, dir_idx;

    c1 = &col_data[car1];
    c2 = &col_data[car2];
    d1 = &car_array[car1];
    d2 = &car_array[car2];

    /* Find closest side */
    del[0] = vec[0] - c2->BODYR[0][0];
    del[1] = vec[1] - c2->BODYR[0][1];
    del[2] = -10.0f;
    del[3] = c2->BODYR[3][0] - vec[0];
    del[4] = c2->BODYR[3][1] - vec[1];
    del[5] = -10.0f;

    dist = -10.0f;
    side = 0;
    for (i = 0; i < 6; i++) {
        if ((del[i] < 0.0f) && (del[i] > dist)) {
            dist = del[i];
            side = i;
        }
    }

    mul = (side < 3) ? 1.0f : -1.0f;
    dir_idx = (side < 3) ? side : (side - 3);

    /* Get relative velocity */
    for (i = 0; i < 3; i++) {
        vec[i] = d1->RWV[i] - d2->RWV[i];
    }
    world_to_body(vec, rvec, d2->dr_uvs, d2->RWR);
    v1 = rvec[dir_idx] * mul;

    /* Calculate force */
    force = collForceNormal(car1, COLL_ALG_SOFTWALL, dist, v1);
    for (i = 0; i < 3; i++) {
        vec[i] = (i == dir_idx) ? (force * mul) : 0.0f;
    }
    body_to_world(vec, rvec, d2->dr_uvs, d2->RWR);
    world_to_body(rvec, c1->BODYFORCE[corner], d1->dr_uvs, d1->RWR);
}

/**
 * CollForceOtherIn - Force when other car's corner is in me (arcade name)
 * Based on arcade: collision.c:CollForceOtherIn()
 */
void CollForceOtherIn(s32 car1, s32 car2, f32 vec[3], s32 corner) {
    CollisionData *c1;
    CarData *d1, *d2;
    f32 del[6], rvec[3], vvec[3], dist, mul, v1, force;
    s32 i, side, dir_idx;

    c1 = &col_data[car1];
    d1 = &car_array[car1];
    d2 = &car_array[car2];

    /* Find closest side */
    del[0] = vec[0] - c1->BODYR[0][0];
    del[1] = vec[1] - c1->BODYR[0][1];
    del[2] = -10.0f;
    del[3] = c1->BODYR[3][0] - vec[0];
    del[4] = c1->BODYR[3][1] - vec[1];
    del[5] = -10.0f;

    dist = -10.0f;
    side = 0;
    for (i = 0; i < 6; i++) {
        if ((del[i] < 0.0f) && (del[i] > dist)) {
            dist = del[i];
            side = i;
        }
    }

    mul = (side < 3) ? 1.0f : -1.0f;
    dir_idx = (side < 3) ? side : (side - 3);

    /* Get relative velocity */
    for (i = 0; i < 3; i++) {
        vvec[i] = d2->RWV[i] - d1->RWV[i];
    }
    world_to_body(vvec, rvec, d1->dr_uvs, d1->RWR);
    v1 = rvec[dir_idx] * mul;

    /* Calculate force */
    force = collForceNormal(car1, COLL_ALG_SOFTWALL, dist, v1);

    /* Distribute force to body corners */
    if (side == 0) {
        distributeForce(-force, vec[1], c1->BODYR[0][1],
                        &c1->BODYFORCE[0][0], &c1->BODYFORCE[1][0]);
    } else if (side == 1) {
        distributeForce(-force, vec[0], c1->BODYR[0][0],
                        &c1->BODYFORCE[0][1], &c1->BODYFORCE[2][1]);
    } else if (side == 3) {
        distributeForce(force, vec[1], c1->BODYR[0][1],
                        &c1->BODYFORCE[2][0], &c1->BODYFORCE[3][0]);
    } else if (side == 4) {
        distributeForce(force, vec[0], c1->BODYR[0][0],
                        &c1->BODYFORCE[1][1], &c1->BODYFORCE[3][1]);
    }
}

/* ========================================================================
 * MODELDAT pointer-based arcade functions (collision.c)
 * These match the arcade function signatures exactly
 * ======================================================================== */

/* Include physics header for MODELDAT/CarPhysics definition */
#include "game/physics.h"

/* External model array (arcade global) */
extern CarPhysics model[];

/**
 * init_collision_m - Initialize collision for a MODELDAT pointer
 * Based on arcade: collision.c:init_collision()
 */
void init_collision_m(MODELDAT *m) {
    s32 i, j;

    /* Clear center force */
    for (i = 0; i < 3; i++) {
        m->CENTERFORCE[i] = 0.0f;
        m->peak_center_force[0][i] = 0.0f;
        m->peak_center_force[1][i] = 0.0f;
        m->peak_body_force[0][i] = 0.0f;
        m->peak_body_force[1][i] = 0.0f;
    }

    /* Clear body forces */
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 3; j++) {
            m->BODYFORCE[i][j] = 0.0f;
        }
    }
}

/**
 * collision_m - Main collision detection for a MODELDAT pointer
 * Based on arcade: collision.c:collision()
 */
void collision_m(MODELDAT *m) {
    MODELDAT *m2;
    f32 vec[3], pos[3], posr[3], dsq;
    s32 mnum, i;

    /* Check if car is collidable */
    if (m->collidable == 0) {
        return;
    }

    /* Velocity sanity check */
    dsq = m->V[0]*m->V[0] + m->V[1]*m->V[1] + m->V[2]*m->V[2];
    if (dsq > MAX_VEL_SQ) {
        return;
    }

    /* Check against all other cars */
    for (mnum = 0, m2 = &model[0]; mnum < MAX_LINKS; mnum++, m2++) {
        /* Don't test collision with yourself */
        if (m2 == m) {
            continue;
        }

        /* Make sure car is real */
        if (!m2->in_game) {
            continue;
        }

        /* Check if car is collidable */
        if (m2->collidable == 0) {
            continue;
        }

        /* Velocity sanity check */
        dsq = m2->V[0]*m2->V[0] + m2->V[1]*m2->V[1] + m2->V[2]*m2->V[2];
        if (dsq > MAX_VEL_SQ) {
            continue;
        }

        /* See if bounding spheres intersect */
        vecsub(m->reckon.RWR, m2->reckon.RWR, vec);
        dsq = 0.0f;
        for (i = 0; i < 3; i++) {
            dsq += vec[i] * vec[i];
        }
        if (dsq > (m->colrad + m2->colrad) * (m->colrad + m2->colrad)) {
            continue;  /* No overlap */
        }

        /* Test if our corners in other */
        for (i = 0; i < 4; i++) {
            fbodtorw(m->BODYR[i], pos, m->reckon.UV[0]);
            vecadd(pos, m->reckon.RWR, pos);
            vecsub(pos, m2->reckon.RWR, pos);
            frwtobod(pos, posr, m2->reckon.UV[0]);
            if (PointInBody_m(m2, posr)) {
                setFBCollisionForce_m(m, m, m2, vec, posr);
                return;
            }
        }

        /* Test if other car corners in */
        for (i = 0; i < 4; i++) {
            fbodtorw(m2->BODYR[i], pos, m2->reckon.UV[0]);
            vecadd(pos, m2->reckon.RWR, pos);
            vecsub(pos, m->reckon.RWR, pos);
            frwtobod(pos, posr, m->reckon.UV[0]);
            if (PointInBody_m(m, posr)) {
                setFBCollisionForce_m(m, m2, m, vec, posr);
                return;
            }
        }
    }
}

/**
 * PointInBody_m - Check if point is inside body bounding box
 * Based on arcade: collision.c:PointInBody()
 */
s32 PointInBody_m(MODELDAT *m, f32 pt[3]) {
    if ((pt[0] > m->BODYR[0][0]) || (pt[0] < m->BODYR[3][0])) return 0;
    if ((pt[1] > m->BODYR[0][1]) || (pt[1] < m->BODYR[3][1])) return 0;
    if ((pt[2] > COLL_BOT_DIST) || (pt[2] < COLL_TOP_DIST)) return 0;
    return 1;
}

/**
 * ForceApart_m - Force two overlapping cars apart
 * Based on arcade: collision.c:ForceApart()
 */
void ForceApart_m(MODELDAT *m, MODELDAT *m1, MODELDAT *m2, f32 dir[3], f32 pos[3]) {
    MODELDAT *mother;
    f32 invdist, force, temp[3];
    s32 i;

    /* Normalize direction to apply force */
    invdist = dir[0]*dir[0] + dir[1]*dir[1] + dir[2]*dir[2];
    if (invdist < 0.0001f) {
        /* If coincident centers, force in opposite directions */
        mother = (m == m1) ? m2 : m1;
        dir[0] = dir[2] = 0.0f;
        dir[1] = (m->slot > mother->slot) ? 1.0f : -1.0f;
        invdist = 1.0f;
    } else {
        invdist = 1.0f / sqrtf(invdist);
        for (i = 0; i < 3; i++) {
            dir[i] = dir[i] * invdist;
        }
    }

    force = 100000.0f;

    for (i = 0; i < 3; i++) {
        temp[i] = force * dir[i];
    }
    rwtobod(temp, m->CENTERFORCE, &m->UV);
}

/**
 * setFBCollisionForce_m - Set front/back collision force
 * Based on arcade: collision.c:setFBCollisionForce()
 */
void setFBCollisionForce_m(MODELDAT *m, MODELDAT *m1, MODELDAT *m2, f32 dir[3], f32 pos[3]) {
    f32 dist[6], force[3], temp[3], rvel[3], cent[3];
    s32 i;
    s32 beside, behind;

    /* 0=front,1=back,2=right,3=left,4=bottom,5=top */
    dist[0] = m2->BODYR[0][0] - pos[0];
    dist[1] = pos[0] - m2->BODYR[3][0];
    dist[2] = m2->BODYR[0][1] - pos[1];
    dist[3] = pos[1] - m2->BODYR[3][1];
    dist[4] = COLL_BOT_DIST - pos[2];
    dist[5] = pos[2] - COLL_TOP_DIST;

    /* Get rel center of m1 */
    vecsub(m1->reckon.RWR, m2->reckon.RWR, temp);
    frwtobod(temp, cent, m2->reckon.UV[0]);
    beside = ((cent[0] < m2->BODYR[0][0]) && (cent[0] > m2->BODYR[3][0]));
    behind = ((cent[1] < m2->BODYR[0][1]) && (cent[1] > m2->BODYR[3][1]));

    if (beside && behind) {
        /* Major overlap - blast apart */
        ForceApart_m(m, m1, m2, dir, pos);
        setCollisionDamage_m(m);
        return;
    }

    /* Get rel velocity from m1 to m2 into m2 frame */
    vecsub(m1->reckon.base_RWV, m2->reckon.base_RWV, temp);
    frwtobod(temp, rvel, m2->reckon.UV[0]);

    /* Get x (front/back) force for m2 */
    if (beside) {
        force[0] = 0.0f;
    } else {
        force[0] = rvel[0] * 4000.0f;
        if ((pos[0] > 0.0f) && (force[0] > -8000.0f)) {
            force[0] = -8000.0f;
        } else if ((pos[0] < 0.0f) && (force[0] < 8000.0f)) {
            force[0] = 8000.0f;
        }
    }

    /* Get y (side) force for m2 */
    if (behind) {
        force[1] = 0.0f;
    } else {
        force[1] = rvel[1] * 4000.0f;
        if ((pos[1] > 0.0f) && (force[1] > -8000.0f)) {
            force[1] = -8000.0f;
        } else if ((pos[1] < 0.0f) && (force[1] < 8000.0f)) {
            force[1] = 8000.0f;
        }
    }

    /* Get z (up/down) force for m2 */
    force[2] = rvel[2] * 4000.0f;

    /* Get forces into correct frame of ref */
    if (m == m2) {
        for (i = 0; i < 3; i++) {
            m->CENTERFORCE[i] = force[i];
        }
    } else {
        for (i = 0; i < 3; i++) {
            force[i] = -force[i];
        }
        fbodtorw(force, temp, m2->reckon.UV[0]);
        rwtobod(temp, m->CENTERFORCE, &m->UV);
    }

    setCollisionDamage_m(m);
}

/**
 * setCollisionDamage_m - Calculate and set collision damage
 * Based on arcade: collision.c:setCollisionDamage()
 */
void setCollisionDamage_m(MODELDAT *m) {
    s16 i, quad, damage, olddamage;
    f32 fsq, f[3], af[3];

    /* Figure out quad and damage level */
    fsq = 0.0f;
    for (i = 0; i < 3; i++) {
        f[i] = m->CENTERFORCE[i];
        af[i] = (f[i] < 0.0f) ? -f[i] : f[i];
        fsq += f[i] * f[i];
    }

    /* Determine damage level */
    if (fsq >= DAMAGE2_FORCE * DAMAGE2_FORCE) {
        damage = 2;
    } else if (fsq >= DAMAGE1_FORCE * DAMAGE1_FORCE) {
        damage = 1;
    } else {
        return;  /* No damage */
    }

    /* Determine which corner */
    if ((f[2] > af[0]) && (f[2] > af[1])) {
        quad = 4;  /* Top */
    } else if (f[0] > 0.0f) {
        quad = (f[1] > 0.0f) ? 3 : 2;  /* Back */
    } else {
        quad = (f[1] > 0.0f) ? 1 : 0;  /* Front */
    }

    /* If more damage, update appearance */
    olddamage = (m->appearance & gDamageMask[quad]) >> gDamageShift[quad];
    if (damage > olddamage) {
        m->appearance &= ~gDamageMask[quad];
        m->appearance |= (damage << gDamageShift[quad]);
    }
}

/**
 * simpleCollForce_m - Simple collision force calculation
 * Based on arcade: collision.c:simpleCollForce()
 */
void simpleCollForce_m(MODELDAT *m, MODELDAT *m2, f32 dir[3]) {
    f32 invdist, vin, force, rvec[3];
    s32 i;

    /* Normalize direction */
    invdist = dir[0]*dir[0] + dir[1]*dir[1] + dir[2]*dir[2];
    if (invdist < 0.0001f) {
        dir[0] = dir[1] = 0.0f;
        dir[2] = -1.0f;
        invdist = 1.0f;
    } else {
        invdist = 1.0f / sqrtf(invdist);
        for (i = 0; i < 3; i++) {
            dir[i] = dir[i] * invdist;
        }
    }

    /* Get relative velocity */
    vecsub(m2->reckon.base_RWV, m->reckon.base_RWV, rvec);
    vin = dotprod(rvec, dir);

    /* Calculate force */
    force = (vin + 5.0f) * 80000.0f;
    if (force < 0.0f) {
        force = 0.0f;
    } else if (force > 100000.0f) {
        force = 100000.0f;
    }

    for (i = 0; i < 3; i++) {
        rvec[i] = force * dir[i];
    }
    rwtobod(rvec, m->CENTERFORCE, &m->UV);
}

/**
 * setCenterForce_m - Set center force from collision
 * Based on arcade: collision.c:setCenterForce()
 */
void setCenterForce_m(MODELDAT *m, MODELDAT *m1, MODELDAT *m2, f32 dir[3], f32 pos[3]) {
    f32 del[6], rvec[3], dist, invdist, v1, force;
    s32 i, side, mul, comp;

    /* Find closest side */
    del[0] = pos[0] - m2->BODYR[0][0];
    del[1] = pos[1] - m2->BODYR[0][1];
    del[2] = -10.0f;
    del[3] = m2->BODYR[3][0] - pos[0];
    del[4] = m2->BODYR[3][1] - pos[1];
    del[5] = -10.0f;

    dist = -10.0f;
    side = 0;
    for (i = 0; i < 6; i++) {
        if ((del[i] < 0.0f) && (del[i] > dist)) {
            dist = del[i];
            side = i;
        }
    }

    mul = (side < 3) ? 1 : -1;
    comp = (side < 3) ? side : (side - 3);

    /* Normalize direction vector */
    invdist = dir[0]*dir[0] + dir[1]*dir[1] + dir[2]*dir[2];
    if (invdist < 0.0001f) {
        dir[0] = dir[1] = 0.0f;
        dir[2] = -1.0f;
        invdist = 1.0f;
    } else {
        invdist = 1.0f / sqrtf(invdist);
        for (i = 0; i < 3; i++) {
            dir[i] = dir[i] * invdist;
        }
    }

    /* Get relative velocity */
    vecsub(m2->RWV, m->RWV, rvec);
    v1 = dotprod(rvec, dir);

    /* Calculate force */
    v1 += 5.0f;
    force = (v1 > 0.0f) ? v1 * m->mass * 50.0f : 0.0f;

    for (i = 0; i < 3; i++) {
        pos[i] = force * dir[i];
    }
    rwtobod(pos, m->CENTERFORCE, &m->UV);
}

/**
 * CollForceMineIn_m - Force when my corner is in other car
 * Based on arcade: collision.c:CollForceMineIn()
 */
void CollForceMineIn_m(MODELDAT *m, MODELDAT *m2, f32 vec[3], s32 corner) {
    f32 del[6], rvec[3], dist, mul, v1, force;
    s32 i, side, dir_idx;

    /* Find closest side */
    del[0] = vec[0] - m2->BODYR[0][0];
    del[1] = vec[1] - m2->BODYR[0][1];
    del[2] = -10.0f;
    del[3] = m2->BODYR[3][0] - vec[0];
    del[4] = m2->BODYR[3][1] - vec[1];
    del[5] = -10.0f;

    dist = -10.0f;
    side = 0;
    for (i = 0; i < 6; i++) {
        if ((del[i] < 0.0f) && (del[i] > dist)) {
            dist = del[i];
            side = i;
        }
    }

    mul = (side < 3) ? 1.0f : -1.0f;
    dir_idx = (side < 3) ? side : (side - 3);

    /* Get relative velocity */
    vecsub(m->RWV, m2->RWV, vec);
    rwtobod(vec, rvec, &m2->UV);
    v1 = rvec[dir_idx] * mul;

    /* Get force */
    force = collForceNormal_m(m, COLL_ALG_SOFTWALL, dist, v1);
    for (i = 0; i < 3; i++) {
        vec[i] = (i == dir_idx) ? (force * mul) : 0.0f;
    }
    bodtorw(vec, rvec, &m2->UV);
    rwtobod(rvec, m->BODYFORCE[corner], &m->UV);
}

/**
 * CollForceOtherIn_m - Force when other car's corner is in me
 * Based on arcade: collision.c:CollForceOtherIn()
 */
void CollForceOtherIn_m(MODELDAT *m, MODELDAT *m2, f32 vec[3], s32 corner) {
    f32 del[6], rvec[3], vvec[3], dist, mul, v1, force;
    s32 i, side, dir_idx;

    /* Find closest side */
    del[0] = vec[0] - m->BODYR[0][0];
    del[1] = vec[1] - m->BODYR[0][1];
    del[2] = -10.0f;
    del[3] = m->BODYR[3][0] - vec[0];
    del[4] = m->BODYR[3][1] - vec[1];
    del[5] = -10.0f;

    dist = -10.0f;
    side = 0;
    for (i = 0; i < 6; i++) {
        if ((del[i] < 0.0f) && (del[i] > dist)) {
            dist = del[i];
            side = i;
        }
    }

    mul = (side < 3) ? 1.0f : -1.0f;
    dir_idx = (side < 3) ? side : (side - 3);

    /* Get relative velocity */
    vecsub(m2->RWV, m->RWV, vvec);
    rwtobod(vvec, rvec, &m->UV);
    v1 = rvec[dir_idx] * mul;

    /* Get force */
    force = collForceNormal_m(m, COLL_ALG_SOFTWALL, dist, v1);

    /* Distribute force to body corners */
    if (side == 0) {
        distributeForce(-force, vec[1], m->BODYR[0][1],
                        &m->BODYFORCE[0][0], &m->BODYFORCE[1][0]);
    } else if (side == 1) {
        distributeForce(-force, vec[0], m->BODYR[0][0],
                        &m->BODYFORCE[0][1], &m->BODYFORCE[2][1]);
    } else if (side == 3) {
        distributeForce(force, vec[1], m->BODYR[0][1],
                        &m->BODYFORCE[2][0], &m->BODYFORCE[3][0]);
    } else if (side == 4) {
        distributeForce(force, vec[0], m->BODYR[0][0],
                        &m->BODYFORCE[1][1], &m->BODYFORCE[3][1]);
    }
}

/**
 * collForceNormal_m - Calculate collision force based on algorithm
 * Based on arcade: collision.c:collForceNormal()
 */
f32 collForceNormal_m(MODELDAT *m, u32 coll_alg, f32 din, f32 vin) {
    f32 force, vout;

    switch (coll_alg) {
        case COLL_ALG_SOFTWALL:
            if (vin < 0.0f) {
                /* Compression */
                force = (din * g_Kcs) - (vin * vin * g_Kcd);
            } else {
                /* Rebound */
                force = (din * g_Krs) + (vin * vin * g_Krd);
            }
            force *= 2.0f;
            if (force < 0.0f) force = 0.0f;
            break;

        case COLL_ALG_HARDWALL:
            vout = (din * 4.0f) + 1.0f;
            force = (vin + vout) * m->mass * 0.5f * m->idt;
            if (force < 0.0f) force = 0.0f;
            break;

        case COLL_ALG_CARBODY:
            force = 50000.0f;
            if (force < 0.0f) force = 0.0f;
            break;

        default:
            force = 0.0f;
            break;
    }

    return force;
}
