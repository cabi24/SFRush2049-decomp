# Rush 2049 N64 Battle Mode System

This document describes the battle mode system for San Francisco Rush 2049 on the N64, as discovered through analysis of the decompiled source code in `src/game/game.c` and the header files in `include/game/battle.h`.

## Overview

Battle mode is an arena-based vehicular combat mode where 2-4 players collect weapons and fight to eliminate opponents. Unlike the arcade "Rush The Rock" source (which focuses on racing), the N64 version of Rush 2049 includes a full battle mode system unique to the console release.

## Game Modes

Battle mode supports three sub-modes, selectable from the mode select screen:

| Mode | Description | Win Condition |
|------|-------------|---------------|
| **Deathmatch** | Free-for-all combat | First to reach frag limit or highest kills when time expires |
| **Team Battle** | 2v2 team combat | Team with most combined kills wins |
| **Last Man Standing** | Stock-based survival | Last player with remaining lives wins |

### Mode Selection Code
From `src/game/game.c` (line 36820-36823):
```c
case 1:  /* Battle */
    draw_text("DEATHMATCH", 110, 80, selection == 0 ? 255 : 150);
    draw_text("TEAM BATTLE", 110, 110, selection == 1 ? 255 : 150);
    draw_text("LAST MAN", 115, 140, selection == 2 ? 255 : 150);
```

## Battle Arenas

The game includes 4 battle arenas, each with a unique ID starting at 200:

| Arena ID | Name | Description |
|----------|------|-------------|
| 200 | WAREHOUSE | Industrial warehouse setting |
| 201 | ROOFTOPS | Urban rooftop environment |
| 202 | DOCKS | Waterfront dock area |
| 203 | FACTORY | Manufacturing plant setting |

Arena definitions from `src/game/game.c` (lines 34982-34985):
```c
arenaNames[0] = "WAREHOUSE";
arenaNames[1] = "ROOFTOPS";
arenaNames[2] = "DOCKS";
arenaNames[3] = "FACTORY";
```

The arena ID is calculated as `selectedArena + 200` when transitioning to gameplay (line 35057).

## Weapon/Pickup System

### Weapon Types

Defined in `include/game/battle.h`:

| Type ID | Weapon | Damage | Description |
|---------|--------|--------|-------------|
| 0 | Missile | 40 | Homing projectile that tracks nearest enemy |
| 1 | Machine Gun | - | Rapid-fire bullets (reduced damage per hit) |
| 2 | Mines | 50 | Dropped behind vehicle, explodes on contact |
| 3 | Shockwave | 30 | Area-effect damage around vehicle |
| 4 | Speed Boost | - | Temporary 1.5x speed multiplier for ramming |
| 5 | Shield | - | 5-second invincibility (300 frames) |

### Pickup Types

| Type ID | Pickup | Effect |
|---------|--------|--------|
| 0 | Weapon | Grants random weapon |
| 1 | Health | Restores health points |
| 2 | Speed | Temporary speed boost |
| 3 | Armor | Reduces incoming damage |

### Weapon Firing Implementation

From `src/game/game.c` (lines 42552-42568):
```c
switch (weaponType) {
    case 0:  /* Missile */
        smoke_spawn(pos, forward, playerIdx);  /* Spawn missile projectile */
        break;
    case 1:  /* Machine gun */
        skid_mark_update(pos, forward, playerIdx);  /* Spawn bullet */
        break;
    case 2:  /* Mine */
        skid_mark_spawn(pos, playerIdx);  /* Drop mine */
        break;
    case 3:  /* Shield */
        *(s32 *)((u8 *)car + 0x1F4) = 300;  /* 5 second shield */
        break;
    case 4:  /* Speed boost */
        *(f32 *)((u8 *)car + 0x200) = 1.5f;  /* Temporary speed boost */
        break;
}
```

### Pickup Constants

From `include/game/battle.h`:
- `BATTLE_MAX_PICKUPS`: 32 pickups per arena
- `BATTLE_PICKUP_RESPAWN`: 300 frames (5 seconds) respawn time

## Scoring System

### Frag Limits

The battle setup menu allows configurable frag limits:

| Option | Value |
|--------|-------|
| 1 | 5 kills |
| 2 | 10 kills |
| 3 | 15 kills |
| 4 | 20 kills |
| 5 | Unlimited (no limit) |

### Score Tracking

Player scores are tracked in the `player_battle_scores[4]` array (line 447 in game.c).

Kill attribution uses proximity-based detection - when a player is destroyed, the closest enemy player receives credit for the kill (lines 42243-42264):
```c
/* Find attacker and award point */
f32 closestDist = 999999.0f;
s32 closestPlayer = -1;

for (j = 0; j < 4; j++) {
    if (j == i) continue;
    /* ... distance calculation ... */
    if (dist < closestDist) {
        closestDist = dist;
        closestPlayer = j;
    }
}

if (closestPlayer >= 0) {
    player_battle_scores[closestPlayer]++;
}
```

### Win Conditions

The `battle_win_check()` function (lines 42576-42603) evaluates:

1. **Score Limit**: First player to reach `battle_score_limit` wins
2. **Last Man Standing**: When only 1 player has lives remaining (`player_lives[i] > 0`)

```c
/* Check score limit */
for (i = 0; i < 4; i++) {
    if (player_battle_scores[i] >= battle_score_limit) {
        winner = i;
        battle_mode_state = 2;  /* End battle, show results */
        return;
    }
}

/* Check Last Man Standing */
if (battle_mode_type == 2) {
    for (i = 0; i < 4; i++) {
        if (player_lives[i] > 0) {
            alivePlayers++;
            winner = i;
        }
    }
    if (alivePlayers <= 1) {
        battle_mode_state = 2;  /* End battle */
    }
}
```

## Respawn Mechanics

### Respawn Process

When a player is destroyed:
1. Lives are decremented (`player_lives[i]--`)
2. Kill is attributed to nearest enemy
3. If lives remain (or not Last Man Standing mode), player respawns
4. Health resets to 100
5. Weapon and ammo reset to 0

### Respawn Implementation

From `src/game/game.c` (lines 42495-42525):
```c
void battle_respawn(void *car, s32 playerIdx) {
    extern f32 spawn_positions[4][3];  /* Spawn positions */
    extern f32 spawn_rotations[4];     /* Spawn rotations */

    /* Set spawn position */
    pos[0] = spawn_positions[playerIdx][0];
    pos[1] = spawn_positions[playerIdx][1];
    pos[2] = spawn_positions[playerIdx][2];

    /* Clear velocity */
    vel[0] = 0.0f;
    vel[1] = 0.0f;
    vel[2] = 0.0f;

    /* Set spawn rotation */
    rot[1] = spawn_rotations[playerIdx];

    /* Brief invincibility after spawn */
    *(s32 *)((u8 *)car + 0x1F4) = 180;  /* 3 seconds invincibility */
}
```

### Respawn Timing

- **Respawn delay**: 180 frames (3 seconds) - `BATTLE_RESPAWN_TIME` in battle.h
- **Spawn invincibility**: 180 frames (3 seconds) after respawning
- Each arena has 4 predefined spawn positions (`spawn_positions[4][3]`)

## Damage Model

### Health System

From `include/game/battle.h`:
- **Starting Health**: 100 (`BATTLE_MAX_HEALTH`)
- Health stored at car offset `0x1F0`
- Max health stored at offset `0x1F4`

### Damage Values

| Source | Damage |
|--------|--------|
| Missile | 40 |
| Mine | 50 |
| Shockwave | 30 |
| Boost Ram | 20 |
| Normal Collision | 10 |

### Damage States

The damage system uses visual states (from `entity_damage_update` lines 10454-10462):

| State | Condition | Visual |
|-------|-----------|--------|
| 0 - Healthy | Health > 50% | Normal appearance |
| 1 - Damaged | Health 25-50% | Minor damage effects |
| 2 - Critical | Health < 25% | Severe damage effects |
| 3 - Destroyed | Health = 0 | Explosion, respawn triggered |

### Invulnerability

- **Post-damage immunity**: 30 frames (0.5 seconds) at offset `0x108`
- **Post-spawn immunity**: 180 frames (3 seconds) at offset `0x1F4`
- **Shield powerup**: 300 frames (5 seconds)

## Battle Mode State Machine

### Battle States

From `include/game/battle.h`:

| State ID | Name | Description |
|----------|------|-------------|
| 0 | INACTIVE | Battle mode not running |
| 1 | WAITING | Waiting for players to join |
| 2 | COUNTDOWN | Pre-battle countdown |
| 3 | ACTIVE | Battle in progress |
| 4 | OVERTIME | Tie-breaker period |
| 5 | FINISHED | Battle complete, showing results |

### Main Update Loop

The `battle_mode_update()` function (lines 42196-42290) runs each frame:

1. Check if battle mode is active
2. Increment battle timer
3. For each player:
   - Skip eliminated players in Last Man Standing
   - Check health status
   - Handle destruction (award kill, respawn)
   - Check weapon pickup collisions
   - Process weapon firing
4. Check win conditions

## Time Limits

Configurable time limits from the battle setup menu:

| Option | Duration |
|--------|----------|
| 1 | 3 minutes (180 seconds) |
| 2 | 5 minutes (300 seconds) |
| 3 | 10 minutes (600 seconds) |
| 4 | Unlimited |

Timer stored in `battle_time_limit` (line 319).

## Data Structures

### Per-Player Battle State

From `include/game/battle.h` (BattlePlayer struct):

```c
typedef struct BattlePlayer {
    u8      active;             /* Player participating? */
    u8      alive;              /* Currently alive? */
    u8      weapon;             /* Current weapon held */
    u8      ammo;               /* Weapon ammo count */

    s32     health;             /* Current health */
    s32     max_health;         /* Maximum health */
    s32     armor;              /* Damage reduction percentage */

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
```

### Projectile Structure

```c
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
```

### Mine Structure

```c
typedef struct BattleMine {
    u8      active;             /* Is mine placed? */
    u8      owner;              /* Player who placed it */
    u8      armed;              /* Is mine armed? */
    u8      pad;
    f32     pos[3];             /* Mine position */
    f32     arm_timer;          /* Time until armed */
    f32     lifetime;           /* Time until despawn */
} BattleMine;
```

## Arena Layout Notes

Arena data is stored in the ROM's track data section with IDs 200-203. The arena layouts include:

- **Spawn Points**: 4 predefined spawn positions per arena, stored in `spawn_positions[4][3]`
- **Pickup Locations**: Up to 32 pickup spawn points per arena
- **Arena Boundaries**: Enclosed arenas with collision walls

Specific arena geometry details would require further ROM analysis of the track data segments.

## Global Variables

Key battle mode globals from `src/game/game.c`:

| Variable | Type | Purpose |
|----------|------|---------|
| `battle_mode_enabled` | s32 | Battle mode flag |
| `battle_mode_state` | s32 | Current battle state |
| `battle_mode_type` | s32 | 0=Deathmatch, 1=Team, 2=Last Man |
| `battle_menu_option` | s32 | Menu cursor position |
| `battle_arena_selection` | s32 | Selected arena (0-3) |
| `battle_frag_limit` | s32 | Frag limit index |
| `battle_time_limit` | s32 | Time limit index |
| `battle_weapons_enabled` | s32 | Weapons toggle |
| `battle_timer_frames` | s32 | Current battle time in frames |
| `player_battle_scores[4]` | s32[] | Player kill counts |
| `player_lives[4]` | s32[] | Player remaining lives |
| `player_weapons[4]` | s32[] | Player current weapons |
| `player_ammo[4]` | s32[] | Player ammo counts |

## Related Files

| File | Purpose |
|------|---------|
| `src/game/game.c` | Main battle mode implementation |
| `include/game/battle.h` | Battle mode structures and constants |
| `include/game/car.h` | Vehicle physics and state |
| `include/game/game.h` | Game state definitions |

## Differences from Arcade

The arcade "Rush The Rock" source code in `reference/repos/rushtherock/` does not contain battle mode functionality - this is a console-exclusive feature added for the N64 version of Rush 2049. The battle mode shares the physics engine and car handling with race mode but adds:

- Health/damage system
- Weapon pickups and projectiles
- Arena maps (separate from race tracks)
- Kill tracking and scoring
- Respawn mechanics with invincibility

## Implementation Status

Based on the decompiled code analysis:

- **Core battle loop**: Implemented (battle_mode_update)
- **Respawn system**: Implemented (battle_respawn)
- **Weapon firing**: Implemented (battle_fire_weapon)
- **Win conditions**: Implemented (battle_win_check)
- **Pickup system**: Stub only (battle_pickup_check marked TODO)
- **HUD rendering**: References exist but implementations are stubs

The battle mode is functional but some helper functions remain as stubs requiring further decompilation work.
