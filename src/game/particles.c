/**
 * particles.c - Particle effects system for Rush 2049 N64
 *
 * Implements visual effects like sparks, smoke, and explosions.
 */

#include "game/particles.h"

/* External functions */
extern f32 sinf(f32 x);
extern f32 cosf(f32 x);
extern f32 sqrtf(f32 x);
extern s32 rand(void);

/* Random helpers */
#define RANDF() ((f32)(rand() % 1000) / 1000.0f)
#define RANDF_RANGE(min, max) ((min) + RANDF() * ((max) - (min)))
#define RAND_SIGN() ((rand() % 2) ? 1.0f : -1.0f)

/* Physics constants */
#define DEFAULT_GRAVITY     -9.8f
#define DEFAULT_DRAG        0.02f

/* Global particle system */
ParticleSystem gParticles;

/* Default particle properties per type */
static const struct {
    f32     gravity;
    f32     drag;
    f32     bounce;
    f32     size;
    f32     life;
    u8      start_color[4];
    u8      end_color[4];
    u8      blend_mode;
} sParticleDefaults[NUM_PARTICLE_TYPES] = {
    /* SPARK */         { 0.5f, 0.01f, 0.3f, 0.2f, 0.5f,  {255,200,100,255}, {255,100,0,0},    BLEND_ADDITIVE },
    /* SMOKE */         { -0.2f, 0.05f, 0.0f, 1.0f, 2.0f, {128,128,128,200}, {64,64,64,0},     BLEND_ALPHA },
    /* DUST */          { 0.1f, 0.03f, 0.0f, 0.5f, 1.5f,  {180,160,120,180}, {120,100,80,0},   BLEND_ALPHA },
    /* TIRE_SMOKE */    { -0.3f, 0.04f, 0.0f, 0.8f, 1.0f, {64,64,64,200},    {32,32,32,0},     BLEND_ALPHA },
    /* EXHAUST */       { -0.1f, 0.02f, 0.0f, 0.3f, 0.5f, {64,64,64,150},    {32,32,32,0},     BLEND_ALPHA },
    /* DEBRIS */        { 1.0f, 0.01f, 0.5f, 0.3f, 2.0f,  {150,150,150,255}, {100,100,100,255},BLEND_NORMAL },
    /* GLASS */         { 1.0f, 0.01f, 0.6f, 0.15f, 1.5f, {200,220,255,200}, {150,180,220,0},  BLEND_ALPHA },
    /* FIRE */          { -0.5f, 0.02f, 0.0f, 0.6f, 0.8f, {255,200,50,255},  {255,50,0,0},     BLEND_ADDITIVE },
    /* EXPLOSION */     { 0.0f, 0.0f, 0.0f, 2.0f, 0.3f,   {255,255,200,255}, {255,100,0,0},    BLEND_ADDITIVE },
    /* WATER_SPLASH */  { 0.8f, 0.02f, 0.0f, 0.4f, 1.0f,  {150,180,255,200}, {100,150,255,0},  BLEND_ALPHA },
    /* WATER_SPRAY */   { 0.3f, 0.03f, 0.0f, 0.2f, 0.8f,  {200,220,255,180}, {180,200,255,0},  BLEND_ALPHA },
    /* DIRT */          { 1.0f, 0.02f, 0.3f, 0.25f, 1.5f, {120,80,40,255},   {80,50,20,200},   BLEND_NORMAL },
    /* GRASS */         { 0.8f, 0.03f, 0.2f, 0.15f, 1.2f, {50,150,50,255},   {30,100,30,200},  BLEND_NORMAL },
    /* BOOST_TRAIL */   { 0.0f, 0.01f, 0.0f, 0.5f, 0.3f,  {100,150,255,255}, {50,100,255,0},   BLEND_ADDITIVE },
    /* WING_TRAIL */    { 0.0f, 0.01f, 0.0f, 0.3f, 0.5f,  {200,200,255,200}, {150,150,255,0},  BLEND_ADDITIVE },
    /* PICKUP_SPARKLE */{ -0.1f, 0.0f, 0.0f, 0.2f, 0.5f,  {255,255,100,255}, {255,255,0,0},    BLEND_ADDITIVE },
};

/* -------------------------------------------------------------------------- */
/* Initialization                                                              */
/* -------------------------------------------------------------------------- */

/**
 * Initialize particle system
 */
void particles_init(void) {
    s32 i;

    /* Clear all data */
    for (i = 0; i < (s32)sizeof(ParticleSystem); i++) {
        ((u8*)&gParticles)[i] = 0;
    }

    gParticles.global_gravity = DEFAULT_GRAVITY;
    gParticles.time_scale = 1.0f;
    gParticles.enabled = 1;
}

/**
 * Reset particle system
 */
void particles_reset(void) {
    particles_clear();
}

/**
 * Clear all particles and emitters
 */
void particles_clear(void) {
    s32 i;

    for (i = 0; i < MAX_PARTICLES; i++) {
        gParticles.particles[i].active = 0;
    }
    gParticles.num_active = 0;

    for (i = 0; i < MAX_EMITTERS; i++) {
        gParticles.emitters[i].active = 0;
    }
    gParticles.num_emitters = 0;

    for (i = 0; i < MAX_TRAILS; i++) {
        gParticles.trails[i].active = 0;
    }
    gParticles.num_trails = 0;

    particles_clear_tire_marks();
}

/* -------------------------------------------------------------------------- */
/* Update                                                                      */
/* -------------------------------------------------------------------------- */

/**
 * Update all particles
 */
void particles_update(void) {
    s32 i;
    f32 dt;

    if (!gParticles.enabled) {
        return;
    }

    dt = (1.0f / 60.0f) * gParticles.time_scale;

    /* Update particles */
    gParticles.num_active = 0;
    for (i = 0; i < MAX_PARTICLES; i++) {
        if (gParticles.particles[i].active) {
            particles_update_particle(&gParticles.particles[i], dt);
            if (gParticles.particles[i].active) {
                gParticles.num_active++;
            }
        }
    }

    /* Update emitters */
    particles_update_emitters(dt);

    /* Update trails */
    particles_update_trails(dt);

    /* Fade tire marks */
    particles_update_tire_marks(dt);
}

/**
 * Update single particle
 */
void particles_update_particle(Particle *p, f32 dt) {
    f32 t;
    f32 ground_y;
    f32 normal[3];

    /* Update life */
    p->life -= dt;
    if (p->life <= 0.0f) {
        p->active = 0;
        return;
    }

    /* Life ratio for interpolation */
    t = 1.0f - (p->life / p->max_life);

    /* Apply gravity */
    p->vel[1] += p->gravity * gParticles.global_gravity * dt;

    /* Apply wind */
    p->vel[0] += gParticles.wind[0] * dt;
    p->vel[1] += gParticles.wind[1] * dt;
    p->vel[2] += gParticles.wind[2] * dt;

    /* Apply drag */
    p->vel[0] *= (1.0f - p->drag);
    p->vel[1] *= (1.0f - p->drag);
    p->vel[2] *= (1.0f - p->drag);

    /* Apply acceleration */
    p->vel[0] += p->accel[0] * dt;
    p->vel[1] += p->accel[1] * dt;
    p->vel[2] += p->accel[2] * dt;

    /* Update position */
    p->pos[0] += p->vel[0] * dt;
    p->pos[1] += p->vel[1] * dt;
    p->pos[2] += p->vel[2] * dt;

    /* Update rotation */
    p->rotation += p->rot_vel * dt;

    /* Interpolate size */
    p->size = p->start_size + (p->end_size - p->start_size) * t;

    /* Interpolate color */
    p->current_color.r = (u8)(p->start_color.r + (s32)(p->end_color.r - p->start_color.r) * t);
    p->current_color.g = (u8)(p->start_color.g + (s32)(p->end_color.g - p->start_color.g) * t);
    p->current_color.b = (u8)(p->start_color.b + (s32)(p->end_color.b - p->start_color.b) * t);
    p->current_color.a = (u8)(p->start_color.a + (s32)(p->end_color.a - p->start_color.a) * t);

    /* Check ground collision */
    if (p->bounce > 0.0f) {
        if (particles_check_ground(p, &ground_y, normal)) {
            if (p->pos[1] < ground_y) {
                p->pos[1] = ground_y;
                particles_bounce_particle(p, normal);
            }
        }
    }

    /* Update animation */
    if (p->num_frames > 1) {
        p->frame_timer += dt * p->frame_rate;
        if (p->frame_timer >= 1.0f) {
            p->frame_timer -= 1.0f;
            p->frame++;
            if (p->frame >= p->num_frames) {
                p->frame = 0;
            }
        }
    }
}

/**
 * Update all emitters
 */
void particles_update_emitters(f32 dt) {
    s32 i, j;
    ParticleEmitter *e;
    f32 spawn_pos[3];
    f32 spawn_vel[3];
    f32 speed, angle;

    gParticles.num_emitters = 0;

    for (i = 0; i < MAX_EMITTERS; i++) {
        e = &gParticles.emitters[i];
        if (!e->active) {
            continue;
        }

        gParticles.num_emitters++;

        /* Update emitter life */
        if (e->emitter_life > 0.0f) {
            e->emitter_life -= dt;
            if (e->emitter_life <= 0.0f) {
                if (e->auto_destroy) {
                    e->active = 0;
                    continue;
                }
            }
        }

        /* Continuous emission */
        if (e->continuous && e->rate > 0.0f) {
            e->accumulator += e->rate * dt;

            while (e->accumulator >= 1.0f) {
                e->accumulator -= 1.0f;

                /* Calculate spawn position based on emitter type */
                spawn_pos[0] = e->pos[0];
                spawn_pos[1] = e->pos[1];
                spawn_pos[2] = e->pos[2];

                switch (e->emitter_type) {
                    case EMITTER_SPHERE:
                        angle = RANDF() * 6.28318f;
                        spawn_pos[0] += cosf(angle) * RANDF() * e->size[0];
                        spawn_pos[1] += RANDF_RANGE(-e->size[1], e->size[1]);
                        spawn_pos[2] += sinf(angle) * RANDF() * e->size[2];
                        break;

                    case EMITTER_BOX:
                        spawn_pos[0] += RANDF_RANGE(-e->size[0], e->size[0]);
                        spawn_pos[1] += RANDF_RANGE(-e->size[1], e->size[1]);
                        spawn_pos[2] += RANDF_RANGE(-e->size[2], e->size[2]);
                        break;

                    case EMITTER_LINE:
                        {
                            f32 t = RANDF();
                            spawn_pos[0] = e->pos[0] + (e->pos2[0] - e->pos[0]) * t;
                            spawn_pos[1] = e->pos[1] + (e->pos2[1] - e->pos[1]) * t;
                            spawn_pos[2] = e->pos[2] + (e->pos2[2] - e->pos[2]) * t;
                        }
                        break;
                }

                /* Calculate velocity */
                speed = RANDF_RANGE(e->speed_min, e->speed_max);

                if (e->emitter_type == EMITTER_CONE) {
                    /* Cone spread */
                    f32 cone_rad = e->cone_angle * 3.14159f / 180.0f;
                    f32 spread = RANDF() * cone_rad;
                    f32 rot = RANDF() * 6.28318f;

                    spawn_vel[0] = e->direction[0] + sinf(rot) * sinf(spread);
                    spawn_vel[1] = e->direction[1] * cosf(spread);
                    spawn_vel[2] = e->direction[2] + cosf(rot) * sinf(spread);
                } else {
                    spawn_vel[0] = e->direction[0] + RANDF_RANGE(-0.2f, 0.2f);
                    spawn_vel[1] = e->direction[1] + RANDF_RANGE(-0.2f, 0.2f);
                    spawn_vel[2] = e->direction[2] + RANDF_RANGE(-0.2f, 0.2f);
                }

                /* Normalize and scale */
                {
                    f32 len = sqrtf(spawn_vel[0]*spawn_vel[0] +
                                   spawn_vel[1]*spawn_vel[1] +
                                   spawn_vel[2]*spawn_vel[2]);
                    if (len > 0.001f) {
                        spawn_vel[0] = spawn_vel[0] / len * speed;
                        spawn_vel[1] = spawn_vel[1] / len * speed;
                        spawn_vel[2] = spawn_vel[2] / len * speed;
                    }
                }

                /* Spawn particle */
                j = particles_spawn_colored(
                    e->particle_type,
                    spawn_pos, spawn_vel,
                    RANDF_RANGE(e->size_min, e->size_max),
                    RANDF_RANGE(e->life_min, e->life_max),
                    &e->color_start, &e->color_end
                );

                if (j >= 0) {
                    gParticles.particles[j].gravity = e->gravity;
                }
            }
        }
    }
}

/**
 * Update trails
 */
void particles_update_trails(f32 dt) {
    s32 i;
    ParticleTrail *trail;

    gParticles.num_trails = 0;

    for (i = 0; i < MAX_TRAILS; i++) {
        trail = &gParticles.trails[i];
        if (!trail->active) {
            continue;
        }

        gParticles.num_trails++;

        /* Fade trail */
        trail->life -= dt;
        if (trail->life <= 0.0f) {
            /* Shift points */
            if (trail->num_points > 1) {
                s32 j;
                for (j = 0; j < trail->num_points - 1; j++) {
                    trail->points[j][0] = trail->points[j + 1][0];
                    trail->points[j][1] = trail->points[j + 1][1];
                    trail->points[j][2] = trail->points[j + 1][2];
                    trail->widths[j] = trail->widths[j + 1];
                }
                trail->num_points--;
            }
            trail->life = 0.05f;  /* Fade rate */

            if (trail->num_points == 0) {
                trail->active = 0;
            }
        }
    }
}

/**
 * Update tire marks
 */
void particles_update_tire_marks(f32 dt) {
    s32 i;
    TireMarkSegment *mark;

    for (i = 0; i < MAX_TIRE_MARKS; i++) {
        mark = &gParticles.tire_marks[i];
        if (!mark->active) {
            continue;
        }

        /* Fade over time */
        mark->opacity -= dt * 0.1f;
        if (mark->opacity <= 0.0f) {
            mark->active = 0;
        }
    }
}

/* -------------------------------------------------------------------------- */
/* Rendering                                                                   */
/* -------------------------------------------------------------------------- */

void particles_draw(void) {
    if (!gParticles.enabled) {
        return;
    }

    /* Draw tire marks first (on ground) */
    particles_draw_tire_marks();

    /* Draw trails */
    particles_draw_trails();

    /* Draw particles */
    /* Would sort by distance and render billboards */
}

void particles_draw_sorted(f32 *camera_pos) {
    /* Would sort particles by distance to camera and draw back-to-front */
    particles_draw();
}

void particles_draw_tire_marks(void) {
    /* Would draw tire mark decals on ground */
}

void particles_draw_trails(void) {
    /* Would draw trail ribbons */
}

/* -------------------------------------------------------------------------- */
/* Particle Spawning                                                           */
/* -------------------------------------------------------------------------- */

/**
 * Spawn a single particle
 */
s32 particles_spawn(s32 type, f32 *pos, f32 *vel, f32 size, f32 life) {
    ParticleColor start, end;

    start.r = sParticleDefaults[type].start_color[0];
    start.g = sParticleDefaults[type].start_color[1];
    start.b = sParticleDefaults[type].start_color[2];
    start.a = sParticleDefaults[type].start_color[3];

    end.r = sParticleDefaults[type].end_color[0];
    end.g = sParticleDefaults[type].end_color[1];
    end.b = sParticleDefaults[type].end_color[2];
    end.a = sParticleDefaults[type].end_color[3];

    return particles_spawn_colored(type, pos, vel, size, life, &start, &end);
}

/**
 * Spawn particle with custom colors
 */
s32 particles_spawn_colored(s32 type, f32 *pos, f32 *vel, f32 size, f32 life,
                            ParticleColor *start, ParticleColor *end) {
    s32 i;
    Particle *p;

    if (type < 0 || type >= NUM_PARTICLE_TYPES) {
        return -1;
    }

    /* Find free slot */
    for (i = 0; i < MAX_PARTICLES; i++) {
        if (!gParticles.particles[i].active) {
            p = &gParticles.particles[i];

            p->active = 1;
            p->type = (u8)type;
            p->blend_mode = sParticleDefaults[type].blend_mode;

            p->pos[0] = pos[0];
            p->pos[1] = pos[1];
            p->pos[2] = pos[2];

            p->vel[0] = vel[0];
            p->vel[1] = vel[1];
            p->vel[2] = vel[2];

            p->accel[0] = p->accel[1] = p->accel[2] = 0.0f;

            p->rotation = RANDF() * 360.0f;
            p->rot_vel = RANDF_RANGE(-180.0f, 180.0f);

            p->size = size;
            p->start_size = size;
            p->end_size = size * 0.5f;

            p->life = life;
            p->max_life = life;

            p->start_color = *start;
            p->end_color = *end;
            p->current_color = *start;

            p->gravity = sParticleDefaults[type].gravity;
            p->drag = sParticleDefaults[type].drag;
            p->bounce = sParticleDefaults[type].bounce;

            p->texture_id = 0;
            p->frame = 0;
            p->num_frames = 1;
            p->frame_rate = 10;
            p->frame_timer = 0.0f;

            gParticles.num_active++;
            return i;
        }
    }

    return -1;  /* No free slots */
}

/**
 * Spawn burst of particles
 */
void particles_spawn_burst(s32 type, f32 *pos, s32 count, f32 speed, f32 life) {
    s32 i;
    f32 vel[3];
    f32 angle1, angle2;

    for (i = 0; i < count; i++) {
        angle1 = RANDF() * 6.28318f;
        angle2 = RANDF() * 3.14159f - 1.5708f;

        vel[0] = cosf(angle1) * cosf(angle2) * speed;
        vel[1] = sinf(angle2) * speed;
        vel[2] = sinf(angle1) * cosf(angle2) * speed;

        particles_spawn(type, pos, vel, sParticleDefaults[type].size, life);
    }
}

/* -------------------------------------------------------------------------- */
/* Emitter Management                                                          */
/* -------------------------------------------------------------------------- */

s32 particles_create_emitter(s32 type, f32 *pos, f32 rate) {
    s32 i;
    ParticleEmitter *e;

    for (i = 0; i < MAX_EMITTERS; i++) {
        if (!gParticles.emitters[i].active) {
            e = &gParticles.emitters[i];

            e->active = 1;
            e->emitter_type = EMITTER_POINT;
            e->particle_type = (u8)type;
            e->continuous = 1;

            e->pos[0] = pos[0];
            e->pos[1] = pos[1];
            e->pos[2] = pos[2];

            e->direction[0] = 0.0f;
            e->direction[1] = 1.0f;
            e->direction[2] = 0.0f;

            e->rate = rate;
            e->accumulator = 0.0f;

            e->speed_min = 1.0f;
            e->speed_max = 2.0f;
            e->life_min = sParticleDefaults[type].life * 0.8f;
            e->life_max = sParticleDefaults[type].life * 1.2f;
            e->size_min = sParticleDefaults[type].size * 0.8f;
            e->size_max = sParticleDefaults[type].size * 1.2f;
            e->gravity = sParticleDefaults[type].gravity;

            e->color_start.r = sParticleDefaults[type].start_color[0];
            e->color_start.g = sParticleDefaults[type].start_color[1];
            e->color_start.b = sParticleDefaults[type].start_color[2];
            e->color_start.a = sParticleDefaults[type].start_color[3];

            e->color_end.r = sParticleDefaults[type].end_color[0];
            e->color_end.g = sParticleDefaults[type].end_color[1];
            e->color_end.b = sParticleDefaults[type].end_color[2];
            e->color_end.a = sParticleDefaults[type].end_color[3];

            e->emitter_life = -1.0f;  /* Infinite */
            e->auto_destroy = 0;
            e->attached_to = -1;

            gParticles.num_emitters++;
            return i;
        }
    }

    return -1;
}

void particles_destroy_emitter(s32 index) {
    if (index >= 0 && index < MAX_EMITTERS) {
        if (gParticles.emitters[index].active) {
            gParticles.emitters[index].active = 0;
            gParticles.num_emitters--;
        }
    }
}

void particles_set_emitter_pos(s32 index, f32 *pos) {
    if (index >= 0 && index < MAX_EMITTERS) {
        gParticles.emitters[index].pos[0] = pos[0];
        gParticles.emitters[index].pos[1] = pos[1];
        gParticles.emitters[index].pos[2] = pos[2];
    }
}

void particles_set_emitter_dir(s32 index, f32 *dir) {
    if (index >= 0 && index < MAX_EMITTERS) {
        gParticles.emitters[index].direction[0] = dir[0];
        gParticles.emitters[index].direction[1] = dir[1];
        gParticles.emitters[index].direction[2] = dir[2];
    }
}

void particles_set_emitter_rate(s32 index, f32 rate) {
    if (index >= 0 && index < MAX_EMITTERS) {
        gParticles.emitters[index].rate = rate;
    }
}

void particles_attach_emitter(s32 index, s32 object_id, f32 *offset) {
    if (index >= 0 && index < MAX_EMITTERS) {
        gParticles.emitters[index].attached_to = object_id;
        gParticles.emitters[index].offset[0] = offset[0];
        gParticles.emitters[index].offset[1] = offset[1];
        gParticles.emitters[index].offset[2] = offset[2];
    }
}

void particles_emit_burst(s32 index, s32 count) {
    if (index >= 0 && index < MAX_EMITTERS) {
        gParticles.emitters[index].burst_count = count;
    }
}

ParticleEmitter* particles_get_emitter(s32 index) {
    if (index >= 0 && index < MAX_EMITTERS) {
        return &gParticles.emitters[index];
    }
    return NULL;
}

/* -------------------------------------------------------------------------- */
/* Trail Management                                                            */
/* -------------------------------------------------------------------------- */

s32 particles_create_trail(s32 type, s32 max_points) {
    s32 i;
    ParticleTrail *trail;

    if (max_points > 32) max_points = 32;

    for (i = 0; i < MAX_TRAILS; i++) {
        if (!gParticles.trails[i].active) {
            trail = &gParticles.trails[i];
            trail->active = 1;
            trail->type = (u8)type;
            trail->num_points = 0;
            trail->max_points = (u8)max_points;
            trail->life = 1.0f;
            trail->attached_to = -1;
            gParticles.num_trails++;
            return i;
        }
    }

    return -1;
}

void particles_destroy_trail(s32 index) {
    if (index >= 0 && index < MAX_TRAILS) {
        gParticles.trails[index].active = 0;
    }
}

void particles_add_trail_point(s32 index, f32 *pos, f32 width) {
    ParticleTrail *trail;

    if (index < 0 || index >= MAX_TRAILS) {
        return;
    }

    trail = &gParticles.trails[index];
    if (!trail->active) {
        return;
    }

    if (trail->num_points >= trail->max_points) {
        /* Shift points */
        s32 i;
        for (i = 0; i < trail->num_points - 1; i++) {
            trail->points[i][0] = trail->points[i + 1][0];
            trail->points[i][1] = trail->points[i + 1][1];
            trail->points[i][2] = trail->points[i + 1][2];
            trail->widths[i] = trail->widths[i + 1];
        }
        trail->num_points--;
    }

    trail->points[trail->num_points][0] = pos[0];
    trail->points[trail->num_points][1] = pos[1];
    trail->points[trail->num_points][2] = pos[2];
    trail->widths[trail->num_points] = width;
    trail->num_points++;
}

void particles_attach_trail(s32 index, s32 object_id, f32 *offset) {
    if (index >= 0 && index < MAX_TRAILS) {
        gParticles.trails[index].attached_to = object_id;
        gParticles.trails[index].offset[0] = offset[0];
        gParticles.trails[index].offset[1] = offset[1];
        gParticles.trails[index].offset[2] = offset[2];
    }
}

/* -------------------------------------------------------------------------- */
/* Tire Marks                                                                  */
/* -------------------------------------------------------------------------- */

void particles_add_tire_mark(f32 *pos, f32 *normal, f32 width, s32 type) {
    TireMarkSegment *mark;

    mark = &gParticles.tire_marks[gParticles.tire_mark_head];

    mark->active = 1;
    mark->type = (u8)type;
    mark->pos[0] = pos[0];
    mark->pos[1] = pos[1];
    mark->pos[2] = pos[2];
    mark->normal[0] = normal[0];
    mark->normal[1] = normal[1];
    mark->normal[2] = normal[2];
    mark->width = width;
    mark->opacity = 1.0f;

    gParticles.tire_mark_head++;
    if (gParticles.tire_mark_head >= MAX_TIRE_MARKS) {
        gParticles.tire_mark_head = 0;
    }

    if (gParticles.num_tire_marks < MAX_TIRE_MARKS) {
        gParticles.num_tire_marks++;
    }
}

void particles_fade_tire_marks(f32 amount) {
    s32 i;
    for (i = 0; i < MAX_TIRE_MARKS; i++) {
        if (gParticles.tire_marks[i].active) {
            gParticles.tire_marks[i].opacity -= amount;
            if (gParticles.tire_marks[i].opacity <= 0.0f) {
                gParticles.tire_marks[i].active = 0;
            }
        }
    }
}

void particles_clear_tire_marks(void) {
    s32 i;
    for (i = 0; i < MAX_TIRE_MARKS; i++) {
        gParticles.tire_marks[i].active = 0;
    }
    gParticles.num_tire_marks = 0;
    gParticles.tire_mark_head = 0;
}

/* -------------------------------------------------------------------------- */
/* Effect Shortcuts                                                            */
/* -------------------------------------------------------------------------- */

void particles_sparks(f32 *pos, f32 *dir, s32 count) {
    s32 i;
    f32 vel[3];
    f32 speed;

    for (i = 0; i < count; i++) {
        speed = RANDF_RANGE(5.0f, 15.0f);
        vel[0] = dir[0] * speed + RANDF_RANGE(-3.0f, 3.0f);
        vel[1] = dir[1] * speed + RANDF_RANGE(0.0f, 5.0f);
        vel[2] = dir[2] * speed + RANDF_RANGE(-3.0f, 3.0f);

        particles_spawn(PARTICLE_SPARK, pos, vel, 0.1f + RANDF() * 0.15f, 0.3f + RANDF() * 0.4f);
    }
}

void particles_smoke_puff(f32 *pos, f32 size) {
    f32 vel[3] = { RANDF_RANGE(-0.5f, 0.5f), RANDF_RANGE(1.0f, 2.0f), RANDF_RANGE(-0.5f, 0.5f) };
    particles_spawn(PARTICLE_SMOKE, pos, vel, size, 2.0f);
}

void particles_dust_cloud(f32 *pos, f32 size, s32 count) {
    particles_spawn_burst(PARTICLE_DUST, pos, count, 2.0f, 1.5f);
}

void particles_explosion(f32 *pos, f32 size) {
    particles_spawn_burst(PARTICLE_EXPLOSION, pos, 1, 0.0f, 0.3f);
    particles_spawn_burst(PARTICLE_FIRE, pos, 20, 8.0f, 0.8f);
    particles_spawn_burst(PARTICLE_SMOKE, pos, 15, 4.0f, 2.0f);
    particles_spawn_burst(PARTICLE_DEBRIS, pos, 10, 12.0f, 2.0f);
}

void particles_water_splash(f32 *pos, f32 size) {
    particles_spawn_burst(PARTICLE_WATER_SPLASH, pos, 15, 6.0f, 1.0f);
    particles_spawn_burst(PARTICLE_WATER_SPRAY, pos, 20, 4.0f, 0.8f);
}

void particles_debris(f32 *pos, f32 *vel, s32 count) {
    s32 i;
    f32 spawn_vel[3];

    for (i = 0; i < count; i++) {
        spawn_vel[0] = vel[0] + RANDF_RANGE(-5.0f, 5.0f);
        spawn_vel[1] = vel[1] + RANDF_RANGE(2.0f, 8.0f);
        spawn_vel[2] = vel[2] + RANDF_RANGE(-5.0f, 5.0f);

        particles_spawn(PARTICLE_DEBRIS, pos, spawn_vel, 0.2f + RANDF() * 0.3f, 2.0f);
    }
}

void particles_burnout_smoke(f32 *pos, f32 intensity) {
    f32 vel[3] = { RANDF_RANGE(-0.3f, 0.3f), RANDF_RANGE(0.5f, 1.5f), RANDF_RANGE(-0.3f, 0.3f) };
    particles_spawn(PARTICLE_TIRE_SMOKE, pos, vel, 0.6f * intensity, 1.0f);
}

void particles_exhaust(f32 *pos, f32 *vel, f32 intensity) {
    f32 spawn_vel[3];
    spawn_vel[0] = vel[0] * -1.0f + RANDF_RANGE(-0.2f, 0.2f);
    spawn_vel[1] = vel[1] + RANDF_RANGE(0.0f, 0.3f);
    spawn_vel[2] = vel[2] * -1.0f + RANDF_RANGE(-0.2f, 0.2f);

    if (intensity > 0.3f) {
        particles_spawn(PARTICLE_EXHAUST, pos, spawn_vel, 0.2f * intensity, 0.4f);
    }
}

void particles_boost_flame(f32 *pos, f32 *dir, f32 intensity) {
    f32 vel[3];
    vel[0] = -dir[0] * 5.0f;
    vel[1] = -dir[1] * 5.0f + 1.0f;
    vel[2] = -dir[2] * 5.0f;

    particles_spawn(PARTICLE_BOOST_TRAIL, pos, vel, 0.4f * intensity, 0.2f);
    particles_spawn(PARTICLE_FIRE, pos, vel, 0.3f * intensity, 0.15f);
}

void particles_pickup_effect(f32 *pos, s32 pickup_type) {
    particles_spawn_burst(PARTICLE_PICKUP_SPARKLE, pos, 12, 3.0f, 0.5f);
}

/* -------------------------------------------------------------------------- */
/* Collision                                                                   */
/* -------------------------------------------------------------------------- */

s32 particles_check_ground(Particle *p, f32 *ground_height, f32 *normal) {
    /* Simple ground plane at y=0 */
    *ground_height = 0.0f;
    normal[0] = 0.0f;
    normal[1] = 1.0f;
    normal[2] = 0.0f;

    return (p->pos[1] <= *ground_height);
}

void particles_bounce_particle(Particle *p, f32 *normal) {
    f32 dot;

    /* Reflect velocity */
    dot = p->vel[0] * normal[0] + p->vel[1] * normal[1] + p->vel[2] * normal[2];

    p->vel[0] = (p->vel[0] - 2.0f * dot * normal[0]) * p->bounce;
    p->vel[1] = (p->vel[1] - 2.0f * dot * normal[1]) * p->bounce;
    p->vel[2] = (p->vel[2] - 2.0f * dot * normal[2]) * p->bounce;
}

/* -------------------------------------------------------------------------- */
/* Settings                                                                    */
/* -------------------------------------------------------------------------- */

void particles_set_gravity(f32 gravity) {
    gParticles.global_gravity = gravity;
}

void particles_set_wind(f32 x, f32 y, f32 z) {
    gParticles.wind[0] = x;
    gParticles.wind[1] = y;
    gParticles.wind[2] = z;
}

void particles_set_time_scale(f32 scale) {
    gParticles.time_scale = scale;
}

void particles_enable(u8 enabled) {
    gParticles.enabled = enabled;
}

/* -------------------------------------------------------------------------- */
/* Queries                                                                     */
/* -------------------------------------------------------------------------- */

s32 particles_get_count(void) {
    return gParticles.num_active;
}

s32 particles_get_emitter_count(void) {
    return gParticles.num_emitters;
}
