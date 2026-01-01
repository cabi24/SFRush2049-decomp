/**
 * wings.c - Wing deployment and aerodynamic system for Rush 2049 N64
 *
 * Rush 2049's signature feature: deployable wings for stunt control.
 * Based on arcade drivsym.c aerodynamic model with N64 adaptations.
 *
 * The arcade Rush The Rock uses this aerodynamic formula for drag:
 *   D = srefpcybo2 * V^2 (where srefpcybo2 = 0.0087)
 *   This lumps coefficient, frontal area, and air density.
 *
 * Rush 2049 extends this with deployable wing surfaces that add:
 *   - Additional lift to counteract gravity during jumps
 *   - Variable drag based on wing angle
 *   - Control surfaces for pitch/roll authority
 */

#include "types.h"
#include "game/wings.h"
#include "game/sound.h"

/* External math functions */
extern f32 sqrtf(f32 x);

/* Global wing enable flag */
s32 wing_enabled = 1;

/* ============================================================================
 * Wing Initialization
 * ============================================================================ */

#ifdef NON_MATCHING
/**
 * wing_init - Initialize wing system
 */
void wing_init(void) {
    wing_enabled = 1;
}

/**
 * wing_reset - Reset wing state for a car
 */
void wing_reset(void *car) {
    s32 *wingState;
    f32 *wingTimer;
    f32 *wingAngle;
    f32 *liftCoef;
    f32 *dragCoef;
    s32 *damage;
    s32 *lockout;

    if (car == NULL) {
        return;
    }

    wingState = (s32 *)((u8 *)car + WING_OFFSET_STATE);
    wingTimer = (f32 *)((u8 *)car + WING_OFFSET_TIMER);
    wingAngle = (f32 *)((u8 *)car + WING_OFFSET_ANGLE);
    liftCoef = (f32 *)((u8 *)car + WING_OFFSET_LIFT_COEF);
    dragCoef = (f32 *)((u8 *)car + WING_OFFSET_DRAG_COEF);
    damage = (s32 *)((u8 *)car + WING_OFFSET_DAMAGE);
    lockout = (s32 *)((u8 *)car + WING_OFFSET_LOCKOUT);

    *wingState = WING_STATE_RETRACTED;
    *wingTimer = 0.0f;
    *wingAngle = 0.0f;
    *liftCoef = WING_CL_BASE;
    *dragCoef = WING_CD_BASE;
    *damage = 0;
    *lockout = 0;
}
#endif

/* ============================================================================
 * Wing Deployment Control
 * ============================================================================ */

#ifdef NON_MATCHING
/**
 * wing_deploy - Begin deploying or retracting wings
 * Address: 0x800F6894 (identified from game.c)
 * Size: ~580 bytes
 *
 * This function handles the player input for wing deployment.
 * When the wing button is pressed, wings begin deploying.
 * When released, wings begin retracting.
 */
void wing_deploy(void *car, s32 deploy) {
    s32 *wingState;
    s32 *wingTimer;
    f32 *liftForce;
    f32 *dragCoeff;
    f32 *carVel;
    f32 speed;

    if (car == NULL) {
        return;
    }

    /* Use alternate offsets matching game.c implementation */
    wingState = (s32 *)((u8 *)car + WING_OFFSET_STATE_ALT);
    wingTimer = (s32 *)((u8 *)car + WING_OFFSET_TIMER_ALT);
    liftForce = (f32 *)((u8 *)car + WING_OFFSET_LIFT_ALT);
    dragCoeff = (f32 *)((u8 *)car + WING_OFFSET_DRAG_ALT);
    carVel = (f32 *)((u8 *)car + 0x34);

    if (deploy) {
        /* Deploy wings */
        if (*wingState == 0) {
            *wingState = 1;
            *wingTimer = 0;

            /* Play deploy sound */
            sound_play(SFX_WING_DEPLOY);
        }

        /* Animate wing extension (takes WING_DEPLOY_TIME frames) */
        if (*wingTimer < WING_DEPLOY_TIME) {
            (*wingTimer)++;
        }

        /* Calculate speed magnitude */
        speed = sqrtf(carVel[0] * carVel[0] +
                      carVel[1] * carVel[1] +
                      carVel[2] * carVel[2]);

        /* Calculate lift based on speed and wing extension */
        /* Lift force increases with speed and wing deployment */
        *liftForce = speed * WING_LIFT_SCALE * ((f32)(*wingTimer) / (f32)WING_DEPLOY_TIME);

        /* Clamp lift force */
        if (*liftForce > WING_MAX_LIFT) {
            *liftForce = WING_MAX_LIFT;
        }

        /* Wings add drag - increases with deployment */
        *dragCoeff = WING_CD_BASE +
                     (WING_CD_DEPLOYED - WING_CD_BASE) *
                     ((f32)(*wingTimer) / (f32)WING_DEPLOY_TIME);
    } else {
        /* Retract wings */
        if (*wingState == 1) {
            *wingState = 0;

            /* Play retract sound */
            sound_play(SFX_WING_RETRACT);
        }

        /* Animate wing retraction */
        if (*wingTimer > 0) {
            (*wingTimer)--;
        }

        /* Calculate speed for lift reduction */
        speed = sqrtf(carVel[0] * carVel[0] +
                      carVel[1] * carVel[1] +
                      carVel[2] * carVel[2]);

        /* Reduce lift as wings retract */
        *liftForce = speed * WING_LIFT_SCALE * ((f32)(*wingTimer) / (f32)WING_DEPLOY_TIME);

        /* Reduce drag as wings retract */
        *dragCoeff = WING_CD_BASE +
                     (WING_CD_DEPLOYED - WING_CD_BASE) *
                     ((f32)(*wingTimer) / (f32)WING_DEPLOY_TIME);
    }
}
#endif

#ifdef NON_MATCHING
/**
 * wing_deploy_update - Update wing deployment animation
 * Address: (from game.c around line 40725)
 * Size: ~1780 bytes
 *
 * Animates wing deployment and calculates aerodynamic coefficients.
 * Called each frame when wings are deploying or deployed.
 */
void wing_deploy_update(void *car) {
    s32 *wingState;
    f32 *deployTimer;
    f32 *wingAngle;
    f32 *liftCoef;
    f32 *dragCoef;
    f32 dt;
    f32 deployRate;

    dt = 0.016f;  /* 1/60 second */
    deployRate = 4.0f;  /* Full deploy in 0.25 seconds */

    if (car == NULL) {
        return;
    }

    wingState = (s32 *)((u8 *)car + WING_OFFSET_STATE);
    deployTimer = (f32 *)((u8 *)car + WING_OFFSET_TIMER);
    wingAngle = (f32 *)((u8 *)car + WING_OFFSET_ANGLE);
    liftCoef = (f32 *)((u8 *)car + WING_OFFSET_LIFT_COEF);
    dragCoef = (f32 *)((u8 *)car + WING_OFFSET_DRAG_COEF);

    /* Start deploy if not already deploying/deployed */
    if (*wingState == WING_STATE_RETRACTED) {
        *wingState = WING_STATE_DEPLOYING;
        *deployTimer = 0.0f;
    }

    /* Animate wing deployment */
    if (*wingState == WING_STATE_DEPLOYING) {
        *wingAngle += deployRate * dt;
        *deployTimer += dt;

        if (*wingAngle >= 1.0f) {
            *wingAngle = 1.0f;
            *wingState = WING_STATE_DEPLOYED;
        }
    }

    /* Calculate aerodynamic coefficients based on wing angle */
    /* Lift coefficient: Cl = base + angle * (max - base) */
    *liftCoef = WING_CL_BASE + (*wingAngle * (WING_CL_MAX - WING_CL_BASE));

    /* Drag coefficient: Cd = base + angle * (deployed - base) */
    *dragCoef = WING_CD_BASE + (*wingAngle * (WING_CD_DEPLOYED - WING_CD_BASE));

    /* Apply wing physics if deployed */
    if (*wingState == WING_STATE_DEPLOYED) {
        glide_physics_apply(car);
    }
}
#endif

#ifdef NON_MATCHING
/**
 * wing_retract - Retract the car's stunt wings
 * Address: 0x80105B74
 * Size: ~572 bytes
 *
 * Called when:
 *   - Player releases wing button
 *   - Car lands
 *   - Car crashes
 */
void wing_retract(void *car) {
    s32 *wingState;
    f32 *wingAngle;
    f32 *liftCoef;
    f32 *dragCoef;
    f32 dt;
    f32 retractRate;

    dt = 0.016f;
    retractRate = 6.0f;  /* Faster retract than deploy */

    if (car == NULL) {
        return;
    }

    wingState = (s32 *)((u8 *)car + WING_OFFSET_STATE);
    wingAngle = (f32 *)((u8 *)car + WING_OFFSET_ANGLE);
    liftCoef = (f32 *)((u8 *)car + WING_OFFSET_LIFT_COEF);
    dragCoef = (f32 *)((u8 *)car + WING_OFFSET_DRAG_COEF);

    /* Skip if already retracted */
    if (*wingState == WING_STATE_RETRACTED) {
        return;
    }

    /* Mark as retracting */
    if (*wingState == WING_STATE_DEPLOYED) {
        *wingState = WING_STATE_RETRACTING;
    }

    /* Animate wing retraction */
    *wingAngle -= retractRate * dt;

    if (*wingAngle <= 0.0f) {
        *wingAngle = 0.0f;
        *wingState = WING_STATE_RETRACTED;
        *liftCoef = WING_CL_BASE;
        *dragCoef = WING_CD_BASE;
    } else {
        /* Update coefficients during retraction */
        *liftCoef = WING_CL_BASE + (*wingAngle * (WING_CL_MAX - WING_CL_BASE));
        *dragCoef = WING_CD_BASE + (*wingAngle * (WING_CD_DEPLOYED - WING_CD_BASE));
    }
}
#endif

#ifdef NON_MATCHING
/**
 * wing_update - Main per-frame wing update
 *
 * Combines state management, animation, and physics.
 */
void wing_update(void *car) {
    s32 *wingState;
    s32 *damage;
    s32 *lockout;

    if (car == NULL || !wing_enabled) {
        return;
    }

    wingState = (s32 *)((u8 *)car + WING_OFFSET_STATE);
    damage = (s32 *)((u8 *)car + WING_OFFSET_DAMAGE);
    lockout = (s32 *)((u8 *)car + WING_OFFSET_LOCKOUT);

    /* Handle damage lockout */
    if (*lockout > 0) {
        (*lockout)--;
        return;
    }

    /* Handle damaged wings */
    if (*wingState == WING_STATE_DAMAGED) {
        if (*damage < 50) {
            /* Minor damage - can still deploy but reduced effectiveness */
            *wingState = WING_STATE_RETRACTED;
        }
        return;
    }

    /* Update based on current state */
    switch (*wingState) {
        case WING_STATE_DEPLOYING:
            wing_deploy_update(car);
            break;

        case WING_STATE_DEPLOYED:
            glide_physics_apply(car);
            break;

        case WING_STATE_RETRACTING:
            wing_retract(car);
            break;

        case WING_STATE_RETRACTED:
        default:
            /* Nothing to do */
            break;
    }
}
#endif

/* ============================================================================
 * Wing Physics
 * Based on arcade drivsym.c aerodynamic calculations
 * ============================================================================ */

#ifdef NON_MATCHING
/**
 * wing_calc_lift - Calculate lift force from wings
 *
 * Implements the standard lift equation:
 *   L = 0.5 * rho * V^2 * S * Cl
 *
 * Where:
 *   rho = air density (0.002377 slug/ft^3)
 *   V = airspeed (ft/s)
 *   S = wing area (sq ft)
 *   Cl = lift coefficient
 */
void wing_calc_lift(void *car, f32 *out_lift) {
    f32 *vel;
    f32 *wingAngle;
    f32 liftCoef;
    f32 speed;
    f32 speedSq;
    f32 wingArea;
    f32 liftMag;

    if (car == NULL || out_lift == NULL) {
        return;
    }

    vel = (f32 *)((u8 *)car + 0x34);
    wingAngle = (f32 *)((u8 *)car + WING_OFFSET_ANGLE);
    liftCoef = *(f32 *)((u8 *)car + WING_OFFSET_LIFT_COEF);

    /* Calculate speed squared */
    speedSq = vel[0]*vel[0] + vel[1]*vel[1] + vel[2]*vel[2];
    speed = sqrtf(speedSq);

    /* No lift at very low speeds */
    if (speed < WING_MIN_SPEED) {
        *out_lift = 0.0f;
        return;
    }

    /* Wing area scales with deployment */
    wingArea = WING_AREA_BASE + (*wingAngle * (WING_AREA_DEPLOYED - WING_AREA_BASE));

    /* L = 0.5 * rho * V^2 * S * Cl */
    liftMag = 0.5f * WING_AIR_DENSITY * speedSq * wingArea * liftCoef;

    /* Scale for game physics */
    liftMag *= WING_LIFT_SCALE;

    /* Clamp to max lift */
    if (liftMag > WING_MAX_LIFT) {
        liftMag = WING_MAX_LIFT;
    }

    *out_lift = liftMag;
}
#endif

#ifdef NON_MATCHING
/**
 * wing_calc_drag - Calculate drag force from wings
 *
 * Implements the standard drag equation:
 *   D = 0.5 * rho * V^2 * S * Cd
 *
 * The arcade uses srefpcybo2 = 0.0087 which combines:
 *   0.5 * rho * Cd * S = 0.0087
 *
 * For wings, we add additional drag based on wing angle.
 */
void wing_calc_drag(void *car, f32 *out_drag) {
    f32 *vel;
    f32 *wingAngle;
    f32 dragCoef;
    f32 speedSq;
    f32 wingArea;
    f32 dragMag;

    if (car == NULL || out_drag == NULL) {
        return;
    }

    vel = (f32 *)((u8 *)car + 0x34);
    wingAngle = (f32 *)((u8 *)car + WING_OFFSET_ANGLE);
    dragCoef = *(f32 *)((u8 *)car + WING_OFFSET_DRAG_COEF);

    /* Calculate speed squared */
    speedSq = vel[0]*vel[0] + vel[1]*vel[1] + vel[2]*vel[2];

    /* Wing area scales with deployment */
    wingArea = WING_AREA_BASE + (*wingAngle * (WING_AREA_DEPLOYED - WING_AREA_BASE));

    /* D = 0.5 * rho * V^2 * S * Cd */
    dragMag = 0.5f * WING_AIR_DENSITY * speedSq * wingArea * dragCoef;

    /* Scale for game physics */
    dragMag *= WING_DRAG_SCALE;

    /* Clamp to max drag */
    if (dragMag > WING_MAX_DRAG) {
        dragMag = WING_MAX_DRAG;
    }

    *out_drag = dragMag;
}
#endif

#ifdef NON_MATCHING
/**
 * glide_physics_apply - Apply wing aerodynamic forces
 * Address: 0x80105EA8
 * Size: ~2508 bytes
 *
 * When wings are deployed, the car experiences:
 *   - Lift force (reduces fall rate) - applied in up direction
 *   - Drag force (reduces forward speed) - applied opposite to velocity
 *   - Control forces (pitch/roll from player input)
 *
 * This enables the "gliding" behavior unique to Rush 2049.
 *
 * Based on arcade drivsym.c:474-516 drag force calculation:
 *   if(m->V[XCOMP] > 0)
 *     m->D[XCOMP] = -(30+ m->rollresist + m->srefpcybo2 * m->V[XCOMP] * m->V[XCOMP]);
 */
void glide_physics_apply(void *car) {
    f32 *vel;
    f32 *angVel;
    f32 liftCoef, dragCoef;
    f32 wingAngle;
    f32 speed, speedSq;
    f32 liftForce, dragForce;
    f32 pitchControl, rollControl;
    f32 dt;
    f32 wingArea;
    f32 dragScale;

    dt = 0.016f;  /* 1/60 second timestep */

    if (car == NULL) {
        return;
    }

    vel = (f32 *)((u8 *)car + 0x34);
    angVel = (f32 *)((u8 *)car + 0x84);
    wingAngle = *(f32 *)((u8 *)car + WING_OFFSET_ANGLE);
    liftCoef = *(f32 *)((u8 *)car + WING_OFFSET_LIFT_COEF);
    dragCoef = *(f32 *)((u8 *)car + WING_OFFSET_DRAG_COEF);

    /* Calculate speed */
    speedSq = vel[0]*vel[0] + vel[1]*vel[1] + vel[2]*vel[2];
    speed = sqrtf(speedSq);

    /* Too slow for aerodynamic effects */
    if (speed < WING_MIN_SPEED) {
        return;
    }

    /* Calculate effective wing area */
    wingArea = WING_AREA_BASE + (wingAngle * (WING_AREA_DEPLOYED - WING_AREA_BASE));

    /* ==============================
     * Lift Force Calculation
     * L = 0.5 * rho * V^2 * S * Cl
     * ============================== */
    liftForce = 0.5f * WING_AIR_DENSITY * speedSq * wingArea * liftCoef * wingAngle;

    /* Reduce vertical velocity (counteract gravity) */
    /* Apply lift in Y direction (up) */
    vel[1] += liftForce * dt * 0.01f;

    /* Clamp to prevent flying upward uncontrollably */
    if (vel[1] > 0.0f) {
        vel[1] *= 0.95f;  /* Dampen upward motion */
    }

    /* ==============================
     * Drag Force Calculation
     * D = 0.5 * rho * V^2 * S * Cd
     * Similar to arcade: srefpcybo2 * V^2
     * ============================== */
    dragForce = 0.5f * WING_AIR_DENSITY * speedSq * wingArea * dragCoef * wingAngle;

    /* Apply drag opposite to velocity direction */
    if (speed > 0.0f) {
        dragScale = 1.0f - (dragForce * dt * 0.001f / speed);

        /* Cap drag effect to prevent reversal */
        if (dragScale < 0.9f) {
            dragScale = 0.9f;
        }

        vel[0] *= dragScale;
        vel[2] *= dragScale;
    }

    /* ==============================
     * Control Forces
     * Wings provide pitch/roll control
     * ============================== */

    /* Get control inputs from angular velocity requests */
    pitchControl = angVel[0] * wingAngle * WING_PITCH_AUTHORITY;
    rollControl = angVel[2] * wingAngle * WING_ROLL_AUTHORITY;

    /* Apply pitch moment (nose up/down) */
    angVel[0] += pitchControl * dt;

    /* Apply roll moment (barrel roll) */
    angVel[2] += rollControl * dt;

    /* Dampen angular velocities for stability */
    angVel[0] *= WING_DAMPING;
    angVel[1] *= WING_DAMPING;
    angVel[2] *= WING_DAMPING;
}
#endif

#ifdef NON_MATCHING
/**
 * wing_apply_control - Apply player input to wing control surfaces
 */
void wing_apply_control(void *car, f32 pitch_input, f32 roll_input) {
    f32 *angVel;
    f32 wingAngle;
    f32 pitchForce, rollForce;
    f32 dt;

    dt = 0.016f;

    if (car == NULL) {
        return;
    }

    angVel = (f32 *)((u8 *)car + 0x84);
    wingAngle = *(f32 *)((u8 *)car + WING_OFFSET_ANGLE);

    /* Control authority scales with wing deployment */
    pitchForce = pitch_input * wingAngle * WING_PITCH_AUTHORITY;
    rollForce = roll_input * wingAngle * WING_ROLL_AUTHORITY;

    /* Apply to angular velocity */
    angVel[0] += pitchForce * dt;
    angVel[2] += rollForce * dt;
}
#endif

/* ============================================================================
 * Wing Query Functions
 * ============================================================================ */

#ifdef NON_MATCHING
/**
 * wing_state_get - Get current wing state
 * Address: 0x80105DB0
 * Size: ~248 bytes
 */
s32 wing_state_get(void *car) {
    if (car == NULL) {
        return WING_STATE_RETRACTED;
    }
    return *(s32 *)((u8 *)car + WING_OFFSET_STATE);
}
#endif

#ifdef NON_MATCHING
/**
 * wing_angle_get - Get current wing angle
 */
f32 wing_angle_get(void *car) {
    if (car == NULL) {
        return 0.0f;
    }
    return *(f32 *)((u8 *)car + WING_OFFSET_ANGLE);
}
#endif

#ifdef NON_MATCHING
/**
 * wing_is_deployed - Check if wings are fully deployed
 */
s32 wing_is_deployed(void *car) {
    s32 state;

    if (car == NULL) {
        return 0;
    }

    state = *(s32 *)((u8 *)car + WING_OFFSET_STATE);
    return (state == WING_STATE_DEPLOYED) ? 1 : 0;
}
#endif

#ifdef NON_MATCHING
/**
 * wing_is_damaged - Check if wings are damaged
 */
s32 wing_is_damaged(void *car) {
    s32 state;
    s32 damage;

    if (car == NULL) {
        return 0;
    }

    state = *(s32 *)((u8 *)car + WING_OFFSET_STATE);
    damage = *(s32 *)((u8 *)car + WING_OFFSET_DAMAGE);

    return (state == WING_STATE_DAMAGED || damage > 0) ? 1 : 0;
}
#endif

#ifdef NON_MATCHING
/**
 * wing_get_lift_coef - Get current lift coefficient
 */
f32 wing_get_lift_coef(void *car) {
    if (car == NULL) {
        return WING_CL_BASE;
    }
    return *(f32 *)((u8 *)car + WING_OFFSET_LIFT_COEF);
}
#endif

#ifdef NON_MATCHING
/**
 * wing_get_drag_coef - Get current drag coefficient
 */
f32 wing_get_drag_coef(void *car) {
    if (car == NULL) {
        return WING_CD_BASE;
    }
    return *(f32 *)((u8 *)car + WING_OFFSET_DRAG_COEF);
}
#endif

/* ============================================================================
 * Wing Damage Functions
 * ============================================================================ */

#ifdef NON_MATCHING
/**
 * wing_damage - Apply damage to wings
 *
 * Called on crash or bad landing.
 * Damage accumulates and can disable wings.
 */
void wing_damage(void *car, s32 amount) {
    s32 *wingState;
    s32 *damage;
    s32 *lockout;

    if (car == NULL || amount <= 0) {
        return;
    }

    wingState = (s32 *)((u8 *)car + WING_OFFSET_STATE);
    damage = (s32 *)((u8 *)car + WING_OFFSET_DAMAGE);
    lockout = (s32 *)((u8 *)car + WING_OFFSET_LOCKOUT);

    /* Accumulate damage */
    *damage += amount;

    /* Cap at 100 */
    if (*damage > 100) {
        *damage = 100;
    }

    /* Set lockout timer */
    *lockout = WING_DAMAGE_LOCKOUT;

    /* Severe damage disables wings */
    if (*damage >= 50) {
        *wingState = WING_STATE_DAMAGED;

        /* Play damage sound */
        sound_play(SFX_WING_DAMAGE);
    }

    /* Force retract if deploying/deployed */
    if (*wingState == WING_STATE_DEPLOYING || *wingState == WING_STATE_DEPLOYED) {
        wing_retract(car);
    }
}
#endif

#ifdef NON_MATCHING
/**
 * wing_repair - Repair wing damage
 *
 * Called when car respawns or enters pit.
 */
void wing_repair(void *car) {
    s32 *wingState;
    s32 *damage;
    s32 *lockout;

    if (car == NULL) {
        return;
    }

    damage = (s32 *)((u8 *)car + WING_OFFSET_DAMAGE);
    lockout = (s32 *)((u8 *)car + WING_OFFSET_LOCKOUT);
    wingState = (s32 *)((u8 *)car + WING_OFFSET_STATE);

    /* Clear damage */
    *damage = 0;
    *lockout = 0;

    /* Reset to retracted if was damaged */
    if (*wingState == WING_STATE_DAMAGED) {
        *wingState = WING_STATE_RETRACTED;
    }
}
#endif
