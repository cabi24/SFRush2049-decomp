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
#include "game/car.h"   /* For TireDes, Car, CollSize, NUM_GEARS */

/* Coordinate system indices */
#define XCOMP   0   /* Forward direction */
#define YCOMP   1   /* Left direction */
#define ZCOMP   2   /* Up direction */

/* Tire indices (alternative names) */
#define FLTIRE  TIRE_LF   /* Front left */
#define FRTIRE  TIRE_RF   /* Front right */
#define RLTIRE  TIRE_LR   /* Rear left */
#define RRTIRE  TIRE_RR   /* Rear right */

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
#ifndef AIR
#define AIR             ROAD_AIR    /* Arcade road code alias */
#endif
#ifndef DIRT
#define DIRT            ROAD_DIRT   /* Arcade road code alias */
#endif
#ifndef PAVE
#define PAVE            ROAD_ASPHALT /* Arcade road code alias */
#endif

/* Anti-spin table - per car type and difficulty */
#define NUM_CAR_TYPES_PHYS  4       /* Different from car.h NUM_CAR_TYPES */
#define NUM_DIFF_OPT    3

/* Gear and transmission limits (use NUM_GEARS from car.h) */
#define MAXGEAR         (MAX_GEAR)  /* Maximum forward gears (from car.h) */

/******* ARCADE CAR PARAMETER STRUCTURES *******/
/* TireDes, Car, CollSize are defined in game/car.h */

/* External car parameter tables */
extern const s16 stdtorquecurve[TORQUE_THROTTLE_STEPS][TORQUE_RPM_STEPS];
extern const s16 rushtorquecurve[TORQUE_THROTTLE_STEPS][TORQUE_RPM_STEPS];
extern const Car * const carlist[];         /* Available cars */
extern const CollSize car_collsizes[];      /* Collision box sizes */

/* Tire state - simplified version (PhysTireState to avoid conflict with tire.h) */
#ifndef TIRESTATE_DEFINED
#define TIRESTATE_DEFINED
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
#endif

/* Suspension state */
typedef struct SuspState {
    f32     compression;    /* Current compression (feet) */
    f32     velocity;       /* Compression velocity */
    f32     springrate;     /* Spring rate (lbs/ft) */
    f32     damping;        /* Damping coefficient */
    f32     travel;         /* Max travel distance */
} SuspState;

/* Orientation matrix (unit vectors) - simplified version
 * Note: If tire.h's UVect is used (with uvs/fuvs), this will be skipped */
#ifndef UVECT_DEFINED
#define UVECT_DEFINED
typedef struct UVect {
    f32     fpuvs[3][3];    /* Forward, Left, Up vectors in world coords */
} UVect;
#endif

/**
 * PhysReckon - Dead reckoning data for physics (arcade RECKON)
 * Used for network synchronization and collision detection
 */
typedef struct PhysReckon {
    /* Base data for dead reckoning */
    s32     base_time;          /* Base time (10 microsec units) */
    f32     base_fudge;         /* Time fudge factor */
    f32     base_RWA[3];        /* Base real world acceleration */
    f32     base_RWV[3];        /* Base real world velocity */
    f32     base_RWR[3];        /* Base real world position */
    f32     base_W[3];          /* Base angular velocity */
    f32     base_UV[3][3];      /* Base rotation matrix */
    f32     base_airdist[4];    /* Shadow distance per tire */
    f32     base_airvel[4];     /* Shadow velocity per tire */
    f32     base_quat[4];       /* Base quaternion */
    f32     delta_quat[4];      /* Delta quaternion */
    s16     num_quat_steps;     /* Steps to target quat */
    s16     torque_rk;          /* Engine torque (ft*lbs) */
    u16     rpm_rk;             /* Engine RPM */
    u16     pad_rk;             /* Padding */
    f32     steerangle_rk;      /* Front tire steering angle */
    f32     tireW[4];           /* Tire rolling velocity */
    f32     suscomp[4];         /* Suspension compression */
    f32     airdist[4];         /* Distance to road per tire */
    u32     look;               /* Engine type/spare */
    u32     appearance_rk;      /* Visual appearance */
    /* Dead reckoned data (current state) */
    f32     RWV[3];             /* Real world velocity */
    f32     RWR[3];             /* Real world position */
    f32     UV[3][3];           /* Rotation matrix */
} PhysReckon;

/* Forward declaration for track pieces (arcade STREETYP) */
struct StreetType;
typedef struct StreetType STREETYP;

/* Car physics state - simplified MODELDAT */
typedef struct CarPhysics {
    /* Position and orientation */
    f32     RWR[3];         /* Real world position (center of mass) */
    UVect   UV;             /* Orientation matrix */

    /* World-frame velocities (arcade RWV, RWA) */
    f32     RWV[3];         /* Real world velocity (ft/sec) */
    f32     RWA[3];         /* Real world acceleration (ft/sec^2) */

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

    /* Extended drivetrain state (arcade drivetra.c) */
    f32     engangvel;          /* Engine angular velocity (rad/sec) */
    f32     engtorque;          /* Engine output torque */
    f32     enginvmi;           /* Engine inverse moment of inertia */
    f32     clutchangvel;       /* Clutch angular velocity */
    f32     clutchtorque;       /* Torque through clutch */
    f32     clutchmaxt;         /* Max clutch torque capacity */
    f32     dwangvel;           /* Driveshaft angular velocity */
    f32     dwratio;            /* Differential ratio */
    f32     dwtorque;           /* Driveshaft torque */
    f32     dwinvmi;            /* Differential inverse MOI */
    f32     efdwinvmi;          /* Effective diff inverse MOI */
    f32     transratio;         /* Current transmission ratio */
    f32     totalratio;         /* Total ratio (trans * diff) */
    s32     gear;               /* Current gear (-1=R, 0=N, 1-4=fwd) */
    s32     commandgear;        /* Commanded gear (for manual) */
    s32     topgear;            /* Top forward gear */
    f32     shifttime;          /* Time of last shift */
    f32     upshiftangvel;      /* Auto upshift threshold */
    f32     downshiftangvel;    /* Auto downshift threshold */
    f32     transarray[MAXGEAR+2]; /* Gear ratios */
    const s16 *torquecurve;     /* Engine torque curve */
    const s16 *dirttorquecurve; /* Dirt torque curve */
    f32     rpmperent;          /* RPM per torque curve entry */
    f32     fgtorquescale;      /* First gear torque scale */
    f32     sgtorquescale;      /* Second gear torque scale */
    f32     torquescale;        /* Gear 3+ torque scale */
    f32     dirttorquescale;    /* Off-road torque scale */
    s32     ignition;           /* Ignition on */
    s32     startermotor;       /* Starter motor engaged */
    s32     magicdif;           /* Magic load-sensing differential */
    f32     amperes;            /* Ammeter reading */
    f32     oilpressure;        /* Oil pressure */
    f32     enginetemp;         /* Engine temperature */
    f32     weight;             /* Vehicle weight (mass * g) */
    f32     idt;                /* Inverse timestep (1/dt) */

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
    f32     catchup;            /* Catch-up speed multiplier (1.0 = normal) */

    /* Peak forces (for damage/effects) */
    f32     peak_center_force[2][3];    /* Min/max center forces */
    f32     peak_body_force[2][3];      /* Min/max body forces */

    /* Network/linking data (arcade modeldat.h) */
    s16     slot;               /* Index into gLink array */
    s16     in_game;            /* Player in current game */
    s16     collidable;         /* Collision enabled flag */
    s16     drone_type;         /* 0=no car, 1=drone, 2=human */
    s16     engine_type;        /* Engine sound model type */
    s16     body_type;          /* Car body style */
    u32     appearance;         /* Visual appearance flags (damage, etc) */

    /* Track reference (for collision init) */
    STREETYP *lasttp[4];        /* Last track piece per tire */

    /* Dead reckoning data (arcade RECKON) */
    PhysReckon reckon;          /* Dead reckoning state */

} CarPhysics;

/* Maximum linked cars (arcade MAX_LINKS) */
#define MAX_LINKS       8

/* External physics state */
extern CarPhysics car_physics[];
extern CarPhysics model[];          /* Arcade-compatible model array alias */
extern const f32 as_scale[];
extern const f32 as_road[][NUM_DIFF_OPT];

/* Damage appearance masks and shifts (arcade collision.c) */
extern const u32 gDamageMask[5];    /* Damage bitmasks per quad (0-3) and top (4) */
extern const s16 gDamageShift[5];   /* Bit shift per damage quad */
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

/* Vector math helpers - see game/vecmath.h for full declarations */
/* Note: vec_add, vec_sub, vec_copy, vec_scale, vec_cross are declared in vecmath.h */
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

/* Vector math and coordinate transforms - see game/vecmath.h for full declarations */
/* Arcade names: vecadd, vecsub, veccopy, scalmul, crossprod, magnitude, rwtobod, bodtorw */

#endif /* PHYSICS_H */
