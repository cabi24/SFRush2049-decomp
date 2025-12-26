/**
 * track.h - Track loading and management for Rush 2049 N64
 *
 * Handles loading track data from ROM, including geometry,
 * checkpoints, spawn points, and environment settings.
 */

#ifndef TRACK_H
#define TRACK_H

#include "types.h"

/* Track limits */
#define MAX_TRACKS              8       /* Maximum race tracks */
#define MAX_BATTLE_ARENAS       4       /* Battle mode arenas */
#define MAX_STUNT_ARENAS        4       /* Stunt mode arenas */
#define MAX_CHECKPOINTS         20      /* Checkpoints per track */
#define MAX_SPAWN_POINTS        8       /* Spawn points per track */
#define MAX_SHORTCUTS           8       /* Shortcuts per track */
#define MAX_HAZARDS             16      /* Hazards per track */
#define MAX_PICKUPS_SPAWNS      32      /* Pickup spawn locations */

/* Track types */
#define TRACK_TYPE_RACE         0       /* Normal race track */
#define TRACK_TYPE_BATTLE       1       /* Battle arena */
#define TRACK_TYPE_STUNT        2       /* Stunt arena */

/* Track flags */
#define TRACK_FLAG_MIRROR       (1 << 0)    /* Track is mirrored */
#define TRACK_FLAG_NIGHT        (1 << 1)    /* Night version */
#define TRACK_FLAG_RAIN         (1 << 2)    /* Rain weather */
#define TRACK_FLAG_UNLOCKED     (1 << 3)    /* Track is unlocked */
#define TRACK_FLAG_HIDDEN       (1 << 4)    /* Hidden track */
#define TRACK_FLAG_REVERSE      (1 << 5)    /* Reverse direction */

/* Difficulty levels */
#define DIFFICULTY_EASY         0
#define DIFFICULTY_MEDIUM       1
#define DIFFICULTY_HARD         2
#define DIFFICULTY_EXPERT       3
#define NUM_DIFFICULTIES        4

/* Environment types */
#define ENV_CITY                0       /* City streets */
#define ENV_COASTAL             1       /* Coastal/beach */
#define ENV_INDUSTRIAL          2       /* Industrial area */
#define ENV_DESERT              3       /* Desert canyon */
#define ENV_MOUNTAIN            4       /* Mountain roads */
#define ENV_TUNNEL              5       /* Underground tunnels */
#define NUM_ENVIRONMENTS        6

/* Weather types */
#define WEATHER_CLEAR           0
#define WEATHER_CLOUDY          1
#define WEATHER_RAIN            2
#define WEATHER_FOG             3
#define WEATHER_NIGHT           4
#define NUM_WEATHER_TYPES       5

/* Time of day */
#define TIME_MORNING            0
#define TIME_NOON               1
#define TIME_EVENING            2
#define TIME_NIGHT              3
#define NUM_TIMES_OF_DAY        4

/* Surface types */
#define SURFACE_ASPHALT         0       /* Normal road */
#define SURFACE_CONCRETE        1       /* Concrete */
#define SURFACE_DIRT            2       /* Dirt/gravel */
#define SURFACE_GRASS           3       /* Off-road grass */
#define SURFACE_WATER           4       /* Shallow water */
#define SURFACE_ICE             5       /* Icy surface */
#define SURFACE_BOOST           6       /* Speed boost pad */
#define SURFACE_RAMP            7       /* Jump ramp */
#define NUM_SURFACE_TYPES       8

/* Checkpoint structure */
typedef struct TrackCheckpoint {
    f32     pos[3];             /* Position */
    f32     dir[3];             /* Direction vector */
    f32     width;              /* Checkpoint width */
    f32     radius;             /* Trigger radius */
    s16     logical_cp;         /* Logical order (0 = finish) */
    s16     next_cp;            /* Next checkpoint index */
    f32     bonus_time[NUM_DIFFICULTIES];   /* Bonus time per difficulty */
} TrackCheckpoint;

/* Spawn point structure */
typedef struct TrackSpawn {
    f32     pos[3];             /* Position */
    f32     dir[3];             /* Initial direction */
    f32     rotation;           /* Yaw rotation */
    u8      player_index;       /* Preferred player (0-7) */
    u8      grid_row;           /* Grid position row */
    u8      grid_col;           /* Grid position column */
    u8      active;             /* Spawn point available */
} TrackSpawn;

/* Shortcut definition */
typedef struct TrackShortcut {
    f32     entry_pos[3];       /* Entry point */
    f32     exit_pos[3];        /* Exit point */
    f32     entry_radius;       /* Entry trigger radius */
    f32     time_save;          /* Approximate time savings */
    u8      difficulty;         /* Minimum difficulty to appear */
    u8      hidden;             /* Is a secret shortcut */
    u8      pad[2];
} TrackShortcut;

/* Hazard definition */
typedef struct TrackHazard {
    f32     pos[3];             /* Position */
    f32     radius;             /* Effect radius */
    u8      type;               /* Hazard type */
    u8      damage;             /* Damage amount */
    u8      active;             /* Currently active */
    u8      respawn;            /* Respawns after triggered */
    u32     respawn_time;       /* Frames until respawn */
} TrackHazard;

/* Pickup spawn location */
typedef struct TrackPickupSpawn {
    f32     pos[3];             /* Position */
    u8      type;               /* Pickup type (battle mode) */
    u8      active;             /* Spawn active */
    u8      pad[2];
    u32     respawn_time;       /* Respawn delay */
} TrackPickupSpawn;

/* Track segment for collision/AI */
typedef struct TrackSegment {
    f32     center[3];          /* Segment center */
    f32     width;              /* Road width */
    f32     banking;            /* Road banking angle */
    u8      surface;            /* Surface type */
    u8      flags;              /* Segment flags */
    s16     next_segment;       /* Next segment index */
    s16     left_barrier;       /* Left barrier type */
    s16     right_barrier;      /* Right barrier type */
} TrackSegment;

/* Environment settings */
typedef struct TrackEnvironment {
    u8      type;               /* ENV_* type */
    u8      weather;            /* WEATHER_* type */
    u8      time_of_day;        /* TIME_* */
    u8      fog_enabled;        /* Fog active */
    f32     fog_near;           /* Fog start distance */
    f32     fog_far;            /* Fog end distance */
    u8      fog_color[4];       /* Fog RGBA */
    u8      ambient_color[4];   /* Ambient light RGBA */
    u8      sun_color[4];       /* Sun/main light RGBA */
    f32     sun_dir[3];         /* Sun direction vector */
    f32     gravity;            /* Track gravity multiplier */
} TrackEnvironment;

/* Track timing data */
typedef struct TrackTiming {
    f32     start_time[NUM_DIFFICULTIES];   /* Initial time per difficulty */
    f32     target_time[NUM_DIFFICULTIES];  /* Target finish time */
    f32     par_time;           /* Par time for track */
    f32     record_time;        /* Track record */
    s16     num_laps;           /* Number of laps */
    s16     loop_checkpoint;    /* Checkpoint to loop to */
    s16     finish_line;        /* Finish line checkpoint index */
    s16     before_finish;      /* Checkpoint before finish */
} TrackTiming;

/* Track metadata */
typedef struct TrackInfo {
    char    name[32];           /* Track display name */
    char    filename[16];       /* ROM data filename */
    u8      id;                 /* Track ID */
    u8      type;               /* TRACK_TYPE_* */
    u8      environment;        /* ENV_* */
    u8      difficulty;         /* Base difficulty (0-3) */
    u8      unlocked;           /* Track available */
    u8      mirror_unlocked;    /* Mirror version available */
    u8      order;              /* Selection order */
    u8      pad;
    u32     flags;              /* TRACK_FLAG_* */
} TrackInfo;

/* Main track structure */
typedef struct Track {
    /* Metadata */
    TrackInfo       info;

    /* Checkpoints */
    TrackCheckpoint checkpoints[MAX_CHECKPOINTS];
    s32             num_checkpoints;

    /* Spawn points */
    TrackSpawn      spawns[MAX_SPAWN_POINTS];
    s32             num_spawns;

    /* Shortcuts */
    TrackShortcut   shortcuts[MAX_SHORTCUTS];
    s32             num_shortcuts;

    /* Hazards (battle mode) */
    TrackHazard     hazards[MAX_HAZARDS];
    s32             num_hazards;

    /* Pickup spawns (battle mode) */
    TrackPickupSpawn pickup_spawns[MAX_PICKUPS_SPAWNS];
    s32             num_pickup_spawns;

    /* Timing */
    TrackTiming     timing;

    /* Environment */
    TrackEnvironment environment;

    /* Track bounds */
    f32             bounds_min[3];
    f32             bounds_max[3];
    f32             track_length;       /* Total track length */

    /* ROM data pointers */
    u32             geometry_rom_addr;  /* ROM address of geometry */
    u32             collision_rom_addr; /* ROM address of collision */
    u32             geometry_size;      /* Size in bytes */
    u32             collision_size;     /* Size in bytes */

    /* Loaded data pointers */
    void            *geometry_data;     /* Loaded geometry */
    void            *collision_data;    /* Loaded collision */

    /* State */
    u8              loaded;             /* Track data loaded */
    u8              mirror;             /* Playing mirrored */
    u8              reverse;            /* Playing reversed */
    u8              pad;

} Track;

/* Track manager state */
typedef struct TrackManager {
    Track           tracks[MAX_TRACKS];
    Track           battle_arenas[MAX_BATTLE_ARENAS];
    Track           stunt_arenas[MAX_STUNT_ARENAS];

    s32             current_track;          /* Currently loaded track */
    s32             current_type;           /* Type of current track */

    u8              initialized;
    u8              loading;                /* Currently loading */
    u8              load_progress;          /* Loading progress 0-100 */
    u8              pad;

} TrackManager;

/* Global track manager */
extern TrackManager gTracks;

/* Current track shorthand */
extern Track *gCurrentTrack;

/* Initialization */
void track_init(void);
void track_reset(void);
void track_shutdown(void);

/* Track loading */
s32 track_load(s32 track_id, u32 flags);
s32 track_load_battle(s32 arena_id);
s32 track_load_stunt(s32 arena_id);
void track_unload(void);
s32 track_is_loaded(void);
s32 track_get_load_progress(void);

/* Track info access */
TrackInfo* track_get_info(s32 track_id);
const char* track_get_name(s32 track_id);
s32 track_get_difficulty(s32 track_id);
s32 track_is_unlocked(s32 track_id);
s32 track_get_num_laps(s32 track_id);
f32 track_get_length(s32 track_id);

/* Checkpoint access */
s32 track_get_num_checkpoints(void);
TrackCheckpoint* track_get_checkpoint(s32 index);
s32 track_get_next_checkpoint(s32 current);
s32 track_get_finish_checkpoint(void);
f32 track_get_checkpoint_bonus(s32 cp_index, s32 difficulty);

/* Spawn points */
s32 track_get_num_spawns(void);
TrackSpawn* track_get_spawn(s32 index);
void track_get_spawn_position(s32 player, f32 *pos, f32 *dir);
void track_get_grid_position(s32 grid_pos, f32 *pos, f32 *dir);

/* Shortcuts */
s32 track_get_num_shortcuts(void);
TrackShortcut* track_get_shortcut(s32 index);
s32 track_check_shortcut_entry(f32 *pos);

/* Environment */
TrackEnvironment* track_get_environment(void);
void track_set_weather(s32 weather);
void track_set_time_of_day(s32 time);
void track_apply_environment(void);

/* Timing */
TrackTiming* track_get_timing(void);
f32 track_get_start_time(s32 difficulty);
f32 track_get_target_time(s32 difficulty);
f32 track_get_par_time(void);

/* Collision helpers */
s32 track_get_surface_at(f32 *pos);
f32 track_get_height_at(f32 x, f32 z);
s32 track_is_on_track(f32 *pos);
s32 track_is_in_bounds(f32 *pos);

/* Mirror/reverse */
void track_set_mirror(s32 enabled);
void track_set_reverse(s32 enabled);
s32 track_is_mirrored(void);
s32 track_is_reversed(void);

/* Utility */
f32 track_get_distance_along(f32 *pos);
f32 track_get_progress_percent(f32 *pos);
void track_get_nearest_point(f32 *pos, f32 *nearest, f32 *normal);
s32 track_get_segment_at(f32 *pos);

#endif /* TRACK_H */
