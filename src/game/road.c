/**
 * road.c - Track/road surface system for Rush 2049 N64
 *
 * Based on arcade game/road.c
 * Handles tire/surface interaction, body collision, and surface properties.
 *
 * Key functions:
 * - road_update (arcade: road()) - Main per-frame road interaction
 * - road_tire_surface (arcade: tiresurf) - Tire-to-ground contact
 * - road_body_surface (arcade: bodysurf) - Body corner collision
 */

#include "types.h"
#include "game/road.h"
#include "game/physics.h"
#include "game/structs.h"

/* External declarations */
extern u8 gstate;
extern u32 frame_counter;
extern CarData car_array[];
extern s32 num_active_cars;
extern s32 this_car;

/* External functions */
extern f32 sqrtf(f32 x);
extern void collision(CarPhysics *m);

/* Global state */
TrackData *gCurrentTrack = NULL;
s32 gTrackLoaded = 0;

/* Friction table indexed by road code */
static const f32 friction_table[] = {
    FRICTION_ASPHALT,   /* 0: Asphalt */
    FRICTION_DIRT,      /* 1: Dirt */
    FRICTION_GRASS,     /* 2: Grass */
    0.0f,               /* 3: Air (no friction) */
    FRICTION_WATER,     /* 4: Water */
    FRICTION_ICE,       /* 5: Ice */
    FRICTION_SAND,      /* 6: Sand */
    FRICTION_BOOST,     /* 7: Boost */
    FRICTION_ASPHALT,   /* 8: Jump */
    0.0f,               /* 9: Wall */
    FRICTION_ASPHALT,   /* 10: Shortcut */
};

/* Temporary vectors for calculations */
static f32 BODYRWV[3];
static f32 tuvs[3][3];

/**
 * road_init - Initialize road system
 */
void road_init(void) {
    gCurrentTrack = NULL;
    gTrackLoaded = 0;
}

/**
 * road_reset - Reset road system for new race
 */
void road_reset(void) {
    /* Track remains loaded, just reset state */
}

/**
 * init_road - Initialize road state for a car (arcade compatible)
 * Based on arcade: road.c:init_road()
 *
 * @param m Car physics state
 */
void init_road(CarPhysics *m) {
    f32 temp[3];
    s32 i;

    /* Initialize tire positions in real world */
    for (i = 0; i < 4; i++) {
        /* Copy tire position and transform to world coords */
        temp[0] = m->TIRER[i][0];
        temp[1] = m->TIRER[i][1];
        temp[2] = m->TIRER[i][2];

        /* Transform body to real world */
        body_to_rw(temp, m->TIRER[i], &m->UV);

        /* Add car position */
        m->TIRER[i][0] += m->RWR[0];
        m->TIRER[i][1] += m->RWR[1];
        m->TIRER[i][2] += m->RWR[2];

        /* Initialize suspension */
        m->suscomp[i] = 0.0f;
        m->roadcode[i] = ROAD_AIR;

        /* Initialize body corner positions */
        temp[0] = m->BODYR[i][0];
        temp[1] = m->BODYR[i][1];
        temp[2] = m->BODYR[i][2];

        body_to_rw(temp, m->BODYR[i], &m->UV);

        m->BODYR[i][0] += m->RWR[0];
        m->BODYR[i][1] += m->RWR[1];
        m->BODYR[i][2] += m->RWR[2];
    }
}

/**
 * road_update - Main road interaction update
 * Based on arcade: road.c:road()
 *
 * @param m Car physics state
 */
void road_update(CarPhysics *m) {
    f32 ground_pos[3], temp[3];
    f32 last_airdist;
    s32 i, j;

    /* Zero surface forces */
    for (i = 0; i < 3; i++) {
        m->CENTERFORCE[i] = 0.0f;
        for (j = 0; j < 4; j++) {
            m->BODYFORCE[j][i] = 0.0f;
        }
    }

    /* Process each tire/body corner */
    for (i = 0; i < 4; i++) {
        /* Find tire interaction with surfaces */
        road_tire_surface(m, m->TIRER[i], ground_pos,
                         &m->roadcode[i], m->roaduvs[i].fpuvs, i);

        /* Calculate body corner velocity: BODYV = V + (W x BODYR) */
        vec_cross(m->W, m->BODYR[i], temp);
        vec_add(temp, m->V, temp);
        body_to_rw(temp, BODYRWV, &m->UV);

        /* Find body surface interaction */
        road_body_surface(m, m->BODYR[i], BODYRWV, tuvs, i);

        /* Update suspension compression */
        last_airdist = m->tires[i].load;  /* Using load as airdist temp */
        m->suscomp[i] = ground_pos[ZCOMP];

        /* Clamp suspension compression */
        if (m->suscomp[i] > 3.0f) {
            m->suscomp[i] = 3.0f;
        } else if (m->suscomp[i] < -3.0f) {
            m->suscomp[i] = -3.0f;
        }

        /* Mark as in air if no ground contact */
        if (m->suscomp[i] < 0.0f) {
            m->roadcode[i] = ROAD_AIR;
        }

        /* Store air distance velocity for suspension damping */
        m->tires[i].load = m->suscomp[i];
    }

    /* Check for collisions with other cars */
    collision(m);
}

/**
 * road_tire_surface - Find tire-to-ground interaction
 * Based on arcade: tiresurf()
 *
 * @param m Car physics state
 * @param tire_pos World position of tire
 * @param ground_pos Output: ground position beneath tire
 * @param roadcode Output: surface type code
 * @param road_uvs Output: road surface orientation
 * @param tire_index Which tire (0-3)
 */
void road_tire_surface(CarPhysics *m, f32 tire_pos[3], f32 ground_pos[3],
                       u8 *roadcode, f32 road_uvs[3][3], s32 tire_index) {
    f32 height;

    /* Default to flat ground at tire position */
    ground_pos[0] = tire_pos[0];
    ground_pos[1] = tire_pos[1];
    ground_pos[2] = 0.0f;  /* Ground height */

    /* Default road orientation (flat) */
    road_uvs[0][0] = 1.0f; road_uvs[0][1] = 0.0f; road_uvs[0][2] = 0.0f;
    road_uvs[1][0] = 0.0f; road_uvs[1][1] = 1.0f; road_uvs[1][2] = 0.0f;
    road_uvs[2][0] = 0.0f; road_uvs[2][1] = 0.0f; road_uvs[2][2] = 1.0f;

    /* Calculate height above ground */
    height = tire_pos[2] - ground_pos[2];

    /* Determine surface type based on height */
    if (height > 0.5f) {
        *roadcode = ROAD_AIR;
        ground_pos[2] = tire_pos[2] - height;
    } else if (height < -0.1f) {
        /* Below ground - push up */
        *roadcode = ROAD_ASPHALT;
        ground_pos[2] = tire_pos[2];
    } else {
        /* On ground */
        *roadcode = ROAD_ASPHALT;
        ground_pos[2] = tire_pos[2] - height;
    }

    /* Update tire ground state */
    m->tires[tire_index].on_ground = (*roadcode != ROAD_AIR) ? 1 : 0;
    m->tires[tire_index].friction = road_get_friction(*roadcode);
}

/**
 * road_body_surface - Find body corner collision
 * Based on arcade: bodysurf()
 *
 * @param m Car physics state
 * @param body_pos World position of body corner
 * @param body_vel Velocity of body corner
 * @param uvs Output: surface orientation
 * @param corner_index Which corner (0-3)
 */
void road_body_surface(CarPhysics *m, f32 body_pos[3], f32 body_vel[3],
                       f32 uvs[3][3], s32 corner_index) {
    f32 height;
    f32 force_mag;

    /* Default orientation */
    uvs[0][0] = 1.0f; uvs[0][1] = 0.0f; uvs[0][2] = 0.0f;
    uvs[1][0] = 0.0f; uvs[1][1] = 1.0f; uvs[1][2] = 0.0f;
    uvs[2][0] = 0.0f; uvs[2][1] = 0.0f; uvs[2][2] = 1.0f;

    /* Check if body corner is below ground */
    height = body_pos[2];  /* Assuming ground at Z=0 */

    if (height < 0.0f) {
        /* Body corner is below ground - apply upward force */
        force_mag = -height * 50000.0f;  /* Spring constant */

        /* Add damping based on velocity */
        if (body_vel[2] < 0.0f) {
            force_mag += -body_vel[2] * 5000.0f;
        }

        /* Apply force at body corner */
        m->BODYFORCE[corner_index][2] = force_mag;
    }
}

/**
 * road_get_friction - Get friction coefficient for surface type
 *
 * @param roadcode Surface type code
 * @return Friction coefficient
 */
f32 road_get_friction(u8 roadcode) {
    if (roadcode >= sizeof(friction_table) / sizeof(friction_table[0])) {
        return FRICTION_ASPHALT;
    }
    return friction_table[roadcode];
}

/**
 * road_get_flags - Get flags for surface type
 *
 * @param roadcode Surface type code
 * @return Surface flags
 */
u8 road_get_flags(u8 roadcode) {
    switch (roadcode) {
        case ROAD_BOOST:
            return ROAD_FLAG_BOOST;
        case ROAD_SHORTCUT:
            return ROAD_FLAG_SHORTCUT;
        case ROAD_DIRT:
        case ROAD_GRASS:
            return ROAD_FLAG_BUMPY;
        case ROAD_WATER:
            return ROAD_FLAG_WET;
        default:
            return 0;
    }
}

/**
 * road_is_boost_surface - Check if surface provides boost
 *
 * @param roadcode Surface type code
 * @return Non-zero if boost surface
 */
s32 road_is_boost_surface(u8 roadcode) {
    return (roadcode == ROAD_BOOST);
}

/**
 * road_is_shortcut - Check if on shortcut path
 *
 * @param roadcode Surface type code
 * @return Non-zero if shortcut
 */
s32 road_is_shortcut(u8 roadcode) {
    return (roadcode == ROAD_SHORTCUT);
}

/**
 * road_load_track - Load track data
 *
 * @param track_id Track identifier
 */
void road_load_track(s32 track_id) {
    /* Track loading would be done from ROM data */
    gTrackLoaded = 1;
}

/**
 * road_unload_track - Unload current track
 */
void road_unload_track(void) {
    gCurrentTrack = NULL;
    gTrackLoaded = 0;
}

/**
 * road_get_track_data - Get current track data
 *
 * @return Pointer to track data or NULL
 */
TrackData* road_get_track_data(void) {
    return gCurrentTrack;
}

/**
 * road_get_track_length - Get total track length
 *
 * @return Track length in feet
 */
f32 road_get_track_length(void) {
    if (gCurrentTrack != NULL) {
        return gCurrentTrack->total_length;
    }
    return 0.0f;
}

/**
 * road_get_segment_at_pos - Get road segment at position
 *
 * @param pos World position
 * @return Segment index or -1
 */
s32 road_get_segment_at_pos(f32 pos[3]) {
    /* Would search track segments */
    return -1;
}

/**
 * road_get_road_direction - Get road direction at position
 *
 * @param pos World position
 * @param dir Output: direction vector
 */
void road_get_road_direction(f32 pos[3], f32 dir[3]) {
    /* Default to forward */
    dir[0] = 1.0f;
    dir[1] = 0.0f;
    dir[2] = 0.0f;
}

/**
 * road_get_road_width - Get road width at position
 *
 * @param pos World position
 * @return Road width in feet
 */
f32 road_get_road_width(f32 pos[3]) {
    /* Default road width */
    return 40.0f;
}

/**
 * uvinterp - Interpolate unit vectors
 * Based on arcade: road.c:uvinterp()
 *
 * Interpolates c1 out of s1 from uv1 and remainder from uv2
 *
 * @param uv1 First unit vector array (9 shorts)
 * @param c1 Weight for uv1
 * @param s1 Total weight
 * @param uv2 Second unit vector array
 * @param res Output result
 */
void uvinterp(s16 *uv1, s32 c1, s32 s1, s16 *uv2, s16 *res) {
    s32 i;
    f32 fc1, fc2, fis1;

    fc1 = (f32)c1;
    fc2 = (f32)(s1 - c1);
    fis1 = 1.0f / (f32)s1;

    for (i = 0; i < 9; i++) {
        res[i] = (s16)((uv1[i] * fc1 + uv2[i] * fc2) * fis1);
    }
}

/**
 * vecinterp - Interpolate integer vectors
 * Based on arcade: road.c:vecinterp()
 *
 * @param v1 First vector
 * @param c1 Weight for v1
 * @param s1 Total weight
 * @param v2 Second vector
 * @param res Output result
 */
void vecinterp(s32 v1[3], s32 c1, s32 s1, s32 v2[3], s32 res[3]) {
    s32 i;
    f32 fc1, fc2, fis1;

    fc1 = (f32)c1;
    fc2 = (f32)(s1 - c1);
    fis1 = 1.0f / (f32)s1;

    for (i = 0; i < 3; i++) {
        res[i] = (s32)((v1[i] * fc1 + v2[i] * fc2) * fis1);
    }
}
