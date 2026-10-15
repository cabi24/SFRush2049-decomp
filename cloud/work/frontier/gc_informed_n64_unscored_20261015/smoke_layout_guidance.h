/* Reference-only N64 context proposal, not an applied matching source.
 * Assumes 32-bit N64 pointers/int and 32-bit IEEE floats.
 * GC DWARF supplies historical names; N64 compiled accesses supply offsets.
 * +0x38 is independently untyped on N64 in the inspected paths.
 */
#ifndef RESEARCH_SMOKE_LAYOUT_GUIDANCE_H
#define RESEARCH_SMOKE_LAYOUT_GUIDANCE_H

typedef struct SmokeParticle SmokeParticle;
struct SmokeParticle {
    SmokeParticle *next;       /* 0x00 */
    float basis[3][3];         /* 0x04 */
    float pos[3];              /* 0x28 */
    int mbIdx;                 /* 0x34: sign-extended 16-bit instance index */
    unsigned int unknown38;    /* 0x38: GC float tlifeTime; N64 use unconfirmed */
    float lifeTime;            /* 0x3C: countdown OR crash target scale */
    float swTime;              /* 0x40: animation countdown */
    float vel[3];              /* 0x44: velocity OR attachment offset */
    unsigned int data;         /* 0x50 */
    signed short slot;         /* 0x54 */
    signed short tIdx;         /* 0x56: signed; -1 is used */
};                            /* 0x58 */

typedef struct SmokePool {
    unsigned char dbl;         /* 0x00 */
    unsigned char pad01[3];
    int numElements;           /* 0x04: capacity */
    int elementSize;           /* 0x08 */
    void *list;                /* 0x0C: backing array */
    SmokeParticle *head;       /* 0x10: active list */
    SmokeParticle *store;      /* 0x14: free list */
} SmokePool;                   /* 0x18 */

enum SmokeKind {
    SMOKE_TIRE_0 = 0, SMOKE_TIRE_1 = 1,
    SMOKE_TIRE_2 = 2, SMOKE_TIRE_3 = 3,
    SMOKE_BLAST_FREE = 4, SMOKE_CRASH_ATTACHED = 5,
    SMOKE_WING_0 = 6, SMOKE_WING_1 = 7
};

/* Particle data flags: separate from the per-car masks below. */
#define SMOKE_ANIMATE_FORWARD 0x00000001u
#define SMOKE_WATER_ATTACHED  0x00000010u
#define SMOKE_KIND_SHIFT      17
#define SMOKE_KIND_MASK       0x001E0000u

/* Per-car masks in D_801392D8[6]. Names are proposals, values observed. */
#define CAR_SMOKE_TIRE_DIRECTION(tire) (1u << (tire))
#define CAR_SMOKE_BLAST_DIRECTION     0x00000010u
#define CAR_SMOKE_BLAST_COLOR_STARTED 0x00000020u
#define CAR_SMOKE_BLAST_SKIP_A        0x00000040u
#define CAR_SMOKE_BLAST_SKIP_B        0x00000080u
#define CAR_SMOKE_WATER_ACTIVE(tire)  (0x100u << (tire))
#define CAR_SMOKE_WING_DIRECTION_0    0x00001000u
#define CAR_SMOKE_WING_DIRECTION_1    0x00002000u
#define CAR_SMOKE_WING_PREVIOUS_0     0x00004000u
#define CAR_SMOKE_WING_PREVIOUS_1     0x00008000u
#define CAR_SMOKE_BLAST_SKIP_C_N64    0x00080000u

/* Existing accepted signatures, expressed with recovered meanings.
 * Preserve original group source/context when scoring: these declarations
 * are documentation and are not an instruction to rename linked symbols.
 */
void entity_spawn_init(signed short slot, unsigned int kind,
                       int water, float *smokeOffset);
void func_8008E408(signed short slot, unsigned int tire);
void physics_collision_test(void);
void func_800CB718(signed short slot);
void func_80092484(signed short slot, signed short side);

#endif
