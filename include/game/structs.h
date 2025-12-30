/**
 * structs.h - Game data structures for Rush 2049
 *
 * Adapted from arcade source (rushtherock) for N64 platform.
 * Key structures for car physics, player data, and game state.
 */

#ifndef STRUCTS_H
#define STRUCTS_H

#include "types.h"

/* Maximum values */
#define MAX_CARS        8       /* Maximum cars in game */
#define MAX_GEARS       6       /* Max transmission gears (MAXGEAR+2) */
#define NUM_TIRES       4       /* Tires per car */
#define MAX_VISUALS     8       /* Visual effects per car */

/* Difficulty levels */
#define DIFFICULTY_EASY     0
#define DIFFICULTY_MEDIUM   1
#define DIFFICULTY_HARD     2
#define DIFFICULTY_EXPERT   3

/* Unlock flags */
#define UNLOCK_CAR_5        0x0001
#define UNLOCK_CAR_6        0x0002
#define UNLOCK_CAR_7        0x0004
#define UNLOCK_CAR_8        0x0008
#define UNLOCK_TRACK_5      0x0010
#define UNLOCK_TRACK_6      0x0020
#define UNLOCK_TRACK_7      0x0040
#define UNLOCK_MIRROR_MODE  0x0100

/* Forward declarations */
struct CarParams;
struct CarData;
struct Reckon;

/**
 * Dead Reckoning Data - Used for network/prediction
 * Based on arcade RECKON struct (modeldat.h)
 * Address pattern: car_array elements at 0x80152818
 */
typedef struct Reckon {
    /* Base data for dead reckoning */
    s32     base_time;          /* 0x00: Base time (10 microsec units) */
    f32     base_fudge;         /* 0x04: Time fudge factor */
    f32     base_RWA[3];        /* 0x08: Base real world acceleration */
    f32     base_RWV[3];        /* 0x14: Base real world velocity */
    f32     base_RWR[3];        /* 0x20: Base real world position */
    f32     base_W[3];          /* 0x2C: Base angular velocity */
    f32     base_UV[3][3];      /* 0x38: Base rotation matrix (unit vectors) */
    f32     base_airdist[4];    /* 0x5C: Shadow distance per tire */
    f32     base_airvel[4];     /* 0x6C: Shadow velocity per tire */
    f32     base_quat[4];       /* 0x7C: Base quaternion */
    f32     delta_quat[4];      /* 0x8C: Delta quaternion */
    s16     num_quat_steps;     /* 0x9C: Steps to target quat */

    /* Updated on change */
    s16     torque;             /* 0x9E: Engine torque (ft*lbs) */
    u16     rpm;                /* 0xA0: Engine RPM */
    f32     steerangle;         /* 0xA4: Front tire steering angle (rad) */
    f32     tireW[4];           /* 0xA8: Tire rolling velocity (rad/sec) */
    f32     suscomp[4];         /* 0xB8: Suspension compression (ft) */
    f32     airdist[4];         /* 0xC8: Distance to road per tire (ft) */
    u32     look;               /* 0xD8: Engine type/spare */
    u32     appearance;         /* 0xDC: Visual appearance flags */

    /* Dead reckoned data */
    f32     RWV[3];             /* 0xE0: Real world velocity */
    f32     RWR[3];             /* 0xEC: Real world position */
    f32     UV[3][3];           /* 0xF8: Rotation matrix */
} Reckon; /* size ~0x11C */

/**
 * Car Runtime Data - Per-car game state
 * Based on arcade CAR_DATA struct (modeldat.h)
 * N64 car_array at 0x80152818
 */
typedef struct CarData {
    /* Dead reckoned display coordinates */
    f32     dr_pos[3];          /* 0x00: Dead reckoned position */
    f32     dr_vel[3];          /* 0x0C: Dead reckoned velocity */
    f32     dr_uvs[3][3];       /* 0x18: Dead reckoned orientation */
    f32     dr_tirepos[4][3];   /* 0x3C: Dead reckoned tire positions */
    f32     dr_acc[3];          /* 0x6C: Reckon base acceleration */

    /* World coordinates */
    f32     RWV[3];             /* Real world velocity */
    f32     RWR[3];             /* Real world position */
    f32     V[3];               /* Velocity in world coords */
    f32     TIRERWR[4][3];      /* Tire positions (real world) */
    f32     TIRER[4][3];        /* Tire positions (body coords) */
    f32     tireW[4];           /* Tire rolling velocity (rad/sec) */

    /* Status */
    f32     mph;                /* Speedometer reading */
    s16     crashflag;          /* Crashed flag */
    s16     rpm;                /* Engine RPM */
    s16     engine_type;        /* Engine type for sound */
    s16     body_type;          /* Car body style */
    u32     appearance;         /* Appearance flags (smoke, damage) */
    u8      data_valid;         /* Data updated at least once */
    s8      place;              /* Race position (1st, 2nd, etc) */
    s8      place_locked;       /* End-of-race place locked? */
    u8      pad1;
    u32     gameover_time;      /* Gameover time resend flag */
    u32     score;              /* Player score at finish */
    s32     objnum;             /* Object number */
    f32     distance;           /* Distance from start */

    /* Crash state */
    s8      we_died;            /* Should resurrect */
    s8      just_crashed;       /* In death throes */
    u8      pad2[2];
    u32     crashtime;          /* Crash timestamp */

    /* Collision */
    s16     collidable;         /* Is collidable */
    u8      pad3[2];
    u32     collide_time;       /* Collision timestamp */
    s8      collide_count;      /* Transparency countdown */
    s8      collide_state;      /* Transparency state */

    /* AI/Path data */
    s32     fwd_progress_t;     /* Help arrows timestamp */
    f32     last_distance;      /* Previous forward progress */
    u16     sound_flags;        /* Sound flags */
    u16     shortcut_flags;     /* Shortcut flags */
    s32     sound_flag_time;    /* Sound flag timestamp */
    s32     scut_flag_time;     /* Shortcut flag timestamp */
    s16     old_shortcut;       /* Last shortcut section */
    u8      pad4[2];
    f32     shortcut_pos[3];    /* Shortcut entry point */
    s16     last_mpath;         /* Last maxpath before shortcut */
    s16     weight_index;       /* Path weighting slot */
    s8      difficulty;         /* Drone difficulty */
    s8      pad5[3];

    /* Shadow/road */
    f32     road_norm[4][3];    /* Road normals for shadow */
    s8      in_tunnel;          /* In tunnel flag */
    s8      laps;               /* Current lap */
    s8      mpath_laps;         /* Maxpath version of lap */
    s8      syncmpath_laps;     /* Target maxpath laps */
    s8      checkpoint;         /* Current checkpoint */
    s8      pad6[3];
    s32     lap_sync_time;      /* Lap sync duration */

    /* Orientation angles (derived from dr_uvs matrix) */
    f32     yaw;                /* Heading angle */
    f32     pitch;              /* Pitch angle */
    f32     roll;               /* Roll angle */

    /* Ground tracking */
    f32     ground_height;      /* Height of ground below car */
    f32     air_time;           /* Time since leaving ground */
} CarData;

/**
 * Game Structure - Main game state
 * Located at 0x801461D0 (160 references)
 * Size estimated at ~0x300 based on reference patterns
 */
typedef struct GameStruct {
    /* Basic state */
    u8      state;              /* 0x00: Current game state (gstate at 0x801146EC offset?) */
    u8      prev_state;         /* 0x01: Previous game state */
    u8      num_players;        /* 0x02: Number of players */
    u8      num_cars;           /* 0x03: Total cars in race */

    /* Race settings */
    s32     track_id;           /* Track selection */
    s32     difficulty;         /* Game difficulty */
    s32     num_laps;           /* Number of laps */

    /* Timing */
    u32     frame_count;        /* Frame counter (0x80142AFC) */
    f32     game_time;          /* Elapsed game time */
    f32     countdown_time;     /* Countdown timer */

    /* Unknown - needs reverse engineering */
    u8      unk[0x200];         /* Placeholder for remaining fields */
} GameStruct;

/**
 * Static Car Parameters - Vehicle physics constants
 * Based on arcade Car struct (drivsym.h)
 * These are ROM constants, not runtime data
 */
typedef struct CarParams {
    /* Identification */
    char    *name;              /* Car name string */

    /* Mass and inertia */
    f32     mass;               /* Mass in slugs */
    f32     I[3];               /* Moment of inertia vector */

    /* Suspension */
    f32     springrate[4];      /* Spring constant per tire */
    f32     farspringrate;      /* Front anti-roll bar rate */
    f32     rarspringrate;      /* Rear anti-roll bar rate */
    f32     cdamping[4];        /* Compression damping */
    f32     rdamping[4];        /* Rebound damping */

    /* Steering */
    f32     steerratio;         /* Steering ratio */
    f32     swtpg;              /* Steering wheel torque per G */
    s16     maxswdamp;          /* Max steering damping */
    s16     minswfrict;         /* Min steering friction */
    s16     maxswfrict;         /* Max steering friction */

    /* Aerodynamics */
    f32     srefpcybo2;         /* Drag factor */
    f32     rollresist;         /* Rolling resistance (lbs) */

    /* Tire positions - vectors from CG to each tire */
    f32     TIRER[4][3];        /* Tire position vectors */

    /* Braking */
    f32     brakebal;           /* Front brake balance */

    /* Engine */
    f32     engmi;              /* Engine flywheel moment of inertia */
    f32     dwratio;            /* Differential gear ratio */
    f32     clutchmaxt;         /* Max clutch torque */

    /* View */
    f32     viewheight;         /* Observer height above CG */

    /* Flags */
    s8      nothrusttorque;     /* Zero yaw torque from thrust */
    s8      magicdif;           /* Magic torque split differential */

    /* Torque */
    f32     fgtorquescale;      /* First gear torque scale */
    f32     sgtorquescale;      /* Second gear torque scale */
    f32     torquescale;        /* Gear 3+ torque scale */
    f32     dirttorquescale;    /* Off-road torque scale */
    f32     transarray[MAX_GEARS]; /* Transmission ratios */

    /* Torque curves (pointers to ROM data) */
    const s16 *torquecp;        /* Pavement torque curve */
    const s16 *dirttorquecp;    /* Dirt torque curve */
    s16     rpmperent;          /* RPM per torque curve entry */
    s16     topgear;            /* Top gear for auto trans */
    f32     upshiftangvel;      /* Auto upshift angular velocity */
    f32     downshiftangvel;    /* Auto downshift angular velocity */
} CarParams;

/* Global variable declarations */
extern u8 gstate;                       /* 0x801146EC - game state byte */
extern CarData car_array[];             /* 0x80152818 - array of car data */
extern GameStruct game_struct;          /* 0x801461D0 - main game structure */
extern u32 frame_counter;               /* 0x80142AFC - frame counter */

/* N64 State Flags (bitmasks used with gstate) */
#define GSTATE_INIT     0x40000     /* Initialization phase */
#define GSTATE_SETUP    0x80000     /* Setup phase */
#define GSTATE_MENU     0x100000    /* In menu */
#define GSTATE_CAMERA   0x200000    /* Camera active */
#define GSTATE_READY    0x400000    /* Ready to play */
#define GSTATE_PLAY     0x800000    /* Gameplay active */
#define GSTATE_CARS     0x1000000   /* Cars active */
#define GSTATE_FINISH   0x2000000   /* Race finished */

/* Car appearance flags */
#define APP_F_SMOKE         0x0001  /* Smoke effect */
#define APP_F_DAMAGE_RF     0x0010  /* Right front damage */
#define APP_F_DAMAGE_LF     0x0020  /* Left front damage */
#define APP_F_DAMAGE_RR     0x0040  /* Right rear damage */
#define APP_F_DAMAGE_LR     0x0080  /* Left rear damage */
#define APP_F_DAMAGE_TOP    0x0100  /* Top damage */

/* Drone types */
#define DRONE_NONE      0   /* No car */
#define DRONE_AI        1   /* AI drone */
#define DRONE_HUMAN     2   /* Human player */

#endif /* STRUCTS_H */
