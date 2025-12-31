/**
 * tire.c - Tire physics for Rush 2049 N64
 *
 * Based on arcade tires.c by Max Behensky (1986, Atari Games)
 * Uses friction circle model with slip angle calculations.
 */

#include "game/tire.h"

/* Math function declarations (from libm) */
extern f32 sqrtf(f32 x);
extern f32 sinf(f32 x);
extern f32 cosf(f32 x);

/* Arcade constants */
#define DAMPSPEED 400           /* Speed threshold for slip angle damping */

/* Local math helpers */
static f32 xxsqrt(f32 x) {
    if (x <= 0.0f) return 0.0f;
    return sqrtf(x);
}

/* ========================================================================
 * INITIALIZATION
 * ======================================================================== */

/**
 * Initialize tire state from parameters
 */
void tire_init(TireState *state, const TireDes *params) {
    s32 i;

    /* Copy parameters */
    state->params = *params;

    /* Initialize suspension */
    state->suspension.compression = 0.0f;
    state->suspension.velocity = 0.0f;
    state->suspension.airdist = 0.0f;
    state->suspension.prev_comp = 0.0f;

    /* Clear forces */
    for (i = 0; i < 3; i++) {
        state->force.force[i] = 0.0f;
        state->tirev[i] = 0.0f;
        state->contact[i] = 0.0f;
        state->roadnorm[i] = 0.0f;
    }
    state->roadnorm[ZCOMP] = 1.0f;  /* Default up */

    state->force.sideforce = 0.0f;
    state->force.traction = 0.0f;
    state->force.normal = 0.0f;
    state->force.slipflag = 0;

    /* Initialize orientation to identity */
    for (i = 0; i < 3; i++) {
        state->uvs.uvs[i][0] = (i == 0) ? 0x4000 : 0;
        state->uvs.uvs[i][1] = (i == 1) ? 0x4000 : 0;
        state->uvs.uvs[i][2] = (i == 2) ? 0x4000 : 0;
        state->uvs.fuvs[i][0] = (i == 0) ? 1.0f : 0.0f;
        state->uvs.fuvs[i][1] = (i == 1) ? 1.0f : 0.0f;
        state->uvs.fuvs[i][2] = (i == 2) ? 1.0f : 0.0f;
    }

    state->steer = 0.0f;
    state->angvel = 0.0f;
    state->surface = SURFACE_PAVEMENT;
    state->onground = 1;

    /* Calculate tire constants */
    tire_constants(&state->params);
}

/**
 * Reset tire state
 */
void tire_reset(TireState *state) {
    const TireDes params = state->params;
    tire_init(state, &params);
}

/* ========================================================================
 * TIRE CONSTANTS
 * ======================================================================== */

/**
 * Calculate derived tire constants
 * Called when tire parameters change (e.g. surface change)
 */
void tire_constants(TireDes *tire) {
    f32 Cfmax = tire->Cfmax;
    f32 k1 = tire->Cstiff;

    /* Calculate Afmax - slip angle at max force */
    /* Afmax = 3 * Cfmax / k1 */
    if (k1 > 0.0f) {
        tire->Afmax = (3.0f * Cfmax) / k1;
    } else {
        tire->Afmax = 0.1f;
    }

    /* Calculate polynomial coefficients for tire curve */
    /* Cf = k1*a - k2*a^2 + k3*a^3 */
    if (Cfmax > 0.0f) {
        tire->k1 = k1;
        tire->k2 = (k1 * k1) / (3.0f * Cfmax);
        tire->k3 = (k1 * k1 * k1) / (27.0f * Cfmax * Cfmax);
    } else {
        tire->k1 = k1;
        tire->k2 = 0.0f;
        tire->k3 = 0.0f;
    }

    tire->l2 = tire->k2;
    tire->l3 = tire->k3;
}

/* ========================================================================
 * SLIP ANGLE CALCULATION
 * ======================================================================== */

/**
 * calcalpha - Calculate slip angle from tire velocity
 * Based on arcade: tires.c:calcalpha()
 *
 * tirev: velocity in tire coordinates (X=forward, Y=lateral, Z=vertical)
 * Uses DAMPSPEED threshold to limit slip angle at high speeds
 */
f32 calcalpha(f32 tirev[3]) {
    f32 alpha;

    alpha = tirev[YCOMP];

    if (tirev[XCOMP] > 0.0f) {
        if (tirev[XCOMP] < DAMPSPEED) {
            alpha = tirev[YCOMP] / tirev[XCOMP];  /* find tire alpha */
        } else {
            alpha = tirev[YCOMP] * (1.0f / DAMPSPEED);  /* damped alpha */
        }
    }

    if (tirev[XCOMP] < 0.0f) {
        if (tirev[XCOMP] > -DAMPSPEED) {
            alpha = -tirev[YCOMP] / tirev[XCOMP];  /* find tire alpha */
        } else {
            alpha = tirev[YCOMP] * (1.0f / DAMPSPEED);  /* damped alpha */
        }
    }

    return alpha;
}

/* N64 wrapper for arcade function name */
f32 tire_calc_alpha(f32 tirev[3]) {
    return calcalpha(tirev);
}

/* ========================================================================
 * FRICTION CIRCLE
 * ======================================================================== */

/**
 * Calculate tire forces using friction circle model
 *
 * The friction circle limits total tire force to a maximum determined
 * by normal force and friction coefficient. Traction and lateral force
 * share this limit.
 */
void tire_friction_circle(
    ModelDat *m,
    f32 tirev[3],
    f32 normalforce,
    f32 torque,
    TireDes *tire,
    f32 *sideforce,
    f32 *traction
) {
    f32 maxtraction, maxtorque, maxf;
    f32 Cfmax, Afmax, k2, k3, l2, l3;
    f32 roadangvel, patchvel, patchspeed;
    f32 realtorque, temp, alpha, ydot, p;

    realtorque = torque;
    temp = m->mass * tire->tradius;

    tire->slipflag = 0;

    /* Adjust torque for wheel inertia */
    torque = temp * torque / (temp + 1.0f / tire->invmi);

    maxtraction = tire->Cfmax * normalforce;
    maxtorque = maxtraction * tire->tradius;
    roadangvel = tirev[XCOMP] / tire->tradius;

    /* Check for wheel spin - accelerating */
    if (tire->angvel > roadangvel) {
        tire->sliptorque = maxtorque;
        tire->angvel += (realtorque - tire->sliptorque) * tire->invmi * m->dt;

        if (tire->angvel <= roadangvel) {
            tire->slipflag = SLIP_LOCKED_BRAKE;
            tire->angvel = roadangvel;
        } else {
            /* Spinning - use friction circle */
            patchvel = tirev[XCOMP] - tire->angvel * tire->tradius;
            patchspeed = xxsqrt(patchvel * patchvel + tirev[YCOMP] * tirev[YCOMP]);

            if (patchspeed == 0.0f) {
                *traction = maxtraction;
                *sideforce = 0.0f;
            } else {
                *traction = -maxtraction * patchvel / patchspeed;
                *sideforce = -maxtraction * tirev[YCOMP] / patchspeed;
            }
            tire->slipflag = SLIP_ACCEL_SPIN;
            return;
        }
    }
    /* Check for wheel spin - decelerating */
    else if (tire->angvel < roadangvel) {
        tire->sliptorque = -maxtorque;
        tire->angvel += (realtorque - tire->sliptorque) * tire->invmi * m->dt;

        if (tire->angvel >= roadangvel) {
            tire->angvel = roadangvel;
            tire->slipflag = SLIP_DECEL_SPIN;
        } else {
            patchvel = tirev[XCOMP] - tire->angvel * tire->tradius;
            patchspeed = xxsqrt(patchvel * patchvel + tirev[YCOMP] * tirev[YCOMP]);

            if (patchspeed == 0.0f) {
                *traction = -maxtraction;
                *sideforce = 0.0f;
            } else {
                *traction = -maxtraction * patchvel / patchspeed;
                *sideforce = -maxtraction * tirev[YCOMP] / patchspeed;
            }
            tire->slipflag = SLIP_BRAKE_SPIN;
            return;
        }
    }

    tire->sliptorque = 0.0f;

    /* No normal force - tire in air */
    if (normalforce <= 0.0f) {
        *traction = 0.0f;
        *sideforce = 0.0f;
        tire->slipflag += SLIP_NO_LOAD;
        tire->angvel += realtorque * tire->invmi * m->dt;
        return;
    }

    /* Handle driven torque */
    if (torque != 0.0f) {
        /* Torque exceeds max - full slip */
        if (torque >= maxtorque) {
            *traction = maxtraction;
            *sideforce = 0.0f;
            tire->slipflag += SLIP_MAX_ACCEL;
            tire->sliptorque = maxtorque;
            tire->angvel += (realtorque - tire->sliptorque) * tire->invmi * m->dt;
            return;
        }

        if (torque <= -maxtorque) {
            *traction = -maxtraction;
            *sideforce = 0.0f;
            tire->slipflag += SLIP_MAX_BRAKE;
            tire->sliptorque = -maxtorque;
            tire->angvel += (realtorque - tire->sliptorque) * tire->invmi * m->dt;
            return;
        }

        /* Partial torque - adjust lateral capacity */
        *traction = torque / tire->tradius;
        temp = *traction / normalforce;

        Cfmax = xxsqrt(tire->Cfmax * tire->Cfmax - temp * temp);
        k2 = tire->k1 * tire->k1 / (3.0f * Cfmax);
        k3 = tire->k1 * tire->k1 * tire->k1 / (27.0f * Cfmax * Cfmax);
        Afmax = 3.0f * Cfmax / tire->k1;
    } else {
        *traction = 0.0f;
        k2 = tire->k2;
        k3 = tire->k3;
        Cfmax = tire->Cfmax;
        Afmax = tire->Afmax;
    }

    l2 = k2;
    l3 = k3;
    maxf = Cfmax * normalforce;

    alpha = calcalpha(tirev);
    tire->sliptorque = 0.0f;

    /* Calculate lateral force based on slip angle */
    if (alpha >= 0.0f) {
        if (alpha >= Afmax) {
            /* Beyond max slip - sliding */
            ydot = -tirev[YCOMP];
            tire->patchy += ydot * m->dt;

            *sideforce = tire->springK * tire->patchy + tire->rubdamp * ydot;

            if (*sideforce < -maxf) {
                *sideforce = -maxf;
                tire->patchy = *sideforce / tire->springK;
            }
            tire->slipflag += SLIP_SIDE_MAX_POS;
            tire->sliptorque = *traction * tire->tradius;
            tire->angvel += (realtorque - tire->sliptorque) * tire->invmi * m->dt;
            return;
        }

        tire->slipflag += SLIP_SIDE_GRIP_POS;
        p = tirev[XCOMP] * tire->springK /
            (normalforce * (tire->k1 - l2 * alpha + l3 * alpha * alpha));
        if (p < 0.0f) p = -p;

        if (p * m->dt < 0.5f) {
            /* Transient response */
            ydot = -p * tire->patchy - tirev[YCOMP];
            tire->patchy += ydot * m->dt;
            *sideforce = tire->springK * tire->patchy + tire->rubdamp * ydot;
            tire->slipflag += 100;  /* Arcade: transient marker */
        } else {
            /* Steady state - polynomial tire curve */
            *sideforce = -(tire->k1 * alpha - k2 * alpha * alpha +
                           k3 * alpha * alpha * alpha) * normalforce;
            tire->patchy = *sideforce / tire->springK;
            tire->slipflag += 200;  /* Arcade: steady state marker */
        }
    } else {
        /* Negative slip angle */
        if (alpha <= -Afmax) {
            ydot = -tirev[YCOMP];
            tire->patchy += ydot * m->dt;

            *sideforce = tire->springK * tire->patchy + tire->rubdamp * ydot;

            if (*sideforce > maxf) {
                *sideforce = maxf;
                tire->patchy = *sideforce / tire->springK;
            }
            tire->slipflag += SLIP_SIDE_MAX_NEG;
            tire->sliptorque = *traction * tire->tradius;
            tire->angvel += (realtorque - tire->sliptorque) * tire->invmi * m->dt;
            return;
        }

        tire->slipflag += SLIP_SIDE_GRIP_NEG;
        p = tirev[XCOMP] * tire->springK /
            (normalforce * (tire->k1 + l2 * alpha + l3 * alpha * alpha));
        if (p < 0.0f) p = -p;

        if (p * m->dt < 0.5f) {
            ydot = -p * tire->patchy - tirev[YCOMP];
            tire->patchy += ydot * m->dt;
            *sideforce = tire->springK * tire->patchy + tire->rubdamp * ydot;
            tire->slipflag += 100;  /* Arcade: transient marker */
        } else {
            *sideforce = (tire->k1 * alpha + k2 * alpha * alpha +
                          k3 * alpha * alpha * alpha) * normalforce;
            tire->patchy = *sideforce / tire->springK;
            tire->slipflag += 200;  /* Arcade: steady state marker */
        }
    }
}

/**
 * frictioncircle - Arcade-compatible alias
 * Based on arcade: tires.c:frictioncircle()
 */
void frictioncircle(
    ModelDat *m,
    f32 tirev[3],
    f32 normalforce,
    f32 torque,
    TireDes *tire,
    f32 *sfp,
    f32 *trp
) {
    tire_friction_circle(m, tirev, normalforce, torque, tire, sfp, trp);
}

/* ========================================================================
 * MAIN TIRE FORCE CALCULATION
 * ======================================================================== */

/**
 * Calculate all tire forces
 */
void tire_calc_force(
    ModelDat *m,
    TireState *tire,
    f32 torque,
    f32 springrate,
    f32 arspringrate,
    f32 cdamping,
    f32 rdamping,
    f32 other_suscomp,
    f32 airfact
) {
    f32 tirev[3];
    f32 suscomp, damping, arforce, normal;
    f32 sideforce, traction;
    s32 i;

    /* Copy tire velocity */
    for (i = 0; i < 3; i++) {
        tirev[i] = tire->tirev[i];
    }

    /* Reset surface parameters if needed */
    if (tire->params.Cstiff != tire->params.PaveCstiff) {
        tire->params.Cstiff = tire->params.PaveCstiff;
        tire->params.Cfmax = tire->params.PaveCfmax;
        tire_constants(&tire->params);
    }

    suscomp = tire->suspension.compression;

    /* Anti-roll bar force */
    if (suscomp > 0.0f && other_suscomp > 0.0f) {
        arforce = (suscomp - other_suscomp) * arspringrate;
    } else {
        arforce = 0.0f;
    }

    /* Select compression/rebound damping */
    if (tirev[ZCOMP] > 0.0f) {
        damping = cdamping;
    } else {
        damping = rdamping;
    }

    /* Calculate vertical force from suspension */
    if (suscomp > 10.0f) {
        /* Bump stop - high compression */
        if (tirev[ZCOMP] > -1.0f) {
            tire->force.force[ZCOMP] = (tirev[ZCOMP] + 1.0f) * m->mass * -0.25f * m->idt;
        } else {
            tire->force.force[ZCOMP] = arforce + suscomp * springrate + damping * tirev[ZCOMP];
        }
    } else if (suscomp > 0.0f) {
        tire->force.force[ZCOMP] = arforce + suscomp * springrate + damping * tirev[ZCOMP];
    } else {
        tire->force.force[ZCOMP] = 0.0f;
    }

    normal = -tire->force.force[ZCOMP];

    /* Clamp normal force */
    if (normal < 0.0f) {
        normal = 0.0f;
        tire->force.force[ZCOMP] = 0.0f;
    } else if (normal > m->weight) {
        normal = m->weight;
    }

    tire->force.normal = normal;

    /* Calculate traction and lateral forces */
    tire_friction_circle(m, tirev, normal, torque, &tire->params, &sideforce, &traction);

    tire->force.sideforce = sideforce;
    tire->force.traction = traction;
    tire->force.force[XCOMP] = traction;
    tire->force.force[YCOMP] = sideforce;
    tire->force.slipflag = tire->params.slipflag;

    /* Store for sound */
    tire->params.sideforce = sideforce;
    tire->params.traction = traction;
}

/* ========================================================================
 * SURFACE HANDLING
 * ======================================================================== */

/**
 * Get friction modifier for surface type
 */
f32 tire_surface_friction(u8 surface) {
    switch (surface) {
        case SURFACE_PAVEMENT:
            return 1.0f;
        case SURFACE_DIRT:
            return 0.7f;
        case SURFACE_GRASS:
            return 0.6f;
        case SURFACE_SAND:
            return 0.5f;
        case SURFACE_ICE:
            return 0.2f;
        case SURFACE_WATER:
            return 0.4f;
        default:
            return 1.0f;
    }
}

/**
 * Update tire constants for new surface
 */
void tire_update_constants(TireState *tire, u8 surface) {
    f32 friction;

    tire->surface = surface;
    friction = tire_surface_friction(surface);

    tire->params.Cstiff = tire->params.PaveCstiff * friction;
    tire->params.Cfmax = tire->params.PaveCfmax * friction;

    tire_constants(&tire->params);
}

/* ========================================================================
 * SOUND INTERFACE
 * ======================================================================== */

/**
 * Get screech level for sound (0-100)
 */
s32 tire_get_screech_level(TireState *tire) {
    f32 slip = tire->params.sideforce;
    f32 max = tire->params.Cfmax * tire->force.normal;

    if (max <= 0.0f) return 0;
    if (slip < 0.0f) slip = -slip;

    slip = (slip / max) * 100.0f;
    if (slip > 100.0f) slip = 100.0f;

    return (s32)slip;
}

/**
 * Check if tire is spinning
 */
s32 tire_is_spinning(TireState *tire) {
    return (tire->force.slipflag >= SLIP_ACCEL_SPIN &&
            tire->force.slipflag <= SLIP_BRAKE_SPIN);
}
