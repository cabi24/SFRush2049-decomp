/**
 * weather.c - Weather and environment effects for Rush 2049 N64
 *
 * Implements dynamic weather including rain, fog, day/night cycle,
 * and environmental particles.
 */

#include "game/weather.h"

/* External functions */
extern f32 sinf(f32 x);
extern f32 cosf(f32 x);
extern f32 sqrtf(f32 x);
extern s32 rand(void);
extern void srand(u32 seed);
extern void sound_play(s32 sound_id);

/* Sound IDs */
#define SND_THUNDER             100
#define SND_RAIN_LOOP           101
#define SND_WIND_LOOP           102

/* Random helper */
#define RANDF() ((f32)(rand() % 1000) / 1000.0f)
#define RANDF_RANGE(min, max) ((min) + RANDF() * ((max) - (min)))

/* Global weather state */
WeatherState gWeather;

/* Default colors for different times of day */
static const WeatherColor sSkyTopColors[NUM_TIMES_OF_DAY] = {
    {255, 180, 120, 255},   /* Dawn - orange tint */
    {135, 206, 235, 255},   /* Morning - light blue */
    {100, 149, 237, 255},   /* Noon - cornflower blue */
    {135, 180, 220, 255},   /* Afternoon - soft blue */
    {255, 140, 100, 255},   /* Dusk - orange/red */
    {20, 24, 82, 255}       /* Night - dark blue */
};

static const WeatherColor sSkyBottomColors[NUM_TIMES_OF_DAY] = {
    {255, 220, 180, 255},   /* Dawn */
    {200, 230, 255, 255},   /* Morning */
    {180, 210, 240, 255},   /* Noon */
    {220, 200, 180, 255},   /* Afternoon */
    {200, 100, 80, 255},    /* Dusk */
    {10, 12, 40, 255}       /* Night */
};

static const WeatherColor sAmbientColors[NUM_TIMES_OF_DAY] = {
    {100, 80, 60, 255},     /* Dawn */
    {120, 120, 130, 255},   /* Morning */
    {140, 140, 150, 255},   /* Noon */
    {130, 120, 110, 255},   /* Afternoon */
    {100, 70, 60, 255},     /* Dusk */
    {40, 40, 60, 255}       /* Night */
};

/* Rain spawn rates per intensity */
static const f32 sRainSpawnRates[5] = {
    0.0f,   /* None */
    2.0f,   /* Light */
    5.0f,   /* Medium */
    10.0f,  /* Heavy */
    15.0f   /* Storm */
};

/* Fog distances per weather type */
static const f32 sFogNear[NUM_WEATHER_TYPES] = {
    5000.0f,    /* Clear */
    3000.0f,    /* Cloudy */
    2000.0f,    /* Overcast */
    1500.0f,    /* Rain */
    1000.0f,    /* Storm */
    300.0f      /* Fog */
};

static const f32 sFogFar[NUM_WEATHER_TYPES] = {
    10000.0f,   /* Clear */
    6000.0f,    /* Cloudy */
    4000.0f,    /* Overcast */
    3000.0f,    /* Rain */
    2000.0f,    /* Storm */
    800.0f      /* Fog */
};

/* -------------------------------------------------------------------------- */
/* Initialization                                                              */
/* -------------------------------------------------------------------------- */

/**
 * Initialize weather system
 */
void weather_init(void) {
    s32 i;

    /* Clear all state */
    for (i = 0; i < (s32)sizeof(WeatherState); i++) {
        ((u8*)&gWeather)[i] = 0;
    }

    /* Set defaults */
    gWeather.weather_type = WEATHER_TYPE_CLEAR;
    gWeather.time_of_day = TIME_OF_DAY_NOON;
    gWeather.rain_intensity = RAIN_NONE;

    /* Default fog */
    gWeather.fog.enabled = 0;
    gWeather.fog.mode = FOG_MODE_LINEAR;
    gWeather.fog.near = 2000.0f;
    gWeather.fog.far = 5000.0f;
    gWeather.fog.color.r = 180;
    gWeather.fog.color.g = 190;
    gWeather.fog.color.b = 200;
    gWeather.fog.color.a = 255;

    /* Default sky */
    gWeather.sky.sky_top = sSkyTopColors[TIME_OF_DAY_NOON];
    gWeather.sky.sky_bottom = sSkyBottomColors[TIME_OF_DAY_NOON];
    gWeather.sky.sun_dir[0] = 0.5f;
    gWeather.sky.sun_dir[1] = 1.0f;
    gWeather.sky.sun_dir[2] = 0.3f;
    gWeather.sky.sun_intensity = 1.0f;
    gWeather.sky.cloud_cover = 0.0f;
    gWeather.sky.cloud_speed = 0.01f;

    /* Default lighting */
    gWeather.light.ambient = sAmbientColors[TIME_OF_DAY_NOON];
    gWeather.light.diffuse.r = 255;
    gWeather.light.diffuse.g = 250;
    gWeather.light.diffuse.b = 240;
    gWeather.light.diffuse.a = 255;
    gWeather.light.light_dir[0] = 0.5f;
    gWeather.light.light_dir[1] = 1.0f;
    gWeather.light.light_dir[2] = 0.3f;
    gWeather.light.shadow_intensity = 0.5f;

    /* Default wind */
    gWeather.wind.direction[0] = 1.0f;
    gWeather.wind.direction[1] = 0.0f;
    gWeather.wind.direction[2] = 0.0f;
    gWeather.wind.speed = 0.0f;

    /* Day cycle */
    gWeather.day_cycle_time = 0.5f;  /* Noon */
    gWeather.day_cycle_speed = 0.0f;
    gWeather.day_cycle_enabled = 0;
}

/**
 * Reset weather to defaults
 */
void weather_reset(void) {
    weather_init();
}

/* -------------------------------------------------------------------------- */
/* Per-frame Update                                                            */
/* -------------------------------------------------------------------------- */

/**
 * Main weather update
 */
void weather_update(void) {
    weather_update_day_cycle();
    weather_update_transition();
    weather_update_rain();
    weather_update_particles();
    weather_update_lightning();
    weather_update_wetness();
}

/**
 * Update rain particles
 */
void weather_update_rain(void) {
    s32 i;
    RainDrop *drop;
    f32 ground_y = 0.0f;  /* Would come from collision */

    /* Spawn new drops */
    if (gWeather.rain_intensity > RAIN_NONE) {
        gWeather.rain_spawn_accum += gWeather.rain_spawn_rate;
        while (gWeather.rain_spawn_accum >= 1.0f) {
            weather_spawn_rain(1);
            gWeather.rain_spawn_accum -= 1.0f;
        }
    }

    /* Update existing drops */
    gWeather.rain_count = 0;
    for (i = 0; i < MAX_RAIN_DROPS; i++) {
        drop = &gWeather.rain[i];
        if (!drop->active) {
            continue;
        }

        /* Apply velocity */
        drop->pos[0] += drop->vel[0];
        drop->pos[1] += drop->vel[1];
        drop->pos[2] += drop->vel[2];

        /* Apply wind */
        drop->pos[0] += gWeather.wind.direction[0] * gWeather.wind.speed * 0.1f;
        drop->pos[2] += gWeather.wind.direction[2] * gWeather.wind.speed * 0.1f;

        /* Check if hit ground */
        if (drop->pos[1] < ground_y) {
            if (drop->splash) {
                weather_spawn_splash(drop->pos, 0.5f);
            }
            drop->active = 0;
        } else {
            gWeather.rain_count++;
        }
    }
}

/**
 * Update general particles
 */
void weather_update_particles(void) {
    s32 i;
    WeatherParticle *p;

    gWeather.particle_count = 0;
    for (i = 0; i < MAX_PARTICLES; i++) {
        p = &gWeather.particles[i];
        if (!p->active) {
            continue;
        }

        /* Update position */
        p->pos[0] += p->vel[0];
        p->pos[1] += p->vel[1];
        p->pos[2] += p->vel[2];

        /* Apply gravity for some types */
        if (p->type == PARTICLE_SPLASH || p->type == PARTICLE_DUST) {
            p->vel[1] -= 0.05f;
        }

        /* Update rotation */
        p->rotation += p->rot_speed;

        /* Decrease life */
        p->life -= 1.0f;
        if (p->life <= 0.0f) {
            p->active = 0;
        } else {
            gWeather.particle_count++;
        }
    }
}

/**
 * Update lightning
 */
void weather_update_lightning(void) {
    s32 i;
    Lightning *l;

    /* Update active lightning */
    for (i = 0; i < MAX_LIGHTNING; i++) {
        l = &gWeather.lightning[i];
        if (!l->active) {
            continue;
        }

        l->timer += 1.0f;
        if (l->timer >= l->duration) {
            l->active = 0;
        } else {
            /* Fade intensity */
            l->intensity = 1.0f - (l->timer / l->duration);
        }

        /* Check thunder delay */
        if (l->thunder_delay > 0) {
            l->thunder_delay--;
            if (l->thunder_delay == 0) {
                sound_play(SND_THUNDER);
            }
        }
    }

    /* Spawn new lightning in storms */
    if (gWeather.weather_type == WEATHER_TYPE_STORM) {
        if (gWeather.next_lightning > 0) {
            gWeather.next_lightning--;
        } else {
            weather_trigger_lightning();
            gWeather.next_lightning = 180 + (rand() % 300);  /* 3-8 seconds */
        }
    }
}

/**
 * Update weather transition
 */
void weather_update_transition(void) {
    f32 t;

    if (!gWeather.transitioning) {
        return;
    }

    gWeather.transition_progress += 1.0f / (gWeather.transition_time * 60.0f);

    if (gWeather.transition_progress >= 1.0f) {
        gWeather.transition_progress = 1.0f;
        gWeather.transitioning = 0;
        gWeather.weather_type = gWeather.target_weather;
        gWeather.time_of_day = gWeather.target_time;
    }

    t = gWeather.transition_progress;

    /* Interpolate fog */
    gWeather.fog.near = sFogNear[gWeather.weather_type] * (1.0f - t) +
                        sFogNear[gWeather.target_weather] * t;
    gWeather.fog.far = sFogFar[gWeather.weather_type] * (1.0f - t) +
                       sFogFar[gWeather.target_weather] * t;
}

/**
 * Update day cycle
 */
void weather_update_day_cycle(void) {
    if (!gWeather.day_cycle_enabled) {
        return;
    }

    gWeather.day_cycle_time += gWeather.day_cycle_speed / 60.0f;
    if (gWeather.day_cycle_time >= 1.0f) {
        gWeather.day_cycle_time -= 1.0f;
    }

    /* Update time of day based on cycle position */
    if (gWeather.day_cycle_time < 0.1f) {
        gWeather.time_of_day = TIME_OF_DAY_NIGHT;
    } else if (gWeather.day_cycle_time < 0.2f) {
        gWeather.time_of_day = TIME_OF_DAY_DAWN;
    } else if (gWeather.day_cycle_time < 0.35f) {
        gWeather.time_of_day = TIME_OF_DAY_MORNING;
    } else if (gWeather.day_cycle_time < 0.65f) {
        gWeather.time_of_day = TIME_OF_DAY_NOON;
    } else if (gWeather.day_cycle_time < 0.75f) {
        gWeather.time_of_day = TIME_OF_DAY_AFTERNOON;
    } else if (gWeather.day_cycle_time < 0.85f) {
        gWeather.time_of_day = TIME_OF_DAY_DUSK;
    } else {
        gWeather.time_of_day = TIME_OF_DAY_NIGHT;
    }

    /* Update sky and lighting */
    gWeather.sky.sky_top = sSkyTopColors[gWeather.time_of_day];
    gWeather.sky.sky_bottom = sSkyBottomColors[gWeather.time_of_day];
    gWeather.light.ambient = sAmbientColors[gWeather.time_of_day];
}

/* -------------------------------------------------------------------------- */
/* Weather Control                                                             */
/* -------------------------------------------------------------------------- */

/**
 * Set weather type (with transition)
 */
void weather_set(s32 type) {
    weather_transition_to(type, 5.0f);  /* 5 second transition */
}

/**
 * Set weather immediately
 */
void weather_set_immediate(s32 type) {
    if (type < 0 || type >= NUM_WEATHER_TYPES) {
        return;
    }

    gWeather.weather_type = (u8)type;
    gWeather.transitioning = 0;

    /* Apply fog settings */
    gWeather.fog.near = sFogNear[type];
    gWeather.fog.far = sFogFar[type];
    gWeather.fog.enabled = (type >= WEATHER_TYPE_RAIN);

    /* Set rain */
    switch (type) {
        case WEATHER_TYPE_RAIN:
            weather_set_rain(RAIN_MEDIUM);
            break;
        case WEATHER_TYPE_STORM:
            weather_set_rain(RAIN_HEAVY);
            break;
        default:
            weather_set_rain(RAIN_NONE);
            break;
    }

    /* Set cloud cover */
    switch (type) {
        case WEATHER_TYPE_CLEAR:
            gWeather.sky.cloud_cover = 0.0f;
            break;
        case WEATHER_TYPE_CLOUDY:
            gWeather.sky.cloud_cover = 0.4f;
            break;
        case WEATHER_TYPE_OVERCAST:
            gWeather.sky.cloud_cover = 0.8f;
            break;
        case WEATHER_TYPE_RAIN:
        case WEATHER_TYPE_STORM:
        case WEATHER_TYPE_FOG:
            gWeather.sky.cloud_cover = 1.0f;
            break;
    }
}

/**
 * Transition to weather type
 */
void weather_transition_to(s32 type, f32 duration) {
    if (type < 0 || type >= NUM_WEATHER_TYPES) {
        return;
    }

    gWeather.target_weather = (u8)type;
    gWeather.transition_time = duration;
    gWeather.transition_progress = 0.0f;
    gWeather.transitioning = 1;
}

/**
 * Get current weather type
 */
s32 weather_get_type(void) {
    return gWeather.weather_type;
}

/* -------------------------------------------------------------------------- */
/* Time of Day                                                                 */
/* -------------------------------------------------------------------------- */

void weather_set_time(s32 time) {
    weather_transition_time(time, 3.0f);
}

void weather_set_time_immediate(s32 time) {
    if (time < 0 || time >= NUM_TIMES_OF_DAY) {
        return;
    }

    gWeather.time_of_day = (u8)time;
    gWeather.sky.sky_top = sSkyTopColors[time];
    gWeather.sky.sky_bottom = sSkyBottomColors[time];
    gWeather.light.ambient = sAmbientColors[time];

    /* Update day cycle position */
    gWeather.day_cycle_time = (f32)time / (f32)NUM_TIMES_OF_DAY;
}

void weather_transition_time(s32 time, f32 duration) {
    if (time < 0 || time >= NUM_TIMES_OF_DAY) {
        return;
    }

    gWeather.target_time = (u8)time;
    gWeather.transition_time = duration;
    gWeather.transition_progress = 0.0f;
    gWeather.transitioning = 1;
}

s32 weather_get_time(void) {
    return gWeather.time_of_day;
}

void weather_set_day_cycle(u8 enabled, f32 speed) {
    gWeather.day_cycle_enabled = enabled;
    gWeather.day_cycle_speed = speed;
}

/* -------------------------------------------------------------------------- */
/* Rain Control                                                                */
/* -------------------------------------------------------------------------- */

void weather_set_rain(s32 intensity) {
    if (intensity < RAIN_NONE || intensity > RAIN_STORM) {
        return;
    }

    gWeather.rain_intensity = (u8)intensity;
    gWeather.rain_spawn_rate = sRainSpawnRates[intensity];
}

void weather_spawn_rain(s32 count) {
    s32 i, spawned = 0;
    RainDrop *drop;
    f32 camera_x = 0.0f, camera_z = 0.0f;  /* Would get from camera */

    for (i = 0; i < MAX_RAIN_DROPS && spawned < count; i++) {
        drop = &gWeather.rain[i];
        if (drop->active) {
            continue;
        }

        drop->active = 1;
        drop->splash = 1;

        /* Spawn around camera */
        drop->pos[0] = camera_x + RANDF_RANGE(-200.0f, 200.0f);
        drop->pos[1] = 100.0f + RANDF() * 50.0f;
        drop->pos[2] = camera_z + RANDF_RANGE(-200.0f, 200.0f);

        /* Fall velocity */
        drop->vel[0] = 0.0f;
        drop->vel[1] = -3.0f - RANDF() * 2.0f;
        drop->vel[2] = 0.0f;

        drop->length = 1.0f + RANDF() * 0.5f;

        spawned++;
    }
}

void weather_clear_rain(void) {
    s32 i;
    for (i = 0; i < MAX_RAIN_DROPS; i++) {
        gWeather.rain[i].active = 0;
    }
    gWeather.rain_count = 0;
}

s32 weather_get_rain_intensity(void) {
    return gWeather.rain_intensity;
}

/* -------------------------------------------------------------------------- */
/* Fog Control                                                                 */
/* -------------------------------------------------------------------------- */

void weather_set_fog(u8 enabled, f32 near, f32 far) {
    gWeather.fog.enabled = enabled;
    gWeather.fog.near = near;
    gWeather.fog.far = far;
}

void weather_set_fog_color(u8 r, u8 g, u8 b) {
    gWeather.fog.color.r = r;
    gWeather.fog.color.g = g;
    gWeather.fog.color.b = b;
}

void weather_set_fog_density(f32 density) {
    gWeather.fog.density = density;
}

FogSettings* weather_get_fog(void) {
    return &gWeather.fog;
}

/* -------------------------------------------------------------------------- */
/* Wind Control                                                                */
/* -------------------------------------------------------------------------- */

void weather_set_wind(f32 speed, f32 dir_x, f32 dir_z) {
    f32 len;

    gWeather.wind.speed = speed;
    gWeather.wind.direction[0] = dir_x;
    gWeather.wind.direction[1] = 0.0f;
    gWeather.wind.direction[2] = dir_z;

    /* Normalize direction */
    len = sqrtf(dir_x * dir_x + dir_z * dir_z);
    if (len > 0.001f) {
        gWeather.wind.direction[0] /= len;
        gWeather.wind.direction[2] /= len;
    }
}

void weather_set_wind_gusts(f32 strength, f32 frequency) {
    gWeather.wind.gust_strength = strength;
    gWeather.wind.gust_frequency = frequency;
}

void weather_get_wind(f32 *dir, f32 *speed) {
    if (dir != NULL) {
        dir[0] = gWeather.wind.direction[0];
        dir[1] = gWeather.wind.direction[1];
        dir[2] = gWeather.wind.direction[2];
    }
    if (speed != NULL) {
        *speed = gWeather.wind.speed;
    }
}

/* -------------------------------------------------------------------------- */
/* Lighting                                                                    */
/* -------------------------------------------------------------------------- */

void weather_set_ambient(u8 r, u8 g, u8 b) {
    gWeather.light.ambient.r = r;
    gWeather.light.ambient.g = g;
    gWeather.light.ambient.b = b;
}

void weather_set_sun_direction(f32 x, f32 y, f32 z) {
    gWeather.sky.sun_dir[0] = x;
    gWeather.sky.sun_dir[1] = y;
    gWeather.sky.sun_dir[2] = z;
    gWeather.light.light_dir[0] = x;
    gWeather.light.light_dir[1] = y;
    gWeather.light.light_dir[2] = z;
}

void weather_set_sun_color(u8 r, u8 g, u8 b) {
    gWeather.sky.sun_color.r = r;
    gWeather.sky.sun_color.g = g;
    gWeather.sky.sun_color.b = b;
    gWeather.light.diffuse.r = r;
    gWeather.light.diffuse.g = g;
    gWeather.light.diffuse.b = b;
}

LightSettings* weather_get_lighting(void) {
    return &gWeather.light;
}

/* -------------------------------------------------------------------------- */
/* Sky                                                                         */
/* -------------------------------------------------------------------------- */

void weather_set_sky_colors(WeatherColor *top, WeatherColor *bottom) {
    if (top != NULL) {
        gWeather.sky.sky_top = *top;
    }
    if (bottom != NULL) {
        gWeather.sky.sky_bottom = *bottom;
    }
}

void weather_set_cloud_cover(f32 cover) {
    if (cover < 0.0f) cover = 0.0f;
    if (cover > 1.0f) cover = 1.0f;
    gWeather.sky.cloud_cover = cover;
}

SkySettings* weather_get_sky(void) {
    return &gWeather.sky;
}

/* -------------------------------------------------------------------------- */
/* Particles                                                                   */
/* -------------------------------------------------------------------------- */

void weather_spawn_particle(s32 type, f32 *pos, f32 *vel, f32 size, f32 life) {
    s32 i;
    WeatherParticle *p;

    for (i = 0; i < MAX_PARTICLES; i++) {
        p = &gWeather.particles[i];
        if (p->active) {
            continue;
        }

        p->active = 1;
        p->type = (u8)type;
        p->pos[0] = pos[0];
        p->pos[1] = pos[1];
        p->pos[2] = pos[2];
        p->vel[0] = vel[0];
        p->vel[1] = vel[1];
        p->vel[2] = vel[2];
        p->size = size;
        p->life = life;
        p->max_life = life;
        p->rotation = RANDF() * 6.28f;
        p->rot_speed = RANDF_RANGE(-0.1f, 0.1f);
        p->color.r = p->color.g = p->color.b = p->color.a = 255;
        break;
    }
}

void weather_spawn_splash(f32 *pos, f32 size) {
    f32 vel[3];
    s32 i;

    for (i = 0; i < 4; i++) {
        vel[0] = RANDF_RANGE(-1.0f, 1.0f);
        vel[1] = RANDF_RANGE(0.5f, 1.5f);
        vel[2] = RANDF_RANGE(-1.0f, 1.0f);
        weather_spawn_particle(PARTICLE_SPLASH, pos, vel, size * 0.3f, 20.0f);
    }
}

void weather_spawn_spray(f32 *pos, f32 *vel, s32 count) {
    s32 i;
    f32 spray_vel[3];

    for (i = 0; i < count; i++) {
        spray_vel[0] = vel[0] + RANDF_RANGE(-0.5f, 0.5f);
        spray_vel[1] = vel[1] + RANDF_RANGE(0.0f, 0.5f);
        spray_vel[2] = vel[2] + RANDF_RANGE(-0.5f, 0.5f);
        weather_spawn_particle(PARTICLE_SPRAY, pos, spray_vel, 0.2f, 30.0f);
    }
}

void weather_spawn_dust(f32 *pos, f32 size) {
    f32 vel[3] = {0.0f, 0.3f, 0.0f};
    weather_spawn_particle(PARTICLE_DUST, pos, vel, size, 60.0f);
}

void weather_clear_particles(void) {
    s32 i;
    for (i = 0; i < MAX_PARTICLES; i++) {
        gWeather.particles[i].active = 0;
    }
    gWeather.particle_count = 0;
}

/* -------------------------------------------------------------------------- */
/* Lightning                                                                   */
/* -------------------------------------------------------------------------- */

void weather_trigger_lightning(void) {
    s32 i;
    Lightning *l;

    for (i = 0; i < MAX_LIGHTNING; i++) {
        l = &gWeather.lightning[i];
        if (l->active) {
            continue;
        }

        l->active = 1;
        l->intensity = 1.0f;
        l->duration = 10.0f + RANDF() * 10.0f;
        l->timer = 0.0f;
        l->thunder_delay = 30 + (rand() % 90);  /* 0.5-2 seconds */
        break;
    }
}

void weather_flash_screen(f32 intensity, f32 duration) {
    Lightning *l;

    /* Use first lightning slot for screen flash */
    l = &gWeather.lightning[0];
    l->active = 1;
    l->intensity = intensity;
    l->duration = duration;
    l->timer = 0.0f;
    l->thunder_delay = 0;
}

/* -------------------------------------------------------------------------- */
/* Puddles and Wetness                                                         */
/* -------------------------------------------------------------------------- */

void weather_add_puddle(f32 *pos, f32 radius) {
    s32 i;
    Puddle *p;

    for (i = 0; i < MAX_PUDDLES; i++) {
        p = &gWeather.puddles[i];
        if (p->active) {
            continue;
        }

        p->active = 1;
        p->pos[0] = pos[0];
        p->pos[1] = pos[1];
        p->pos[2] = pos[2];
        p->radius = radius;
        p->ripple = 0.0f;
        p->opacity = 0.6f;
        gWeather.puddle_count++;
        break;
    }
}

void weather_clear_puddles(void) {
    s32 i;
    for (i = 0; i < MAX_PUDDLES; i++) {
        gWeather.puddles[i].active = 0;
    }
    gWeather.puddle_count = 0;
}

void weather_update_wetness(void) {
    /* Increase wetness while raining */
    if (gWeather.rain_intensity > RAIN_NONE) {
        gWeather.wetness += 0.001f * gWeather.rain_intensity;
        if (gWeather.wetness > 1.0f) {
            gWeather.wetness = 1.0f;
        }
    } else {
        /* Dry out slowly */
        gWeather.wetness -= 0.0002f;
        if (gWeather.wetness < 0.0f) {
            gWeather.wetness = 0.0f;
        }
    }
}

f32 weather_get_wetness(void) {
    return gWeather.wetness;
}

/* -------------------------------------------------------------------------- */
/* Rendering                                                                   */
/* -------------------------------------------------------------------------- */

void weather_apply_fog(void) {
    /* Would set RDP fog registers */
}

void weather_apply_lighting(void) {
    /* Would set lighting state */
}

void weather_draw_rain(void) {
    /* Would draw rain particles */
}

void weather_draw_particles(void) {
    /* Would draw general particles */
}

void weather_draw_sky(void) {
    /* Would draw sky gradient and clouds */
}

void weather_draw_lightning_flash(void) {
    /* Would draw screen flash overlay */
}

/* -------------------------------------------------------------------------- */
/* Queries                                                                     */
/* -------------------------------------------------------------------------- */

s32 weather_is_raining(void) {
    return (gWeather.rain_intensity > RAIN_NONE);
}

s32 weather_is_night(void) {
    return (gWeather.time_of_day == TIME_OF_DAY_NIGHT);
}

s32 weather_is_foggy(void) {
    return (gWeather.weather_type == WEATHER_TYPE_FOG ||
            gWeather.fog.enabled);
}

s32 weather_is_transitioning(void) {
    return gWeather.transitioning;
}

f32 weather_get_visibility(void) {
    return gWeather.fog.far;
}

/* -------------------------------------------------------------------------- */
/* Presets                                                                     */
/* -------------------------------------------------------------------------- */

void weather_apply_preset_clear(void) {
    weather_set_immediate(WEATHER_TYPE_CLEAR);
    weather_set_time_immediate(TIME_OF_DAY_NOON);
    weather_set_wind(0.0f, 1.0f, 0.0f);
}

void weather_apply_preset_rain(void) {
    weather_set_immediate(WEATHER_TYPE_RAIN);
    weather_set_wind(5.0f, 1.0f, 0.5f);
}

void weather_apply_preset_storm(void) {
    weather_set_immediate(WEATHER_TYPE_STORM);
    weather_set_wind(15.0f, 1.0f, 0.3f);
    weather_set_wind_gusts(10.0f, 0.1f);
}

void weather_apply_preset_fog(void) {
    weather_set_immediate(WEATHER_TYPE_FOG);
    weather_set_fog(1, 100.0f, 500.0f);
}

void weather_apply_preset_night(void) {
    weather_set_immediate(WEATHER_TYPE_CLEAR);
    weather_set_time_immediate(TIME_OF_DAY_NIGHT);
}
