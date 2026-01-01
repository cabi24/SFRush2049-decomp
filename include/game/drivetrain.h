/**
 * drivetrain.h - Drivetrain simulation for Rush 2049 N64
 *
 * Based on arcade drivetra.c - handles engine, transmission, clutch,
 * and differential torque distribution.
 */

#ifndef DRIVETRAIN_H
#define DRIVETRAIN_H

#include "types.h"
#include "game/car.h"

/* Gear definitions */
#define GEAR_REVERSE    -1
#define GEAR_NEUTRAL    0
#define MIN_GEAR        1           /* Minimum forward gear */

/* Arcade-compatible gear aliases */
#define NEUTRALGEAR     GEAR_NEUTRAL
#define REVERSEGEAR     GEAR_REVERSE
#define MINGEAR         MIN_GEAR

/* Transmission loss for auto not in top gear (torque converter) */
#define AUTO_TRANS_LOSS 0.925f
#define AUTOLOSS        AUTO_TRANS_LOSS     /* Arcade alias */

/* RPM conversion constants */
#define RPM_TO_RDPS     (2.0f * 3.141592654f / 60.0f)   /* RPM to rad/sec */
#define RDPS_TO_RPM     (60.0f / (2.0f * 3.141592654f)) /* rad/sec to RPM */
#define rpmtordps       RPM_TO_RDPS         /* Arcade alias */
#define rdpstorpm       RDPS_TO_RPM         /* Arcade alias */

/* Clutch parameters */
#define CLUTCH_FRICTION_POINT   0.8f    /* Friction engagement point */
#define CLUTCH_MIN_RPM          1500.0f /* Centrifugal clutch engage RPM */
#define CLUTCH_MAX_RPM          3000.0f /* Full clutch engagement RPM */
#define CLUTCH_SLIP_DELTA       100.0f  /* RPM delta for clutch slip detection */

/* Drivetrain state (extends CarState) */
typedef struct DrivetrainState {
    /* Engine state */
    f32     engangvel;          /* Engine angular velocity (rad/sec) */
    f32     engtorque;          /* Engine output torque */
    f32     enginvmi;           /* Engine inverse moment of inertia */

    /* Clutch state */
    f32     clutchangvel;       /* Clutch angular velocity */
    f32     clutchtorque;       /* Torque through clutch */
    f32     clutchmaxt;         /* Max clutch torque capacity */
    f32     clutch;             /* Clutch pedal position (0=engaged, 1=disengaged) */

    /* Transmission state */
    s32     gear;               /* Current gear (-1=R, 0=N, 1-4=forward) */
    s32     commandgear;        /* Commanded gear (for manual) */
    s32     topgear;            /* Top forward gear */
    s32     autotrans;          /* Using automatic transmission */
    f32     transratio;         /* Current transmission ratio */
    f32     totalratio;         /* Total ratio (trans * diff) */

    /* Differential state */
    f32     dwangvel;           /* Driveshaft angular velocity */
    f32     dwratio;            /* Differential ratio */
    f32     dwtorque;           /* Driveshaft torque */
    f32     dwinvmi;            /* Differential inverse MOI */
    f32     efdwinvmi;          /* Effective diff inverse MOI */

    /* Shift timing */
    f32     shifttime;          /* Time of last shift */
    f32     thetime;            /* Current game time */
    f32     upshiftangvel;      /* Auto upshift threshold */
    f32     downshiftangvel;    /* Auto downshift threshold */

    /* Torque curves */
    const s16   *torquecurve;   /* Pavement torque curve */
    const s16   *dirttorquecurve; /* Dirt torque curve */
    f32     rpmperent;          /* RPM per torque curve entry */

    /* Torque scaling */
    f32     fgtorquescale;      /* First gear torque scale */
    f32     sgtorquescale;      /* Second gear torque scale */
    f32     torquescale;        /* Gear 3+ torque scale */
    f32     dirttorquescale;    /* Off-road torque scale */

    /* Transmission gear array */
    f32     transarray[NUM_GEARS]; /* Gear ratios (index = gear+1) */

    /* Control inputs */
    f32     throttle;           /* Throttle position (0-1) */
    s32     ignition;           /* Ignition on */
    s32     startermotor;       /* Starter motor engaged */

    /* Surface flags */
    u8      roadcode[4];        /* Road surface per tire */

    /* Tire interface (torque output to tires) */
    f32     torque[4];          /* Torque per tire */
    f32     tireforce_z[4];     /* Tire normal force (for diff split) */

    /* Flags */
    s32     magicdif;           /* Magic load-sensing differential */

    /* Gauge outputs (dashboard) */
    f32     amperes;            /* Ammeter reading */
    f32     oilpressure;        /* Oil pressure */
    f32     enginetemp;         /* Engine temperature */
    f32     magvel;             /* Magnitude of velocity (ft/sec) */

    /* Time step */
    f32     dt;                 /* Physics time step */
} DrivetrainState;

/* Initialization */
void drivetrain_init(DrivetrainState *dt, const Car *cardef);
void drivetrain_reset(DrivetrainState *dt);

/* Main drivetrain update (call once per physics frame) */
void drivetrain_update(DrivetrainState *dt);

/* Sub-systems */
void drivetrain_engine(DrivetrainState *dt);
void drivetrain_transmission(DrivetrainState *dt);
void drivetrain_clutch(DrivetrainState *dt);
void drivetrain_differential(DrivetrainState *dt);

/* Automatic transmission */
void drivetrain_autoshift(DrivetrainState *dt);
void drivetrain_find_best_gear(DrivetrainState *dt, f32 upshift_av, f32 downshift_av);
void drivetrain_upshift(DrivetrainState *dt);
void drivetrain_downshift(DrivetrainState *dt);

/* Torque calculation */
s16 drivetrain_engine_torque(
    DrivetrainState *dt,
    s16 rpm,
    s16 throttle,
    s16 ignition,
    s16 starter,
    const s16 *torquecurve
);

/* Interpolation helper */
s16 drivetrain_interp(s32 a, s32 b, s32 rem, s32 total);

/* Manual transmission controls */
void drivetrain_shift_up(DrivetrainState *dt);
void drivetrain_shift_down(DrivetrainState *dt);
void drivetrain_set_gear(DrivetrainState *dt, s32 gear);

/* Queries */
f32 drivetrain_get_rpm(DrivetrainState *dt);
f32 drivetrain_get_wheel_rpm(DrivetrainState *dt);
s32 drivetrain_is_clutch_slipping(DrivetrainState *dt);

/* ========================================================================
 * Arcade-compatible function aliases (drivetra.c)
 * ======================================================================== */

/* Core drivetrain functions */
void whatslips(DrivetrainState *m);         /* Clutch slip calculation */
void autoshift(DrivetrainState *m);         /* Automatic shifting */
void transmission(DrivetrainState *m);      /* Transmission ratio */
void engine(DrivetrainState *m);            /* Engine torque */

/* Gear shifting */
void find_best_gear(DrivetrainState *m, f32 usang, f32 dsang);
void upshift(DrivetrainState *m);
void downshift(DrivetrainState *m);

/* Helpers */
s16 enginetorque(DrivetrainState *m, s16 rpm, s16 throttle, s16 ignition,
                 s16 start, const s16 *torquecurve);
s16 interp(s32 a, s32 b, s32 rem, s32 total);

/* ========================================================================
 * MODELDAT pointer-based arcade functions (drivetra.c)
 * These match the arcade function signatures exactly using MODELDAT (CarPhysics)
 * ======================================================================== */

/* Forward declare MODELDAT if physics.h not included */
#ifndef PHYSICS_H
struct CarPhysics;
typedef struct CarPhysics MODELDAT;
#endif

/**
 * drivetrain_m - Main drivetrain update function (arcade)
 * Based on arcade: drivetra.c:drivetrain()
 *
 * Calls autoshift, engine, transmission, whatslips in sequence.
 * Distributes driveshaft torque to rear wheels via differential.
 */
void drivetrain_m(MODELDAT *m);

/**
 * whatslips_m - Clutch slip calculation (MODELDAT version)
 * Based on arcade: drivetra.c:whatslips()
 */
void whatslips_m(MODELDAT *m);

/**
 * autoshift_m - Automatic transmission shift logic (MODELDAT version)
 * Based on arcade: drivetra.c:autoshift()
 */
void autoshift_m(MODELDAT *m);

/**
 * transmission_m - Transmission ratio calculation (MODELDAT version)
 * Based on arcade: drivetra.c:transmission()
 */
void transmission_m(MODELDAT *m);

/**
 * engine_m - Engine torque calculation (MODELDAT version)
 * Based on arcade: drivetra.c:engine()
 */
void engine_m(MODELDAT *m);

/**
 * find_best_gear_m - Find optimal gear for current speed (MODELDAT version)
 * Based on arcade: drivetra.c:find_best_gear()
 */
void find_best_gear_m(MODELDAT *m, f32 usang, f32 dsang);

/**
 * upshift_m - Shift up one gear (MODELDAT version)
 * Based on arcade: drivetra.c:upshift()
 */
void upshift_m(MODELDAT *m);

/**
 * downshift_m - Shift down one gear (MODELDAT version)
 * Based on arcade: drivetra.c:downshift()
 */
void downshift_m(MODELDAT *m);

/**
 * enginetorque_m - Engine torque curve lookup (MODELDAT version)
 * Based on arcade: drivetra.c:enginetorque()
 */
s16 enginetorque_m(MODELDAT *m, s16 rpm, s16 throttle, s16 ignition,
                   s16 start, const s16 *torquecurve);

#endif /* DRIVETRAIN_H */
