/**
 * particles.h - Particle effects system for Rush 2049 N64
 *
 * Handles visual effects like sparks, smoke, tire marks,
 * explosions, debris, and other particle-based effects.
 */

#ifndef PARTICLES_H
#define PARTICLES_H

#include "types.h"

/* Particle types */
#define PARTICLE_SPARK          0   /* Metal sparks */
#define PARTICLE_SMOKE          1   /* Smoke puff */
#define PARTICLE_DUST           2   /* Dust cloud */
#define PARTICLE_TIRE_SMOKE     3   /* Tire smoke (burnout) */
#define PARTICLE_EXHAUST        4   /* Exhaust fumes */
#define PARTICLE_DEBRIS         5   /* Debris chunks */
#define PARTICLE_GLASS          6   /* Glass shards */
#define PARTICLE_FIRE           7   /* Fire/flame */
#define PARTICLE_EXPLOSION      8   /* Explosion flash */
#define PARTICLE_WATER_SPLASH   9   /* Water splash */
#define PARTICLE_WATER_SPRAY    10  /* Water spray */
#define PARTICLE_DIRT           11  /* Dirt clumps */
#define PARTICLE_GRASS          12  /* Grass particles */
#define PARTICLE_BOOST_TRAIL    13  /* Boost afterburner */
#define PARTICLE_WING_TRAIL     14  /* Wing deploy trail */
#define PARTICLE_PICKUP_SPARKLE 15  /* Pickup collection */
#define NUM_PARTICLE_TYPES      16

/* Emitter types */
#define EMITTER_POINT           0   /* Single point */
#define EMITTER_SPHERE          1   /* Spherical volume */
#define EMITTER_BOX             2   /* Box volume */
#define EMITTER_CONE            3   /* Cone direction */
#define EMITTER_LINE            4   /* Line between points */
#define EMITTER_RING            5   /* Ring shape */
#define NUM_EMITTER_TYPES       6

/* Blend modes */
#define BLEND_NORMAL            0   /* Normal blend */
#define BLEND_ADDITIVE          1   /* Additive (glow) */
#define BLEND_MULTIPLY          2   /* Multiply */
#define BLEND_ALPHA             3   /* Alpha blend */
#define NUM_BLEND_MODES         4

/* Particle limits */
#define MAX_PARTICLES           256     /* Maximum active particles */
#define MAX_EMITTERS            32      /* Maximum emitters */
#define MAX_TIRE_MARKS          64      /* Tire mark segments */
#define MAX_TRAILS              8       /* Active trails */

/* Tire mark types */
#define TIREMARK_NONE           0
#define TIREMARK_SKID           1       /* Skidding */
#define TIREMARK_BURNOUT        2       /* Burnout/wheelspin */
#define TIREMARK_BRAKE          3       /* Braking */
#define TIREMARK_DRIFT          4       /* Drifting */

/* RGBA color */
typedef struct ParticleColor {
    u8      r, g, b, a;
} ParticleColor;

/* Single particle */
typedef struct Particle {
    /* State */
    u8      active;
    u8      type;               /* PARTICLE_* type */
    u8      blend_mode;         /* BLEND_* mode */
    u8      flags;

    /* Position and motion */
    f32     pos[3];             /* Current position */
    f32     vel[3];             /* Velocity */
    f32     accel[3];           /* Acceleration */

    /* Rotation */
    f32     rotation;           /* Current rotation */
    f32     rot_vel;            /* Rotation velocity */

    /* Size */
    f32     size;               /* Current size */
    f32     start_size;         /* Initial size */
    f32     end_size;           /* Final size */

    /* Life */
    f32     life;               /* Remaining life */
    f32     max_life;           /* Initial life */

    /* Color */
    ParticleColor start_color;  /* Initial color */
    ParticleColor end_color;    /* Final color */
    ParticleColor current_color;/* Interpolated color */

    /* Physics */
    f32     gravity;            /* Gravity multiplier */
    f32     drag;               /* Air resistance */
    f32     bounce;             /* Bounce factor */

    /* Texture */
    u8      texture_id;         /* Sprite texture */
    u8      frame;              /* Animation frame */
    u8      num_frames;         /* Total frames */
    u8      frame_rate;         /* Animation speed */
    f32     frame_timer;        /* Frame timer */

} Particle;

/* Particle emitter */
typedef struct ParticleEmitter {
    /* State */
    u8      active;
    u8      emitter_type;       /* EMITTER_* type */
    u8      particle_type;      /* PARTICLE_* to emit */
    u8      continuous;         /* Continuous emission */

    /* Position */
    f32     pos[3];             /* Emitter position */
    f32     pos2[3];            /* Second point (for line emitter) */
    f32     size[3];            /* Emitter size (box/sphere radius) */
    f32     direction[3];       /* Emission direction */
    f32     cone_angle;         /* Cone spread angle */

    /* Emission rate */
    f32     rate;               /* Particles per second */
    f32     accumulator;        /* Spawn accumulator */
    s32     burst_count;        /* Burst particle count */

    /* Particle properties */
    f32     speed_min;          /* Min initial speed */
    f32     speed_max;          /* Max initial speed */
    f32     life_min;           /* Min particle life */
    f32     life_max;           /* Max particle life */
    f32     size_min;           /* Min particle size */
    f32     size_max;           /* Max particle size */
    f32     gravity;            /* Gravity for particles */

    /* Colors */
    ParticleColor color_start;
    ParticleColor color_end;

    /* Emitter life */
    f32     emitter_life;       /* Remaining emitter life */
    u8      auto_destroy;       /* Destroy when done */
    u8      pad[3];

    /* Attachment */
    s32     attached_to;        /* Object to follow (-1 = none) */
    f32     offset[3];          /* Offset from attachment */

} ParticleEmitter;

/* Tire mark segment */
typedef struct TireMarkSegment {
    f32     pos[3];             /* Position */
    f32     normal[3];          /* Surface normal */
    f32     width;              /* Mark width */
    f32     opacity;            /* Current opacity */
    u8      type;               /* TIREMARK_* type */
    u8      active;
    u8      pad[2];
} TireMarkSegment;

/* Trail effect */
typedef struct ParticleTrail {
    u8      active;
    u8      type;               /* PARTICLE_* type */
    u8      num_points;         /* Points in trail */
    u8      max_points;         /* Max trail length */

    f32     points[32][3];      /* Trail point positions */
    f32     widths[32];         /* Width at each point */
    f32     life;               /* Trail fade timer */

    ParticleColor color;

    /* Attachment */
    s32     attached_to;        /* Object to follow */
    f32     offset[3];          /* Spawn offset */

} ParticleTrail;

/* Particle system state */
typedef struct ParticleSystem {
    /* Particles */
    Particle particles[MAX_PARTICLES];
    s32     num_active;

    /* Emitters */
    ParticleEmitter emitters[MAX_EMITTERS];
    s32     num_emitters;

    /* Tire marks */
    TireMarkSegment tire_marks[MAX_TIRE_MARKS];
    s32     num_tire_marks;
    s32     tire_mark_head;     /* Circular buffer head */

    /* Trails */
    ParticleTrail trails[MAX_TRAILS];
    s32     num_trails;

    /* Global settings */
    f32     global_gravity;     /* Global gravity */
    f32     wind[3];            /* Wind velocity */
    f32     time_scale;         /* Time multiplier */
    u8      enabled;            /* System enabled */
    u8      pad[3];

} ParticleSystem;

/* Global particle system */
extern ParticleSystem gParticles;

/* Initialization */
void particles_init(void);
void particles_reset(void);
void particles_clear(void);

/* Update */
void particles_update(void);
void particles_update_particle(Particle *p, f32 dt);
void particles_update_emitters(f32 dt);
void particles_update_trails(f32 dt);
void particles_update_tire_marks(f32 dt);

/* Rendering */
void particles_draw(void);
void particles_draw_sorted(f32 *camera_pos);
void particles_draw_tire_marks(void);
void particles_draw_trails(void);

/* Single particle spawning */
s32 particles_spawn(s32 type, f32 *pos, f32 *vel, f32 size, f32 life);
s32 particles_spawn_colored(s32 type, f32 *pos, f32 *vel, f32 size, f32 life,
                            ParticleColor *start, ParticleColor *end);
void particles_spawn_burst(s32 type, f32 *pos, s32 count, f32 speed, f32 life);

/* Emitter management */
s32 particles_create_emitter(s32 type, f32 *pos, f32 rate);
void particles_destroy_emitter(s32 index);
void particles_set_emitter_pos(s32 index, f32 *pos);
void particles_set_emitter_dir(s32 index, f32 *dir);
void particles_set_emitter_rate(s32 index, f32 rate);
void particles_attach_emitter(s32 index, s32 object_id, f32 *offset);
void particles_emit_burst(s32 index, s32 count);
ParticleEmitter* particles_get_emitter(s32 index);

/* Trail management */
s32 particles_create_trail(s32 type, s32 max_points);
void particles_destroy_trail(s32 index);
void particles_add_trail_point(s32 index, f32 *pos, f32 width);
void particles_attach_trail(s32 index, s32 object_id, f32 *offset);

/* Tire marks */
void particles_add_tire_mark(f32 *pos, f32 *normal, f32 width, s32 type);
void particles_fade_tire_marks(f32 amount);
void particles_clear_tire_marks(void);

/* Effect shortcuts */
void particles_sparks(f32 *pos, f32 *dir, s32 count);
void particles_smoke_puff(f32 *pos, f32 size);
void particles_dust_cloud(f32 *pos, f32 size, s32 count);
void particles_explosion(f32 *pos, f32 size);
void particles_water_splash(f32 *pos, f32 size);
void particles_debris(f32 *pos, f32 *vel, s32 count);
void particles_burnout_smoke(f32 *pos, f32 intensity);
void particles_exhaust(f32 *pos, f32 *vel, f32 intensity);
void particles_boost_flame(f32 *pos, f32 *dir, f32 intensity);
void particles_pickup_effect(f32 *pos, s32 pickup_type);

/* Collision */
s32 particles_check_ground(Particle *p, f32 *ground_height, f32 *normal);
void particles_bounce_particle(Particle *p, f32 *normal);

/* Settings */
void particles_set_gravity(f32 gravity);
void particles_set_wind(f32 x, f32 y, f32 z);
void particles_set_time_scale(f32 scale);
void particles_enable(u8 enabled);

/* Queries */
s32 particles_get_count(void);
s32 particles_get_emitter_count(void);

#endif /* PARTICLES_H */
