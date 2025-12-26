/**
 * boost.h - Boost/Nitro system for Rush 2049 N64
 *
 * Handles boost meter, nitro pickups, boost pads,
 * and speed multipliers during gameplay.
 */

#ifndef BOOST_H
#define BOOST_H

#include "types.h"

/* Boost states */
#define BOOST_STATE_IDLE        0   /* Not boosting */
#define BOOST_STATE_ACTIVE      1   /* Boost in progress */
#define BOOST_STATE_COOLDOWN    2   /* Cooldown after boost */
#define BOOST_STATE_RECHARGING  3   /* Recharging meter */
#define NUM_BOOST_STATES        4

/* Boost types */
#define BOOST_TYPE_NITRO        0   /* Player-activated nitro */
#define BOOST_TYPE_PAD          1   /* Road boost pad */
#define BOOST_TYPE_JUMP         2   /* Jump boost (from ramps) */
#define BOOST_TYPE_STUNT        3   /* Stunt completion boost */
#define BOOST_TYPE_PICKUP       4   /* Instant pickup boost */
#define NUM_BOOST_TYPES         5

/* Boost sources */
#define BOOST_SOURCE_BUTTON     0   /* Player pressed boost */
#define BOOST_SOURCE_PAD        1   /* Drove over boost pad */
#define BOOST_SOURCE_STUNT      2   /* Completed stunt */
#define BOOST_SOURCE_PICKUP     3   /* Collected boost pickup */
#define BOOST_SOURCE_CHEAT      4   /* Cheat/debug */
#define NUM_BOOST_SOURCES       5

/* Boost limits */
#define MAX_BOOST_CHARGES       3       /* Maximum stored charges */
#define MAX_BOOST_PADS          32      /* Maximum boost pads per track */
#define MAX_ACTIVE_BOOSTS       8       /* Max concurrent boosts */

/* Boost timing (frames at 60fps) */
#define BOOST_DURATION_NITRO    90      /* 1.5 seconds */
#define BOOST_DURATION_PAD      60      /* 1 second */
#define BOOST_DURATION_STUNT    45      /* 0.75 seconds */
#define BOOST_DURATION_PICKUP   30      /* 0.5 seconds */
#define BOOST_COOLDOWN_TIME     30      /* 0.5 second cooldown */
#define BOOST_RECHARGE_TIME     300     /* 5 seconds to recharge */

/* Boost multipliers */
#define BOOST_MULT_NITRO        1.5f    /* 50% speed increase */
#define BOOST_MULT_PAD          1.3f    /* 30% speed increase */
#define BOOST_MULT_STUNT        1.2f    /* 20% speed increase */
#define BOOST_MULT_PICKUP       1.4f    /* 40% speed increase */
#define BOOST_MULT_MAX          2.0f    /* Maximum combined boost */

/* Meter values */
#define BOOST_METER_MAX         100.0f  /* Full meter */
#define BOOST_METER_PER_USE     100.0f  /* Meter used per boost */
#define BOOST_METER_PICKUP      50.0f   /* Meter from pickup */
#define BOOST_METER_STUNT       25.0f   /* Meter from stunt */

/* Active boost effect */
typedef struct ActiveBoost {
    u8      active;             /* Boost is active */
    u8      type;               /* BOOST_TYPE_* */
    u8      source;             /* BOOST_SOURCE_* */
    u8      pad;

    f32     multiplier;         /* Speed multiplier */
    f32     duration;           /* Remaining duration (frames) */
    f32     max_duration;       /* Initial duration */

} ActiveBoost;

/* Boost pad on track */
typedef struct BoostPad {
    f32     pos[3];             /* World position */
    f32     size[2];            /* Width, length */
    f32     rotation;           /* Facing angle */
    f32     multiplier;         /* Speed boost amount */
    f32     duration;           /* How long boost lasts */
    u8      active;             /* Pad is active */
    u8      type;               /* Pad type (normal, super, etc.) */
    u8      pad[2];
} BoostPad;

/* Per-player boost state */
typedef struct PlayerBoost {
    /* Meter/charges */
    f32     meter;              /* Current meter (0-100) */
    s32     charges;            /* Available charges */
    s32     max_charges;        /* Maximum charges */

    /* State */
    u8      state;              /* BOOST_STATE_* */
    u8      can_boost;          /* Player can activate boost */
    u8      button_held;        /* Boost button is held */
    u8      pad;

    /* Timing */
    f32     state_timer;        /* Timer for current state */
    f32     cooldown;           /* Remaining cooldown */
    f32     recharge_timer;     /* Time since last boost */

    /* Active boosts */
    ActiveBoost active[MAX_ACTIVE_BOOSTS];
    s32     num_active;         /* Number of active boosts */

    /* Combined effect */
    f32     total_multiplier;   /* Combined speed multiplier */
    f32     time_boost;         /* Time scaling (arcade compat) */

    /* Visual state */
    f32     flame_intensity;    /* Exhaust flame intensity */
    f32     trail_alpha;        /* Boost trail alpha */
    u8      effect_active;      /* Visual effects on */
    u8      pad2[3];

    /* Statistics */
    s32     boosts_used;        /* Total boosts activated */
    s32     pads_hit;           /* Boost pads hit */
    s32     pickups_collected;  /* Boost pickups collected */

} PlayerBoost;

/* Boost system state */
typedef struct BoostSystem {
    /* Player boost states */
    PlayerBoost players[4];     /* Up to 4 players */
    s32     num_players;

    /* Track boost pads */
    BoostPad pads[MAX_BOOST_PADS];
    s32     num_pads;

    /* Settings */
    u8      enabled;            /* System enabled */
    u8      infinite;           /* Infinite boost (cheat) */
    u8      auto_recharge;      /* Auto recharge meter */
    u8      pad;

    f32     recharge_rate;      /* Meter recharge per frame */
    f32     global_mult;        /* Global boost multiplier */

} BoostSystem;

/* Global boost system */
extern BoostSystem gBoost;

/* Initialization */
void boost_init(void);
void boost_reset(void);
void boost_init_player(s32 player);

/* Update */
void boost_update(void);
void boost_update_player(s32 player);
void boost_update_active(PlayerBoost *pb);
void boost_update_meter(PlayerBoost *pb);
void boost_update_effects(s32 player);

/* Activation */
s32 boost_activate(s32 player, s32 type, s32 source);
s32 boost_can_activate(s32 player);
void boost_deactivate(s32 player, s32 boost_index);
void boost_deactivate_all(s32 player);

/* Meter management */
void boost_add_meter(s32 player, f32 amount);
void boost_drain_meter(s32 player, f32 amount);
void boost_set_meter(s32 player, f32 amount);
f32 boost_get_meter(s32 player);
s32 boost_get_charges(s32 player);
void boost_add_charge(s32 player);

/* Boost pads */
void boost_load_pads(void *track_data);
void boost_clear_pads(void);
s32 boost_add_pad(f32 *pos, f32 width, f32 length, f32 rotation, f32 mult);
void boost_check_pads(s32 player, f32 *pos);
BoostPad* boost_get_pad(s32 index);

/* Queries */
f32 boost_get_multiplier(s32 player);
f32 boost_get_time_boost(s32 player);
s32 boost_is_active(s32 player);
s32 boost_get_state(s32 player);
f32 boost_get_flame_intensity(s32 player);

/* Pickups and stunts */
void boost_on_pickup(s32 player, s32 pickup_type);
void boost_on_stunt(s32 player, f32 bonus);
void boost_on_jump(s32 player);

/* Input */
void boost_press(s32 player);
void boost_release(s32 player);

/* Settings */
void boost_set_enabled(u8 enabled);
void boost_set_infinite(u8 infinite);
void boost_set_recharge_rate(f32 rate);
void boost_set_global_mult(f32 mult);

/* Visual effects */
void boost_draw_effects(s32 player);
void boost_draw_meter(s32 player, s16 x, s16 y, s16 width, s16 height);
void boost_draw_charges(s32 player, s16 x, s16 y);
void boost_draw_pads(void);

/* Debug */
void boost_debug_print(s32 player);
void boost_give_full(s32 player);

#endif /* BOOST_H */
