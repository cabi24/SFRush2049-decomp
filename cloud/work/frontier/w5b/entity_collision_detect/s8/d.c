/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* entity_collision_detect: per-frame update of a growing smoke/dust puff
 * attached to a scene node (name is a historical label). node->index selects
 * the car (D_8014A250, 2056-byte records), the player position source
 * (D_80152818, 952-byte records) and the puff record (D_80154FD8, 60 bytes).
 *  - paused (D_801170FC) -> nothing;
 *  - car disabled -> kill the puff's sound/effect handle (entity_spawn_callback
 *    (handle,0,0), handle = -1) and release the node (entity_transform_apply);
 *  - update == 0 -> release the node;
 *  - otherwise the puff follows the player position; every 1/30 s (delay
 *    counts down by the volatile frame time D_8002EB94) its scale matrix
 *    diagonal grows by 1.5; after phase 16 it is killed and released; from
 *    phase 7 its colour (255,255,255,alpha) is written to the resource record
 *    D_8012E700[id.index] and alpha fades by 24 while >= 25.
 * Shaping: every access is written D_80154FD8[node->index].field (no Effect
 * pointer local): uopt keeps the address as one expression web in v0 and
 * re-forms it after the alpha store (the shift-form x*60 on that edge).
 * `phase` is one variable reused for the resource index (v1). The frame needs
 * 8 bytes of unreferenced local storage (pad[2], declared first); the
 * original probably had an unused aggregate local. D_8002EB94 is volatile
 * (lui/addiu/lwc1 0(reg)). Own .rodata: 0.0333333f (0x3D088880 at 0x801239CC).
 * No arcade ancestor identified.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct Node {struct Node *next;u16 tag;s16 handle,index;u8 opaque10[10];s32 state;} Node;
typedef union Id {s32 handle;struct {s16 high,index;} part;} Id;
typedef struct Vec3 {f32 x,y,z;} Vec3;
typedef struct Effect {
    Id id;
    f32 matrix[3][3];
    Vec3 position;
    u8 alpha,phase,opaque54[2];
    f32 delay;
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
extern volatile f32 D_8002EB94;
extern void entity_spawn_callback(s16,s32,s32);
extern void entity_transform_apply(Node *,s32);
void entity_collision_detect(Node *node,s16 update) {
    s32 pad[4];
    s32 phase;
    Color color;
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
        D_80154FD8[node->index].matrix[0][0]+=1.5f;
        D_80154FD8[node->index].matrix[1][1]+=1.5f;
        D_80154FD8[node->index].matrix[2][2]+=1.5f;
        phase=D_80154FD8[node->index].phase;
        if(phase>=16) {
            if(D_80154FD8[node->index].id.handle!=-1) {
                entity_spawn_callback(D_80154FD8[node->index].id.handle,0,0);
                D_80154FD8[node->index].id.handle=-1;
            }
            goto release;
        }
        if(phase>=7) {
            color.component.red=255;
            color.component.green=255;
            color.component.blue=255;
            color.component.alpha=D_80154FD8[node->index].alpha;
            phase=D_80154FD8[node->index].id.part.index;
            D_8012E700[phase].color=color.word;
            if(D_80154FD8[node->index].alpha>=25) {
                D_80154FD8[node->index].alpha-=24;
            }
        }
        D_80154FD8[node->index].phase++;
        D_80154FD8[node->index].delay=0.0333333f;
    }
    return;
}
