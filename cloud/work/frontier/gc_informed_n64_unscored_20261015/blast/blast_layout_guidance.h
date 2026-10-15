/* Reference only. Excluded from group files: no new ABI or score is asserted.
 * Sizes/offsets assume N64 O32: 4-byte pointers, int and IEEE float.
 * GC DWARF names are hypotheses for N64; target accesses decide signedness.
 */
#ifndef TASK48_BLAST_LAYOUT_GUIDANCE_H
#define TASK48_BLAST_LAYOUT_GUIDANCE_H
typedef union BlastTransform48 {
    struct { float basis[3][3]; float pos[3]; } mat3;
    float matrix[4][3];
} BlastTransform48;

typedef union BlastColor4 {
    unsigned char c[4];
    unsigned int rgba;
} BlastColor4;

typedef struct BlastVisual24 BlastVisual24;
struct BlastVisual24 {
    BlastVisual24 *next;                        /* +00 */
    signed short index;                         /* +04: baseline count */
    signed short objnum;                        /* +06: baseline id */
    signed short slot;                          /* +08: baseline state */
    unsigned int data;                          /* +0C: GC type, not used here */
    float timeStamp;                           /* +10: baseline value */
    void (*func)(BlastVisual24 *, signed short); /* +14: callback */
};

typedef struct BlastObject84 {
    int mbIdx;                                /* +00 */
    BlastTransform48 mat;                      /* +04 */
    float scale, scaleInc;                     /* +34, +38 */
    float yaw, yawInc, pitch, pitchInc;         /* +3C..+48 */
    unsigned char alpha, alphaInc;             /* +4C,+4D */
    signed short unused;                       /* +4E: actual record member */
    float ht;                                  /* +50 */
} BlastObject84;

typedef struct BlastAggregate400 {
    BlastObject84 blastObjs[4];                 /* +000..+14F */
    int glowIdx;                               /* +150 */
    BlastTransform48 mat;                      /* +154 */
    unsigned char alpha, alphaCnt, ralpha, ralphaCnt; /* +184..+187 */
    float lifeTime, incTime;                    /* +188,+18C */
} BlastAggregate400;

/* StartBlast's historical local is BlastTransform48 *mat: a four-byte
 * pointer, with a 48-byte pointee. It is not float mat[12] on the stack.
 * Preserve direct signed slot reloads and the static initializer's signed
 * short parameter; preserve Color.rgba word copies and native resource gates.
 */
#endif
