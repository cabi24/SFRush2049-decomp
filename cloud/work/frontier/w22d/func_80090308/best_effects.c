/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * w22d draft (NOT A MATCH): func_80090308 (internal, 282 words) + entity_physics_update (kept, 178 words), with
 * save_write_data (best_swd.c) as the second real caller. Score with
 *   blob_unit --tag w22d score func_80090308 entity_physics_update save_write_data --with best_effects.c
 *     --with best_swd.c --internal func_80090308 --neighbours      (locked bodies that differ: 0)
 * State: entity_physics_update 7 rows (colour only, see RESULTS); func_80090308 frame 168 vs 272 (299 rows);
 *   save_write_data 153 rows, frame 192 vs 352.
 * Identified: func_80090770 (locked, 0x80090770) is the texture setter entity_physics_update inlines;
 *   func_8008E06C (locked) is the colour setter func_80090308 inlines; func_8008B2E4 (locked) is the
 *   random-float helper (4x). puff_init is an `unknown identity` helper (shaping hypothesis).
 */
typedef unsigned char u8;typedef signed char s8;typedef short s16;typedef unsigned short u16;typedef unsigned int u32;typedef int s32;typedef float f32;
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
extern f32 D_801543CC;
extern volatile f32 D_8002EB94;
extern Color D_8011B550;
extern u32 D_8011735C;
extern s16 active_player_count;
extern s8 D_80156994,D_8014978C;
extern int D_801170FC;
Node *func_80090284(void);
void func_80090FEC(Node *,s16);
void entity_spawn_callback(s16,int,int);
void entity_transform_apply(void *,s32);
void math_utility(void *,void *);
s32 func_8008E26C(s32,void *,s16,s32);
void entity_spawn_init(s16,u32,s32,f32 *);
void func_8038A408(f32 *);
void func_8008E06C(s16 id,Color *color);
void func_80090770(s16 index,u16 value);
f32 func_8008B2E4(f32 max);
#define RANDOM15 (((D_8011735C=D_8011735C*0x41C64E6D+12345)>>16)&0x7fff)
static void puff_init(Particle *p, Car952 *vehicle, u16 texture, Color *color)
{
    f32 a,b,c;
    if (p->handle.word!=-1) entity_spawn_callback((s16)p->handle.word,0,0);
    math_utility(D_8011418C,p->matrix);
    p->pos[0]=vehicle->pos[0];
    p->pos[1]=vehicle->pos[1]-3.0f;
    p->pos[2]=vehicle->pos[2];
    p->handle.word=func_8008E26C(texture,p->matrix,-1,0x42000);
    p->size=0.4f;
    color->c.a=128;
    p->matrix[0][0]=p->matrix[1][1]=p->matrix[2][2]=p->size;
    p->zero80=0.0f;
    p->zero60=0.0f;
    p->zero68=0.0f;
    p->alpha=128;
    a=func_8008B2E4(0.005f);
    b=func_8008B2E4(0.2f);
    c=func_8008B2E4(0.2f);
    p->stretch=a+0.05f;
    p->spin_y=0.1f-b;
    p->spin_z=0.1f-c;
    p->timer=(int)func_8008B2E4(10.0f)+25;
    func_8008E06C(p->handle.half.low,color);
}

void func_80090308(s16 car)
{
    Node *node;
    Particle *p;
    Car952 *vehicle;
    u16 *texture;
    Color color;
    color=D_8011B550;
    node=func_80090284();
    if (node) {
        node->callback=func_80090FEC;
        node->value=D_801543CC;
        node->state=car;
        color.c.r=244;color.c.g=205;color.c.b=20;
        p=D_80154660[car].particles;
        vehicle=&player_array[car];
        texture=&D_801427C0[158];
        do {
            puff_init(p,vehicle,*texture,&color);
            p++;texture++;
        } while (texture!=&D_801427C0[162]);
        if (active_player_count<4 && (D_80156994 || D_8014978C>=6)) {
            ExtraParticle *extra=&D_80154660[car].extra;
            math_utility(D_8011418C,extra->matrix);
            extra->pos[0]=vehicle->pos[0];extra->pos[1]=vehicle->pos[1];extra->pos[2]=vehicle->pos[2];
            if (extra->handle!=-1) entity_spawn_callback((s16)extra->handle,0,0);
            extra->handle=func_8008E26C(D_80142904,extra->matrix,-1,0x52000);
            extra->alpha=95;extra->flag=0;
            D_8012E700[extra->handle].scale=0.2f;
            D_8012E700[extra->handle].scale2=1.4f;
            color.c.r=color.c.g=color.c.b=255;color.c.a=95;
            D_8012E700[extra->handle].color=color;
        }
        D_80154660[car].extra.delay=0.7f;
        D_80154660[car].extra.time=0.0333333015f;
        node->next=D_801391F0;
        D_801391F0=node;
    }
    if (active_player_count<4 && (D_80156994 || D_8014978C>=6)) entity_spawn_init(car,5,0,0);
}
void entity_physics_update(Node *node,s16 update)
{
    int slot;
    if (D_801170FC) return;
    if (!update) {
kill:
        entity_spawn_callback(node->id,0,0);
        node->id=-1;
        entity_transform_apply(node,1);
        return;
    }
    slot=node->state;
    if (slot>=0) {
        if (!D_8014A250[slot].active) goto kill;
        ((f32 *)D_8012E700[node->id].data)[9]=player_array[slot].pos[0];
        ((f32 *)D_8012E700[node->id].data)[10]=player_array[node->state].pos[1];
        ((f32 *)D_8012E700[node->id].data)[11]=player_array[node->state].pos[2];
        slot=node->state;
    }
    node->value-=D_8002EB94;
    if (slot<0 || (slot>=0 && node->value<=0.0f)) {
        node->value=0.0333333015f;
        if (node->count==30) goto kill;
        func_80090770(node->id,D_80142908[node->count]);
        node->count++;
        if (slot>=0) {
            if (active_player_count<4 && (D_80156994 || D_8014978C>=6) && node->count==4) func_8038A408(player_array[slot].pos);
            if (node->count==3) func_80090308(node->state);
        }
    }
}
