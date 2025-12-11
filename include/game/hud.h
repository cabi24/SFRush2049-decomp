/**
 * hud.h - Heads-Up Display for Rush 2049 N64
 *
 * Based on arcade game/hud.c
 * Handles in-race display elements:
 * - Speedometer
 * - Lap counter
 * - Race position
 * - Timer
 * - Mini-map/radar
 */

#ifndef HUD_H
#define HUD_H

#include "types.h"

/* HUD element flags */
#define HUD_SHOW_SPEED      0x0001
#define HUD_SHOW_LAPS       0x0002
#define HUD_SHOW_POSITION   0x0004
#define HUD_SHOW_TIMER      0x0008
#define HUD_SHOW_RADAR      0x0010
#define HUD_SHOW_WRONG_WAY  0x0020
#define HUD_SHOW_CHECKTIME  0x0040
#define HUD_SHOW_MARKERS    0x0080
#define HUD_SHOW_ALL        0x00FF

/* HUD position constants */
#define HUD_SPEED_X         280
#define HUD_SPEED_Y         200
#define HUD_LAPS_X          30
#define HUD_LAPS_Y          30
#define HUD_POS_X           160
#define HUD_POS_Y           30
#define HUD_TIMER_X         160
#define HUD_TIMER_Y         220
#define HUD_RADAR_X         30
#define HUD_RADAR_Y         180
#define HUD_RADAR_SIZE      50

/* HUD state */
typedef struct HudState {
    u16     flags;              /* Which elements to show */
    s16     speed_display;      /* Current speed shown (mph) */
    s16     target_speed;       /* Speed we're animating toward */
    s8      current_lap;        /* Current lap number */
    s8      total_laps;         /* Total laps in race */
    s8      position;           /* Race position (1-8) */
    s8      total_cars;         /* Total cars in race */
    u32     elapsed_time;       /* Elapsed race time (frames) */
    u32     checkpoint_time;    /* Time added at checkpoint */
    s32     checkpoint_flash;   /* Flash timer for checkpoint bonus */
    s8      wrong_way;          /* Wrong way indicator active? */
    s8      markers_init;       /* Markers initialized? */
    u8      pad[2];
} HudState;

/* Car marker on radar */
typedef struct CarMarker {
    f32     x;                  /* Screen X position */
    f32     y;                  /* Screen Y position */
    u8      color;              /* Marker color index */
    s8      visible;            /* Is marker visible? */
    s8      is_player;          /* Is this the player? */
    s8      is_flashing;        /* Flash when resurrecting? */
} CarMarker;

/* External HUD state */
extern HudState gHud;
extern CarMarker hud_markers[];

/* Initialization */
void hud_init(void);
void hud_reset(void);

/* Per-frame update */
void hud_update(void);
void hud_draw(void);

/* Individual elements */
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

/* Events */
void hud_checkpoint_hit(u32 time_bonus);
void hud_lap_complete(s32 lap_num);
void hud_set_wrong_way(s32 wrong);

/* Utility */
void hud_format_time(u32 frames, char *buffer);
void hud_format_speed(s32 speed, char *buffer);

#endif /* HUD_H */
