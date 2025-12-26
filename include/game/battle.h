/**
 * battle.h - Battle mode system for Rush 2049 N64
 *
 * Battle mode is an arena combat mode where players
 * collect weapons and fight to eliminate opponents.
 *
 * Features:
 * - Multiple arena maps
 * - Weapon pickups (missiles, mines, etc.)
 * - Health/damage system
 * - Score tracking
 */

#ifndef BATTLE_H
#define BATTLE_H

#include "types.h"

/* Battle mode states */
#define BATTLE_STATE_INACTIVE   0   /* Not in battle mode */
#define BATTLE_STATE_WAITING    1   /* Waiting for players */
#define BATTLE_STATE_COUNTDOWN  2   /* Pre-battle countdown */
#define BATTLE_STATE_ACTIVE     3   /* Battle in progress */
#define BATTLE_STATE_OVERTIME   4   /* Overtime (tie breaker) */
#define BATTLE_STATE_FINISHED   5   /* Battle over */

/* Weapon types */
#define WEAPON_NONE             0
#define WEAPON_MISSILE          1   /* Homing missile */
#define WEAPON_MINE             2   /* Dropped mine */
#define WEAPON_SHOCKWAVE        3   /* Area effect */
#define WEAPON_BOOST            4   /* Speed boost / ram */
#define WEAPON_SHIELD           5   /* Temporary shield */
#define NUM_WEAPON_TYPES        6

/* Pickup types */
#define PICKUP_WEAPON           0   /* Random weapon */
#define PICKUP_HEALTH           1   /* Health restore */
#define PICKUP_SPEED            2   /* Speed boost */
#define PICKUP_ARMOR            3   /* Damage reduction */
#define NUM_PICKUP_TYPES        4

/* Battle constants */
#define BATTLE_MAX_PLAYERS      4       /* Max players in battle */
#define BATTLE_MAX_PICKUPS      32      /* Max pickups on map */
#define BATTLE_MAX_PROJECTILES  16      /* Max active projectiles */
#define BATTLE_MAX_MINES        8       /* Max mines per player */
#define BATTLE_MAX_HEALTH       100     /* Starting health */
#define BATTLE_RESPAWN_TIME     180     /* Frames to respawn (3 sec) */
#define BATTLE_PICKUP_RESPAWN   300     /* Pickup respawn time (5 sec) */

/* Damage values */
#define DAMAGE_MISSILE          40
#define DAMAGE_MINE             50
#define DAMAGE_SHOCKWAVE        30
#define DAMAGE_RAM              20      /* Boosted collision */
#define DAMAGE_COLLISION        10      /* Normal collision */

/* Projectile structure */
typedef struct BattleProjectile {
    u8      active;             /* Is projectile active? */
    u8      type;               /* Weapon type */
    u8      owner;              /* Player who fired it */
    u8      target;             /* Target player (for homing) */
    f32     pos[3];             /* Current position */
    f32     vel[3];             /* Current velocity */
    f32     dir[3];             /* Direction vector */
    f32     speed;              /* Movement speed */
    f32     lifetime;           /* Remaining lifetime */
    f32     turn_rate;          /* Homing turn rate */
} BattleProjectile;

/* Mine structure */
typedef struct BattleMine {
    u8      active;             /* Is mine placed? */
    u8      owner;              /* Player who placed it */
    u8      armed;              /* Is mine armed? */
    u8      pad;
    f32     pos[3];             /* Mine position */
    f32     arm_timer;          /* Time until armed */
    f32     lifetime;           /* Time until despawn */
} BattleMine;

/* Weapon pickup structure */
typedef struct BattlePickup {
    u8      active;             /* Is pickup available? */
    u8      type;               /* PICKUP_* type */
    u8      weapon;             /* Weapon type (if weapon pickup) */
    u8      respawning;         /* Currently respawning? */
    f32     pos[3];             /* Pickup position */
    f32     rotation;           /* Visual rotation */
    u32     respawn_timer;      /* Frames until respawn */
} BattlePickup;

/* Per-player battle state */
typedef struct BattlePlayer {
    u8      active;             /* Player participating? */
    u8      alive;              /* Currently alive? */
    u8      weapon;             /* Current weapon held */
    u8      ammo;               /* Weapon ammo count */

    s32     health;             /* Current health */
    s32     max_health;         /* Maximum health */
    s32     armor;              /* Armor percentage (damage reduction) */

    u32     respawn_timer;      /* Frames until respawn */
    f32     spawn_pos[3];       /* Respawn position */

    /* Stats */
    s32     kills;              /* Total kills */
    s32     deaths;             /* Total deaths */
    s32     self_destructs;     /* Self-destructs */
    s32     pickups_collected;  /* Pickups grabbed */

    /* Temporary effects */
    u32     shield_timer;       /* Shield active time */
    u32     boost_timer;        /* Speed boost time */
    f32     damage_mult;        /* Damage multiplier */

} BattlePlayer;

/* Battle mode state */
typedef struct BattleState {
    u8      state;              /* BATTLE_STATE_* */
    u8      num_players;        /* Number of active players */
    u8      arena_id;           /* Current arena */
    u8      game_mode;          /* Battle sub-mode */

    u32     time_limit;         /* Time limit (frames) */
    u32     time_remaining;     /* Time left */
    s32     kill_limit;         /* First to X kills wins */
    s32     stock_limit;        /* Lives per player (0 = infinite) */

    /* Players */
    BattlePlayer players[BATTLE_MAX_PLAYERS];

    /* Projectiles */
    BattleProjectile projectiles[BATTLE_MAX_PROJECTILES];
    s32     num_projectiles;

    /* Mines */
    BattleMine mines[BATTLE_MAX_MINES * BATTLE_MAX_PLAYERS];
    s32     num_mines;

    /* Pickups */
    BattlePickup pickups[BATTLE_MAX_PICKUPS];
    s32     num_pickups;

    /* Match stats */
    u32     match_time;         /* Total match duration */
    s32     winner;             /* Winner player index (-1 if none) */

} BattleState;

/* Global battle state */
extern BattleState gBattle;

/* Initialization */
void battle_init(void);
void battle_reset(void);
void battle_start(s32 arena_id, s32 num_players);
void battle_end(void);

/* Per-frame update */
void battle_update(void);
void battle_update_players(void);
void battle_update_projectiles(void);
void battle_update_mines(void);
void battle_update_pickups(void);

/* Player actions */
void battle_fire_weapon(s32 player);
void battle_use_boost(s32 player);
void battle_drop_mine(s32 player);

/* Damage and health */
void battle_damage_player(s32 player, s32 damage, s32 attacker);
void battle_heal_player(s32 player, s32 amount);
void battle_kill_player(s32 player, s32 killer);
void battle_respawn_player(s32 player);

/* Weapons and pickups */
void battle_give_weapon(s32 player, s32 weapon_type);
void battle_collect_pickup(s32 player, s32 pickup_index);
void battle_spawn_pickup(s32 index, f32 pos[3], s32 type);
void battle_respawn_pickups(void);

/* Projectile management */
s32 battle_spawn_projectile(s32 owner, s32 type, f32 pos[3], f32 dir[3]);
void battle_update_projectile(s32 index);
void battle_destroy_projectile(s32 index);

/* Mine management */
s32 battle_place_mine(s32 owner, f32 pos[3]);
void battle_update_mine(s32 index);
void battle_trigger_mine(s32 index);

/* Collision */
void battle_check_projectile_hits(void);
void battle_check_mine_triggers(void);
void battle_check_pickup_collection(void);
void battle_check_player_collisions(void);

/* Queries */
s32 battle_is_active(void);
s32 battle_player_alive(s32 player);
s32 battle_get_leader(void);
s32 battle_get_kills(s32 player);
s32 battle_get_deaths(s32 player);
s32 battle_get_health(s32 player);
s32 battle_get_weapon(s32 player);

/* Match flow */
void battle_check_win_condition(void);
void battle_enter_overtime(void);
void battle_declare_winner(s32 player);

/* Arena */
void battle_load_arena(s32 arena_id);
void battle_get_spawn_point(s32 player, f32 *pos);

/* Display */
void battle_draw_hud(void);
void battle_draw_player_hud(s32 player);
void battle_draw_weapons(void);
void battle_draw_pickups(void);
void battle_draw_projectiles(void);
void battle_draw_results(void);

#endif /* BATTLE_H */
