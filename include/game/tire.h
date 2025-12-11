/**
 * tire.h - Tire physics for Rush 2049 N64
 *
 * Based on arcade tires.c - calculates forces between tires and road.
 * Uses friction circle model with slip angle calculations.
 */

#ifndef TIRE_H
#define TIRE_H

#include "types.h"
#include "game/car.h"

/* Coordinate indices */
#define XCOMP   0
#define YCOMP   1
#define ZCOMP   2

/* Slip flags (debug/sound) */
#define SLIP_NONE           0
#define SLIP_LOCKED_BRAKE   10
#define SLIP_ACCEL_SPIN     20
#define SLIP_DECEL_SPIN     30
#define SLIP_BRAKE_SPIN     40
#define SLIP_NO_LOAD        3
#define SLIP_MAX_ACCEL      4
#define SLIP_MAX_BRAKE      5
#define SLIP_SIDE_MAX_POS   6
#define SLIP_SIDE_GRIP_POS  7
#define SLIP_SIDE_MAX_NEG   8
#define SLIP_SIDE_GRIP_NEG  9

/* Surface types */
#define SURFACE_PAVEMENT    0
#define SURFACE_DIRT        1
#define SURFACE_GRASS       2
#define SURFACE_SAND        3
#define SURFACE_ICE         4
#define SURFACE_WATER       5

/* Unit vector structure (integer scaled) */
typedef struct UVect {
    s16     uvs[3][3];      /* Short unit vectors */
    f32     fuvs[3][3];     /* Float unit vectors */
} UVect;

/* Tire force output */
typedef struct TireForce {
    f32     force[3];       /* Force vector (body coords) */
    f32     sideforce;      /* Lateral force component */
    f32     traction;       /* Longitudinal force component */
    f32     normal;         /* Normal (vertical) force */
    s32     slipflag;       /* Slip state for sound */
} TireForce;

/* Suspension state */
typedef struct SuspensionState {
    f32     compression;    /* Current compression (feet) */
    f32     velocity;       /* Compression velocity (ft/sec) */
    f32     airdist;        /* Distance to ground */
    f32     prev_comp;      /* Previous compression */
} SuspensionState;

/* Per-tire runtime state */
typedef struct TireState {
    /* Tire physics */
    TireDes         params;         /* Tire parameters (copy for runtime mods) */
    SuspensionState suspension;     /* Suspension state */
    TireForce       force;          /* Computed forces */

    /* Orientation */
    UVect           uvs;            /* Tire unit vectors */
    f32             steer;          /* Steer angle (rad, front only) */

    /* Velocities */
    f32             tirev[3];       /* Tire velocity (tire coords) */
    f32             angvel;         /* Wheel angular velocity */

    /* Position */
    f32             contact[3];     /* Ground contact position */
    f32             roadnorm[3];    /* Road surface normal */

    /* Surface */
    u8              surface;        /* Surface type */
    u8              onground;       /* Tire touching ground */
    u8              pad[2];
} TireState;

/* Model data structure (simplified from arcade MODELDAT) */
typedef struct ModelDat {
    f32     mass;           /* Vehicle mass */
    f32     weight;         /* Vehicle weight */
    f32     dt;             /* Time step */
    f32     idt;            /* Inverse dt */
} ModelDat;

/* Initialization */
void tire_init(TireState *state, const TireDes *params);
void tire_reset(TireState *state);

/* Tire constants calculation (when surface changes) */
void tire_constants(TireDes *tire);

/* Main tire force calculation */
void tire_calc_force(
    ModelDat *m,
    TireState *tire,
    f32 torque,
    f32 springrate,
    f32 arspringrate,
    f32 cdamping,
    f32 rdamping,
    f32 other_suscomp,
    f32 airfact
);

/* Friction circle calculation */
void tire_friction_circle(
    ModelDat *m,
    f32 tirev[3],
    f32 normalforce,
    f32 torque,
    TireDes *tire,
    f32 *sideforce,
    f32 *traction
);

/* Tire unit vector calculation */
void tire_calc_uv(
    f32 v[3],               /* Body velocity */
    f32 w[3],               /* Angular velocity */
    f32 r[3],               /* Tire position from CG */
    f32 steer,              /* Steer angle (rad) */
    UVect *caruvs,          /* Car orientation */
    UVect *roaduvs,         /* Road orientation */
    UVect *tireuvs,         /* Output: tire orientation */
    f32 tirev[3]            /* Output: tire velocity */
);

/* Calculate slip angle */
f32 tire_calc_alpha(f32 tirev[3]);

/* Suspension force calculation */
f32 tire_calc_suspension(
    f32 compression,
    f32 velocity,
    f32 springrate,
    f32 damping_c,
    f32 damping_r
);

/* Surface friction modifier */
f32 tire_surface_friction(u8 surface);

/* Utility */
void tire_update_constants(TireState *tire, u8 surface);

/* Sound interface */
s32 tire_get_screech_level(TireState *tire);
s32 tire_is_spinning(TireState *tire);

#endif /* TIRE_H */
