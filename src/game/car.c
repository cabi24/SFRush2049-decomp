/**
 * car.c - Car physics parameters for Rush 2049 N64
 *
 * Based on arcade cars.c - defines torque curves and car configurations.
 * Copyright 1996 Time Warner Interactive / 1999 Midway Games
 */

#include "game/car.h"

/* ========================================================================
 * TORQUE CURVES
 *
 * Each row is a throttle opening (0% to 100% in ~11% increments)
 * Each column is RPM (0 to 11000 in 1000 RPM increments)
 * Values are torque in ft-lbs, scaled by torquescale
 *
 * BHP = torque * RPM * 6.28 * torquescale / 33000
 * ======================================================================== */

/* Standard torque curve - balanced performance */
const TorqueCurve gStdTorqueCurve = {
    {  75, -10, -30, -50, -70, -80, -80, -80, -80, -80, -80, -80 },
    {  75, 100,   1, -12, -29, -41, -49, -60, -71, -76, -78, -80 },
    {  75, 150,  32,  26,  12,  -2, -18, -40, -62, -71, -76, -80 },
    {  75, 150,  63,  63,  53,  37,  13, -20, -53, -67, -73, -80 },
    {  75, 150,  94, 101,  94,  76,  44,   0, -44, -62, -71, -80 },
    {  75, 175, 126, 139, 136, 114,  76,  20, -36, -58, -69, -80 },
    {  75, 175, 157, 177, 177, 153, 107,  40, -27, -53, -67, -80 },
    {  75, 175, 188, 214, 218, 192, 138,  60, -18, -49, -64, -80 },
    {  75, 177, 219, 252, 259, 231, 169,  80,  -9, -44, -62, -80 },
    {  75, 200, 250, 290, 300, 270, 200, 100,   0, -40, -60, -80 }
};

/* Rush torque curve - more aggressive mid-range */
const TorqueCurve gRushTorqueCurve = {
    { 100, -12, -33, -51, -61, -54, -71, -75, -75, -75, -75, -75 },
    { 100, 112,  62,   3, -30, -51, -65, -69, -71, -73, -75, -75 },
    { 100, 114, 113, 112, 104,  70,  87,  32, -32, -62, -70, -75 },
    { 100, 116, 126, 131, 136, 136, 131,  90,  -1, -36, -69, -75 },
    { 100, 119, 137, 151, 161, 170, 175, 160,  90,   0, -50, -75 },
    { 100, 127, 156, 178, 196, 215, 223, 225, 205,  75, -32, -75 },
    { 100, 133, 163, 190, 212, 230, 245, 256, 243, 108, -23, -75 },
    { 100, 137, 175, 206, 230, 250, 265, 280, 282, 175, -10, -75 },
    { 100, 146, 186, 222, 248, 275, 293, 305, 310, 251,   4, -69 },
    { 100, 155, 202, 240, 275, 300, 320, 327, 328, 312,  49, -65 }
};

/* Dirt/off-road torque curve - flatter response */
const TorqueCurve gDirtTorqueCurve = {
    {  75, -10, -30, -30, -40, -40, -40, -40, -40, -40, -40, -40 },
    {  75, 100,   1, -12, -29, -41, -40, -40, -40, -40, -40, -40 },
    {  75, 150,  60,  26,  12,  -2, -18, -40, -40, -40, -40, -40 },
    {  75, 150,  94,  63,  53,  37,  13, -10, -40, -40, -40, -40 },
    {  75, 150,  94, 101,  94,  76,  44,   0, -40, -40, -40, -40 },
    {  75, 150,  94, 101,  94,  76,  44,   0, -40, -40, -40, -40 },
    {  75, 150,  94, 101, 150,  76,  44,   0, -40, -40, -40, -40 },
    {  75, 150, 150, 150, 150,  76,  44,   0, -40, -40, -40, -40 },
    {  75, 150, 150, 150, 150,  76,  44,   0, -40, -40, -40, -40 },
    {  75, 150, 150, 150, 150,  76,  44,   0, -40, -40, -40, -40 }
};

/* ========================================================================
 * DEFAULT TIRE PARAMETERS
 * ======================================================================== */

/* Tire templates as macros for use in static initializers (C89 compatible) */
#define FRONT_TIRE_INIT { \
    1.0f,           /* tradius */ \
    340 * 12,       /* springK */ \
    200,            /* rubdamp */ \
    16000,          /* PaveCstiff */ \
    1.15f,          /* PaveCfmax */ \
    16000,          /* Cstiff */ \
    1.15f,          /* Cfmax */ \
    0.1f,           /* invmi */ \
    /* Runtime state initialized to 0 */ \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 \
}

#define REAR_TIRE_INIT { \
    1.0f,           /* tradius */ \
    340 * 12,       /* springK */ \
    200,            /* rubdamp */ \
    50000,          /* PaveCstiff */ \
    1.3f,           /* PaveCfmax */ \
    50000,          /* Cstiff */ \
    1.3f,           /* Cfmax */ \
    0.1f,           /* invmi */ \
    /* Runtime state initialized to 0 */ \
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 \
}

/* Static tire templates for reference */
static const TireDes sFrontTire = FRONT_TIRE_INIT;
static const TireDes sRearTire = REAR_TIRE_INIT;

/* ========================================================================
 * CAR DEFINITIONS
 *
 * 8 cars for Rush 2049:
 * 0: Rocket   - High speed, low handling
 * 1: Venom    - Balanced
 * 2: Crusher  - Heavy, high grip
 * 3: Phantom  - Light, agile
 * 4: Wraith   - All-around
 * 5: Eliminator - Expert, high power
 * 6: Banshee  - Stunt mode specialty
 * 7: Secret   - Unlockable
 * ======================================================================== */

const char *gCarNames[NUM_CAR_TYPES] = {
    "Rocket",
    "Venom",
    "Crusher",
    "Phantom",
    "Wraith",
    "Eliminator",
    "Banshee",
    "Secret"
};

const Car gCarDefs[NUM_CAR_TYPES] = {
    /* Car 0: Rocket - High speed sports car */
    {
        "Rocket",
        3200.0f / GRAVITY_ACCEL,    /* mass (slugs) - lighter */
        { 1400.0f, 2400.0f, 2400.0f },  /* I - moments of inertia */

        /* Suspension - stiff for speed */
        { -4200, -4200, -2800, -2800 },  /* springrate */
        -1200,                          /* farspringrate */
        0,                              /* rarspringrate */
        { -550, -550, -350, -350 },     /* cdamping */
        { -550, -550, -350, -350 },     /* rdamping */

        /* Steering */
        14.0f,          /* steerratio */
        4.0f,           /* swtpg */
        4, 0x10, 0x100, /* damping/friction */
        0,              /* pad */

        /* Aero */
        0.0082f,        /* srefpcybo2 - low drag */
        32.0f,          /* rollresist */

        /* Tires */
        { FRONT_TIRE_INIT, FRONT_TIRE_INIT, REAR_TIRE_INIT, REAR_TIRE_INIT },

        /* Geometry - shorter wheelbase */
        {
            { 4.2f,  2.5f, 1.4f },   /* RF */
            { 4.2f, -2.5f, 1.4f },   /* LF */
            { -5.2f,  2.5f, 1.5f },  /* RR */
            { -5.2f, -2.5f, 1.5f }   /* LR */
        },

        0.68f,          /* brakebal */
        0.28f,          /* engmi */
        3.5f,           /* dwratio */
        850.0f,         /* clutchmaxt */
        -2.0f,          /* viewheight */

        /* Collision */
        { 2.8f, 5.8f, 2.6f, -2.6f, 1.8f, 0.0f },

        0, 0, { 0, 0 },     /* flags */

        /* Torque */
        1.1f, 1.05f, 1.0f, 0.9f,    /* torquescales */

        /* Transmission - close ratios */
        { -2.4f, 0.0f, 1.9f, 1.5f, 1.2f, 1.0f },
        &gRushTorqueCurve[0][0],
        &gDirtTorqueCurve[0][0],
        1000, 4,
        5750 * RPM_TO_RDPS,
        3500 * RPM_TO_RDPS
    },

    /* Car 1: Venom - Balanced performance */
    {
        "Venom",
        3400.0f / GRAVITY_ACCEL,
        { 1500.0f, 2500.0f, 2500.0f },

        { -4000, -4000, -2500, -2500 },
        -1000, 0,
        { -500, -500, -300, -300 },
        { -500, -500, -300, -300 },

        14.0f, 4.0f, 4, 0x10, 0x100, 0,
        0.0087f, 36.0f,

        { FRONT_TIRE_INIT, FRONT_TIRE_INIT, REAR_TIRE_INIT, REAR_TIRE_INIT },

        {
            { 4.5f,  2.75f, 1.5f },
            { 4.5f, -2.75f, 1.5f },
            { -5.5f,  2.75f, 1.5f },
            { -5.5f, -2.75f, 1.5f }
        },

        0.65f, 0.3f, 3.3f, 800.0f, -2.0f,
        { 3.0f, 6.0f, 2.8f, -2.8f, 2.0f, 0.0f },
        0, 0, { 0, 0 },

        1.0f, 1.0f, 1.0f, 1.0f,
        { -2.4f, 0.0f, 1.8f, 1.45f, 1.2f, 1.0f },
        &gRushTorqueCurve[0][0],
        &gDirtTorqueCurve[0][0],
        1000, 4,
        5750 * RPM_TO_RDPS,
        3500 * RPM_TO_RDPS
    },

    /* Car 2: Crusher - Heavy muscle car */
    {
        "Crusher",
        4000.0f / GRAVITY_ACCEL,
        { 1700.0f, 2800.0f, 2800.0f },

        { -3600, -3600, -2200, -2200 },
        -800, -200,
        { -450, -450, -280, -280 },
        { -450, -450, -280, -280 },

        16.0f, 3.0f, 4, 0x10, 0x80, 0,
        0.0095f, 40.0f,

        { FRONT_TIRE_INIT, FRONT_TIRE_INIT, REAR_TIRE_INIT, REAR_TIRE_INIT },

        {
            { 4.8f,  2.9f, 1.6f },
            { 4.8f, -2.9f, 1.6f },
            { -5.8f,  2.9f, 1.6f },
            { -5.8f, -2.9f, 1.6f }
        },

        0.60f, 0.35f, 3.0f, 900.0f, -2.1f,
        { 3.2f, 6.5f, 3.0f, -3.0f, 2.2f, 0.0f },
        0, 1, { 0, 0 },     /* magicdif enabled */

        1.15f, 1.1f, 1.05f, 1.0f,
        { -2.2f, 0.0f, 2.0f, 1.6f, 1.3f, 1.0f },
        &gRushTorqueCurve[0][0],
        &gDirtTorqueCurve[0][0],
        1000, 4,
        5500 * RPM_TO_RDPS,
        3200 * RPM_TO_RDPS
    },

    /* Car 3: Phantom - Light and agile */
    {
        "Phantom",
        2800.0f / GRAVITY_ACCEL,
        { 1200.0f, 2100.0f, 2100.0f },

        { -4400, -4400, -2800, -2800 },
        -1400, 0,
        { -580, -580, -380, -380 },
        { -580, -580, -380, -380 },

        12.0f, 5.0f, 4, 0x10, 0x100, 0,
        0.0078f, 28.0f,

        { FRONT_TIRE_INIT, FRONT_TIRE_INIT, REAR_TIRE_INIT, REAR_TIRE_INIT },

        {
            { 4.0f,  2.4f, 1.3f },
            { 4.0f, -2.4f, 1.3f },
            { -5.0f,  2.4f, 1.4f },
            { -5.0f, -2.4f, 1.4f }
        },

        0.70f, 0.25f, 3.6f, 750.0f, -1.9f,
        { 2.6f, 5.4f, 2.4f, -2.4f, 1.6f, 0.0f },
        0, 0, { 0, 0 },

        0.95f, 0.95f, 0.95f, 0.95f,
        { -2.6f, 0.0f, 2.0f, 1.55f, 1.25f, 1.0f },
        &gRushTorqueCurve[0][0],
        &gDirtTorqueCurve[0][0],
        1000, 4,
        6000 * RPM_TO_RDPS,
        3800 * RPM_TO_RDPS
    },

    /* Car 4: Wraith - All-around performer */
    {
        "Wraith",
        3300.0f / GRAVITY_ACCEL,
        { 1450.0f, 2450.0f, 2450.0f },

        { -4100, -4100, -2600, -2600 },
        -1100, 0,
        { -520, -520, -320, -320 },
        { -520, -520, -320, -320 },

        14.0f, 4.0f, 4, 0x10, 0x100, 0,
        0.0085f, 34.0f,

        { FRONT_TIRE_INIT, FRONT_TIRE_INIT, REAR_TIRE_INIT, REAR_TIRE_INIT },

        {
            { 4.4f,  2.7f, 1.45f },
            { 4.4f, -2.7f, 1.45f },
            { -5.4f,  2.7f, 1.5f },
            { -5.4f, -2.7f, 1.5f }
        },

        0.66f, 0.29f, 3.4f, 820.0f, -2.0f,
        { 2.9f, 5.9f, 2.7f, -2.7f, 1.9f, 0.0f },
        0, 0, { 0, 0 },

        1.02f, 1.0f, 1.0f, 1.0f,
        { -2.4f, 0.0f, 1.85f, 1.48f, 1.22f, 1.0f },
        &gRushTorqueCurve[0][0],
        &gDirtTorqueCurve[0][0],
        1000, 4,
        5800 * RPM_TO_RDPS,
        3600 * RPM_TO_RDPS
    },

    /* Car 5: Eliminator - Expert high power */
    {
        "Eliminator",
        3500.0f / GRAVITY_ACCEL,
        { 1550.0f, 2550.0f, 2550.0f },

        { -4500, -4500, -3000, -3000 },
        -1500, -200,
        { -600, -600, -400, -400 },
        { -600, -600, -400, -400 },

        13.0f, 4.5f, 4, 0x10, 0x100, 0,
        0.0080f, 35.0f,

        { FRONT_TIRE_INIT, FRONT_TIRE_INIT, REAR_TIRE_INIT, REAR_TIRE_INIT },

        {
            { 4.3f,  2.65f, 1.4f },
            { 4.3f, -2.65f, 1.4f },
            { -5.3f,  2.65f, 1.45f },
            { -5.3f, -2.65f, 1.45f }
        },

        0.67f, 0.32f, 3.2f, 880.0f, -2.0f,
        { 2.85f, 5.85f, 2.65f, -2.65f, 1.85f, 0.0f },
        0, 1, { 0, 0 },

        1.2f, 1.15f, 1.1f, 1.0f,
        { -2.3f, 0.0f, 1.75f, 1.4f, 1.15f, 0.95f },
        &gRushTorqueCurve[0][0],
        &gDirtTorqueCurve[0][0],
        1000, 4,
        6200 * RPM_TO_RDPS,
        4000 * RPM_TO_RDPS
    },

    /* Car 6: Banshee - Stunt mode specialty */
    {
        "Banshee",
        3000.0f / GRAVITY_ACCEL,
        { 1300.0f, 2200.0f, 2200.0f },

        { -4600, -4600, -3000, -3000 },
        -1600, 0,
        { -620, -620, -420, -420 },
        { -620, -620, -420, -420 },

        11.0f, 5.5f, 4, 0x10, 0x100, 0,
        0.0075f, 30.0f,

        { FRONT_TIRE_INIT, FRONT_TIRE_INIT, REAR_TIRE_INIT, REAR_TIRE_INIT },

        {
            { 4.1f,  2.5f, 1.35f },
            { 4.1f, -2.5f, 1.35f },
            { -5.1f,  2.5f, 1.4f },
            { -5.1f, -2.5f, 1.4f }
        },

        0.68f, 0.26f, 3.7f, 780.0f, -1.85f,
        { 2.7f, 5.5f, 2.5f, -2.5f, 1.7f, 0.0f },
        0, 0, { 0, 0 },

        1.0f, 1.0f, 1.0f, 1.0f,
        { -2.5f, 0.0f, 1.95f, 1.52f, 1.22f, 1.0f },
        &gRushTorqueCurve[0][0],
        &gDirtTorqueCurve[0][0],
        1000, 4,
        6100 * RPM_TO_RDPS,
        3900 * RPM_TO_RDPS
    },

    /* Car 7: Secret - Unlockable */
    {
        "Secret",
        3100.0f / GRAVITY_ACCEL,
        { 1350.0f, 2300.0f, 2300.0f },

        { -4300, -4300, -2900, -2900 },
        -1300, -100,
        { -560, -560, -360, -360 },
        { -560, -560, -360, -360 },

        13.0f, 4.5f, 4, 0x10, 0x100, 0,
        0.0076f, 31.0f,

        { FRONT_TIRE_INIT, FRONT_TIRE_INIT, REAR_TIRE_INIT, REAR_TIRE_INIT },

        {
            { 4.25f,  2.55f, 1.38f },
            { 4.25f, -2.55f, 1.38f },
            { -5.25f,  2.55f, 1.42f },
            { -5.25f, -2.55f, 1.42f }
        },

        0.67f, 0.28f, 3.55f, 830.0f, -1.95f,
        { 2.75f, 5.7f, 2.55f, -2.55f, 1.75f, 0.0f },
        0, 1, { 0, 0 },

        1.15f, 1.1f, 1.05f, 1.0f,
        { -2.35f, 0.0f, 1.82f, 1.46f, 1.18f, 0.98f },
        &gRushTorqueCurve[0][0],
        &gDirtTorqueCurve[0][0],
        1000, 4,
        6000 * RPM_TO_RDPS,
        3700 * RPM_TO_RDPS
    }
};

/* ========================================================================
 * FUNCTIONS
 * ======================================================================== */

/**
 * Initialize car physics system
 */
void car_physics_init(void) {
    /* Nothing to init - all data is const ROM */
}

/**
 * Get engine torque from curve
 * throttle: 0.0 to 1.0
 * rpm: engine RPM
 */
s16 car_get_torque(const Car *car, f32 throttle, f32 rpm) {
    const s16 *curve = car->torquecp;
    s32 throttle_idx, rpm_idx;
    s32 t0, t1, r0, r1;
    s32 val00, val01, val10, val11;
    f32 tf, rf;

    /* Clamp inputs */
    if (throttle < 0.0f) throttle = 0.0f;
    if (throttle > 1.0f) throttle = 1.0f;
    if (rpm < 0.0f) rpm = 0.0f;

    /* Calculate indices */
    throttle_idx = (s32)(throttle * (TORQUE_THROTTLE_STEPS - 1));
    rpm_idx = (s32)(rpm / car->rpmperent);

    /* Clamp indices */
    if (throttle_idx >= TORQUE_THROTTLE_STEPS - 1) {
        throttle_idx = TORQUE_THROTTLE_STEPS - 2;
    }
    if (rpm_idx >= TORQUE_RPM_STEPS - 1) {
        rpm_idx = TORQUE_RPM_STEPS - 2;
    }

    /* Get interpolation factors */
    tf = (throttle * (TORQUE_THROTTLE_STEPS - 1)) - throttle_idx;
    rf = (rpm / car->rpmperent) - rpm_idx;

    /* Get four corner values */
    t0 = throttle_idx;
    t1 = throttle_idx + 1;
    r0 = rpm_idx;
    r1 = rpm_idx + 1;

    val00 = curve[t0 * TORQUE_RPM_STEPS + r0];
    val01 = curve[t0 * TORQUE_RPM_STEPS + r1];
    val10 = curve[t1 * TORQUE_RPM_STEPS + r0];
    val11 = curve[t1 * TORQUE_RPM_STEPS + r1];

    /* Bilinear interpolation */
    {
        f32 v0 = val00 + (val01 - val00) * rf;
        f32 v1 = val10 + (val11 - val10) * rf;
        return (s16)(v0 + (v1 - v0) * tf);
    }
}

/**
 * Get dirt torque from curve
 */
s16 car_get_dirt_torque(const Car *car, f32 throttle, f32 rpm) {
    const s16 *curve = car->dirttorquecp;
    s32 throttle_idx, rpm_idx;

    if (throttle < 0.0f) throttle = 0.0f;
    if (throttle > 1.0f) throttle = 1.0f;
    if (rpm < 0.0f) rpm = 0.0f;

    throttle_idx = (s32)(throttle * (TORQUE_THROTTLE_STEPS - 1));
    rpm_idx = (s32)(rpm / car->rpmperent);

    if (throttle_idx >= TORQUE_THROTTLE_STEPS) {
        throttle_idx = TORQUE_THROTTLE_STEPS - 1;
    }
    if (rpm_idx >= TORQUE_RPM_STEPS) {
        rpm_idx = TORQUE_RPM_STEPS - 1;
    }

    return curve[throttle_idx * TORQUE_RPM_STEPS + rpm_idx];
}

/**
 * Check if should upshift
 */
s32 car_should_upshift(const Car *car, f32 angvel) {
    return (angvel >= car->upshiftangvel);
}

/**
 * Check if should downshift
 */
s32 car_should_downshift(const Car *car, f32 angvel) {
    return (angvel <= car->downshiftangvel);
}

/**
 * Get gear ratio
 */
f32 car_get_gear_ratio(const Car *car, s32 gear) {
    if (gear < -1) gear = -1;
    if (gear > MAX_GEAR) gear = MAX_GEAR;
    return car->transarray[gear + 1];  /* +1 because index 0 is reverse */
}

/**
 * Get car weight in pounds
 */
f32 car_get_weight(const Car *car) {
    return car->mass * GRAVITY_ACCEL;
}

/**
 * Get wheelbase (front to rear axle distance)
 */
f32 car_get_wheelbase(const Car *car) {
    f32 front = car->TIRER[TIRE_RF][0];
    f32 rear = car->TIRER[TIRE_RR][0];
    return front - rear;  /* front is positive, rear is negative */
}

/**
 * Get track width (left to right tire distance)
 */
f32 car_get_track_width(const Car *car) {
    f32 right = car->TIRER[TIRE_RF][1];
    f32 left = car->TIRER[TIRE_LF][1];
    return right - left;
}

/* ========================================================================
 * CAR STATE FUNCTIONS
 * ======================================================================== */

/**
 * Initialize car state
 */
void car_state_init(CarState *state, const Car *cardef) {
    s32 i;

    state->cardef = cardef;

    /* Zero position/velocity */
    for (i = 0; i < 3; i++) {
        state->RWR[i] = 0.0f;
        state->RWV[i] = 0.0f;
        state->V[i] = 0.0f;
        state->W[i] = 0.0f;
    }

    /* Identity orientation */
    for (i = 0; i < 3; i++) {
        state->UV[i][0] = (i == 0) ? 1.0f : 0.0f;
        state->UV[i][1] = (i == 1) ? 1.0f : 0.0f;
        state->UV[i][2] = (i == 2) ? 1.0f : 0.0f;
    }

    /* Engine */
    state->rpm = 1000.0f;
    state->throttle = 0.0f;
    state->torque = 0.0f;
    state->gear = 1;
    state->autotrans = 1;

    /* Controls */
    state->steerangle = 0.0f;
    state->brake = 0.0f;
    state->clutch = 0.0f;

    /* Tires */
    for (i = 0; i < NUM_TIRES; i++) {
        state->tireW[i] = 0.0f;
        state->suscomp[i] = 0.0f;
    }

    /* Status */
    state->mph = 0.0f;
    state->distance = 0.0f;
    state->flags = 0;
}

/**
 * Reset car state to spawn position
 */
void car_state_reset(CarState *state) {
    /* Keep cardef, reset everything else */
    const Car *cardef = state->cardef;
    car_state_init(state, cardef);
}

/**
 * Update car state (placeholder - actual physics in physics.c)
 */
void car_state_update(CarState *state, f32 dt) {
    /* Update MPH from velocity */
    f32 speed_sq = state->RWV[0] * state->RWV[0] +
                   state->RWV[1] * state->RWV[1] +
                   state->RWV[2] * state->RWV[2];
    f32 speed_fps = sqrtf(speed_sq);
    state->mph = speed_fps * (3600.0f / 5280.0f);  /* ft/s to mph */

    /* Update distance */
    state->distance += speed_fps * dt;
}

/**
 * Update engine state
 */
void car_update_engine(CarState *state, f32 dt) {
    const Car *car = state->cardef;
    f32 scale;

    /* Get torque from curve */
    state->torque = car_get_torque(car, state->throttle, state->rpm);

    /* Apply gear-dependent scaling */
    if (state->gear == 1) {
        scale = car->fgtorquescale;
    } else if (state->gear == 2) {
        scale = car->sgtorquescale;
    } else {
        scale = car->torquescale;
    }

    state->torque = (f32)state->torque * scale;
}

/**
 * Update transmission (auto shift)
 */
void car_update_transmission(CarState *state) {
    const Car *car = state->cardef;
    f32 angvel;

    if (!state->autotrans) {
        return;
    }

    /* Convert RPM to angular velocity */
    angvel = state->rpm * RPM_TO_RDPS;

    /* Check for shifts */
    if (car_should_upshift(car, angvel) && state->gear < car->topgear) {
        state->gear++;
    } else if (car_should_downshift(car, angvel) && state->gear > MIN_GEAR) {
        state->gear--;
    }
}

/**
 * Update steering angle from wheel input
 */
void car_update_steering(CarState *state, f32 wheel_input) {
    const Car *car = state->cardef;

    /* wheel_input is -1.0 to 1.0 */
    /* Convert through steering ratio to tire angle */
    state->steerangle = (wheel_input * DEG_TO_RAD * 180.0f) / car->steerratio;
}

/**
 * Update brake state
 */
void car_update_brakes(CarState *state, f32 brake_input) {
    state->brake = brake_input;
    if (brake_input > 0.0f) {
        state->flags |= CAR_FLAG_BRAKING;
    } else {
        state->flags &= ~CAR_FLAG_BRAKING;
    }
}

/* ========================================================================
 * ARCADE-COMPATIBLE TORQUE CURVES
 *
 * These match the arcade source format exactly.
 * ======================================================================== */

const s16 stdtorquecurve[10][12] = {
    {  75, -10, -30, -50, -70, -80, -80, -80, -80, -80, -80, -80 },
    {  75, 100,   1, -12, -29, -41, -49, -60, -71, -76, -78, -80 },
    {  75, 150,  32,  26,  12,  -2, -18, -40, -62, -71, -76, -80 },
    {  75, 150,  63,  63,  53,  37,  13, -20, -53, -67, -73, -80 },
    {  75, 150,  94, 101,  94,  76,  44,   0, -44, -62, -71, -80 },
    {  75, 175, 126, 139, 136, 114,  76,  20, -36, -58, -69, -80 },
    {  75, 175, 157, 177, 177, 153, 107,  40, -27, -53, -67, -80 },
    {  75, 175, 188, 214, 218, 192, 138,  60, -18, -49, -64, -80 },
    {  75, 177, 219, 252, 259, 231, 169,  80,  -9, -44, -62, -80 },
    {  75, 200, 250, 290, 300, 270, 200, 100,   0, -40, -60, -80 }
};

const s16 stddirttorquecurve[10][12] = {
    {  75, -10, -30, -30, -40, -40, -40, -40, -40, -40, -40, -40 },
    {  75, 100,   1, -12, -29, -41, -40, -40, -40, -40, -40, -40 },
    {  75, 150,  60,  26,  12,  -2, -18, -40, -40, -40, -40, -40 },
    {  75, 150,  94,  63,  53,  37,  13, -10, -40, -40, -40, -40 },
    {  75, 150,  94, 101,  94,  76,  44,   0, -40, -40, -40, -40 },
    {  75, 150,  94, 101,  94,  76,  44,   0, -40, -40, -40, -40 },
    {  75, 150,  94, 101, 150,  76,  44,   0, -40, -40, -40, -40 },
    {  75, 150, 150, 150, 150,  76,  44,   0, -40, -40, -40, -40 },
    {  75, 150, 150, 150, 150,  76,  44,   0, -40, -40, -40, -40 },
    {  75, 150, 150, 150, 150,  76,  44,   0, -40, -40, -40, -40 }
};

const s16 rushtorquecurve[10][12] = {
    { 100, -12, -33, -51, -61, -54, -71, -75, -75, -75, -75, -75 },
    { 100, 112,  62,   3, -30, -51, -65, -69, -71, -73, -75, -75 },
    { 100, 114, 113, 112, 104,  70,  87,  32, -32, -62, -70, -75 },
    { 100, 116, 126, 131, 136, 136, 131,  90,  -1, -36, -69, -75 },
    { 100, 119, 137, 151, 161, 170, 175, 160,  90,   0, -50, -75 },
    { 100, 127, 156, 178, 196, 215, 223, 225, 205,  75, -32, -75 },
    { 100, 133, 163, 190, 212, 230, 245, 256, 243, 108, -23, -75 },
    { 100, 137, 175, 206, 230, 250, 265, 280, 282, 175, -10, -75 },
    { 100, 146, 186, 222, 248, 275, 293, 305, 310, 251,   4, -69 },
    { 100, 155, 202, 240, 275, 300, 320, 327, 328, 312,  49, -65 }
};

/* Arcade collision sizes per car model */
static const COLLSIZE sCollSize[NUM_CAR_TYPES] = {
    { 5.75f, 6.50f, 3.0f, 2.0f },   /* Rocket */
    { 6.29f, 6.58f, 2.9f, 2.0f },   /* Venom */
    { 6.05f, 6.65f, 2.9f, 2.0f },   /* Crusher */
    { 6.25f, 5.75f, 3.0f, 2.0f },   /* Phantom */
    { 6.44f, 6.64f, 3.0f, 2.0f },   /* Wraith */
    { 6.25f, 6.75f, 3.0f, 2.0f },   /* Eliminator */
    { 5.75f, 6.00f, 3.0f, 2.0f },   /* Banshee */
    { 6.70f, 6.97f, 2.8f, 2.0f }    /* Secret */
};

/* Arcade-style car pointer list */
static const Car * const sCarList[NUM_CAR_TYPES] = {
    &gCarDefs[0],
    &gCarDefs[1],
    &gCarDefs[2],
    &gCarDefs[3],
    &gCarDefs[4],
    &gCarDefs[5],
    &gCarDefs[6],
    &gCarDefs[7]
};

const Car * const carlist[] = {
    &gCarDefs[0],
    &gCarDefs[1],
    &gCarDefs[2],
    &gCarDefs[3],
    &gCarDefs[4],
    &gCarDefs[5],
    &gCarDefs[6],
    &gCarDefs[7]
};

const COLLSIZE collsize[] = {
    { 5.75f, 6.50f, 3.0f, 2.0f },
    { 6.29f, 6.58f, 2.9f, 2.0f },
    { 6.05f, 6.65f, 2.9f, 2.0f },
    { 6.25f, 5.75f, 3.0f, 2.0f },
    { 6.44f, 6.64f, 3.0f, 2.0f },
    { 6.25f, 6.75f, 3.0f, 2.0f },
    { 5.75f, 6.00f, 3.0f, 2.0f },
    { 6.70f, 6.97f, 2.8f, 2.0f }
};

const s16 num_cars = NUM_CAR_TYPES;

/* ========================================================================
 * ARCADE-COMPATIBLE ACCESSOR FUNCTIONS
 * ======================================================================== */

/**
 * Get car mass in slugs
 */
f32 GetCarMass(s32 car_index) {
    if (car_index < 0 || car_index >= NUM_CAR_TYPES) {
        car_index = 0;
    }
    return gCarDefs[car_index].mass;
}

/**
 * Get car weight in pounds
 */
f32 GetCarWeight(s32 car_index) {
    return GetCarMass(car_index) * GRAVITY_ACCEL;
}

/**
 * Get differential gear ratio
 */
f32 GetCarDwRatio(s32 car_index) {
    if (car_index < 0 || car_index >= NUM_CAR_TYPES) {
        car_index = 0;
    }
    return gCarDefs[car_index].dwratio;
}

/**
 * Get engine/flywheel moment of inertia
 */
f32 GetCarEngMI(s32 car_index) {
    if (car_index < 0 || car_index >= NUM_CAR_TYPES) {
        car_index = 0;
    }
    return gCarDefs[car_index].engmi;
}

/**
 * Get brake balance (front wheel fraction)
 */
f32 GetCarBrakeBal(s32 car_index) {
    if (car_index < 0 || car_index >= NUM_CAR_TYPES) {
        car_index = 0;
    }
    return gCarDefs[car_index].brakebal;
}

/**
 * Get collision size for car model
 */
const COLLSIZE *GetCarCollSize(s32 car_model) {
    if (car_model < 0 || car_model >= NUM_CAR_TYPES) {
        car_model = 0;
    }
    return &sCollSize[car_model];
}

/**
 * Get transmission ratio for gear
 */
f32 GetTransRatio(s32 car_index, s32 gear) {
    if (car_index < 0 || car_index >= NUM_CAR_TYPES) {
        car_index = 0;
    }
    if (gear < -1) gear = -1;
    if (gear > MAX_GEAR) gear = MAX_GEAR;
    return gCarDefs[car_index].transarray[gear + 1];
}

/**
 * Get torque scale for gear
 */
f32 GetTorqueScale(s32 car_index, s32 gear) {
    const Car *car;
    if (car_index < 0 || car_index >= NUM_CAR_TYPES) {
        car_index = 0;
    }
    car = &gCarDefs[car_index];

    if (gear == 1) {
        return car->fgtorquescale;
    } else if (gear == 2) {
        return car->sgtorquescale;
    }
    return car->torquescale;
}

/**
 * Lookup torque from curve (arcade-compatible)
 * Uses bilinear interpolation like arcade version
 */
s16 LookupTorque(const s16 *torquecp, f32 throttle, f32 rpm, s16 rpmperent) {
    s32 throttle_idx, rpm_idx;
    s32 t0, t1, r0, r1;
    s32 val00, val01, val10, val11;
    f32 tf, rf;
    f32 v0, v1;

    /* Clamp inputs */
    if (throttle < 0.0f) throttle = 0.0f;
    if (throttle > 1.0f) throttle = 1.0f;
    if (rpm < 0.0f) rpm = 0.0f;

    /* Calculate indices */
    throttle_idx = (s32)(throttle * 9.0f);
    rpm_idx = (s32)(rpm / (f32)rpmperent);

    /* Clamp indices */
    if (throttle_idx >= 9) {
        throttle_idx = 8;
    }
    if (rpm_idx >= 11) {
        rpm_idx = 10;
    }

    /* Get interpolation factors */
    tf = (throttle * 9.0f) - (f32)throttle_idx;
    rf = (rpm / (f32)rpmperent) - (f32)rpm_idx;

    /* Get four corner values */
    t0 = throttle_idx;
    t1 = throttle_idx + 1;
    r0 = rpm_idx;
    r1 = rpm_idx + 1;

    val00 = torquecp[t0 * 12 + r0];
    val01 = torquecp[t0 * 12 + r1];
    val10 = torquecp[t1 * 12 + r0];
    val11 = torquecp[t1 * 12 + r1];

    /* Bilinear interpolation */
    v0 = (f32)val00 + ((f32)(val01 - val00) * rf);
    v1 = (f32)val10 + ((f32)(val11 - val10) * rf);
    return (s16)(v0 + (v1 - v0) * tf);
}

/**
 * Check if automatic shift is needed
 * Returns new gear or current gear if no shift
 */
s32 CheckAutoShift(s32 car_index, f32 angvel, s32 current_gear) {
    const Car *car;

    if (car_index < 0 || car_index >= NUM_CAR_TYPES) {
        car_index = 0;
    }
    car = &gCarDefs[car_index];

    /* Check upshift */
    if (angvel >= car->upshiftangvel && current_gear < car->topgear) {
        return current_gear + 1;
    }

    /* Check downshift */
    if (angvel <= car->downshiftangvel && current_gear > MIN_GEAR) {
        return current_gear - 1;
    }

    return current_gear;
}

/* ========================================================================
 * ARCADE MDRIVE INTERFACE (Stubs for N64)
 *
 * These functions provide the arcade-compatible interface for car control.
 * Full implementations will be added as the decompilation progresses.
 * ======================================================================== */

/* Model control globals (arcade-compatible) */
static s16 sModelRun = 0;
static s32 sLastMTime = 0;

/**
 * Initialize car model for race start or resurrection
 * mode: Initialize (0) = race start, Do_it (1) = respawn
 * drone_index: which car to initialize
 */
void Init_MDrive(s16 mode, s16 drone_index) {
    /* Reset model state */
    sModelRun = 1;

#ifdef NON_MATCHING
    /* TODO: Full arcade implementation */
    /* multiinit(mode, drone_index); */
#endif
}

/**
 * Main car update loop - called each frame
 * Handles controls, physics, and model synchronization
 */
void Update_MDrive(void) {
    /* Read control inputs */
    ReadGasAndBrake();

    /* Check resurrection state */
    check_if_finished_resurrecting();

    /* Update communication with model */
    multicomm();

    /* Update game task data */
    update_game_data();

    /* Update camera */
    CheckCameraView();
}

/**
 * End car model operation
 */
void End_MDrive(s16 drone_index) {
    sModelRun = 0;
}

/**
 * Initialize all cars in race
 */
void init_cars(void) {
#ifdef NON_MATCHING
    /* TODO: Full arcade implementation */
#endif
}

/**
 * Initialize model task
 */
void init_model_task(void) {
    sLastMTime = 0;
}

/**
 * Main model iteration - called from model task
 */
void model_iteration(void) {
    if (sModelRun == 0) {
        return;
    }

#ifdef NON_MATCHING
    /* TODO: update_player_model, update_drone_models */
#endif
}

/**
 * Stop model processing
 */
void modelstop(void) {
    sModelRun = 0;
}

/**
 * Start model for specific car
 */
void multigo(s16 drone_index) {
    sModelRun = 1;
}

/**
 * Multi-car communication update
 */
void multicomm(void) {
    communication();
}

/**
 * Communication with model processor
 */
void communication(void) {
#ifdef NON_MATCHING
    /* TODO: PutModelInfo, network sync */
#endif
}

/**
 * Update game task's version of model data
 */
void update_game_data(void) {
#ifdef NON_MATCHING
    /* TODO: Dead reckon all cars, copy to game_car */
#endif
}

/**
 * Update player's car model
 */
void update_player_model(s32 curtime) {
#ifdef NON_MATCHING
    /* TODO: sym() call, steering force */
#endif
}

/**
 * Update drone car models
 */
void update_drone_models(s32 curtime) {
#ifdef NON_MATCHING
    /* TODO: MaxPathControls, sym() for drones */
#endif
}

/**
 * Update single car model
 */
void update_model(s32 slot, s32 curtime) {
#ifdef NON_MATCHING
    /* TODO: Resurrection check, sym() call */
#endif
}

/**
 * Update linked (network) cars
 */
void update_link_cars(void) {
#ifdef NON_MATCHING
    /* TODO: ProcessPDUs, dead reckoning */
#endif
}

/**
 * Read gas and brake inputs
 */
void ReadGasAndBrake(void) {
#ifdef NON_MATCHING
    /* TODO: Read from controller */
#endif
}

/**
 * Read gear selection
 */
s32 ReadGear(s32 *gear_out) {
    *gear_out = 1;
    return 0; /* clutch value */
}

/**
 * Put model info to communication buffer
 */
void PutModelInfo(void *m) {
#ifdef NON_MATCHING
    /* TODO: Copy control inputs to model struct */
#endif
}

/**
 * Check if car has crashed and handle resurrection
 * Based on arcade resurrect.c:CheckCrash
 */
void CheckCrash(s16 drone_index) {
#ifdef NON_MATCHING
    /* TODO: Check crashflag, set resurrection data */
    /* if (game_car[drone_index].crashflag) {
     *     if (game_car[drone_index].crashtime == 0) {
     *         set_resurrection_data(Do_it, &model[drone_index]);
     *         game_car[drone_index].just_crashed = 1;
     *         game_car[drone_index].crashtime = IRQTIME;
     *         StartBlast(drone_index);
     *     }
     *     if (IRQTIME - game_car[drone_index].crashtime > crash_delay) {
     *         game_car[drone_index].we_died = true;
     *     }
     * } else {
     *     game_car[drone_index].crashtime = 0;
     * }
     */
#endif
}

/**
 * Check if finished resurrecting
 */
void check_if_finished_resurrecting(void) {
#ifdef NON_MATCHING
    /* TODO: Check resurrection state for all cars */
#endif
}

/**
 * Set resurrection data for crashed car
 */
void set_resurrection_data(s16 mode, void *m) {
#ifdef NON_MATCHING
    /* TODO: Save position, velocity, orientation */
#endif
}

/**
 * Move car to resurrection point
 */
void resurrect_car(void *m) {
#ifdef NON_MATCHING
    /* TODO: Interpolate to resurrection position */
#endif
}

/**
 * Check if all cars have stopped moving
 */
s16 are_all_cars_stopped(void) {
#ifdef NON_MATCHING
    /* TODO: Check magvel for all human cars */
#endif
    return 1; /* Assume stopped */
}

/**
 * Set camera for model view
 */
void SetMCamera(s16 mode) {
#ifdef NON_MATCHING
    /* TODO: Camera setup based on mode */
#endif
}

/**
 * Check and update camera view
 */
void CheckCameraView(void) {
#ifdef NON_MATCHING
    /* TODO: Update camera based on game state */
#endif
}
