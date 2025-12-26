/**
 * minimap.h - Minimap and radar system for Rush 2049 N64
 *
 * Displays track overview, player positions, pickups, and
 * other important markers during gameplay.
 */

#ifndef MINIMAP_H
#define MINIMAP_H

#include "types.h"

/* Minimap modes */
#define MINIMAP_MODE_OFF        0       /* Hidden */
#define MINIMAP_MODE_CORNER     1       /* Corner radar */
#define MINIMAP_MODE_FULL       2       /* Full track view */
#define MINIMAP_MODE_ROTATING   3       /* Player-centered rotating */
#define NUM_MINIMAP_MODES       4

/* Minimap positions */
#define MINIMAP_POS_TOP_LEFT    0
#define MINIMAP_POS_TOP_RIGHT   1
#define MINIMAP_POS_BOT_LEFT    2
#define MINIMAP_POS_BOT_RIGHT   3
#define NUM_MINIMAP_POSITIONS   4

/* Marker types */
#define MARKER_PLAYER           0       /* Player car */
#define MARKER_OPPONENT         1       /* Opponent car */
#define MARKER_CHECKPOINT       2       /* Checkpoint */
#define MARKER_PICKUP_WEAPON    3       /* Weapon pickup */
#define MARKER_PICKUP_HEALTH    4       /* Health pickup */
#define MARKER_PICKUP_BOOST     5       /* Boost pickup */
#define MARKER_HAZARD           6       /* Hazard/obstacle */
#define MARKER_SHORTCUT         7       /* Shortcut entry */
#define MARKER_FINISH           8       /* Finish line */
#define MARKER_ARROW            9       /* Direction arrow */
#define NUM_MARKER_TYPES        10

/* Marker limits */
#define MAX_MARKERS             32      /* Maximum active markers */

/* Minimap sizes */
#define MINIMAP_SIZE_SMALL      48      /* Small radar */
#define MINIMAP_SIZE_MEDIUM     64      /* Medium radar */
#define MINIMAP_SIZE_LARGE      96      /* Large radar */
#define MINIMAP_SIZE_FULL       160     /* Full screen */

/* Track point for map outline */
typedef struct MapPoint {
    f32     x, z;               /* World coordinates */
    f32     u, v;               /* Map UV coordinates */
} MapPoint;

/* Track segment for map */
typedef struct MapSegment {
    MapPoint start;
    MapPoint end;
    u8      type;               /* Segment type (road, shortcut, etc.) */
    u8      pad[3];
} MapSegment;

/* Marker on minimap */
typedef struct MapMarker {
    f32     world_x, world_z;   /* World position */
    f32     rotation;           /* Marker rotation */
    u8      type;               /* MARKER_* type */
    u8      player;             /* Player index (if player marker) */
    u8      active;             /* Marker visible */
    u8      blinking;           /* Blinking animation */
    u8      color[4];           /* RGBA color */
    f32     scale;              /* Size multiplier */
} MapMarker;

/* Track map data */
typedef struct TrackMap {
    /* Bounds in world coordinates */
    f32     min_x, min_z;
    f32     max_x, max_z;

    /* Track outline points */
    MapPoint *outline;
    s32     outline_count;

    /* Track segments */
    MapSegment *segments;
    s32     segment_count;

    /* Checkpoint positions */
    MapPoint checkpoints[20];
    s32     checkpoint_count;

    /* Map texture if pre-rendered */
    u32     texture_id;
    u8      has_texture;
    u8      pad[3];

} TrackMap;

/* Minimap state */
typedef struct MinimapState {
    /* Display settings */
    u8      mode;               /* MINIMAP_MODE_* */
    u8      position;           /* MINIMAP_POS_* */
    u8      visible;            /* Currently visible */
    u8      size;               /* Pixel size */

    /* Screen position */
    s16     screen_x;
    s16     screen_y;
    s16     width;
    s16     height;

    /* View settings */
    f32     zoom;               /* Zoom level */
    f32     rotation;           /* Map rotation (for rotating mode) */
    f32     center_x, center_z; /* Center point (for rotating mode) */

    /* Appearance */
    u8      opacity;            /* Map opacity 0-255 */
    u8      show_track;         /* Show track outline */
    u8      show_checkpoints;   /* Show checkpoints */
    u8      show_pickups;       /* Show pickups */
    u8      show_opponents;     /* Show opponent markers */
    u8      show_shortcuts;     /* Show shortcut markers */
    u8      border;             /* Show border */
    u8      pad;

    /* Colors */
    u8      bg_color[4];        /* Background color */
    u8      track_color[4];     /* Track line color */
    u8      border_color[4];    /* Border color */

    /* Markers */
    MapMarker markers[MAX_MARKERS];
    s32     marker_count;

    /* Animation */
    f32     blink_timer;        /* Blink animation timer */
    u8      blink_state;        /* Current blink state */
    u8      pad2[3];

    /* Track map data */
    TrackMap *track_map;

} MinimapState;

/* Global minimap state */
extern MinimapState gMinimap;

/* Initialization */
void minimap_init(void);
void minimap_reset(void);

/* Track map loading */
s32 minimap_load_track(s32 track_id);
void minimap_unload_track(void);
void minimap_generate_from_checkpoints(void);

/* Display control */
void minimap_show(void);
void minimap_hide(void);
void minimap_toggle(void);
s32 minimap_is_visible(void);
void minimap_set_mode(s32 mode);
s32 minimap_get_mode(void);
void minimap_cycle_mode(void);

/* Position and size */
void minimap_set_position(s32 position);
void minimap_set_screen_pos(s16 x, s16 y);
void minimap_set_size(s32 size);
void minimap_set_zoom(f32 zoom);

/* View control */
void minimap_set_center(f32 x, f32 z);
void minimap_set_rotation(f32 rotation);
void minimap_follow_player(s32 player);

/* Appearance */
void minimap_set_opacity(u8 opacity);
void minimap_set_bg_color(u8 r, u8 g, u8 b, u8 a);
void minimap_set_track_color(u8 r, u8 g, u8 b, u8 a);
void minimap_show_element(s32 element, s32 visible);

/* Markers */
void minimap_clear_markers(void);
s32 minimap_add_marker(s32 type, f32 x, f32 z, f32 rotation);
void minimap_remove_marker(s32 index);
void minimap_update_marker(s32 index, f32 x, f32 z, f32 rotation);
void minimap_set_marker_color(s32 index, u8 r, u8 g, u8 b, u8 a);
void minimap_set_marker_blinking(s32 index, u8 blinking);

/* Player markers */
void minimap_update_player(s32 player, f32 x, f32 z, f32 rotation);
void minimap_set_player_color(s32 player, u8 r, u8 g, u8 b);

/* Coordinate conversion */
void minimap_world_to_screen(f32 world_x, f32 world_z, s16 *screen_x, s16 *screen_y);
void minimap_screen_to_world(s16 screen_x, s16 screen_y, f32 *world_x, f32 *world_z);

/* Update and render */
void minimap_update(void);
void minimap_draw(void);
void minimap_draw_track(void);
void minimap_draw_markers(void);
void minimap_draw_player_marker(s32 player);
void minimap_draw_border(void);

/* Split-screen support */
void minimap_set_viewport(s32 player, s16 x, s16 y, s16 w, s16 h);
void minimap_draw_for_player(s32 player);

/* Arrow indicator */
void minimap_draw_direction_arrow(f32 target_x, f32 target_z);
void minimap_point_to_checkpoint(s32 checkpoint);
void minimap_point_to_position(f32 x, f32 z);

#endif /* MINIMAP_H */
