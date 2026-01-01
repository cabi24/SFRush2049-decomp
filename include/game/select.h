/**
 * select.h - Track and car selection for Rush 2049 N64
 *
 * Handles track selection, car selection, and transmission choice.
 * Based on arcade sselect.c patterns from rushtherock/game/sselect.c
 *
 * Key arcade functions matched:
 *   InitSelect, InitTrkSel, TrackSel, ShowTrackSelect
 *   init_car_select, CarSel, ShowCarSelect, ClutchSel
 *   create_cars, SelectCam, track_negotiation, race_negotiation
 */

#ifndef SELECT_H
#define SELECT_H

#include "types.h"

/* ========================== ARCADE-COMPATIBLE DEFINES ========================== */

/* Number of tracks, cars, and pads (from arcade sselect.c) */
#define NTRACKS                 8       /* Total tracks (arcade: 8) */
#define NCARS                   12      /* Total car types (arcade: 12 = 3 banks of 4) */
#define NPADS                   4       /* Number of pads to hold cars in select */
#define MAX_LINKS               8       /* Maximum linked players (arcade networking) */

/* N64 aliases */
#define NUM_TRACKS              NTRACKS
#define NUM_CARS                8       /* N64 has fewer cars visible */
#define NUM_CAR_COLORS          8       /* Colors per car type */
#define MAX_SELECT_PADS         NPADS

/* Selection timing - arcade uses ONE_SEC (1000ms), N64 uses frames at 60fps */
#define ONE_SEC                 60      /* One second in frames (N64) */
#define TRK_SELECT_TIME         (20 * ONE_SEC)  /* 20 seconds track select */
#define CAR_SELECT_TIME         (30 * ONE_SEC)  /* 30 seconds car select */
#define QCK_SELECT_TIME         (10 * ONE_SEC)  /* Quick tourney time */
#define CLUTCH_SEL_TIME         (11 * ONE_SEC)  /* Transmission select time */
#define TRK_DELAY_TIME          ONE_SEC         /* Delay before track lock */
#define CAR_DELAY_TIME          (ONE_SEC / 3)   /* Delay before car lock */
#define HURRY_SPEED             ONE_SEC         /* How fast hurry-up countdown works */
#define HURRY_BLINK_TIME        30              /* Blink rate when hurry up */
#define BLINK_TIME              0x100           /* How fast to blink (arcade) */

/* Car display layout (from arcade) */
#define CARROW1                 12      /* Y offset of car display row 1 */
#define CARROW2                 36      /* Y offset row 2 */
#define CARROW_Y                24      /* Y distance between rows */
#define CARTRK_Y                67      /* Y distance between car displays */
#define LINKCOL1                12      /* X position for track 0-3 icons */
#define LINKCOL2                448     /* X position for track 4-7 icons */

/* Wheel input ranges (from arcade - steering wheel based) */
#define WHEEL_RANGE             1024    /* Full wheel range */
#define WHEEL_THIRD             (WHEEL_RANGE / 3)
#define WHEEL_SLICE             (WHEEL_RANGE / NPADS)
#define GAS_TRIGGER             (2 * 0x1000 / 3)    /* Gas pedal trigger position */
#define CLUTCH_TRIGGER          (2 * 0x1000 / 3)    /* Clutch pedal trigger */

/* Force feedback (arcade only - stub on N64) */
#define DEFAULT_FEEDBACK_LEVEL  1

/* Camera positions */
#define MAX_SELCAMPOS           3

/* Animation constants (from arcade) */
#define ROTOR_OFFSET            (100.0f * 3.14159f / 180.0f)
#define MAX_ACC                 0.0025f
#define MAX_VEL                 0.05f

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

/* Link flags (from arcade - for multiplayer state) */
#define LINK_ACTIVE             0x0001  /* Player is active */
#define LINK_BLINK              0x0002  /* Icon should blink */
#define LINK_TLOCK              0x0004  /* Track selection locked */
#define LINK_SOLO               0x0008  /* Playing solo (no link) */
#define LINK_DRONES             0x0010  /* Drones enabled */
#define LINK_RACERX             0x0020  /* Racer X mode */
#define LINK_CATCHUP            0x0040  /* Catchup enabled */
#define DEFAULT_LINK_FLAGS      0

/* Car movement animation states (from arcade CM_* enum) */
typedef enum CarMoves {
    CM_NOT = 0,         /* Not moving */
    CM_SETUP,           /* Setup initial move off pad */
    CM_OFFPAD,          /* Moving off the pad */
    CM_TURN,            /* Doing a turn */
    CM_FISHTAIL,        /* Fishtail animation */
    CM_PEELOUT,         /* Peel out to garage door */
    CM_DONE             /* Animation complete */
} CarMoves;

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

/* ========================== ARCADE-COMPATIBLE FUNCTIONS ========================== */
/* These match the arcade sselect.c function signatures for reference/matching */

/* Main selection functions (arcade: sselect.c) */
void InitSelect(void);              /* Called to load models and prepare for select */
void InitTrkSel(void);              /* Initialize global variables for track select */
void TrackSel(void);                /* Track selection state handler */
void ShowTrackSelect(s32 active);   /* Show/hide track select screen */
void TrackSelForce(void);           /* Handle force feedback during track select */

/* Car selection (arcade: sselect.c) */
void init_car_select(void);         /* Initialize car selection state */
void CarSel(void);                  /* Car selection state handler */
void ShowCarSelect(s32 active);     /* Show/hide car select screen */
void CarSelForce(void);             /* Handle force feedback during car select */
void ClutchSel(void);               /* Transmission selection */

/* Car creation and management */
void create_cars(void);             /* Create all cars for car select */
void SelectCam(void);               /* Update selection screen camera */

/* Negotiation (arcade networking - stub on N64) */
void track_negotiation(void);       /* Negotiate track selection with linked players */
void race_negotiation(void);        /* Negotiate race start with linked players */
void waiting_for_others(s16 type);  /* Play "waiting for others" sound */

/* Countdown management */
void SetCountdownTimer(u32 time);   /* Set the countdown timer */
u32 GetCountdownTime(void);         /* Get remaining countdown time */
s32 TimeOut(void);                  /* Check if timer has expired */
void ShowCountdown(s32 active);     /* Show/hide countdown display */
void start_countdown(void);         /* Start the race countdown */

/* Input helpers (arcade uses steering wheel/pedals, N64 uses controller) */
s32 WheelSection(s32 sections, s32 snap);     /* Get wheel position section */
s32 RawWheelSection(s32 sections, s32 snap);  /* Get raw wheel section */
s32 GasPressed(s32 check);                    /* Check if gas/confirm pressed */

/* Arcade globals (extern declarations for matching) */
extern u32 start_time;              /* IRQTIME when game started */
extern u32 play_time;               /* Milliseconds of time to play */
extern u8 gFeedbackLevel;           /* Force feedback level (0-2) */
extern s16 gThisCar;                /* Currently selected car index */
extern s32 gCarLocked;              /* Car selection locked flag */
extern s32 gClutchLocked;           /* Transmission locked flag */
extern s32 gTrackLocked;            /* Track selection locked flag */
extern s16 num_pads;                /* Number of selectable car pads */
extern s8 negotiate_track;          /* Track negotiation phase */
extern s8 negotiate_race;           /* Race negotiation phase */
extern s16 trackno;                 /* Current track number */
extern s32 gMirror;                 /* Mirror mode flag */
extern s32 solo_flag;               /* Solo play (no link) flag */
extern s32 autotrans;               /* Automatic transmission flag */
extern u8 drones_flag;              /* Drones enabled flag */
extern u8 catchup_flag;             /* Catchup enabled flag */

#endif /* SELECT_H */
