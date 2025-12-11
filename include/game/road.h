/**
 * road.h - Track/road surface system for Rush 2049 N64
 *
 * Based on arcade game/road.c
 * Handles tire/surface interaction, body surface detection, collision calling
 */

#ifndef ROAD_H
#define ROAD_H

#include "types.h"
#include "game/physics.h"

/* Road surface codes (from arcade road.h) */
#define ROAD_ASPHALT    0   /* Normal road */
#define ROAD_DIRT       1   /* Dirt/gravel */
#define ROAD_GRASS      2   /* Grass */
#define ROAD_AIR        3   /* In the air (no surface) */
#define ROAD_WATER      4   /* Water */
#define ROAD_ICE        5   /* Ice/slippery */
#define ROAD_SAND       6   /* Sand */
#define ROAD_BOOST      7   /* Speed boost pad */
#define ROAD_JUMP       8   /* Jump ramp */
#define ROAD_WALL       9   /* Wall collision */
#define ROAD_SHORTCUT   10  /* Shortcut surface */

/* Road interaction flags */
#define ROAD_FLAG_BOOST     0x01    /* Surface provides boost */
#define ROAD_FLAG_SHORTCUT  0x02    /* On shortcut path */
#define ROAD_FLAG_BUMPY     0x04    /* Bumpy surface */
#define ROAD_FLAG_WET       0x08    /* Wet surface */
#define ROAD_FLAG_RUMBLE    0x10    /* Rumble strip */

/* Friction coefficients per surface type */
#define FRICTION_ASPHALT    1.0f
#define FRICTION_DIRT       0.7f
#define FRICTION_GRASS      0.5f
#define FRICTION_ICE        0.2f
#define FRICTION_SAND       0.4f
#define FRICTION_WATER      0.3f
#define FRICTION_BOOST      1.2f

/* Surface interaction result */
typedef struct SurfaceInfo {
    f32     ground_pos[3];      /* Ground position under tire */
    f32     ground_normal[3];   /* Surface normal vector */
    f32     friction;           /* Surface friction coefficient */
    u8      roadcode;           /* Surface type code */
    u8      flags;              /* Surface flags */
    u8      boost_amount;       /* Boost strength if applicable */
    u8      pad;
} SurfaceInfo;

/* Road segment structure */
typedef struct RoadSegment {
    f32     start_pos[3];       /* Segment start position */
    f32     end_pos[3];         /* Segment end position */
    f32     width;              /* Road width */
    f32     banking;            /* Road banking angle */
    u8      surface_type;       /* Primary surface type */
    u8      flags;              /* Segment flags */
    u16     next_segment;       /* Index of next segment */
    u16     alt_segment;        /* Alternate path segment */
    u16     checkpoint_id;      /* Associated checkpoint */
} RoadSegment;

/* Track data structure */
typedef struct TrackData {
    s32         num_segments;       /* Total road segments */
    RoadSegment *segments;          /* Segment array */
    f32         total_length;       /* Track total length */
    s32         num_checkpoints;    /* Number of checkpoints */
    s32         num_laps;           /* Laps for this track */
    u8          track_id;           /* Track identifier */
    u8          difficulty;         /* Track difficulty */
    u8          has_shortcuts;      /* Track has shortcuts */
    u8          pad;
} TrackData;

/* Current track state */
extern TrackData *gCurrentTrack;
extern s32 gTrackLoaded;

/* Road system functions */
void road_init(void);
void road_reset(void);

/* Surface detection */
void road_update(CarPhysics *m);
void road_tire_surface(CarPhysics *m, f32 tire_pos[3], f32 ground_pos[3],
                       u8 *roadcode, f32 road_uvs[3][3], s32 tire_index);
void road_body_surface(CarPhysics *m, f32 body_pos[3], f32 body_vel[3],
                       f32 uvs[3][3], s32 corner_index);

/* Surface info queries */
f32 road_get_friction(u8 roadcode);
u8 road_get_flags(u8 roadcode);
s32 road_is_boost_surface(u8 roadcode);
s32 road_is_shortcut(u8 roadcode);

/* Track loading */
void road_load_track(s32 track_id);
void road_unload_track(void);
TrackData* road_get_track_data(void);

/* Track queries */
f32 road_get_track_length(void);
s32 road_get_segment_at_pos(f32 pos[3]);
void road_get_road_direction(f32 pos[3], f32 dir[3]);
f32 road_get_road_width(f32 pos[3]);

/* Arcade compatibility - init_road */
void init_road(CarPhysics *m);

/* Vector interpolation helpers */
void uvinterp(s16 *uv1, s32 c1, s32 s1, s16 *uv2, s16 *res);
void vecinterp(s32 v1[3], s32 c1, s32 s1, s32 v2[3], s32 res[3]);

#endif /* ROAD_H */
