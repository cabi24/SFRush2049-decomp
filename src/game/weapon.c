/**
 * weapon.c - Weapon system for Rush 2049 N64 Battle Mode
 *
 * This file implements the weapon system for battle mode combat.
 * Rush 2049 battle mode is unique to the N64 version - the arcade
 * "Rush The Rock" source does not include weapons or battle mode.
 *
 * The weapon system handles:
 * - Weapon state per player
 * - Firing mechanics and cooldowns
 * - Target acquisition for homing weapons
 * - Projectile spawning
 * - Weapon effects (boost, shield)
 */

#include "types.h"
#include "game/weapon.h"
#include "game/battle.h"
#include "game/sound.h"
#include "game/structs.h"

/* External dependencies */
extern CarData car_array[];
extern s32 num_active_cars;
extern u32 frame_counter;
extern f32 sqrtf(f32 x);

/* ========================================================================
 * Weapon Properties Table
 * ======================================================================== */

/**
 * gWeaponProps - Static properties for each weapon type
 *
 * Index corresponds to WEAPON_* constants.
 * Properties define damage, speed, ammo, and behavior flags.
 */
const WeaponProps gWeaponProps[NUM_WEAPONS] = {
    /* WEAPON_NONE */
    { 0, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0, 0, 0, 0 },

    /* WEAPON_MISSILE - Homing projectile */
    { MISSILE_DAMAGE, MISSILE_SPEED, MISSILE_LIFETIME, MISSILE_TURN_RATE,
      MISSILE_AMMO, MISSILE_RADIUS, 1, 1, 0, 0 },

    /* WEAPON_MINE - Dropped explosive */
    { MINE_DAMAGE, 0.0f, MINE_LIFETIME, 0.0f,
      MINE_AMMO, MINE_BLAST_RADIUS, 0, 0, 1, 0 },

    /* WEAPON_SHOCKWAVE - Area effect */
    { SHOCKWAVE_DAMAGE, 0.0f, SHOCKWAVE_DURATION, 0.0f,
      SHOCKWAVE_AMMO, SHOCKWAVE_RADIUS, 0, 0, 1, 0 },

    /* WEAPON_BOOST - Speed boost for ramming */
    { BOOST_RAM_DAMAGE, BOOST_SPEED_MULT, (f32)BOOST_DURATION / 60.0f, 0.0f,
      BOOST_AMMO, 3.0f, 0, 0, 0, 0 },

    /* WEAPON_SHIELD - Temporary invincibility */
    { 0, 0.0f, (f32)SHIELD_DURATION / 60.0f, 0.0f,
      SHIELD_AMMO, 0.0f, 0, 0, 0, 0 },

    /* WEAPON_MACHINEGUN - Rapid fire bullets */
    { MACHINEGUN_DAMAGE, MACHINEGUN_SPEED, MACHINEGUN_LIFETIME, 0.0f,
      MACHINEGUN_AMMO, 2.0f, 1, 0, 0, 0 }
};

/* Weapon name strings */
static const char *sWeaponNames[NUM_WEAPONS] = {
    "NONE",
    "MISSILE",
    "MINE",
    "SHOCKWAVE",
    "BOOST",
    "SHIELD",
    "MACHINEGUN"
};

/* ========================================================================
 * Per-Player Weapon State
 * ======================================================================== */

/* Maximum players in battle mode */
#define WEAPON_MAX_PLAYERS 4

/* Player weapon states */
static WeaponState sPlayerWeapons[WEAPON_MAX_PLAYERS];

/* ========================================================================
 * Initialization Functions
 * ======================================================================== */

#ifdef NON_MATCHING

/**
 * weapon_init - Initialize the weapon system
 *
 * Called at game startup to set up weapon state.
 */
void weapon_init(void) {
    weapon_reset();
}

/**
 * weapon_reset - Reset all weapon state
 *
 * Called when starting a new battle to clear all player weapons.
 */
void weapon_reset(void) {
    s32 i;

    for (i = 0; i < WEAPON_MAX_PLAYERS; i++) {
        weapon_reset_player(i);
    }
}

/**
 * weapon_reset_player - Reset weapon state for one player
 *
 * @param player Player index
 */
void weapon_reset_player(s32 player) {
    WeaponState *ws;

    if (player < 0 || player >= WEAPON_MAX_PLAYERS) {
        return;
    }

    ws = &sPlayerWeapons[player];
    ws->type = WEAPON_NONE;
    ws->ammo = 0;
    ws->fire_cooldown = 0;
    ws->locked_on = 0;
    ws->target = -1;
    ws->last_fire_time = 0;
}

/* ========================================================================
 * Weapon State Management
 * ======================================================================== */

/**
 * weapon_give - Give a weapon to a player
 *
 * @param player Player index
 * @param weapon_type Weapon type to give
 */
void weapon_give(s32 player, s32 weapon_type) {
    WeaponState *ws;

    if (player < 0 || player >= WEAPON_MAX_PLAYERS) {
        return;
    }
    if (weapon_type < 0 || weapon_type >= NUM_WEAPONS) {
        return;
    }

    ws = &sPlayerWeapons[player];
    ws->type = (u8)weapon_type;
    ws->ammo = (u8)gWeaponProps[weapon_type].ammo;
    ws->fire_cooldown = 0;
    ws->locked_on = 0;
    ws->target = -1;

    /* Also update battle state for compatibility */
    battle_give_weapon(player, weapon_type);
}

/**
 * weapon_clear - Clear a player's weapon
 *
 * @param player Player index
 */
void weapon_clear(s32 player) {
    if (player < 0 || player >= WEAPON_MAX_PLAYERS) {
        return;
    }

    sPlayerWeapons[player].type = WEAPON_NONE;
    sPlayerWeapons[player].ammo = 0;
    sPlayerWeapons[player].locked_on = 0;
    sPlayerWeapons[player].target = -1;
}

/**
 * weapon_get_type - Get player's current weapon type
 *
 * @param player Player index
 * @return Weapon type or WEAPON_NONE
 */
s32 weapon_get_type(s32 player) {
    if (player < 0 || player >= WEAPON_MAX_PLAYERS) {
        return WEAPON_NONE;
    }
    return sPlayerWeapons[player].type;
}

/**
 * weapon_get_ammo - Get player's current ammo count
 *
 * @param player Player index
 * @return Ammo count
 */
s32 weapon_get_ammo(s32 player) {
    if (player < 0 || player >= WEAPON_MAX_PLAYERS) {
        return 0;
    }
    return sPlayerWeapons[player].ammo;
}

/**
 * weapon_has_ammo - Check if player has ammo
 *
 * @param player Player index
 * @return 1 if has ammo, 0 otherwise
 */
s32 weapon_has_ammo(s32 player) {
    if (player < 0 || player >= WEAPON_MAX_PLAYERS) {
        return 0;
    }
    return sPlayerWeapons[player].ammo > 0;
}

/* ========================================================================
 * Weapon Firing
 * ======================================================================== */

/**
 * weapon_can_fire - Check if player can fire their weapon
 *
 * @param player Player index
 * @return 1 if can fire, 0 otherwise
 */
s32 weapon_can_fire(s32 player) {
    WeaponState *ws;

    if (player < 0 || player >= WEAPON_MAX_PLAYERS) {
        return 0;
    }

    ws = &sPlayerWeapons[player];

    /* Must have weapon and ammo */
    if (ws->type == WEAPON_NONE || ws->ammo <= 0) {
        return 0;
    }

    /* Must not be on cooldown */
    if (ws->fire_cooldown > 0) {
        return 0;
    }

    return 1;
}

/**
 * weapon_fire - Fire the player's current weapon
 *
 * @param player Player index
 */
void weapon_fire(s32 player) {
    WeaponState *ws;
    CarData *car;
    f32 pos[3], dir[3];

    if (!weapon_can_fire(player)) {
        return;
    }

    ws = &sPlayerWeapons[player];
    car = &car_array[player];

    /* Get firing position (front of car) */
    pos[0] = car->dr_pos[0];
    pos[1] = car->dr_pos[1] + 1.0f;
    pos[2] = car->dr_pos[2];

    /* Get direction from car orientation - simplified for stub */
    dir[0] = 0.0f;
    dir[1] = 0.0f;
    dir[2] = 1.0f;

    /* Fire based on weapon type */
    switch (ws->type) {
        case WEAPON_MISSILE:
            weapon_spawn_missile(player, pos, dir, ws->target);
            ws->fire_cooldown = 30;  /* 0.5 second cooldown */
            break;

        case WEAPON_MINE:
            weapon_place_mine(player, car->dr_pos);
            ws->fire_cooldown = 15;  /* 0.25 second cooldown */
            break;

        case WEAPON_SHOCKWAVE:
            weapon_fire_shockwave(player, car->dr_pos);
            ws->fire_cooldown = 60;  /* 1 second cooldown */
            break;

        case WEAPON_BOOST:
            weapon_activate_boost(player);
            ws->fire_cooldown = 0;
            break;

        case WEAPON_SHIELD:
            weapon_activate_shield(player);
            ws->fire_cooldown = 0;
            break;

        case WEAPON_MACHINEGUN:
            weapon_spawn_bullet(player, pos, dir);
            ws->fire_cooldown = MACHINEGUN_FIRE_RATE;
            break;

        default:
            return;
    }

    /* Consume ammo */
    ws->ammo--;
    ws->last_fire_time = frame_counter;

    /* Clear weapon if out of ammo */
    if (ws->ammo <= 0) {
        ws->type = WEAPON_NONE;
        ws->locked_on = 0;
        ws->target = -1;
    }

    sound_play(SFX_BOOST);  /* Placeholder sound */
}

/**
 * weapon_update_cooldown - Update fire cooldown timer
 *
 * @param player Player index
 */
void weapon_update_cooldown(s32 player) {
    if (player < 0 || player >= WEAPON_MAX_PLAYERS) {
        return;
    }

    if (sPlayerWeapons[player].fire_cooldown > 0) {
        sPlayerWeapons[player].fire_cooldown--;
    }
}

/* ========================================================================
 * Targeting System
 * ======================================================================== */

/**
 * weapon_find_target - Find nearest valid target
 *
 * @param player Player index
 * @param max_range Maximum targeting range
 * @return Target player index or -1
 */
s32 weapon_find_target(s32 player, f32 max_range) {
    s32 i;
    s32 best_target = -1;
    f32 best_dist = max_range;
    f32 my_x, my_z;

    if (player < 0 || player >= WEAPON_MAX_PLAYERS) {
        return -1;
    }

    my_x = car_array[player].dr_pos[0];
    my_z = car_array[player].dr_pos[2];

    for (i = 0; i < WEAPON_MAX_PLAYERS; i++) {
        if (i != player && gBattle.players[i].alive) {
            f32 dx = car_array[i].dr_pos[0] - my_x;
            f32 dz = car_array[i].dr_pos[2] - my_z;
            f32 dist = sqrtf(dx*dx + dz*dz);

            if (dist < best_dist) {
                best_dist = dist;
                best_target = i;
            }
        }
    }

    return best_target;
}

/**
 * weapon_lock_target - Lock onto nearest target
 *
 * @param player Player index
 * @return Target player index or -1
 */
s32 weapon_lock_target(s32 player) {
    s32 target;

    if (player < 0 || player >= WEAPON_MAX_PLAYERS) {
        return -1;
    }

    target = weapon_find_target(player, 200.0f);

    if (target >= 0) {
        sPlayerWeapons[player].locked_on = 1;
        sPlayerWeapons[player].target = target;
    }

    return target;
}

/**
 * weapon_clear_lock - Clear target lock
 *
 * @param player Player index
 */
void weapon_clear_lock(s32 player) {
    if (player < 0 || player >= WEAPON_MAX_PLAYERS) {
        return;
    }

    sPlayerWeapons[player].locked_on = 0;
    sPlayerWeapons[player].target = -1;
}

/**
 * weapon_get_locked_target - Get currently locked target
 *
 * @param player Player index
 * @return Target player index or -1
 */
s32 weapon_get_locked_target(s32 player) {
    if (player < 0 || player >= WEAPON_MAX_PLAYERS) {
        return -1;
    }

    if (!sPlayerWeapons[player].locked_on) {
        return -1;
    }

    return sPlayerWeapons[player].target;
}

/* ========================================================================
 * Projectile Spawning
 * ======================================================================== */

/**
 * weapon_spawn_missile - Spawn a homing missile
 *
 * @param owner Owner player
 * @param pos Starting position
 * @param dir Initial direction
 * @param target Target player (-1 for no homing)
 * @return Projectile index or -1
 */
s32 weapon_spawn_missile(s32 owner, f32 pos[3], f32 dir[3], s32 target) {
    /* Delegate to battle system projectile spawning */
    return battle_spawn_projectile(owner, WEAPON_MISSILE, pos, dir);
}

/**
 * weapon_spawn_bullet - Spawn a machine gun bullet
 *
 * @param owner Owner player
 * @param pos Starting position
 * @param dir Direction
 * @return Projectile index or -1
 */
s32 weapon_spawn_bullet(s32 owner, f32 pos[3], f32 dir[3]) {
    /* Use battle projectile system with machinegun type */
    /* For now, use missile type as placeholder */
    return battle_spawn_projectile(owner, WEAPON_MACHINEGUN, pos, dir);
}

/**
 * weapon_place_mine - Place a mine
 *
 * @param owner Owner player
 * @param pos Position to place mine
 * @return Mine index or -1
 */
s32 weapon_place_mine(s32 owner, f32 pos[3]) {
    return battle_place_mine(owner, pos);
}

/**
 * weapon_fire_shockwave - Fire a shockwave attack
 *
 * @param player Player firing
 * @param pos Center position
 */
void weapon_fire_shockwave(s32 player, f32 pos[3]) {
    s32 i;
    f32 radius_sq = SHOCKWAVE_RADIUS * SHOCKWAVE_RADIUS;

    /* Damage all players within radius */
    for (i = 0; i < WEAPON_MAX_PLAYERS; i++) {
        if (i != player && gBattle.players[i].alive) {
            f32 dx = car_array[i].dr_pos[0] - pos[0];
            f32 dz = car_array[i].dr_pos[2] - pos[2];
            f32 dist_sq = dx*dx + dz*dz;

            if (dist_sq < radius_sq) {
                battle_damage_player(i, SHOCKWAVE_DAMAGE, player);
            }
        }
    }

    sound_play(SFX_CRASH_HEAVY);  /* Placeholder */
}

/* ========================================================================
 * Weapon Effects
 * ======================================================================== */

/**
 * weapon_activate_boost - Activate speed boost
 *
 * @param player Player index
 */
void weapon_activate_boost(s32 player) {
    if (player < 0 || player >= WEAPON_MAX_PLAYERS) {
        return;
    }

    gBattle.players[player].boost_timer = BOOST_DURATION;
    sound_play(SFX_BOOST);
}

/**
 * weapon_activate_shield - Activate shield
 *
 * @param player Player index
 */
void weapon_activate_shield(s32 player) {
    if (player < 0 || player >= WEAPON_MAX_PLAYERS) {
        return;
    }

    gBattle.players[player].shield_timer = SHIELD_DURATION;
    sound_play(SFX_CHECKPOINT);  /* Placeholder */
}

/**
 * weapon_is_boosting - Check if player is boosting
 *
 * @param player Player index
 * @return 1 if boosting, 0 otherwise
 */
s32 weapon_is_boosting(s32 player) {
    if (player < 0 || player >= WEAPON_MAX_PLAYERS) {
        return 0;
    }

    return gBattle.players[player].boost_timer > 0;
}

/**
 * weapon_has_shield - Check if player has shield active
 *
 * @param player Player index
 * @return 1 if shielded, 0 otherwise
 */
s32 weapon_has_shield(s32 player) {
    if (player < 0 || player >= WEAPON_MAX_PLAYERS) {
        return 0;
    }

    return gBattle.players[player].shield_timer > 0;
}

/* ========================================================================
 * Query Functions
 * ======================================================================== */

/**
 * weapon_get_props - Get weapon properties
 *
 * @param weapon_type Weapon type
 * @return Pointer to properties or NULL
 */
const WeaponProps *weapon_get_props(s32 weapon_type) {
    if (weapon_type < 0 || weapon_type >= NUM_WEAPONS) {
        return &gWeaponProps[WEAPON_NONE];
    }

    return &gWeaponProps[weapon_type];
}

/**
 * weapon_get_name - Get weapon display name
 *
 * @param weapon_type Weapon type
 * @return Name string
 */
const char *weapon_get_name(s32 weapon_type) {
    if (weapon_type < 0 || weapon_type >= NUM_WEAPONS) {
        return sWeaponNames[WEAPON_NONE];
    }

    return sWeaponNames[weapon_type];
}

/**
 * weapon_get_damage - Get weapon damage value
 *
 * @param weapon_type Weapon type
 * @return Damage value
 */
s32 weapon_get_damage(s32 weapon_type) {
    if (weapon_type < 0 || weapon_type >= NUM_WEAPONS) {
        return 0;
    }

    return gWeaponProps[weapon_type].damage;
}

/**
 * weapon_get_speed - Get weapon projectile speed
 *
 * @param weapon_type Weapon type
 * @return Speed value
 */
f32 weapon_get_speed(s32 weapon_type) {
    if (weapon_type < 0 || weapon_type >= NUM_WEAPONS) {
        return 0.0f;
    }

    return gWeaponProps[weapon_type].speed;
}

/**
 * weapon_is_offensive - Check if weapon deals damage
 *
 * @param weapon_type Weapon type
 * @return 1 if offensive, 0 otherwise
 */
s32 weapon_is_offensive(s32 weapon_type) {
    if (weapon_type < 0 || weapon_type >= NUM_WEAPONS) {
        return 0;
    }

    return gWeaponProps[weapon_type].damage > 0;
}

/* ========================================================================
 * Pickup Integration
 * ======================================================================== */

/**
 * weapon_random_type - Get a random weapon type
 *
 * @return Random offensive weapon type
 */
s32 weapon_random_type(void) {
    s32 types[] = {
        WEAPON_MISSILE,
        WEAPON_MINE,
        WEAPON_SHOCKWAVE,
        WEAPON_BOOST,
        WEAPON_SHIELD,
        WEAPON_MACHINEGUN
    };

    return types[frame_counter % 6];
}

/**
 * weapon_on_pickup - Handle weapon pickup
 *
 * @param player Player index
 * @param pickup_type Pickup type (PICKUP_WEAPON for random weapon)
 */
void weapon_on_pickup(s32 player, s32 pickup_type) {
    s32 weapon_type;

    if (player < 0 || player >= WEAPON_MAX_PLAYERS) {
        return;
    }

    if (pickup_type == PICKUP_WEAPON) {
        weapon_type = weapon_random_type();
        weapon_give(player, weapon_type);
    }
}

#endif /* NON_MATCHING */
