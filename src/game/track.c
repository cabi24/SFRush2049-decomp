/**
 * track.c - Track loading and management for Rush 2049 N64
 *
 * Handles loading track data from ROM and managing track state.
 */

#include "game/track.h"

/* External functions */
extern void *malloc(u32 size);
extern void free(void *ptr);
extern void dma_read(void *dest, u32 rom_addr, u32 size);
extern s32 inflate_decompress(void *dest, void *src, u32 src_size);
extern f32 sqrtf(f32 x);
extern f32 fabsf(f32 x);

/* Global track manager */
TrackManager gTracks;

/* Current track pointer */
Track *gCurrentTrack = NULL;

/* Track names */
static const char *sTrackNames[MAX_TRACKS] = {
    "Marina District",
    "Haight-Ashbury",
    "Financial District",
    "Alcatraz",
    "Golden Gate",
    "Fisherman's Wharf",
    "Chinatown",
    "Pacific Heights"
};

/* Battle arena names */
static const char *sBattleArenaNames[MAX_BATTLE_ARENAS] = {
    "The Pit",
    "Industrial Zone",
    "Rooftops",
    "Underground"
};

/* Stunt arena names */
static const char *sStuntArenaNames[MAX_STUNT_ARENAS] = {
    "Stunt Park",
    "Half Pipe",
    "Canyon Run",
    "Sky Ramps"
};

/* Default checkpoint bonus times per difficulty */
static const f32 sDefaultBonusTimes[NUM_DIFFICULTIES] = {
    20.0f,  /* Easy */
    15.0f,  /* Medium */
    10.0f,  /* Hard */
    7.0f    /* Expert */
};

/* Default start times per difficulty (in seconds) */
static const f32 sDefaultStartTimes[NUM_DIFFICULTIES] = {
    90.0f,  /* Easy */
    75.0f,  /* Medium */
    60.0f,  /* Hard */
    45.0f   /* Expert */
};

/* -------------------------------------------------------------------------- */
/* Initialization                                                              */
/* -------------------------------------------------------------------------- */

/**
 * Initialize the track system
 */
void track_init(void) {
    s32 i, j;
    Track *track;
    TrackInfo *info;

    /* Clear all data */
    for (i = 0; i < (s32)sizeof(TrackManager); i++) {
        ((u8*)&gTracks)[i] = 0;
    }

    /* Initialize race tracks */
    for (i = 0; i < MAX_TRACKS; i++) {
        track = &gTracks.tracks[i];
        info = &track->info;

        /* Copy name */
        for (j = 0; sTrackNames[i][j] && j < 31; j++) {
            info->name[j] = sTrackNames[i][j];
        }
        info->name[j] = '\0';

        info->id = (u8)i;
        info->type = TRACK_TYPE_RACE;
        info->difficulty = (u8)(i / 2);  /* Increase difficulty every 2 tracks */
        info->order = (u8)i;

        /* First 4 tracks unlocked by default */
        info->unlocked = (i < 4) ? 1 : 0;

        /* Set default timing */
        track->timing.num_laps = 3;
        for (j = 0; j < NUM_DIFFICULTIES; j++) {
            track->timing.start_time[j] = sDefaultStartTimes[j];
            track->timing.target_time[j] = sDefaultStartTimes[j] * 3.0f;  /* 3 laps */
        }

        /* Set default environment based on track */
        track->environment.type = (u8)(i % NUM_ENVIRONMENTS);
        track->environment.weather = WEATHER_CLEAR;
        track->environment.time_of_day = TIME_NOON;
        track->environment.gravity = 1.0f;

        /* Default fog settings */
        track->environment.fog_enabled = 0;
        track->environment.fog_near = 500.0f;
        track->environment.fog_far = 2000.0f;
        track->environment.fog_color[0] = 128;
        track->environment.fog_color[1] = 128;
        track->environment.fog_color[2] = 140;
        track->environment.fog_color[3] = 255;

        /* Default lighting */
        track->environment.ambient_color[0] = 80;
        track->environment.ambient_color[1] = 80;
        track->environment.ambient_color[2] = 90;
        track->environment.ambient_color[3] = 255;
        track->environment.sun_color[0] = 255;
        track->environment.sun_color[1] = 250;
        track->environment.sun_color[2] = 230;
        track->environment.sun_color[3] = 255;
        track->environment.sun_dir[0] = 0.5f;
        track->environment.sun_dir[1] = 1.0f;
        track->environment.sun_dir[2] = 0.3f;
    }

    /* Initialize battle arenas */
    for (i = 0; i < MAX_BATTLE_ARENAS; i++) {
        track = &gTracks.battle_arenas[i];
        info = &track->info;

        for (j = 0; sBattleArenaNames[i][j] && j < 31; j++) {
            info->name[j] = sBattleArenaNames[i][j];
        }
        info->name[j] = '\0';

        info->id = (u8)i;
        info->type = TRACK_TYPE_BATTLE;
        info->difficulty = 1;
        info->order = (u8)i;
        info->unlocked = (i < 2) ? 1 : 0;

        track->environment.gravity = 1.0f;
    }

    /* Initialize stunt arenas */
    for (i = 0; i < MAX_STUNT_ARENAS; i++) {
        track = &gTracks.stunt_arenas[i];
        info = &track->info;

        for (j = 0; sStuntArenaNames[i][j] && j < 31; j++) {
            info->name[j] = sStuntArenaNames[i][j];
        }
        info->name[j] = '\0';

        info->id = (u8)i;
        info->type = TRACK_TYPE_STUNT;
        info->difficulty = 1;
        info->order = (u8)i;
        info->unlocked = (i < 2) ? 1 : 0;

        track->environment.gravity = 1.0f;
    }

    gTracks.current_track = -1;
    gTracks.initialized = 1;
    gCurrentTrack = NULL;
}

/**
 * Reset track system
 */
void track_reset(void) {
    track_unload();
    gTracks.current_track = -1;
    gCurrentTrack = NULL;
}

/**
 * Shutdown track system
 */
void track_shutdown(void) {
    track_unload();
    gTracks.initialized = 0;
}

/* -------------------------------------------------------------------------- */
/* Track Loading                                                               */
/* -------------------------------------------------------------------------- */

/**
 * Initialize default checkpoints for a track
 */
static void track_init_default_checkpoints(Track *track) {
    s32 i;
    TrackCheckpoint *cp;

    /* Create simple oval checkpoint layout */
    track->num_checkpoints = 4;

    for (i = 0; i < track->num_checkpoints; i++) {
        cp = &track->checkpoints[i];

        /* Position checkpoints in a square pattern */
        cp->pos[0] = (i == 0 || i == 3) ? 0.0f : 500.0f;
        cp->pos[1] = 0.0f;
        cp->pos[2] = (i < 2) ? 0.0f : 500.0f;

        cp->dir[0] = (i == 1) ? 1.0f : (i == 3) ? -1.0f : 0.0f;
        cp->dir[1] = 0.0f;
        cp->dir[2] = (i == 0) ? 1.0f : (i == 2) ? -1.0f : 0.0f;

        cp->width = 50.0f;
        cp->radius = 30.0f;
        cp->logical_cp = (s16)i;
        cp->next_cp = (s16)((i + 1) % track->num_checkpoints);

        /* Set bonus times */
        cp->bonus_time[DIFFICULTY_EASY] = sDefaultBonusTimes[DIFFICULTY_EASY];
        cp->bonus_time[DIFFICULTY_MEDIUM] = sDefaultBonusTimes[DIFFICULTY_MEDIUM];
        cp->bonus_time[DIFFICULTY_HARD] = sDefaultBonusTimes[DIFFICULTY_HARD];
        cp->bonus_time[DIFFICULTY_EXPERT] = sDefaultBonusTimes[DIFFICULTY_EXPERT];
    }

    track->timing.finish_line = 0;
    track->timing.before_finish = (s16)(track->num_checkpoints - 1);
    track->timing.loop_checkpoint = 0;
}

/**
 * Initialize default spawn points
 */
static void track_init_default_spawns(Track *track) {
    s32 i;
    TrackSpawn *spawn;

    track->num_spawns = MAX_SPAWN_POINTS;

    for (i = 0; i < track->num_spawns; i++) {
        spawn = &track->spawns[i];

        /* Grid formation behind start line */
        spawn->pos[0] = ((i % 2) * 15.0f) - 7.5f;  /* Left/right offset */
        spawn->pos[1] = 0.0f;
        spawn->pos[2] = -((i / 2) * 20.0f) - 10.0f;  /* Rows back */

        spawn->dir[0] = 0.0f;
        spawn->dir[1] = 0.0f;
        spawn->dir[2] = 1.0f;  /* Facing forward */

        spawn->rotation = 0.0f;
        spawn->player_index = (u8)i;
        spawn->grid_row = (u8)(i / 2);
        spawn->grid_col = (u8)(i % 2);
        spawn->active = 1;
    }
}

/**
 * Load a race track
 */
s32 track_load(s32 track_id, u32 flags) {
    Track *track;

    if (track_id < 0 || track_id >= MAX_TRACKS) {
        return 0;
    }

    /* Unload current track if any */
    track_unload();

    track = &gTracks.tracks[track_id];
    gTracks.loading = 1;
    gTracks.load_progress = 0;

    /* Apply flags */
    track->mirror = (flags & TRACK_FLAG_MIRROR) ? 1 : 0;
    track->reverse = (flags & TRACK_FLAG_REVERSE) ? 1 : 0;

    /* Initialize defaults if ROM data not available */
    track_init_default_checkpoints(track);
    gTracks.load_progress = 30;

    track_init_default_spawns(track);
    gTracks.load_progress = 50;

    /* Set track bounds (placeholder) */
    track->bounds_min[0] = -1000.0f;
    track->bounds_min[1] = -100.0f;
    track->bounds_min[2] = -1000.0f;
    track->bounds_max[0] = 1000.0f;
    track->bounds_max[1] = 200.0f;
    track->bounds_max[2] = 1000.0f;

    track->track_length = 2000.0f;  /* Placeholder */
    gTracks.load_progress = 80;

    /* Apply environment settings */
    if (flags & TRACK_FLAG_NIGHT) {
        track->environment.time_of_day = TIME_NIGHT;
        track->environment.fog_enabled = 1;
    }
    if (flags & TRACK_FLAG_RAIN) {
        track->environment.weather = WEATHER_RAIN;
        track->environment.fog_enabled = 1;
    }

    track->loaded = 1;
    gTracks.load_progress = 100;
    gTracks.loading = 0;
    gTracks.current_track = track_id;
    gTracks.current_type = TRACK_TYPE_RACE;
    gCurrentTrack = track;

    return 1;
}

/**
 * Load a battle arena
 */
s32 track_load_battle(s32 arena_id) {
    Track *track;

    if (arena_id < 0 || arena_id >= MAX_BATTLE_ARENAS) {
        return 0;
    }

    track_unload();

    track = &gTracks.battle_arenas[arena_id];
    gTracks.loading = 1;

    /* Initialize battle-specific spawns */
    track_init_default_spawns(track);

    /* Battle arenas have pickup spawns */
    track->num_pickup_spawns = 8;

    track->loaded = 1;
    gTracks.loading = 0;
    gTracks.current_track = arena_id;
    gTracks.current_type = TRACK_TYPE_BATTLE;
    gCurrentTrack = track;

    return 1;
}

/**
 * Load a stunt arena
 */
s32 track_load_stunt(s32 arena_id) {
    Track *track;

    if (arena_id < 0 || arena_id >= MAX_STUNT_ARENAS) {
        return 0;
    }

    track_unload();

    track = &gTracks.stunt_arenas[arena_id];
    gTracks.loading = 1;

    track_init_default_spawns(track);

    track->loaded = 1;
    gTracks.loading = 0;
    gTracks.current_track = arena_id;
    gTracks.current_type = TRACK_TYPE_STUNT;
    gCurrentTrack = track;

    return 1;
}

/**
 * Unload current track
 */
void track_unload(void) {
    if (gCurrentTrack == NULL) {
        return;
    }

    /* Free loaded data */
    if (gCurrentTrack->geometry_data != NULL) {
        free(gCurrentTrack->geometry_data);
        gCurrentTrack->geometry_data = NULL;
    }

    if (gCurrentTrack->collision_data != NULL) {
        free(gCurrentTrack->collision_data);
        gCurrentTrack->collision_data = NULL;
    }

    gCurrentTrack->loaded = 0;
    gCurrentTrack = NULL;
    gTracks.current_track = -1;
}

/**
 * Check if track is loaded
 */
s32 track_is_loaded(void) {
    return (gCurrentTrack != NULL && gCurrentTrack->loaded);
}

/**
 * Get loading progress
 */
s32 track_get_load_progress(void) {
    return gTracks.load_progress;
}

/* -------------------------------------------------------------------------- */
/* Track Info Access                                                           */
/* -------------------------------------------------------------------------- */

TrackInfo* track_get_info(s32 track_id) {
    if (track_id < 0 || track_id >= MAX_TRACKS) {
        return NULL;
    }
    return &gTracks.tracks[track_id].info;
}

const char* track_get_name(s32 track_id) {
    if (track_id < 0 || track_id >= MAX_TRACKS) {
        return "Unknown";
    }
    return gTracks.tracks[track_id].info.name;
}

s32 track_get_difficulty(s32 track_id) {
    if (track_id < 0 || track_id >= MAX_TRACKS) {
        return 0;
    }
    return gTracks.tracks[track_id].info.difficulty;
}

s32 track_is_unlocked(s32 track_id) {
    if (track_id < 0 || track_id >= MAX_TRACKS) {
        return 0;
    }
    return gTracks.tracks[track_id].info.unlocked;
}

s32 track_get_num_laps(s32 track_id) {
    if (track_id < 0 || track_id >= MAX_TRACKS) {
        return 3;
    }
    return gTracks.tracks[track_id].timing.num_laps;
}

f32 track_get_length(s32 track_id) {
    if (track_id < 0 || track_id >= MAX_TRACKS) {
        return 0.0f;
    }
    return gTracks.tracks[track_id].track_length;
}

/* -------------------------------------------------------------------------- */
/* Checkpoint Access                                                           */
/* -------------------------------------------------------------------------- */

s32 track_get_num_checkpoints(void) {
    if (gCurrentTrack == NULL) {
        return 0;
    }
    return gCurrentTrack->num_checkpoints;
}

TrackCheckpoint* track_get_checkpoint(s32 index) {
    if (gCurrentTrack == NULL) {
        return NULL;
    }
    if (index < 0 || index >= gCurrentTrack->num_checkpoints) {
        return NULL;
    }
    return &gCurrentTrack->checkpoints[index];
}

s32 track_get_next_checkpoint(s32 current) {
    TrackCheckpoint *cp;

    if (gCurrentTrack == NULL) {
        return 0;
    }

    cp = track_get_checkpoint(current);
    if (cp == NULL) {
        return 0;
    }

    return cp->next_cp;
}

s32 track_get_finish_checkpoint(void) {
    if (gCurrentTrack == NULL) {
        return 0;
    }
    return gCurrentTrack->timing.finish_line;
}

f32 track_get_checkpoint_bonus(s32 cp_index, s32 difficulty) {
    TrackCheckpoint *cp;

    cp = track_get_checkpoint(cp_index);
    if (cp == NULL) {
        return 0.0f;
    }

    if (difficulty < 0 || difficulty >= NUM_DIFFICULTIES) {
        difficulty = DIFFICULTY_MEDIUM;
    }

    return cp->bonus_time[difficulty];
}

/* -------------------------------------------------------------------------- */
/* Spawn Points                                                                */
/* -------------------------------------------------------------------------- */

s32 track_get_num_spawns(void) {
    if (gCurrentTrack == NULL) {
        return 0;
    }
    return gCurrentTrack->num_spawns;
}

TrackSpawn* track_get_spawn(s32 index) {
    if (gCurrentTrack == NULL) {
        return NULL;
    }
    if (index < 0 || index >= gCurrentTrack->num_spawns) {
        return NULL;
    }
    return &gCurrentTrack->spawns[index];
}

void track_get_spawn_position(s32 player, f32 *pos, f32 *dir) {
    TrackSpawn *spawn;

    spawn = track_get_spawn(player);
    if (spawn == NULL) {
        pos[0] = pos[1] = pos[2] = 0.0f;
        dir[0] = dir[2] = 0.0f;
        dir[1] = 1.0f;
        return;
    }

    pos[0] = spawn->pos[0];
    pos[1] = spawn->pos[1];
    pos[2] = spawn->pos[2];

    dir[0] = spawn->dir[0];
    dir[1] = spawn->dir[1];
    dir[2] = spawn->dir[2];
}

void track_get_grid_position(s32 grid_pos, f32 *pos, f32 *dir) {
    track_get_spawn_position(grid_pos, pos, dir);
}

/* -------------------------------------------------------------------------- */
/* Shortcuts                                                                   */
/* -------------------------------------------------------------------------- */

s32 track_get_num_shortcuts(void) {
    if (gCurrentTrack == NULL) {
        return 0;
    }
    return gCurrentTrack->num_shortcuts;
}

TrackShortcut* track_get_shortcut(s32 index) {
    if (gCurrentTrack == NULL) {
        return NULL;
    }
    if (index < 0 || index >= gCurrentTrack->num_shortcuts) {
        return NULL;
    }
    return &gCurrentTrack->shortcuts[index];
}

s32 track_check_shortcut_entry(f32 *pos) {
    s32 i;
    TrackShortcut *sc;
    f32 dx, dy, dz, dist;

    if (gCurrentTrack == NULL) {
        return -1;
    }

    for (i = 0; i < gCurrentTrack->num_shortcuts; i++) {
        sc = &gCurrentTrack->shortcuts[i];

        dx = pos[0] - sc->entry_pos[0];
        dy = pos[1] - sc->entry_pos[1];
        dz = pos[2] - sc->entry_pos[2];
        dist = sqrtf(dx*dx + dy*dy + dz*dz);

        if (dist < sc->entry_radius) {
            return i;
        }
    }

    return -1;
}

/* -------------------------------------------------------------------------- */
/* Environment                                                                 */
/* -------------------------------------------------------------------------- */

TrackEnvironment* track_get_environment(void) {
    if (gCurrentTrack == NULL) {
        return NULL;
    }
    return &gCurrentTrack->environment;
}

void track_set_weather(s32 weather) {
    if (gCurrentTrack == NULL) {
        return;
    }
    if (weather >= 0 && weather < NUM_WEATHER_TYPES) {
        gCurrentTrack->environment.weather = (u8)weather;
    }
}

void track_set_time_of_day(s32 time) {
    if (gCurrentTrack == NULL) {
        return;
    }
    if (time >= 0 && time < NUM_TIMES_OF_DAY) {
        gCurrentTrack->environment.time_of_day = (u8)time;
    }
}

void track_apply_environment(void) {
    /* Would apply fog, lighting, etc. to the renderer */
    if (gCurrentTrack == NULL) {
        return;
    }

    /* Environment application would happen here */
}

/* -------------------------------------------------------------------------- */
/* Timing                                                                      */
/* -------------------------------------------------------------------------- */

TrackTiming* track_get_timing(void) {
    if (gCurrentTrack == NULL) {
        return NULL;
    }
    return &gCurrentTrack->timing;
}

f32 track_get_start_time(s32 difficulty) {
    if (gCurrentTrack == NULL) {
        return 60.0f;
    }
    if (difficulty < 0 || difficulty >= NUM_DIFFICULTIES) {
        difficulty = DIFFICULTY_MEDIUM;
    }
    return gCurrentTrack->timing.start_time[difficulty];
}

f32 track_get_target_time(s32 difficulty) {
    if (gCurrentTrack == NULL) {
        return 180.0f;
    }
    if (difficulty < 0 || difficulty >= NUM_DIFFICULTIES) {
        difficulty = DIFFICULTY_MEDIUM;
    }
    return gCurrentTrack->timing.target_time[difficulty];
}

f32 track_get_par_time(void) {
    if (gCurrentTrack == NULL) {
        return 120.0f;
    }
    return gCurrentTrack->timing.par_time;
}

/* -------------------------------------------------------------------------- */
/* Collision Helpers                                                           */
/* -------------------------------------------------------------------------- */

s32 track_get_surface_at(f32 *pos) {
    /* Would query collision data */
    return SURFACE_ASPHALT;
}

f32 track_get_height_at(f32 x, f32 z) {
    /* Would query collision data */
    return 0.0f;
}

s32 track_is_on_track(f32 *pos) {
    if (gCurrentTrack == NULL) {
        return 0;
    }

    /* Simple bounds check */
    if (pos[0] < gCurrentTrack->bounds_min[0] ||
        pos[0] > gCurrentTrack->bounds_max[0] ||
        pos[2] < gCurrentTrack->bounds_min[2] ||
        pos[2] > gCurrentTrack->bounds_max[2]) {
        return 0;
    }

    return 1;
}

s32 track_is_in_bounds(f32 *pos) {
    if (gCurrentTrack == NULL) {
        return 0;
    }

    if (pos[0] < gCurrentTrack->bounds_min[0] ||
        pos[0] > gCurrentTrack->bounds_max[0] ||
        pos[1] < gCurrentTrack->bounds_min[1] ||
        pos[1] > gCurrentTrack->bounds_max[1] ||
        pos[2] < gCurrentTrack->bounds_min[2] ||
        pos[2] > gCurrentTrack->bounds_max[2]) {
        return 0;
    }

    return 1;
}

/* -------------------------------------------------------------------------- */
/* Mirror/Reverse                                                              */
/* -------------------------------------------------------------------------- */

void track_set_mirror(s32 enabled) {
    if (gCurrentTrack == NULL) {
        return;
    }
    gCurrentTrack->mirror = enabled ? 1 : 0;
}

void track_set_reverse(s32 enabled) {
    if (gCurrentTrack == NULL) {
        return;
    }
    gCurrentTrack->reverse = enabled ? 1 : 0;
}

s32 track_is_mirrored(void) {
    if (gCurrentTrack == NULL) {
        return 0;
    }
    return gCurrentTrack->mirror;
}

s32 track_is_reversed(void) {
    if (gCurrentTrack == NULL) {
        return 0;
    }
    return gCurrentTrack->reverse;
}

/* -------------------------------------------------------------------------- */
/* Utility                                                                     */
/* -------------------------------------------------------------------------- */

f32 track_get_distance_along(f32 *pos) {
    /* Would calculate distance along track centerline */
    if (gCurrentTrack == NULL) {
        return 0.0f;
    }

    /* Simplified: use Z coordinate as distance */
    return pos[2];
}

f32 track_get_progress_percent(f32 *pos) {
    f32 distance;
    f32 length;

    if (gCurrentTrack == NULL) {
        return 0.0f;
    }

    distance = track_get_distance_along(pos);
    length = gCurrentTrack->track_length;

    if (length <= 0.0f) {
        return 0.0f;
    }

    return (distance / length) * 100.0f;
}

void track_get_nearest_point(f32 *pos, f32 *nearest, f32 *normal) {
    /* Would find nearest point on track surface */
    if (nearest != NULL) {
        nearest[0] = pos[0];
        nearest[1] = 0.0f;
        nearest[2] = pos[2];
    }

    if (normal != NULL) {
        normal[0] = 0.0f;
        normal[1] = 1.0f;
        normal[2] = 0.0f;
    }
}

s32 track_get_segment_at(f32 *pos) {
    /* Would return which track segment the position is in */
    return 0;
}
