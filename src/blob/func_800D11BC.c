/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800D11BC: N64 port of arcade game/drivsym.c syminit(MODELDAT *m):
 *   initialize(m) = func_800D1004, road(m) = func_800CF06C, positions(m) = func_800D0424,
 *   then mcommunication(m) (game/communic.c) reduced to its rpm line and written inline:
 *   m->rpm = (S16)(m->engangvel * rdpstorpm * .9).  rdpstorpm is the single-precision
 *   9.549305f (retail word 0x4118C9F4); .9f is 0x3F666666.  The arcade's mph line is gone.
 * N64 addition before road(): a four-float record at 0x720 is set to (0, 0, 0, 1) (an identity
 * quaternion, by the values) and the byte at 0x730 to 1.  Field names are hypotheses.
 *
 * SHAPING (disclosed): the quaternion and the byte flag are declared `volatile`.  Retail keeps the
 * four float stores, the byte store and the road() argument move in source order (as1 does not
 * hoist `li/sb/move a0` into the mtc1 latency slots); with plain fields as1 reorders them (7 words).
 * Proven with the listing oracle: the plain listing reproduces retail only when the byte store cannot
 * pass the float stores.  Volatile on the floats alone leaves 5 words; both are needed.
 * Unit check: blob_unit --tag w20f score func_800D11BC --with cloud/matches/func_800D11BC.c --neighbours
 *   -> EQUAL, locked bodies that differ in this unit: 0.
 */
typedef signed char s8;
typedef short s16;
typedef float f32;
typedef struct MODELDAT {
    char pad0[1032];
    f32 engangvel;              /* 0x408 */
    char pad1036[1824 - 1036];
    volatile f32 quat[4];       /* 0x720 */
    volatile s8 quatvalid;      /* 0x730 */
    char pad1841[2000 - 1841];
    s16 rpm;                    /* 0x7D0 */
} MODELDAT;
void func_800D1004(MODELDAT *m);
void func_800CF06C(MODELDAT *m);
void func_800D0424(MODELDAT *m);
void func_800D11BC(MODELDAT *m)
{
    func_800D1004(m);
    m->quat[0] = 0.0f;
    m->quat[1] = 0.0f;
    m->quat[2] = 0.0f;
    m->quat[3] = 1.0f;
    m->quatvalid = 1;
    func_800CF06C(m);
    func_800D0424(m);
    m->rpm = (s16)(m->engangvel * 9.549305f * .9f);
}
