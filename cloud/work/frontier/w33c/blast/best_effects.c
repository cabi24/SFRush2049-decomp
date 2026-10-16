/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * w33c draft (NOT A MATCH): func_80090308 = GC StartNewBlast (visuals.c, static void StartNewBlast(s16 slot)),
 * entity_physics_update = GC AnimateBlast (unchanged from the PR #379 natural effects.c, 21/178 colour only).
 * Unit (Pi): blob_unit --tag w33c score func_80090308 entity_physics_update save_write_data --with best_effects.c
 *   --with <PR>/blast/baseline_start_blast.c --internal func_80090308 --neighbours
 *   FAIL func_80090308: 180 of 282 words differ (was 210); aligned rows 152 / ops 16 (was 198/87); frame 272/168;
 *   body 281 vs 282 (the missing word is the rand4 seed reload)
 *   FAIL entity_physics_update: 21 of 178 (unchanged) | FAIL save_write_data: 149 of 300 (unchanged) | locked differ 0
 * Changes vs the PR natural effects.c (w33c):
 *  - loop body in GC StartNewBlast's statement order: stretch = 0.05f + Random(.005f) right after size, the
 *    diagonal chain m00 = m11 = m22 = size, spin_y/spin_z = 0.1f - Random(.2f) as single statements, then
 *    color.a = 128, the zero stores (GC ht, yaw, pitch order), alpha, timer = (int)Random(10)+25, SetColor.
 *    No a/b/c temporaries, `p` only for timer and the SetColor handle (retail s5): ops 87 -> 28.
 *  - extra block colour through an inlined pointer setter `color_set(&color,255,255,255,95)` (w25d's helper):
 *    retail's shared 255 (a0) and 95 constants, ops 26 -> 16. SHAPING HELPER OF UNKNOWN IDENTITY (disclosed);
 *    it is static here (measure only); package it non-static/unkept (w17j) to avoid a deleted stub.
 * Open: rand4 seed reload (retail `lw t6,0(s1)` before rand4; nothing in this source kills the seed: stores
 *   through `p` are copy-propagated — tested zero/alpha/spin_z through p, no change); IV order s2/s3 swapped;
 *   extra-block colour byte stores scheduled before the `sw v0,336(s0)` (as1); FRAME 168 vs 272 (104 B):
 *   retail node -4 / color -60 (13 words between; ours 2) and ~60 B more inline/temp area below color.
 */
/*
 * w24e draft (NOT A MATCH), built on w23d best_effects.c. func_80090308 (internal) + entity_physics_update.
 * Score: blob_unit --tag w24e score func_80090308 entity_physics_update save_write_data --with best_effects.c
 *   --with swd.c (= w22d/w23d best_swd.c) --internal func_80090308 --neighbours
 * func_80090308: aligned rows 216 -> 198 (ops 90 -> 87), body length 285 -> 282 = retail, frame 160 vs 272.
 *   Lever: the extra block writes the colour as separate byte statements and copies it as a word
 *   (`.color.word = color.word`, retail lw/sw instead of the union struct copy).
 * entity_physics_update: natural form, NO device: no `slot` variable, node->state read directly; PRE
 *   inserts retail's reload at the end of the section-1 then-block (lh a1,8(a3) at +0x158) by itself.
 *   21 words off = the same single colour decision as w23d's slot form (the expression web has the same
 *   7 live blocks + card 5, tot 10, save 2.5 > 2.0 of func_80090770's inlined params). Unit EQUAL still
 *   needs w23d's six empty gotos (device, see w23d best_effects.c); not used here.
 */
/*
 * w23d draft (NOT A MATCH): func_80090308 (internal, 282 words) and entity_physics_update (kept, 178 words).
 * The second real caller is save_write_data (best_swd.c = w22d's, unchanged). Score with
 *   blob_unit --tag w23d score func_80090308 entity_physics_update save_write_data --with best_effects.c
 *     --with best_swd.c --internal func_80090308 --neighbours
 *   (EQUAL entity_physics_update; func_80090308 210/282; save_write_data 223/300; locked bodies differ: 0)
 * entity_physics_update: unit EQUAL only with a disclosed DEVICE, the six empty `goto gN; gN:;` statements
 *   after the reload `slot=node->state;`. These add 6 live blocks to slot's allocation unit
 *   (n 12 -> 18, nocs 4 -> 6, save 10/4=2.5 -> 1.67 < 2.0), so slot is coloured after func_80090770's
 *   params (save 2.0) and takes a1 as in retail. A natural source form with the same effect is not found.
 *   The match is provisional while func_80090308 is a draft, because it is an internal callee with an IPA convention.
 * func_80090308 (w23d rewrite of the w22d puff_init draft): an in-place `for (i=0;i<4;i++)` loop with direct
 *   D_80154660[car].particles[i] / player_array[car] indexing. This reproduces retail's strength-reduced IVs
 *   (s0 particle, s2 texture via lhu 316(s2), s3 matrix, s6 player, s7 0x42000, s4 LCG mult, s8 128), and its
 *   pos loads before stores. `p` (= &particles[i]) is used only for the size reload (diag), stretch/spin, timer
 *   and handle.half.low, giving retail's s5 copy. Open: rand4's seed reload, the store order around the rands,
 *   the extra block (direct indexing), and the frame (160 vs 272).
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

static void color_set(Color *c,u8 r,u8 g,u8 b,u8 a)
{
    c->c.r=r;c->c.g=g;c->c.b=b;c->c.a=a;
}
void func_80090308(s16 car)
{
    Node *node;
    s32 i;
    Particle *p;
    Color color;
    color=D_8011B550;
    node=func_80090284();
    if (node) {
        node->callback=func_80090FEC;
        node->value=D_801543CC;
        node->state=car;
        color.c.r=244;color.c.g=205;color.c.b=20;
        for (i=0;i<4;i++) {
            p=&D_80154660[car].particles[i];
            if (D_80154660[car].particles[i].handle.word!=-1) entity_spawn_callback((s16)D_80154660[car].particles[i].handle.word,0,0);
            math_utility(D_8011418C,D_80154660[car].particles[i].matrix);
            D_80154660[car].particles[i].pos[0]=player_array[car].pos[0];
            D_80154660[car].particles[i].pos[1]=player_array[car].pos[1]-3.0f;
            D_80154660[car].particles[i].pos[2]=player_array[car].pos[2];
            D_80154660[car].particles[i].handle.word=func_8008E26C(D_801427C0[158+i],D_80154660[car].particles[i].matrix,-1,0x42000);
            D_80154660[car].particles[i].size=0.4f;
            D_80154660[car].particles[i].stretch=0.05f+func_8008B2E4(0.005f);
            D_80154660[car].particles[i].matrix[0][0]=D_80154660[car].particles[i].matrix[1][1]=D_80154660[car].particles[i].matrix[2][2]=D_80154660[car].particles[i].size;
            D_80154660[car].particles[i].spin_y=0.1f-func_8008B2E4(0.2f);
            D_80154660[car].particles[i].spin_z=0.1f-func_8008B2E4(0.2f);
            color.c.a=128;
            D_80154660[car].particles[i].zero80=0.0f;
            D_80154660[car].particles[i].zero60=0.0f;
            D_80154660[car].particles[i].zero68=0.0f;
            D_80154660[car].particles[i].alpha=128;
            p->timer=(int)func_8008B2E4(10.0f)+25;
            func_8008E06C(p->handle.half.low,&color);
        }
        if (active_player_count<4 && (D_80156994 || D_8014978C>=6)) {
            math_utility(D_8011418C,D_80154660[car].extra.matrix);
            D_80154660[car].extra.pos[0]=player_array[car].pos[0];
            D_80154660[car].extra.pos[1]=player_array[car].pos[1];
            D_80154660[car].extra.pos[2]=player_array[car].pos[2];
            if (D_80154660[car].extra.handle!=-1) entity_spawn_callback((s16)D_80154660[car].extra.handle,0,0);
            D_80154660[car].extra.handle=func_8008E26C(D_80142904,D_80154660[car].extra.matrix,-1,0x52000);
            D_80154660[car].extra.alpha=95;
            D_80154660[car].extra.flag=0;
            D_8012E700[D_80154660[car].extra.handle].scale=0.2f;
            D_8012E700[D_80154660[car].extra.handle].scale2=1.4f;
            color_set(&color,255,255,255,95);
            D_8012E700[D_80154660[car].extra.handle].color.word=color.word;
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
    if (D_801170FC) return;
    if (!update) {
kill:
        entity_spawn_callback(node->id,0,0);
        node->id=-1;
        entity_transform_apply(node,1);
        return;
    }
    if (node->state>=0) {
        if (!D_8014A250[node->state].active) goto kill;
        ((f32 *)D_8012E700[node->id].data)[9]=player_array[node->state].pos[0];
        ((f32 *)D_8012E700[node->id].data)[10]=player_array[node->state].pos[1];
        ((f32 *)D_8012E700[node->id].data)[11]=player_array[node->state].pos[2];
    }
    node->value-=D_8002EB94;
    if (node->state<0 || (node->state>=0 && node->value<=0.0f)) {
        node->value=0.0333333015f;
        if (node->count==30) goto kill;
        func_80090770(node->id,D_80142908[node->count]);
        node->count++;
        if (node->state>=0) {
            if (active_player_count<4 && (D_80156994 || D_8014978C>=6) && node->count==4) func_8038A408(player_array[node->state].pos);
            if (node->count==3) func_80090308(node->state);
        }
    }
}
