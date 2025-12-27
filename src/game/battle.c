/**
 * battle.c - Battle mode system for Rush 2049 N64
 *
 * Battle mode is an arena combat mode where players
 * collect weapons and fight to eliminate opponents.
 */

#include "types.h"
#include "game/battle.h"
#include "game/structs.h"
#include "game/sound.h"

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

/* Weapon properties */
static const struct {
    s32 damage;
    f32 speed;
    f32 lifetime;
    f32 turn_rate;
    s32 ammo;
} sWeaponProps[] = {
    { 0, 0.0f, 0.0f, 0.0f, 0 },              /* NONE */
    { DAMAGE_MISSILE, 80.0f, 5.0f, 3.0f, 3 }, /* MISSILE */
    { DAMAGE_MINE, 0.0f, 30.0f, 0.0f, 2 },    /* MINE */
    { DAMAGE_SHOCKWAVE, 0.0f, 0.5f, 0.0f, 1 }, /* SHOCKWAVE */
    { DAMAGE_RAM, 0.0f, 3.0f, 0.0f, 5 },      /* BOOST */
    { 0, 0.0f, 5.0f, 0.0f, 1 }                /* SHIELD */
};

#define ABS(x) (((x) < 0) ? -(x) : (x))

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

    gBattle.time_limit = 60 * 60 * 3;  /* 3 minutes */
    gBattle.time_remaining = gBattle.time_limit;
    gBattle.kill_limit = 10;
    gBattle.stock_limit = 0;

    /* Reset players */
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
 *
 * @param arena_id Arena to use
 * @param num_players Number of players
 */
void battle_start(s32 arena_id, s32 num_players) {
    s32 i;

    battle_reset();

    gBattle.arena_id = (u8)arena_id;
    gBattle.num_players = (u8)num_players;
    gBattle.state = BATTLE_STATE_COUNTDOWN;
    gBattle.time_remaining = gBattle.time_limit;

    /* Activate players */
    for (i = 0; i < num_players && i < BATTLE_MAX_PLAYERS; i++) {
        gBattle.players[i].active = 1;
        gBattle.players[i].alive = 1;
        gBattle.players[i].health = BATTLE_MAX_HEALTH;
        gBattle.players[i].weapon = WEAPON_NONE;
        gBattle.players[i].ammo = 0;

        /* Set spawn position */
        battle_get_spawn_point(i, gBattle.players[i].spawn_pos);
    }

    /* Load arena and spawn pickups */
    battle_load_arena(arena_id);
}

/**
 * battle_end - End current battle
 */
void battle_end(void) {
    gBattle.state = BATTLE_STATE_FINISHED;
}

/**
 * battle_update - Main battle update
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
            /* Pre-battle countdown */
            gBattle.match_time++;
            if (gBattle.match_time >= 180) {  /* 3 seconds */
                gBattle.state = BATTLE_STATE_ACTIVE;
                gBattle.match_time = 0;
            }
            break;

        case BATTLE_STATE_ACTIVE:
            /* Main battle logic */
            gBattle.match_time++;

            /* Update timer */
            if (gBattle.time_remaining > 0) {
                gBattle.time_remaining--;
            }

            /* Update all systems */
            battle_update_players();
            battle_update_projectiles();
            battle_update_mines();
            battle_update_pickups();

            /* Check collisions */
            battle_check_projectile_hits();
            battle_check_mine_triggers();
            battle_check_pickup_collection();
            battle_check_player_collisions();

            /* Check win condition */
            battle_check_win_condition();
            break;

        case BATTLE_STATE_OVERTIME:
            /* Sudden death */
            gBattle.match_time++;
            battle_update_players();
            battle_update_projectiles();
            battle_check_projectile_hits();
            battle_check_win_condition();
            break;

        case BATTLE_STATE_FINISHED:
            /* Show results */
            break;
    }
}

/**
 * battle_update_players - Update all players
 */
void battle_update_players(void) {
    s32 i;
    BattlePlayer *p;

    for (i = 0; i < BATTLE_MAX_PLAYERS; i++) {
        p = &gBattle.players[i];

        if (!p->active) {
            continue;
        }

        /* Update respawn timer */
        if (!p->alive && p->respawn_timer > 0) {
            p->respawn_timer--;
            if (p->respawn_timer == 0) {
                battle_respawn_player(i);
            }
        }

        /* Update temporary effects */
        if (p->shield_timer > 0) {
            p->shield_timer--;
        }
        if (p->boost_timer > 0) {
            p->boost_timer--;
        }
    }
}

/**
 * battle_update_projectiles - Update all projectiles
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
 * battle_update_mines - Update all mines
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
 * battle_update_pickups - Update all pickups
 */
void battle_update_pickups(void) {
    s32 i;
    BattlePickup *pickup;

    for (i = 0; i < BATTLE_MAX_PICKUPS; i++) {
        pickup = &gBattle.pickups[i];

        if (pickup->respawning) {
            pickup->respawn_timer--;
            if (pickup->respawn_timer == 0) {
                pickup->respawning = 0;
                pickup->active = 1;
            }
        }

        if (pickup->active) {
            /* Rotate pickup for visual effect */
            pickup->rotation += 2.0f;
            if (pickup->rotation >= 360.0f) {
                pickup->rotation -= 360.0f;
            }
        }
    }
}

/**
 * battle_fire_weapon - Fire current weapon
 *
 * @param player Player index
 */
void battle_fire_weapon(s32 player) {
    BattlePlayer *p;
    CarData *car;
    f32 pos[3], dir[3];

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
    dir[2] = 1.0f;  /* Would use car's forward vector */

    switch (p->weapon) {
        case WEAPON_MISSILE:
            battle_spawn_projectile(player, WEAPON_MISSILE, pos, dir);
            break;

        case WEAPON_MINE:
            battle_place_mine(player, car->dr_pos);
            break;

        case WEAPON_SHOCKWAVE:
            /* Damage all nearby players */
            {
                s32 i;
                for (i = 0; i < BATTLE_MAX_PLAYERS; i++) {
                    if (i != player && gBattle.players[i].alive) {
                        f32 dx = car_array[i].dr_pos[0] - car->dr_pos[0];
                        f32 dz = car_array[i].dr_pos[2] - car->dr_pos[2];
                        f32 dist = sqrtf(dx*dx + dz*dz);
                        if (dist < 30.0f) {
                            battle_damage_player(i, DAMAGE_SHOCKWAVE, player);
                        }
                    }
                }
            }
            break;

        case WEAPON_BOOST:
            /* Activate speed boost */
            p->boost_timer = 180;  /* 3 seconds */
            break;

        case WEAPON_SHIELD:
            /* Activate shield */
            p->shield_timer = 300;  /* 5 seconds */
            break;
    }

    p->ammo--;
    if (p->ammo <= 0) {
        p->weapon = WEAPON_NONE;
    }

    sound_play(SFX_BOOST);  /* Placeholder */
}

/**
 * battle_use_boost - Use boost weapon
 *
 * @param player Player index
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
 * battle_drop_mine - Drop a mine
 *
 * @param player Player index
 */
void battle_drop_mine(s32 player) {
    if (player < 0 || player >= BATTLE_MAX_PLAYERS) {
        return;
    }

    if (gBattle.players[player].weapon == WEAPON_MINE) {
        battle_fire_weapon(player);
    }
}

/**
 * battle_damage_player - Apply damage to a player
 *
 * @param player Player to damage
 * @param damage Damage amount
 * @param attacker Attacking player (-1 for environment)
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

    /* Check for shield */
    if (p->shield_timer > 0) {
        return;  /* No damage while shielded */
    }

    /* Apply armor reduction */
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
 *
 * @param player Player to heal
 * @param amount Health amount
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
 * battle_kill_player - Kill a player
 *
 * @param player Player who died
 * @param killer Player who got the kill (-1 for self/environment)
 */
void battle_kill_player(s32 player, s32 killer) {
    BattlePlayer *p;

    p = &gBattle.players[player];
    p->alive = 0;
    p->deaths++;
    p->respawn_timer = BATTLE_RESPAWN_TIME;

    /* Award kill */
    if (killer >= 0 && killer < BATTLE_MAX_PLAYERS && killer != player) {
        gBattle.players[killer].kills++;
    } else {
        p->self_destructs++;
    }

    /* Drop weapon */
    p->weapon = WEAPON_NONE;
    p->ammo = 0;

    sound_play(SFX_CRASH_HEAVY);
}

/**
 * battle_respawn_player - Respawn a player
 *
 * @param player Player to respawn
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

    /* Move car to spawn point */
    car->dr_pos[0] = p->spawn_pos[0];
    car->dr_pos[1] = p->spawn_pos[1];
    car->dr_pos[2] = p->spawn_pos[2];
    car->mph = 0.0f;  /* Zero out speed */
}

/**
 * battle_give_weapon - Give weapon to player
 *
 * @param player Player index
 * @param weapon_type Weapon type
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
 * battle_collect_pickup - Collect a pickup
 *
 * @param player Player index
 * @param pickup_index Pickup index
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
            battle_heal_player(player, 50);
            break;

        case PICKUP_SPEED:
            p->boost_timer = 180;
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
 * battle_spawn_pickup - Spawn a pickup
 *
 * @param index Pickup slot index
 * @param pos Position
 * @param type Pickup type
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

    /* Random weapon for weapon pickups */
    if (type == PICKUP_WEAPON) {
        pickup->weapon = (u8)(1 + (frame_counter % 5));  /* Random 1-5 */
    }

    gBattle.num_pickups++;
}

/**
 * battle_respawn_pickups - Respawn all pickups
 */
void battle_respawn_pickups(void) {
    s32 i;

    for (i = 0; i < gBattle.num_pickups; i++) {
        if (!gBattle.pickups[i].active && !gBattle.pickups[i].respawning) {
            gBattle.pickups[i].active = 1;
        }
    }
}

/**
 * battle_spawn_projectile - Spawn a projectile
 *
 * @param owner Owner player
 * @param type Projectile type
 * @param pos Start position
 * @param dir Direction
 * @return Projectile index or -1
 */
s32 battle_spawn_projectile(s32 owner, s32 type, f32 pos[3], f32 dir[3]) {
    s32 i;
    BattleProjectile *proj;

    /* Find free slot */
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

            /* Find homing target */
            if (type == WEAPON_MISSILE) {
                f32 best_dist = 1000.0f;
                s32 best_target = -1;
                s32 j;

                for (j = 0; j < BATTLE_MAX_PLAYERS; j++) {
                    if (j != owner && gBattle.players[j].alive) {
                        f32 dx = car_array[j].dr_pos[0] - pos[0];
                        f32 dz = car_array[j].dr_pos[2] - pos[2];
                        f32 dist = sqrtf(dx*dx + dz*dz);
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
 * battle_update_projectile - Update one projectile
 *
 * @param index Projectile index
 */
void battle_update_projectile(s32 index) {
    BattleProjectile *proj;
    f32 dt = 1.0f / 60.0f;

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

    /* Homing behavior */
    if (proj->target < BATTLE_MAX_PLAYERS && gBattle.players[proj->target].alive) {
        f32 target_pos[3];
        f32 to_target[3];
        f32 len;

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

            /* Renormalize */
            len = sqrtf(proj->dir[0]*proj->dir[0] + proj->dir[1]*proj->dir[1] + proj->dir[2]*proj->dir[2]);
            if (len > 0.001f) {
                proj->dir[0] /= len;
                proj->dir[1] /= len;
                proj->dir[2] /= len;
            }
        }
    }

    /* Update velocity */
    proj->vel[0] = proj->dir[0] * proj->speed;
    proj->vel[1] = proj->dir[1] * proj->speed;
    proj->vel[2] = proj->dir[2] * proj->speed;

    /* Update position */
    proj->pos[0] += proj->vel[0] * dt;
    proj->pos[1] += proj->vel[1] * dt;
    proj->pos[2] += proj->vel[2] * dt;
}

/**
 * battle_destroy_projectile - Destroy a projectile
 *
 * @param index Projectile index
 */
void battle_destroy_projectile(s32 index) {
    if (index >= 0 && index < BATTLE_MAX_PROJECTILES) {
        gBattle.projectiles[index].active = 0;
        gBattle.num_projectiles--;
    }
}

/**
 * battle_place_mine - Place a mine
 *
 * @param owner Owner player
 * @param pos Position
 * @return Mine index or -1
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
            mine->arm_timer = 1.0f;  /* 1 second to arm */
            mine->lifetime = 30.0f;  /* 30 seconds lifetime */

            gBattle.num_mines++;
            return i;
        }
    }

    return -1;
}

/**
 * battle_update_mine - Update one mine
 *
 * @param index Mine index
 */
void battle_update_mine(s32 index) {
    BattleMine *mine;
    f32 dt = 1.0f / 60.0f;

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

    /* Lifetime */
    mine->lifetime -= dt;
    if (mine->lifetime <= 0.0f) {
        mine->active = 0;
        gBattle.num_mines--;
    }
}

/**
 * battle_trigger_mine - Trigger a mine explosion
 *
 * @param index Mine index
 */
void battle_trigger_mine(s32 index) {
    BattleMine *mine;
    s32 i;

    mine = &gBattle.mines[index];

    if (!mine->active || !mine->armed) {
        return;
    }

    /* Damage nearby players */
    for (i = 0; i < BATTLE_MAX_PLAYERS; i++) {
        if (gBattle.players[i].alive) {
            f32 dx = car_array[i].dr_pos[0] - mine->pos[0];
            f32 dz = car_array[i].dr_pos[2] - mine->pos[2];
            f32 dist = sqrtf(dx*dx + dz*dz);

            if (dist < 15.0f) {
                battle_damage_player(i, DAMAGE_MINE, mine->owner);
            }
        }
    }

    /* Destroy mine */
    mine->active = 0;
    gBattle.num_mines--;

    sound_play(SFX_CRASH_HEAVY);
}

/**
 * battle_check_projectile_hits - Check projectile collisions
 */
void battle_check_projectile_hits(void) {
    s32 i, j;
    BattleProjectile *proj;

    for (i = 0; i < BATTLE_MAX_PROJECTILES; i++) {
        proj = &gBattle.projectiles[i];

        if (!proj->active) {
            continue;
        }

        for (j = 0; j < BATTLE_MAX_PLAYERS; j++) {
            if (j == proj->owner || !gBattle.players[j].alive) {
                continue;
            }

            /* Check distance */
            {
                f32 dx = car_array[j].dr_pos[0] - proj->pos[0];
                f32 dy = car_array[j].dr_pos[1] - proj->pos[1];
                f32 dz = car_array[j].dr_pos[2] - proj->pos[2];
                f32 dist = sqrtf(dx*dx + dy*dy + dz*dz);

                if (dist < 5.0f) {  /* Hit radius */
                    battle_damage_player(j, sWeaponProps[proj->type].damage, proj->owner);
                    battle_destroy_projectile(i);
                    break;
                }
            }
        }
    }
}

/**
 * battle_check_mine_triggers - Check if mines should trigger
 */
void battle_check_mine_triggers(void) {
    s32 i, j;
    BattleMine *mine;

    for (i = 0; i < BATTLE_MAX_MINES * BATTLE_MAX_PLAYERS; i++) {
        mine = &gBattle.mines[i];

        if (!mine->active || !mine->armed) {
            continue;
        }

        for (j = 0; j < BATTLE_MAX_PLAYERS; j++) {
            if (!gBattle.players[j].alive) {
                continue;
            }

            /* Check distance */
            {
                f32 dx = car_array[j].dr_pos[0] - mine->pos[0];
                f32 dz = car_array[j].dr_pos[2] - mine->pos[2];
                f32 dist = sqrtf(dx*dx + dz*dz);

                if (dist < 8.0f) {  /* Trigger radius */
                    battle_trigger_mine(i);
                    break;
                }
            }
        }
    }
}

/**
 * battle_check_pickup_collection - Check pickup collisions
 */
void battle_check_pickup_collection(void) {
    s32 i, j;
    BattlePickup *pickup;

    for (i = 0; i < BATTLE_MAX_PICKUPS; i++) {
        pickup = &gBattle.pickups[i];

        if (!pickup->active) {
            continue;
        }

        for (j = 0; j < BATTLE_MAX_PLAYERS; j++) {
            if (!gBattle.players[j].alive) {
                continue;
            }

            /* Check distance */
            {
                f32 dx = car_array[j].dr_pos[0] - pickup->pos[0];
                f32 dz = car_array[j].dr_pos[2] - pickup->pos[2];
                f32 dist = sqrtf(dx*dx + dz*dz);

                if (dist < 5.0f) {
                    battle_collect_pickup(j, i);
                    break;
                }
            }
        }
    }
}

/**
 * battle_check_player_collisions - Check player-to-player collisions
 */
void battle_check_player_collisions(void) {
    s32 i, j;

    for (i = 0; i < BATTLE_MAX_PLAYERS; i++) {
        if (!gBattle.players[i].alive) {
            continue;
        }

        for (j = i + 1; j < BATTLE_MAX_PLAYERS; j++) {
            if (!gBattle.players[j].alive) {
                continue;
            }

            /* Check distance */
            {
                f32 dx = car_array[j].dr_pos[0] - car_array[i].dr_pos[0];
                f32 dz = car_array[j].dr_pos[2] - car_array[i].dr_pos[2];
                f32 dist = sqrtf(dx*dx + dz*dz);

                if (dist < 3.0f) {
                    /* Collision! */
                    f32 speed_i = car_array[i].mph;
                    f32 speed_j = car_array[j].mph;

                    /* Damage based on relative speed */
                    s32 damage = (s32)((ABS(speed_i - speed_j)) / 10.0f);

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
}

/**
 * battle_is_active - Check if battle is active
 *
 * @return 1 if active
 */
s32 battle_is_active(void) {
    return gBattle.state == BATTLE_STATE_ACTIVE || gBattle.state == BATTLE_STATE_OVERTIME;
}

/**
 * battle_player_alive - Check if player is alive
 *
 * @param player Player index
 * @return 1 if alive
 */
s32 battle_player_alive(s32 player) {
    if (player < 0 || player >= BATTLE_MAX_PLAYERS) {
        return 0;
    }
    return gBattle.players[player].alive;
}

/**
 * battle_get_leader - Get current leader
 *
 * @return Player index of leader
 */
s32 battle_get_leader(void) {
    s32 i, best = 0;
    s32 best_kills = -1;

    for (i = 0; i < BATTLE_MAX_PLAYERS; i++) {
        if (gBattle.players[i].active && gBattle.players[i].kills > best_kills) {
            best_kills = gBattle.players[i].kills;
            best = i;
        }
    }

    return best;
}

/**
 * battle_get_kills - Get kill count
 *
 * @param player Player index
 * @return Kill count
 */
s32 battle_get_kills(s32 player) {
    if (player < 0 || player >= BATTLE_MAX_PLAYERS) {
        return 0;
    }
    return gBattle.players[player].kills;
}

/**
 * battle_get_deaths - Get death count
 *
 * @param player Player index
 * @return Death count
 */
s32 battle_get_deaths(s32 player) {
    if (player < 0 || player >= BATTLE_MAX_PLAYERS) {
        return 0;
    }
    return gBattle.players[player].deaths;
}

/**
 * battle_get_health - Get player health
 *
 * @param player Player index
 * @return Current health
 */
s32 battle_get_health(s32 player) {
    if (player < 0 || player >= BATTLE_MAX_PLAYERS) {
        return 0;
    }
    return gBattle.players[player].health;
}

/**
 * battle_get_weapon - Get current weapon
 *
 * @param player Player index
 * @return Weapon type
 */
s32 battle_get_weapon(s32 player) {
    if (player < 0 || player >= BATTLE_MAX_PLAYERS) {
        return WEAPON_NONE;
    }
    return gBattle.players[player].weapon;
}

/**
 * battle_check_win_condition - Check if someone won
 */
void battle_check_win_condition(void) {
    s32 i;
    s32 alive_count = 0;
    s32 last_alive = -1;

    /* Check kill limit */
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
        /* Find player with most kills */
        s32 best = battle_get_leader();
        s32 best_kills = gBattle.players[best].kills;
        s32 tie = 0;

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

    /* Check if only one player alive (elimination mode) */
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
 * battle_enter_overtime - Enter overtime mode
 */
void battle_enter_overtime(void) {
    gBattle.state = BATTLE_STATE_OVERTIME;
    /* First kill wins */
}

/**
 * battle_declare_winner - Declare battle winner
 *
 * @param player Winner player index
 */
void battle_declare_winner(s32 player) {
    gBattle.winner = player;
    gBattle.state = BATTLE_STATE_FINISHED;

    sound_play(SFX_RACE_FINISH);
}

/**
 * battle_load_arena - Load arena map
 *
 * @param arena_id Arena to load
 */
void battle_load_arena(s32 arena_id) {
    s32 i;
    f32 pos[3];

    gBattle.arena_id = (u8)arena_id;

    /* Spawn some pickups */
    for (i = 0; i < 8; i++) {
        pos[0] = (f32)((i * 50) - 175);
        pos[1] = 0.0f;
        pos[2] = (f32)(((i / 4) * 100) - 50);

        battle_spawn_pickup(i, pos, (i % 2) ? PICKUP_WEAPON : PICKUP_HEALTH);
    }
}

/**
 * battle_get_spawn_point - Get spawn point for player
 *
 * @param player Player index
 * @param pos Output position
 */
void battle_get_spawn_point(s32 player, f32 *pos) {
    s32 arena = gBattle.arena_id;

    if (arena < 0 || arena > 3) arena = 0;
    if (player < 0 || player >= 4) player = 0;

    pos[0] = sSpawnPoints[arena][player][0];
    pos[1] = sSpawnPoints[arena][player][1];
    pos[2] = sSpawnPoints[arena][player][2];
}

/**
 * battle_draw_hud - Draw battle HUD
 */
void battle_draw_hud(void) {
    /* Would draw health bars, weapon icons, scores, etc. */
}

/**
 * battle_draw_player_hud - Draw single player's HUD
 *
 * @param player Player index
 */
void battle_draw_player_hud(s32 player) {
    /* Would draw player-specific HUD elements */
}

/**
 * battle_draw_weapons - Draw weapon indicators
 */
void battle_draw_weapons(void) {
    /* Would draw weapon ammo, cooldowns, etc. */
}

/**
 * battle_draw_pickups - Draw pickup indicators
 */
void battle_draw_pickups(void) {
    /* Would draw pickup locations/status */
}

/**
 * battle_draw_projectiles - Draw active projectiles
 */
void battle_draw_projectiles(void) {
    /* Would draw missiles, etc. */
}

/**
 * battle_draw_results - Draw end-of-match results
 */
void battle_draw_results(void) {
    /* Would draw final scores, winner, etc. */
}
