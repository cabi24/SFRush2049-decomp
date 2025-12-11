/**
 * replay.h - Replay recording and playback for Rush 2049 N64
 *
 * Records car state each frame for ghost playback and instant replay.
 */

#ifndef REPLAY_H
#define REPLAY_H

#include "types.h"

/* Replay limits */
#define MAX_REPLAY_FRAMES       (60 * 60 * 5)   /* 5 minutes at 60fps */
#define MAX_REPLAY_CARS         8
#define REPLAY_SAMPLE_RATE      2               /* Record every N frames */
#define GHOST_MAX_LAPS          10

/* Replay modes */
#define REPLAY_MODE_NONE        0
#define REPLAY_MODE_RECORDING   1
#define REPLAY_MODE_PLAYBACK    2
#define REPLAY_MODE_PAUSED      3

/* Replay types */
#define REPLAY_TYPE_FULL        0   /* Full replay with all cars */
#define REPLAY_TYPE_GHOST       1   /* Single car ghost for time trial */
#define REPLAY_TYPE_HIGHLIGHT   2   /* Key moments only */

/* Playback controls */
#define PLAYBACK_PLAY           0
#define PLAYBACK_PAUSE          1
#define PLAYBACK_REWIND         2
#define PLAYBACK_FAST_FORWARD   3
#define PLAYBACK_SLOW_MO        4

/* Playback speeds (fixed point 8.8) */
#define SPEED_NORMAL            256     /* 1.0x */
#define SPEED_SLOW              64      /* 0.25x */
#define SPEED_FAST              512     /* 2.0x */
#define SPEED_SUPER_FAST        1024    /* 4.0x */

/* Replay flags */
#define REPLAY_FLAG_VALID       0x01
#define REPLAY_FLAG_COMPLETE    0x02
#define REPLAY_FLAG_COMPRESSED  0x04
#define REPLAY_FLAG_BEST_LAP    0x08

/* Per-frame car state snapshot */
typedef struct ReplayFrame {
    /* Position (16-bit fixed point for compression) */
    s16     pos_x;
    s16     pos_y;
    s16     pos_z;

    /* Orientation (compressed quaternion) */
    s16     quat_x;
    s16     quat_y;
    s16     quat_z;
    s16     quat_w;

    /* Velocity (8-bit scaled) */
    s8      vel_x;
    s8      vel_y;
    s8      vel_z;

    /* Wheel rotation angles (8-bit) */
    u8      wheel_rot[4];

    /* Steering angle (8-bit) */
    s8      steer_angle;

    /* State flags */
    u8      flags;

    /* Pad for alignment */
    u8      pad[2];

} ReplayFrame;

/* Extended frame data (recorded less frequently) */
typedef struct ReplayFrameExt {
    /* Full precision position */
    f32     pos[3];
    f32     vel[3];

    /* Full orientation matrix */
    f32     matrix[3][3];

    /* Engine/physics state */
    f32     engine_rpm;
    f32     speed;
    s8      gear;
    u8      on_ground;
    u8      surface_type;
    u8      pad;

} ReplayFrameExt;

/* Single car replay data */
typedef struct CarReplay {
    ReplayFrame     *frames;        /* Frame data array */
    ReplayFrameExt  *ext_frames;    /* Extended frames (every Nth) */
    u32             num_frames;     /* Number of frames recorded */
    u32             max_frames;     /* Maximum capacity */
    u8              car_index;      /* Original car index */
    u8              car_type;       /* Car type/model */
    u8              car_color;      /* Car color */
    u8              valid;          /* Has valid data */
} CarReplay;

/* Lap ghost data */
typedef struct GhostLap {
    CarReplay       data;           /* Replay data for this lap */
    u32             lap_time;       /* Lap time in frames */
    u8              lap_number;     /* Which lap (1-based) */
    u8              is_best;        /* Is this the best lap */
    u8              track_id;       /* Track identifier */
    u8              pad;
} GhostLap;

/* Full replay state */
typedef struct ReplayState {
    /* Recording */
    CarReplay       cars[MAX_REPLAY_CARS];
    s32             num_cars;
    u32             current_frame;
    u32             total_frames;

    /* Playback */
    u32             playback_frame;
    s16             playback_speed;     /* Fixed point 8.8 */
    u8              playback_state;     /* PLAYBACK_* */
    u8              loop_enabled;

    /* Mode */
    u8              mode;               /* REPLAY_MODE_* */
    u8              type;               /* REPLAY_TYPE_* */
    u8              flags;
    u8              pad;

    /* Camera */
    s32             camera_car;         /* Car to follow (-1 for free) */
    s32             camera_mode;        /* Camera angle mode */

    /* Metadata */
    u32             track_id;
    u32             start_time;
    u32             end_time;

} ReplayState;

/* Ghost system state */
typedef struct GhostState {
    GhostLap        best_lap;           /* Best lap ghost */
    GhostLap        current_lap;        /* Current lap being recorded */
    GhostLap        comparison_lap;     /* Lap to compare against */

    u8              ghost_visible;      /* Show ghost car */
    u8              recording;          /* Currently recording */
    u8              comparing;          /* Showing comparison ghost */
    u8              pad;

    f32             time_delta;         /* Time difference from ghost */

} GhostState;

/* Global state */
extern ReplayState gReplay;
extern GhostState gGhost;

/* Initialization */
void replay_init(void);
void replay_reset(void);
void replay_allocate(s32 num_cars, u32 max_frames);
void replay_free(void);

/* Recording */
void replay_start_recording(void);
void replay_stop_recording(void);
void replay_record_frame(void);
void replay_record_car_state(s32 car_index);

/* Playback */
void replay_start_playback(void);
void replay_stop_playback(void);
void replay_update_playback(void);
void replay_set_frame(u32 frame);
void replay_set_speed(s16 speed);
void replay_toggle_pause(void);

/* Playback controls */
void replay_play(void);
void replay_pause(void);
void replay_rewind(void);
void replay_fast_forward(void);
void replay_slow_motion(void);
void replay_skip_forward(u32 frames);
void replay_skip_backward(u32 frames);

/* State retrieval for rendering */
void replay_get_car_state(s32 car_index, u32 frame, f32 *pos, f32 *matrix);
void replay_interpolate_state(s32 car_index, f32 frame_f, f32 *pos, f32 *matrix);

/* Ghost system */
void ghost_init(void);
void ghost_reset(void);
void ghost_start_lap(void);
void ghost_end_lap(u32 lap_time);
void ghost_record_frame(void);
void ghost_update(void);
void ghost_draw(void);

/* Ghost comparison */
void ghost_load_best(u32 track_id);
void ghost_save_best(u32 track_id);
void ghost_set_comparison(GhostLap *lap);
f32 ghost_get_time_delta(void);

/* Queries */
s32 replay_is_recording(void);
s32 replay_is_playing(void);
u32 replay_get_length(void);
u32 replay_get_current_frame(void);
f32 replay_get_progress(void);

/* Camera */
void replay_set_camera_car(s32 car_index);
void replay_set_camera_mode(s32 mode);
void replay_cycle_camera(void);

/* Save/Load */
s32 replay_save(const char *filename);
s32 replay_load(const char *filename);
s32 ghost_save_to_pak(u32 track_id);
s32 ghost_load_from_pak(u32 track_id);

#endif /* REPLAY_H */
