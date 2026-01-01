/**
 * boost.c - Boost/Nitro system implementation for Rush 2049 N64
 *
 * Handles boost meter, nitro pickups, boost pads,
 * and speed multipliers during gameplay.
 */

#include "game/boost.h"
#include "game/particles.h"

/* Global boost system */
BoostSystem gBoost;

/* Duration lookup table */
static const f32 sBoostDurations[NUM_BOOST_TYPES] = {
    BOOST_DURATION_NITRO,   /* BOOST_TYPE_NITRO */
    BOOST_DURATION_PAD,     /* BOOST_TYPE_PAD */
    BOOST_DURATION_STUNT,   /* BOOST_TYPE_JUMP */
    BOOST_DURATION_STUNT,   /* BOOST_TYPE_STUNT */
    BOOST_DURATION_PICKUP   /* BOOST_TYPE_PICKUP */
};

/* Multiplier lookup table */
static const f32 sBoostMultipliers[NUM_BOOST_TYPES] = {
    BOOST_MULT_NITRO,       /* BOOST_TYPE_NITRO */
    BOOST_MULT_PAD,         /* BOOST_TYPE_PAD */
    BOOST_MULT_STUNT,       /* BOOST_TYPE_JUMP */
    BOOST_MULT_STUNT,       /* BOOST_TYPE_STUNT */
    BOOST_MULT_PICKUP       /* BOOST_TYPE_PICKUP */
};

/**
 * Initialize boost system
 */
void boost_init(void) {
    s32 i;

    gBoost.num_players = 0;
    gBoost.num_pads = 0;
    gBoost.enabled = 1;
    gBoost.infinite = 0;
    gBoost.auto_recharge = 1;
    gBoost.recharge_rate = BOOST_METER_MAX / BOOST_RECHARGE_TIME;
    gBoost.global_mult = 1.0f;

    /* Clear all players */
    for (i = 0; i < 4; i++) {
        boost_init_player(i);
    }

    /* Clear all pads */
    for (i = 0; i < MAX_BOOST_PADS; i++) {
        gBoost.pads[i].active = 0;
    }
}

/**
 * Reset boost system (new race)
 */
void boost_reset(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        boost_init_player(i);
    }
}

/**
 * Initialize single player boost state
 */
void boost_init_player(s32 player) {
    PlayerBoost *pb;
    s32 i;

    if (player < 0 || player >= 4) {
        return;
    }

    pb = &gBoost.players[player];

    /* Meter */
    pb->meter = BOOST_METER_MAX;
    pb->charges = 1;
    pb->max_charges = MAX_BOOST_CHARGES;

    /* State */
    pb->state = BOOST_STATE_IDLE;
    pb->can_boost = 1;
    pb->button_held = 0;

    /* Timing */
    pb->state_timer = 0.0f;
    pb->cooldown = 0.0f;
    pb->recharge_timer = 0.0f;

    /* Active boosts */
    for (i = 0; i < MAX_ACTIVE_BOOSTS; i++) {
        pb->active[i].active = 0;
    }
    pb->num_active = 0;

    /* Combined effect */
    pb->total_multiplier = 1.0f;
    pb->time_boost = 1.0f;

    /* Visual */
    pb->flame_intensity = 0.0f;
    pb->trail_alpha = 0.0f;
    pb->effect_active = 0;

    /* Stats */
    pb->boosts_used = 0;
    pb->pads_hit = 0;
    pb->pickups_collected = 0;
}

/**
 * Update boost system for all players
 */
void boost_update(void) {
    s32 i;

    if (!gBoost.enabled) {
        return;
    }

    for (i = 0; i < gBoost.num_players; i++) {
        boost_update_player(i);
    }
}

/**
 * Update single player's boost state
 */
void boost_update_player(s32 player) {
    PlayerBoost *pb;

    if (player < 0 || player >= 4) {
        return;
    }

    pb = &gBoost.players[player];

    /* Update active boosts */
    boost_update_active(pb);

    /* Update meter recharge */
    boost_update_meter(pb);

    /* Update cooldown */
    if (pb->cooldown > 0.0f) {
        pb->cooldown -= 1.0f;
        if (pb->cooldown <= 0.0f) {
            pb->cooldown = 0.0f;
            pb->can_boost = 1;
        }
    }

    /* Update state timer */
    if (pb->state_timer > 0.0f) {
        pb->state_timer -= 1.0f;
        if (pb->state_timer <= 0.0f) {
            pb->state_timer = 0.0f;
            if (pb->state == BOOST_STATE_ACTIVE && pb->num_active == 0) {
                pb->state = BOOST_STATE_COOLDOWN;
                pb->cooldown = BOOST_COOLDOWN_TIME;
                pb->can_boost = 0;
            }
        }
    }

    /* Update visual effects */
    boost_update_effects(player);
}

/**
 * Update active boost effects
 */
void boost_update_active(PlayerBoost *pb) {
    s32 i;
    f32 max_mult;
    s32 active_count;

    max_mult = 1.0f;
    active_count = 0;

    for (i = 0; i < MAX_ACTIVE_BOOSTS; i++) {
        if (!pb->active[i].active) {
            continue;
        }

        /* Decrement duration */
        pb->active[i].duration -= 1.0f;

        if (pb->active[i].duration <= 0.0f) {
            /* Boost expired */
            pb->active[i].active = 0;
        } else {
            /* Track highest multiplier */
            if (pb->active[i].multiplier > max_mult) {
                max_mult = pb->active[i].multiplier;
            }
            active_count++;
        }
    }

    pb->num_active = active_count;

    /* Apply multiplier cap */
    if (max_mult > BOOST_MULT_MAX) {
        max_mult = BOOST_MULT_MAX;
    }

    pb->total_multiplier = max_mult * gBoost.global_mult;
    pb->time_boost = pb->total_multiplier;

    /* Update state based on active boosts */
    if (active_count > 0) {
        pb->state = BOOST_STATE_ACTIVE;
        pb->effect_active = 1;
    } else if (pb->state == BOOST_STATE_ACTIVE) {
        pb->state = BOOST_STATE_COOLDOWN;
        pb->cooldown = BOOST_COOLDOWN_TIME;
        pb->effect_active = 0;
    }
}

/**
 * Update meter recharge
 */
void boost_update_meter(PlayerBoost *pb) {
    if (!gBoost.auto_recharge) {
        return;
    }

    /* Only recharge when not boosting */
    if (pb->state == BOOST_STATE_IDLE || pb->state == BOOST_STATE_RECHARGING) {
        if (pb->meter < BOOST_METER_MAX) {
            pb->recharge_timer += 1.0f;
            pb->meter += gBoost.recharge_rate;

            if (pb->meter >= BOOST_METER_MAX) {
                pb->meter = BOOST_METER_MAX;

                /* Add charge if we have room */
                if (pb->charges < pb->max_charges) {
                    pb->charges++;
                    pb->meter = 0.0f;
                }
            }

            pb->state = BOOST_STATE_RECHARGING;
        } else {
            pb->state = BOOST_STATE_IDLE;
        }
    }
}

/**
 * Update visual effects
 */
void boost_update_effects(s32 player) {
    PlayerBoost *pb;
    f32 target_flame;
    f32 target_trail;

    pb = &gBoost.players[player];

    /* Calculate target intensities */
    if (pb->num_active > 0) {
        target_flame = (pb->total_multiplier - 1.0f) * 2.0f;
        target_trail = pb->total_multiplier - 1.0f;

        if (target_flame > 1.0f) target_flame = 1.0f;
        if (target_trail > 1.0f) target_trail = 1.0f;
    } else {
        target_flame = 0.0f;
        target_trail = 0.0f;
    }

    /* Smooth transitions */
    if (pb->flame_intensity < target_flame) {
        pb->flame_intensity += 0.1f;
        if (pb->flame_intensity > target_flame) {
            pb->flame_intensity = target_flame;
        }
    } else if (pb->flame_intensity > target_flame) {
        pb->flame_intensity -= 0.05f;
        if (pb->flame_intensity < target_flame) {
            pb->flame_intensity = target_flame;
        }
    }

    if (pb->trail_alpha < target_trail) {
        pb->trail_alpha += 0.08f;
        if (pb->trail_alpha > target_trail) {
            pb->trail_alpha = target_trail;
        }
    } else if (pb->trail_alpha > target_trail) {
        pb->trail_alpha -= 0.04f;
        if (pb->trail_alpha < target_trail) {
            pb->trail_alpha = target_trail;
        }
    }
}

/**
 * Activate a boost effect
 */
s32 boost_activate(s32 player, s32 type, s32 source) {
    PlayerBoost *pb;
    ActiveBoost *ab;
    s32 i;
    s32 slot;

    if (player < 0 || player >= 4) {
        return 0;
    }

    if (type < 0 || type >= NUM_BOOST_TYPES) {
        return 0;
    }

    pb = &gBoost.players[player];

    /* Check if player can boost (for button-activated boosts) */
    if (source == BOOST_SOURCE_BUTTON) {
        if (!pb->can_boost) {
            return 0;
        }

        /* Need meter or charges */
        if (!gBoost.infinite) {
            if (pb->charges <= 0 && pb->meter < BOOST_METER_PER_USE) {
                return 0;
            }

            /* Consume charge or meter */
            if (pb->charges > 0) {
                pb->charges--;
            } else {
                pb->meter -= BOOST_METER_PER_USE;
                if (pb->meter < 0.0f) {
                    pb->meter = 0.0f;
                }
            }
        }

        pb->boosts_used++;
    }

    /* Find free slot */
    slot = -1;
    for (i = 0; i < MAX_ACTIVE_BOOSTS; i++) {
        if (!pb->active[i].active) {
            slot = i;
            break;
        }
    }

    if (slot < 0) {
        /* No free slots - replace oldest */
        slot = 0;
    }

    /* Activate boost */
    ab = &pb->active[slot];
    ab->active = 1;
    ab->type = type;
    ab->source = source;
    ab->multiplier = sBoostMultipliers[type];
    ab->duration = sBoostDurations[type];
    ab->max_duration = sBoostDurations[type];

    /* Update state */
    pb->state = BOOST_STATE_ACTIVE;
    pb->state_timer = ab->duration;
    pb->num_active++;

    /* Start cooldown for button boosts */
    if (source == BOOST_SOURCE_BUTTON) {
        pb->can_boost = 0;
    }

    return 1;
}

/**
 * Check if player can activate boost
 */
s32 boost_can_activate(s32 player) {
    PlayerBoost *pb;

    if (player < 0 || player >= 4) {
        return 0;
    }

    pb = &gBoost.players[player];

    if (!pb->can_boost) {
        return 0;
    }

    if (gBoost.infinite) {
        return 1;
    }

    return (pb->charges > 0 || pb->meter >= BOOST_METER_PER_USE);
}

/**
 * Deactivate a specific boost
 */
void boost_deactivate(s32 player, s32 boost_index) {
    PlayerBoost *pb;

    if (player < 0 || player >= 4) {
        return;
    }

    if (boost_index < 0 || boost_index >= MAX_ACTIVE_BOOSTS) {
        return;
    }

    pb = &gBoost.players[player];
    pb->active[boost_index].active = 0;
}

/**
 * Deactivate all active boosts
 */
void boost_deactivate_all(s32 player) {
    PlayerBoost *pb;
    s32 i;

    if (player < 0 || player >= 4) {
        return;
    }

    pb = &gBoost.players[player];

    for (i = 0; i < MAX_ACTIVE_BOOSTS; i++) {
        pb->active[i].active = 0;
    }

    pb->num_active = 0;
    pb->total_multiplier = 1.0f;
    pb->time_boost = 1.0f;
    pb->state = BOOST_STATE_IDLE;
}

/**
 * Add meter to player
 */
void boost_add_meter(s32 player, f32 amount) {
    PlayerBoost *pb;

    if (player < 0 || player >= 4) {
        return;
    }

    pb = &gBoost.players[player];
    pb->meter += amount;

    /* Cap at max */
    if (pb->meter >= BOOST_METER_MAX) {
        pb->meter = BOOST_METER_MAX;

        /* Add charge if we have room */
        if (pb->charges < pb->max_charges) {
            pb->charges++;
            pb->meter = 0.0f;
        }
    }
}

/**
 * Drain meter from player
 */
void boost_drain_meter(s32 player, f32 amount) {
    PlayerBoost *pb;

    if (player < 0 || player >= 4) {
        return;
    }

    pb = &gBoost.players[player];
    pb->meter -= amount;

    if (pb->meter < 0.0f) {
        pb->meter = 0.0f;
    }
}

/**
 * Set meter to specific value
 */
void boost_set_meter(s32 player, f32 amount) {
    PlayerBoost *pb;

    if (player < 0 || player >= 4) {
        return;
    }

    pb = &gBoost.players[player];

    if (amount < 0.0f) {
        amount = 0.0f;
    }
    if (amount > BOOST_METER_MAX) {
        amount = BOOST_METER_MAX;
    }

    pb->meter = amount;
}

/**
 * Get current meter value
 */
f32 boost_get_meter(s32 player) {
    if (player < 0 || player >= 4) {
        return 0.0f;
    }

    return gBoost.players[player].meter;
}

/**
 * Get available charges
 */
s32 boost_get_charges(s32 player) {
    if (player < 0 || player >= 4) {
        return 0;
    }

    return gBoost.players[player].charges;
}

/**
 * Add a boost charge
 */
void boost_add_charge(s32 player) {
    PlayerBoost *pb;

    if (player < 0 || player >= 4) {
        return;
    }

    pb = &gBoost.players[player];

    if (pb->charges < pb->max_charges) {
        pb->charges++;
    }
}

/**
 * Load boost pads from track data
 */
void boost_load_pads(void *track_data) {
    /* Track data format TBD - clear for now */
    boost_clear_pads();
}

/**
 * Clear all boost pads
 */
void boost_clear_pads(void) {
    s32 i;

    for (i = 0; i < MAX_BOOST_PADS; i++) {
        gBoost.pads[i].active = 0;
    }

    gBoost.num_pads = 0;
}

/**
 * Add a boost pad to the track
 */
s32 boost_add_pad(f32 *pos, f32 width, f32 length, f32 rotation, f32 mult) {
    BoostPad *pad;
    s32 i;

    /* Find free slot */
    for (i = 0; i < MAX_BOOST_PADS; i++) {
        if (!gBoost.pads[i].active) {
            pad = &gBoost.pads[i];

            pad->pos[0] = pos[0];
            pad->pos[1] = pos[1];
            pad->pos[2] = pos[2];
            pad->size[0] = width;
            pad->size[1] = length;
            pad->rotation = rotation;
            pad->multiplier = mult;
            pad->duration = BOOST_DURATION_PAD;
            pad->active = 1;
            pad->type = 0;

            gBoost.num_pads++;
            return i;
        }
    }

    return -1;
}

/**
 * Check if player is on a boost pad
 */
void boost_check_pads(s32 player, f32 *pos) {
    BoostPad *pad;
    PlayerBoost *pb;
    f32 dx, dz;
    f32 dist;
    s32 i;

    if (player < 0 || player >= 4) {
        return;
    }

    pb = &gBoost.players[player];

    for (i = 0; i < MAX_BOOST_PADS; i++) {
        pad = &gBoost.pads[i];

        if (!pad->active) {
            continue;
        }

        /* Simple distance check (could use rotation for accurate bounds) */
        dx = pos[0] - pad->pos[0];
        dz = pos[2] - pad->pos[2];
        dist = dx * dx + dz * dz;

        if (dist < (pad->size[0] * pad->size[0] + pad->size[1] * pad->size[1])) {
            /* On boost pad - activate */
            boost_activate(player, BOOST_TYPE_PAD, BOOST_SOURCE_PAD);
            pb->pads_hit++;
        }
    }
}

/**
 * Get boost pad by index
 */
BoostPad* boost_get_pad(s32 index) {
    if (index < 0 || index >= MAX_BOOST_PADS) {
        return NULL;
    }

    return &gBoost.pads[index];
}

/**
 * Get current speed multiplier
 */
f32 boost_get_multiplier(s32 player) {
    if (player < 0 || player >= 4) {
        return 1.0f;
    }

    return gBoost.players[player].total_multiplier;
}

/**
 * Get time boost value (arcade compatible)
 */
f32 boost_get_time_boost(s32 player) {
    if (player < 0 || player >= 4) {
        return 1.0f;
    }

    return gBoost.players[player].time_boost;
}

/**
 * Check if player is currently boosting
 */
s32 boost_is_active(s32 player) {
    if (player < 0 || player >= 4) {
        return 0;
    }

    return (gBoost.players[player].num_active > 0);
}

/**
 * Get player's boost state
 */
s32 boost_get_state(s32 player) {
    if (player < 0 || player >= 4) {
        return BOOST_STATE_IDLE;
    }

    return gBoost.players[player].state;
}

/**
 * Get flame intensity for visuals
 */
f32 boost_get_flame_intensity(s32 player) {
    if (player < 0 || player >= 4) {
        return 0.0f;
    }

    return gBoost.players[player].flame_intensity;
}

/**
 * Handle boost pickup collection
 */
void boost_on_pickup(s32 player, s32 pickup_type) {
    PlayerBoost *pb;

    if (player < 0 || player >= 4) {
        return;
    }

    pb = &gBoost.players[player];

    /* Add meter from pickup */
    boost_add_meter(player, BOOST_METER_PICKUP);

    /* Instant boost effect */
    boost_activate(player, BOOST_TYPE_PICKUP, BOOST_SOURCE_PICKUP);

    pb->pickups_collected++;
}

/**
 * Handle stunt completion boost
 */
void boost_on_stunt(s32 player, f32 bonus) {
    if (player < 0 || player >= 4) {
        return;
    }

    /* Add meter based on stunt quality */
    boost_add_meter(player, BOOST_METER_STUNT * bonus);

    /* Activate stunt boost */
    boost_activate(player, BOOST_TYPE_STUNT, BOOST_SOURCE_STUNT);
}

/**
 * Handle jump (potential for jump boost)
 */
void boost_on_jump(s32 player) {
    if (player < 0 || player >= 4) {
        return;
    }

    /* Jump boost could be activated here */
    /* For now, just a placeholder */
}

/**
 * Handle boost button press
 */
void boost_press(s32 player) {
    PlayerBoost *pb;

    if (player < 0 || player >= 4) {
        return;
    }

    pb = &gBoost.players[player];

    if (!pb->button_held) {
        pb->button_held = 1;

        /* Try to activate nitro boost */
        boost_activate(player, BOOST_TYPE_NITRO, BOOST_SOURCE_BUTTON);
    }
}

/**
 * Handle boost button release
 */
void boost_release(s32 player) {
    PlayerBoost *pb;

    if (player < 0 || player >= 4) {
        return;
    }

    pb = &gBoost.players[player];
    pb->button_held = 0;
}

/**
 * Enable/disable boost system
 */
void boost_set_enabled(u8 enabled) {
    gBoost.enabled = enabled;
}

/**
 * Enable/disable infinite boost
 */
void boost_set_infinite(u8 infinite) {
    gBoost.infinite = infinite;
}

/**
 * Set meter recharge rate
 */
void boost_set_recharge_rate(f32 rate) {
    gBoost.recharge_rate = rate;
}

/**
 * Set global boost multiplier
 */
void boost_set_global_mult(f32 mult) {
    if (mult < 0.1f) {
        mult = 0.1f;
    }
    if (mult > 5.0f) {
        mult = 5.0f;
    }

    gBoost.global_mult = mult;
}

/**
 * Draw boost visual effects for player
 */
void boost_draw_effects(s32 player) {
    PlayerBoost *pb;

    if (player < 0 || player >= 4) {
        return;
    }

    pb = &gBoost.players[player];

    if (pb->flame_intensity <= 0.0f) {
        return;
    }

    /* Particle effects would be spawned here via particles_boost_flame() */
    /* This is called per-frame during car rendering */
}

/**
 * Draw boost meter UI
 */
void boost_draw_meter(s32 player, s16 x, s16 y, s16 width, s16 height) {
    PlayerBoost *pb;
    f32 fill;

    if (player < 0 || player >= 4) {
        return;
    }

    pb = &gBoost.players[player];

    /* Calculate fill percentage */
    fill = pb->meter / BOOST_METER_MAX;
    if (fill > 1.0f) fill = 1.0f;
    if (fill < 0.0f) fill = 0.0f;

    /* Draw meter background */
    /* gfx_draw_rect(x, y, width, height, 0, 0, 0, 128); */

    /* Draw meter fill */
    /* gfx_draw_rect(x, y, (s16)(width * fill), height, 0, 200, 255, 200); */

    /* Draw border */
    /* gfx_draw_rect_outline(x, y, width, height, 255, 255, 255, 255); */
}

/**
 * Draw boost charge indicators
 */
void boost_draw_charges(s32 player, s16 x, s16 y) {
    PlayerBoost *pb;
    s32 i;

    if (player < 0 || player >= 4) {
        return;
    }

    pb = &gBoost.players[player];

    /* Draw charge icons */
    for (i = 0; i < pb->max_charges; i++) {
        /* Draw full or empty charge icon */
        if (i < pb->charges) {
            /* Full charge */
            /* gfx_draw_sprite(SPRITE_BOOST_FULL, x + i * 16, y); */
        } else {
            /* Empty charge */
            /* gfx_draw_sprite(SPRITE_BOOST_EMPTY, x + i * 16, y); */
        }
    }
}

/**
 * Draw boost pads on track
 */
void boost_draw_pads(void) {
    BoostPad *pad;
    s32 i;

    for (i = 0; i < MAX_BOOST_PADS; i++) {
        pad = &gBoost.pads[i];

        if (!pad->active) {
            continue;
        }

        /* Draw boost pad texture/effect */
        /* Actual rendering depends on track system */
    }
}

/**
 * Debug: Print boost state
 */
void boost_debug_print(s32 player) {
    PlayerBoost *pb;

    if (player < 0 || player >= 4) {
        return;
    }

    pb = &gBoost.players[player];

    /* Debug output would go here */
    /* osSyncPrintf("Boost P%d: meter=%.1f charges=%d mult=%.2f state=%d\n",
                 player, pb->meter, pb->charges, pb->total_multiplier, pb->state); */
}

/**
 * Debug: Give player full boost
 */
void boost_give_full(s32 player) {
    PlayerBoost *pb;

    if (player < 0 || player >= 4) {
        return;
    }

    pb = &gBoost.players[player];
    pb->meter = BOOST_METER_MAX;
    pb->charges = pb->max_charges;
}

/*
 * =============================================================================
 * Arcade-compatible roadboost surface system
 * From reference/repos/rushtherock/game/mdrive.c and stree.c
 *
 * The arcade uses a per-wheel roadboost value extracted from surface flags.
 * Combined with catchup and time_boost to form the final time_fudge multiplier.
 * =============================================================================
 */

/* Global roadboost data for each car slot */
RoadBoostData gRoadBoost[4];

/**
 * roadboost_init - Initialize road boost data
 * Arcade: Part of init_model() in mdrive.c line 581
 */
#ifdef NON_MATCHING
void roadboost_init(RoadBoostData *rb) {
    s32 i;

    if (rb == NULL) {
        return;
    }

    for (i = 0; i < NUM_WHEELS; i++) {
        rb->roadboost[i] = 0;
    }
    rb->time_boost = 1.0f;
    rb->catchup = 1.0f;
    rb->time_fudge = 1.0f;
}
#else
INCLUDE_ASM("asm/us/nonmatchings/game/boost", roadboost_init);
#endif

/**
 * roadboost_reset - Reset road boost to defaults
 * Arcade: Part of init_model() reset path
 */
#ifdef NON_MATCHING
void roadboost_reset(RoadBoostData *rb) {
    s32 i;

    if (rb == NULL) {
        return;
    }

    for (i = 0; i < NUM_WHEELS; i++) {
        rb->roadboost[i] = 0;
    }
    /* Keep time_boost and catchup, just clear surface boosts */
    rb->time_fudge = rb->catchup * rb->time_boost;
}
#else
INCLUDE_ASM("asm/us/nonmatchings/game/boost", roadboost_reset);
#endif

/**
 * roadboost_set_wheel - Set roadboost value for a wheel
 * Arcade: stree.c line 323
 *   m->roadboost[whl] = (savesp->flags & SURF_BOOST_MASK) >> SURF_BOOST_SHIFT;
 */
#ifdef NON_MATCHING
void roadboost_set_wheel(RoadBoostData *rb, s32 wheel, u16 boost_value) {
    if (rb == NULL) {
        return;
    }
    if (wheel < 0 || wheel >= NUM_WHEELS) {
        return;
    }

    /* Clamp to 4-bit value (0-15) */
    if (boost_value > SURF_BOOST_MAX) {
        boost_value = SURF_BOOST_MAX;
    }

    rb->roadboost[wheel] = boost_value;
}
#else
INCLUDE_ASM("asm/us/nonmatchings/game/boost", roadboost_set_wheel);
#endif

/**
 * roadboost_clear_all - Clear roadboost for all wheels
 * Arcade: road.c line 44
 *   m->roadboost[i] = 0;
 */
#ifdef NON_MATCHING
void roadboost_clear_all(RoadBoostData *rb) {
    s32 i;

    if (rb == NULL) {
        return;
    }

    for (i = 0; i < NUM_WHEELS; i++) {
        rb->roadboost[i] = 0;
    }
}
#else
INCLUDE_ASM("asm/us/nonmatchings/game/boost", roadboost_clear_all);
#endif

/**
 * roadboost_calc_time_fudge - Calculate combined time multiplier
 * Arcade: mdrive.c line 1086-1087
 *   // scaled roadboost = 1.0 to 2.0
 *   m->time_fudge = m->catchup * m->time_boost * (1.0 + (F32)m->roadboost[0] * 0.06667);
 *
 * Uses wheel 0 (front-left) for the boost value, matching arcade behavior.
 * Returns the combined multiplier for physics time scaling.
 */
#ifdef NON_MATCHING
f32 roadboost_calc_time_fudge(RoadBoostData *rb) {
    f32 roadboost_mult;

    if (rb == NULL) {
        return 1.0f;
    }

    /* Calculate roadboost multiplier: 1.0 to ~2.0 based on roadboost[0] */
    roadboost_mult = ROADBOOST_BASE + ((f32)rb->roadboost[0] * ROADBOOST_SCALE);

    /* Combine all factors */
    rb->time_fudge = rb->catchup * rb->time_boost * roadboost_mult;

    return rb->time_fudge;
}
#else
INCLUDE_ASM("asm/us/nonmatchings/game/boost", roadboost_calc_time_fudge);
#endif

/**
 * roadboost_extract_from_surface - Extract boost value from surface flags
 * Arcade: stree.c line 323
 *   m->roadboost[whl] = (savesp->flags & SURF_BOOST_MASK) >> SURF_BOOST_SHIFT;
 *
 * @param surface_flags Raw surface flags from track data
 * @return Boost value (0-15)
 */
#ifdef NON_MATCHING
u16 roadboost_extract_from_surface(u16 surface_flags) {
    return (surface_flags & SURF_BOOST_MASK) >> SURF_BOOST_SHIFT;
}
#else
INCLUDE_ASM("asm/us/nonmatchings/game/boost", roadboost_extract_from_surface);
#endif
