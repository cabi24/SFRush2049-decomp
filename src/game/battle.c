/**
 * battle.c - Battle mode system for Rush 2049 N64
 *
 * Battle mode is an arena combat mode where players
 * collect weapons and fight to eliminate opponents.
 *
 * N64-exclusive mode not present in Rush The Rock arcade.
 * Based on similar vehicle combat game mechanics.
 */

#include "types.h"
#include "game/battle.h"
#include "game/structs.h"
#include "game/sound.h"

#ifdef NON_MATCHING

/* External game state */
extern CarData car_array[];
extern s32 num_active_cars;
extern u32 frame_counter;

/* External math */
extern f32 sqrtf(f32 x);

/* Global battle state */
BattleState gBattle;

/* Arena spawn points (per arena, per player) */
static const f32 sSpawnPoints[4][4][3] = {
    /* Arena 0 */
    { {100.0f, 0.0f, 100.0f}, {-100.0f, 0.0f, 100.0f},
      {100.0f, 0.0f, -100.0f}, {-100.0f, 0.0f, -100.0f} },
    /* Arena 1 */
    { {150.0f, 0.0f, 0.0f}, {-150.0f, 0.0f, 0.0f},
      {0.0f, 0.0f, 150.0f}, {0.0f, 0.0f, -150.0f} },
    /* Arena 2 */
    { {50.0f, 0.0f, 50.0f}, {-50.0f, 0.0f, 50.0f},
      {50.0f, 0.0f, -50.0f}, {-50.0f, 0.0f, -50.0f} },
    /* Arena 3 */
    { {200.0f, 0.0f, 200.0f}, {-200.0f, 0.0f, 200.0f},
      {200.0f, 0.0f, -200.0f}, {-200.0f, 0.0f, -200.0f} }
};

/* Weapon properties table */
static const struct WeaponProps {
    s32 damage;
    f32 speed;
    f32 lifetime;
    f32 turn_rate;
    s32 ammo;
} sWeaponProps[NUM_WEAPON_TYPES] = {
    { 0, 0.0f, 0.0f, 0.0f, 0 },                /* WEAPON_NONE */
    { DAMAGE_MISSILE, 80.0f, 5.0f, 3.0f, 3 },  /* WEAPON_MISSILE */
    { DAMAGE_MINE, 0.0f, 30.0f, 0.0f, 2 },     /* WEAPON_MINE */
    { DAMAGE_SHOCKWAVE, 0.0f, 0.5f, 0.0f, 1 }, /* WEAPON_SHOCKWAVE */
    { DAMAGE_RAM, 0.0f, 3.0f, 0.0f, 5 },       /* WEAPON_BOOST */
    { 0, 0.0f, 5.0f, 0.0f, 1 }                 /* WEAPON_SHIELD */
};

/* Pickup health restore amount */
#define PICKUP_HEALTH_AMOUNT 50

/* Collision radii */
#define PROJECTILE_HIT_RADIUS   5.0f
#define MINE_TRIGGER_RADIUS     8.0f
#define PICKUP_COLLECT_RADIUS   5.0f
#define PLAYER_COLLISION_RADIUS 3.0f
#define SHOCKWAVE_RADIUS        30.0f
#define MINE_DAMAGE_RADIUS      15.0f

/* ABS macro for integer/float absolute value */
#define ABS(x) (((x) < 0) ? -(x) : (x))

/*
 * ==========================================================================
 * Initialization Functions
 * ==========================================================================
 */

/**
 * battle_init - Initialize battle system
 */
void battle_init(void) {
    battle_reset();
}

/**
 * battle_reset - Reset all battle state
 */
void battle_reset(void) {
    s32 i;

    gBattle.state = BATTLE_STATE_INACTIVE;
    gBattle.num_players = 0;
    gBattle.arena_id = 0;
    gBattle.game_mode = 0;

    gBattle.time_limit = 60 * 60 * 3;  /* 3 minutes at 60fps */
    gBattle.time_remaining = gBattle.time_limit;
    gBattle.kill_limit = 10;
    gBattle.stock_limit = 0;

    /* Reset all players */
    for (i = 0; i < BATTLE_MAX_PLAYERS; i++) {
        gBattle.players[i].active = 0;
        gBattle.players[i].alive = 0;
        gBattle.players[i].weapon = WEAPON_NONE;
        gBattle.players[i].ammo = 0;
        gBattle.players[i].health = BATTLE_MAX_HEALTH;
        gBattle.players[i].max_health = BATTLE_MAX_HEALTH;
        gBattle.players[i].armor = 0;
        gBattle.players[i].respawn_timer = 0;
        gBattle.players[i].kills = 0;
        gBattle.players[i].deaths = 0;
        gBattle.players[i].self_destructs = 0;
        gBattle.players[i].pickups_collected = 0;
        gBattle.players[i].shield_timer = 0;
        gBattle.players[i].boost_timer = 0;
        gBattle.players[i].damage_mult = 1.0f;
    }

    /* Clear projectiles */
    gBattle.num_projectiles = 0;
    for (i = 0; i < BATTLE_MAX_PROJECTILES; i++) {
        gBattle.projectiles[i].active = 0;
    }

    /* Clear mines */
    gBattle.num_mines = 0;
    for (i = 0; i < BATTLE_MAX_MINES * BATTLE_MAX_PLAYERS; i++) {
        gBattle.mines[i].active = 0;
    }

    /* Clear pickups */
    gBattle.num_pickups = 0;
    for (i = 0; i < BATTLE_MAX_PICKUPS; i++) {
        gBattle.pickups[i].active = 0;
    }

    gBattle.match_time = 0;
    gBattle.winner = -1;
}

/**
 * battle_start - Start a battle match
 */
void battle_start(s32 arena_id, s32 num_players) {
    s32 i;

    battle_reset();

    gBattle.arena_id = (u8)arena_id;
    gBattle.num_players = (u8)num_players;
    gBattle.state = BATTLE_STATE_COUNTDOWN;
    gBattle.time_remaining = gBattle.time_limit;

    /* Activate participating players */
    for (i = 0; i < num_players && i < BATTLE_MAX_PLAYERS; i++) {
        gBattle.players[i].active = 1;
        gBattle.players[i].alive = 1;
        gBattle.players[i].health = BATTLE_MAX_HEALTH;
        gBattle.players[i].weapon = WEAPON_NONE;
        gBattle.players[i].ammo = 0;

        /* Assign spawn position */
        battle_get_spawn_point(i, gBattle.players[i].spawn_pos);
    }

    /* Load arena geometry and spawn initial pickups */
    battle_load_arena(arena_id);
}

/**
 * battle_end - End current battle
 */
void battle_end(void) {
    gBattle.state = BATTLE_STATE_FINISHED;
}

/*
 * ==========================================================================
 * Per-Frame Update Functions
 * ==========================================================================
 */

/**
 * battle_update - Main battle update (called every frame)
 */
void battle_update(void) {
    switch (gBattle.state) {
        case BATTLE_STATE_INACTIVE:
            /* Nothing to do */
            break;

        case BATTLE_STATE_WAITING:
            /* Wait for all players ready */
            break;

        case BATTLE_STATE_COUNTDOWN:
            /* Pre-battle countdown (3 seconds) */
            gBattle.match_time++;
            if (gBattle.match_time >= 180) {
                gBattle.state = BATTLE_STATE_ACTIVE;
                gBattle.match_time = 0;
            }
            break;

        case BATTLE_STATE_ACTIVE:
            /* Main battle logic */
            gBattle.match_time++;

            /* Decrement time remaining */
            if (gBattle.time_remaining > 0) {
                gBattle.time_remaining--;
            }

            /* Update all subsystems */
            battle_update_players();
            battle_update_projectiles();
            battle_update_mines();
            battle_update_pickups();

            /* Check all collisions */
            battle_check_projectile_hits();
            battle_check_mine_triggers();
            battle_check_pickup_collection();
            battle_check_player_collisions();

            /* Check for win condition */
            battle_check_win_condition();
            break;

        case BATTLE_STATE_OVERTIME:
            /* Sudden death - first kill wins */
            gBattle.match_time++;
            battle_update_players();
            battle_update_projectiles();
            battle_check_projectile_hits();
            battle_check_win_condition();
            break;

        case BATTLE_STATE_FINISHED:
            /* Show results screen */
            break;
    }
}

/**
 * battle_update_players - Update all player states
 */
void battle_update_players(void) {
    s32 i;
    BattlePlayer *p;

    for (i = 0; i < BATTLE_MAX_PLAYERS; i++) {
        p = &gBattle.players[i];

        if (!p->active) {
            continue;
        }

        /* Update respawn timer for dead players */
        if (!p->alive && p->respawn_timer > 0) {
            p->respawn_timer--;
            if (p->respawn_timer == 0) {
                battle_respawn_player(i);
            }
        }

        /* Update temporary effect timers */
        if (p->shield_timer > 0) {
            p->shield_timer--;
        }
        if (p->boost_timer > 0) {
            p->boost_timer--;
        }
    }
}

/**
 * battle_update_projectiles - Update all active projectiles
 */
void battle_update_projectiles(void) {
    s32 i;

    for (i = 0; i < BATTLE_MAX_PROJECTILES; i++) {
        if (gBattle.projectiles[i].active) {
            battle_update_projectile(i);
        }
    }
}

/**
 * battle_update_mines - Update all placed mines
 */
void battle_update_mines(void) {
    s32 i;

    for (i = 0; i < BATTLE_MAX_MINES * BATTLE_MAX_PLAYERS; i++) {
        if (gBattle.mines[i].active) {
            battle_update_mine(i);
        }
    }
}

/**
 * battle_update_pickups - Update all pickup spawns
 */
void battle_update_pickups(void) {
    s32 i;
    BattlePickup *pickup;

    for (i = 0; i < BATTLE_MAX_PICKUPS; i++) {
        pickup = &gBattle.pickups[i];

        /* Handle respawning pickups */
        if (pickup->respawning) {
            pickup->respawn_timer--;
            if (pickup->respawn_timer == 0) {
                pickup->respawning = 0;
                pickup->active = 1;
            }
        }

        /* Rotate active pickups for visual effect */
        if (pickup->active) {
            pickup->rotation += 2.0f;
            if (pickup->rotation >= 360.0f) {
                pickup->rotation -= 360.0f;
            }
        }
    }
}

/*
 * ==========================================================================
 * Player Action Functions
 * ==========================================================================
 */

/**
 * battle_fire_weapon - Fire player's current weapon
 */
void battle_fire_weapon(s32 player) {
    BattlePlayer *p;
    CarData *car;
    f32 pos[3];
    f32 dir[3];
    s32 i;
    f32 dx, dz, dist;

    if (player < 0 || player >= BATTLE_MAX_PLAYERS) {
        return;
    }

    p = &gBattle.players[player];

    if (!p->alive || p->weapon == WEAPON_NONE || p->ammo <= 0) {
        return;
    }

    car = &car_array[player];

    /* Get firing position (front of car) */
    pos[0] = car->dr_pos[0];
    pos[1] = car->dr_pos[1] + 1.0f;
    pos[2] = car->dr_pos[2];

    /* Get direction from car orientation */
    dir[0] = 0.0f;
    dir[1] = 0.0f;
    dir[2] = 1.0f;  /* TODO: Use car's actual forward vector */

    switch (p->weapon) {
        case WEAPON_MISSILE:
            battle_spawn_projectile(player, WEAPON_MISSILE, pos, dir);
            break;

        case WEAPON_MINE:
            battle_place_mine(player, car->dr_pos);
            break;

        case WEAPON_SHOCKWAVE:
            /* Damage all nearby players */
            for (i = 0; i < BATTLE_MAX_PLAYERS; i++) {
                if (i != player && gBattle.players[i].alive) {
                    dx = car_array[i].dr_pos[0] - car->dr_pos[0];
                    dz = car_array[i].dr_pos[2] - car->dr_pos[2];
                    dist = sqrtf(dx*dx + dz*dz);
                    if (dist < SHOCKWAVE_RADIUS) {
                        battle_damage_player(i, DAMAGE_SHOCKWAVE, player);
                    }
                }
            }
            break;

        case WEAPON_BOOST:
            /* Activate speed boost */
            p->boost_timer = 180;  /* 3 seconds */
            break;

        case WEAPON_SHIELD:
            /* Activate protective shield */
            p->shield_timer = 300;  /* 5 seconds */
            break;
    }

    p->ammo--;
    if (p->ammo <= 0) {
        p->weapon = WEAPON_NONE;
    }

    sound_play(SFX_BOOST);  /* Placeholder sound */
}

/**
 * battle_use_boost - Use boost weapon if equipped
 */
void battle_use_boost(s32 player) {
    if (player < 0 || player >= BATTLE_MAX_PLAYERS) {
        return;
    }

    if (gBattle.players[player].weapon == WEAPON_BOOST) {
        battle_fire_weapon(player);
    }
}

/**
 * battle_drop_mine - Drop a mine if equipped
 */
void battle_drop_mine(s32 player) {
    if (player < 0 || player >= BATTLE_MAX_PLAYERS) {
        return;
    }

    if (gBattle.players[player].weapon == WEAPON_MINE) {
        battle_fire_weapon(player);
    }
}

/*
 * ==========================================================================
 * Damage and Health Functions
 * ==========================================================================
 */

/**
 * battle_damage_player - Apply damage to a player
 */
void battle_damage_player(s32 player, s32 damage, s32 attacker) {
    BattlePlayer *p;
    s32 actual_damage;

    if (player < 0 || player >= BATTLE_MAX_PLAYERS) {
        return;
    }

    p = &gBattle.players[player];

    if (!p->alive) {
        return;
    }

    /* Shield blocks all damage */
    if (p->shield_timer > 0) {
        return;
    }

    /* Apply armor damage reduction */
    actual_damage = damage - (damage * p->armor / 100);
    if (actual_damage < 1) {
        actual_damage = 1;
    }

    p->health -= actual_damage;

    if (p->health <= 0) {
        p->health = 0;
        battle_kill_player(player, attacker);
    }

    sound_play(SFX_CRASH_LIGHT);
}

/**
 * battle_heal_player - Heal a player
 */
void battle_heal_player(s32 player, s32 amount) {
    BattlePlayer *p;

    if (player < 0 || player >= BATTLE_MAX_PLAYERS) {
        return;
    }

    p = &gBattle.players[player];
    p->health += amount;

    if (p->health > p->max_health) {
        p->health = p->max_health;
    }
}

/**
 * battle_kill_player - Handle player death
 */
void battle_kill_player(s32 player, s32 killer) {
    BattlePlayer *p;

    p = &gBattle.players[player];
    p->alive = 0;
    p->deaths++;
    p->respawn_timer = BATTLE_RESPAWN_TIME;

    /* Award kill to attacker */
    if (killer >= 0 && killer < BATTLE_MAX_PLAYERS && killer != player) {
        gBattle.players[killer].kills++;
    } else {
        /* Self-destruct or environment kill */
        p->self_destructs++;
    }

    /* Drop weapon on death */
    p->weapon = WEAPON_NONE;
    p->ammo = 0;

    sound_play(SFX_CRASH_HEAVY);
}

/**
 * battle_respawn_player - Respawn a dead player
 */
void battle_respawn_player(s32 player) {
    BattlePlayer *p;
    CarData *car;

    if (player < 0 || player >= BATTLE_MAX_PLAYERS) {
        return;
    }

    p = &gBattle.players[player];
    car = &car_array[player];

    p->alive = 1;
    p->health = p->max_health;
    p->weapon = WEAPON_NONE;
    p->ammo = 0;
    p->shield_timer = 60;  /* Brief spawn protection */

    /* Teleport car to spawn point */
    car->dr_pos[0] = p->spawn_pos[0];
    car->dr_pos[1] = p->spawn_pos[1];
    car->dr_pos[2] = p->spawn_pos[2];
    car->mph = 0.0f;
}

/*
 * ==========================================================================
 * Weapon and Pickup Functions
 * ==========================================================================
 */

/**
 * battle_give_weapon - Give a weapon to player
 */
void battle_give_weapon(s32 player, s32 weapon_type) {
    BattlePlayer *p;

    if (player < 0 || player >= BATTLE_MAX_PLAYERS) {
        return;
    }

    p = &gBattle.players[player];
    p->weapon = (u8)weapon_type;
    p->ammo = (u8)sWeaponProps[weapon_type].ammo;
}

/**
 * battle_collect_pickup - Collect a pickup item
 */
void battle_collect_pickup(s32 player, s32 pickup_index) {
    BattlePickup *pickup;
    BattlePlayer *p;

    if (player < 0 || player >= BATTLE_MAX_PLAYERS) {
        return;
    }
    if (pickup_index < 0 || pickup_index >= BATTLE_MAX_PICKUPS) {
        return;
    }

    pickup = &gBattle.pickups[pickup_index];
    p = &gBattle.players[player];

    if (!pickup->active || !p->alive) {
        return;
    }

    switch (pickup->type) {
        case PICKUP_WEAPON:
            battle_give_weapon(player, pickup->weapon);
            break;

        case PICKUP_HEALTH:
            battle_heal_player(player, PICKUP_HEALTH_AMOUNT);
            break;

        case PICKUP_SPEED:
            p->boost_timer = 180;  /* 3 seconds */
            break;

        case PICKUP_ARMOR:
            p->armor = 50;  /* 50% damage reduction */
            break;
    }

    p->pickups_collected++;

    /* Start respawn timer */
    pickup->active = 0;
    pickup->respawning = 1;
    pickup->respawn_timer = BATTLE_PICKUP_RESPAWN;

    sound_play(SFX_CHECKPOINT);
}

/**
 * battle_spawn_pickup - Spawn a pickup at location
 */
void battle_spawn_pickup(s32 index, f32 pos[3], s32 type) {
    BattlePickup *pickup;

    if (index < 0 || index >= BATTLE_MAX_PICKUPS) {
        return;
    }

    pickup = &gBattle.pickups[index];
    pickup->active = 1;
    pickup->type = (u8)type;
    pickup->respawning = 0;
    pickup->pos[0] = pos[0];
    pickup->pos[1] = pos[1];
    pickup->pos[2] = pos[2];
    pickup->rotation = 0.0f;
    pickup->respawn_timer = 0;

    /* Assign random weapon for weapon pickups */
    if (type == PICKUP_WEAPON) {
        pickup->weapon = (u8)(1 + (frame_counter % 5));  /* Random 1-5 */
    }

    gBattle.num_pickups++;
}

/**
 * battle_respawn_pickups - Force respawn all inactive pickups
 */
void battle_respawn_pickups(void) {
    s32 i;

    for (i = 0; i < gBattle.num_pickups; i++) {
        if (!gBattle.pickups[i].active && !gBattle.pickups[i].respawning) {
            gBattle.pickups[i].active = 1;
        }
    }
}

/*
 * ==========================================================================
 * Projectile Management Functions
 * ==========================================================================
 */

/**
 * battle_spawn_projectile - Create a new projectile
 */
s32 battle_spawn_projectile(s32 owner, s32 type, f32 pos[3], f32 dir[3]) {
    s32 i, j;
    BattleProjectile *proj;
    f32 best_dist;
    s32 best_target;
    f32 dx, dz, dist;

    /* Find free projectile slot */
    for (i = 0; i < BATTLE_MAX_PROJECTILES; i++) {
        if (!gBattle.projectiles[i].active) {
            proj = &gBattle.projectiles[i];

            proj->active = 1;
            proj->type = (u8)type;
            proj->owner = (u8)owner;
            proj->target = 255;  /* No target initially */

            proj->pos[0] = pos[0];
            proj->pos[1] = pos[1];
            proj->pos[2] = pos[2];

            proj->dir[0] = dir[0];
            proj->dir[1] = dir[1];
            proj->dir[2] = dir[2];

            proj->vel[0] = dir[0] * sWeaponProps[type].speed;
            proj->vel[1] = dir[1] * sWeaponProps[type].speed;
            proj->vel[2] = dir[2] * sWeaponProps[type].speed;

            proj->speed = sWeaponProps[type].speed;
            proj->lifetime = sWeaponProps[type].lifetime;
            proj->turn_rate = sWeaponProps[type].turn_rate;

            /* Find homing target for missiles */
            if (type == WEAPON_MISSILE) {
                best_dist = 1000.0f;
                best_target = -1;

                for (j = 0; j < BATTLE_MAX_PLAYERS; j++) {
                    if (j != owner && gBattle.players[j].alive) {
                        dx = car_array[j].dr_pos[0] - pos[0];
                        dz = car_array[j].dr_pos[2] - pos[2];
                        dist = sqrtf(dx*dx + dz*dz);
                        if (dist < best_dist) {
                            best_dist = dist;
                            best_target = j;
                        }
                    }
                }
                if (best_target >= 0) {
                    proj->target = (u8)best_target;
                }
            }

            gBattle.num_projectiles++;
            return i;
        }
    }

    return -1;  /* No free slot */
}

/**
 * battle_update_projectile - Update a single projectile
 */
void battle_update_projectile(s32 index) {
    BattleProjectile *proj;
    f32 dt;
    f32 target_pos[3];
    f32 to_target[3];
    f32 len;

    dt = 1.0f / 60.0f;
    proj = &gBattle.projectiles[index];

    if (!proj->active) {
        return;
    }

    /* Update lifetime */
    proj->lifetime -= dt;
    if (proj->lifetime <= 0.0f) {
        battle_destroy_projectile(index);
        return;
    }

    /* Homing behavior for missiles */
    if (proj->target < BATTLE_MAX_PLAYERS && gBattle.players[proj->target].alive) {
        target_pos[0] = car_array[proj->target].dr_pos[0];
        target_pos[1] = car_array[proj->target].dr_pos[1];
        target_pos[2] = car_array[proj->target].dr_pos[2];

        to_target[0] = target_pos[0] - proj->pos[0];
        to_target[1] = target_pos[1] - proj->pos[1];
        to_target[2] = target_pos[2] - proj->pos[2];

        len = sqrtf(to_target[0]*to_target[0] + to_target[1]*to_target[1] + to_target[2]*to_target[2]);

        if (len > 0.001f) {
            to_target[0] /= len;
            to_target[1] /= len;
            to_target[2] /= len;

            /* Turn toward target */
            proj->dir[0] += (to_target[0] - proj->dir[0]) * proj->turn_rate * dt;
            proj->dir[1] += (to_target[1] - proj->dir[1]) * proj->turn_rate * dt;
            proj->dir[2] += (to_target[2] - proj->dir[2]) * proj->turn_rate * dt;

            /* Renormalize direction */
            len = sqrtf(proj->dir[0]*proj->dir[0] + proj->dir[1]*proj->dir[1] + proj->dir[2]*proj->dir[2]);
            if (len > 0.001f) {
                proj->dir[0] /= len;
                proj->dir[1] /= len;
                proj->dir[2] /= len;
            }
        }
    }

    /* Update velocity from direction */
    proj->vel[0] = proj->dir[0] * proj->speed;
    proj->vel[1] = proj->dir[1] * proj->speed;
    proj->vel[2] = proj->dir[2] * proj->speed;

    /* Update position */
    proj->pos[0] += proj->vel[0] * dt;
    proj->pos[1] += proj->vel[1] * dt;
    proj->pos[2] += proj->vel[2] * dt;
}

/**
 * battle_destroy_projectile - Remove a projectile
 */
void battle_destroy_projectile(s32 index) {
    if (index >= 0 && index < BATTLE_MAX_PROJECTILES) {
        gBattle.projectiles[index].active = 0;
        gBattle.num_projectiles--;
    }
}

/*
 * ==========================================================================
 * Mine Management Functions
 * ==========================================================================
 */

/**
 * battle_place_mine - Place a mine at location
 */
s32 battle_place_mine(s32 owner, f32 pos[3]) {
    s32 i;
    BattleMine *mine;

    for (i = 0; i < BATTLE_MAX_MINES * BATTLE_MAX_PLAYERS; i++) {
        if (!gBattle.mines[i].active) {
            mine = &gBattle.mines[i];

            mine->active = 1;
            mine->owner = (u8)owner;
            mine->armed = 0;
            mine->pos[0] = pos[0];
            mine->pos[1] = pos[1];
            mine->pos[2] = pos[2];
            mine->arm_timer = 1.0f;   /* 1 second to arm */
            mine->lifetime = 30.0f;   /* 30 seconds lifetime */

            gBattle.num_mines++;
            return i;
        }
    }

    return -1;
}

/**
 * battle_update_mine - Update a single mine
 */
void battle_update_mine(s32 index) {
    BattleMine *mine;
    f32 dt;

    dt = 1.0f / 60.0f;
    mine = &gBattle.mines[index];

    if (!mine->active) {
        return;
    }

    /* Arm timer */
    if (!mine->armed) {
        mine->arm_timer -= dt;
        if (mine->arm_timer <= 0.0f) {
            mine->armed = 1;
        }
    }

    /* Lifetime countdown */
    mine->lifetime -= dt;
    if (mine->lifetime <= 0.0f) {
        mine->active = 0;
        gBattle.num_mines--;
    }
}

/**
 * battle_trigger_mine - Explode a mine
 */
void battle_trigger_mine(s32 index) {
    BattleMine *mine;
    s32 i;
    f32 dx, dz, dist;

    mine = &gBattle.mines[index];

    if (!mine->active || !mine->armed) {
        return;
    }

    /* Damage all players in blast radius */
    for (i = 0; i < BATTLE_MAX_PLAYERS; i++) {
        if (gBattle.players[i].alive) {
            dx = car_array[i].dr_pos[0] - mine->pos[0];
            dz = car_array[i].dr_pos[2] - mine->pos[2];
            dist = sqrtf(dx*dx + dz*dz);

            if (dist < MINE_DAMAGE_RADIUS) {
                battle_damage_player(i, DAMAGE_MINE, mine->owner);
            }
        }
    }

    /* Remove mine */
    mine->active = 0;
    gBattle.num_mines--;

    sound_play(SFX_CRASH_HEAVY);
}

/*
 * ==========================================================================
 * Collision Detection Functions
 * ==========================================================================
 */

/**
 * battle_check_projectile_hits - Check projectile-player collisions
 */
void battle_check_projectile_hits(void) {
    s32 i, j;
    BattleProjectile *proj;
    f32 dx, dy, dz, dist;

    for (i = 0; i < BATTLE_MAX_PROJECTILES; i++) {
        proj = &gBattle.projectiles[i];

        if (!proj->active) {
            continue;
        }

        for (j = 0; j < BATTLE_MAX_PLAYERS; j++) {
            if (j == proj->owner || !gBattle.players[j].alive) {
                continue;
            }

            /* Distance check */
            dx = car_array[j].dr_pos[0] - proj->pos[0];
            dy = car_array[j].dr_pos[1] - proj->pos[1];
            dz = car_array[j].dr_pos[2] - proj->pos[2];
            dist = sqrtf(dx*dx + dy*dy + dz*dz);

            if (dist < PROJECTILE_HIT_RADIUS) {
                battle_damage_player(j, sWeaponProps[proj->type].damage, proj->owner);
                battle_destroy_projectile(i);
                break;
            }
        }
    }
}

/**
 * battle_check_mine_triggers - Check if mines should explode
 */
void battle_check_mine_triggers(void) {
    s32 i, j;
    BattleMine *mine;
    f32 dx, dz, dist;

    for (i = 0; i < BATTLE_MAX_MINES * BATTLE_MAX_PLAYERS; i++) {
        mine = &gBattle.mines[i];

        if (!mine->active || !mine->armed) {
            continue;
        }

        for (j = 0; j < BATTLE_MAX_PLAYERS; j++) {
            if (!gBattle.players[j].alive) {
                continue;
            }

            dx = car_array[j].dr_pos[0] - mine->pos[0];
            dz = car_array[j].dr_pos[2] - mine->pos[2];
            dist = sqrtf(dx*dx + dz*dz);

            if (dist < MINE_TRIGGER_RADIUS) {
                battle_trigger_mine(i);
                break;
            }
        }
    }
}

/**
 * battle_check_pickup_collection - Check pickup-player collisions
 */
void battle_check_pickup_collection(void) {
    s32 i, j;
    BattlePickup *pickup;
    f32 dx, dz, dist;

    for (i = 0; i < BATTLE_MAX_PICKUPS; i++) {
        pickup = &gBattle.pickups[i];

        if (!pickup->active) {
            continue;
        }

        for (j = 0; j < BATTLE_MAX_PLAYERS; j++) {
            if (!gBattle.players[j].alive) {
                continue;
            }

            dx = car_array[j].dr_pos[0] - pickup->pos[0];
            dz = car_array[j].dr_pos[2] - pickup->pos[2];
            dist = sqrtf(dx*dx + dz*dz);

            if (dist < PICKUP_COLLECT_RADIUS) {
                battle_collect_pickup(j, i);
                break;
            }
        }
    }
}

/**
 * battle_check_player_collisions - Check player-to-player collisions
 */
void battle_check_player_collisions(void) {
    s32 i, j;
    f32 dx, dz, dist;
    f32 speed_i, speed_j;
    s32 damage;

    for (i = 0; i < BATTLE_MAX_PLAYERS; i++) {
        if (!gBattle.players[i].alive) {
            continue;
        }

        for (j = i + 1; j < BATTLE_MAX_PLAYERS; j++) {
            if (!gBattle.players[j].alive) {
                continue;
            }

            dx = car_array[j].dr_pos[0] - car_array[i].dr_pos[0];
            dz = car_array[j].dr_pos[2] - car_array[i].dr_pos[2];
            dist = sqrtf(dx*dx + dz*dz);

            if (dist < PLAYER_COLLISION_RADIUS) {
                /* Collision detected */
                speed_i = car_array[i].mph;
                speed_j = car_array[j].mph;

                /* Damage based on relative speed */
                damage = (s32)((ABS(speed_i - speed_j)) / 10.0f);

                if (gBattle.players[i].boost_timer > 0) {
                    damage = DAMAGE_RAM;
                    battle_damage_player(j, damage, i);
                } else if (gBattle.players[j].boost_timer > 0) {
                    damage = DAMAGE_RAM;
                    battle_damage_player(i, damage, j);
                } else if (damage > DAMAGE_COLLISION) {
                    battle_damage_player(i, damage / 2, j);
                    battle_damage_player(j, damage / 2, i);
                }
            }
        }
    }
}

/*
 * ==========================================================================
 * Query Functions
 * ==========================================================================
 */

/**
 * battle_is_active - Check if battle is in progress
 */
s32 battle_is_active(void) {
    return gBattle.state == BATTLE_STATE_ACTIVE || gBattle.state == BATTLE_STATE_OVERTIME;
}

/**
 * battle_player_alive - Check if specific player is alive
 */
s32 battle_player_alive(s32 player) {
    if (player < 0 || player >= BATTLE_MAX_PLAYERS) {
        return 0;
    }
    return gBattle.players[player].alive;
}

/**
 * battle_get_leader - Get player with most kills
 */
s32 battle_get_leader(void) {
    s32 i;
    s32 best;
    s32 best_kills;

    best = 0;
    best_kills = -1;

    for (i = 0; i < BATTLE_MAX_PLAYERS; i++) {
        if (gBattle.players[i].active && gBattle.players[i].kills > best_kills) {
            best_kills = gBattle.players[i].kills;
            best = i;
        }
    }

    return best;
}

/**
 * battle_get_kills - Get player's kill count
 */
s32 battle_get_kills(s32 player) {
    if (player < 0 || player >= BATTLE_MAX_PLAYERS) {
        return 0;
    }
    return gBattle.players[player].kills;
}

/**
 * battle_get_deaths - Get player's death count
 */
s32 battle_get_deaths(s32 player) {
    if (player < 0 || player >= BATTLE_MAX_PLAYERS) {
        return 0;
    }
    return gBattle.players[player].deaths;
}

/**
 * battle_get_health - Get player's current health
 */
s32 battle_get_health(s32 player) {
    if (player < 0 || player >= BATTLE_MAX_PLAYERS) {
        return 0;
    }
    return gBattle.players[player].health;
}

/**
 * battle_get_weapon - Get player's current weapon
 */
s32 battle_get_weapon(s32 player) {
    if (player < 0 || player >= BATTLE_MAX_PLAYERS) {
        return WEAPON_NONE;
    }
    return gBattle.players[player].weapon;
}

/*
 * ==========================================================================
 * Match Flow Functions
 * ==========================================================================
 */

/**
 * battle_check_win_condition - Check if match should end
 */
void battle_check_win_condition(void) {
    s32 i;
    s32 alive_count;
    s32 last_alive;
    s32 best;
    s32 best_kills;
    s32 tie;

    alive_count = 0;
    last_alive = -1;

    /* Check if anyone reached kill limit */
    for (i = 0; i < BATTLE_MAX_PLAYERS; i++) {
        if (gBattle.players[i].active) {
            if (gBattle.players[i].kills >= gBattle.kill_limit) {
                battle_declare_winner(i);
                return;
            }
        }
    }

    /* Check time limit */
    if (gBattle.time_remaining == 0) {
        best = battle_get_leader();
        best_kills = gBattle.players[best].kills;
        tie = 0;

        for (i = 0; i < BATTLE_MAX_PLAYERS; i++) {
            if (i != best && gBattle.players[i].active &&
                gBattle.players[i].kills == best_kills) {
                tie = 1;
                break;
            }
        }

        if (tie) {
            battle_enter_overtime();
        } else {
            battle_declare_winner(best);
        }
        return;
    }

    /* Check elimination mode (stock limit) */
    if (gBattle.stock_limit > 0) {
        for (i = 0; i < BATTLE_MAX_PLAYERS; i++) {
            if (gBattle.players[i].active && gBattle.players[i].alive) {
                alive_count++;
                last_alive = i;
            }
        }

        if (alive_count <= 1) {
            battle_declare_winner(last_alive);
        }
    }
}

/**
 * battle_enter_overtime - Enter sudden death overtime
 */
void battle_enter_overtime(void) {
    gBattle.state = BATTLE_STATE_OVERTIME;
    /* First kill wins in overtime */
}

/**
 * battle_declare_winner - End match with winner
 */
void battle_declare_winner(s32 player) {
    gBattle.winner = player;
    gBattle.state = BATTLE_STATE_FINISHED;

    sound_play(SFX_RACE_FINISH);
}

/*
 * ==========================================================================
 * Arena Functions
 * ==========================================================================
 */

/**
 * battle_load_arena - Load arena and spawn pickups
 */
void battle_load_arena(s32 arena_id) {
    s32 i;
    f32 pos[3];

    gBattle.arena_id = (u8)arena_id;

    /* Spawn some initial pickups */
    for (i = 0; i < 8; i++) {
        pos[0] = (f32)((i * 50) - 175);
        pos[1] = 0.0f;
        pos[2] = (f32)(((i / 4) * 100) - 50);

        battle_spawn_pickup(i, pos, (i % 2) ? PICKUP_WEAPON : PICKUP_HEALTH);
    }
}

/**
 * battle_get_spawn_point - Get spawn location for player
 */
void battle_get_spawn_point(s32 player, f32 *pos) {
    s32 arena;

    arena = gBattle.arena_id;

    if (arena < 0 || arena > 3) {
        arena = 0;
    }
    if (player < 0 || player >= 4) {
        player = 0;
    }

    pos[0] = sSpawnPoints[arena][player][0];
    pos[1] = sSpawnPoints[arena][player][1];
    pos[2] = sSpawnPoints[arena][player][2];
}

/*
 * ==========================================================================
 * Display Functions (Stubs - to be implemented with RDP)
 * ==========================================================================
 */

/**
 * battle_draw_hud - Draw battle mode HUD
 */
void battle_draw_hud(void) {
    /* TODO: Draw health bars, weapon icons, scores, timer */
}

/**
 * battle_draw_player_hud - Draw HUD for specific player
 */
void battle_draw_player_hud(s32 player) {
    /* TODO: Draw player-specific HUD in split-screen */
}

/**
 * battle_draw_weapons - Draw weapon indicators
 */
void battle_draw_weapons(void) {
    /* TODO: Draw weapon ammo and cooldowns */
}

/**
 * battle_draw_pickups - Draw pickup locations
 */
void battle_draw_pickups(void) {
    /* TODO: Draw pickup locations on radar/map */
}

/**
 * battle_draw_projectiles - Draw active projectiles
 */
void battle_draw_projectiles(void) {
    /* TODO: Draw missiles and other projectiles */
}

/**
 * battle_draw_results - Draw end-of-match results
 */
void battle_draw_results(void) {
    /* TODO: Draw final scores, winner announcement */
}

#else /* !NON_MATCHING */

/*
 * Matching build - empty stubs, actual code is in assembly
 * TODO: Disassemble and match actual N64 battle mode functions
 */

BattleState gBattle;

void battle_init(void) {}
void battle_reset(void) {}
void battle_start(s32 arena_id, s32 num_players) {}
void battle_end(void) {}
void battle_update(void) {}
void battle_update_players(void) {}
void battle_update_projectiles(void) {}
void battle_update_mines(void) {}
void battle_update_pickups(void) {}
void battle_fire_weapon(s32 player) {}
void battle_use_boost(s32 player) {}
void battle_drop_mine(s32 player) {}
void battle_damage_player(s32 player, s32 damage, s32 attacker) {}
void battle_heal_player(s32 player, s32 amount) {}
void battle_kill_player(s32 player, s32 killer) {}
void battle_respawn_player(s32 player) {}
void battle_give_weapon(s32 player, s32 weapon_type) {}
void battle_collect_pickup(s32 player, s32 pickup_index) {}
void battle_spawn_pickup(s32 index, f32 pos[3], s32 type) {}
void battle_respawn_pickups(void) {}
s32 battle_spawn_projectile(s32 owner, s32 type, f32 pos[3], f32 dir[3]) { return -1; }
void battle_update_projectile(s32 index) {}
void battle_destroy_projectile(s32 index) {}
s32 battle_place_mine(s32 owner, f32 pos[3]) { return -1; }
void battle_update_mine(s32 index) {}
void battle_trigger_mine(s32 index) {}
void battle_check_projectile_hits(void) {}
void battle_check_mine_triggers(void) {}
void battle_check_pickup_collection(void) {}
void battle_check_player_collisions(void) {}
s32 battle_is_active(void) { return 0; }
s32 battle_player_alive(s32 player) { return 0; }
s32 battle_get_leader(void) { return 0; }
s32 battle_get_kills(s32 player) { return 0; }
s32 battle_get_deaths(s32 player) { return 0; }
s32 battle_get_health(s32 player) { return 0; }
s32 battle_get_weapon(s32 player) { return 0; }
void battle_check_win_condition(void) {}
void battle_enter_overtime(void) {}
void battle_declare_winner(s32 player) {}
void battle_load_arena(s32 arena_id) {}
void battle_get_spawn_point(s32 player, f32 *pos) {}
void battle_draw_hud(void) {}
void battle_draw_player_hud(s32 player) {}
void battle_draw_weapons(void) {}
void battle_draw_pickups(void) {}
void battle_draw_projectiles(void) {}
void battle_draw_results(void) {}

#endif /* NON_MATCHING */
