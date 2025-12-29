# Battle Mode System - Rush 2049 N64

## Overview

Rush 2049 features a Battle Mode where up to 4 players engage in vehicular combat in enclosed arenas. This mode is unique to Rush 2049 (not present in Rush The Rock arcade source) and adds weapons, pickups, and deathmatch-style gameplay to the racing formula.

Battle Mode uses the same car physics as the racing modes but adds weapon systems, health/damage, respawning, and scoring mechanics.

## Game Modes

Battle Mode supports three sub-modes (configured in `battle_mode_type`):

| Mode | Value | Description |
|------|-------|-------------|
| **Deathmatch** | 0 | Most kills (frags) wins. Players respawn indefinitely. |
| **Team Battle** | 1 | Team-based deathmatch. Teams compete for most combined kills. |
| **Last Man Standing** | 2 | Limited lives. Last surviving player wins. |

### Mode Selection UI

From `src/game/game.c` lines 36761-36766:
```c
case 1:  /* Battle */
    draw_text(110, 30, "BATTLE MODE", 0xFFFFFFFF);
    draw_text("DEATHMATCH", 110, 80, selection == 0 ? 255 : 150);
    draw_text("TEAM BATTLE", 110, 110, selection == 1 ? 255 : 150);
    draw_text("LAST MAN", 115, 140, selection == 2 ? 255 : 150);
```

## Battle Arenas

Four dedicated battle arenas are available (arena IDs 200-203):

| Arena | ID | Description |
|-------|-----|-------------|
| **WAREHOUSE** | 200 | Industrial warehouse interior |
| **ROOFTOPS** | 201 | Urban rooftop arena |
| **DOCKS** | 202 | Waterfront dock area |
| **FACTORY** | 203 | Factory floor with machinery |

Arena selection code from `battle_mode_setup()`:
```c
arenaNames[0] = "WAREHOUSE";
arenaNames[1] = "ROOFTOPS";
arenaNames[2] = "DOCKS";
arenaNames[3] = "FACTORY";
// ...
trackno = selectedArena + 200;  /* Battle arena IDs */
```

## Weapon System

### Weapon Types

Five weapon/powerup types are available as pickups:

| ID | Type | Effect | Implementation |
|----|------|--------|----------------|
| 0 | **Missile** | Fires homing projectile at enemies | `smoke_spawn(pos, forward, playerIdx)` |
| 1 | **Machine Gun** | Rapid-fire bullets | `skid_mark_update(pos, forward, playerIdx)` |
| 2 | **Mine** | Drops proximity mine behind car | `skid_mark_spawn(pos, playerIdx)` |
| 3 | **Shield** | Temporary invincibility (5 seconds) | Sets car offset 0x1F4 to 300 frames |
| 4 | **Speed Boost** | Temporary speed increase | Sets car offset 0x200 to 1.5f multiplier |

### Weapon Data Structures

```c
extern s32 D_8014B02C[4];  /* Player weapons (current weapon type) */
extern s32 D_8014B03C[4];  /* Player ammo count */
```

### Firing Weapons

Weapon firing is handled in `battle_fire_weapon()`:
```c
void battle_fire_weapon(void *car, s32 weaponType, s32 playerIdx) {
    switch (weaponType) {
        case 0:  /* Missile */
            smoke_spawn(pos, forward, playerIdx);
            break;
        case 1:  /* Machine gun */
            skid_mark_update(pos, forward, playerIdx);
            break;
        case 2:  /* Mine */
            skid_mark_spawn(pos, playerIdx);
            break;
        case 3:  /* Shield */
            *(s32 *)((u8 *)car + 0x1F4) = 300;  /* 5 second shield */
            break;
        case 4:  /* Speed boost */
            *(f32 *)((u8 *)car + 0x200) = 1.5f;  /* Temporary speed boost */
            break;
    }
}
```

### Weapons Toggle

Battle mode allows disabling weapons entirely:
```c
s32 weaponsEnabled;  // D_80159E30
draw_text("WEAPONS", 40, 130, selectedOption == 3 ? 255 : 180);
draw_text(weaponsEnabled ? "ON" : "OFF", 180, 130, weaponsEnabled ? 200 : 150);
```

## Damage and Health System

### Health Structure

Player health is stored at car structure offset 0x1F0:
```c
health = (s32 *)((u8 *)car + 0x1F0);
```

### Damage Processing

From `entity_damage_update()`:
```c
void entity_damage_update(void *entity, s32 damage) {
    health = (s32 *)((u8 *)entity + 0x100);
    maxHealth = (s32 *)((u8 *)entity + 0x104);
    damageState = (s32 *)((u8 *)entity + 0x10C);

    /* Apply damage */
    *health -= damage;

    /* Set damage state */
    if (*health <= 0) {
        *damageState = 3;  /* Destroyed */
    } else if (*health < *maxHealth / 4) {
        *damageState = 2;  /* Critical */
    } else if (*health < *maxHealth / 2) {
        *damageState = 1;  /* Damaged */
    } else {
        *damageState = 0;  /* Healthy */
    }
}
```

### Visual Damage Levels

| State | Value | Description |
|-------|-------|-------------|
| Healthy | 0 | No damage - car at full health |
| Damaged | 1 | Light damage - minor dents |
| Critical | 2 | Medium damage - visible damage |
| Destroyed | 3 | Heavy damage - severe deformation + smoke |

From `car_damage_visual()`:
```c
if (damageLevel == 1) {
    *damageState = 1;  /* Light damage - minor dents */
}
else if (damageLevel == 2) {
    *damageState = 2;  /* Medium damage - visible damage */
}
else if (damageLevel >= 3) {
    *damageState = 3;  /* Heavy damage */
    /* Trigger smoke effect on heavy damage */
    if (damageLevel >= 4) {
        // Additional effects
    }
}
```

## Scoring System

### Score Data Structures

```c
extern s32 D_8014B00C[4];  /* Player scores (frags/kills) */
extern s32 player_lives[4]; /* Player lives remaining */
extern s32 D_8015A2D0[4];  /* Frag count per player */
extern s32 D_8015A2E0[4];  /* Death count per player */
extern s32 D_8015A2F0[4];  /* Lives/health per player */
extern s32 D_8014B04C;     /* Score limit to win */
```

### Frag Limits

Configurable frag limits for Deathmatch/Team Battle:
```c
fragLimits[0] = 5;
fragLimits[1] = 10;
fragLimits[2] = 15;
fragLimits[3] = 20;
fragLimits[4] = 0;  /* No limit */
```

### Kill Attribution

When a player is destroyed, the nearest other player is credited:
```c
if (*health <= 0) {
    player_lives[i]--;

    /* Find attacker and award point */
    /* (simplified - award to closest other player) */
    f32 closestDist = 999999.0f;
    s32 closestPlayer = -1;

    for (j = 0; j < 4; j++) {
        if (j == i) continue;
        // Calculate distance...
        if (dist < closestDist) {
            closestDist = dist;
            closestPlayer = j;
        }
    }

    if (closestPlayer >= 0) {
        D_8014B00C[closestPlayer]++;
    }
}
```

## Time System

### Time Limits

```c
timeLimits[0] = 180;   /* 3 minutes */
timeLimits[1] = 300;   /* 5 minutes */
timeLimits[2] = 600;   /* 10 minutes */
timeLimits[3] = 0;     /* No limit */
```

### Battle Timer

```c
extern s32 D_8014B004;    /* Battle timer (frames, increments each frame) */
```

## Respawn System

### Respawn Data

```c
extern f32 D_8014B100[4][3];  /* Spawn positions per player */
extern f32 D_8014B130[4];     /* Spawn rotations per player */
```

### Respawn Logic

From `battle_respawn()`:
```c
void battle_respawn(void *car, s32 playerIdx) {
    /* Set spawn position */
    pos[0] = D_8014B100[playerIdx][0];
    pos[1] = D_8014B100[playerIdx][1];
    pos[2] = D_8014B100[playerIdx][2];

    /* Clear velocity */
    vel[0] = 0.0f;
    vel[1] = 0.0f;
    vel[2] = 0.0f;

    /* Set spawn rotation */
    rot[1] = D_8014B130[playerIdx];

    /* Brief invincibility after spawn */
    *(s32 *)((u8 *)car + 0x1F4) = 180;  /* 3 seconds invincibility */
}
```

### Respawn Conditions

- **Deathmatch/Team Battle**: Respawn immediately with reset health and weapon
- **Last Man Standing**: Respawn only if lives > 0

```c
if (battle_mode_type != 2 || player_lives[i] > 0) {
    battle_respawn(car, i);
    *health = 100;
    *weapon = 0;  /* Reset weapon */
    *ammo = 0;
}
```

## Win Conditions

From `battle_win_check()`:
```c
void battle_win_check(void) {
    /* Check score limit */
    for (i = 0; i < 4; i++) {
        if (D_8014B00C[i] >= D_8014B04C) {
            /* Player reached score limit */
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
}
```

## Battle Mode State Machine

### State Values

```c
extern s32 battle_mode_state;  /* 0=inactive, 1=active, 2=results */
extern s32 battle_mode_enabled; /* Global battle mode flag */
```

### Initialization

From `race_init()` line 21494:
```c
if (game_mode == 3) {  /* Battle mode */
    for (i = 0; i < 4; i++) {
        D_8015A2D0[i] = 0;  /* Frags */
        D_8015A2E0[i] = 0;  /* Deaths */
        D_8015A2F0[i] = 3;  /* Lives/health */
    }
}
```

## N64 Function Mapping

| Function | Address | Size | Description |
|----------|---------|------|-------------|
| `battle_mode_setup` | func_800D4EF4 | 532 bytes | Battle arena/rules configuration menu |
| `battle_mode_update` | func_8010EA14 | 2052 bytes | Main battle mode logic loop |
| `battle_respawn` | func_8010F0D0 | 280 bytes | Player respawn at arena spawn point |
| `battle_pickup_check` | func_8010F158 | 72 bytes | Weapon pickup collision check |
| `battle_fire_weapon` | func_8010F1A0 | 64 bytes | Fire equipped weapon |
| `battle_win_check` | func_8010F1E0 | 56 bytes | Check win conditions |
| `entity_damage_update` | - | 516 bytes | Process damage and health |
| `car_damage_visual` | - | 932 bytes | Apply visual damage to car model |
| `explosion_spawn` | - | 1176 bytes | Create explosion effect |
| `explosion_effect` | - | - | Large explosion with fireball |

## Global Variables Summary

| Address | Name | Type | Description |
|---------|------|------|-------------|
| 0x8014B004 | D_8014B004 | s32 | Battle timer (frames) |
| 0x8014B00C | D_8014B00C | s32[4] | Player scores/frags |
| 0x8014B02C | D_8014B02C | s32[4] | Player current weapon |
| 0x8014B03C | D_8014B03C | s32[4] | Player ammo count |
| 0x8014B04C | D_8014B04C | s32 | Score limit to win |
| 0x8014B100 | D_8014B100 | f32[4][3] | Spawn positions |
| 0x8014B130 | D_8014B130 | f32[4] | Spawn rotations |
| 0x80159E20 | D_80159E20 | s32 | Menu selection state |
| 0x80159E24 | D_80159E24 | s32 | Selected arena index |
| 0x80159E28 | D_80159E28 | s32 | Frag limit setting |
| 0x80159E2C | D_80159E2C | s32 | Time limit setting |
| 0x80159E30 | D_80159E30 | s32 | Weapons enabled flag |
| 0x8015A2D0 | D_8015A2D0 | s32[4] | Per-player frag count |
| 0x8015A2E0 | D_8015A2E0 | s32[4] | Per-player death count |
| 0x8015A2F0 | D_8015A2F0 | s32[4] | Per-player lives |
| - | battle_mode_state | s32 | Battle state (0/1/2) |
| - | battle_mode_enabled | s32 | Battle mode active flag |
| - | battle_mode_type | s32 | Mode type (0-2) |
| - | player_lives | s32[4] | Lives remaining |

## Car Structure Offsets (Battle-Relevant)

| Offset | Type | Description |
|--------|------|-------------|
| 0x24 | f32[3] | Position |
| 0x34 | f32[3] | Velocity |
| 0x60 | f32[3] | Rotation/Forward |
| 0x1F0 | s32 | Health |
| 0x1F4 | s32 | Invincibility timer |
| 0x200 | f32 | Speed multiplier |
| 0x220 | s32 | Damage state |

## Notes

- Battle Mode is unique to Rush 2049 - not present in Rush The Rock arcade source
- Uses same physics/collision as racing but adds weapon/damage layer
- Maximum 4 players (multiplayer required - minimum 2 players to start)
- Projectile types reuse existing particle/effect systems (`smoke_spawn`, `skid_mark_spawn`, etc.)
- Shield and Speed Boost are "use on pickup" instant effects, not stored weapons
