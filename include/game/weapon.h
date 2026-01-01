/**
 * weapon.h - Weapon system for Rush 2049 N64 Battle Mode
 *
 * This file defines the weapon types, projectile properties, and
 * weapon-related functions for the battle mode combat system.
 *
 * Rush 2049 battle mode is unique to the N64 version - the arcade
 * "Rush The Rock" source does not include weapons or battle mode.
 * This is a console-exclusive feature.
 *
 * Weapon Categories:
 * - Offensive: Missile, Machine Gun, Mine, Shockwave
 * - Defensive: Shield
 * - Utility: Speed Boost
 */

#ifndef WEAPON_H
#define WEAPON_H

#include "types.h"

/* ========================================================================
 * Weapon Type Constants
 * ======================================================================== */

/* Weapon type IDs - matches battle.h for compatibility */
#define WEAPON_NONE             0   /* No weapon equipped */
#define WEAPON_MISSILE          1   /* Homing missile - tracks nearest enemy */
#define WEAPON_MINE             2   /* Dropped mine - explodes on contact */
#define WEAPON_SHOCKWAVE        3   /* Area effect - damages nearby enemies */
#define WEAPON_BOOST            4   /* Speed boost - enables ramming damage */
#define WEAPON_SHIELD           5   /* Temporary shield - blocks damage */
#define WEAPON_MACHINEGUN       6   /* Machine gun - rapid fire bullets */
#define NUM_WEAPONS             7   /* Total weapon types */

/* ========================================================================
 * Weapon Property Constants
 * ======================================================================== */

/* Missile properties */
#define MISSILE_SPEED           80.0f   /* Units per second */
#define MISSILE_TURN_RATE       3.0f    /* Radians per second */
#define MISSILE_LIFETIME        5.0f    /* Seconds */
#define MISSILE_DAMAGE          40      /* Damage points */
#define MISSILE_AMMO            3       /* Starting ammo */
#define MISSILE_RADIUS          5.0f    /* Hit radius */

/* Mine properties */
#define MINE_DAMAGE             50      /* Damage points */
#define MINE_AMMO               2       /* Starting ammo */
#define MINE_ARM_TIME           1.0f    /* Seconds to arm */
#define MINE_LIFETIME           30.0f   /* Seconds before despawn */
#define MINE_TRIGGER_RADIUS     8.0f    /* Detection radius */
#define MINE_BLAST_RADIUS       15.0f   /* Explosion radius */

/* Shockwave properties */
#define SHOCKWAVE_DAMAGE        30      /* Damage points */
#define SHOCKWAVE_AMMO          1       /* Single use */
#define SHOCKWAVE_RADIUS        30.0f   /* Effect radius */
#define SHOCKWAVE_DURATION      0.5f    /* Visual duration */

/* Speed boost properties */
#define BOOST_AMMO              5       /* Number of uses */
#define BOOST_DURATION          180     /* Frames (3 seconds) */
#define BOOST_SPEED_MULT        1.5f    /* Speed multiplier */
#define BOOST_RAM_DAMAGE        20      /* Collision damage while boosting */

/* Shield properties */
#define SHIELD_AMMO             1       /* Single use */
#define SHIELD_DURATION         300     /* Frames (5 seconds) */

/* Machine gun properties */
#define MACHINEGUN_DAMAGE       5       /* Damage per hit */
#define MACHINEGUN_AMMO         50      /* Starting ammo */
#define MACHINEGUN_FIRE_RATE    6       /* Frames between shots */
#define MACHINEGUN_SPEED        120.0f  /* Bullet speed */
#define MACHINEGUN_LIFETIME     1.0f    /* Bullet lifetime */

/* ========================================================================
 * Projectile Constants
 * ======================================================================== */

/* Maximum active projectiles */
#define WEAPON_MAX_PROJECTILES  16
#define WEAPON_MAX_MINES        8   /* Per player */

/* Projectile states */
#define PROJ_STATE_INACTIVE     0
#define PROJ_STATE_ACTIVE       1
#define PROJ_STATE_EXPLODING    2

/* ========================================================================
 * Weapon Properties Structure
 * ======================================================================== */

/**
 * WeaponProps - Static weapon property data
 *
 * Used to define the behavior of each weapon type.
 * Index corresponds to WEAPON_* constants.
 */
typedef struct WeaponProps {
    s32     damage;         /* Damage dealt on hit */
    f32     speed;          /* Projectile speed (0 for instant) */
    f32     lifetime;       /* Duration in seconds */
    f32     turn_rate;      /* Homing turn rate (0 for non-homing) */
    s32     ammo;           /* Starting ammo when picked up */
    f32     radius;         /* Hit/effect radius */
    u8      is_projectile;  /* Creates a projectile? */
    u8      is_homing;      /* Homes toward target? */
    u8      is_area;        /* Area effect weapon? */
    u8      pad;
} WeaponProps;

/* Global weapon properties table - indexed by WEAPON_* */
extern const WeaponProps gWeaponProps[NUM_WEAPONS];

/* ========================================================================
 * Weapon State Structure
 * ======================================================================== */

/**
 * WeaponState - Per-player weapon state
 */
typedef struct WeaponState {
    u8      type;           /* Current weapon type */
    u8      ammo;           /* Remaining ammo */
    u8      fire_cooldown;  /* Frames until can fire again */
    u8      locked_on;      /* Has homing lock? */
    s32     target;         /* Locked target player (-1 if none) */
    u32     last_fire_time; /* Frame of last fire */
} WeaponState;

/* ========================================================================
 * Weapon Function Prototypes
 * ======================================================================== */

/* Initialization */
void weapon_init(void);
void weapon_reset(void);
void weapon_reset_player(s32 player);

/* Weapon state management */
void weapon_give(s32 player, s32 weapon_type);
void weapon_clear(s32 player);
s32 weapon_get_type(s32 player);
s32 weapon_get_ammo(s32 player);
s32 weapon_has_ammo(s32 player);

/* Weapon firing */
s32 weapon_can_fire(s32 player);
void weapon_fire(s32 player);
void weapon_update_cooldown(s32 player);

/* Targeting */
s32 weapon_find_target(s32 player, f32 max_range);
s32 weapon_lock_target(s32 player);
void weapon_clear_lock(s32 player);
s32 weapon_get_locked_target(s32 player);

/* Projectile spawning */
s32 weapon_spawn_missile(s32 owner, f32 pos[3], f32 dir[3], s32 target);
s32 weapon_spawn_bullet(s32 owner, f32 pos[3], f32 dir[3]);
s32 weapon_place_mine(s32 owner, f32 pos[3]);
void weapon_fire_shockwave(s32 player, f32 pos[3]);

/* Weapon effects */
void weapon_activate_boost(s32 player);
void weapon_activate_shield(s32 player);
s32 weapon_is_boosting(s32 player);
s32 weapon_has_shield(s32 player);

/* Query functions */
const WeaponProps *weapon_get_props(s32 weapon_type);
const char *weapon_get_name(s32 weapon_type);
s32 weapon_get_damage(s32 weapon_type);
f32 weapon_get_speed(s32 weapon_type);
s32 weapon_is_offensive(s32 weapon_type);

/* Pickup integration */
s32 weapon_random_type(void);
void weapon_on_pickup(s32 player, s32 pickup_type);

#endif /* WEAPON_H */
