/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * STATE: code identical, own-rodata unverified (not strict, not spliceable
 * until the scorer/splicer can place a function's own .rodata):
 *   MATCH (6 section-relative relocations unverified: .rodata+0x0 at +0x10,
 *   .rodata+0x0 at +0x74, .rodata+0x4 at +0x8c, .rodata+0x4 at +0x90,
 *   .rodata+0x8 at +0x170, .rodata+0x8 at +0x1b0)
 * The three literals are, in order, 0.1f, 0.025f, .025f = retail words at
 * 0x801243B4/B8/BC (3DCCCCCD, 3CCCCCCD, 3CCCCCCD), which are the addresses the
 * retail HI16/LO16 pairs at those sites encode.
 *
 * Arcade ancestor: velocities(MODELDAT *m) in
 * reference/repos/rushtherock/game/drivsym.c (V += A*dt, clamp |V|, W += AA*dt),
 * with N64 additions: axis stop under light brake, 300 fps clamp with early
 * return, angular damping, quadratic angular drag when all four tires touch
 * or the car slides (coefficients D_80114184 / D_80114188 in .data), and
 * low-speed stops. func_8008B3C8 = vector magnitude.
 *
 * Shaping quirks (each one is needed):
 *  - `0.025f` and `.025f` are spelled differently on purpose: uopt identifies
 *    float constants by spelling, so the same spelling twice becomes one
 *    web that is loaded early and reloaded after the call (retail has two
 *    separate rodata words and two separate loads);
 *  - the contact test compares against the int literal 0 (separate zero
 *    register f0), every other zero is 0.0f;
 *  - vecadd operand order is the arcade's (`V + temp`, `W + temp`);
 *  - the drag factor is built in `speed` in two steps with an if/else for the
 *    coefficient, and is a different variable from the clamp's `velfact`.
 * An `extern f32 D_801243B8` cannot replace the middle literal: it is
 * reloaded after the store to V[0] (195 words differ).
 * Prior attempts: cloud/work/ipa-groups/codex_velocities_* (202/237).
 */
typedef unsigned char u8;typedef signed char s8;typedef short s16;typedef float f32;
typedef struct ModelView {
    u8 other0[40];f32 A[3],AA[3],V[3],W[3];
    u8 other88[676];f32 up;u8 other768[208];f32 brake,throttle;
    u8 other984[24];f32 magvel;u8 other1012[504];f32 contact[4];
    u8 other1532[56];f32 dt;u8 other1592[8];s8 sliding;
    u8 other1601[15];f32 damping;u8 other1620[370];s16 node;
} ModelView;
typedef struct Car952 {u8 other0[857];s8 mode;u8 other858[94];} Car952;
extern Car952 D_80152818[];
extern f32 D_80114184,D_80114188;
extern f32 fabsf(f32),func_8008B3C8(f32 *);
#pragma intrinsic (fabsf)
void func_800E114C(ModelView *m)
{
    f32 temp[3],velfact,speed;f32 coef;
    temp[0]=m->A[0]*m->dt;
    temp[1]=m->A[1]*m->dt;
    temp[2]=m->A[2]*m->dt;
    m->V[0]=m->V[0]+temp[0];
    m->V[1]=m->V[1]+temp[1];
    m->V[2]=m->V[2]+temp[2];
    if(m->brake<0.1f) {
        if(fabsf(m->V[0])<0.025f)m->V[0]=0.0f;
        if(fabsf(m->V[2])<0.025f)m->V[2]=0.0f;
    }
    if(D_80152818[m->node].mode!=2)m->magvel=func_8008B3C8(m->V);
    if(m->magvel>300.0f) {
        velfact=300.0f/m->magvel;
        m->V[0]*=velfact;
        m->V[1]*=velfact;
        m->V[2]*=velfact;
        return;
    }
    temp[0]=m->AA[0]*m->dt;
    temp[1]=m->AA[1]*m->dt;
    temp[2]=m->AA[2]*m->dt;
    if(m->damping!=0.0f) {
        temp[0]-=m->W[0]*.025f;
        temp[1]-=m->W[1]*.025f;
        temp[2]-=m->W[2]*.025f;
    }
    m->W[0]=m->W[0]+temp[0];
    m->W[1]=m->W[1]+temp[1];
    m->W[2]=m->W[2]+temp[2];
    if((m->contact[0]>0 && m->contact[1]>0 && m->contact[2]>0 && m->contact[3]>0) || m->sliding) {
        speed=func_8008B3C8(m->W);
        speed=speed*speed;
        if(m->sliding)coef=D_80114188;else coef=D_80114184;
        speed=speed*coef;
        m->W[0]-=m->W[0]*speed;
        m->W[1]-=m->W[1]*speed;
        m->W[2]-=m->W[2]*speed;
    }
    if(m->magvel<3.0f && (m->throttle>0.5f || m->up<0.0f)) {
        m->V[0]=0.0f;
        m->V[2]=0.0f;
    }
    if(m->magvel<3.0f && m->up< -0.75f) {
        if(func_8008B3C8(m->W)<0.5f) {
            m->W[0]=0.0f;
            m->W[1]=0.0f;
            m->W[2]=0.0f;
        }
    }
}
