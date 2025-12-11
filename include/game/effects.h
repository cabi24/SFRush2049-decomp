/**
 * effects.h - Visual effects and particle system for Rush 2049 N64
 *
 * Handles tire smoke, sparks, dust, water splash, and other particles.
 */

#ifndef EFFECTS_H
#define EFFECTS_H

#include "types.h"

/* Maximum particles */
#define MAX_PARTICLES       256
#define MAX_PARTICLE_TYPES  16

/* Particle types */
#define PARTICLE_NONE       0
#define PARTICLE_SMOKE      1   /* Tire smoke */
#define PARTICLE_DUST       2   /* Dust from dirt/grass */
#define PARTICLE_SPARK      3   /* Collision sparks */
#define PARTICLE_WATER      4   /* Water splash */
#define PARTICLE_FIRE       5   /* Fire/explosion */
#define PARTICLE_DEBRIS     6   /* Car debris */
#define PARTICLE_SKID       7   /* Skid mark particles */
#define PARTICLE_EXHAUST    8   /* Exhaust smoke */
#define PARTICLE_BOOST      9   /* Boost effect */
#define PARTICLE_TRAIL      10  /* Wheel trail */

/* Particle flags */
#define PARTICLE_FLAG_ACTIVE    0x01
#define PARTICLE_FLAG_FADE      0x02    /* Fading out */
#define PARTICLE_FLAG_GROW      0x04    /* Growing */
#define PARTICLE_FLAG_SHRINK    0x08    /* Shrinking */
#define PARTICLE_FLAG_GRAVITY   0x10    /* Affected by gravity */
#define PARTICLE_FLAG_COLLIDE   0x20    /* Collides with ground */
#define PARTICLE_FLAG_TRAIL     0x40    /* Leaves trail */

/* Effect emitter types */
#define EMITTER_NONE        0
#define EMITTER_TIRE        1   /* Attached to tire */
#define EMITTER_EXHAUST     2   /* Attached to exhaust */
#define EMITTER_POINT       3   /* Fixed point emitter */
#define EMITTER_BURST       4   /* One-shot burst */

/* Colors (RGBA8888) */
#define COLOR_SMOKE_WHITE   0xFFFFFF80
#define COLOR_SMOKE_BLACK   0x40404080
#define COLOR_DUST_BROWN    0xA0805080
#define COLOR_DUST_YELLOW   0xC0B06080
#define COLOR_SPARK_ORANGE  0xFF8000FF
#define COLOR_SPARK_WHITE   0xFFFFFFFF
#define COLOR_WATER_BLUE    0x8080FFFF
#define COLOR_FIRE_ORANGE   0xFF6000FF
#define COLOR_FIRE_RED      0xFF2000FF

/* Single particle */
typedef struct Particle {
    f32     pos[3];         /* World position */
    f32     vel[3];         /* Velocity */
    f32     acc[3];         /* Acceleration */
    f32     size;           /* Current size */
    f32     size_vel;       /* Size change rate */
    f32     rotation;       /* Rotation angle */
    f32     rot_vel;        /* Rotation velocity */
    u32     color;          /* Current color (RGBA) */
    u32     start_color;    /* Initial color */
    u32     end_color;      /* Final color */
    u16     life;           /* Remaining lifetime (frames) */
    u16     max_life;       /* Initial lifetime */
    u8      type;           /* Particle type */
    u8      flags;          /* Particle flags */
    u8      alpha;          /* Current alpha */
    u8      texture;        /* Texture ID */
} Particle;

/* Particle emitter */
typedef struct Emitter {
    f32     pos[3];         /* Emitter position */
    f32     dir[3];         /* Emission direction */
    f32     spread;         /* Spread angle (radians) */
    f32     speed_min;      /* Min initial speed */
    f32     speed_max;      /* Max initial speed */
    f32     size_min;       /* Min particle size */
    f32     size_max;       /* Max particle size */
    u16     life_min;       /* Min lifetime (frames) */
    u16     life_max;       /* Max lifetime (frames) */
    u16     rate;           /* Particles per frame (fixed point 8.8) */
    u16     accumulator;    /* Rate accumulator */
    u32     start_color;    /* Starting color */
    u32     end_color;      /* Ending color */
    u8      type;           /* Emitter type */
    u8      particle_type;  /* Type of particles to emit */
    u8      active;         /* Is emitter active */
    u8      flags;          /* Emitter flags */
    s32     car_index;      /* Attached car (-1 if none) */
    s32     attach_point;   /* Attachment point on car */
} Emitter;

/* Skid mark segment */
typedef struct SkidMark {
    f32     start_pos[3];   /* Start position */
    f32     end_pos[3];     /* End position */
    f32     width;          /* Mark width */
    u8      alpha;          /* Visibility */
    u8      surface;        /* Surface type */
    u8      active;         /* Is active */
    u8      pad;
} SkidMark;

/* Effect system state */
typedef struct EffectsState {
    s32         num_active_particles;
    s32         num_active_emitters;
    s32         num_skid_marks;
    u8          effects_enabled;
    u8          particle_density;   /* 0-100% */
    u8          pad[2];
} EffectsState;

/* Global state */
extern Particle gParticles[];
extern Emitter gEmitters[];
extern SkidMark gSkidMarks[];
extern EffectsState gEffects;

/* Initialization */
void effects_init(void);
void effects_reset(void);

/* Per-frame update */
void effects_update(void);
void effects_draw(void);

/* Particle management */
s32 particle_spawn(u8 type, f32 pos[3], f32 vel[3], f32 size, u16 life);
void particle_spawn_burst(u8 type, f32 pos[3], s32 count, f32 spread, f32 speed);
void particle_kill(s32 index);
void particle_kill_all(void);

/* Emitter management */
s32 emitter_create(u8 type, f32 pos[3], u8 particle_type);
void emitter_destroy(s32 index);
void emitter_set_rate(s32 index, u16 rate);
void emitter_set_active(s32 index, u8 active);
void emitter_attach_to_car(s32 emitter, s32 car_index, s32 attach_point);

/* Car-specific effects */
void effects_tire_smoke(s32 car_index, s32 tire, f32 slip);
void effects_tire_dust(s32 car_index, s32 tire, u8 surface);
void effects_sparks(f32 pos[3], f32 vel[3], s32 count);
void effects_water_splash(f32 pos[3], f32 speed);
void effects_collision(f32 pos[3], f32 force, u8 type);
void effects_boost_flame(s32 car_index, f32 intensity);
void effects_exhaust(s32 car_index, f32 throttle);

/* Skid marks */
void skidmark_start(s32 car_index, s32 tire, f32 pos[3]);
void skidmark_update(s32 car_index, s32 tire, f32 pos[3]);
void skidmark_end(s32 car_index, s32 tire);
void skidmark_draw_all(void);

/* Utility */
void effects_set_density(u8 percent);
void effects_enable(s32 enable);

#endif /* EFFECTS_H */
