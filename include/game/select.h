/**
 * select.h - Track and car selection for Rush 2049 N64
 *
 * Handles track selection, car selection, and transmission choice.
 * Based on arcade sselect.c patterns.
 */

#ifndef SELECT_H
#define SELECT_H

#include "types.h"

/* Number of tracks and cars */
#define NUM_TRACKS              8       /* Total tracks available */
#define NUM_CARS                8       /* Total car types */
#define NUM_CAR_COLORS          8       /* Colors per car type */
#define MAX_SELECT_PADS         4       /* Pads showing cars in selection */

/* Selection timing (frames at 60fps) */
#define TRK_SELECT_TIME         (20 * 60)   /* 20 seconds track select */
#define CAR_SELECT_TIME         (30 * 60)   /* 30 seconds car select */
#define CAR_DELAY_TIME          (60 / 3)    /* Delay before car lock */
#define TRK_DELAY_TIME          60          /* Delay before track lock */
#define HURRY_BLINK_TIME        30          /* Blink rate when hurry up */

/* Selection states */
#define SELECT_STATE_IDLE       0
#define SELECT_STATE_INIT       1
#define SELECT_STATE_ACTIVE     2
#define SELECT_STATE_LOCKED     3
#define SELECT_STATE_DONE       4
#define SELECT_STATE_TIMEOUT    5

/* Track selection sub-states */
#define TRKSEL_BROWSING         0       /* Player browsing tracks */
#define TRKSEL_CONFIRM          1       /* Confirming selection */
#define TRKSEL_LOCKED           2       /* Selection locked in */
#define TRKSEL_WAITING          3       /* Waiting for others (N/A on N64) */

/* Car selection sub-states */
#define CARSEL_BROWSING         0       /* Player browsing cars */
#define CARSEL_CONFIRM          1       /* Confirming selection */
#define CARSEL_COLOR            2       /* Selecting color */
#define CARSEL_LOCKED           3       /* Selection locked in */

/* Track flags */
#define TRACK_FLAG_UNLOCKED     0x01    /* Track available */
#define TRACK_FLAG_MIRROR       0x02    /* Mirror mode variant */
#define TRACK_FLAG_HIDDEN       0x04    /* Hidden track */
#define TRACK_FLAG_BONUS        0x08    /* Bonus track */

/* Car flags */
#define CAR_FLAG_UNLOCKED       0x01    /* Car available */
#define CAR_FLAG_HIDDEN         0x02    /* Hidden car */
#define CAR_FLAG_SPECIAL        0x04    /* Special car (stunt mode) */

/* Transmission types */
#define TRANS_AUTO              0       /* Automatic transmission */
#define TRANS_MANUAL            1       /* Manual transmission */

/* Track info */
typedef struct TrackInfo {
    const char  *name;              /* Track name */
    const char  *location;          /* Track location */
    u8          track_id;           /* Internal ID */
    u8          difficulty;         /* 1-5 difficulty */
    u8          laps;               /* Default lap count */
    u8          flags;              /* TRACK_FLAG_* */
    u16         best_time;          /* Best lap time (seconds * 100) */
    u8          checkpoint_count;   /* Number of checkpoints */
    u8          pad;
} TrackInfo;

/* Car info */
typedef struct CarInfo {
    const char  *name;              /* Car name */
    u8          car_id;             /* Internal ID */
    u8          flags;              /* CAR_FLAG_* */
    u8          speed_rating;       /* 1-10 speed */
    u8          handling_rating;    /* 1-10 handling */
    u8          accel_rating;       /* 1-10 acceleration */
    u8          weight_rating;      /* 1-10 weight */
    u8          default_color;      /* Default color index */
    u8          pad;
} CarInfo;

/* Selection cursor state */
typedef struct SelectCursor {
    s32         position;           /* Current position */
    s32         target;             /* Target position (for animation) */
    f32         scroll_offset;      /* Smooth scroll offset */
    f32         scroll_speed;       /* Current scroll speed */
    u8          moving;             /* Is cursor moving */
    u8          direction;          /* Movement direction */
    u8          pad[2];
} SelectCursor;

/* Track selection state */
typedef struct TrackSelectState {
    u8          state;              /* SELECT_STATE_* */
    u8          sub_state;          /* TRKSEL_* sub-state */
    u8          current_track;      /* Currently highlighted track */
    u8          selected_track;     /* Locked track selection */

    u8          mirror_mode;        /* Mirror mode enabled */
    u8          pad[3];

    u32         timer;              /* Selection timer */
    u32         lock_delay;         /* Time before lock allowed */

    SelectCursor cursor;            /* Cursor animation */

    /* Preview */
    f32         preview_rotation;   /* Track preview rotation */
    u8          show_preview;       /* Show track preview */
    u8          pad2[3];
} TrackSelectState;

/* Car selection state */
typedef struct CarSelectState {
    u8          state;              /* SELECT_STATE_* */
    u8          sub_state;          /* CARSEL_* sub-state */
    u8          current_car;        /* Currently highlighted car */
    u8          selected_car;       /* Locked car selection */

    u8          current_color;      /* Current color selection */
    u8          selected_color;     /* Locked color */
    u8          show_stats;         /* Show car stats */
    u8          pad;

    u32         timer;              /* Selection timer */
    u32         lock_delay;         /* Time before lock allowed */

    SelectCursor cursor;            /* Cursor animation */

    /* Car display */
    f32         car_rotation;       /* Display car rotation */
    f32         rotation_speed;     /* Rotation speed */
    u8          auto_rotate;        /* Auto-rotate display car */
    u8          pad2[3];

    /* Transmission */
    u8          transmission;       /* TRANS_AUTO or TRANS_MANUAL */
    u8          trans_locked;       /* Transmission locked */
    u8          pad3[2];
} CarSelectState;

/* Overall selection state */
typedef struct SelectState {
    /* Current phase */
    u8          phase;              /* 0=track, 1=car, 2=trans */
    u8          complete;           /* All selections made */
    u8          cancelled;          /* Selection cancelled */
    u8          pad;

    /* Sub-states */
    TrackSelectState track;         /* Track selection */
    CarSelectState car;             /* Car selection */

    /* Final selections */
    u8          final_track;        /* Final track ID */
    u8          final_car;          /* Final car ID */
    u8          final_color;        /* Final car color */
    u8          final_trans;        /* Final transmission */
    u8          final_mirror;       /* Mirror mode */
    u8          final_laps;         /* Number of laps */
    u8          pad2[2];

    /* Player input */
    u8          player;             /* Player index (0-3) */
    u8          pad3[3];

} SelectState;

/* Global state */
extern SelectState gSelect;

/* Track and car data */
extern const TrackInfo gTrackData[];
extern const CarInfo gCarData[];

/* Initialization */
void select_init(void);
void select_reset(void);

/* Track selection */
void track_select_init(void);
void track_select_update(void);
void track_select_draw(void);
void track_select_input(void);
void track_select_confirm(void);
void track_select_cancel(void);
s32 track_select_is_locked(void);

/* Car selection */
void car_select_init(void);
void car_select_update(void);
void car_select_draw(void);
void car_select_input(void);
void car_select_confirm(void);
void car_select_cancel(void);
s32 car_select_is_locked(void);

/* Transmission selection */
void trans_select_init(void);
void trans_select_update(void);
void trans_select_draw(void);
void trans_select_toggle(void);

/* Selection flow */
void select_start(void);
void select_update(void);
void select_draw(void);
void select_next_phase(void);
void select_prev_phase(void);
s32 select_is_complete(void);

/* Queries */
u8 select_get_track(void);
u8 select_get_car(void);
u8 select_get_color(void);
u8 select_get_transmission(void);
u8 select_get_mirror(void);
u8 select_get_laps(void);

/* Track/car availability */
s32 track_is_available(u8 track_id);
s32 car_is_available(u8 car_id);
void track_unlock(u8 track_id);
void car_unlock(u8 car_id);

/* Preview rendering */
void track_preview_render(u8 track_id, f32 rotation);
void car_preview_render(u8 car_id, u8 color, f32 rotation);

/* Timer display */
void select_draw_timer(s32 x, s32 y, u32 frames);
void select_draw_countdown(void);

#endif /* SELECT_H */
