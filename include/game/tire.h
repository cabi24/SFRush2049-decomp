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

/* Unit vector structure for tire calculations */
#ifndef UVECT_DEFINED
#define UVECT_DEFINED
typedef struct UVect {
    s16     uvs[3][3];      /* Short unit vectors */
    f32     fuvs[3][3];     /* Float unit vectors */
} UVect;
#endif

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
#ifndef TIRESTATE_DEFINED
#define TIRESTATE_DEFINED
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
#endif

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

/* ========================================================================
 * Arcade-compatible function aliases (tires.c)
 * ======================================================================== */

/* Forward declare MODELDAT from physics.h for arcade compatibility */
struct CarPhysics;
typedef struct CarPhysics MODELDAT;

/* Tire description alias (arcade uses lowercase) */
typedef TireDes tiredes;

/* Arcade constants */
#ifndef radtosdeg
#define radtosdeg       (16384.0f / 3.14159265f)    /* Radians to short degrees */
#endif
#define DIDDLE          0                           /* Debug flag (disabled) */

/* Unit vector short operations (arcade uvect.c) */
extern s16 mssin(s16 ang);
extern s16 mscos(s16 ang);
extern void mpitch(s16 sptch, s16 cptch, s16 uvs[3][3]);
extern void myaw(s16 syaw, s16 cyaw, s16 uvs[3][3]);
extern void matcopy(s16 *src, s16 *dst);
extern void transpose(s16 src[3][3], s16 dst[3][3]);
extern void makefpuvs(UVect *uv);
extern void shypotsincos(s16 x, s16 y, s16 *sval, s16 *cval);
extern void sdirection(s16 *vec, s16 *result);
extern void scrossprod(s16 *a, s16 *b, s16 *result);

/* Vector operations (arcade vecmath.c) */
extern void crossprod(f32 *a, f32 *b, f32 *result);
extern void vecadd(f32 *a, f32 *b, f32 *result);
extern void vecsub(f32 *a, f32 *b, f32 *result);
extern void veccopy(f32 *src, f32 *dst);
extern void bodtorw(f32 *bod, f32 *rw, UVect *uv);
extern void rwtobod(f32 *rw, f32 *bod, UVect *uv);
extern void srwtobod(s16 *rw, s16 *bod, UVect *uv);
extern void irwtobod(s32 *rw, s32 *bod, UVect *uv);
extern void ibodtorw(s32 *bod, s32 *rw, UVect *uv);

/* Arcade tire functions (tires.c) */

/**
 * calctireuv - Calculate tire unit vectors and velocity
 * Based on arcade: tires.c:calctireuv()
 *
 * @param v     Body velocity
 * @param w     Angular velocity
 * @param r     Tire position from CG
 * @param steer Steer angle (radians)
 * @param caruvs    Car orientation
 * @param roaduvs   Road surface orientation
 * @param tireuvs   Output: tire orientation
 * @param tirev     Output: tire velocity in tire coords
 */
void calctireuv(f32 v[3], f32 w[3], f32 r[3], f32 steer,
                UVect *caruvs, UVect *roaduvs,
                UVect *tireuvs, f32 tirev[3]);

/**
 * dotireforce - Calculate tire forces with suspension
 * Based on arcade: tires.c:dotireforce()
 *
 * @param m         Model data (mass, dt, etc.)
 * @param tirev     Tire velocity in tire coords
 * @param ottirev   Other tire velocity (for anti-roll)
 * @param tireuvs   Tire orientation
 * @param tire      Tire parameters
 * @param torque    Applied torque
 * @param forcevec  Output: force in car coords
 * @param suscomp   Suspension compression
 * @param otsuscomp Other tire suspension compression
 * @param springrate Suspension spring rate
 * @param arspringrate Anti-roll bar spring rate
 * @param cdamping  Compression damping
 * @param rdamping  Rebound damping
 * @param poortract Poor traction flag
 * @param airfact   Air factor (0-1)
 */
void dotireforce(MODELDAT *m, f32 tirev[3], f32 ottirev[3], UVect *tireuvs,
                 tiredes *tire, f32 torque, f32 forcevec[3], f32 suscomp,
                 f32 otsuscomp, f32 springrate, f32 arspringrate,
                 f32 cdamping, f32 rdamping, s32 poortract, f32 airfact);

/**
 * make_tire_road_uvs - Create tire-road unit vectors
 * Based on arcade: tires.c:make_tire_road_uvs()
 */
void make_tire_road_uvs(UVect *caruvs, f32 steer, UVect *roaduvs, UVect *truvs);

/**
 * frictioncircle - Friction circle calculation (original arcade signature)
 * Based on arcade: tires.c:frictioncircle()
 *
 * This is the core tire force calculation using the Milliken friction circle model.
 * The friction circle limits total tire force based on normal force and friction
 * coefficient. Traction and lateral forces share this limit.
 *
 * @param m         Model data (mass, dt, weight, idt)
 * @param tirev     Tire velocity in tire coords (X=forward, Y=lateral, Z=vertical)
 * @param normalforce   Normal force on tire (lbs)
 * @param torque    Applied torque (ft-lbs)
 * @param tire      Tire parameters
 * @param sfp       Output: sideforce (lateral force, lbs)
 * @param trp       Output: traction force (longitudinal force, lbs)
 */
void frictioncircle(ModelDat *m, f32 tirev[3], f32 normalforce, f32 torque,
                    TireDes *tire, f32 *sfp, f32 *trp);

/**
 * frictioncircle_m - Friction circle calculation with MODELDAT pointer
 * Based on arcade: tires.c:frictioncircle()
 *
 * Wrapper for tire_friction_circle that uses the full MODELDAT/CarPhysics pointer.
 * Extracts mass, weight, dt, and idt fields for the calculation.
 */
void frictioncircle_m(MODELDAT *m, f32 tirev[3], f32 normalforce, f32 torque,
                      tiredes *tire, f32 *sfp, f32 *trp);

/**
 * calcalpha - Calculate slip angle from tire velocity
 * Based on arcade: tires.c:calcalpha()
 *
 * Computes the slip angle (lateral slip) from the tire velocity vector.
 * Uses DAMPSPEED threshold to prevent numerical instability at low speeds.
 *
 * @param tirev     Tire velocity in tire coords
 * @return          Slip angle (radians, positive = counterclockwise)
 */
f32 calcalpha(f32 tirev[3]);

#endif /* TIRE_H */
