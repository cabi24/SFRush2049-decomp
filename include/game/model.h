/**
 * model.h - Catch-up and model control for Rush 2049 N64
 *
 * Based on arcade: model.h and model.c
 * Implements rubber-band catchup algorithm for competitive racing.
 */

#ifndef MODEL_H
#define MODEL_H

#include "types.h"
#include "game/checkpoint.h"  /* For MAX_TRACKS, MAX_LINKS */

/* ========================================================================
 * Catch-up Constants (arcade model.c)
 * ======================================================================== */

/**
 * cuzone - Catch-up zone in feet behind leader per track
 * cuscale - Maximum catch-up boost as fraction per track
 * cusolo - Solo player catch-up compensation
 */
extern const f32 cuzone[MAX_TRACKS];
extern const f32 cuscale[MAX_TRACKS];
extern const f32 cusolo;

/* ========================================================================
 * Function Prototypes (arcade model.c)
 * ======================================================================== */

/**
 * set_catchup - Rubber-band catch-up algorithm
 * Based on arcade: model.c:set_catchup()
 */
void set_catchup(void);

/**
 * no_catchup - Disable catch-up for tournament mode
 * Based on arcade: model.c:no_catchup()
 */
void no_catchup(void);

/**
 * avg_catchup - Apply average catch-up for solo player
 */
void avg_catchup(void);

/* Wrapper functions */
void model_set_catchup(void);
void model_no_catchup(void);

#endif /* MODEL_H */
