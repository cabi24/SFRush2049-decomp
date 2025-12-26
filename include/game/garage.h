/**
 * garage.h - Garage and car customization for Rush 2049 N64
 *
 * Handles car selection, paint colors, performance upgrades,
 * and visual customization options.
 */

#ifndef GARAGE_H
#define GARAGE_H

#include "types.h"

/* Car types */
#define CAR_TYPE_COMPACT        0   /* Small, agile */
#define CAR_TYPE_SPORTS         1   /* Balanced */
#define CAR_TYPE_MUSCLE         2   /* Fast, heavy */
#define CAR_TYPE_EXOTIC         3   /* High performance */
#define CAR_TYPE_TRUCK          4   /* Heavy, durable */
#define CAR_TYPE_CONCEPT        5   /* Futuristic */
#define CAR_TYPE_CLASSIC        6   /* Classic car */
#define CAR_TYPE_SPECIAL        7   /* Unlockable special */
#define NUM_CAR_TYPES           8

/* Paint finish types */
#define PAINT_MATTE             0
#define PAINT_GLOSS             1
#define PAINT_METALLIC          2
#define PAINT_PEARL             3
#define PAINT_CHROME            4
#define NUM_PAINT_FINISHES      5

/* Upgrade categories */
#define UPGRADE_ENGINE          0   /* Top speed */
#define UPGRADE_TURBO           1   /* Acceleration */
#define UPGRADE_HANDLING        2   /* Steering response */
#define UPGRADE_BRAKES          3   /* Braking power */
#define UPGRADE_TIRES           4   /* Grip */
#define UPGRADE_SUSPENSION      5   /* Stability */
#define UPGRADE_WEIGHT          6   /* Weight reduction */
#define UPGRADE_ARMOR           7   /* Durability (battle) */
#define NUM_UPGRADE_TYPES       8

/* Upgrade levels */
#define UPGRADE_STOCK           0
#define UPGRADE_LEVEL_1         1
#define UPGRADE_LEVEL_2         2
#define UPGRADE_LEVEL_3         3
#define UPGRADE_MAX             4
#define NUM_UPGRADE_LEVELS      5

/* Visual options */
#define VISUAL_SPOILER          0
#define VISUAL_HOOD             1
#define VISUAL_WHEELS           2
#define VISUAL_BODY_KIT         3
#define VISUAL_DECALS           4
#define VISUAL_WINDOWS          5
#define NUM_VISUAL_OPTIONS      6

/* Decal positions */
#define DECAL_HOOD              0
#define DECAL_DOORS             1
#define DECAL_ROOF              2
#define DECAL_REAR              3
#define NUM_DECAL_POSITIONS     4

/* Maximum values */
#define MAX_COLORS              32      /* Available paint colors */
#define MAX_DECALS              16      /* Available decals */
#define MAX_WHEEL_STYLES        8       /* Wheel options */
#define MAX_SPOILER_STYLES      4       /* Spoiler options */
#define MAX_SAVED_CARS          8       /* Saved configurations */

/* Garage states */
#define GARAGE_STATE_CLOSED     0
#define GARAGE_STATE_BROWSE     1       /* Browsing cars */
#define GARAGE_STATE_CUSTOMIZE  2       /* Customizing */
#define GARAGE_STATE_PAINT      3       /* Paint shop */
#define GARAGE_STATE_UPGRADES   4       /* Upgrade shop */
#define GARAGE_STATE_VISUALS    5       /* Visual mods */
#define GARAGE_STATE_CONFIRM    6       /* Confirming selection */

/* RGB color */
typedef struct PaintColor {
    u8      r, g, b;
    u8      finish;             /* PAINT_* finish type */
    char    name[16];           /* Color name */
} PaintColor;

/* Car statistics */
typedef struct CarStats {
    f32     top_speed;          /* Maximum speed */
    f32     acceleration;       /* 0-60 time equivalent */
    f32     handling;           /* Steering response */
    f32     braking;            /* Brake power */
    f32     grip;               /* Tire grip */
    f32     weight;             /* Car weight */
    f32     durability;         /* Damage resistance */
    f32     boost_power;        /* Boost strength */
} CarStats;

/* Car definition */
typedef struct CarDef {
    char    name[24];           /* Car name */
    u8      type;               /* CAR_TYPE_* */
    u8      unlocked;           /* Available to player */
    u8      hidden;             /* Secret car */
    u8      pad;

    /* Base stats (stock) */
    CarStats base_stats;

    /* Upgrade multipliers (per level) */
    f32     upgrade_mult[NUM_UPGRADE_TYPES];

    /* Model info */
    u32     model_id;           /* 3D model ID */
    u32     wheel_model;        /* Wheel model ID */

    /* Default colors */
    u8      default_color;      /* Default paint index */
    u8      default_finish;     /* Default finish */
    u8      pad2[2];

} CarDef;

/* Player's car configuration */
typedef struct CarConfig {
    u8      car_type;           /* CAR_TYPE_* */
    u8      color_index;        /* Paint color index */
    u8      finish;             /* Paint finish */
    u8      transmission;       /* 0=auto, 1=manual */

    /* Upgrades */
    u8      upgrades[NUM_UPGRADE_TYPES];

    /* Visual options */
    u8      spoiler_style;
    u8      hood_style;
    u8      wheel_style;
    u8      body_kit;
    u8      window_tint;
    u8      decal_index;
    u8      decal_position;
    u8      pad;

    /* Calculated stats */
    CarStats current_stats;

    /* Custom color (if custom) */
    PaintColor custom_color;

} CarConfig;

/* Saved car slot */
typedef struct SavedCar {
    u8      used;               /* Slot in use */
    u8      car_type;           /* Base car */
    u8      pad[2];
    char    name[16];           /* Player's name for config */
    CarConfig config;           /* Full configuration */
} SavedCar;

/* Garage state */
typedef struct GarageState {
    u8      state;              /* GARAGE_STATE_* */
    u8      current_car;        /* Selected car type */
    u8      current_menu;       /* Current menu section */
    u8      current_option;     /* Selected option */

    /* Browsing */
    s32     browse_index;       /* Carousel position */
    f32     browse_rotation;    /* Car rotation angle */
    f32     browse_target_rot;  /* Target rotation */

    /* Current configuration being edited */
    CarConfig working_config;

    /* Saved configurations */
    SavedCar saved_cars[MAX_SAVED_CARS];

    /* Preview */
    u8      preview_mode;       /* Showing preview */
    u8      stats_visible;      /* Stats overlay shown */
    u8      pad[2];

    /* Animation */
    f32     camera_angle;       /* Camera orbit angle */
    f32     camera_zoom;        /* Camera distance */
    f32     turntable_speed;    /* Auto-rotation speed */

} GarageState;

/* Global garage state */
extern GarageState gGarage;

/* Car definitions */
extern CarDef gCarDefs[NUM_CAR_TYPES];

/* Available colors */
extern PaintColor gPaintColors[MAX_COLORS];

/* Initialization */
void garage_init(void);
void garage_reset(void);

/* State control */
void garage_open(void);
void garage_close(void);
void garage_update(void);
void garage_draw(void);
s32 garage_is_open(void);

/* Navigation */
void garage_next_car(void);
void garage_prev_car(void);
void garage_select_car(s32 car_type);
s32 garage_get_current_car(void);

/* Menu navigation */
void garage_enter_menu(s32 menu);
void garage_exit_menu(void);
void garage_menu_up(void);
void garage_menu_down(void);
void garage_menu_left(void);
void garage_menu_right(void);
void garage_menu_select(void);
void garage_menu_back(void);

/* Paint shop */
void garage_set_color(s32 color_index);
void garage_set_finish(s32 finish);
void garage_set_custom_color(u8 r, u8 g, u8 b);
s32 garage_get_color(void);
s32 garage_get_finish(void);
PaintColor* garage_get_paint_color(s32 index);

/* Upgrades */
void garage_set_upgrade(s32 upgrade_type, s32 level);
s32 garage_get_upgrade(s32 upgrade_type);
s32 garage_get_upgrade_cost(s32 upgrade_type, s32 level);
s32 garage_can_afford_upgrade(s32 upgrade_type, s32 level);
void garage_apply_upgrades(CarConfig *config);

/* Visual options */
void garage_set_spoiler(s32 style);
void garage_set_wheels(s32 style);
void garage_set_body_kit(s32 style);
void garage_set_decal(s32 decal, s32 position);
void garage_set_window_tint(s32 tint);

/* Statistics */
void garage_calculate_stats(CarConfig *config, CarStats *out);
CarStats* garage_get_base_stats(s32 car_type);
CarStats* garage_get_current_stats(void);
f32 garage_get_stat_percent(CarStats *stats, s32 stat_type);

/* Saved configurations */
s32 garage_save_config(s32 slot, const char *name);
s32 garage_load_config(s32 slot);
s32 garage_delete_config(s32 slot);
s32 garage_get_saved_count(void);
SavedCar* garage_get_saved(s32 slot);

/* Car access */
CarDef* garage_get_car_def(s32 car_type);
s32 garage_is_car_unlocked(s32 car_type);
void garage_unlock_car(s32 car_type);
s32 garage_get_unlocked_count(void);

/* Configuration */
CarConfig* garage_get_config(void);
void garage_apply_config(s32 player, CarConfig *config);
void garage_reset_config(void);
void garage_copy_config(CarConfig *src, CarConfig *dst);

/* Preview */
void garage_start_preview(void);
void garage_stop_preview(void);
void garage_rotate_preview(f32 amount);
void garage_zoom_preview(f32 amount);

/* Drawing helpers */
void garage_draw_car_model(s32 car_type, CarConfig *config);
void garage_draw_stats_bars(CarStats *stats);
void garage_draw_color_picker(void);
void garage_draw_upgrade_menu(void);

#endif /* GARAGE_H */
