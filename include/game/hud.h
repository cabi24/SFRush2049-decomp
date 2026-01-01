/**
 * hud.h - Heads-Up Display for Rush 2049 N64
 *
 * Based on arcade game/hud.c and game/hud.h
 * Handles in-race display elements:
 * - Speedometer/tachometer
 * - Lap counter
 * - Race position
 * - Timer (elapsed, countdown, checkpoint)
 * - Mini-map/radar with car markers
 * - Wrong way indicator
 *
 * Copyright 1996-1999 Time Warner Interactive / Atari Games
 */

#ifndef HUD_H
#define HUD_H

#include "types.h"

/*===============================  DEFINES  =================================*/

/* Screen coordinates (arcade-compatible) */
#define SCR_W               320         /* Screen width */
#define SCR_H               240         /* Screen height */
#define SCR_L               5           /* Safe left edge */
#define SCR_R               (SCR_W - 10)    /* Safe right edge */
#define SCR_T               5           /* Safe top edge */
#define SCR_B               (SCR_H - 5)     /* Safe bottom edge */
#define SCR_MH              ((SCR_R - SCR_L) / 2)   /* Screen middle horizontal */
#define SCR_MV              ((SCR_B - SCR_T) / 2)   /* Screen middle vertical */

/* Timing constants */
#define ONE_SEC             1000        /* Milliseconds per second */
#define CHECKPT_TIME        (3 * ONE_SEC)   /* Checkpoint display duration */

/* Math constants (arcade-compatible) */
#define HUD_PI              3.14159265358979f
#define DEG_TO_RADS         (HUD_PI / 180.0f)
#define RAD_TO_DEGS         (180.0f / HUD_PI)

/* Radar map constants (arcade-compatible) */
#define TCAR_H              8.0f        /* Height of radar car blit */
#define TCAR_W              7.0f        /* Width of radar car blit */
#define RADAR_X             SCR_L       /* Radar position X */
#define RADAR_Y             (SCR_T + 60)    /* Radar position Y */
#define RADAR_WIDE          64.0f       /* Radar frame width */
#define RADAR_HIGH          256.0f      /* Radar frame height */
#define RADAR_BWIDE         3.0f        /* Radar border width */
#define RADAR_BHIGH         3.0f        /* Radar border height */
#define RADAR_MIN_X         (RADAR_X + RADAR_BWIDE)
#define RADAR_MAX_X         (RADAR_X + RADAR_WIDE - RADAR_BWIDE - TCAR_W)
#define RADAR_MIN_Y         (RADAR_Y + RADAR_BHIGH)
#define RADAR_MAX_Y         (RADAR_Y + RADAR_HIGH - RADAR_BHIGH - TCAR_H)
#define RADAR_CENTER_X      (RADAR_WIDE / 2.0f)
#define RADAR_CENTER_Y      42.0f

/* Wrong way detection threshold (radians) */
#define STRAY_THRESHOLD     (3.0f * HUD_PI / 8.0f)

/* HUD element flags */
#define HUD_SHOW_SPEED      0x0001
#define HUD_SHOW_LAPS       0x0002
#define HUD_SHOW_POSITION   0x0004
#define HUD_SHOW_TIMER      0x0008
#define HUD_SHOW_RADAR      0x0010
#define HUD_SHOW_WRONG_WAY  0x0020
#define HUD_SHOW_CHECKTIME  0x0040
#define HUD_SHOW_MARKERS    0x0080
#define HUD_SHOW_TACH       0x0100
#define HUD_SHOW_SHIFTER    0x0200
#define HUD_SHOW_TRACKMAP   0x0400
#define HUD_SHOW_ALL        0x07FF

/* HUD position constants */
#define HUD_SPEED_X         (SCR_MH - 33)
#define HUD_SPEED_Y         SCR_T
#define HUD_LAPS_X          30
#define HUD_LAPS_Y          30
#define HUD_POS_X           (SCR_R - 55)
#define HUD_POS_Y           (SCR_T + 40)
#define HUD_TIMER_X         (SCR_L + 2)
#define HUD_TIMER_Y         SCR_T
#define HUD_RADAR_X         30
#define HUD_RADAR_Y         180
#define HUD_RADAR_SIZE      50

/* Maximum values */
#define MAX_LINKS           8           /* Maximum players/cars */

/*===============================  TYPEDEFS  =================================*/

/**
 * DigitType - Font types for numeric displays
 * Based on arcade: hud.h:DigitType enum
 */
typedef enum DigitType {
    SpeedDigit = 0,     /* Large speed digits */
    YelDigit,           /* Yellow digits */
    SmallYelDigit,      /* Small yellow digits */
    RedDigit,           /* Red digits */
    SmallRedDigit,      /* Small red digits */
    LapDigit,           /* Lap counter digits */
    HudTimerDigit,      /* HUD timer digits */
    CountdownDigit,     /* Large countdown digits */
    PlaceDigit,         /* Race place digits */
    TimerDigit,         /* Timer digits */
    NUM_DIGIT_TYPES
} DigitType;

/**
 * MBlits - Multi-blit types for HUD overlays
 * Based on arcade: hud.h:mBlits enum
 */
typedef enum MBlits {
    MBLIT_COUNT,        /* Countdown display */
    MBLIT_WRONGWAY,     /* Wrong way indicator */
    MBLIT_PLACE,        /* Race position display */
    MBLIT_JOIN,         /* Join now message */
    MBLIT_WAIT,         /* Wait message */
    MBLIT_PREP,         /* Preparing message */
    MBLIT_CD            /* CD player/radio display */
} MBlits;

/**
 * HudState - Main HUD state structure
 */
typedef struct HudState {
    u16     flags;              /* Which elements to show */
    s16     speed_display;      /* Current speed shown (mph/kph) */
    s16     target_speed;       /* Speed we're animating toward */
    s16     rpm_display;        /* Current RPM shown */
    s8      current_lap;        /* Current lap number */
    s8      total_laps;         /* Total laps in race */
    s8      position;           /* Race position (1-8) */
    s8      total_cars;         /* Total cars in race */
    u32     elapsed_time;       /* Elapsed race time (ms) */
    u32     countdown_time;     /* Remaining countdown time (ms) */
    u32     checkpoint_time;    /* Time added at checkpoint */
    u32     checkpoint_timeout; /* When checkpoint display goes away */
    u32     checkpoint_timeout2;/* Bonus time display timeout */
    s32     checkpoint_flash;   /* Flash timer for checkpoint bonus */
    u32     radio_timeout;      /* Radio display timeout */
    s8      wrong_way;          /* Wrong way indicator active */
    s8      markers_init;       /* Markers initialized */
    s8      metric_mode;        /* Show KPH instead of MPH */
    s8      auto_trans;         /* Using automatic transmission */
    s8      man_gear;           /* Manual gear selection */
    s8      mBlit_index;        /* Current multi-blit display */
    f32     stray_angle;        /* Angle of stray from course (radians) */
    s16     finish_x;           /* Finish flag X position */
    s16     finish_y;           /* Finish flag Y position */
    s8      finish_hidden;      /* Finish flag hidden */
    s8      laps_left;          /* Laps remaining */
    u8      pad[2];
} HudState;

/**
 * CarMarker - Car marker on radar/track map
 */
typedef struct CarMarker {
    f32     x;                  /* Screen X position */
    f32     y;                  /* Screen Y position */
    f32     z;                  /* Depth for sorting */
    u8      color;              /* Marker color index */
    s8      visible;            /* Is marker visible */
    s8      is_player;          /* Is this the player */
    s8      is_flashing;        /* Flash when resurrecting */
} CarMarker;

/**
 * RankList - Ranking order for marker display
 */
typedef struct RankList {
    s8      slot[MAX_LINKS];    /* Slot indices sorted by rank */
    s8      dot_order[MAX_LINKS];   /* Drawing order for dots */
} RankList;

/* External HUD state */
extern HudState gHud;
extern CarMarker hud_markers[];
extern s32 gHudMovie;
extern s8 wrong_way;

/*===============================  FUNCTIONS  ================================*/

/* Initialization (arcade-compatible) */
void InitHUD(void);
void ShowHUD(void);
void RemoveHUD(void);
void InitDigitList(void);

/* Legacy N64-style functions */
void hud_init(void);
void hud_reset(void);

/* Per-frame update */
void hud_update(void);
void hud_draw(void);

/* Individual element drawing */
void hud_draw_speed(void);
void hud_draw_laps(void);
void hud_draw_position(void);
void hud_draw_timer(void);
void hud_draw_radar(void);
void hud_draw_wrong_way(void);
void hud_draw_checkpoint_bonus(void);

/* Radar/mini-map */
void hud_update_markers(void);
void hud_draw_markers(void);
void hud_calc_marker_pos(s32 car_index, f32 *out_x, f32 *out_y);

/* Control */
void hud_show(u16 flags);
void hud_hide(u16 flags);
void hud_toggle(u16 flags);

/* Events (arcade-compatible) */
void hud_checkpoint_hit(u32 time_bonus);
void hud_lap_complete(s32 lap_num);
void hud_set_wrong_way(s32 wrong);
void ShowCheckpointTime(s32 delta);
void ShowFinishFlag(f32 pos[3], s32 hide);
void EnableRadioHUD(void);

/* Utility */
void hud_format_time(u32 frames, char *buffer);
void hud_format_speed(s32 speed, char *buffer);
void hud_format_position(s32 position, char *buffer);
void hud_format_lap(s32 current, s32 total, char *buffer);

/* Time conversion (arcade-compatible) */
u8 cvt_time(s32 t, u8 *dest, char format);
u8 cvt_time_frames(u32 frames, u8 *dest, char format);

/* Speed queries */
s32 hud_get_speed_mph(void);
s32 hud_get_speed_kph(void);

/* Timer queries */
u32 hud_get_elapsed_frames(void);
u32 hud_get_elapsed_ms(void);
void hud_set_countdown_time(u32 ms);
u32 hud_get_countdown_time(void);

/* Units mode */
void hud_set_metric(s32 metric);
s32 hud_is_metric(void);

/* Radar map */
void set_radar_mapping(f32 min_x, f32 max_x, f32 min_z, f32 max_z);
void world_to_radar(f32 world_x, f32 world_z, f32 *radar_x, f32 *radar_y);

/* Blit utilities (arcade-compatible) */
s32 Hidden(void *blt, s32 hide);
s32 UpdateNumber(void *blt, u8 digit);
s32 updateList(void *blt, s32 val, s32 num, ...);
s32 updateList2(void *blt, s32 val, s32 num, ...);
void CropBlit(void *blt, s32 l, s32 t, s32 r, s32 b);

#endif /* HUD_H */
