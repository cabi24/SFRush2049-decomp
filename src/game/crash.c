/**
 * crash.c - Crash and resurrect handling for Rush 2049 N64
 *
 * Based on arcade game/resurrect.c
 * Handles crash detection, respawning, and damage tracking.
 */

#include "game/crash.h"
#include "game/particles.h"

/* Global crash system */
CrashSystem gCrash;

/* Forward declarations */
static void crash_find_respawn_point(s32 car);
static void crash_start_tumble(s32 car);
static void crash_check_ground(s32 car);

/**
 * Initialize crash system
 */
void crash_init(void) {
    s32 i;

    gCrash.num_cars = 0;
    gCrash.num_respawn_points = 0;

    gCrash.enabled = 1;
    gCrash.damage_enabled = 1;
    gCrash.auto_respawn = 1;
    gCrash.instant_respawn = 0;

    gCrash.damage_mult = 1.0f;
    gCrash.crash_delay = CRASH_DELAY_FRAMES;

    /* Clear all cars */
    for (i = 0; i < 8; i++) {
        crash_init_car(i);
    }

    /* Clear respawn points */
    for (i = 0; i < 32; i++) {
        gCrash.respawn_points[i].active = 0;
    }
}

/**
 * Reset crash system (new race)
 */
void crash_reset(void) {
    s32 i;

    for (i = 0; i < 8; i++) {
        crash_init_car(i);
    }
}

/**
 * Initialize single car crash state
 */
void crash_init_car(s32 car) {
    CrashData *cd;

    if (car < 0 || car >= 8) {
        return;
    }

    cd = &gCrash.cars[car];

    /* State */
    cd->state = CRASH_STATE_NONE;
    cd->cause = CRASH_CAUSE_NONE;
    cd->damage_level = DAMAGE_LEVEL_NONE;
    cd->invincible = 0;

    /* Damage */
    cd->damage = 0.0f;
    cd->max_damage = DAMAGE_THRESHOLD_CRASH;
    cd->last_impact = 0.0f;

    /* Timing */
    cd->state_timer = 0.0f;
    cd->crash_timer = 0.0f;
    cd->invincible_timer = 0.0f;

    /* Physics */
    cd->physics.velocity[0] = 0.0f;
    cd->physics.velocity[1] = 0.0f;
    cd->physics.velocity[2] = 0.0f;
    cd->physics.angular[0] = 0.0f;
    cd->physics.angular[1] = 0.0f;
    cd->physics.angular[2] = 0.0f;
    cd->physics.position[0] = 0.0f;
    cd->physics.position[1] = 0.0f;
    cd->physics.position[2] = 0.0f;
    cd->physics.rotation[0] = 0.0f;
    cd->physics.rotation[1] = 0.0f;
    cd->physics.rotation[2] = 0.0f;
    cd->physics.bounce_count = 0.0f;
    cd->physics.ground_timer = 0.0f;

    /* Respawn */
    cd->respawn.active = 0;
    cd->respawn_alpha = 1.0f;

    /* Stats */
    cd->crash_count = 0;
    cd->flip_count = 0;
    cd->fall_count = 0;
    cd->total_damage = 0.0f;
}

/**
 * Load respawn points from track data
 */
void crash_load_respawn_points(void *track_data) {
    /* Track data format TBD - clear for now */
    crash_clear_respawn_points();
}

/**
 * Clear all respawn points
 */
void crash_clear_respawn_points(void) {
    s32 i;

    for (i = 0; i < 32; i++) {
        gCrash.respawn_points[i].active = 0;
    }

    gCrash.num_respawn_points = 0;
}

/**
 * Add a respawn point
 */
s32 crash_add_respawn_point(f32 *pos, f32 rotation, s32 checkpoint) {
    RespawnPoint *rp;
    s32 i;

    /* Find free slot */
    for (i = 0; i < 32; i++) {
        if (!gCrash.respawn_points[i].active) {
            rp = &gCrash.respawn_points[i];

            rp->position[0] = pos[0];
            rp->position[1] = pos[1];
            rp->position[2] = pos[2];
            rp->rotation = rotation;
            rp->checkpoint = checkpoint;
            rp->active = 1;

            gCrash.num_respawn_points++;
            return i;
        }
    }

    return -1;
}

/**
 * Get nearest respawn point to position
 */
RespawnPoint* crash_get_nearest_respawn(f32 *pos) {
    RespawnPoint *rp, *nearest;
    f32 dx, dy, dz, dist, min_dist;
    s32 i;

    nearest = NULL;
    min_dist = 1e10f;

    for (i = 0; i < 32; i++) {
        rp = &gCrash.respawn_points[i];

        if (!rp->active) {
            continue;
        }

        dx = pos[0] - rp->position[0];
        dy = pos[1] - rp->position[1];
        dz = pos[2] - rp->position[2];
        dist = dx*dx + dy*dy + dz*dz;

        if (dist < min_dist) {
            min_dist = dist;
            nearest = rp;
        }
    }

    return nearest;
}

/**
 * Get respawn point for specific checkpoint
 */
RespawnPoint* crash_get_checkpoint_respawn(s32 checkpoint) {
    RespawnPoint *rp;
    s32 i;

    for (i = 0; i < 32; i++) {
        rp = &gCrash.respawn_points[i];

        if (rp->active && rp->checkpoint == checkpoint) {
            return rp;
        }
    }

    return NULL;
}

/**
 * Update crash system
 */
void crash_update(void) {
    s32 i;

    if (!gCrash.enabled) {
        return;
    }

    for (i = 0; i < gCrash.num_cars; i++) {
        crash_update_car(i);
    }
}

/**
 * Update single car crash state
 */
void crash_update_car(s32 car) {
    CrashData *cd;

    if (car < 0 || car >= 8) {
        return;
    }

    cd = &gCrash.cars[car];

    /* Update invincibility timer */
    if (cd->invincible && cd->invincible_timer > 0.0f) {
        cd->invincible_timer -= 1.0f;
        if (cd->invincible_timer <= 0.0f) {
            cd->invincible_timer = 0.0f;
            cd->invincible = 0;
        }
    }

    /* Update based on state */
    switch (cd->state) {
        case CRASH_STATE_NONE:
            /* Normal - nothing to do */
            break;

        case CRASH_STATE_SPINNING:
        case CRASH_STATE_FLIPPING:
        case CRASH_STATE_AIRBORNE:
            crash_update_physics(car);
            break;

        case CRASH_STATE_EXPLODING:
            cd->state_timer -= 1.0f;
            if (cd->state_timer <= 0.0f) {
                cd->state = CRASH_STATE_WAITING;
                cd->state_timer = gCrash.crash_delay;
            }
            break;

        case CRASH_STATE_WAITING:
            cd->state_timer -= 1.0f;
            if (cd->state_timer <= 0.0f && gCrash.auto_respawn) {
                crash_begin_respawn(car);
            }
            break;

        case CRASH_STATE_RESPAWNING:
            crash_update_respawn(car);
            break;
    }

    /* Update crash timer */
    if (cd->state != CRASH_STATE_NONE) {
        cd->crash_timer += 1.0f;
    }
}

/**
 * Update crash physics (tumbling car)
 */
void crash_update_physics(s32 car) {
    CrashData *cd;
    CrashPhysics *phys;
    f32 gravity = -32.0f;   /* ft/s^2 */
    f32 drag = 0.98f;
    f32 angular_drag = 0.95f;

    cd = &gCrash.cars[car];
    phys = &cd->physics;

    /* Apply gravity */
    phys->velocity[1] += gravity / 60.0f;

    /* Apply drag */
    phys->velocity[0] *= drag;
    phys->velocity[2] *= drag;

    /* Update position */
    phys->position[0] += phys->velocity[0] / 60.0f;
    phys->position[1] += phys->velocity[1] / 60.0f;
    phys->position[2] += phys->velocity[2] / 60.0f;

    /* Update rotation (tumble) */
    phys->rotation[0] += phys->angular[0] / 60.0f;
    phys->rotation[1] += phys->angular[1] / 60.0f;
    phys->rotation[2] += phys->angular[2] / 60.0f;

    /* Apply angular drag */
    phys->angular[0] *= angular_drag;
    phys->angular[1] *= angular_drag;
    phys->angular[2] *= angular_drag;

    /* Check for ground collision */
    crash_check_ground(car);

    /* Transition to waiting after settling */
    if (cd->state == CRASH_STATE_SPINNING || cd->state == CRASH_STATE_FLIPPING) {
        phys->ground_timer += 1.0f;
        if (phys->ground_timer > 60.0f) {  /* 1 second on ground */
            cd->state = CRASH_STATE_WAITING;
            cd->state_timer = gCrash.crash_delay;
        }
    }
}

/**
 * Check ground collision during crash
 */
static void crash_check_ground(s32 car) {
    CrashData *cd;
    CrashPhysics *phys;
    f32 ground_height = 0.0f;  /* Would query track for actual height */
    f32 bounce = 0.5f;

    cd = &gCrash.cars[car];
    phys = &cd->physics;

    if (phys->position[1] < ground_height) {
        /* Hit ground */
        phys->position[1] = ground_height;

        /* Bounce */
        if (phys->velocity[1] < -2.0f) {
            phys->velocity[1] = -phys->velocity[1] * bounce;
            phys->bounce_count += 1.0f;

            /* Add random tumble on bounce */
            phys->angular[0] += (phys->velocity[2] * 0.1f);
            phys->angular[2] += (phys->velocity[0] * 0.1f);
        } else {
            phys->velocity[1] = 0.0f;
            phys->ground_timer += 1.0f;
        }
    }
}

/**
 * Update respawn animation
 */
void crash_update_respawn(s32 car) {
    CrashData *cd;

    cd = &gCrash.cars[car];

    cd->state_timer -= 1.0f;

    /* Fade in */
    cd->respawn_alpha = 1.0f - (cd->state_timer / RESURRECT_FRAMES);
    if (cd->respawn_alpha > 1.0f) {
        cd->respawn_alpha = 1.0f;
    }

    if (cd->state_timer <= 0.0f) {
        crash_complete_respawn(car);
    }
}

/**
 * Trigger a crash
 */
void crash_trigger(s32 car, s32 cause) {
    CrashData *cd;

    if (car < 0 || car >= 8) {
        return;
    }

    cd = &gCrash.cars[car];

    /* Don't crash if invincible or already crashed */
    if (cd->invincible || cd->state != CRASH_STATE_NONE) {
        return;
    }

    /* Set crash state */
    cd->state = CRASH_STATE_EXPLODING;
    cd->cause = cause;
    cd->state_timer = 30.0f;  /* 0.5 second explosion */
    cd->crash_timer = 0.0f;

    /* Update stats */
    cd->crash_count++;

    /* Find respawn point */
    crash_find_respawn_point(car);

    /* Spawn visual effects */
    crash_spawn_effects(car, cause);
}

/**
 * Crash from collision impact
 */
void crash_from_collision(s32 car, f32 impact_force, f32 *impact_dir) {
    CrashData *cd;
    f32 damage;

    if (car < 0 || car >= 8) {
        return;
    }

    cd = &gCrash.cars[car];

    /* Calculate damage */
    damage = DAMAGE_PER_IMPACT + (impact_force * DAMAGE_SPEED_MULT);
    damage *= gCrash.damage_mult;

    /* Apply damage */
    crash_apply_damage(car, damage);
    cd->last_impact = impact_force;

    /* Start tumble physics */
    if (cd->state == CRASH_STATE_NONE && cd->damage >= cd->max_damage) {
        crash_start_tumble(car);
        crash_trigger(car, CRASH_CAUSE_COLLISION);
    }
}

/**
 * Crash from flipping over
 */
void crash_from_flip(s32 car) {
    CrashData *cd;

    if (car < 0 || car >= 8) {
        return;
    }

    cd = &gCrash.cars[car];
    cd->flip_count++;

    crash_trigger(car, CRASH_CAUSE_FLIP);

    /* Set flipping state */
    cd->state = CRASH_STATE_FLIPPING;
}

/**
 * Crash from falling off track
 */
void crash_from_fall(s32 car) {
    CrashData *cd;

    if (car < 0 || car >= 8) {
        return;
    }

    cd = &gCrash.cars[car];
    cd->fall_count++;

    crash_trigger(car, CRASH_CAUSE_FALL);

    /* Set airborne state */
    cd->state = CRASH_STATE_AIRBORNE;
}

/**
 * Crash from water
 */
void crash_from_water(s32 car) {
    crash_trigger(car, CRASH_CAUSE_WATER);
}

/**
 * Crash from explosion (battle mode)
 */
void crash_from_explosion(s32 car) {
    crash_trigger(car, CRASH_CAUSE_EXPLOSION);
}

/**
 * Start tumble physics
 */
static void crash_start_tumble(s32 car) {
    CrashData *cd;
    CrashPhysics *phys;

    cd = &gCrash.cars[car];
    phys = &cd->physics;

    /* Copy current car velocity */
    /* In real implementation, would get from car_array */
    phys->velocity[0] = 0.0f;
    phys->velocity[1] = 20.0f;  /* Launch upward */
    phys->velocity[2] = 0.0f;

    /* Random tumble */
    phys->angular[0] = 180.0f + (car * 45.0f);  /* Different per car */
    phys->angular[1] = 90.0f;
    phys->angular[2] = 120.0f;

    phys->bounce_count = 0.0f;
    phys->ground_timer = 0.0f;
}

/**
 * Find appropriate respawn point
 */
static void crash_find_respawn_point(s32 car) {
    CrashData *cd;
    RespawnPoint *rp;
    f32 pos[3] = {0.0f, 0.0f, 0.0f};

    cd = &gCrash.cars[car];

    /* In real implementation, would get car position */
    rp = crash_get_nearest_respawn(pos);

    if (rp) {
        cd->respawn.position[0] = rp->position[0];
        cd->respawn.position[1] = rp->position[1];
        cd->respawn.position[2] = rp->position[2];
        cd->respawn.rotation = rp->rotation;
        cd->respawn.checkpoint = rp->checkpoint;
        cd->respawn.active = 1;
    } else {
        /* Default respawn at origin */
        cd->respawn.position[0] = 0.0f;
        cd->respawn.position[1] = 0.0f;
        cd->respawn.position[2] = 0.0f;
        cd->respawn.rotation = 0.0f;
        cd->respawn.checkpoint = 0;
        cd->respawn.active = 1;
    }
}

/**
 * Apply damage to car
 */
void crash_apply_damage(s32 car, f32 amount) {
    CrashData *cd;

    if (car < 0 || car >= 8) {
        return;
    }

    if (!gCrash.damage_enabled) {
        return;
    }

    cd = &gCrash.cars[car];

    cd->damage += amount;
    cd->total_damage += amount;

    /* Update damage level */
    if (cd->damage >= cd->max_damage) {
        cd->damage_level = DAMAGE_LEVEL_DESTROYED;
    } else if (cd->damage >= cd->max_damage * 0.8f) {
        cd->damage_level = DAMAGE_LEVEL_CRITICAL;
    } else if (cd->damage >= cd->max_damage * 0.5f) {
        cd->damage_level = DAMAGE_LEVEL_HEAVY;
    } else if (cd->damage >= cd->max_damage * 0.25f) {
        cd->damage_level = DAMAGE_LEVEL_DENT;
    } else if (cd->damage > 0.0f) {
        cd->damage_level = DAMAGE_LEVEL_SCRATCH;
    } else {
        cd->damage_level = DAMAGE_LEVEL_NONE;
    }
}

/**
 * Set damage directly
 */
void crash_set_damage(s32 car, f32 damage) {
    CrashData *cd;

    if (car < 0 || car >= 8) {
        return;
    }

    cd = &gCrash.cars[car];

    if (damage < 0.0f) damage = 0.0f;
    if (damage > cd->max_damage) damage = cd->max_damage;

    cd->damage = damage;
}

/**
 * Get current damage
 */
f32 crash_get_damage(s32 car) {
    if (car < 0 || car >= 8) {
        return 0.0f;
    }

    return gCrash.cars[car].damage;
}

/**
 * Get damage level
 */
s32 crash_get_damage_level(s32 car) {
    if (car < 0 || car >= 8) {
        return DAMAGE_LEVEL_NONE;
    }

    return gCrash.cars[car].damage_level;
}

/**
 * Repair damage
 */
void crash_repair(s32 car, f32 amount) {
    CrashData *cd;

    if (car < 0 || car >= 8) {
        return;
    }

    cd = &gCrash.cars[car];

    cd->damage -= amount;
    if (cd->damage < 0.0f) {
        cd->damage = 0.0f;
    }

    /* Update damage level */
    crash_apply_damage(car, 0.0f);  /* Recalculate level */
}

/**
 * Full repair
 */
void crash_full_repair(s32 car) {
    CrashData *cd;

    if (car < 0 || car >= 8) {
        return;
    }

    cd = &gCrash.cars[car];
    cd->damage = 0.0f;
    cd->damage_level = DAMAGE_LEVEL_NONE;
}

/**
 * Begin respawn animation
 */
void crash_begin_respawn(s32 car) {
    CrashData *cd;

    if (car < 0 || car >= 8) {
        return;
    }

    cd = &gCrash.cars[car];

    cd->state = CRASH_STATE_RESPAWNING;
    cd->state_timer = RESURRECT_FRAMES;
    cd->respawn_alpha = 0.0f;
}

/**
 * Complete respawn
 */
void crash_complete_respawn(s32 car) {
    CrashData *cd;

    if (car < 0 || car >= 8) {
        return;
    }

    cd = &gCrash.cars[car];

    /* Reset state */
    cd->state = CRASH_STATE_NONE;
    cd->cause = CRASH_CAUSE_NONE;
    cd->state_timer = 0.0f;
    cd->crash_timer = 0.0f;
    cd->respawn_alpha = 1.0f;

    /* Full repair on respawn */
    crash_full_repair(car);

    /* Grant invincibility */
    cd->invincible = 1;
    cd->invincible_timer = INVINCIBLE_FRAMES;

    /* Car would be teleported to respawn point here */
}

/**
 * Set respawn point
 */
void crash_set_respawn_point(s32 car, f32 *pos, f32 rotation) {
    CrashData *cd;

    if (car < 0 || car >= 8) {
        return;
    }

    cd = &gCrash.cars[car];

    cd->respawn.position[0] = pos[0];
    cd->respawn.position[1] = pos[1];
    cd->respawn.position[2] = pos[2];
    cd->respawn.rotation = rotation;
    cd->respawn.active = 1;
}

/**
 * Force immediate respawn (debug)
 */
void crash_force_respawn(s32 car) {
    CrashData *cd;

    if (car < 0 || car >= 8) {
        return;
    }

    cd = &gCrash.cars[car];

    if (cd->state != CRASH_STATE_NONE) {
        crash_complete_respawn(car);
    }
}

/**
 * Check if respawning
 */
s32 crash_is_respawning(s32 car) {
    if (car < 0 || car >= 8) {
        return 0;
    }

    return (gCrash.cars[car].state == CRASH_STATE_RESPAWNING);
}

/**
 * Check if crashed
 */
s32 crash_is_crashed(s32 car) {
    if (car < 0 || car >= 8) {
        return 0;
    }

    return (gCrash.cars[car].state != CRASH_STATE_NONE);
}

/**
 * Check if invincible
 */
s32 crash_is_invincible(s32 car) {
    if (car < 0 || car >= 8) {
        return 0;
    }

    return gCrash.cars[car].invincible;
}

/**
 * Get crash state
 */
s32 crash_get_state(s32 car) {
    if (car < 0 || car >= 8) {
        return CRASH_STATE_NONE;
    }

    return gCrash.cars[car].state;
}

/**
 * Get crash cause
 */
s32 crash_get_cause(s32 car) {
    if (car < 0 || car >= 8) {
        return CRASH_CAUSE_NONE;
    }

    return gCrash.cars[car].cause;
}

/**
 * Get respawn alpha for rendering
 */
f32 crash_get_alpha(s32 car) {
    if (car < 0 || car >= 8) {
        return 1.0f;
    }

    return gCrash.cars[car].respawn_alpha;
}

/**
 * Enable/disable crash system
 */
void crash_set_enabled(u8 enabled) {
    gCrash.enabled = enabled;
}

/**
 * Enable/disable damage
 */
void crash_set_damage_enabled(u8 enabled) {
    gCrash.damage_enabled = enabled;
}

/**
 * Enable/disable auto respawn
 */
void crash_set_auto_respawn(u8 auto_respawn) {
    gCrash.auto_respawn = auto_respawn;
}

/**
 * Set damage multiplier
 */
void crash_set_damage_mult(f32 mult) {
    if (mult < 0.0f) mult = 0.0f;
    if (mult > 10.0f) mult = 10.0f;
    gCrash.damage_mult = mult;
}

/**
 * Set crash delay
 */
void crash_set_crash_delay(f32 delay) {
    if (delay < 0.0f) delay = 0.0f;
    gCrash.crash_delay = delay;
}

/**
 * Spawn crash visual effects
 */
void crash_spawn_effects(s32 car, s32 cause) {
    CrashData *cd;
    f32 pos[3];

    if (car < 0 || car >= 8) {
        return;
    }

    cd = &gCrash.cars[car];

    /* Get crash position */
    pos[0] = cd->physics.position[0];
    pos[1] = cd->physics.position[1];
    pos[2] = cd->physics.position[2];

    /* Spawn appropriate effects based on cause */
    switch (cause) {
        case CRASH_CAUSE_COLLISION:
        case CRASH_CAUSE_WALL:
            particles_sparks(pos, NULL, 20);
            particles_debris(pos, NULL, 10);
            break;

        case CRASH_CAUSE_FLIP:
        case CRASH_CAUSE_FALL:
            particles_dust_cloud(pos, 3.0f, 15);
            particles_debris(pos, NULL, 8);
            break;

        case CRASH_CAUSE_WATER:
            particles_water_splash(pos, 4.0f);
            break;

        case CRASH_CAUSE_EXPLOSION:
            particles_explosion(pos, 5.0f);
            break;

        default:
            particles_smoke_puff(pos, 2.0f);
            break;
    }
}

/**
 * Draw explosion effect
 */
void crash_draw_explosion(s32 car) {
    CrashData *cd;

    if (car < 0 || car >= 8) {
        return;
    }

    cd = &gCrash.cars[car];

    if (cd->state != CRASH_STATE_EXPLODING) {
        return;
    }

    /* Explosion rendering would go here */
}

/**
 * Draw respawn effect
 */
void crash_draw_respawn(s32 car) {
    CrashData *cd;

    if (car < 0 || car >= 8) {
        return;
    }

    cd = &gCrash.cars[car];

    if (cd->state != CRASH_STATE_RESPAWNING) {
        return;
    }

    /* Respawn effect rendering would go here */
    /* Car would be drawn with alpha = respawn_alpha */
}

/**
 * Debug print crash state
 */
void crash_debug_print(s32 car) {
    CrashData *cd;

    if (car < 0 || car >= 8) {
        return;
    }

    cd = &gCrash.cars[car];

    /* Debug output would go here */
    /* osSyncPrintf("Crash P%d: state=%d dmg=%.1f crashes=%d\n",
                 car, cd->state, cd->damage, cd->crash_count); */
}
