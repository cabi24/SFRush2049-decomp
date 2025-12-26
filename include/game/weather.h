/**
 * weather.h - Weather and environment effects for Rush 2049 N64
 *
 * Handles dynamic weather effects including rain, fog, time of day,
 * and environmental particles.
 */

#ifndef WEATHER_H
#define WEATHER_H

#include "types.h"

/* Weather types */
#define WEATHER_TYPE_CLEAR      0
#define WEATHER_TYPE_CLOUDY     1
#define WEATHER_TYPE_OVERCAST   2
#define WEATHER_TYPE_RAIN       3
#define WEATHER_TYPE_STORM      4
#define WEATHER_TYPE_FOG        5
#define NUM_WEATHER_TYPES       6

/* Time of day */
#define TIME_OF_DAY_DAWN        0
#define TIME_OF_DAY_MORNING     1
#define TIME_OF_DAY_NOON        2
#define TIME_OF_DAY_AFTERNOON   3
#define TIME_OF_DAY_DUSK        4
#define TIME_OF_DAY_NIGHT       5
#define NUM_TIMES_OF_DAY        6

/* Particle types */
#define PARTICLE_RAIN           0
#define PARTICLE_SPLASH         1
#define PARTICLE_SPRAY          2
#define PARTICLE_DUST           3
#define PARTICLE_SMOKE          4
#define PARTICLE_SPARK          5
#define PARTICLE_LEAF           6
#define PARTICLE_PAPER          7
#define NUM_PARTICLE_TYPES      8

/* Effect limits */
#define MAX_RAIN_DROPS          200     /* Max raindrops */
#define MAX_PARTICLES           64      /* General particles */
#define MAX_PUDDLES             16      /* Puddle reflections */
#define MAX_LIGHTNING           4       /* Lightning flashes */

/* Rain intensity levels */
#define RAIN_NONE               0
#define RAIN_LIGHT              1
#define RAIN_MEDIUM             2
#define RAIN_HEAVY              3
#define RAIN_STORM              4

/* Fog modes */
#define FOG_MODE_NONE           0
#define FOG_MODE_LINEAR         1
#define FOG_MODE_EXPONENTIAL    2

/* Color structure */
typedef struct WeatherColor {
    u8      r, g, b, a;
} WeatherColor;

/* Raindrop particle */
typedef struct RainDrop {
    f32     pos[3];             /* Current position */
    f32     vel[3];             /* Velocity */
    f32     length;             /* Streak length */
    u8      active;
    u8      splash;             /* Create splash on ground */
    u8      pad[2];
} RainDrop;

/* General particle */
typedef struct WeatherParticle {
    f32     pos[3];             /* Position */
    f32     vel[3];             /* Velocity */
    f32     size;               /* Particle size */
    f32     life;               /* Remaining life */
    f32     max_life;           /* Initial life */
    f32     rotation;           /* Rotation angle */
    f32     rot_speed;          /* Rotation speed */
    u8      type;               /* PARTICLE_* type */
    u8      active;
    u8      pad[2];
    WeatherColor color;         /* Particle color */
} WeatherParticle;

/* Puddle (for reflections) */
typedef struct Puddle {
    f32     pos[3];             /* Center position */
    f32     radius;             /* Puddle size */
    f32     ripple;             /* Ripple animation */
    f32     opacity;            /* Transparency */
    u8      active;
    u8      pad[3];
} Puddle;

/* Lightning flash */
typedef struct Lightning {
    f32     pos[3];             /* Origin position */
    f32     intensity;          /* Current brightness */
    f32     duration;           /* Flash duration */
    f32     timer;              /* Current timer */
    u8      active;
    u8      type;               /* Fork or sheet */
    u8      pad[2];
    u32     thunder_delay;      /* Frames until thunder */
} Lightning;

/* Fog settings */
typedef struct FogSettings {
    u8      enabled;
    u8      mode;               /* FOG_MODE_* */
    u8      pad[2];
    f32     near;               /* Start distance */
    f32     far;                /* End distance */
    f32     density;            /* For exponential fog */
    WeatherColor color;
} FogSettings;

/* Sky settings */
typedef struct SkySettings {
    WeatherColor sky_top;       /* Top of sky gradient */
    WeatherColor sky_bottom;    /* Horizon color */
    WeatherColor sun_color;     /* Sun/moon color */
    f32     sun_dir[3];         /* Sun direction */
    f32     sun_intensity;      /* Sun brightness */
    f32     cloud_cover;        /* 0.0 = clear, 1.0 = overcast */
    f32     cloud_speed;        /* Cloud movement speed */
    u8      stars_visible;      /* Show stars (night) */
    u8      moon_visible;       /* Show moon */
    u8      pad[2];
} SkySettings;

/* Lighting settings */
typedef struct LightSettings {
    WeatherColor ambient;       /* Ambient light color */
    WeatherColor diffuse;       /* Main light color */
    f32     light_dir[3];       /* Main light direction */
    f32     shadow_intensity;   /* Shadow darkness */
} LightSettings;

/* Wind settings */
typedef struct WindSettings {
    f32     direction[3];       /* Wind direction vector */
    f32     speed;              /* Wind speed */
    f32     gust_strength;      /* Gust intensity */
    f32     gust_frequency;     /* Gust frequency */
    f32     turbulence;         /* Random variation */
} WindSettings;

/* Weather state */
typedef struct WeatherState {
    /* Current conditions */
    u8      weather_type;       /* WEATHER_TYPE_* */
    u8      time_of_day;        /* TIME_OF_DAY_* */
    u8      rain_intensity;     /* RAIN_* level */
    u8      transitioning;      /* Weather changing */

    /* Transition */
    f32     transition_time;    /* Total transition time */
    f32     transition_progress;/* Current progress 0-1 */
    u8      target_weather;     /* Target weather type */
    u8      target_time;        /* Target time of day */
    u8      pad[2];

    /* Settings */
    FogSettings fog;
    SkySettings sky;
    LightSettings light;
    WindSettings wind;

    /* Rain system */
    RainDrop rain[MAX_RAIN_DROPS];
    s32     rain_count;
    f32     rain_spawn_rate;    /* Drops per frame */
    f32     rain_spawn_accum;   /* Accumulator */

    /* Particles */
    WeatherParticle particles[MAX_PARTICLES];
    s32     particle_count;

    /* Puddles */
    Puddle puddles[MAX_PUDDLES];
    s32     puddle_count;
    f32     wetness;            /* Ground wetness 0-1 */

    /* Lightning */
    Lightning lightning[MAX_LIGHTNING];
    u32     next_lightning;     /* Frames until next flash */
    u32     thunder_queued;     /* Thunder sounds queued */

    /* Time */
    f32     day_cycle_time;     /* Current time in day (0-1) */
    f32     day_cycle_speed;    /* Time progression speed */
    u8      day_cycle_enabled;  /* Auto time progression */
    u8      pad2[3];

} WeatherState;

/* Global weather state */
extern WeatherState gWeather;

/* Initialization */
void weather_init(void);
void weather_reset(void);

/* Per-frame update */
void weather_update(void);
void weather_update_rain(void);
void weather_update_particles(void);
void weather_update_lightning(void);
void weather_update_transition(void);
void weather_update_day_cycle(void);

/* Weather control */
void weather_set(s32 type);
void weather_set_immediate(s32 type);
void weather_transition_to(s32 type, f32 duration);
s32 weather_get_type(void);

/* Time of day */
void weather_set_time(s32 time);
void weather_set_time_immediate(s32 time);
void weather_transition_time(s32 time, f32 duration);
s32 weather_get_time(void);
void weather_set_day_cycle(u8 enabled, f32 speed);

/* Rain control */
void weather_set_rain(s32 intensity);
void weather_spawn_rain(s32 count);
void weather_clear_rain(void);
s32 weather_get_rain_intensity(void);

/* Fog control */
void weather_set_fog(u8 enabled, f32 near, f32 far);
void weather_set_fog_color(u8 r, u8 g, u8 b);
void weather_set_fog_density(f32 density);
FogSettings* weather_get_fog(void);

/* Wind control */
void weather_set_wind(f32 speed, f32 dir_x, f32 dir_z);
void weather_set_wind_gusts(f32 strength, f32 frequency);
void weather_get_wind(f32 *dir, f32 *speed);

/* Lighting */
void weather_set_ambient(u8 r, u8 g, u8 b);
void weather_set_sun_direction(f32 x, f32 y, f32 z);
void weather_set_sun_color(u8 r, u8 g, u8 b);
LightSettings* weather_get_lighting(void);

/* Sky */
void weather_set_sky_colors(WeatherColor *top, WeatherColor *bottom);
void weather_set_cloud_cover(f32 cover);
SkySettings* weather_get_sky(void);

/* Particles */
void weather_spawn_particle(s32 type, f32 *pos, f32 *vel, f32 size, f32 life);
void weather_spawn_splash(f32 *pos, f32 size);
void weather_spawn_spray(f32 *pos, f32 *vel, s32 count);
void weather_spawn_dust(f32 *pos, f32 size);
void weather_clear_particles(void);

/* Lightning */
void weather_trigger_lightning(void);
void weather_flash_screen(f32 intensity, f32 duration);

/* Puddles */
void weather_add_puddle(f32 *pos, f32 radius);
void weather_clear_puddles(void);
void weather_update_wetness(void);
f32 weather_get_wetness(void);

/* Rendering */
void weather_apply_fog(void);
void weather_apply_lighting(void);
void weather_draw_rain(void);
void weather_draw_particles(void);
void weather_draw_sky(void);
void weather_draw_lightning_flash(void);

/* Queries */
s32 weather_is_raining(void);
s32 weather_is_night(void);
s32 weather_is_foggy(void);
s32 weather_is_transitioning(void);
f32 weather_get_visibility(void);

/* Presets */
void weather_apply_preset_clear(void);
void weather_apply_preset_rain(void);
void weather_apply_preset_storm(void);
void weather_apply_preset_fog(void);
void weather_apply_preset_night(void);

#endif /* WEATHER_H */
