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
#define CROSS(a,b,r) do { (r).x = (a).y*(b).z-(b).y*(a).z; (r).y=(a).z*(b).x-(b).z*(a).x; (r).z=(a).x*(b).y-(b).x*(a).y; } while(0)
#define ADD(a,b) do { (a).x=(b).x+(a).x; (a).y=(b).y+(a).y; (a).z=(b).z+(a).z; } while(0)
#define POINT(n) do { force=state->source->force[n]; force.y=0.0f; CROSS(state->otherArm[n],force,torque); ADD(state->sum,torque); } while(0)
void func_800E15A0(State *state) {
    Vec3 force;
    Vec3 torque;
    s32 i;
    f32 gain, factor;
    Player *player;
    Model *model;
    s32 sign;
    state->sum.x=0.0f;
    state->sum.y=0.0f;
    state->sum.z=0.0f;
    for(i=0;i<4;i++) {
        force=state->source->force[i];
        force.y += state->delta[i];
        CROSS(state->arm[i],force,torque);
        ADD(state->sum,torque);
    }
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
