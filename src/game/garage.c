/**
 * garage.c - Garage and car customization for Rush 2049 N64
 *
 * Implements car selection, customization, and upgrade systems.
 */

#include "game/garage.h"

/* External functions */
extern f32 sinf(f32 x);
extern f32 cosf(f32 x);

/* Global garage state */
GarageState gGarage;

/* Car definitions */
CarDef gCarDefs[NUM_CAR_TYPES] = {
    /* Compact */
    {
        "Zipster",
        CAR_TYPE_COMPACT, 1, 0, 0,
        { 180.0f, 3.2f, 0.9f, 0.7f, 0.75f, 2200.0f, 0.6f, 1.0f },
        { 1.1f, 1.15f, 1.2f, 1.1f, 1.15f, 1.1f, 1.2f, 1.0f },
        0, 0, 0, PAINT_GLOSS, {0, 0}
    },
    /* Sports */
    {
        "Velocity",
        CAR_TYPE_SPORTS, 1, 0, 0,
        { 200.0f, 2.8f, 0.85f, 0.8f, 0.8f, 2800.0f, 0.7f, 1.1f },
        { 1.12f, 1.12f, 1.15f, 1.12f, 1.12f, 1.08f, 1.15f, 1.05f },
        1, 1, 1, PAINT_METALLIC, {0, 0}
    },
    /* Muscle */
    {
        "Dominator",
        CAR_TYPE_MUSCLE, 1, 0, 0,
        { 210.0f, 2.5f, 0.7f, 0.75f, 0.7f, 3400.0f, 0.8f, 1.2f },
        { 1.15f, 1.1f, 1.1f, 1.1f, 1.1f, 1.05f, 1.1f, 1.1f },
        2, 2, 2, PAINT_GLOSS, {0, 0}
    },
    /* Exotic */
    {
        "Phantom",
        CAR_TYPE_EXOTIC, 1, 0, 0,
        { 230.0f, 2.2f, 0.9f, 0.85f, 0.85f, 2600.0f, 0.65f, 1.15f },
        { 1.1f, 1.15f, 1.12f, 1.15f, 1.15f, 1.12f, 1.18f, 1.08f },
        3, 3, 3, PAINT_PEARL, {0, 0}
    },
    /* Truck */
    {
        "Bruiser",
        CAR_TYPE_TRUCK, 0, 0, 0,
        { 170.0f, 3.5f, 0.6f, 0.7f, 0.65f, 4200.0f, 1.0f, 0.9f },
        { 1.08f, 1.1f, 1.15f, 1.1f, 1.1f, 1.05f, 1.08f, 1.15f },
        4, 4, 4, PAINT_MATTE, {0, 0}
    },
    /* Concept */
    {
        "Prototype X",
        CAR_TYPE_CONCEPT, 0, 0, 0,
        { 240.0f, 2.0f, 0.95f, 0.9f, 0.9f, 2400.0f, 0.55f, 1.25f },
        { 1.08f, 1.12f, 1.1f, 1.12f, 1.12f, 1.15f, 1.2f, 1.1f },
        5, 5, 5, PAINT_CHROME, {0, 0}
    },
    /* Classic */
    {
        "Retro Racer",
        CAR_TYPE_CLASSIC, 0, 0, 0,
        { 190.0f, 3.0f, 0.75f, 0.65f, 0.7f, 3000.0f, 0.75f, 1.0f },
        { 1.12f, 1.1f, 1.12f, 1.15f, 1.1f, 1.08f, 1.12f, 1.05f },
        6, 6, 6, PAINT_METALLIC, {0, 0}
    },
    /* Special */
    {
        "Rush 2049",
        CAR_TYPE_SPECIAL, 0, 1, 0,
        { 250.0f, 1.8f, 0.95f, 0.95f, 0.95f, 2200.0f, 0.7f, 1.5f },
        { 1.05f, 1.08f, 1.05f, 1.08f, 1.08f, 1.1f, 1.15f, 1.1f },
        7, 7, 7, PAINT_CHROME, {0, 0}
    }
};

/* Paint colors */
PaintColor gPaintColors[MAX_COLORS] = {
    { 255, 0, 0, PAINT_GLOSS, "Red" },
    { 0, 0, 255, PAINT_GLOSS, "Blue" },
    { 0, 255, 0, PAINT_GLOSS, "Green" },
    { 255, 255, 0, PAINT_GLOSS, "Yellow" },
    { 255, 128, 0, PAINT_GLOSS, "Orange" },
    { 128, 0, 255, PAINT_GLOSS, "Purple" },
    { 255, 255, 255, PAINT_GLOSS, "White" },
    { 0, 0, 0, PAINT_GLOSS, "Black" },
    { 192, 192, 192, PAINT_METALLIC, "Silver" },
    { 255, 215, 0, PAINT_METALLIC, "Gold" },
    { 100, 149, 237, PAINT_METALLIC, "Steel Blue" },
    { 220, 20, 60, PAINT_METALLIC, "Crimson" },
    { 0, 100, 0, PAINT_METALLIC, "Forest" },
    { 75, 0, 130, PAINT_PEARL, "Indigo" },
    { 255, 182, 193, PAINT_PEARL, "Pink" },
    { 64, 224, 208, PAINT_PEARL, "Turquoise" },
    { 50, 50, 50, PAINT_MATTE, "Matte Black" },
    { 80, 80, 80, PAINT_MATTE, "Gunmetal" },
    { 139, 69, 19, PAINT_MATTE, "Brown" },
    { 128, 128, 0, PAINT_MATTE, "Olive" },
    { 200, 200, 200, PAINT_CHROME, "Chrome" },
    { 180, 180, 255, PAINT_CHROME, "Ice Chrome" },
    { 255, 200, 180, PAINT_CHROME, "Rose Gold" },
    { 0, 0, 0, 0, "" },  /* Empty slots */
    { 0, 0, 0, 0, "" },
    { 0, 0, 0, 0, "" },
    { 0, 0, 0, 0, "" },
    { 0, 0, 0, 0, "" },
    { 0, 0, 0, 0, "" },
    { 0, 0, 0, 0, "" },
    { 0, 0, 0, 0, "" },
    { 0, 0, 0, 0, "" }
};

/* Upgrade costs (per level) */
static const s32 sUpgradeCosts[NUM_UPGRADE_TYPES][NUM_UPGRADE_LEVELS] = {
    { 0, 1000, 2500, 5000, 10000 },  /* Engine */
    { 0, 800, 2000, 4000, 8000 },    /* Turbo */
    { 0, 600, 1500, 3000, 6000 },    /* Handling */
    { 0, 500, 1200, 2500, 5000 },    /* Brakes */
    { 0, 700, 1800, 3500, 7000 },    /* Tires */
    { 0, 600, 1500, 3000, 6000 },    /* Suspension */
    { 0, 1200, 3000, 6000, 12000 },  /* Weight */
    { 0, 800, 2000, 4000, 8000 }     /* Armor */
};

/* -------------------------------------------------------------------------- */
/* Initialization                                                              */
/* -------------------------------------------------------------------------- */

/**
 * Initialize garage system
 */
void garage_init(void) {
    s32 i;

    /* Clear state */
    for (i = 0; i < (s32)sizeof(GarageState); i++) {
        ((u8*)&gGarage)[i] = 0;
    }

    gGarage.state = GARAGE_STATE_CLOSED;
    gGarage.camera_zoom = 1.0f;
    gGarage.turntable_speed = 0.5f;

    /* Set default working config */
    garage_reset_config();
}

/**
 * Reset garage
 */
void garage_reset(void) {
    garage_init();
}

/* -------------------------------------------------------------------------- */
/* State Control                                                               */
/* -------------------------------------------------------------------------- */

/**
 * Open garage
 */
void garage_open(void) {
    gGarage.state = GARAGE_STATE_BROWSE;
    gGarage.browse_index = 0;
    gGarage.browse_rotation = 0.0f;
    gGarage.camera_angle = 0.0f;

    /* Load current car's config */
    garage_reset_config();
}

/**
 * Close garage
 */
void garage_close(void) {
    gGarage.state = GARAGE_STATE_CLOSED;
}

/**
 * Update garage
 */
void garage_update(void) {
    if (gGarage.state == GARAGE_STATE_CLOSED) {
        return;
    }

    /* Auto-rotate car in browse mode */
    if (gGarage.state == GARAGE_STATE_BROWSE) {
        gGarage.browse_rotation += gGarage.turntable_speed;
        if (gGarage.browse_rotation >= 360.0f) {
            gGarage.browse_rotation -= 360.0f;
        }
    }

    /* Smooth rotation transition */
    if (gGarage.browse_rotation != gGarage.browse_target_rot) {
        f32 diff = gGarage.browse_target_rot - gGarage.browse_rotation;
        if (diff > 180.0f) diff -= 360.0f;
        if (diff < -180.0f) diff += 360.0f;
        gGarage.browse_rotation += diff * 0.1f;
    }
}

/**
 * Draw garage
 */
void garage_draw(void) {
    if (gGarage.state == GARAGE_STATE_CLOSED) {
        return;
    }

    /* Draw current car */
    garage_draw_car_model(gGarage.current_car, &gGarage.working_config);

    /* Draw appropriate menu */
    switch (gGarage.state) {
        case GARAGE_STATE_BROWSE:
            /* Draw car name and basic stats */
            if (gGarage.stats_visible) {
                garage_draw_stats_bars(&gGarage.working_config.current_stats);
            }
            break;

        case GARAGE_STATE_PAINT:
            garage_draw_color_picker();
            break;

        case GARAGE_STATE_UPGRADES:
            garage_draw_upgrade_menu();
            break;
    }
}

/**
 * Check if garage is open
 */
s32 garage_is_open(void) {
    return (gGarage.state != GARAGE_STATE_CLOSED);
}

/* -------------------------------------------------------------------------- */
/* Navigation                                                                  */
/* -------------------------------------------------------------------------- */

/**
 * Select next car
 */
void garage_next_car(void) {
    s32 start = gGarage.current_car;

    do {
        gGarage.current_car++;
        if (gGarage.current_car >= NUM_CAR_TYPES) {
            gGarage.current_car = 0;
        }
    } while (!garage_is_car_unlocked(gGarage.current_car) &&
             gGarage.current_car != start);

    garage_reset_config();
    gGarage.browse_target_rot = gGarage.browse_rotation + 45.0f;
}

/**
 * Select previous car
 */
void garage_prev_car(void) {
    s32 start = gGarage.current_car;

    do {
        gGarage.current_car--;
        if (gGarage.current_car < 0) {
            gGarage.current_car = NUM_CAR_TYPES - 1;
        }
    } while (!garage_is_car_unlocked(gGarage.current_car) &&
             gGarage.current_car != start);

    garage_reset_config();
    gGarage.browse_target_rot = gGarage.browse_rotation - 45.0f;
}

/**
 * Select specific car
 */
void garage_select_car(s32 car_type) {
    if (car_type >= 0 && car_type < NUM_CAR_TYPES) {
        if (garage_is_car_unlocked(car_type)) {
            gGarage.current_car = (u8)car_type;
            garage_reset_config();
        }
    }
}

/**
 * Get current car type
 */
s32 garage_get_current_car(void) {
    return gGarage.current_car;
}

/* -------------------------------------------------------------------------- */
/* Menu Navigation                                                             */
/* -------------------------------------------------------------------------- */

void garage_enter_menu(s32 menu) {
    gGarage.state = (u8)menu;
    gGarage.current_option = 0;
}

void garage_exit_menu(void) {
    gGarage.state = GARAGE_STATE_BROWSE;
}

void garage_menu_up(void) {
    if (gGarage.current_option > 0) {
        gGarage.current_option--;
    }
}

void garage_menu_down(void) {
    gGarage.current_option++;
    /* Would clamp to menu size */
}

void garage_menu_left(void) {
    switch (gGarage.state) {
        case GARAGE_STATE_PAINT:
            if (gGarage.working_config.color_index > 0) {
                gGarage.working_config.color_index--;
            }
            break;
        case GARAGE_STATE_UPGRADES:
            /* Decrease upgrade level */
            if (gGarage.working_config.upgrades[gGarage.current_option] > 0) {
                gGarage.working_config.upgrades[gGarage.current_option]--;
                garage_apply_upgrades(&gGarage.working_config);
            }
            break;
    }
}

void garage_menu_right(void) {
    switch (gGarage.state) {
        case GARAGE_STATE_PAINT:
            if (gGarage.working_config.color_index < MAX_COLORS - 1) {
                gGarage.working_config.color_index++;
            }
            break;
        case GARAGE_STATE_UPGRADES:
            /* Increase upgrade level */
            if (gGarage.working_config.upgrades[gGarage.current_option] < UPGRADE_MAX) {
                gGarage.working_config.upgrades[gGarage.current_option]++;
                garage_apply_upgrades(&gGarage.working_config);
            }
            break;
    }
}

void garage_menu_select(void) {
    switch (gGarage.state) {
        case GARAGE_STATE_BROWSE:
            gGarage.state = GARAGE_STATE_CUSTOMIZE;
            break;
        case GARAGE_STATE_CUSTOMIZE:
            /* Enter sub-menu based on option */
            switch (gGarage.current_option) {
                case 0: gGarage.state = GARAGE_STATE_PAINT; break;
                case 1: gGarage.state = GARAGE_STATE_UPGRADES; break;
                case 2: gGarage.state = GARAGE_STATE_VISUALS; break;
                case 3: gGarage.state = GARAGE_STATE_CONFIRM; break;
            }
            gGarage.current_option = 0;
            break;
        case GARAGE_STATE_CONFIRM:
            /* Confirm selection */
            garage_close();
            break;
    }
}

void garage_menu_back(void) {
    switch (gGarage.state) {
        case GARAGE_STATE_BROWSE:
            garage_close();
            break;
        case GARAGE_STATE_CUSTOMIZE:
            gGarage.state = GARAGE_STATE_BROWSE;
            break;
        case GARAGE_STATE_PAINT:
        case GARAGE_STATE_UPGRADES:
        case GARAGE_STATE_VISUALS:
        case GARAGE_STATE_CONFIRM:
            gGarage.state = GARAGE_STATE_CUSTOMIZE;
            break;
    }
    gGarage.current_option = 0;
}

/* -------------------------------------------------------------------------- */
/* Paint Shop                                                                  */
/* -------------------------------------------------------------------------- */

void garage_set_color(s32 color_index) {
    if (color_index >= 0 && color_index < MAX_COLORS) {
        gGarage.working_config.color_index = (u8)color_index;
        gGarage.working_config.finish = gPaintColors[color_index].finish;
    }
}

void garage_set_finish(s32 finish) {
    if (finish >= 0 && finish < NUM_PAINT_FINISHES) {
        gGarage.working_config.finish = (u8)finish;
    }
}

void garage_set_custom_color(u8 r, u8 g, u8 b) {
    gGarage.working_config.custom_color.r = r;
    gGarage.working_config.custom_color.g = g;
    gGarage.working_config.custom_color.b = b;
}

s32 garage_get_color(void) {
    return gGarage.working_config.color_index;
}

s32 garage_get_finish(void) {
    return gGarage.working_config.finish;
}

PaintColor* garage_get_paint_color(s32 index) {
    if (index >= 0 && index < MAX_COLORS) {
        return &gPaintColors[index];
    }
    return NULL;
}

/* -------------------------------------------------------------------------- */
/* Upgrades                                                                    */
/* -------------------------------------------------------------------------- */

void garage_set_upgrade(s32 upgrade_type, s32 level) {
    if (upgrade_type >= 0 && upgrade_type < NUM_UPGRADE_TYPES) {
        if (level >= 0 && level <= UPGRADE_MAX) {
            gGarage.working_config.upgrades[upgrade_type] = (u8)level;
            garage_apply_upgrades(&gGarage.working_config);
        }
    }
}

s32 garage_get_upgrade(s32 upgrade_type) {
    if (upgrade_type >= 0 && upgrade_type < NUM_UPGRADE_TYPES) {
        return gGarage.working_config.upgrades[upgrade_type];
    }
    return 0;
}

s32 garage_get_upgrade_cost(s32 upgrade_type, s32 level) {
    if (upgrade_type >= 0 && upgrade_type < NUM_UPGRADE_TYPES) {
        if (level >= 0 && level < NUM_UPGRADE_LEVELS) {
            return sUpgradeCosts[upgrade_type][level];
        }
    }
    return 0;
}

s32 garage_can_afford_upgrade(s32 upgrade_type, s32 level) {
    /* Would check player's money */
    return 1;
}

/**
 * Apply upgrades to calculate current stats
 */
void garage_apply_upgrades(CarConfig *config) {
    CarDef *def;
    CarStats *base;
    f32 mult;
    s32 i;

    def = garage_get_car_def(config->car_type);
    if (def == NULL) {
        return;
    }

    base = &def->base_stats;

    /* Start with base stats */
    config->current_stats = *base;

    /* Apply upgrade multipliers */
    for (i = 0; i < NUM_UPGRADE_TYPES; i++) {
        if (config->upgrades[i] > 0) {
            mult = 1.0f;
            /* Each level adds the multiplier bonus */
            mult += (def->upgrade_mult[i] - 1.0f) * config->upgrades[i];

            switch (i) {
                case UPGRADE_ENGINE:
                    config->current_stats.top_speed *= mult;
                    break;
                case UPGRADE_TURBO:
                    config->current_stats.acceleration /= mult;  /* Lower is better */
                    break;
                case UPGRADE_HANDLING:
                    config->current_stats.handling *= mult;
                    break;
                case UPGRADE_BRAKES:
                    config->current_stats.braking *= mult;
                    break;
                case UPGRADE_TIRES:
                    config->current_stats.grip *= mult;
                    break;
                case UPGRADE_SUSPENSION:
                    /* Improves handling and grip slightly */
                    config->current_stats.handling *= (1.0f + (mult - 1.0f) * 0.5f);
                    config->current_stats.grip *= (1.0f + (mult - 1.0f) * 0.5f);
                    break;
                case UPGRADE_WEIGHT:
                    config->current_stats.weight /= mult;  /* Lower is better */
                    break;
                case UPGRADE_ARMOR:
                    config->current_stats.durability *= mult;
                    break;
            }
        }
    }
}

/* -------------------------------------------------------------------------- */
/* Visual Options                                                              */
/* -------------------------------------------------------------------------- */

void garage_set_spoiler(s32 style) {
    if (style >= 0 && style < MAX_SPOILER_STYLES) {
        gGarage.working_config.spoiler_style = (u8)style;
    }
}

void garage_set_wheels(s32 style) {
    if (style >= 0 && style < MAX_WHEEL_STYLES) {
        gGarage.working_config.wheel_style = (u8)style;
    }
}

void garage_set_body_kit(s32 style) {
    gGarage.working_config.body_kit = (u8)style;
}

void garage_set_decal(s32 decal, s32 position) {
    if (decal >= 0 && decal < MAX_DECALS) {
        gGarage.working_config.decal_index = (u8)decal;
    }
    if (position >= 0 && position < NUM_DECAL_POSITIONS) {
        gGarage.working_config.decal_position = (u8)position;
    }
}

void garage_set_window_tint(s32 tint) {
    gGarage.working_config.window_tint = (u8)tint;
}

/* -------------------------------------------------------------------------- */
/* Statistics                                                                  */
/* -------------------------------------------------------------------------- */

void garage_calculate_stats(CarConfig *config, CarStats *out) {
    garage_apply_upgrades(config);
    *out = config->current_stats;
}

CarStats* garage_get_base_stats(s32 car_type) {
    if (car_type >= 0 && car_type < NUM_CAR_TYPES) {
        return &gCarDefs[car_type].base_stats;
    }
    return NULL;
}

CarStats* garage_get_current_stats(void) {
    return &gGarage.working_config.current_stats;
}

/**
 * Get stat as percentage (for bar display)
 */
f32 garage_get_stat_percent(CarStats *stats, s32 stat_type) {
    f32 value, max_value;

    switch (stat_type) {
        case 0: /* Speed */
            value = stats->top_speed;
            max_value = 300.0f;
            break;
        case 1: /* Acceleration */
            value = 5.0f - stats->acceleration;  /* Invert (lower is better) */
            max_value = 4.0f;
            break;
        case 2: /* Handling */
            value = stats->handling;
            max_value = 1.0f;
            break;
        case 3: /* Braking */
            value = stats->braking;
            max_value = 1.0f;
            break;
        case 4: /* Grip */
            value = stats->grip;
            max_value = 1.0f;
            break;
        default:
            return 0.0f;
    }

    if (value < 0.0f) value = 0.0f;
    if (value > max_value) value = max_value;

    return (value / max_value) * 100.0f;
}

/* -------------------------------------------------------------------------- */
/* Saved Configurations                                                        */
/* -------------------------------------------------------------------------- */

s32 garage_save_config(s32 slot, const char *name) {
    s32 i;
    SavedCar *saved;

    if (slot < 0 || slot >= MAX_SAVED_CARS) {
        return 0;
    }

    saved = &gGarage.saved_cars[slot];
    saved->used = 1;
    saved->car_type = gGarage.current_car;

    for (i = 0; name[i] && i < 15; i++) {
        saved->name[i] = name[i];
    }
    saved->name[i] = '\0';

    garage_copy_config(&gGarage.working_config, &saved->config);

    return 1;
}

s32 garage_load_config(s32 slot) {
    SavedCar *saved;

    if (slot < 0 || slot >= MAX_SAVED_CARS) {
        return 0;
    }

    saved = &gGarage.saved_cars[slot];
    if (!saved->used) {
        return 0;
    }

    gGarage.current_car = saved->car_type;
    garage_copy_config(&saved->config, &gGarage.working_config);

    return 1;
}

s32 garage_delete_config(s32 slot) {
    if (slot < 0 || slot >= MAX_SAVED_CARS) {
        return 0;
    }

    gGarage.saved_cars[slot].used = 0;
    return 1;
}

s32 garage_get_saved_count(void) {
    s32 i, count = 0;

    for (i = 0; i < MAX_SAVED_CARS; i++) {
        if (gGarage.saved_cars[i].used) {
            count++;
        }
    }

    return count;
}

SavedCar* garage_get_saved(s32 slot) {
    if (slot >= 0 && slot < MAX_SAVED_CARS) {
        return &gGarage.saved_cars[slot];
    }
    return NULL;
}

/* -------------------------------------------------------------------------- */
/* Car Access                                                                  */
/* -------------------------------------------------------------------------- */

CarDef* garage_get_car_def(s32 car_type) {
    if (car_type >= 0 && car_type < NUM_CAR_TYPES) {
        return &gCarDefs[car_type];
    }
    return NULL;
}

s32 garage_is_car_unlocked(s32 car_type) {
    if (car_type >= 0 && car_type < NUM_CAR_TYPES) {
        return gCarDefs[car_type].unlocked;
    }
    return 0;
}

void garage_unlock_car(s32 car_type) {
    if (car_type >= 0 && car_type < NUM_CAR_TYPES) {
        gCarDefs[car_type].unlocked = 1;
    }
}

s32 garage_get_unlocked_count(void) {
    s32 i, count = 0;

    for (i = 0; i < NUM_CAR_TYPES; i++) {
        if (gCarDefs[i].unlocked) {
            count++;
        }
    }

    return count;
}

/* -------------------------------------------------------------------------- */
/* Configuration                                                               */
/* -------------------------------------------------------------------------- */

CarConfig* garage_get_config(void) {
    return &gGarage.working_config;
}

void garage_apply_config(s32 player, CarConfig *config) {
    /* Would apply config to actual game car */
}

void garage_reset_config(void) {
    s32 i;
    CarDef *def;

    def = garage_get_car_def(gGarage.current_car);
    if (def == NULL) {
        return;
    }

    gGarage.working_config.car_type = gGarage.current_car;
    gGarage.working_config.color_index = def->default_color;
    gGarage.working_config.finish = def->default_finish;
    gGarage.working_config.transmission = 0;  /* Auto */

    /* Reset upgrades to stock */
    for (i = 0; i < NUM_UPGRADE_TYPES; i++) {
        gGarage.working_config.upgrades[i] = UPGRADE_STOCK;
    }

    /* Reset visuals */
    gGarage.working_config.spoiler_style = 0;
    gGarage.working_config.hood_style = 0;
    gGarage.working_config.wheel_style = 0;
    gGarage.working_config.body_kit = 0;
    gGarage.working_config.window_tint = 0;
    gGarage.working_config.decal_index = 0;
    gGarage.working_config.decal_position = 0;

    /* Calculate initial stats */
    gGarage.working_config.current_stats = def->base_stats;
}

void garage_copy_config(CarConfig *src, CarConfig *dst) {
    s32 i;

    for (i = 0; i < (s32)sizeof(CarConfig); i++) {
        ((u8*)dst)[i] = ((u8*)src)[i];
    }
}

/* -------------------------------------------------------------------------- */
/* Preview                                                                     */
/* -------------------------------------------------------------------------- */

void garage_start_preview(void) {
    gGarage.preview_mode = 1;
}

void garage_stop_preview(void) {
    gGarage.preview_mode = 0;
}

void garage_rotate_preview(f32 amount) {
    gGarage.camera_angle += amount;
    if (gGarage.camera_angle >= 360.0f) {
        gGarage.camera_angle -= 360.0f;
    }
    if (gGarage.camera_angle < 0.0f) {
        gGarage.camera_angle += 360.0f;
    }
}

void garage_zoom_preview(f32 amount) {
    gGarage.camera_zoom += amount;
    if (gGarage.camera_zoom < 0.5f) {
        gGarage.camera_zoom = 0.5f;
    }
    if (gGarage.camera_zoom > 2.0f) {
        gGarage.camera_zoom = 2.0f;
    }
}

/* -------------------------------------------------------------------------- */
/* Drawing                                                                     */
/* -------------------------------------------------------------------------- */

void garage_draw_car_model(s32 car_type, CarConfig *config) {
    /* Would render 3D car model with current configuration */
}

void garage_draw_stats_bars(CarStats *stats) {
    /* Would draw stat bars UI */
}

void garage_draw_color_picker(void) {
    /* Would draw color selection grid */
}

void garage_draw_upgrade_menu(void) {
    /* Would draw upgrade selection UI */
}
