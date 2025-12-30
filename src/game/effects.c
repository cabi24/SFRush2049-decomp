/**
 * effects.c - Visual effects and particle system for Rush 2049 N64
 *
 * Handles tire smoke, sparks, dust, water splash, and other particles.
 */

#include "types.h"
#include "game/effects.h"
#include "game/structs.h"

/* External game state */
extern u8 gstate;
extern u32 frame_counter;
extern CarData car_array[];
extern s32 num_active_cars;

/* External functions */
extern f32 sqrtf(f32 x);
extern f32 sinf(f32 x);
extern f32 cosf(f32 x);

/* Random number generator state */
static u32 rand_state = 12345;

/* Global particle arrays */
Particle gParticles[MAX_PARTICLES];
Emitter gEmitters[MAX_PARTICLE_TYPES];
SkidMark gSkidMarks[128];
EffectsState gEffects;

/**
 * rand_float - Generate random float 0.0 to 1.0
 */
static f32 rand_float(void) {
    rand_state = rand_state * 1103515245 + 12345;
    return (f32)((rand_state >> 16) & 0x7FFF) / 32768.0f;
}

/**
 * rand_range - Generate random float in range
 */
static f32 rand_range(f32 min, f32 max) {
    return min + rand_float() * (max - min);
}

/**
 * lerp_color - Interpolate between two colors
 */
static u32 lerp_color(u32 c1, u32 c2, f32 t) {
    u8 r1 = (c1 >> 24) & 0xFF;
    u8 g1 = (c1 >> 16) & 0xFF;
    u8 b1 = (c1 >> 8) & 0xFF;
    u8 a1 = c1 & 0xFF;

    u8 r2 = (c2 >> 24) & 0xFF;
    u8 g2 = (c2 >> 16) & 0xFF;
    u8 b2 = (c2 >> 8) & 0xFF;
    u8 a2 = c2 & 0xFF;

    u8 r = (u8)(r1 + (r2 - r1) * t);
    u8 g = (u8)(g1 + (g2 - g1) * t);
    u8 b = (u8)(b1 + (b2 - b1) * t);
    u8 a = (u8)(a1 + (a2 - a1) * t);

    return ((u32)r << 24) | ((u32)g << 16) | ((u32)b << 8) | a;
}

/**
 * effects_init - Initialize effects system
 */
void effects_init(void) {
    s32 i;

    gEffects.num_active_particles = 0;
    gEffects.num_active_emitters = 0;
    gEffects.num_skid_marks = 0;
    gEffects.effects_enabled = 1;
    gEffects.particle_density = 100;

    /* Clear all particles */
    for (i = 0; i < MAX_PARTICLES; i++) {
        gParticles[i].flags = 0;
        gParticles[i].type = PARTICLE_NONE;
        gParticles[i].life = 0;
    }

    /* Clear all emitters */
    for (i = 0; i < MAX_PARTICLE_TYPES; i++) {
        gEmitters[i].active = 0;
        gEmitters[i].type = EMITTER_NONE;
    }

    /* Clear skid marks */
    for (i = 0; i < 128; i++) {
        gSkidMarks[i].active = 0;
    }
}

/**
 * effects_reset - Reset effects for new race
 */
void effects_reset(void) {
    particle_kill_all();
    effects_init();
}

/**
 * effects_update - Per-frame effects update
 */
void effects_update(void) {
    s32 i;
    Particle *p;
    Emitter *e;
    f32 life_t;
    f32 vel[3];
    f32 speed, angle1, angle2;

    if (!gEffects.effects_enabled) {
        return;
    }

    /* Update emitters */
    for (i = 0; i < MAX_PARTICLE_TYPES; i++) {
        e = &gEmitters[i];
        if (!e->active) {
            continue;
        }

        /* Accumulate rate and spawn particles */
        e->accumulator += e->rate;
        while (e->accumulator >= 256) {  /* Fixed point 8.8 */
            e->accumulator -= 256;

            /* Spawn particle */
            speed = rand_range(e->speed_min, e->speed_max);
            angle1 = rand_range(-e->spread, e->spread);
            angle2 = rand_range(-e->spread, e->spread);

            vel[0] = e->dir[0] * speed + sinf(angle1) * speed * 0.5f;
            vel[1] = e->dir[1] * speed + cosf(angle1) * speed * 0.5f;
            vel[2] = e->dir[2] * speed + sinf(angle2) * speed * 0.5f;

            particle_spawn(e->particle_type, e->pos, vel,
                          rand_range(e->size_min, e->size_max),
                          (u16)rand_range((f32)e->life_min, (f32)e->life_max));
        }
    }

    /* Update particles */
    gEffects.num_active_particles = 0;
    for (i = 0; i < MAX_PARTICLES; i++) {
        p = &gParticles[i];

        if (!(p->flags & PARTICLE_FLAG_ACTIVE)) {
            continue;
        }

        gEffects.num_active_particles++;

        /* Decrement life */
        if (p->life > 0) {
            p->life--;
        }

        /* Check for death */
        if (p->life == 0) {
            p->flags = 0;
            continue;
        }

        /* Apply acceleration */
        p->vel[0] += p->acc[0];
        p->vel[1] += p->acc[1];
        p->vel[2] += p->acc[2];

        /* Apply gravity */
        if (p->flags & PARTICLE_FLAG_GRAVITY) {
            p->vel[2] -= 0.5f;  /* Gravity */
        }

        /* Update position */
        p->pos[0] += p->vel[0];
        p->pos[1] += p->vel[1];
        p->pos[2] += p->vel[2];

        /* Ground collision */
        if ((p->flags & PARTICLE_FLAG_COLLIDE) && p->pos[2] < 0.0f) {
            p->pos[2] = 0.0f;
            p->vel[2] = -p->vel[2] * 0.3f;  /* Bounce */
            p->vel[0] *= 0.8f;
            p->vel[1] *= 0.8f;
        }

        /* Update size */
        p->size += p->size_vel;
        if (p->size < 0.1f) {
            p->size = 0.1f;
        }

        /* Update rotation */
        p->rotation += p->rot_vel;

        /* Calculate life ratio for color interpolation */
        life_t = 1.0f - (f32)p->life / (f32)p->max_life;

        /* Interpolate color */
        p->color = lerp_color(p->start_color, p->end_color, life_t);

        /* Update alpha */
        if (p->flags & PARTICLE_FLAG_FADE) {
            p->alpha = (u8)(255 * (1.0f - life_t));
        }
    }
}

/**
 * effects_draw - Draw all effects
 */
void effects_draw(void) {
    s32 i;
    Particle *p;

    if (!gEffects.effects_enabled) {
        return;
    }

    /* Draw particles */
    /* Would use N64 graphics to draw billboard sprites */
    for (i = 0; i < MAX_PARTICLES; i++) {
        p = &gParticles[i];
        if (!(p->flags & PARTICLE_FLAG_ACTIVE)) {
            continue;
        }

        /* Draw billboard at p->pos with p->size and p->color */
    }

    /* Draw skid marks */
    skidmark_draw_all();
}

/**
 * particle_spawn - Spawn a single particle
 *
 * @param type Particle type
 * @param pos Initial position
 * @param vel Initial velocity
 * @param size Particle size
 * @param life Lifetime in frames
 * @return Particle index or -1
 */
s32 particle_spawn(u8 type, f32 pos[3], f32 vel[3], f32 size, u16 life) {
    s32 i;
    Particle *p;

    /* Density check */
    if (rand_float() * 100.0f > gEffects.particle_density) {
        return -1;
    }

    /* Find free particle slot */
    for (i = 0; i < MAX_PARTICLES; i++) {
        if (!(gParticles[i].flags & PARTICLE_FLAG_ACTIVE)) {
            break;
        }
    }

    if (i >= MAX_PARTICLES) {
        return -1;  /* No free slots */
    }

    p = &gParticles[i];

    /* Initialize particle */
    p->pos[0] = pos[0];
    p->pos[1] = pos[1];
    p->pos[2] = pos[2];

    p->vel[0] = vel[0];
    p->vel[1] = vel[1];
    p->vel[2] = vel[2];

    p->acc[0] = 0.0f;
    p->acc[1] = 0.0f;
    p->acc[2] = 0.0f;

    p->size = size;
    p->size_vel = 0.0f;
    p->rotation = rand_float() * 6.28f;
    p->rot_vel = rand_range(-0.1f, 0.1f);
    p->life = life;
    p->max_life = life;
    p->type = type;
    p->alpha = 255;

    /* Set type-specific properties */
    switch (type) {
        case PARTICLE_SMOKE:
            p->start_color = COLOR_SMOKE_WHITE;
            p->end_color = COLOR_SMOKE_BLACK;
            p->flags = PARTICLE_FLAG_ACTIVE | PARTICLE_FLAG_FADE | PARTICLE_FLAG_GROW;
            p->size_vel = 0.05f;
            break;

        case PARTICLE_DUST:
            p->start_color = COLOR_DUST_BROWN;
            p->end_color = COLOR_DUST_YELLOW;
            p->flags = PARTICLE_FLAG_ACTIVE | PARTICLE_FLAG_FADE | PARTICLE_FLAG_GRAVITY;
            break;

        case PARTICLE_SPARK:
            p->start_color = COLOR_SPARK_ORANGE;
            p->end_color = COLOR_SPARK_WHITE;
            p->flags = PARTICLE_FLAG_ACTIVE | PARTICLE_FLAG_GRAVITY | PARTICLE_FLAG_COLLIDE;
            break;

        case PARTICLE_WATER:
            p->start_color = COLOR_WATER_BLUE;
            p->end_color = COLOR_WATER_BLUE;
            p->flags = PARTICLE_FLAG_ACTIVE | PARTICLE_FLAG_FADE | PARTICLE_FLAG_GRAVITY;
            break;

        case PARTICLE_FIRE:
            p->start_color = COLOR_FIRE_ORANGE;
            p->end_color = COLOR_FIRE_RED;
            p->flags = PARTICLE_FLAG_ACTIVE | PARTICLE_FLAG_FADE | PARTICLE_FLAG_SHRINK;
            p->size_vel = -0.02f;
            p->acc[2] = 0.1f;  /* Rise */
            break;

        default:
            p->start_color = 0xFFFFFFFF;
            p->end_color = 0xFFFFFF00;
            p->flags = PARTICLE_FLAG_ACTIVE | PARTICLE_FLAG_FADE;
            break;
    }

    p->color = p->start_color;

    return i;
}

/**
 * particle_spawn_burst - Spawn multiple particles at once
 *
 * @param type Particle type
 * @param pos Center position
 * @param count Number of particles
 * @param spread Spread radius
 * @param speed Initial speed
 */
void particle_spawn_burst(u8 type, f32 pos[3], s32 count, f32 spread, f32 speed) {
    s32 i;
    f32 vel[3];
    f32 angle1, angle2;

    for (i = 0; i < count; i++) {
        angle1 = rand_float() * 6.28f;
        angle2 = rand_float() * 3.14f - 1.57f;

        vel[0] = cosf(angle1) * cosf(angle2) * speed;
        vel[1] = sinf(angle1) * cosf(angle2) * speed;
        vel[2] = sinf(angle2) * speed;

        particle_spawn(type, pos, vel, rand_range(0.5f, 2.0f), (u16)rand_range(30, 60));
    }
}

/**
 * particle_kill - Kill a specific particle
 *
 * @param index Particle index
 */
void particle_kill(s32 index) {
    if (index >= 0 && index < MAX_PARTICLES) {
        gParticles[index].flags = 0;
    }
}

/**
 * particle_kill_all - Kill all particles
 */
void particle_kill_all(void) {
    s32 i;
    for (i = 0; i < MAX_PARTICLES; i++) {
        gParticles[i].flags = 0;
    }
    gEffects.num_active_particles = 0;
}

/**
 * emitter_create - Create a particle emitter
 *
 * @param type Emitter type
 * @param pos Position
 * @param particle_type Type of particles to emit
 * @return Emitter index or -1
 */
s32 emitter_create(u8 type, f32 pos[3], u8 particle_type) {
    s32 i;
    Emitter *e;

    for (i = 0; i < MAX_PARTICLE_TYPES; i++) {
        if (!gEmitters[i].active) {
            break;
        }
    }

    if (i >= MAX_PARTICLE_TYPES) {
        return -1;
    }

    e = &gEmitters[i];

    e->pos[0] = pos[0];
    e->pos[1] = pos[1];
    e->pos[2] = pos[2];
    e->dir[0] = 0.0f;
    e->dir[1] = 0.0f;
    e->dir[2] = 1.0f;
    e->spread = 0.5f;
    e->speed_min = 1.0f;
    e->speed_max = 3.0f;
    e->size_min = 0.5f;
    e->size_max = 1.5f;
    e->life_min = 30;
    e->life_max = 60;
    e->rate = 256;  /* 1 per frame */
    e->accumulator = 0;
    e->type = type;
    e->particle_type = particle_type;
    e->active = 1;
    e->car_index = -1;
    e->attach_point = 0;

    gEffects.num_active_emitters++;

    return i;
}

/**
 * emitter_destroy - Destroy an emitter
 *
 * @param index Emitter index
 */
void emitter_destroy(s32 index) {
    if (index >= 0 && index < MAX_PARTICLE_TYPES) {
        if (gEmitters[index].active) {
            gEmitters[index].active = 0;
            gEffects.num_active_emitters--;
        }
    }
}

/**
 * emitter_set_rate - Set emitter spawn rate
 *
 * @param index Emitter index
 * @param rate Rate (fixed point 8.8, 256 = 1 per frame)
 */
void emitter_set_rate(s32 index, u16 rate) {
    if (index >= 0 && index < MAX_PARTICLE_TYPES) {
        gEmitters[index].rate = rate;
    }
}

/**
 * emitter_set_active - Enable/disable emitter
 *
 * @param index Emitter index
 * @param active Active state
 */
void emitter_set_active(s32 index, u8 active) {
    if (index >= 0 && index < MAX_PARTICLE_TYPES) {
        gEmitters[index].active = active;
    }
}

/**
 * emitter_attach_to_car - Attach emitter to car
 *
 * @param emitter Emitter index
 * @param car_index Car index
 * @param attach_point Attachment point
 */
void emitter_attach_to_car(s32 emitter, s32 car_index, s32 attach_point) {
    if (emitter >= 0 && emitter < MAX_PARTICLE_TYPES) {
        gEmitters[emitter].car_index = car_index;
        gEmitters[emitter].attach_point = attach_point;
    }
}

/**
 * effects_tire_smoke - Spawn tire smoke
 *
 * @param car_index Car producing smoke
 * @param tire Tire index (0-3)
 * @param slip Slip amount (0-1)
 */
void effects_tire_smoke(s32 car_index, s32 tire, f32 slip) {
    CarData *car;
    f32 pos[3], vel[3];
    f32 intensity;

    if (slip < 0.3f) {
        return;  /* Not enough slip */
    }

    car = &car_array[car_index];
    intensity = (slip - 0.3f) / 0.7f;

    /* Get tire position */
    pos[0] = car->dr_pos[0];  /* Would use actual tire position */
    pos[1] = car->dr_pos[1];
    pos[2] = 0.0f;  /* Ground level */

    vel[0] = rand_range(-0.5f, 0.5f);
    vel[1] = rand_range(-0.5f, 0.5f);
    vel[2] = rand_range(0.5f, 1.5f);

    if (rand_float() < intensity * 0.5f) {
        particle_spawn(PARTICLE_SMOKE, pos, vel,
                      rand_range(1.0f, 3.0f) * intensity,
                      (u16)(60 * intensity));
    }
}

/**
 * effects_tire_dust - Spawn tire dust
 *
 * @param car_index Car
 * @param tire Tire index
 * @param surface Surface type
 */
void effects_tire_dust(s32 car_index, s32 tire, u8 surface) {
    CarData *car;
    f32 pos[3], vel[3];

    car = &car_array[car_index];

    pos[0] = car->dr_pos[0];
    pos[1] = car->dr_pos[1];
    pos[2] = 0.0f;

    vel[0] = rand_range(-1.0f, 1.0f);
    vel[1] = rand_range(-1.0f, 1.0f);
    vel[2] = rand_range(0.5f, 2.0f);

    particle_spawn(PARTICLE_DUST, pos, vel, rand_range(0.5f, 1.5f), 40);
}

/**
 * effects_sparks - Spawn collision sparks
 *
 * @param pos Spark position
 * @param vel Impact velocity
 * @param count Number of sparks
 */
void effects_sparks(f32 pos[3], f32 vel[3], s32 count) {
    particle_spawn_burst(PARTICLE_SPARK, pos, count, 2.0f, 5.0f);
}

/**
 * effects_water_splash - Spawn water splash
 *
 * @param pos Splash position
 * @param speed Impact speed
 */
void effects_water_splash(f32 pos[3], f32 speed) {
    s32 count = (s32)(speed * 0.5f);
    if (count > 50) count = 50;
    particle_spawn_burst(PARTICLE_WATER, pos, count, 3.0f, speed * 0.3f);
}

/**
 * effects_collision - Spawn collision effects
 *
 * @param pos Collision position
 * @param force Collision force
 * @param type Collision type
 */
void effects_collision(f32 pos[3], f32 force, u8 type) {
    s32 count;

    if (force < 500.0f) {
        return;
    }

    count = (s32)(force / 200.0f);
    if (count > 30) count = 30;

    effects_sparks(pos, pos, count);  /* Using pos as dummy vel */
}

/**
 * effects_boost_flame - Spawn boost flame effect
 *
 * @param car_index Car index
 * @param intensity Boost intensity (0-1)
 */
void effects_boost_flame(s32 car_index, f32 intensity) {
    CarData *car;
    f32 pos[3], vel[3];

    if (intensity < 0.1f) {
        return;
    }

    car = &car_array[car_index];

    /* Position at rear of car */
    pos[0] = car->dr_pos[0] - 3.0f;
    pos[1] = car->dr_pos[1];
    pos[2] = 1.0f;

    vel[0] = -2.0f * intensity;
    vel[1] = rand_range(-0.2f, 0.2f);
    vel[2] = rand_range(-0.2f, 0.2f);

    particle_spawn(PARTICLE_FIRE, pos, vel,
                  rand_range(0.5f, 1.5f) * intensity, 15);
}

/**
 * effects_exhaust - Spawn exhaust smoke
 *
 * @param car_index Car index
 * @param throttle Throttle amount (0-1)
 */
void effects_exhaust(s32 car_index, f32 throttle) {
    CarData *car;
    f32 pos[3], vel[3];

    if (throttle < 0.2f) {
        return;
    }

    if (rand_float() > throttle * 0.2f) {
        return;
    }

    car = &car_array[car_index];

    pos[0] = car->dr_pos[0] - 2.5f;
    pos[1] = car->dr_pos[1];
    pos[2] = 1.0f;

    vel[0] = -1.0f;
    vel[1] = rand_range(-0.1f, 0.1f);
    vel[2] = rand_range(0.0f, 0.2f);

    particle_spawn(PARTICLE_EXHAUST, pos, vel, 0.3f, 30);
}

/**
 * skidmark_start - Start a new skid mark
 */
void skidmark_start(s32 car_index, s32 tire, f32 pos[3]) {
    /* Would find free skid mark slot and initialize */
}

/**
 * skidmark_update - Update ongoing skid mark
 */
void skidmark_update(s32 car_index, s32 tire, f32 pos[3]) {
    /* Would extend skid mark segment */
}

/**
 * skidmark_end - End skid mark
 */
void skidmark_end(s32 car_index, s32 tire) {
    /* Would finalize skid mark */
}

/**
 * skidmark_draw_all - Draw all skid marks
 */
void skidmark_draw_all(void) {
    s32 i;

    for (i = 0; i < 128; i++) {
        if (!gSkidMarks[i].active) {
            continue;
        }

        /* Would draw textured quad on ground */
    }
}

/**
 * effects_set_density - Set particle density
 *
 * @param percent Density percentage (0-100)
 */
void effects_set_density(u8 percent) {
    if (percent > 100) percent = 100;
    gEffects.particle_density = percent;
}

/**
 * effects_enable - Enable/disable effects
 *
 * @param enable Enable state
 */
void effects_enable(s32 enable) {
    gEffects.effects_enabled = enable ? 1 : 0;
    if (!enable) {
        particle_kill_all();
    }
}
