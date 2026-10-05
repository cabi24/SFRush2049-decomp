/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32;
typedef int s32;
typedef short s16;
typedef signed char s8;
typedef unsigned char u8;
typedef f32 Vec3[3];
typedef struct Model { u8 pad[16]; f32 x, y; } Model;
typedef struct Player { u8 pad[896]; Model *model; u8 rest[52]; } Player;
typedef struct Source { u8 pad[112]; Vec3 force[4]; } Source;
typedef struct State {
    Source *source;
    u8 pad4[24];
    Vec3 sum;
    u8 pad40[60];
    Vec3 arm[4];
    u8 pad148[48];
    Vec3 otherArm[4];
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
static void veccopy(f32 *a,f32 *r) { int i; f32 *ap,*rp; ap=a; rp=r; for(i=0;i<3;++i) *rp++=*ap++; }
static void vecadd(register f32 *ap,register f32 *bp,register f32 *rp) { *rp++=*ap++ + *bp++; *rp++=*ap++ + *bp++; *rp++=*ap++ + *bp++; }
static void vecsub(register f32 *ap,register f32 *bp,register f32 *rp) { *rp++=*ap++ - *bp++; *rp++=*ap++ - *bp++; *rp++=*ap++ - *bp++; }
static void crossprod(f32 a[3],f32 b[3],f32 r[3]) { f32 temp[3]; r[0]=a[1]*b[2]; temp[0]=a[2]*b[1]; r[1]=a[2]*b[0]; temp[1]=a[0]*b[2]; r[2]=a[0]*b[1]; temp[2]=a[1]*b[0]; vecsub(r,temp,r); }
void func_800E15A0(State *state) {
    f32 temp[3],temp1[3],*rp,*fp;
    int i;
    f32 gain,factor;
    Player *player;
    Model *model;
    int sign;
    state->sum[0]=0; state->sum[1]=0; state->sum[2]=0;
    for(i=0,rp=state->source->force[0],fp=state->arm[0];i<4;++i,rp+=3,fp+=3) {
        veccopy(rp,temp1);
        temp1[1] += state->delta[i];
        crossprod(fp,temp1,temp);
        vecadd(state->sum,temp,state->sum);
    }
    for(i=0,rp=state->source->force[0],fp=state->otherArm[0];i<4;++i,rp+=3,fp+=3) {
        veccopy(rp,temp1);
        temp1[1]=0.0f;
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
                sign=1;
                state->sum[0] -= model->y * gain * factor;
                if(D_80140A04) sign=-1;
                state->sum[2] += player->model->x * gain * factor * (f32)sign;
            }
        }
    } else state->active=0;
    state->sum[0] += state->extra[0];
    state->sum[1] += state->extra[1];
    state->sum[2] += state->extra[2];
}
