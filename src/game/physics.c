/**
 * physics.c - Car physics system for Rush 2049 N64
 *
 * Based on arcade drivsym.c (Milliken Engineering model)
 * Copyright 1985 Milliken Engineering, translation by Max Behensky.
 *
 * This implements a simplified vehicle dynamics model:
 * - 4-wheel tire forces with slip angle/ratio
 * - Suspension compression and damping
 * - Anti-spin torque for stability
 * - Aerodynamic drag
 *
 * Key N64 functions:
 * - PhysicsObjectList_Update @ 0x800B0868 - per-frame physics
 * - Effects_UpdateEmitters @ 0x800B811C - particle/trail FX
 */

#include "types.h"
#include "game/physics.h"
#include "game/structs.h"

/* External game state */
extern u8 gstate;
extern u32 frame_counter;
extern CarData car_array[];
extern s32 num_active_cars;
extern s32 this_car;

/* External math functions */
extern f32 sqrtf(f32 x);
extern f32 sinf(f32 x);
extern f32 cosf(f32 x);

/* Physics state for all cars */
CarPhysics car_physics[MAX_CARS];

/* Anti-spin constants per car type and difficulty */
const f32 as_road[NUM_CAR_TYPES][NUM_DIFF_OPT] = {
    { 600.0f, 300.0f, 150.0f },
    { 400.0f, 200.0f, 100.0f },
    { 200.0f, 100.0f,  50.0f },
    {  10.0f,   5.0f,   0.0f }
};

const f32 as_air[NUM_CAR_TYPES][NUM_DIFF_OPT] = {
    { 600.0f, 300.0f, 150.0f },
    { 400.0f, 200.0f, 100.0f },
    { 200.0f, 100.0f,  50.0f },
    {  10.0f,   5.0f,   0.0f }
};

/* Anti-spin scale per car type */
const f32 as_scale[NUM_CAR_TYPES] = { 1200.0f, 600.0f, 100.0f, 0.0f };

/* Dirt drag coefficient */
static const f32 dirtdamp = -(1000.0f / 100.0f);

/* World gravity vector */
static const f32 world_gravity[3] = { 0.0f, 0.0f, -GRAVITY };

/**
 * physics_init - Initialize physics for one car
 *
 * @param m Physics state to initialize
 */
void physics_init(CarPhysics *m) {
    s32 i, j;

    /* Zero out position and orientation */
    for (i = 0; i < 3; i++) {
        m->RWR[i] = 0.0f;
        m->V[i] = 0.0f;
        m->W[i] = 0.0f;
        m->A[i] = 0.0f;
        m->AA[i] = 0.0f;
        m->F[i] = 0.0f;
        m->M[i] = 0.0f;
        m->G[i] = 0.0f;
        m->GRW[i] = world_gravity[i];
        m->D[i] = 0.0f;
        m->CENTERFORCE[i] = 0.0f;
        m->CENTERMOMENT[i] = 0.0f;
    }

    /* Initialize orientation to identity */
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            m->UV.fpuvs[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }

    /* Initialize tire data */
    for (i = 0; i < 4; i++) {
        m->tires[i].angvel = 0.0f;
        m->tires[i].slip_ratio = 0.0f;
        m->tires[i].slip_angle = 0.0f;
        m->tires[i].load = 0.0f;
        m->tires[i].radius = 1.0f;
        m->tires[i].friction = 1.0f;
        m->tires[i].on_ground = 0;
        m->tires[i].roadcode = ROAD_AIR;

        m->suscomp[i] = 0.0f;
        m->springrate[i] = 3000.0f;
        m->cdamping[i] = 200.0f;
        m->rdamping[i] = 150.0f;
        m->torque[i] = 0.0f;
        m->roadcode[i] = ROAD_AIR;

        for (j = 0; j < 3; j++) {
            m->TIREFORCE[i][j] = 0.0f;
            m->TIREV[i][j] = 0.0f;
            m->BODYFORCE[i][j] = 0.0f;
        }
    }

    /* Default tire positions relative to CG */
    /* Front left */
    m->TIRER[FLTIRE][XCOMP] =  3.0f;
    m->TIRER[FLTIRE][YCOMP] =  2.5f;
    m->TIRER[FLTIRE][ZCOMP] = -1.5f;
    /* Front right */
    m->TIRER[FRTIRE][XCOMP] =  3.0f;
    m->TIRER[FRTIRE][YCOMP] = -2.5f;
    m->TIRER[FRTIRE][ZCOMP] = -1.5f;
    /* Rear left */
    m->TIRER[RLTIRE][XCOMP] = -4.0f;
    m->TIRER[RLTIRE][YCOMP] =  2.5f;
    m->TIRER[RLTIRE][ZCOMP] = -1.5f;
    /* Rear right */
    m->TIRER[RRTIRE][XCOMP] = -4.0f;
    m->TIRER[RRTIRE][YCOMP] = -2.5f;
    m->TIRER[RRTIRE][ZCOMP] = -1.5f;

    /* Vehicle parameters */
    m->mass = 3000.0f;      /* ~1360 kg */
    m->massinv = 1.0f / m->mass;
    m->rollresist = 30.0f;
    m->srefpcybo2 = 0.001f; /* Aero drag */
    m->farspringrate = 10000.0f;

    /* Steering feedback */
    m->swtorque = 0.0f;
    m->swdamp = 0.0f;
    m->swfrict = 0.0f;
    m->swtorquegain = 1.0f;
    m->maxswdamp = 100.0f;
    m->maxswfrict = 0x100;
    m->minswfrict = 0x20;

    /* Drivetrain state */
    m->rpm = 0;
    m->throttle = 0.0f;
    m->brake = 0.0f;
    m->clutch = 1.0f;
    m->steerangle = 0.0f;
    m->autotrans = 1;
    m->bog_state = 0;
    m->nothrusttorque = 0;

    /* Timing */
    m->thetime = 0.0f;
    m->dt = DT_PHYSICS;
    m->airtime = 0.0f;
    m->magvel = 0.0f;

    /* State flags */
    m->crashflag = 0;
    m->gameover = 0;
    m->carnum = 0;
    m->we_control = 0;
    m->net_node = 0;
    m->colrad = 5.0f;

    /* Peak forces */
    for (i = 0; i < 3; i++) {
        m->peak_center_force[0][i] = 0.0f;
        m->peak_center_force[1][i] = 0.0f;
        m->peak_body_force[0][i] = 0.0f;
        m->peak_body_force[1][i] = 0.0f;
    }
}

/**
 * physics_init_all - Initialize physics for all cars
 */
void physics_init_all(void) {
    s32 i;

    for (i = 0; i < MAX_CARS; i++) {
        physics_init(&car_physics[i]);
        car_physics[i].carnum = i;
    }
}

/**
 * physics_syminit - Initialize physics simulation
 * Based on arcade: drivsym.c:syminit()
 *
 * @param m Physics state
 */
void physics_syminit(CarPhysics *m) {
    physics_init(m);
    physics_road(m);
    physics_positions(m);
    physics_mcommunication(m);
}

/**
 * physics_sym - Main physics simulation step
 * Based on arcade: drivsym.c:sym()
 *
 * @param m Physics state
 */
void physics_sym(CarPhysics *m) {
    f32 pneumtrail;
    s32 absvel, i, j;

    physics_controls(m);

    /* Zero controls if crashed */
    if (m->crashflag) {
        m->steerangle = 0.0f;
        m->clutch = 0.0f;
        m->brake = 0.0f;
        m->throttle = 0.0f;
    }

    physics_checkok(m);
    physics_drivetrain(m);
    physics_regular(m);

    /* Calculate steering wheel feedback */
    if ((m->TIREFORCE[FLTIRE][ZCOMP] + m->TIREFORCE[FRTIRE][ZCOMP]) != 0.0f) {
        pneumtrail = 1.0f;

        /* Steering torque from front wheel side force and steer angle */
        m->swtorque = (m->TIREFORCE[FLTIRE][YCOMP] + m->TIREFORCE[FRTIRE][YCOMP])
            * m->swtorquegain * pneumtrail -
            (m->TIREFORCE[FLTIRE][XCOMP] + m->TIREFORCE[FRTIRE][XCOMP])
            * m->swtorquegain * m->steerangle;

        /* Reduce feedback after crash or at start */
        if (m->crashflag || (m->thetime < 0.5f)) {
            if (m->swtorque > 0x1000) m->swtorque = 0x1000;
            if (m->swtorque < -0x1000) m->swtorque = -0x1000;
            m->swtorque *= 0.3f;
        }

        /* Speed-dependent steering damping */
        absvel = (s32)m->V[XCOMP];
        if (absvel < 0) absvel = -absvel;
        if (absvel < 88) {
            m->swdamp = (absvel * m->maxswdamp) / 88;
        } else {
            m->swdamp = m->maxswdamp;
        }

        /* Wheel speed-dependent friction */
        absvel = (s32)(2.5f * (m->tires[FLTIRE].angvel + m->tires[FRTIRE].angvel));
        if (absvel < 0) absvel = -absvel;
        if (absvel < 100) {
            m->swfrict = m->maxswfrict -
                (((m->maxswfrict - m->minswfrict) * absvel) / 100);
        } else {
            m->swfrict = m->minswfrict;
        }
    } else {
        m->swtorque = 0.0f;
        m->swdamp = 0.0f;
        m->swfrict = 0x20;
    }

    physics_mcommunication(m);

    /* Engine bog state machine */
    if ((m->autotrans == 0) && (m->throttle > 0.5f) && (m->rpm < 600 && m->rpm > -600)) {
        if (m->bog_state == 0) {
            m->bog_state = 1;  /* Start bogging */
        }
    } else {
        if (m->bog_state) {
            m->bog_state = 3;  /* Stop bogging */
        }
    }

    /* Track peak forces (for local player only) */
    if (m->net_node == this_car) {
        for (i = 0; i < 3; i++) {
            if (m->CENTERFORCE[i] > m->peak_center_force[0][i]) {
                m->peak_center_force[0][i] = m->CENTERFORCE[i];
            }
            if (m->CENTERFORCE[i] < m->peak_center_force[1][i]) {
                m->peak_center_force[1][i] = m->CENTERFORCE[i];
            }

            for (j = 0; j < 4; j++) {
                if (m->BODYFORCE[j][i] > m->peak_body_force[0][i]) {
                    m->peak_body_force[0][i] = m->BODYFORCE[j][i];
                }
                if (m->BODYFORCE[j][i] < m->peak_body_force[1][i]) {
                    m->peak_body_force[1][i] = m->BODYFORCE[j][i];
                }
            }
        }
    }
}

/**
 * physics_regular - Standard physics update cycle
 * Based on arcade: drivsym.c:regular()
 *
 * @param m Physics state
 */
void physics_regular(CarPhysics *m) {
    physics_road(m);
    physics_forces(m);
    physics_antispin(m);
    physics_torques(m);
    physics_accelerations(m);
    physics_velocities(m);
    physics_positions(m);
}

/**
 * physics_antispin - Calculate anti-spin stabilization torque
 * Based on arcade: drivsym.c:antispin()
 *
 * Prevents excessive yaw (spin) by applying counter-moment
 * when car is rotating away from direction of travel.
 *
 * @param m Physics state
 */
void physics_antispin(CarPhysics *m) {
    f32 as_val;
    s32 air, drone;
    f32 M, f, f1, f2, f4;

    /* Check if stuck on a wall */
    /* If slow, moving backwards, right-side-up, with front contact */
    if ((m->magvel < 5.0f) && (m->V[XCOMP] < 0.0f) &&
        (m->UV.fpuvs[ZCOMP][ZCOMP] > 0.0f) &&
        ((m->BODYFORCE[FRTIRE][XCOMP] < 0.0f) || (m->BODYFORCE[FLTIRE][XCOMP] < 0.0f))) {
        m->CENTERMOMENT[XCOMP] = 0.0f;
        m->CENTERMOMENT[YCOMP] = 0.0f;
        m->CENTERMOMENT[ZCOMP] = (m->steerangle > 0.0f) ? 90000.0f : -90000.0f;
        return;
    }

    /* Check if all tires in air */
    air = ((m->roadcode[0] == ROAD_AIR) &&
           (m->roadcode[1] == ROAD_AIR) &&
           (m->roadcode[2] == ROAD_AIR) &&
           (m->roadcode[3] == ROAD_AIR));

    drone = m->we_control;

    if (drone) {
        /* Drones get fixed anti-spin */
        as_val = 100.0f;

        m->CENTERMOMENT[XCOMP] = 0.0f;
        m->CENTERMOMENT[YCOMP] = 0.0f;

        /* Only anti-spin if rotating away from direction of travel */
        if ((m->W[ZCOMP] * m->V[YCOMP]) < 0.0f) {
            m->CENTERMOMENT[ZCOMP] = m->V[YCOMP] * as_val;
        } else {
            m->CENTERMOMENT[ZCOMP] = 0.0f;
        }
    } else {
        /* Human players get car-type-based anti-spin */
        as_val = as_scale[m->carnum & 0x3];

        m->CENTERMOMENT[XCOMP] = 0.0f;
        m->CENTERMOMENT[YCOMP] = 0.0f;

        /* Calculate slip factor */
        f1 = (m->magvel < 0.1f) ? 0.0f : (1.0f - (m->V[XCOMP] / m->magvel));
        f2 = f1 * f1;
        f4 = f2 * f2;

        /* Blend based on throttle */
        f = f4 + (f1 - f4) * m->throttle;

        M = f * m->magvel * as_val;

        /* Only anti-spin if rotating away from direction of travel */
        if ((m->W[ZCOMP] * m->V[YCOMP]) < 0.0f) {
            m->CENTERMOMENT[ZCOMP] = (m->V[YCOMP] > 0.0f) ? M : -M;
        } else {
            m->CENTERMOMENT[ZCOMP] = 0.0f;
        }
    }
}

/**
 * physics_forces - Calculate all forces on vehicle
 * Based on arcade: drivsym.c:forces()
 *
 * @param m Physics state
 */
void physics_forces(CarPhysics *m) {
    physics_forces1(m);
    physics_forces2(m);
}

/**
 * physics_forces1 - Calculate tire and drag forces
 * Based on arcade: drivsym.c:forces1()
 *
 * @param m Physics state
 */
void physics_forces1(CarPhysics *m) {
    s32 poortract, i;
    f32 gameoverdrag, airfact;

    /* Zero forces at simulation start */
    if (m->thetime <= m->dt) {
        m->D[XCOMP] = 0.0f;
        m->D[YCOMP] = 0.0f;
        m->D[ZCOMP] = 0.0f;

        for (i = 0; i < 4; i++) {
            m->TIREFORCE[i][XCOMP] = 0.0f;
            m->TIREFORCE[i][YCOMP] = 0.0f;
            m->TIREFORCE[i][ZCOMP] = 0.0f;
            m->suscomp[i] = 0.0f;
        }
    }

    /* Track time in air */
    if ((m->roadcode[0] == ROAD_AIR) ||
        (m->roadcode[1] == ROAD_AIR) ||
        (m->roadcode[2] == ROAD_AIR) ||
        (m->roadcode[3] == ROAD_AIR)) {
        m->airtime = m->thetime;
    }

    airfact = 1.0f;
    poortract = 0;  /* Same traction on all surfaces */

    /* Calculate tire forces (simplified) */
    /* In full implementation, would call calctireuv and dotireforce for each tire */
    /* For now, apply simplified forces based on inputs */

    for (i = 0; i < 4; i++) {
        if (m->roadcode[i] != ROAD_AIR) {
            /* Tire on ground - apply traction force */
            m->TIREFORCE[i][XCOMP] = m->torque[i] * 0.1f;  /* Simplified */
            m->TIREFORCE[i][YCOMP] = -m->steerangle * m->V[XCOMP] * 10.0f * (i < 2 ? 1.0f : 0.0f);
            m->TIREFORCE[i][ZCOMP] = m->mass * GRAVITY * 0.25f;  /* Weight distribution */
        } else {
            /* Tire in air - no forces */
            m->TIREFORCE[i][XCOMP] = 0.0f;
            m->TIREFORCE[i][YCOMP] = 0.0f;
            m->TIREFORCE[i][ZCOMP] = 0.0f;
        }
    }

    /* Calculate drag forces */
    m->D[YCOMP] = 0.0f;
    m->D[ZCOMP] = 0.0f;

    if (m->V[XCOMP] > 0.0f) {
        m->D[XCOMP] = -(30.0f + m->rollresist + m->srefpcybo2 * m->V[XCOMP] * m->V[XCOMP]);
    } else {
        m->D[XCOMP] = -30.0f + m->rollresist + m->srefpcybo2 * m->V[XCOMP] * m->V[XCOMP];
    }

    /* Extra drag on dirt */
    if ((m->roadcode[RLTIRE] == ROAD_DIRT) && (m->roadcode[RRTIRE] == ROAD_DIRT)) {
        m->D[XCOMP] += dirtdamp * m->V[XCOMP];
    }

    /* Game over drag (strong slowdown) */
    if (m->gameover) {
        gameoverdrag = vec_magnitude(m->V);
        if (gameoverdrag > 1.0f) {
            gameoverdrag = -1500.0f / gameoverdrag;
            vec_scale(m->V, gameoverdrag, m->D);
        }
    }
}

/**
 * physics_forces2 - Sum all forces
 * Based on arcade: drivsym.c:forces2()
 *
 * @param m Physics state
 */
void physics_forces2(CarPhysics *m) {
    f32 temp;

    if (m->nothrusttorque) {
        /* Remove yaw from different wheel thrust */
        temp = (m->TIREFORCE[RLTIRE][XCOMP] + m->TIREFORCE[RRTIRE][XCOMP]) * 0.5f;
        m->TIREFORCE[RLTIRE][XCOMP] = temp;
        m->TIREFORCE[RRTIRE][XCOMP] = temp;
    }

    /* Sum tire forces */
    vec_add(m->TIREFORCE[0], m->TIREFORCE[1], m->F);
    vec_add(m->TIREFORCE[2], m->F, m->F);
    vec_add(m->TIREFORCE[3], m->F, m->F);

    /* Add gravity in body frame */
    rw_to_body(m->GRW, m->G, &m->UV);
    vec_add(m->G, m->F, m->F);

    /* Add drag */
    vec_add(m->D, m->F, m->F);

    /* Add body corner forces (collision) */
    vec_add(m->BODYFORCE[0], m->F, m->F);
    vec_add(m->BODYFORCE[1], m->F, m->F);
    vec_add(m->BODYFORCE[2], m->F, m->F);
    vec_add(m->BODYFORCE[3], m->F, m->F);

    /* Add centering force */
    vec_add(m->CENTERFORCE, m->F, m->F);
}

/**
 * physics_torques - Calculate moments from forces
 * Based on arcade: drivsym.c:torques()
 *
 * @param m Physics state
 */
void physics_torques(CarPhysics *m) {
    f32 temp[3], temp1[3];
    s32 i;

    /* Zero moment */
    m->M[XCOMP] = 0.0f;
    m->M[YCOMP] = 0.0f;
    m->M[ZCOMP] = 0.0f;

    /* Add moment from each tire: M = r x F */
    for (i = 0; i < 4; i++) {
        vec_copy(m->TIRER[i], temp1);
        temp1[ZCOMP] -= m->suscomp[i];  /* Account for suspension */
        vec_cross(temp1, m->TIREFORCE[i], temp);
        vec_add(m->M, temp, m->M);
    }

    /* Add moment from body corners */
    for (i = 0; i < 4; i++) {
        vec_copy(m->TIRER[i], temp1);
        temp1[ZCOMP] = 0.0f;
        vec_cross(temp1, m->BODYFORCE[i], temp);
        vec_add(m->M, temp, m->M);
    }

    /* Add anti-spin moment */
    vec_add(m->CENTERMOMENT, m->M, m->M);
}

/**
 * physics_accelerations - Calculate accelerations from forces
 * Based on arcade: drivsym.c:accelerations()
 *
 * @param m Physics state
 */
void physics_accelerations(CarPhysics *m) {
    /* F = ma, so A = F/m */
    vec_scale(m->F, m->massinv, m->A);

    /* Angular acceleration (simplified - would need inertia tensor) */
    /* For now, use simplified model */
    m->AA[XCOMP] = m->M[XCOMP] * 0.0001f;  /* Roll */
    m->AA[YCOMP] = m->M[YCOMP] * 0.0001f;  /* Pitch */
    m->AA[ZCOMP] = m->M[ZCOMP] * 0.00005f; /* Yaw */
}

/**
 * physics_velocities - Integrate accelerations to velocities
 * Based on arcade: drivsym.c:velocities()
 *
 * @param m Physics state
 */
void physics_velocities(CarPhysics *m) {
    f32 temp[3], velfact;

    /* V = V + A*dt */
    vec_scale(m->A, m->dt, temp);
    vec_add(m->V, temp, m->V);

    /* Clamp velocity to speed of sound */
    m->magvel = vec_magnitude(m->V);
    if (m->magvel > MAX_VELOCITY) {
        velfact = MAX_VELOCITY / m->magvel;
        m->magvel = MAX_VELOCITY;
        vec_scale(m->V, velfact, m->V);
    }

    /* W = W + AA*dt */
    vec_scale(m->AA, m->dt, temp);
    vec_add(m->W, temp, m->W);
}

/**
 * physics_positions - Integrate velocities to positions
 * Based on arcade: drivsym.c:positions()
 *
 * @param m Physics state
 */
void physics_positions(CarPhysics *m) {
    f32 temp[3], delta_rw[3];

    /* Convert body velocity to world velocity */
    body_to_rw(m->V, delta_rw, &m->UV);

    /* RWR = RWR + V_world * dt */
    vec_scale(delta_rw, m->dt, temp);
    vec_add(m->RWR, temp, m->RWR);

    /* Update orientation (simplified - should use quaternions or proper rotation) */
    /* For now, just update UV matrix based on angular velocity */
    /* This is a simplification - real implementation needs proper rotation math */

    m->thetime += m->dt;
}

/**
 * physics_controls - Apply control inputs
 *
 * @param m Physics state
 */
void physics_controls(CarPhysics *m) {
    /* Control inputs come from car_array via game logic */
    /* This would map controller inputs to physics inputs */
}

/**
 * physics_road - Determine road surface under each tire
 *
 * @param m Physics state
 */
void physics_road(CarPhysics *m) {
    /* Would query track collision to determine surface type */
    /* For now, assume all on ground */
    s32 i;
    for (i = 0; i < 4; i++) {
        m->roadcode[i] = ROAD_ASPHALT;
        m->tires[i].on_ground = 1;
        m->tires[i].roadcode = ROAD_ASPHALT;
    }
}

/**
 * physics_drivetrain - Calculate engine and wheel torques
 *
 * @param m Physics state
 */
void physics_drivetrain(CarPhysics *m) {
    f32 engine_torque;
    f32 wheel_torque;

    /* Simple torque model */
    engine_torque = m->throttle * 500.0f;  /* Simplified engine */

    /* Apply brakes */
    if (m->brake > 0.0f) {
        wheel_torque = -m->brake * 2000.0f;
    } else {
        wheel_torque = engine_torque * 0.25f;  /* Split to 4 wheels */
    }

    /* Rear wheel drive */
    m->torque[FLTIRE] = 0.0f;
    m->torque[FRTIRE] = 0.0f;
    m->torque[RLTIRE] = wheel_torque;
    m->torque[RRTIRE] = wheel_torque;

    /* Update RPM (simplified) */
    m->rpm = (s32)(m->V[XCOMP] * 30.0f + 1000.0f);
    if (m->rpm < 0) m->rpm = 0;
    if (m->rpm > 8000) m->rpm = 8000;
}

/**
 * physics_checkok - Validate physics values are in range
 *
 * @param m Physics state
 */
void physics_checkok(CarPhysics *m) {
    /* Would check for NaN, infinity, out of range values */
    /* And reset if needed */
}

/**
 * physics_mcommunication - Communicate results for display/network
 *
 * @param m Physics state
 */
void physics_mcommunication(CarPhysics *m) {
    /* Would copy physics state to display/network structures */
}

/* Vector math helpers */

void vec_add(f32 a[3], f32 b[3], f32 out[3]) {
    out[0] = a[0] + b[0];
    out[1] = a[1] + b[1];
    out[2] = a[2] + b[2];
}

void vec_sub(f32 a[3], f32 b[3], f32 out[3]) {
    out[0] = a[0] - b[0];
    out[1] = a[1] - b[1];
    out[2] = a[2] - b[2];
}

void vec_copy(f32 src[3], f32 dst[3]) {
    dst[0] = src[0];
    dst[1] = src[1];
    dst[2] = src[2];
}

void vec_scale(f32 v[3], f32 s, f32 out[3]) {
    out[0] = v[0] * s;
    out[1] = v[1] * s;
    out[2] = v[2] * s;
}

void vec_cross(f32 a[3], f32 b[3], f32 out[3]) {
    out[0] = a[1] * b[2] - a[2] * b[1];
    out[1] = a[2] * b[0] - a[0] * b[2];
    out[2] = a[0] * b[1] - a[1] * b[0];
}

f32 vec_magnitude(f32 v[3]) {
    return sqrtf(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}

/**
 * rw_to_body - Transform vector from world to body frame
 *
 * @param rw World frame vector
 * @param body Output body frame vector
 * @param uv Orientation matrix
 */
void rw_to_body(f32 rw[3], f32 body[3], UVect *uv) {
    /* body = UV^T * rw (transpose because UV rows are basis vectors) */
    body[0] = uv->fpuvs[0][0]*rw[0] + uv->fpuvs[0][1]*rw[1] + uv->fpuvs[0][2]*rw[2];
    body[1] = uv->fpuvs[1][0]*rw[0] + uv->fpuvs[1][1]*rw[1] + uv->fpuvs[1][2]*rw[2];
    body[2] = uv->fpuvs[2][0]*rw[0] + uv->fpuvs[2][1]*rw[1] + uv->fpuvs[2][2]*rw[2];
}

/**
 * body_to_rw - Transform vector from body to world frame
 *
 * @param body Body frame vector
 * @param rw Output world frame vector
 * @param uv Orientation matrix
 */
void body_to_rw(f32 body[3], f32 rw[3], UVect *uv) {
    /* rw = UV * body */
    rw[0] = uv->fpuvs[0][0]*body[0] + uv->fpuvs[1][0]*body[1] + uv->fpuvs[2][0]*body[2];
    rw[1] = uv->fpuvs[0][1]*body[0] + uv->fpuvs[1][1]*body[1] + uv->fpuvs[2][1]*body[2];
    rw[2] = uv->fpuvs[0][2]*body[0] + uv->fpuvs[1][2]*body[1] + uv->fpuvs[2][2]*body[2];
}

/******* ARCADE TORQUE CURVES *******/
/* From arcade: cars.c */
/* Rows = throttle 0-100%, Columns = RPM 0-11000 */

const s16 stdtorquecurve[10][12] = {
    {  75, -10, -30, -50, -70, -80, -80, -80, -80, -80, -80, -80 },
    {  75, 100,   1, -12, -29, -41, -49, -60, -71, -76, -78, -80 },
    {  75, 150,  32,  26,  12,  -2, -18, -40, -62, -71, -76, -80 },
    {  75, 150,  63,  63,  53,  37,  13, -20, -53, -67, -73, -80 },
    {  75, 150,  94, 101,  94,  76,  44,   0, -44, -62, -71, -80 },
    {  75, 175, 126, 139, 136, 114,  76,  20, -36, -58, -69, -80 },
    {  75, 175, 157, 177, 177, 153, 107,  40, -27, -53, -67, -80 },
    {  75, 175, 188, 214, 218, 192, 138,  60, -18, -49, -64, -80 },
    {  75, 177, 219, 252, 259, 231, 169,  80,  -9, -44, -62, -80 },
    {  75, 200, 250, 290, 300, 270, 200, 100,   0, -40, -60, -80 }
};

const s16 rushtorquecurve[10][12] = {
    { 100, -12, -33, -51, -61, -54, -71, -75, -75, -75, -75, -75 },
    { 100, 112,  62,   3, -30, -51, -65, -69, -71, -73, -75, -75 },
    { 100, 114, 113, 112, 104,  70,  87,  32, -32, -62, -70, -75 },
    { 100, 116, 126, 131, 136, 136, 131,  90,  -1, -36, -69, -75 },
    { 100, 119, 137, 151, 161, 170, 175, 160,  90,   0, -50, -75 },
    { 100, 127, 156, 178, 196, 215, 223, 225, 205,  75, -32, -75 },
    { 100, 133, 163, 190, 212, 230, 245, 256, 243, 108, -23, -75 },
    { 100, 137, 175, 206, 230, 250, 265, 280, 282, 175, -10, -75 },
    { 100, 146, 186, 222, 248, 275, 293, 305, 310, 251,   4, -69 },
    { 100, 155, 202, 240, 275, 300, 320, 327, 328, 312,  49, -65 }
};

/**
 * tire_constants - Calculate derived tire constants
 * Based on arcade: initiali.c:tire_constants()
 *
 * Calculates polynomial coefficients for tire force model
 * based on cornering stiffness and max friction.
 *
 * @param tdes Tire description to update
 */
void tire_constants(TireDes *tdes) {
    /* Calculate slip angle at max force */
    tdes->Afmax = 3.0f * tdes->Cfmax * tdes->Zforce / tdes->Cstiff;

    /* Lateral force polynomial: Cf = k1*a + k2*|a|*a + k3*a^3 */
    tdes->k1 = tdes->Cstiff / tdes->Zforce;
    tdes->k2 = tdes->k1 * tdes->k1 / (3.0f * tdes->Cfmax);
    tdes->k3 = tdes->k1 * tdes->k1 * tdes->k1 / (27.0f * tdes->Cfmax * tdes->Cfmax);

    /* Derivative coefficients */
    tdes->l2 = tdes->k2 * 2.0f;
    tdes->l3 = tdes->k3 * 3.0f;

    /* Aligning torque coefficients */
    tdes->m1 = 40.0f * 0.052f / (tdes->Cfmax / 2.0f);
    tdes->m2 = tdes->m1 * tdes->m1 / 3.4f;
    tdes->m3 = tdes->m1 * tdes->m1 * tdes->m1 / 46.3f;
    tdes->m4 = 1.0f / 0.000055f;

    /* Reset contact patch deformation */
    tdes->patchy = 0.0f;
}

/**
 * copy_tire_info - Copy tire parameters with load adjustment
 * Based on arcade: initiali.c:copy_tire_info()
 *
 * @param mass Vehicle mass (for load calculation)
 * @param tp1 Source tire parameters
 * @param tp2 Destination tire parameters
 * @param otw Track width to other tire (feet)
 * @param wb Wheelbase (feet)
 */
void copy_tire_info(f32 mass, TireDes *tp1, TireDes *tp2, f32 otw, f32 wb) {
    tp2->tradius = tp1->tradius;
    tp2->springK = tp1->springK;
    tp2->rubdamp = tp1->rubdamp;
    tp2->Cstiff = tp1->PaveCstiff;
    tp2->PaveCstiff = tp1->PaveCstiff;
    tp2->Cfmax = tp1->PaveCfmax;
    tp2->PaveCfmax = tp1->PaveCfmax;
    tp2->invmi = tp1->invmi;

    /* Calculate static load based on geometry */
    tp2->Zforce = (mass * GRAVCON * otw / wb) * 0.5f;
}

/**
 * get_engine_torque - Interpolate torque from curve
 * Based on arcade: engine torque lookup
 *
 * @param curve Torque curve (10x12 table)
 * @param throttle Throttle position (0-1)
 * @param rpm Engine RPM
 * @param rpm_per_entry RPM per curve column
 * @return Interpolated torque value
 */
f32 get_engine_torque(const s16 curve[10][12], f32 throttle, s32 rpm, s32 rpm_per_entry) {
    s32 throttle_idx, rpm_idx;
    f32 throttle_frac, rpm_frac;
    f32 t00, t01, t10, t11;
    f32 t0, t1;

    /* Clamp throttle to valid range */
    if (throttle < 0.0f) throttle = 0.0f;
    if (throttle > 1.0f) throttle = 1.0f;

    /* Clamp RPM to valid range */
    if (rpm < 0) rpm = 0;
    if (rpm > 11 * rpm_per_entry) rpm = 11 * rpm_per_entry;

    /* Calculate indices and fractions */
    throttle_idx = (s32)(throttle * 9.0f);
    if (throttle_idx > 8) throttle_idx = 8;
    throttle_frac = throttle * 9.0f - (f32)throttle_idx;

    rpm_idx = rpm / rpm_per_entry;
    if (rpm_idx > 10) rpm_idx = 10;
    rpm_frac = (f32)(rpm % rpm_per_entry) / (f32)rpm_per_entry;

    /* Bilinear interpolation */
    t00 = (f32)curve[throttle_idx][rpm_idx];
    t01 = (f32)curve[throttle_idx][rpm_idx + 1];
    t10 = (f32)curve[throttle_idx + 1][rpm_idx];
    t11 = (f32)curve[throttle_idx + 1][rpm_idx + 1];

    t0 = t00 + (t01 - t00) * rpm_frac;
    t1 = t10 + (t11 - t10) * rpm_frac;

    return t0 + (t1 - t0) * throttle_frac;
}

/**
 * init_car_from_params - Initialize car physics from Car parameters
 *
 * @param m Physics state to initialize
 * @param car Car parameter definition
 */
void init_car_from_params(CarPhysics *m, const Car *car) {
    s32 i;

    m->mass = car->mass;
    m->massinv = 1.0f / car->mass;
    m->rollresist = car->rollresist;
    m->srefpcybo2 = car->srefpcybo2;

    for (i = 0; i < 4; i++) {
        m->springrate[i] = -car->springrate[i];  /* Negate (compression = positive) */
        m->cdamping[i] = -car->cdamping[i];
        m->rdamping[i] = -car->rdamping[i];

        m->TIRER[i][XCOMP] = car->TIRER[i][XCOMP];
        m->TIRER[i][YCOMP] = car->TIRER[i][YCOMP];
        m->TIRER[i][ZCOMP] = car->TIRER[i][ZCOMP];
    }

    m->farspringrate = -car->farspringrate;

    /* Steering */
    m->swtorquegain = car->swtpg;
    m->maxswdamp = (f32)car->maxswdamp;
    m->maxswfrict = (f32)car->maxswfrict;
    m->minswfrict = (f32)car->minswfrict;

    m->nothrusttorque = car->nothrusttorque;
}

/* Stub declarations for original disassembled functions */
void physics_object_list_update(void);  /* TODO: Decompile */
void effects_emitters_update(void);  /* TODO: Decompile */

/* ========================================================================
 * Arcade-compatible function implementations (drivsym.c)
 * ======================================================================== */

/**
 * syminit - Initialize physics simulation
 * Arcade: drivsym.c:syminit()
 */
void syminit(MODELDAT *m) {
    physics_syminit(m);
}

/**
 * initialize - Initialize physics state
 * Arcade: initiali.c:initialize()
 */
void initialize(MODELDAT *m) {
    physics_init(m);
}

/**
 * sym - Main physics simulation step
 * Arcade: drivsym.c:sym()
 */
void sym(MODELDAT *m) {
    physics_sym(m);
}

/**
 * regular - Standard physics update cycle
 * Arcade: drivsym.c:regular()
 */
void regular(MODELDAT *m) {
    physics_regular(m);
}

/**
 * forces - Calculate all forces on vehicle
 * Arcade: drivsym.c:forces()
 */
void forces(MODELDAT *m) {
    physics_forces(m);
}

/**
 * forces1 - Calculate tire and drag forces
 * Arcade: drivsym.c:forces1()
 */
void forces1(MODELDAT *m) {
    physics_forces1(m);
}

/**
 * forces2 - Sum all forces
 * Arcade: drivsym.c:forces2()
 */
void forces2(MODELDAT *m) {
    physics_forces2(m);
}

/**
 * antispin - Calculate anti-spin stabilization torque
 * Arcade: drivsym.c:antispin()
 */
void antispin(MODELDAT *m) {
    physics_antispin(m);
}

/**
 * torques - Calculate moments from forces
 * Arcade: drivsym.c:torques()
 */
void torques(MODELDAT *m) {
    physics_torques(m);
}

/**
 * accelerations - Calculate accelerations from forces
 * Arcade: drivsym.c:accelerations()
 */
void accelerations(MODELDAT *m) {
    physics_accelerations(m);
}

/**
 * velocities - Integrate accelerations to velocities
 * Arcade: drivsym.c:velocities()
 */
void velocities(MODELDAT *m) {
    physics_velocities(m);
}

/**
 * positions - Integrate velocities to positions
 * Arcade: drivsym.c:positions()
 */
void positions(MODELDAT *m) {
    physics_positions(m);
}

/**
 * controls - Apply control inputs
 * Arcade: controls.c:controls()
 */
void controls(MODELDAT *m) {
    physics_controls(m);
}

/**
 * road - Determine road surface under tires
 * Arcade: road.c:road()
 */
void road(MODELDAT *m) {
    physics_road(m);
}

/**
 * mcommunication - Communicate results for display/network
 * Arcade: communic.c:mcommunication()
 */
void mcommunication(MODELDAT *m) {
    physics_mcommunication(m);
}

/**
 * checkok - Validate physics values are in range
 * Arcade: drivsym.c:checkok()
 */
void checkok(MODELDAT *m) {
    physics_checkok(m);
}

/**
 * drivetrain - Calculate engine and wheel torques
 * Arcade: drivetra.c:drivetrain()
 */
void drivetrain(MODELDAT *m) {
    physics_drivetrain(m);
}

/**
 * calctireuv - Calculate tire unit vectors and velocity
 * Arcade: tires.c:calctireuv()
 */
void calctireuv(f32 V[3], f32 W[3], f32 tireR[3], f32 steer,
                UVect *carUV, UVect *roadUV, UVect *tireUV, f32 tireV[3]) {
    /* Calculate tire velocity: tireV = V + W x tireR */
    f32 wxr[3];
    vec_cross(W, tireR, wxr);
    vec_add(V, wxr, tireV);

    /* Tire UV would be rotated by steer angle - simplified */
    /* Full implementation needs rotation matrix */
    tireUV->fpuvs[0][0] = carUV->fpuvs[0][0];
    tireUV->fpuvs[0][1] = carUV->fpuvs[0][1];
    tireUV->fpuvs[0][2] = carUV->fpuvs[0][2];
    tireUV->fpuvs[1][0] = carUV->fpuvs[1][0];
    tireUV->fpuvs[1][1] = carUV->fpuvs[1][1];
    tireUV->fpuvs[1][2] = carUV->fpuvs[1][2];
    tireUV->fpuvs[2][0] = carUV->fpuvs[2][0];
    tireUV->fpuvs[2][1] = carUV->fpuvs[2][1];
    tireUV->fpuvs[2][2] = carUV->fpuvs[2][2];
}

/**
 * dotireforce - Calculate tire force
 * Arcade: tires.c:dotireforce()
 */
void dotireforce(MODELDAT *m, f32 tireV[3], f32 otherV[3],
                 UVect *tireUV, TireDes *tire, f32 torque,
                 f32 force[3], f32 comp, f32 otherComp,
                 f32 spring, f32 farspring, f32 cdamp, f32 rdamp,
                 s32 poortract, f32 airfact) {
    /* Simplified tire force - full Milliken model is complex */
    /* Would calculate slip angle, longitudinal slip, etc. */
    force[XCOMP] = torque * 0.1f;
    force[YCOMP] = 0.0f;
    force[ZCOMP] = m->mass * GRAVITY * 0.25f;
}

/**
 * calcaa - Calculate angular acceleration from moment
 * Arcade: drivsym.c:calcaa()
 */
void calcaa(MODELDAT *m, f32 M[3], f32 AA[3]) {
    /* AA = I^-1 * M (simplified - no full inertia tensor) */
    AA[XCOMP] = M[XCOMP] * 0.0001f;  /* Roll */
    AA[YCOMP] = M[YCOMP] * 0.0001f;  /* Pitch */
    AA[ZCOMP] = M[ZCOMP] * 0.00005f; /* Yaw */
}

/* Vector math arcade aliases */
void vecadd(f32 a[3], f32 b[3], f32 out[3]) {
    vec_add(a, b, out);
}

void vecsub(f32 a[3], f32 b[3], f32 out[3]) {
    vec_sub(a, b, out);
}

void veccopy(f32 src[3], f32 dst[3]) {
    vec_copy(src, dst);
}

void scalmul(f32 v[3], f32 s, f32 out[3]) {
    vec_scale(v, s, out);
}

void crossprod(f32 a[3], f32 b[3], f32 out[3]) {
    vec_cross(a, b, out);
}

f32 magnitude(f32 v[3]) {
    return vec_magnitude(v);
}

/* Coordinate transform arcade aliases */
void rwtobod(f32 rw[3], f32 body[3], UVect *uv) {
    rw_to_body(rw, body, uv);
}

void bodtorw(f32 body[3], f32 rw[3], UVect *uv) {
    body_to_rw(body, rw, uv);
}
