/**
 * ghost.h - Ghost car replay system for Rush 2049 N64
 *
 * Ghost car system for time trial mode. Records player position/orientation
 * for playback as a translucent "ghost" to race against.
 *
 * Based on arcade Rush The Rock dead reckoning concepts (reckon.c, modeldat.h)
 * but adapted for N64 ghost car functionality.
 *
 * Arcade Reference:
 *   - RECKON structure: Dead reckoning for network sync (modeldat.h)
 *   - MPATH structure: Path point data for drones (maxpath.h)
 *   - make_uvs_from_quat/make_quat_from_uvs: Quaternion-matrix conversion
 */

#ifndef GHOST_H
#define GHOST_H

#include "types.h"

/* Ghost system limits */
#define GHOST_MAX_FRAMES        (60 * 60 * 3)   /* 3 minutes at 60fps */
#define GHOST_SAMPLE_RATE       2               /* Record every N frames */
#define GHOST_MAX_SLOTS         4               /* Controller Pak slots */
#define GHOST_DATA_PER_FRAME    9               /* Floats per frame: pos(3), rot(3), vel(3) */
#define GHOST_FRAME_SIZE        (GHOST_DATA_PER_FRAME * sizeof(f32))

/* Ghost visibility/render modes */
#define GHOST_VISIBLE           1
#define GHOST_HIDDEN            0

/* Ghost transparency levels */
#define GHOST_ALPHA_NORMAL      128             /* 50% transparent */
#define GHOST_ALPHA_FADING      64              /* 25% when far ahead/behind */
#define GHOST_ALPHA_SOLID       200             /* Nearly opaque for close race */

/* Ghost render colors (RGBA) */
#define GHOST_COLOR_BLUE        0x4080FF80      /* Default blue tint */
#define GHOST_COLOR_RED         0xFF404080      /* Behind ghost */
#define GHOST_COLOR_GREEN       0x40FF4080      /* Ahead of ghost */
#define GHOST_COLOR_GOLD        0xFFD70080      /* Best time ghost */

/* Ghost interpolation modes (based on arcade dead_reckon_posuv) */
#define GHOST_INTERP_LINEAR     0               /* Simple linear */
#define GHOST_INTERP_SPLINE     1               /* Catmull-Rom spline */
#define GHOST_INTERP_QUAT       2               /* Quaternion slerp */

/* Ghost state flags */
#define GHOST_STATE_IDLE        0
#define GHOST_STATE_RECORDING   1
#define GHOST_STATE_PLAYING     2
#define GHOST_STATE_PAUSED      3

/* Ghost file magic and version */
#define GHOST_FILE_MAGIC        0x47485354      /* 'GHST' */
#define GHOST_FILE_VERSION      1

/**
 * Dead reckoning base data (adapted from arcade RECKON structure)
 * Used for ghost interpolation between recorded frames.
 */
typedef struct GhostReckon {
    /* Base state for interpolation */
    f32         base_pos[3];        /* Base position (ft) */
    f32         base_vel[3];        /* Base velocity (ft/sec) */
    f32         base_quat[4];       /* Base quaternion orientation */
    f32         delta_quat[4];      /* Quaternion change per step */
    s16         num_quat_steps;     /* Steps remaining to target */
    s16         pad;

    /* Interpolated output */
    f32         pos[3];             /* Current interpolated position */
    f32         rot[3];             /* Current rotation (pitch, yaw, roll) */
    f32         vel[3];             /* Current velocity */
    f32         matrix[3][3];       /* Rotation matrix for rendering */
} GhostReckon;

/**
 * Ghost frame data - recorded per-frame state
 * Stored in floating point for accuracy (compressed for Controller Pak save)
 */
typedef struct GhostFrame {
    f32         pos[3];             /* World position */
    f32         rot[3];             /* Rotation (pitch, yaw, roll in radians) */
    f32         vel[3];             /* Velocity vector */
} GhostFrame;

/**
 * Compressed ghost frame for Controller Pak storage
 * Based on arcade dead reckoning compression
 */
typedef struct GhostFrameCompressed {
    s16         pos_x;              /* Position / 16.0 */
    s16         pos_y;
    s16         pos_z;
    s16         rot_x;              /* Rotation * 10430 (2^15 / pi) */
    s16         rot_y;
    s16         rot_z;
    s8          vel_x;              /* Velocity / 4.0 */
    s8          vel_y;
    s8          vel_z;
    u8          flags;              /* State flags (on_ground, boosting, etc) */
} GhostFrameCompressed;

/**
 * Ghost car state - main ghost data structure
 */
typedef struct GhostCar {
    /* Recording state */
    s32         state;              /* GHOST_STATE_* */
    s32         frame_count;        /* Current frame index */
    s32         max_frames;         /* Total recorded frames */
    s32         track_id;           /* Track identifier */
    s32         lap_time;           /* Lap time in centiseconds */

    /* Car info */
    s32         car_model;          /* Car model index */
    s32         car_color;          /* Car color index */
    u8          valid;              /* Has valid recorded data */
    u8          is_best;            /* Is this the best lap ghost */
    u8          pad[2];

    /* Position/orientation for rendering */
    f32         pos[3];             /* Current interpolated position */
    f32         rot[3];             /* Current rotation */
    f32         vel[3];             /* Current velocity */

    /* Dead reckoning state for interpolation */
    GhostReckon reckon;

    /* Frame data buffer (allocated separately) */
    f32         *frame_data;        /* [frame * GHOST_DATA_PER_FRAME] */
} GhostCar;

/**
 * Ghost system global state
 */
typedef struct GhostSystem {
    /* Current recording ghost */
    GhostCar    recording;

    /* Best lap ghost for comparison */
    GhostCar    best_lap;

    /* Loaded ghost for racing */
    GhostCar    loaded;

    /* System state */
    u8          ghost_visible;      /* Show ghost during race */
    u8          recording_active;   /* Recording in progress */
    u8          playback_active;    /* Playing ghost */
    u8          comparison_mode;    /* Show time delta */

    /* Time comparison */
    f32         time_delta;         /* Time difference from ghost (seconds) */
    f32         distance_delta;     /* Position difference (feet) */

    /* Render settings */
    s32         render_color;       /* Current ghost color */
    s32         alpha;              /* Current transparency */
    s32         interp_mode;        /* Interpolation mode */

    /* Static frame buffers */
    f32         record_buffer[GHOST_MAX_FRAMES / GHOST_SAMPLE_RATE * GHOST_DATA_PER_FRAME];
    f32         best_buffer[GHOST_MAX_FRAMES / GHOST_SAMPLE_RATE * GHOST_DATA_PER_FRAME];
    f32         loaded_buffer[GHOST_MAX_FRAMES / GHOST_SAMPLE_RATE * GHOST_DATA_PER_FRAME];
} GhostSystem;

/**
 * Ghost file header for Controller Pak saves
 */
typedef struct GhostFileHeader {
    u32         magic;              /* GHOST_FILE_MAGIC */
    u16         version;            /* GHOST_FILE_VERSION */
    u16         frame_count;        /* Number of frames */
    u8          track_id;           /* Track identifier */
    u8          car_model;          /* Car model */
    u8          car_color;          /* Car color */
    u8          flags;              /* File flags */
    u32         lap_time;           /* Lap time in centiseconds */
    u32         checksum;           /* Data checksum */
} GhostFileHeader;

/* Global ghost system */
extern GhostSystem gGhostSystem;

/*
 * Initialization
 */
void ghost_system_init(void);
void ghost_system_reset(void);

/*
 * Recording functions
 */
void ghost_start_recording(s32 track_id, s32 car_model, s32 car_color);
void ghost_stop_recording(s32 lap_time);
void ghost_cancel_recording(void);
void ghost_record_car_frame(f32 *pos, f32 *rot, f32 *vel);

/*
 * Playback functions
 */
void ghost_start_playback(void);
void ghost_stop_playback(void);
void ghost_update_playback(void);
void ghost_set_playback_frame(s32 frame);

/*
 * Interpolation (based on arcade dead_reckon_posuv)
 */
void ghost_interpolate_frame(GhostCar *ghost, f32 frame_f);
void ghost_dead_reckon(GhostReckon *reckon, f32 dt);
void ghost_make_matrix_from_quat(f32 *quat, f32 matrix[3][3]);
void ghost_make_quat_from_euler(f32 *rot, f32 *quat);

/*
 * Rendering
 */
void ghost_render(void);
void ghost_render_car(GhostCar *ghost, s32 color, s32 alpha);
void ghost_set_visibility(s32 visible);
void ghost_set_color(s32 color);
void ghost_set_alpha(s32 alpha);

/*
 * Time comparison
 */
f32 ghost_get_time_delta(void);
f32 ghost_get_distance_delta(void);
void ghost_update_comparison(f32 *player_pos);

/*
 * Best lap management
 */
s32 ghost_is_best_lap(s32 lap_time);
void ghost_promote_to_best(void);
void ghost_copy_recording_to_best(void);

/*
 * Controller Pak I/O
 */
s32 ghost_save_to_pak(s32 slot);
s32 ghost_load_from_pak(s32 slot);
s32 ghost_delete_from_pak(s32 slot);
s32 ghost_get_pak_slot_info(s32 slot, s32 *track_id, s32 *lap_time);
s32 ghost_count_saved_ghosts(void);

/*
 * Data compression (for Controller Pak storage)
 */
void ghost_compress_frame(GhostFrame *in, GhostFrameCompressed *out);
void ghost_decompress_frame(GhostFrameCompressed *in, GhostFrame *out);
u32 ghost_calc_checksum(void *data, s32 size);

/*
 * Queries
 */
s32 ghost_is_recording(void);
s32 ghost_is_playing(void);
s32 ghost_is_visible(void);
s32 ghost_has_valid_data(void);
s32 ghost_get_frame_count(void);
s32 ghost_get_lap_time(void);

/*
 * Position scale factors (from arcade dead reckoning)
 */
#define GHOST_POS_SCALE         16.0f           /* Position compression scale */
#define GHOST_VEL_SCALE         4.0f            /* Velocity compression scale */
#define GHOST_ROT_SCALE         10430.0f        /* Rotation: 2^15 / pi */

#endif /* GHOST_H */
