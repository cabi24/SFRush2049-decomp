/**
 * physics.h - Car physics system for Rush 2049 N64
 *
 * Based on arcade drivsym.c (Milliken Engineering model)
 * Handles tire forces, suspension, anti-spin, and vehicle dynamics.
 *
 * Key concepts:
 * - MODELDAT: Main physics state structure (called CarPhysics here)
 * - 4 tires with independent suspension and forces
 * - Body-fixed coordinate system (X=forward, Y=left, Z=up)
 * - Anti-spin torque to prevent spinouts
 */

#ifndef PHYSICS_H
#define PHYSICS_H

#include "types.h"

/* Coordinate system indices */
#define XCOMP   0   /* Forward direction */
#define YCOMP   1   /* Left direction */
#define ZCOMP   2   /* Up direction */

/* Tire indices */
#define FLTIRE  0   /* Front left */
#define FRTIRE  1   /* Front right */
#define RLTIRE  2   /* Rear left */
#define RRTIRE  3   /* Rear right */

/* Road surface types */
#define ROAD_ASPHALT    0
#define ROAD_DIRT       1
#define ROAD_GRASS      2
#define ROAD_AIR        3   /* In the air */
#define ROAD_WATER      4

/* Physics constants */
#define GRAVITY         32.174f     /* ft/s^2 */
#define GRAVCON         32.174f     /* Alternative gravity name */
#define MAX_VELOCITY    1000.0f     /* fps (speed of sound limit) */
#define DT_PHYSICS      (1.0f/60.0f)  /* Physics timestep at 60Hz */
#define CTLSCALE        (1.0f/32767.0f) /* Control input scale */

/* Arcade drivsym.c constants */
#define maxrwr          10.0f       /* Max value for extended precision on RWR */
#define AIR             ROAD_AIR    /* Arcade road code alias */
#define DIRT            ROAD_DIRT   /* Arcade road code alias */
#define PAVE            ROAD_ASPHALT /* Arcade road code alias */

/* Anti-spin table - per car type and difficulty */
#define NUM_CAR_TYPES   4
#define NUM_DIFF_OPT    3

/* Gear and transmission limits */
#define MAXGEAR         6       /* Maximum number of gears */

/******* ARCADE CAR PARAMETER STRUCTURES *******/
/* Based on arcade drivsym.h Car and tiredes structs */

/**
 * TireDes - Tire parameters for physics model
 * Based on arcade: tiredes.h
 *
 * The Milliken tire model uses slip angle and longitudinal
 * slip to compute lateral and tractive forces.
 */
typedef struct TireDes {
    f32     tradius;        /* Tire radius (feet) */
    f32     springK;        /* Static lateral stiffness (lbs/ft) */
    f32     rubdamp;        /* Lateral damping coefficient */
    f32     PaveCstiff;     /* Cornering stiffness on pavement (lbs/rad) */
    f32     PaveCfmax;      /* Max cornering friction on pavement (g) */
    f32     Cstiff;         /* Current cornering stiffness */
    f32     Cfmax;          /* Current max cornering friction */
    f32     invmi;          /* 1/moment of inertia of wheel */
    f32     Zforce;         /* Vertical loading for cornering */
    f32     Afmax;          /* Slip angle at max force */
    f32     k1, k2, k3;     /* Lateral force polynomial coefficients */
    f32     l2, l3;         /* Derivative coefficients */
    f32     m1, m2, m3, m4; /* Aligning torque coefficients */
    f32     patchy;         /* Contact patch deformation */
    f32     angvel;         /* Current angular velocity */
    f32     sliptorque;     /* Torque from tire slipping */
    f32     sideforce;      /* For tire squeal sound */
    s32     slipflag;       /* Tire slipping flag */
} TireDes;

/**
 * Car - Complete car parameter definition
 * Based on arcade: drivsym.h struct car
 *
 * Defines all physical properties for a vehicle type.
 */
typedef struct Car {
    char    *name;              /* Name of car */
    f32     mass;               /* Mass in slugs (weight/32.2) */
    f32     I[3];               /* Moment of inertia (roll, pitch, yaw) */

    f32     springrate[4];      /* Suspension spring rates (lbs/ft) */
    f32     farspringrate;      /* Front anti-roll bar rate */
    f32     rarspringrate;      /* Rear anti-roll bar rate */
    f32     cdamping[4];        /* Compression damping (lbs-sec/ft) */
    f32     rdamping[4];        /* Rebound damping */

    f32     steerratio;         /* Steering ratio (deg/deg) */
    f32     swtpg;              /* Steering wheel torque per G */
    s16     maxswdamp;          /* Max steering damping */
    s16     minswfrict;         /* Min steering friction */
    s16     maxswfrict;         /* Max steering friction */
    s16     pad;                /* Alignment */

    f32     srefpcybo2;         /* Aerodynamic drag factor */
    f32     rollresist;         /* Rolling resistance (lbs) */

    TireDes tires[4];           /* Tire descriptions (FR, FL, RR, RL) */
    f32     TIRER[4][3];        /* Tire positions relative to CG */

    f32     brakebal;           /* Front brake balance (0-1) */
    f32     engmi;              /* Engine flywheel moment of inertia */
    f32     dwratio;            /* Differential gear ratio */
    f32     clutchmaxt;         /* Max clutch torque */
    f32     viewheight;         /* Observer height above CG */

    s8      nothrusttorque;     /* Flag for zero yaw torque from thrust */
    s8      magicdif;           /* Magic load-sensitive differential */
    s8      pad2[2];

    f32     fgtorquescale;      /* Torque scale in 1st gear */
    f32     sgtorquescale;      /* Torque scale in 2nd gear */
    f32     torquescale;        /* Torque scale in higher gears */
    f32     dirttorquescale;    /* Torque scale off-road */
    f32     transarray[MAXGEAR+2]; /* Transmission gear ratios */

    const s16 *torquecp;        /* Engine torque curve pointer */
    const s16 *dirttorquecp;    /* Dirt torque curve pointer */
    s16     rpmperent;          /* RPM per torque curve entry */
    s16     topgear;            /* Top gear for auto transmission */
    f32     upshiftangvel;      /* Upshift angular velocity */
    f32     downshiftangvel;    /* Downshift angular velocity */
} Car;

/**
 * CollSize - Car collision box dimensions
 * Based on arcade: drivsym.h COLLSIZE
 */
typedef struct CollSize {
    f32     colfront;           /* Distance to front */
    f32     colrear;            /* Distance to rear */
    f32     colside;            /* Half-width */
    f32     colheight;          /* Height */
} CollSize;

/* External car parameter tables */
extern const s16 stdtorquecurve[10][12];    /* Standard torque curve */
extern const s16 rushtorquecurve[10][12];   /* Rush-style torque curve */
extern const Car *carlist[];                /* Available cars */
extern const CollSize car_collsizes[];      /* Collision box sizes */

/* Tire state */
typedef struct TireState {
    f32     angvel;         /* Angular velocity (rad/s) */
    f32     slip_ratio;     /* Longitudinal slip */
    f32     slip_angle;     /* Lateral slip angle */
    f32     load;           /* Vertical load on tire */
    f32     radius;         /* Tire radius */
    f32     friction;       /* Current friction coefficient */
    u8      on_ground;      /* Is tire on ground? */
    u8      roadcode;       /* Surface type under tire */
    u8      pad[2];
} TireState;

/* Suspension state */
typedef struct SuspState {
    f32     compression;    /* Current compression (feet) */
    f32     velocity;       /* Compression velocity */
    f32     springrate;     /* Spring rate (lbs/ft) */
    f32     damping;        /* Damping coefficient */
    f32     travel;         /* Max travel distance */
} SuspState;

/* Orientation matrix (unit vectors) */
typedef struct UVect {
    f32     fpuvs[3][3];    /* Forward, Left, Up vectors in world coords */
} UVect;

/* Car physics state - simplified MODELDAT */
typedef struct CarPhysics {
    /* Position and orientation */
    f32     RWR[3];         /* Real world position (center of mass) */
    UVect   UV;             /* Orientation matrix */

    /* Velocities (body frame) */
    f32     V[3];           /* Linear velocity */
    f32     W[3];           /* Angular velocity (rad/s) */
    f32     magvel;         /* Speed magnitude */

    /* Accelerations */
    f32     A[3];           /* Linear acceleration */
    f32     AA[3];          /* Angular acceleration */

    /* Forces and moments (body frame) */
    f32     F[3];           /* Total force */
    f32     M[3];           /* Total moment (torque) */
    f32     G[3];           /* Gravity in body frame */
    f32     GRW[3];         /* Gravity in world frame */
    f32     D[3];           /* Drag force */

    /* Tire forces */
    f32     TIREFORCE[4][3];    /* Force from each tire */
    f32     TIREV[4][3];        /* Tire velocity */
    f32     TIRER[4][3];        /* Tire position relative to CG */

    /* Body corner forces (collision) */
    f32     BODYFORCE[4][3];    /* Force at body corners */
    f32     BODYR[4][3];        /* Body corner positions */

    /* Anti-spin */
    f32     CENTERFORCE[3];     /* Centering force */
    f32     CENTERMOMENT[3];    /* Anti-spin moment */

    /* Suspension */
    f32     suscomp[4];         /* Suspension compression */
    f32     springrate[4];      /* Spring rates */
    f32     cdamping[4];        /* Compression damping */
    f32     rdamping[4];        /* Rebound damping */
    f32     farspringrate;      /* Far spring rate (bump stop) */

    /* Tire state */
    TireState tires[4];
    UVect   tireuvs[4];         /* Tire orientation */
    UVect   roaduvs[4];         /* Road surface orientation */
    u8      roadcode[4];        /* Surface type per tire */

    /* Drivetrain */
    f32     torque[4];          /* Torque to each wheel */
    s32     rpm;                /* Engine RPM */
    f32     throttle;           /* Throttle position (0-1) */
    f32     brake;              /* Brake position (0-1) */
    f32     clutch;             /* Clutch position (0-1) */
    f32     steerangle;         /* Steering angle (radians) */

    /* Vehicle parameters */
    f32     mass;               /* Vehicle mass */
    f32     massinv;            /* 1/mass */
    f32     rollresist;         /* Rolling resistance */
    f32     srefpcybo2;         /* Aero drag coefficient */

    /* Steering feedback */
    f32     swtorque;           /* Steering wheel torque */
    f32     swdamp;             /* Steering damping */
    f32     swfrict;            /* Steering friction */
    f32     swtorquegain;       /* Torque feedback gain */
    f32     maxswdamp;          /* Max steering damping */
    f32     maxswfrict;         /* Max steering friction */
    f32     minswfrict;         /* Min steering friction */

    /* State flags */
    f32     thetime;            /* Current simulation time */
    f32     dt;                 /* Timestep */
    f32     airtime;            /* Last time in air */
    s32     crashflag;          /* Car has crashed */
    s32     gameover;           /* Game over state */
    s32     autotrans;          /* Auto transmission mode */
    s32     bog_state;          /* Engine bogging state */
    s32     nothrusttorque;     /* Disable thrust torque */

    /* Car type/identity */
    s32     carnum;             /* Car type index */
    s32     we_control;         /* Is this car AI-controlled? */
    s32     net_node;           /* Network node ID */
    f32     colrad;             /* Collision radius */

    /* Peak forces (for damage/effects) */
    f32     peak_center_force[2][3];    /* Min/max center forces */
    f32     peak_body_force[2][3];      /* Min/max body forces */

} CarPhysics;

/* External physics state */
extern CarPhysics car_physics[];
extern const f32 as_scale[];
extern const f32 as_road[][NUM_DIFF_OPT];
extern const f32 as_air[][NUM_DIFF_OPT];

/* Initialization */
void physics_init(CarPhysics *m);
void physics_init_all(void);

/* Main simulation step */
void physics_sym(CarPhysics *m);
void physics_syminit(CarPhysics *m);
void physics_regular(CarPhysics *m);

/* Force calculation */
void physics_forces(CarPhysics *m);
void physics_forces1(CarPhysics *m);
void physics_forces2(CarPhysics *m);
void physics_torques(CarPhysics *m);
void physics_antispin(CarPhysics *m);

/* Motion integration */
void physics_accelerations(CarPhysics *m);
void physics_velocities(CarPhysics *m);
void physics_positions(CarPhysics *m);

/* Input handling */
void physics_controls(CarPhysics *m);

/* Road/surface */
void physics_road(CarPhysics *m);

/* Drivetrain */
void physics_drivetrain(CarPhysics *m);

/* Tire forces */
void physics_calc_tire_uv(f32 V[3], f32 W[3], f32 tireR[3], f32 steer,
                          UVect *carUV, UVect *roadUV, UVect *tireUV, f32 tireV[3]);
void physics_do_tire_force(CarPhysics *m, f32 tireV[3], f32 otherV[3],
                           UVect *tireUV, TireState *tire, f32 torque,
                           f32 force[3], f32 comp, f32 otherComp,
                           f32 spring, f32 farspring, f32 cdamp, f32 rdamp,
                           s32 poortract, f32 airfact);

/* Validation */
void physics_checkok(CarPhysics *m);

/* Communication (network/display) */
void physics_mcommunication(CarPhysics *m);

/* Vector math helpers */
void vec_add(f32 a[3], f32 b[3], f32 out[3]);
void vec_sub(f32 a[3], f32 b[3], f32 out[3]);
void vec_copy(f32 src[3], f32 dst[3]);
void vec_scale(f32 v[3], f32 s, f32 out[3]);
void vec_cross(f32 a[3], f32 b[3], f32 out[3]);
f32  vec_magnitude(f32 v[3]);
void rw_to_body(f32 rw[3], f32 body[3], UVect *uv);
void body_to_rw(f32 body[3], f32 rw[3], UVect *uv);

/******* TIRE AND ENGINE FUNCTIONS *******/

/**
 * tire_constants - Calculate derived tire constants
 * Based on arcade: initiali.c:tire_constants()
 */
void tire_constants(TireDes *tdes);

/**
 * copy_tire_info - Copy tire parameters with load adjustment
 * Based on arcade: initiali.c:copy_tire_info()
 */
void copy_tire_info(f32 mass, TireDes *tp1, TireDes *tp2, f32 otw, f32 wb);

/**
 * get_engine_torque - Interpolate torque from curve
 * Based on arcade engine torque lookup
 */
f32 get_engine_torque(const s16 curve[10][12], f32 throttle, s32 rpm, s32 rpm_per_entry);

/**
 * init_car_from_params - Initialize car physics from Car parameters
 */
void init_car_from_params(CarPhysics *m, const Car *car);

/* ========================================================================
 * Arcade-compatible function aliases (drivsym.c)
 * ======================================================================== */

/* Arcade type aliases */
typedef CarPhysics MODELDAT;    /* Arcade uses MODELDAT for physics state */

/* Initialization (drivsym.c) */
void syminit(MODELDAT *m);
void initialize(MODELDAT *m);

/* Main simulation (drivsym.c) */
void sym(MODELDAT *m);
void regular(MODELDAT *m);

/* Force calculation (drivsym.c) */
void forces(MODELDAT *m);
void forces1(MODELDAT *m);
void forces2(MODELDAT *m);
void antispin(MODELDAT *m);
void torques(MODELDAT *m);

/* Motion integration (drivsym.c) */
void accelerations(MODELDAT *m);
void velocities(MODELDAT *m);
void positions(MODELDAT *m);

/* Controls and road (drivsym.c) */
void controls(MODELDAT *m);
void road(MODELDAT *m);

/* Communication (communic.c) */
void mcommunication(MODELDAT *m);

/* Validation (drivsym.c) */
void checkok(MODELDAT *m);

/* Drivetrain (drivetra.c) */
void drivetrain(MODELDAT *m);

/* Tire force calculation (tires.c) */
void calctireuv(f32 V[3], f32 W[3], f32 tireR[3], f32 steer,
                UVect *carUV, UVect *roadUV, UVect *tireUV, f32 tireV[3]);
void dotireforce(MODELDAT *m, f32 tireV[3], f32 otherV[3],
                 UVect *tireUV, TireDes *tire, f32 torque,
                 f32 force[3], f32 comp, f32 otherComp,
                 f32 spring, f32 farspring, f32 cdamp, f32 rdamp,
                 s32 poortract, f32 airfact);

/* Angular acceleration (drivsym.c) */
void calcaa(MODELDAT *m, f32 M[3], f32 AA[3]);

/* Vector math (vecmath.c arcade aliases) */
void vecadd(f32 a[3], f32 b[3], f32 out[3]);
void vecsub(f32 a[3], f32 b[3], f32 out[3]);
void veccopy(f32 src[3], f32 dst[3]);
void scalmul(f32 v[3], f32 s, f32 out[3]);
void crossprod(f32 a[3], f32 b[3], f32 out[3]);
f32  magnitude(f32 v[3]);

/* Coordinate transform (drivsym.c) */
void rwtobod(f32 rw[3], f32 body[3], UVect *uv);
void bodtorw(f32 body[3], f32 rw[3], UVect *uv);

#endif /* PHYSICS_H */
