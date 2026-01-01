/**
 * wings.h - Wing deployment and aerodynamic system for Rush 2049 N64
 *
 * Rush 2049's signature feature: deployable wings for stunt control.
 * This is unique to Rush 2049 - the arcade Rush The Rock doesn't have wings.
 *
 * The wing system provides:
 *   - Lift force (reduces fall rate during jumps)
 *   - Drag force (reduces forward speed)
 *   - Air control (pitch/roll influence via player input)
 *   - Enhanced trick scoring potential
 *
 * Based on arcade drivsym.c aerodynamic calculations with N64 adaptations.
 */

#ifndef WINGS_H
#define WINGS_H

#include "types.h"

/* ============================================================================
 * Wing States
 * ============================================================================ */
#define WING_STATE_RETRACTED    0   /* Wings fully hidden in body */
#define WING_STATE_DEPLOYING    1   /* Wings extending outward */
#define WING_STATE_DEPLOYED     2   /* Wings fully extended */
#define WING_STATE_RETRACTING   3   /* Wings folding back in */
#define WING_STATE_DAMAGED      4   /* Wings damaged from crash */

/* ============================================================================
 * Wing Physics Constants
 * Derived from arcade drivsym.c aerodynamic model
 * ============================================================================ */

/* Deployment timing (frames at 60 FPS) */
#define WING_DEPLOY_TIME        15      /* Frames to fully deploy */
#define WING_RETRACT_TIME       10      /* Frames to fully retract (faster) */
#define WING_DAMAGE_LOCKOUT     120     /* Frames before damaged wings work again */

/* Aerodynamic constants (based on arcade srefpcybo2 model) */
#define WING_AIR_DENSITY        0.002377f   /* Slug/ft^3 (sea level) */
#define WING_AREA_BASE          10.0f       /* Base wing area (sq ft) */
#define WING_AREA_DEPLOYED      30.0f       /* Full wing area (sq ft) */

/* Lift coefficients */
#define WING_CL_BASE            0.2f        /* Base lift coefficient (no wings) */
#define WING_CL_MAX             2.0f        /* Max lift coefficient (wings full) */
#define WING_LIFT_SCALE         0.015f      /* Lift force scaling for N64 physics */

/* Drag coefficients (from arcade srefpcybo2 = 0.0087) */
#define WING_CD_BASE            0.05f       /* Base drag (retracted) */
#define WING_CD_DEPLOYED        0.35f       /* Deployed wing drag */
#define WING_DRAG_SCALE         0.008f      /* Drag force scaling */

/* Control authority (how much wings affect rotation) */
#define WING_PITCH_AUTHORITY    2.5f        /* Pitch control multiplier */
#define WING_ROLL_AUTHORITY     3.0f        /* Roll control multiplier */
#define WING_DAMPING            0.96f       /* Angular velocity damping */

/* Speed thresholds */
#define WING_MIN_SPEED          10.0f       /* Min speed for aero effects (ft/s) */
#define WING_OPTIMAL_SPEED      100.0f      /* Optimal glide speed */
#define WING_MAX_LIFT_SPEED     200.0f      /* Speed at max lift */

/* Force limits */
#define WING_MAX_LIFT           25.0f       /* Maximum lift force */
#define WING_MAX_DRAG           15.0f       /* Maximum drag force */

/* ============================================================================
 * Wing Damage Thresholds
 * ============================================================================ */
#define WING_DAMAGE_THRESHOLD   50.0f       /* Impact speed to damage wings */
#define WING_DAMAGE_ANGLE       60.0f       /* Landing angle that damages wings */

/* ============================================================================
 * Wing State Structure
 * Offsets from car base pointer (identified from N64 ROM analysis)
 * ============================================================================ */

/* Car structure offsets for wing data */
#define WING_OFFSET_STATE       0x1D4       /* s32: wing state */
#define WING_OFFSET_TIMER       0x1D8       /* f32: deploy/retract timer */
#define WING_OFFSET_ANGLE       0x1DC       /* f32: wing angle (0.0-1.0) */
#define WING_OFFSET_LIFT_COEF   0x1E0       /* f32: current lift coefficient */
#define WING_OFFSET_DRAG_COEF   0x1E4       /* f32: current drag coefficient */
#define WING_OFFSET_DAMAGE      0x1E8       /* s32: damage level (0-100) */
#define WING_OFFSET_LOCKOUT     0x1EC       /* s32: damage lockout timer */

/* Alternate offsets used by some functions */
#define WING_OFFSET_STATE_ALT   0x1E0       /* Alternative state offset */
#define WING_OFFSET_TIMER_ALT   0x1E4       /* Alternative timer offset */
#define WING_OFFSET_LIFT_ALT    0x1E8       /* Alternative lift offset */
#define WING_OFFSET_DRAG_ALT    0x1EC       /* Alternative drag offset */

/* ============================================================================
 * Wing Data Structure (for external tracking)
 * ============================================================================ */
typedef struct WingData {
    s32     state;              /* WING_STATE_* */
    f32     timer;              /* Deploy/retract timer (0.0-1.0) */
    f32     angle;              /* Current wing angle (0.0 = retracted, 1.0 = deployed) */
    f32     lift_coef;          /* Current lift coefficient */
    f32     drag_coef;          /* Current drag coefficient */
    s32     damage;             /* Damage level (0-100, 100 = destroyed) */
    s32     lockout;            /* Damage lockout timer */
    f32     lift_force;         /* Calculated lift force */
    f32     drag_force;         /* Calculated drag force */
} WingData;

/* ============================================================================
 * Global Wing State
 * ============================================================================ */
extern s32 wing_enabled;        /* Global wing system enable flag */

/* ============================================================================
 * Wing Control Functions
 * ============================================================================ */

/**
 * wing_init - Initialize wing system
 * Call once at game start
 */
void wing_init(void);

/**
 * wing_reset - Reset wing state for a car
 * @param car Pointer to car data structure
 */
void wing_reset(void *car);

/**
 * wing_deploy - Begin deploying wings
 * @param car Pointer to car data structure
 * @param deploy 1 to deploy, 0 to retract
 *
 * Called when player presses/releases wing button (C-Down).
 * Initiates wing deployment animation and physics.
 */
void wing_deploy(void *car, s32 deploy);

/**
 * wing_deploy_update - Update wing deployment animation
 * @param car Pointer to car data structure
 *
 * Called each frame to animate wing deployment.
 * Updates wing angle and calculates aerodynamic coefficients.
 */
void wing_deploy_update(void *car);

/**
 * wing_retract - Begin retracting wings
 * @param car Pointer to car data structure
 *
 * Called when player releases wing button or car lands.
 */
void wing_retract(void *car);

/**
 * wing_update - Main per-frame wing update
 * @param car Pointer to car data structure
 *
 * Updates wing state, animation, and applies physics.
 */
void wing_update(void *car);

/* ============================================================================
 * Wing Physics Functions
 * ============================================================================ */

/**
 * wing_calc_lift - Calculate lift force from wings
 * @param car Pointer to car data structure
 * @param out_lift Output lift force vector
 *
 * Implements L = 0.5 * rho * V^2 * S * Cl
 * Based on arcade drivsym.c aerodynamic model.
 */
void wing_calc_lift(void *car, f32 *out_lift);

/**
 * wing_calc_drag - Calculate drag force from wings
 * @param car Pointer to car data structure
 * @param out_drag Output drag force vector
 *
 * Implements D = 0.5 * rho * V^2 * S * Cd
 * Based on arcade srefpcybo2 constant (0.0087).
 */
void wing_calc_drag(void *car, f32 *out_drag);

/**
 * glide_physics_apply - Apply all wing aerodynamic forces
 * @param car Pointer to car data structure
 *
 * Applies lift, drag, and control forces to car.
 * Called from wing_update when wings are deployed.
 */
void glide_physics_apply(void *car);

/**
 * wing_apply_control - Apply player input to wing control surfaces
 * @param car Pointer to car data structure
 * @param pitch_input Player pitch input (-1.0 to 1.0)
 * @param roll_input Player roll input (-1.0 to 1.0)
 *
 * Modifies car angular velocity based on wing control authority.
 */
void wing_apply_control(void *car, f32 pitch_input, f32 roll_input);

/* ============================================================================
 * Wing Query Functions
 * ============================================================================ */

/**
 * wing_state_get - Get current wing state
 * @param car Pointer to car data structure
 * @return WING_STATE_* value
 */
s32 wing_state_get(void *car);

/**
 * wing_angle_get - Get current wing angle
 * @param car Pointer to car data structure
 * @return Wing angle (0.0 = retracted, 1.0 = deployed)
 */
f32 wing_angle_get(void *car);

/**
 * wing_is_deployed - Check if wings are fully deployed
 * @param car Pointer to car data structure
 * @return 1 if deployed, 0 otherwise
 */
s32 wing_is_deployed(void *car);

/**
 * wing_is_damaged - Check if wings are damaged
 * @param car Pointer to car data structure
 * @return 1 if damaged, 0 otherwise
 */
s32 wing_is_damaged(void *car);

/**
 * wing_get_lift_coef - Get current lift coefficient
 * @param car Pointer to car data structure
 * @return Current lift coefficient
 */
f32 wing_get_lift_coef(void *car);

/**
 * wing_get_drag_coef - Get current drag coefficient
 * @param car Pointer to car data structure
 * @return Current drag coefficient
 */
f32 wing_get_drag_coef(void *car);

/* ============================================================================
 * Wing Damage Functions
 * ============================================================================ */

/**
 * wing_damage - Apply damage to wings
 * @param car Pointer to car data structure
 * @param amount Damage amount (0-100)
 *
 * Called on crash or bad landing.
 */
void wing_damage(void *car, s32 amount);

/**
 * wing_repair - Repair wing damage
 * @param car Pointer to car data structure
 *
 * Called when car respawns or enters pit.
 */
void wing_repair(void *car);

/* ============================================================================
 * Wing Sound Effects (defined in sound.h)
 * SFX_WING_DEPLOY   - Wing deploy sound (0x71)
 * SFX_WING_RETRACT  - Wing retract sound (0x72)
 * SFX_WING_WHOOSH   - Wing wind sound (0x73)
 * SFX_WING_DAMAGE   - Wing damage sound (0x74)
 * ============================================================================ */

#endif /* WINGS_H */
