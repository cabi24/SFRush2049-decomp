/**
 * drivetrain.c - Drivetrain simulation for Rush 2049 N64
 *
 * Based on arcade drivetra.c - simulates engine, transmission,
 * clutch, and differential behavior.
 */

#include "game/drivetrain.h"
#include "game/car.h"

/* Surface types for torque curve selection */
#define DIRT    1

/**
 * Initialize drivetrain state from car definition
 */
void drivetrain_init(DrivetrainState *dt, const Car *cardef) {
    s32 i;

    /* Clear state */
    dt->engangvel = 0.0f;
    dt->engtorque = 0.0f;
    dt->clutchangvel = 0.0f;
    dt->clutchtorque = 0.0f;
    dt->clutch = 0.0f;
    dt->dwangvel = 0.0f;
    dt->dwtorque = 0.0f;

    /* Copy car parameters */
    dt->enginvmi = 1.0f / cardef->engmi;
    dt->clutchmaxt = cardef->clutchmaxt;
    dt->dwratio = cardef->dwratio;
    dt->topgear = cardef->topgear;
    dt->upshiftangvel = cardef->upshiftangvel;
    dt->downshiftangvel = cardef->downshiftangvel;
    dt->torquecurve = cardef->torquecp;
    dt->dirttorquecurve = cardef->dirttorquecp;
    dt->rpmperent = (f32)cardef->rpmperent;

    /* Torque scaling */
    dt->fgtorquescale = cardef->fgtorquescale;
    dt->sgtorquescale = cardef->sgtorquescale;
    dt->torquescale = cardef->torquescale;
    dt->dirttorquescale = cardef->dirttorquescale;

    /* Copy gear ratios */
    for (i = 0; i < NUM_GEARS; i++) {
        dt->transarray[i] = cardef->transarray[i];
    }

    /* Calculate differential inverse MOI (simplified) */
    dt->dwinvmi = 1.0f / (cardef->engmi * 0.5f);
    dt->efdwinvmi = dt->dwinvmi;

    /* Initial gear */
    dt->gear = GEAR_NEUTRAL;
    dt->commandgear = GEAR_NEUTRAL;
    dt->autotrans = 1;
    dt->transratio = 0.0f;
    dt->totalratio = 0.0f;

    /* Timing */
    dt->shifttime = 0.0f;
    dt->thetime = 0.0f;
    dt->dt = 1.0f / 60.0f;  /* Default 60Hz physics */

    /* Controls */
    dt->throttle = 0.0f;
    dt->ignition = 1;
    dt->startermotor = 0;

    /* Surface */
    for (i = 0; i < 4; i++) {
        dt->roadcode[i] = 0;
        dt->torque[i] = 0.0f;
        dt->tireforce_z[i] = -500.0f;  /* Default load */
    }

    /* Flags */
    dt->magicdif = cardef->magicdif;

    /* Gauges */
    dt->amperes = 0.0f;
    dt->oilpressure = 0.0f;
    dt->enginetemp = 70.0f;
    dt->magvel = 0.0f;
}

/**
 * Reset drivetrain to starting state
 */
void drivetrain_reset(DrivetrainState *dt) {
    s32 i;

    dt->engangvel = 0.0f;
    dt->engtorque = 0.0f;
    dt->clutchangvel = 0.0f;
    dt->clutchtorque = 0.0f;
    dt->dwangvel = 0.0f;
    dt->dwtorque = 0.0f;

    dt->gear = GEAR_NEUTRAL;
    dt->commandgear = GEAR_NEUTRAL;
    dt->transratio = 0.0f;
    dt->totalratio = 0.0f;
    dt->efdwinvmi = dt->dwinvmi;

    dt->shifttime = 0.0f;
    dt->thetime = 0.0f;

    for (i = 0; i < 4; i++) {
        dt->torque[i] = 0.0f;
    }

    dt->amperes = 0.0f;
    dt->oilpressure = 0.0f;
    dt->enginetemp = 70.0f;
}

/**
 * Main drivetrain update - call once per physics frame
 */
void drivetrain_update(DrivetrainState *dt) {
    f32 rearload;

    /* Automatic shifting if enabled */
    if (dt->autotrans) {
        drivetrain_autoshift(dt);
    }

    /* Engine torque calculation */
    drivetrain_engine(dt);

    /* Transmission ratio */
    drivetrain_transmission(dt);

    /* Average driveshaft angular velocity from rear tires */
    /* Note: In full sim, this would come from tire state */
    /* dt->dwangvel = (tires[2].angvel + tires[3].angvel) * 0.5f; */

    /* Clutch simulation */
    drivetrain_clutch(dt);

    /* Differential torque distribution */
    rearload = dt->tireforce_z[2] + dt->tireforce_z[3];

    /* Magic differential with load-sensitive torque split */
    if ((!dt->magicdif) || (rearload > -500.0f)) {
        /* Standard 50/50 split */
        dt->torque[2] += dt->dwtorque * 0.5f;
        dt->torque[3] += dt->dwtorque * 0.5f;
    } else {
        /* Load-proportional split for better traction */
        if (dt->tireforce_z[2] >= 0.0f) {
            /* Right rear unloaded - all torque to left */
            dt->torque[2] = 0.0f;
            dt->torque[3] = dt->dwtorque;
        } else if (dt->tireforce_z[3] >= 0.0f) {
            /* Left rear unloaded - all torque to right */
            dt->torque[2] = dt->dwtorque;
            dt->torque[3] = 0.0f;
        } else {
            /* Split proportional to load */
            dt->torque[2] = (dt->dwtorque * dt->tireforce_z[2]) / rearload;
            dt->torque[3] = (dt->dwtorque * dt->tireforce_z[3]) / rearload;
        }
    }

    /* Update effective inverse MOI for tires (simplified) */
    /* Real implementation would factor in clutch state */

    /* Update time */
    dt->thetime += dt->dt;
}

/**
 * Engine torque calculation
 */
void drivetrain_engine(DrivetrainState *dt) {
    s16 rpm_val;
    s16 throttle_val;
    const s16 *curve;
    f32 scale;

    rpm_val = (s16)(dt->engangvel * RDPS_TO_RPM);
    throttle_val = (s16)(dt->throttle * 128.0f);

    /* Select torque curve and scale based on surface and gear */
    if ((dt->roadcode[2] == DIRT) && (dt->roadcode[3] == DIRT) &&
        (dt->magvel > 40.0f)) {
        /* Both rear tires on dirt at speed */
        curve = dt->dirttorquecurve;
        scale = dt->dirttorquescale;
    } else {
        /* Normal pavement */
        curve = dt->torquecurve;

        /* Gear-specific torque scaling */
        if (dt->gear == 1) {
            scale = dt->fgtorquescale;
        } else if (dt->gear == 2) {
            scale = dt->sgtorquescale;
        } else {
            scale = dt->torquescale;
        }
    }

    /* Get engine torque from curve */
    dt->engtorque = scale * (f32)drivetrain_engine_torque(
        dt, rpm_val, throttle_val,
        (s16)dt->ignition, (s16)dt->startermotor,
        curve
    );

    /* Auto transmission torque converter loss (not in top gear) */
    if (dt->autotrans && (dt->gear != dt->topgear)) {
        dt->engtorque *= AUTO_TRANS_LOSS;
    }
}

/**
 * Transmission ratio calculation
 */
void drivetrain_transmission(DrivetrainState *dt) {
    /* Gear ratio from array (index = gear + 1 to handle reverse at -1) */
    dt->transratio = dt->transarray[dt->gear + 1];
    dt->totalratio = dt->transratio * dt->dwratio;
}

/**
 * Clutch slip calculation - determines torque transfer
 */
void drivetrain_clutch(DrivetrainState *dt) {
    f32 totratsq;
    f32 centrifvel;
    f32 curclmaxt;

    totratsq = dt->totalratio * dt->totalratio;

    /* Neutral gear - no torque transfer */
    if (dt->gear == GEAR_NEUTRAL) {
        dt->clutchtorque = 0.0f;
        dt->dwtorque = 0.0f;
        dt->clutchangvel = dt->engangvel;
        dt->engangvel += dt->engtorque * dt->enginvmi * dt->dt;
        dt->efdwinvmi = dt->dwinvmi;
        return;
    }

    /* Clutch output angular velocity */
    dt->clutchangvel = dt->dwangvel * dt->totalratio;

    /* Calculate current clutch max torque */
    if (dt->autotrans) {
        /* Centrifugal clutch behavior */
        if (dt->engangvel < (dt->clutchangvel - (CLUTCH_SLIP_DELTA * RPM_TO_RDPS))) {
            /* Engine dragging - full clutch engagement */
            curclmaxt = dt->clutchmaxt;
        } else {
            /* Centrifugal engagement based on engine RPM */
            centrifvel = dt->engangvel - (dt->clutchangvel * 0.25f);

            if (centrifvel < (CLUTCH_MIN_RPM * RPM_TO_RDPS)) {
                curclmaxt = 0.0f;
            } else if (centrifvel > (CLUTCH_MAX_RPM * RPM_TO_RDPS)) {
                curclmaxt = dt->clutchmaxt;
            } else {
                /* Linear ramp between min and max RPM */
                curclmaxt = (dt->clutchmaxt / (CLUTCH_MIN_RPM * RPM_TO_RDPS)) *
                    (centrifvel - (CLUTCH_MIN_RPM * RPM_TO_RDPS));
            }
        }
    } else {
        /* Manual clutch */
        if (dt->clutch < 0.0f) {
            curclmaxt = dt->clutchmaxt;
        } else if (dt->clutch > CLUTCH_FRICTION_POINT) {
            curclmaxt = 0.0f;
        } else {
            /* Linear engagement */
            curclmaxt = (1.0f / CLUTCH_FRICTION_POINT) *
                (CLUTCH_FRICTION_POINT - dt->clutch) * dt->clutchmaxt;
        }
    }

    if (curclmaxt < 0.0f) {
        curclmaxt = 0.0f;
    }

    /* Slip logic - engine faster than driveshaft */
    if (dt->engangvel > dt->clutchangvel) {
        dt->clutchtorque = curclmaxt;
        dt->engangvel += (dt->engtorque - dt->clutchtorque) * dt->enginvmi * dt->dt;

        if (dt->engangvel < dt->clutchangvel) {
            /* Engine slowed below clutch - check for lockup */
            if (dt->engtorque < -curclmaxt) {
                dt->clutchtorque = -curclmaxt;
                dt->engangvel += (dt->engtorque - dt->clutchtorque) *
                    dt->enginvmi * dt->dt;
                dt->efdwinvmi = dt->dwinvmi;
            } else {
                /* Locked together */
                dt->engangvel = dt->clutchangvel;
                dt->clutchtorque = dt->engtorque;
                dt->efdwinvmi = 1.0f / (1.0f / dt->dwinvmi +
                    totratsq / dt->enginvmi);
            }
        } else {
            /* Still slipping */
            dt->efdwinvmi = 1.0f / (1.0f / dt->dwinvmi +
                totratsq / dt->enginvmi);
        }
    } else {
        /* Driveshaft faster than engine */
        dt->clutchtorque = -curclmaxt;
        dt->engangvel += (dt->engtorque - dt->clutchtorque) * dt->enginvmi * dt->dt;

        if (dt->engangvel > dt->clutchangvel) {
            /* Engine sped up past clutch */
            if (dt->engtorque > curclmaxt) {
                dt->clutchtorque = curclmaxt;
                dt->engangvel += (dt->engtorque - dt->clutchtorque) *
                    dt->enginvmi * dt->dt;
                dt->efdwinvmi = dt->dwinvmi;
            } else {
                /* Locked together */
                dt->engangvel = dt->clutchangvel;
                dt->clutchtorque = dt->engtorque;
                dt->efdwinvmi = 1.0f / (1.0f / dt->dwinvmi +
                    totratsq / dt->enginvmi);
            }
        } else {
            /* Still slipping (reverse direction) */
            dt->efdwinvmi = 1.0f / (1.0f / dt->dwinvmi +
                totratsq / dt->enginvmi);
        }
    }

    /* Drive wheel torque */
    dt->dwtorque = dt->clutchtorque * dt->totalratio;
}

/**
 * Automatic transmission shift logic
 */
void drivetrain_autoshift(DrivetrainState *dt) {
    f32 modupshiftangvel, moddownshiftangvel;
    f32 fact;

    /* Modify shift points for throttle position */
    fact = (3.0f + dt->throttle) * 0.25f;
    modupshiftangvel = dt->upshiftangvel * fact;
    moddownshiftangvel = dt->downshiftangvel * fact;

    /* Handle neutral and reverse as direct commands */
    if ((dt->commandgear == GEAR_NEUTRAL) || (dt->commandgear == GEAR_REVERSE)) {
        dt->gear = dt->commandgear;
    } else {
        /* In forward gears - auto shift */
        if ((dt->gear == GEAR_NEUTRAL) || (dt->gear == GEAR_REVERSE)) {
            /* Find appropriate gear when coming out of N/R */
            drivetrain_find_best_gear(dt, modupshiftangvel, moddownshiftangvel);
        }

        /* Check for shifts */
        if (dt->engangvel > modupshiftangvel) {
            drivetrain_upshift(dt);
        }
        if (dt->engangvel < moddownshiftangvel) {
            drivetrain_downshift(dt);
        }
    }
}

/**
 * Find best gear for current speed
 */
void drivetrain_find_best_gear(DrivetrainState *dt, f32 usang, f32 dsang) {
    f32 angvel, tstratio;

    for (dt->gear = 1; dt->gear < dt->topgear; dt->gear++) {
        tstratio = dt->dwratio * dt->transarray[dt->gear + 1];
        angvel = dt->dwangvel * tstratio;
        if (angvel < usang) {
            break;
        }
    }
}

/**
 * Upshift one gear
 */
void drivetrain_upshift(DrivetrainState *dt) {
    if ((dt->gear < dt->topgear) && (dt->thetime > dt->shifttime)) {
        dt->shifttime = dt->thetime + 1.0f;
        dt->gear++;
    }
}

/**
 * Downshift one gear
 */
void drivetrain_downshift(DrivetrainState *dt) {
    if ((dt->gear > MIN_GEAR) && (dt->thetime > dt->shifttime)) {
        dt->shifttime = dt->thetime + 1.0f;
        dt->gear--;
    }
}

/**
 * Engine torque curve lookup with bilinear interpolation
 */
s16 drivetrain_engine_torque(
    DrivetrainState *dt,
    s16 rpm,
    s16 throttle,
    s16 ignition,
    s16 start,
    const s16 *torquecurve
) {
    s16 rindex, tindex, rrem, trem;
    s16 left, right;
    const s16 *low_ptr, *hi_ptr;
    const s16 *zzp, *fnp;

    zzp = torquecurve;
    fnp = torquecurve + 11;

    /* Dashboard gauge updates */
    if (rpm < 2000) {
        dt->amperes = ((10.0f / 2000.0f) * (f32)rpm) - 10.0f;
        dt->oilpressure = (40.0f / 2000.0f) * (f32)rpm;
    } else {
        dt->oilpressure = 40.0f;
        dt->amperes = 10.0f;
    }

    if (start) {
        dt->amperes = -50.0f;
    }

    /* Very low RPM handling */
    if (rpm < 500) {
        if (rpm >= 0) {
            if (start) {
                return *zzp;
            } else {
                return (s16)((-(*zzp) * rpm) / 500);
            }
        }

        dt->oilpressure = 0.0f;

        if (rpm >= -1000) {
            if (start) {
                return *zzp + (s16)((*fnp * rpm) / 1000);
            } else {
                return (s16)((*fnp * rpm) / 1000);
            }
        }
        return 70;
    }

    /* Engine temperature warmup */
    if (dt->thetime < 60.0f) {
        dt->enginetemp = 70.0f + ((130.0f / 60.0f) * dt->thetime);
    } else {
        dt->enginetemp = 200.0f;
    }

    /* Ignition off */
    if (!ignition) {
        return -(*zzp);
    }

    /* Starter motor cranking */
    if ((rpm < 1000) && start) {
        return *zzp;
    }

    /* Bilinear interpolation from torque table */
    rindex = rpm / (s16)dt->rpmperent;
    rrem = rpm % (s16)dt->rpmperent;

    if (rindex < 0) {
        rindex = 0;
        rrem = 0;
    }
    if (rindex >= 11) {
        rindex = 10;
        rrem = (s16)dt->rpmperent - 1;
    }

    tindex = throttle / 14;
    trem = throttle % 14;

    if (tindex >= 9) {
        tindex = 8;
        trem = 13;
    }

    /* Interpolate in RPM direction first */
    low_ptr = torquecurve + (tindex * 12) + rindex;
    hi_ptr = torquecurve + (tindex * 12) + rindex + 1;
    left = drivetrain_interp(*low_ptr, *hi_ptr, rrem, (s32)dt->rpmperent - 1);

    /* Then interpolate in throttle direction */
    low_ptr = torquecurve + ((tindex + 1) * 12) + rindex;
    hi_ptr = torquecurve + ((tindex + 1) * 12) + rindex + 1;
    right = drivetrain_interp(*low_ptr, *hi_ptr, rrem, (s32)dt->rpmperent - 1);

    /* Final interpolation between throttle rows */
    right = drivetrain_interp(left, right, trem, 14);

    return right;
}

/**
 * Linear interpolation helper
 */
s16 drivetrain_interp(s32 a, s32 b, s32 rem, s32 total) {
    return (s16)(a + (((b - a) * rem) / total));
}

/**
 * Manual shift up
 */
void drivetrain_shift_up(DrivetrainState *dt) {
    if (!dt->autotrans) {
        dt->commandgear = dt->gear + 1;
        if (dt->commandgear > dt->topgear) {
            dt->commandgear = dt->topgear;
        }
        drivetrain_upshift(dt);
    }
}

/**
 * Manual shift down
 */
void drivetrain_shift_down(DrivetrainState *dt) {
    if (!dt->autotrans) {
        dt->commandgear = dt->gear - 1;
        if (dt->commandgear < GEAR_REVERSE) {
            dt->commandgear = GEAR_REVERSE;
        }
        drivetrain_downshift(dt);
    }
}

/**
 * Set gear directly (for neutral/reverse)
 */
void drivetrain_set_gear(DrivetrainState *dt, s32 gear) {
    dt->commandgear = gear;
    if (dt->autotrans) {
        /* Auto will handle gear change */
    } else {
        dt->gear = gear;
    }
}

/**
 * Get engine RPM
 */
f32 drivetrain_get_rpm(DrivetrainState *dt) {
    return dt->engangvel * RDPS_TO_RPM;
}

/**
 * Get wheel RPM (average of driven wheels)
 */
f32 drivetrain_get_wheel_rpm(DrivetrainState *dt) {
    return dt->dwangvel * RDPS_TO_RPM;
}

/**
 * Check if clutch is currently slipping
 */
s32 drivetrain_is_clutch_slipping(DrivetrainState *dt) {
    f32 diff;

    if (dt->gear == GEAR_NEUTRAL) {
        return 0;
    }

    diff = dt->engangvel - dt->clutchangvel;
    if (diff < 0.0f) {
        diff = -diff;
    }

    /* Slipping if difference exceeds threshold */
    return (diff > (50.0f * RPM_TO_RDPS));
}

/* ========================================================================
 * Arcade-compatible function aliases (drivetra.c)
 * ======================================================================== */

/**
 * whatslips - Clutch slip calculation (arcade name)
 * Wrapper for drivetrain_clutch()
 */
void whatslips(DrivetrainState *m) {
    drivetrain_clutch(m);
}

/**
 * autoshift - Automatic transmission shift logic (arcade name)
 * Wrapper for drivetrain_autoshift()
 */
void autoshift(DrivetrainState *m) {
    drivetrain_autoshift(m);
}

/**
 * transmission - Transmission ratio calculation (arcade name)
 * Wrapper for drivetrain_transmission()
 */
void transmission(DrivetrainState *m) {
    drivetrain_transmission(m);
}

/**
 * engine - Engine torque calculation (arcade name)
 * Wrapper for drivetrain_engine()
 */
void engine(DrivetrainState *m) {
    drivetrain_engine(m);
}

/**
 * find_best_gear - Find optimal gear for current speed (arcade name)
 * Wrapper for drivetrain_find_best_gear()
 */
void find_best_gear(DrivetrainState *m, f32 usang, f32 dsang) {
    drivetrain_find_best_gear(m, usang, dsang);
}

/**
 * upshift - Shift up one gear (arcade name)
 * Wrapper for drivetrain_upshift()
 */
void upshift(DrivetrainState *m) {
    drivetrain_upshift(m);
}

/**
 * downshift - Shift down one gear (arcade name)
 * Wrapper for drivetrain_downshift()
 */
void downshift(DrivetrainState *m) {
    drivetrain_downshift(m);
}

/**
 * enginetorque - Engine torque curve lookup (arcade name)
 * Wrapper for drivetrain_engine_torque()
 */
s16 enginetorque(DrivetrainState *m, s16 rpm, s16 throttle, s16 ignition,
                 s16 start, const s16 *torquecurve) {
    return drivetrain_engine_torque(m, rpm, throttle, ignition, start, torquecurve);
}

/**
 * interp - Linear interpolation helper (arcade name)
 * Matches arcade: interp(int a, int b, int rem, int total)
 */
s16 interp(s32 a, s32 b, s32 rem, s32 total) {
    return drivetrain_interp(a, b, rem, total);
}

/* ========================================================================
 * MODELDAT pointer-based arcade functions (drivetra.c)
 * These match the arcade function signatures exactly using MODELDAT (CarPhysics)
 * ======================================================================== */

#include "game/physics.h"

/**
 * drivetrain_m - Main drivetrain update function
 * Based on arcade: drivetra.c:drivetrain()
 *
 * This is the main entry point called each physics frame.
 * Updates engine, transmission, clutch, and distributes torque to wheels.
 */
void drivetrain_m(MODELDAT *m) {
    f32 rearload;

    /* Automatic shifting if enabled */
    if (m->autotrans) {
        autoshift_m(m);
    }

    /* Engine torque calculation */
    engine_m(m);

    /* Transmission ratio */
    transmission_m(m);

    /* Average driveshaft angular velocity from rear tires */
    m->dwangvel = (m->tires[2].angvel + m->tires[3].angvel) * 0.5f;

    /* Clutch slip calculation */
    whatslips_m(m);

    /* Calculate rear load for differential */
    rearload = m->TIREFORCE[2][ZCOMP] + m->TIREFORCE[3][ZCOMP];

    /* Magic rear differential with load sensitive torque split */
    if ((!m->magicdif) || (rearload > -500.0f)) {
        /* Standard 50/50 split */
        m->torque[2] += m->dwtorque * 0.5f;
        m->torque[3] += m->dwtorque * 0.5f;
    } else {
        /* Load-proportional split for better traction */
        if (m->TIREFORCE[2][ZCOMP] >= 0.0f) {
            /* Left rear unloaded - all torque to right */
            m->torque[2] = 0.0f;
            m->torque[3] = m->dwtorque;
        } else if (m->TIREFORCE[3][ZCOMP] >= 0.0f) {
            /* Right rear unloaded - all torque to left */
            m->torque[2] = m->dwtorque;
            m->torque[3] = 0.0f;
        } else {
            /* Split proportional to load */
            m->torque[2] = (m->dwtorque * m->TIREFORCE[2][ZCOMP]) / rearload;
            m->torque[3] = (m->dwtorque * m->TIREFORCE[3][ZCOMP]) / rearload;
        }
    }

    /* Update effective inverse MOI for tires (simplified) */
    m->tires[2].radius = m->efdwinvmi * 2.0f;  /* This isn't correct!!! (arcade comment) */
    m->tires[3].radius = m->efdwinvmi * 2.0f;
}

/**
 * whatslips_m - Clutch slip calculation
 * Based on arcade: drivetra.c:whatslips()
 *
 * Determines clutch engagement and torque transfer based on
 * engine vs driveshaft angular velocities.
 */
void whatslips_m(MODELDAT *m) {
    f32 totratsq, centrifvel, curclmaxt;

    totratsq = m->totalratio * m->totalratio;

    /* Neutral gear - no torque transfer */
    if (m->gear == NEUTRALGEAR) {
        m->clutchtorque = 0.0f;
        m->dwtorque = 0.0f;
        m->clutchangvel = m->engangvel;
        m->engangvel += m->engtorque * m->enginvmi * m->dt;
        m->efdwinvmi = m->dwinvmi;
        return;
    }

    /* Clutch output angular velocity */
    m->clutchangvel = m->dwangvel * m->totalratio;

    /* Calculate current clutch max torque */
    if (m->autotrans) {
        /* Centrifugal clutch behavior */
        if (m->engangvel < (m->clutchangvel - (100.0f * rpmtordps))) {
            /* Engine dragging - full clutch engagement */
            curclmaxt = m->clutchmaxt;
        } else {
            /* Centrifugal engagement based on engine RPM */
            centrifvel = m->engangvel - (m->clutchangvel * 0.25f);

            if (centrifvel < (1500.0f * rpmtordps)) {
                curclmaxt = 0.0f;
            } else if (centrifvel > (3000.0f * rpmtordps)) {
                curclmaxt = m->clutchmaxt;
            } else {
                /* Linear ramp between 1500 and 3000 RPM */
                curclmaxt = (m->clutchmaxt * (1.0f / (1500.0f * rpmtordps))) *
                    (centrifvel - (1500.0f * rpmtordps));
            }
        }
    } else {
        /* Manual clutch */
        if (m->clutch < 0.0f) {
            curclmaxt = m->clutchmaxt;
        } else if (m->clutch > 0.8f) {
            /* Friction point at 0.8 */
            curclmaxt = 0.0f;
        } else {
            curclmaxt = (1.0f / 0.8f) * (0.8f - m->clutch) * m->clutchmaxt;
        }
    }

    if (curclmaxt < 0.0f) {
        curclmaxt = 0.0f;
    }

    /* Slip logic - engine faster than driveshaft */
    if (m->engangvel > m->clutchangvel) {
        m->clutchtorque = curclmaxt;
        m->engangvel += (m->engtorque - m->clutchtorque) * m->enginvmi * m->dt;

        if (m->engangvel < m->clutchangvel) {
            /* Engine slowed below clutch - check for lockup */
            if (m->engtorque < -curclmaxt) {
                m->clutchtorque = -curclmaxt;
                /* We just did this, why now? (arcade comment) */
                m->engangvel += (m->engtorque - m->clutchtorque) * m->enginvmi * m->dt;
                m->efdwinvmi = m->dwinvmi;
            } else {
                /* Locked together */
                m->engangvel = m->clutchangvel;
                m->clutchtorque = m->engtorque;
                m->efdwinvmi = 1.0f / (1.0f / m->dwinvmi + totratsq / m->enginvmi);
            }
        } else {
            /* Clutch slipping, so why engine? (arcade comment) */
            m->efdwinvmi = 1.0f / (1.0f / m->dwinvmi + totratsq / m->enginvmi);
        }
    } else {
        /* Driveshaft faster than engine */
        m->clutchtorque = -curclmaxt;
        m->engangvel += (m->engtorque - m->clutchtorque) * m->enginvmi * m->dt;

        if (m->engangvel > m->clutchangvel) {
            /* Engine sped up past clutch */
            if (m->engtorque > curclmaxt) {
                m->clutchtorque = curclmaxt;
                m->engangvel += (m->engtorque - m->clutchtorque) * m->enginvmi * m->dt;
                m->efdwinvmi = m->dwinvmi;
            } else {
                /* Locked together */
                m->engangvel = m->clutchangvel;
                m->clutchtorque = m->engtorque;
                m->efdwinvmi = 1.0f / (1.0f / m->dwinvmi + totratsq / m->enginvmi);
            }
        } else {
            /* Still slipping (reverse direction) */
            m->efdwinvmi = 1.0f / (1.0f / m->dwinvmi + totratsq / m->enginvmi);
        }
    }

    /* Drive wheel torque */
    m->dwtorque = m->clutchtorque * m->totalratio;
}

/**
 * autoshift_m - Automatic transmission shift logic
 * Based on arcade: drivetra.c:autoshift()
 */
void autoshift_m(MODELDAT *m) {
    f32 modupshiftangvel, moddownshiftangvel, fact;

    /* Modify shift points for throttle position */
    fact = (3.0f + m->throttle) * 0.25f;
    modupshiftangvel = m->upshiftangvel * fact;
    moddownshiftangvel = m->downshiftangvel * fact;

    /* Handle neutral and reverse as direct commands */
    if ((m->commandgear == NEUTRALGEAR) || (m->commandgear == REVERSEGEAR)) {
        m->gear = m->commandgear;
    } else {
        /* In forward gears - auto shift */
        if ((m->gear == NEUTRALGEAR) || (m->gear == REVERSEGEAR)) {
            /* Find appropriate gear when coming out of N/R */
            find_best_gear_m(m, modupshiftangvel, moddownshiftangvel);
        }

        /* Check for shifts */
        if (m->engangvel > modupshiftangvel) {
            upshift_m(m);
        }
        if (m->engangvel < moddownshiftangvel) {
            downshift_m(m);
        }
    }
}

/**
 * transmission_m - Transmission ratio calculation
 * Based on arcade: drivetra.c:transmission()
 */
void transmission_m(MODELDAT *m) {
    /* Gear ratio from array (index = gear + 1 to handle reverse at -1) */
    m->transratio = m->transarray[m->gear + 1];
    m->totalratio = m->transratio * m->dwratio;
}

/**
 * find_best_gear_m - Find optimal gear for current speed
 * Based on arcade: drivetra.c:find_best_gear()
 */
void find_best_gear_m(MODELDAT *m, f32 usang, f32 dsang) {
    f32 angvel, tstratio;

    for (m->gear = 1; m->gear < m->topgear; m->gear++) {
        tstratio = m->dwratio * m->transarray[m->gear + 1];
        angvel = m->dwangvel * tstratio;
        if (angvel < usang) {
            break;
        }
    }
}

/**
 * upshift_m - Shift up one gear
 * Based on arcade: drivetra.c:upshift()
 */
void upshift_m(MODELDAT *m) {
    if ((m->gear < m->topgear) && (m->thetime > m->shifttime)) {
        m->shifttime = m->thetime + 1.0f;
        m->gear++;
    }
}

/**
 * downshift_m - Shift down one gear
 * Based on arcade: drivetra.c:downshift()
 */
void downshift_m(MODELDAT *m) {
    if ((m->gear > MINGEAR) && (m->thetime > m->shifttime)) {
        m->shifttime = m->thetime + 1.0f;
        m->gear--;
    }
}

/**
 * engine_m - Engine torque calculation
 * Based on arcade: drivetra.c:engine()
 */
void engine_m(MODELDAT *m) {
    s16 rpm_val, throttle_val;
    const s16 *curve;
    f32 scale;

    rpm_val = (s16)(m->engangvel * rdpstorpm);
    throttle_val = (s16)(m->throttle * 128.0f);

    /* Select torque curve and scale based on surface and gear */
    /* Don't do dirt slowdown if under 40 fps (~28 mph) */
    if ((m->roadcode[2] == DIRT) && (m->roadcode[3] == DIRT) && (m->magvel > 40.0f)) {
        /* Both rear tires on dirt at speed */
        curve = m->dirttorquecurve;
        scale = m->dirttorquescale;
    } else {
        /* Normal pavement */
        curve = m->torquecurve;

        /* Gear-specific torque scaling */
        if (m->gear == 1) {
            scale = m->fgtorquescale;
        } else if (m->gear == 2) {
            scale = m->sgtorquescale;
        } else {
            scale = m->torquescale;
        }
    }

    /* Get engine torque from curve */
    m->engtorque = scale * (f32)enginetorque_m(m, rpm_val, throttle_val,
        (s16)m->ignition, (s16)m->startermotor, curve);

    /* Reduce transmission torque if in auto and not top gear
       (car has locking torque converter which locks in top gear) */
    if (m->autotrans && (m->gear != m->topgear)) {
        m->engtorque *= AUTOLOSS;
    }
}

/**
 * enginetorque_m - Engine torque curve lookup with bilinear interpolation
 * Based on arcade: drivetra.c:enginetorque()
 */
s16 enginetorque_m(MODELDAT *m, s16 rpm, s16 throttle, s16 ignition,
                   s16 start, const s16 *torquecurve) {
    s16 rindex, tindex, rrem, trem;
    s16 left, right;
    const s16 *low_ptr, *hi_ptr;
    const s16 *zzp, *fnp;

    zzp = torquecurve;
    fnp = torquecurve + 11;

    /* Dashboard gauge updates */
    if (rpm < 2000) {
        m->amperes = ((10.0f / 2000.0f) * (f32)rpm) - 10.0f;
        m->oilpressure = (40.0f / 2000.0f) * (f32)rpm;
    } else {
        m->oilpressure = 40.0f;
        m->amperes = 10.0f;
    }

    if (start) {
        m->amperes = -50.0f;
    }

    /* Very low RPM handling */
    if (rpm < 500) {
        if (rpm >= 0) {
            if (start) {
                return *zzp;
            } else {
                return (s16)((-(*zzp) * rpm) / 500);
            }
        }

        m->oilpressure = 0.0f;

        if (rpm >= -1000) {
            if (start) {
                return *zzp + (s16)((*fnp * rpm) / 1000);
            } else {
                return (s16)((*fnp * rpm) / 1000);
            }
        }
        return 70;
    }

    /* Engine temperature warmup */
    if (m->thetime < 60.0f) {
        m->enginetemp = 70.0f + ((130.0f / 60.0f) * m->thetime);
    } else {
        m->enginetemp = 200.0f;
    }

    /* Ignition off */
    if (!ignition) {
        return -(*zzp);
    }

    /* Starter motor cranking */
    if ((rpm < 1000) && start) {
        return *zzp;
    }

    /* Bilinear interpolation from torque table */
    rindex = rpm / (s16)m->rpmperent;
    rrem = rpm % (s16)m->rpmperent;

    if (rindex < 0) {
        rindex = 0;
        rrem = 0;
    }
    if (rindex >= 11) {
        rindex = 10;
        rrem = (s16)m->rpmperent - 1;
    }

    tindex = throttle / 14;
    trem = throttle % 14;

    if (tindex >= 9) {
        tindex = 8;
        trem = 13;
    }

    /* Interpolate in RPM direction first */
    low_ptr = torquecurve + (tindex * 12) + rindex;
    hi_ptr = torquecurve + (tindex * 12) + rindex + 1;
    left = interp(*low_ptr, *hi_ptr, rrem, (s32)m->rpmperent - 1);

    /* Then interpolate in throttle direction */
    low_ptr = torquecurve + ((tindex + 1) * 12) + rindex;
    hi_ptr = torquecurve + ((tindex + 1) * 12) + rindex + 1;
    right = interp(*low_ptr, *hi_ptr, rrem, (s32)m->rpmperent - 1);

    /* Final interpolation between throttle rows */
    right = interp(left, right, trem, 14);

    return right;
}
