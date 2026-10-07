/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned char u8;typedef signed char s8;typedef short s16;typedef unsigned short u16;typedef unsigned int u32;typedef float f32;
typedef struct Node {struct Node *next;s16 count,id,state;u8 other10[2];void *object;f32 value;void (*callback)(struct Node *,s16);} Node;
typedef union Color {u32 word;struct {u8 r,g,b,a;} c;} Color;
typedef union Handle {int word;struct {s16 high,low;} half;} Handle;
typedef struct Particle {Handle handle;f32 matrix[3][3],pos[3],size,stretch,zero60,spin_y,zero68,spin_z;u8 alpha,timer,other78[2];f32 zero80;} Particle;
typedef struct ExtraParticle {int handle;f32 matrix[3][3],pos[3];u8 alpha,flag,other54[2];f32 delay,time;} ExtraParticle;
typedef struct ParticlePool {Particle particles[4];ExtraParticle extra;} ParticlePool;
typedef struct Car952 {u8 other0[8];f32 pos[3];u8 other20[932];} Car952;
typedef struct Model2056 {u8 other0[1600];s8 active;u8 other1601[455];} Model2056;
typedef struct Object68 {u8 other0[8];void *data;f32 scale,scale2;u16 texture;u8 other22[38];Color color;u8 other64[4];} Object68;
extern Node *D_801391F0;
extern ParticlePool D_80154660[];
extern Car952 player_array[];
extern Model2056 D_8014A250[];
extern Object68 D_8012E700[];
extern u16 D_801427C0[];
extern u16 D_80142904,D_80142908[];
extern f32 D_8011418C[3][3];
extern f32 D_801543CC,D_801239A8,D_801239AC,D_801239B0,D_801239B4,D_801239B8,D_801239BC,D_801239C0,D_801239C4,D_801239C8,D_8002EB94;
extern Color D_8011B550;
extern u32 D_8011735C;
extern s16 active_player_count;
extern s8 D_80156994,D_8014978C;
extern int D_801170FC;
Node *func_80090284(void);
void func_80090FEC(Node *,s16);
void entity_spawn_callback(s16,int,int);
void entity_transform_apply(Node *,int);
void math_utility(f32 [3][3],f32 [3][3]);
int func_8008E26C(u16,f32 [3][3],int,u32);
void entity_spawn_init(s16,int,int,int);
void func_8038A408(f32 *);
#define RANDOM15 (((D_8011735C=D_8011735C*0x41C64E6D+12345)>>16)&0x7fff)
void func_80090308(s16 car);
void entity_physics_update(Node *node,s16 update)
{
    if (D_801170FC) return;
    if (!update) {
remove_node:
        entity_spawn_callback(node->id,0,0);
        node->id=-1;
        entity_transform_apply(node,1);
        return;
    }
    if (node->state>=0) {
        if(!D_8014A250[node->state].active) goto remove_node;
        ((f32 *)D_8012E700[node->id].data)[9]=player_array[node->state].pos[0];
        ((f32 *)D_8012E700[node->id].data)[10]=player_array[node->state].pos[1];
        ((f32 *)D_8012E700[node->id].data)[11]=player_array[node->state].pos[2];
    }
    node->value-=*(f32 *)(u32)&D_8002EB94;
    if (node->state<0 || (node->state>=0 && node->value<=0.0f)) {
        node->value=D_801239C8;
        if (node->count==30) {
            goto remove_node;
        }
        D_8012E700[node->id].texture=D_80142908[node->count];
        node->count++;
        if (node->state>=0) {
            if (active_player_count<4 && (D_80156994 || D_8014978C>=6) && node->count==4) func_8038A408(player_array[node->state].pos);
            if (node->count==3) func_80090308(node->state);
        }
    }
}
