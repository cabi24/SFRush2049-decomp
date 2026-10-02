/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef float f32;
typedef int s32;
typedef short s16;
typedef signed char s8;
typedef unsigned char u8;
typedef struct Vec3 { f32 x, y, z; } Vec3;
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
#define CROSS(a,b,r) do { (r)[0]=(a).y*(b)[2]-(b)[1]*(a).z; (r)[1]=(a).z*(b)[0]-(b)[2]*(a).x; (r)[2]=(a).x*(b)[1]-(b)[0]*(a).y; } while(0)
#define ADD(a,b) do { (a).x=(b)[0]+(a).x; (a).y=(b)[1]+(a).y; (a).z=(b)[2]+(a).z; } while(0)
#define POINT(n) do { force[0]=src[n].x; force[1]=src[n].y; force[2]=src[n].z; force[1]=0.0f; CROSS(state->otherArm[n],force,torque); ADD(state->sum,torque); } while(0)
void func_800E15A0(State *state) {
    f32 torque[3];
    f32 force[3];
    Vec3 *src;
    Vec3 *arm;
    f32 *delta;
    s32 i;
    f32 gain, factor;
    Player *player;
    Model *model;
    s32 sign;
    state->sum.x=0.0f;
    state->sum.y=0.0f;
    state->sum.z=0.0f;
    src=state->source->force;
    arm=state->arm;
    i=0;
    do {
        force[0]=src->x;
        force[1]=src->y;
        force[2]=src->z;
        force[1] += state->delta[i++];
        CROSS(*arm,force,torque);
        ADD(state->sum,torque);
        src++;
        arm++;
    } while(i<4);
    src=state->source->force;
    POINT(0);
    POINT(1);
    POINT(2);
    POINT(3);
    if (!(state->flags & 16) && state->check[0]>5.0f && state->check[1]>5.0f && state->check[2]>5.0f && state->check[3]>5.0f && gameplay_mode != 6) {
        if(state->enabled) state->active=1; else state->active=0;
        if(state->active) {
            player=&player_array[state->player];
            model=player->model;
            if(model) {
                gain=state->gain;
                factor=state->factor;
                sign=1;
                state->sum.x -= model->y * gain * factor;
                if(D_80140A04) sign=-1;
                state->sum.z += player->model->x * gain * factor * (f32)sign;
            }
        }
    } else state->active=0;
    state->sum.x += state->extra.x;
    state->sum.y += state->extra.y;
    state->sum.z += state->extra.z;
}
