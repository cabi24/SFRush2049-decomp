/**
 * car.h - Car physics parameters for Rush 2049 N64
 *
 * Based on arcade drivsym.h and cars.c
 * Defines vehicle physics constants, torque curves, and car configurations.
 */

#ifndef CAR_H
#define CAR_H

#include "types.h"

/* Physics constants */
#define GRAVITY_ACCEL       32.2f       /* ft/sec^2 */
#define PI_VAL              3.141592654f
#define RAD_TO_DEG          (180.0f / PI_VAL)
#define DEG_TO_RAD          (PI_VAL / 180.0f)
#define RPM_TO_RDPS         (2.0f * PI_VAL / 60.0f)     /* RPM to rad/sec */
#define RDPS_TO_RPM         (60.0f / (2.0f * PI_VAL))   /* rad/sec to RPM */

/* Gear limits */
#define MIN_GEAR            1           /* Minimum gear for auto trans */
#define MAX_GEAR            4           /* Number of forward speeds */
#define NUM_GEARS           (MAX_GEAR + 2)  /* Include reverse and neutral */

/* Torque curve dimensions */
#define TORQUE_THROTTLE_STEPS   10      /* Throttle positions (0-100%) */
#define TORQUE_RPM_STEPS        12      /* RPM columns */

/* Car count */
#define NUM_CAR_TYPES       8           /* Total car types in game */

/* Tire indices */
#define TIRE_RF             0           /* Right front */
#define TIRE_LF             1           /* Left front */
#define TIRE_RR             2           /* Right rear */
#define TIRE_LR             3           /* Left rear */
#define NUM_TIRES           4

/* Tire physics parameters */
typedef struct TireDes {
    f32     tradius;            /* Tire radius (feet) */
    f32     springK;            /* Static lateral stiffness */
    f32     rubdamp;            /* Lateral damping */
    f32     PaveCstiff;         /* Pavement cornering stiffness (lb/rad) */
    f32     PaveCfmax;          /* Pavement max friction coefficient */
    f32     Cstiff;             /* Current cornering stiffness */
    f32     Cfmax;              /* Current max friction coefficient */
    f32     invmi;              /* 1/moment of inertia */

    /* Runtime state (initialized to 0) */
    f32     Zforce;             /* Vertical loading */
    f32     Afmax;              /* Alpha at max force */
    f32     k1, k2, k3;         /* Force curve coefficients */
    f32     l2, l3;             /* Additional coefficients */
    f32     m1, m2, m3, m4;     /* Aligning torque coefficients */
    f32     patchy;             /* Contact patch Y */
    f32     angvel;             /* Rotational velocity (rad/sec) */
    f32     sliptorque;         /* Torque from slipping */
    f32     sideforce;          /* Lateral force (for sound) */
    f32     traction;           /* Traction level */
    s8      slipflag;           /* Tire is slipping */
    s8      pad[3];
} TireDes;

/* Collision box dimensions */
typedef struct CollSize {
    f32     colfront;           /* Front extent */
    f32     colrear;            /* Rear extent */
    f32     colright;           /* Right extent */
    f32     colleft;            /* Left extent */
    f32     coltop;             /* Top extent */
    f32     colbottom;          /* Bottom extent */
} CollSize;

/* Complete car physics definition */
typedef struct Car {
    const char  *name;          /* Car name string */

    /* Mass and inertia */
    f32     mass;               /* Mass in slugs */
    f32     I[3];               /* Moment of inertia (roll, pitch, yaw) */

    /* Suspension */
    f32     springrate[4];      /* Spring constant per tire */
    f32     farspringrate;      /* Front anti-roll bar rate */
    f32     rarspringrate;      /* Rear anti-roll bar rate */
    f32     cdamping[4];        /* Compression damping */
    f32     rdamping[4];        /* Rebound damping */

    /* Steering */
    f32     steerratio;         /* Steering ratio (wheel/kingpin) */
    f32     swtpg;              /* Steering wheel torque per G */
    s16     maxswdamp;          /* Max steering damping */
    s16     minswfrict;         /* Min steering friction */
    s16     maxswfrict;         /* Max steering friction */
    s16     pad;

    /* Aerodynamics */
    f32     srefpcybo2;         /* Drag factor */
    f32     rollresist;         /* Rolling resistance (lbs) */

    /* Tires */
    TireDes tires[4];           /* Tire parameters per wheel */

    /* Geometry */
    f32     TIRER[4][3];        /* Vector from CG to each tire */

    /* Braking */
    f32     brakebal;           /* Front brake balance (0-1) */

    /* Engine */
    f32     engmi;              /* Flywheel moment of inertia */
    f32     dwratio;            /* Differential gear ratio */
    f32     clutchmaxt;         /* Max clutch torque */

    /* View */
    f32     viewheight;         /* Observer height above CG */

    /* Collision */
    CollSize    collision;      /* Collision box */

    /* Flags */
    s8      nothrusttorque;     /* Zero yaw torque from thrust */
    s8      magicdif;           /* Magic torque-split differential */
    s8      pad2[2];

    /* Torque scaling */
    f32     fgtorquescale;      /* First gear torque scale */
    f32     sgtorquescale;      /* Second gear torque scale */
    f32     torquescale;        /* Gear 3+ torque scale */
    f32     dirttorquescale;    /* Off-road torque scale */

    /* Transmission */
    f32     transarray[NUM_GEARS];  /* Gear ratios */
    const s16   *torquecp;      /* Torque curve pointer */
    const s16   *dirttorquecp;  /* Dirt torque curve pointer */
    s16     rpmperent;          /* RPM per torque curve entry */
    s16     topgear;            /* Top gear for auto trans */
    f32     upshiftangvel;      /* Auto upshift angular velocity */
    f32     downshiftangvel;    /* Auto downshift angular velocity */
} Car;

/* Torque curve type */
typedef s16 TorqueCurve[TORQUE_THROTTLE_STEPS][TORQUE_RPM_STEPS];

/* Standard torque curves (ROM data) */
extern const TorqueCurve gStdTorqueCurve;       /* Standard engine */
extern const TorqueCurve gRushTorqueCurve;      /* Rush-style engine */
extern const TorqueCurve gDirtTorqueCurve;      /* Off-road */

/* Car definitions (ROM data) */
extern const Car gCarDefs[NUM_CAR_TYPES];

/* Car names */
extern const char *gCarNames[NUM_CAR_TYPES];

/* Initialization */
void car_physics_init(void);

/* Engine torque lookup */
s16 car_get_torque(const Car *car, f32 throttle, f32 rpm);
s16 car_get_dirt_torque(const Car *car, f32 throttle, f32 rpm);

/* Gear management */
s32 car_should_upshift(const Car *car, f32 angvel);
s32 car_should_downshift(const Car *car, f32 angvel);
f32 car_get_gear_ratio(const Car *car, s32 gear);

/* Queries */
f32 car_get_weight(const Car *car);
f32 car_get_wheelbase(const Car *car);
f32 car_get_track_width(const Car *car);

/* Runtime car state (per-car instance) */
typedef struct CarState {
    /* Kinematics */
    f32     RWR[3];             /* Real world position */
    f32     RWV[3];             /* Real world velocity */
    f32     V[3];               /* Body velocity */
    f32     W[3];               /* Angular velocity */
    f32     UV[3][3];           /* Orientation matrix */

    /* Engine */
    f32     rpm;                /* Engine RPM */
    f32     throttle;           /* Throttle position (0-1) */
    f32     torque;             /* Current engine torque */
    s32     gear;               /* Current gear */
    s32     autotrans;          /* Using automatic transmission */

    /* Controls */
    f32     steerangle;         /* Front tire steer angle (rad) */
    f32     brake;              /* Brake position (0-1) */
    f32     clutch;             /* Clutch position (0-1) */

    /* Tire state */
    f32     tireW[4];           /* Tire angular velocities */
    f32     suscomp[4];         /* Suspension compression */
    f32     TIRERWR[4][3];      /* Tire world positions */

    /* Status */
    f32     mph;                /* Speed in MPH */
    f32     distance;           /* Distance traveled */
    u32     flags;              /* State flags */

    /* Reference to static params */
    const Car   *cardef;        /* Pointer to car definition */
} CarState;

/* Car state flags */
#define CAR_FLAG_AIRBORNE       0x0001  /* All tires off ground */
#define CAR_FLAG_CRASHED        0x0002  /* Crashed/disabled */
#define CAR_FLAG_REVERSED       0x0004  /* Going backwards */
#define CAR_FLAG_BRAKING        0x0008  /* Brakes applied */
#define CAR_FLAG_WHEELSPIN      0x0010  /* Tires spinning */
#define CAR_FLAG_OFFROAD        0x0020  /* Off paved surface */
#define CAR_FLAG_IN_TUNNEL      0x0040  /* In tunnel */
#define CAR_FLAG_BOOSTING       0x0080  /* Speed boost active */

/* Car state management */
void car_state_init(CarState *state, const Car *cardef);
void car_state_reset(CarState *state);
void car_state_update(CarState *state, f32 dt);

/* Physics update */
void car_update_engine(CarState *state, f32 dt);
void car_update_transmission(CarState *state);
void car_update_steering(CarState *state, f32 wheel_input);
void car_update_brakes(CarState *state, f32 brake_input);

/* ========================================================================
 * Arcade-compatible definitions (drivsym.h, cars.c)
 * ======================================================================== */

/* Arcade unit conversion constants */
#define radtodeg        (180.0f / 3.141592654f)
#define degtorad        (3.141592654f / 180.0f)
#define radtoideg       (2147483648.0f / 3.141592654f)  /* 0x80000000 */
#define idegtorad       (3.141592654f / 2147483648.0f)
#define radtosdeg       (32768.0f / 3.141592654f)       /* 0x8000 */
#define sdegtorad       (3.141592654f / 32768.0f)
#define idegtodeg       (90.0f / 1073741824.0f)         /* 0x40000000 */
#define degtoideg       (1073741824.0f / 90.0f)
#define msininv         (1.0f / 32767.0f)               /* 0x7fff */
#define rdpstorpm       (60.0f / (2.0f * 3.14159f))
#define rpmtordps       (2.0f * 3.14159f / 60.0f)
#define pival           3.141592654f

/* Arcade car count and constants */
#define NCARS           12          /* Number of car models (arcade) */
#define MINGEAR_ARCADE  1           /* Minimum gear for auto trans */
#define MAXGEAR_ARCADE  4           /* Number of forward speeds */

/* Arcade collision size structure */
typedef struct CollSizeArcade {
    f32     colfront;               /* Distance to front */
    f32     colrear;                /* Distance to rear */
    f32     colside;                /* Half-width */
    f32     colheight;              /* Height */
} CollSizeArcade;

/* Arcade-style COLLSIZE typedef (compatibility) */
typedef CollSizeArcade COLLSIZE;

/* Arcade torque curve externs */
extern const s16 stdtorquecurve[10][12];    /* Standard torque curve */
extern const s16 stddirttorquecurve[10][12]; /* Dirt torque curve */
extern const s16 rushtorquecurve[10][12];   /* Rush-style torque curve */

/* Arcade car list and collision sizes */
extern const Car * const carlist[];         /* Pointer array to car defs */
extern const COLLSIZE collsize[];           /* Collision box sizes per car model */
extern const s16 num_cars;                  /* Number of cars in carlist */

/* Arcade-style car struct typedef (compatibility) */
typedef Car car;

/* Arcade function aliases */
#define car_init        car_physics_init

/* Arcade-compatible function declarations */
f32 GetCarMass(s32 car_index);
f32 GetCarWeight(s32 car_index);
f32 GetCarDwRatio(s32 car_index);
f32 GetCarEngMI(s32 car_index);
f32 GetCarBrakeBal(s32 car_index);
const COLLSIZE *GetCarCollSize(s32 car_model);
f32 GetTransRatio(s32 car_index, s32 gear);
f32 GetTorqueScale(s32 car_index, s32 gear);
s16 LookupTorque(const s16 *torquecp, f32 throttle, f32 rpm, s16 rpmperent);
s32 CheckAutoShift(s32 car_index, f32 angvel, s32 current_gear);

/* ========================================================================
 * Arcade MDrive Interface (mdrive.c / resurrect.c)
 * ======================================================================== */

/* MDrive modes */
#define Initialize      0       /* Initial car setup at race start */
#define Do_it           1       /* Resurrection/respawn mode */

/* Default crash threshold */
#define DEFCRASHTHRESH  0x7fff

/* Maximum active cars in race */
#define MAX_LINKS       8

/* Car type identifiers - guarded to avoid conflicts with drone.h */
#ifndef HUMAN
#define HUMAN           1       /* Human player */
#endif
#ifndef DRONE
#define DRONE           2       /* AI-controlled drone */
#endif

/* MDrive interface functions */
void Init_MDrive(s16 mode, s16 drone_index);
void Update_MDrive(void);
void End_MDrive(s16 drone_index);

/* Model control */
void init_cars(void);
void init_model_task(void);
void model_iteration(void);
void modelstop(void);
void multigo(s16 drone_index);

/* Communication and data update */
void multicomm(void);
void communication(void);
void update_game_data(void);
void update_player_model(s32 curtime);
void update_drone_models(s32 curtime);
void update_model(s32 slot, s32 curtime);
void update_link_cars(void);

/* Control input functions */
void ReadGasAndBrake(void);
s32 ReadGear(s32 *gear_out);
void PutModelInfo(void *m);

/* Crash and resurrection */
void CheckCrash(s16 drone_index);
void check_if_finished_resurrecting(void);
void set_resurrection_data(s16 mode, void *m);
void resurrect_car(void *m);
s16 are_all_cars_stopped(void);

/* Camera functions */
void SetMCamera(s16 mode);
void CheckCameraView(void);

#endif /* CAR_H */
