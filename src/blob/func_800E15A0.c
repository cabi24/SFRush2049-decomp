/* func_800E15A0: sum tire and body torques, apply airborne controller
 * correction, and add the center moment.
 * Arcade: reference/repos/rushtherock/game/drivsym.c:torques;
 * vector spelling from reference/repos/rushtherock/game/vecmath.h.
 * N64 uses its vertical Y axis, parameter-held tire arms, and an added
 * per-player airborne steering correction. Tier: portable physics with
 * native field layout adaptation.
 * Exact flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul.
 * Ordinary O3 also matches; no helper, local-pool, or hidden ABI dependency.
 */
typedef float f32;
typedef int s32;
typedef short s16;
typedef signed char s8;
typedef unsigned char u8;
typedef f32 Vec3[3];
typedef struct Model { u8 pad[16]; f32 x, y; } Model;
typedef struct Player { u8 pad[896]; Model *model; u8 rest[52]; } Player;
typedef struct Parameters { u8 pad[112]; Vec3 tireArm[4]; } Parameters;
typedef struct State {
    Parameters *parameters;
    u8 pad4[24];
    Vec3 sum;
    u8 pad40[60];
    Vec3 tireForce[4];
    u8 pad148[48];
    Vec3 bodyForce[4];
    u8 pad244[72];
    Vec3 extra;
    u8 pad328[680];
    f32 factor;
    u8 pad1012[4];
    s16 active;
    u8 pad1018[446];
    f32 gain;
    u8 pad1468[16];
    f32 delta[4];
    u8 pad1500[16];
    f32 check[4];
    u8 pad1532[309];
    s8 enabled;
    u8 pad1842[148];
    s16 player;
    u8 pad1992[12];
    s32 flags;
} State;
extern s32 gameplay_mode;
extern Player player_array[];
extern s8 D_80140A04;
/* Native Y is the arcade vertical Z; vectors follow native axes.
 * Arcade ancestry: drivsym.c:torques, vecmath.h vector macros.
 * N64 adds four-body unrolling and controller correction. */
#define veccopy(a,r) {r[0]=a[0]; r[1]=a[1]; r[2]=a[2];}
#define crossprod(a,b,r) {r[0]=a[1]*b[2]-a[2]*b[1]; r[1]=a[2]*b[0]-a[0]*b[2]; r[2]=a[0]*b[1]-a[1]*b[0];}
#define vecadd(a,b,r) {r[0]=a[0]+b[0]; r[1]=a[1]+b[1]; r[2]=a[2]+b[2];}
void func_800E15A0(State *state) {
    f32 temp[3],temp1[3],*rp,*fp;
    int i;
    f32 gain,factor;
    Player *player;
    Model *model;
    int sign;
    state->sum[0]=0; state->sum[1]=0; state->sum[2]=0;
    for(i=0,rp=state->parameters->tireArm[0],fp=state->tireForce[0];i<4;++i,rp+=3,fp+=3) {
        veccopy(rp,temp1);
        temp1[1] += state->delta[i];
        crossprod(fp,temp1,temp);
        vecadd(state->sum,temp,state->sum);
    }
    for(i=0,rp=state->parameters->tireArm[0],fp=state->bodyForce[0];i<4;++i,rp+=3,fp+=3) {
        veccopy(rp,temp1);
        temp1[1]=0;
        crossprod(fp,temp1,temp);
        vecadd(state->sum,temp,state->sum);
    }
    if (!(state->flags & 16) && state->check[0]>5.0f && state->check[1]>5.0f && state->check[2]>5.0f && state->check[3]>5.0f && gameplay_mode != 6) {
        if(state->enabled) state->active=1; else state->active=0;
        if(state->active) {
            player=&player_array[state->player];
            model=player->model;
            if(model) {
                gain=state->gain;
                factor=state->factor;
                state->sum[0] -= model->y * gain * factor;
                if(D_80140A04) sign=-1; else sign=1;
                state->sum[2] += player->model->x * gain * factor * (f32)sign;
            }
        }
    } else state->active=0;
    state->sum[0] += state->extra[0];
    state->sum[1] += state->extra[1];
    state->sum[2] += state->extra[2];
}

/* Complete scanner/closure extent: 0x800E15A0..0x800E1AA0, 1,280 bytes.
 * Native ABI is one ordinary model pointer, with a 24-byte leaf frame.
 * parameters->tireArm starts at owner+112; tireForce is state+100 and
 * bodyForce is state+196. Both passes compute cross(force,arm) in the
 * native coordinate system. delta changes the arm's vertical Y coordinate.
 * The parameter tire-arm vector is copied to temp1 before each operation.
 * temp and temp1 are the two consumed native three-float arrays.
 * Native N64 correction is reconstructed from its actual controller lookup,
 * player stride 952, model pointer+896 and original condition/store order.
 * Integer zero shares the initial FP zero across both moment passes; the
 * explicit signed direction selection preserves the native branch shape.
 * No standalone vector-helper ABI or source-owned pool is required.
 */
