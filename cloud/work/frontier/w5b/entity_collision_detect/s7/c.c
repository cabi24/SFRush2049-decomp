/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef struct Node {struct Node *next;u16 tag;s16 handle,index;u8 opaque10[10];s32 state;} Node;
typedef union Id {s32 handle;struct {s16 high,index;} part;} Id;
typedef struct Vec3 {float x,y,z;} Vec3;
typedef struct Effect {
    Id id;
    float value4;
    u8 opaque8[12];
    float value20;
    u8 opaque24[12];
    float value36;
    Vec3 position;
    u8 alpha,phase,opaque54[2];
    float delay;
} Effect;
typedef struct Car {u8 opaque0[1600];s8 enabled;u8 opaque1601[455];} Car;
typedef struct Player {u8 opaque0[8];Vec3 position;u8 opaque20[932];} Player;
typedef struct RGBA {u8 r,g,b,a;} RGBA;
typedef struct Resource {u8 opaque0[60];RGBA color;u8 opaque64[4];} Resource;
extern s32 D_801170FC;
extern Car D_8014A250[];
extern Player D_80152818[];
extern Effect D_80154FD8[];
extern Resource D_8012E700[];
extern volatile float D_8002EB94;
extern void entity_spawn_callback(s16,s32,s32);
extern void entity_transform_apply(Node *,s32);
void entity_collision_detect(Node *node,s16 update) {
    s32 pad[2];
    s32 phase;
    RGBA color;
    if(D_801170FC) return;
    if(!D_8014A250[node->index].enabled) {
        if(D_80154FD8[node->index].id.handle!=-1) {
            entity_spawn_callback(D_80154FD8[node->index].id.handle,0,0);
            D_80154FD8[node->index].id.handle=-1;
        }
        goto release;
    }
    if(!update) {
release:
        entity_transform_apply(node,1);
        return;
    }
    D_80154FD8[node->index].position.x=D_80152818[node->index].position.x;
    D_80154FD8[node->index].position.y=D_80152818[node->index].position.y;
    D_80154FD8[node->index].position.z=D_80152818[node->index].position.z;
    D_80154FD8[node->index].delay-=D_8002EB94;
    if(D_80154FD8[node->index].delay<=0.0f) {
        D_80154FD8[node->index].value4+=1.5f;
        D_80154FD8[node->index].value20+=1.5f;
        D_80154FD8[node->index].value36+=1.5f;
        phase=D_80154FD8[node->index].phase;
        if(phase>=16) {
            if(D_80154FD8[node->index].id.handle!=-1) {
                entity_spawn_callback(D_80154FD8[node->index].id.handle,0,0);
                D_80154FD8[node->index].id.handle=-1;
            }
            goto release;
        }
        if(phase>=7) {
            color.r=255;
            color.g=255;
            color.b=255;
            color.a=D_80154FD8[node->index].alpha;
            phase=D_80154FD8[node->index].id.part.index;
            D_8012E700[phase].color=color;
            if(D_80154FD8[node->index].alpha>=25) {
                D_80154FD8[node->index].alpha-=24;
            }
        }
        D_80154FD8[node->index].phase++;
        D_80154FD8[node->index].delay=0.0333333f;
    }
    return;
}
