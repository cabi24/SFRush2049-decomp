/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
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
typedef struct Resource {u8 opaque0[60];u32 color;u8 opaque64[4];} Resource;
typedef union Color {struct {u8 red,green,blue,alpha;} component;u32 word;} Color;
extern s32 D_801170FC;
extern Car D_8014A250[];
extern Player D_80152818[];
extern Effect D_80154FD8[];
extern Resource D_8012E700[];
extern float D_8002EB94,D_801239CC;
extern void entity_spawn_callback(s16,s32,s32);
extern void entity_transform_apply(Node *,s32);
void entity_collision_detect(Node *node,s16 update) {
    s32 handle,phase;
    u32 color;
    Effect *effect;
    if(D_801170FC) return;
    if(!D_8014A250[node->index].enabled) {
        handle=D_80154FD8[node->index].id.handle;
        if(handle!=-1) {
            entity_spawn_callback((s16)handle,0,0);
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
        effect=&D_80154FD8[node->index];
        phase=effect->phase;
        if(phase>=16) {
            handle=effect->id.handle;
            if(handle!=-1) {
                entity_spawn_callback((s16)handle,0,0);
                D_80154FD8[node->index].id.handle=-1;
            }
            goto release;
        }
        if(phase>=7) {
            ((u8 *)&color)[0]=255;
            ((u8 *)&color)[1]=255;
            ((u8 *)&color)[2]=255;
            ((u8 *)&color)[3]=D_80154FD8[node->index].alpha;
            D_8012E700[D_80154FD8[node->index].id.part.index].color=color;
            effect=&D_80154FD8[node->index];
            if(effect->alpha>=25) {
                effect->alpha-=24;
                effect=&D_80154FD8[node->index];
            }
        }
        effect->phase++;
        D_80154FD8[node->index].delay=D_801239CC;
    }
    return;
}
