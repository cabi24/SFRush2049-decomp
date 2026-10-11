/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * entity_spawn_init (0x8008EA10): spawn N smoke/dust/exhaust effect objects for car `slot`, effect `type` 0..7.
 * Behavioural ancestor: arcade game/visuals.c StartSmoke (surface/mph gates, smoke object pick, per-tire
 * position); the N64 routine is heavily expanded (8 types, state-bit toggles in D_801392D8, trail
 * interpolation for types 6/7, per-type colours).
 * STATE: EQUAL in the whole-program unit (w21f). It inlines the locked func_8008B2E4 19x and two file-static
 *   helpers. Scorer (Pi): blob_unit --tag w21f score entity_spawn_init --with <this file> --neighbours ->
 *   "EQUAL entity_spawn_init: 1386 words", "locked bodies that differ in this unit: 0".
 * w21f lever (7 words -> 0): two inlined static helpers, defined after the function with prototypes first.
 *   - get_car(slot) returns &D_80152818[slot]. car is then assigned from the inlined return value (v0), so uopt
 *     keeps it as a variable (s1) instead of substituting the address expression. That removes car's spill-temp
 *     home, and the helper's 8-byte inline area makes the named area 632. Result: retail's homes (model address
 *     92, switch type copy 84). Defined after the function: the inlined addu carries the helper's line number,
 *     and as1 breaks the aftercycles tie with `sw zero,644(sp)` (i = 0) by the lower line, as in retail.
 *   - get_mph(car) returns car->mph >> 2. Without it, uopt propagates the v0 return into `car->mph`, v0 is in
 *     use in mph's block, and mph gets v1. With it, mph is the second helper's v0 return, as in retail.
 *   Neither helper's identity is known. Neither is a kept function in retail (no tiny function computes
 *   &D_80152818[x] or loads 248(a0)). Like attach() in the locked func_8008E408 group, they are file statics
 *   with one call site each. In the shadow unit they leave two unsymbolled `jr ra; nop` stubs before
 *   entity_spawn_init, which retail does not have (same as attach()'s stub before func_8008E408).
 * Shaping devices / hypotheses (all disclosed):
 *   - `if (type) {}` at the top of the loop body: a compiled-out read (weight-10 use). The two helper return
 *     labels add one uopt block each to type's live range (nocs 46 -> 47, save 192/47 = 4.085). type then loses
 *     callee-saved s4 to the constants 0 and 68 (save 150/36 = 4.1667), and s4/s5/s6 are permuted. The read
 *     raises type's total to 202. No natural source form found (see w21f RESULTS.md).
 *   - `*(s16 *)&surface == 8`: surface is address-taken through this one use, so it stays memory-only as in
 *     retail (w17f; `volatile` is wrong). Still required: plain `surface == 8` gives 1114 words.
 *   - Unused locals pB0, pB1, pB2, pC1..pC3, pD00..pD43 (50 slots: 49 s32 and one u16) are the exact frame
 *     residual. Retail frame 728; frame without them 528. With one pD fewer the frame is 720 (159 words).
 *     One more still gives 728 (8-byte granularity). They reproduce every retail named-local offset: pos 708,
 *     delta 696, spacing 692, previous 668, surface 666, mph 664, first 662, count 648, i 644, colours
 *     628..608.
 *   - Lifetime sites 2 and 4 spell 0.5 as `.5f`: uopt keys real constants by their text, so these are
 *     separate constants and are rematerialised (lui 0x3f00) as in retail. The other 0.5 uses share $f22.
 *   - Position accumulation is written `pos[k] = pos[k] + B * A`, which gives retail's add(prod,pos) and
 *     mul(A,B) operand order. `pos[1] += suspension` stays `+=` (retail add(pos,susp)).
 *   - Each colour store uses a block local: `{ s16 h = v->handle.word; ... }`. Retail colours that value v0.
 *     The identity of the helper or macro is a hypothesis (inlined func_8008E06C does not fit umerge's budget).
 *   - `model_transform_setup(v->handle.word, first = 0, 15)` places the `first = 0` store after the a0 load.
 *   - The six state-bit cases are written `if (bit) { clear; break; } set; ...; break;` (w20g lever, no device).
 */
typedef signed char s8; typedef unsigned char u8;
typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct Car { u8 pad0[8]; f32 pos[3]; f32 dir[3]; u8 pad32[12]; f32 right[3]; f32 up[3]; f32 forward[3]; u8 pad80[36]; f32 wheel[4][3]; u8 pad164[84]; s16 mph; u8 pad250[610]; s8 viewport; s8 visibility; u8 pad862[90]; } Car;
typedef struct Model {u8 pad0[8];u8 type;u8 pad9[1555];u16 surface[8];u8 pad1580[304];f32 suspension[4];u8 pad1900[156];} Model;
typedef struct Effect { void *next; f32 basis[9]; f32 pos[3]; union { s32 word; struct {s16 bank,id;} part; } handle; s32 unknown56; f32 lifetime; f32 timestep; f32 velocity[3]; u32 flags; s16 slot; s16 object; } Effect;
typedef struct Scene {u32 flags;u32 flags4;void *owner;f32 scaleA;f32 scaleB;u16 id;s16 links[3];s32 words[8];union { u8 byte[4]; u32 word; } rgba;s32 tail;} Scene;
typedef struct Bank {u8 *data;u32 other;} Bank;
extern Car D_80152818[];
extern Model D_8014A250[];
extern Scene D_8012E700[];
extern u32 D_801392D8[];
extern u8 D_8013A020[];
extern f32 D_8013C098[][6];
extern u8 D_8013F1E0[];
extern u16 D_8013F380[];
extern u16 D_80142948[];
extern Bank D_80151AE8[];
extern s8 D_80111299[];
extern f32 D_8011744C[];
extern s32 D_8011749C,D_801174A0,D_801174A4,D_801174A8,D_801174AC,D_801174B0;
void func_8008D870(s16 handle,void *resource,s32 mode);
f32 func_8008E0B8(f32 *v);
s32 func_8008E26C(s32 object,void *basis,s16 parent,s32 flags);
Effect *func_8008E3C0(void *pool);
void func_8008E408(s16 slot,u32 type);
void model_data_load(s32 handle,s32 mode,u32 viewport);
void model_transform_setup(s32 handle,s32 mode,u32 viewport);
f32 func_8008B2E4(f32 max);
f32 fabsf(f32 x);


#pragma intrinsic (fabsf)
#define C_80123960 0.0333333015f
#define C_80123964 1.00999999f
#define C_80123968 0.0700000003f
#define C_8012396C 0.0799999982f
#define C_80123990 0.200000003f
#define C_80123994 -1.14999998f
#define C_80123998 -2.5999999f
#define C_8012399C 2.5999999f
#define C_801239A0 0.100000001f
#define C_801239A4 0.075000003f
extern u16 D_80142946, D_80142958;
extern s8 D_80143F14, D_8014978C, D_80156994;
extern s16 D_8014A108;

static s16 get_mph(Car *c);
static Car *get_car(s16 slot);

void entity_spawn_init(s16 slot, u32 type, s32 fast, f32 *offset)
{
    Effect *v;
    Car *car;
    f32 pos[3];
    f32 delta[3];
    f32 spacing;
    f32 distance;
    f32 scale;
    Model *model = &D_8014A250[slot];
    f32 previous[3];
    s16 surface = model->surface[type];
    s16 mph;
    s16 first;
    u16 pB0;
    s32 pB1;
    s32 pB2;
    s32 count;
    s32 i;
    s32 pC1, pC2, pC3;
    s32 color0 = D_8011749C;
    s32 color1 = D_801174A0;
    s32 color2 = D_801174A4;
    s32 color3 = D_801174A8;
    s32 color4 = D_801174AC;
    s32 color5 = D_801174B0;
    s32 pD00;
    s32 pD01;
    s32 pD02;
    s32 pD03;
    s32 pD04;
    s32 pD05;
    s32 pD06;
    s32 pD07;
    s32 pD08;
    s32 pD09;
    s32 pD10;
    s32 pD11;
    s32 pD12;
    s32 pD13;
    s32 pD14;
    s32 pD15;
    s32 pD16;
    s32 pD17;
    s32 pD18;
    s32 pD19;
    s32 pD20;
    s32 pD21;
    s32 pD22;
    s32 pD23;
    s32 pD24;
    s32 pD25;
    s32 pD26;
    s32 pD27;
    s32 pD28;
    s32 pD29;
    s32 pD30;
    s32 pD31;
    s32 pD32;
    s32 pD33;
    s32 pD34;
    s32 pD35;
    s32 pD36;
    s32 pD37;
    s32 pD38;
    s32 pD39;
    s32 pD40;
    s32 pD41;
    s32 pD42;
    s32 pD43;

    
    if (!D_80143F14 && type < 4 && !fast) return;
    
    
    i = 0;
    count = 1;
    car = get_car(slot);
    mph = get_mph(car);
    mph = fabsf((f32)mph);
    
    if (*(s16 *)&surface == 8 && type < 5) return;
    if (mph < 10 && type < 4) return;
    first = 1;
    spacing = type >= 6 ? 0.75f : 2.25f;
    if (fast) {
        func_8008E408(slot, type);
        return;
    }
    if (type == 5) count = 5;
    else if (type < 4 || type >= 6) {
        if (D_8014A108 >= 3) return;
        if (mph >= 115) count = 4;
        else if (mph >= 100) count = 3;
        else if (mph >= 85) count = 2;
        if (type >= 6) {
            if (type == 6 && !(D_801392D8[slot] & 0x4000)) count = 1;
            else if (type == 7 && !(D_801392D8[slot] & 0x8000)) count = 1;
            else count += 5;
            if (mph >= 150 && count != 1) count += 3;
        }
    } else {
        if (D_801392D8[slot] & 0x40) {
            D_801392D8[slot] &= ~0x40;
            D_801392D8[slot] |= 0x80;
            return;
        }
        if (D_801392D8[slot] & 0x80) {
            D_801392D8[slot] &= ~0x80;
            if (D_8014A108 >= 3) D_801392D8[slot] |= 0x80000;
            return;
        }
        if (D_801392D8[slot] & 0x80000) {
            D_801392D8[slot] &= ~0x80000;
            return;
        }
    }
    do {
        v = func_8008E3C0(D_8013F1E0);
        if (type) {}
        if (!v) return;
         v->timestep = C_80123960; 
        v->flags = type << 17;
        
        v->slot = slot;
        
        
        if (type == 5) {
            v->object = -1;
            v->handle.word = func_8008E26C(D_80142958, v->basis, -1, 0x52000);
            D_8012E700[v->handle.word].scaleB = C_80123964;
        } else {
            if (D_80156994 == 0 && D_8014978C >= 0 && D_8014978C < 6) {
                v->object = 0;
                v->handle.word = func_8008E26C(D_80142946, v->basis, -1, 0x52000);
            } else {
                v->object = (s32)func_8008B2E4(8.0f);
                v->handle.word = func_8008E26C(D_80142948[v->object], v->basis, -1, 0x52000);
            }
        }
        if (type >= 6) v->lifetime = func_8008B2E4(C_80123968) + C_8012396C;
        else if (count == 1) v->lifetime = func_8008B2E4(1.5f) + .5f;
        else if (type == 5) v->lifetime = func_8008B2E4(2.25f) + 0.5f;
        else v->lifetime = func_8008B2E4(0.25f) + .5f;
        if (type < 5) {
            v->velocity[0] = car->dir[0];
            v->velocity[1] = car->dir[1];
            v->velocity[2] = car->dir[2];
            v->velocity[0] *= 0.125f;
            v->velocity[1] *= 0.125f;
            v->velocity[2] *= 0.125f;
            if (type == 4) {
                v->velocity[0] *= 0.5f;
                v->velocity[1] *= 0.5f;
                v->velocity[2] *= 0.5f;
            }
        } else if (type >= 6) {
            v->velocity[0] = car->dir[0];
            v->velocity[1] = car->dir[1];
            v->velocity[2] = car->dir[2];
            v->velocity[0] *= 0.5f;
            v->velocity[1] *= 0.5f;
            v->velocity[2] *= 0.5f;
        }
        switch (type) {
        case 0:
            if (D_801392D8[slot] & 1) { D_801392D8[slot] &= ~1; break; }
            D_801392D8[slot] |= 1; v->flags |= 1;
            break;
        case 1:
            if (D_801392D8[slot] & 2) { D_801392D8[slot] &= ~2; break; }
            D_801392D8[slot] |= 2; v->flags |= 1;
            break;
        case 2:
            if (D_801392D8[slot] & 4) { D_801392D8[slot] &= ~4; break; }
            D_801392D8[slot] |= 4; v->flags |= 1;
            break;
        case 3:
            if (D_801392D8[slot] & 8) { D_801392D8[slot] &= ~8; break; }
            D_801392D8[slot] |= 8; v->flags |= 1;
            break;
        case 4:
            D_801392D8[slot] |= 0x40;
            if (D_8014A108 == 1) v->lifetime += 0.5f;
            if (!(D_801392D8[slot] & 0x20)) {
                D_801392D8[slot] |= 0x20;
                D_8013A020[slot] = 175;
            }
            if (D_801392D8[slot] & 0x10) D_801392D8[slot] &= ~0x10;
            else { D_801392D8[slot] |= 0x10; v->flags |= 1; }
            pos[0] = car->pos[0];
            pos[1] = car->pos[1];
            pos[2] = car->pos[2];
            pos[0] = pos[0] + car->right[0] * (func_8008B2E4(5.0f) - 2.5f);
            pos[1] = pos[1] + car->right[1] * (func_8008B2E4(5.0f) - 2.5f);
            pos[2] = pos[2] + car->right[2] * (func_8008B2E4(5.0f) - 2.5f);
            pos[0] = pos[0] + car->forward[0] * (func_8008B2E4(7.0f) - 3.5f);
            pos[1] = pos[1] + car->forward[1] * (func_8008B2E4(7.0f) - 3.5f);
            pos[2] = pos[2] + car->forward[2] * (func_8008B2E4(7.0f) - 3.5f);
            pos[0] = pos[0] + car->up[0] * D_8011744C[D_8014A250[slot].type];
            pos[1] = pos[1] + car->up[1] * D_8011744C[D_8014A250[slot].type];
            pos[2] = pos[2] + car->up[2] * D_8011744C[D_8014A250[slot].type];
            pos[1] = pos[1] + 0.75f;
            D_8012E700[v->handle.word].scaleA = func_8008B2E4(0.5f) + 0.75f;
            break;
        case 5:
            D_8012E700[v->handle.word].scaleA = C_80123990;
            func_8008D870(v->handle.part.id, D_80151AE8[D_8013F380[i] >> 10].data + (D_8013F380[i] & 0x3ff) * 36, -1);
            pos[0] = car->pos[0];
            pos[1] = car->pos[1];
            pos[2] = car->pos[2];
            v->velocity[0] = func_8008B2E4(4.0f) - 2.0f;
            v->velocity[2] = func_8008B2E4(8.0f) - 4.0f;
            pos[0] = pos[0] + car->right[0] * v->velocity[0];
            pos[1] = pos[1] + car->right[1] * v->velocity[0];
            pos[2] = pos[2] + car->right[2] * v->velocity[0];
            pos[0] = pos[0] + car->forward[0] * v->velocity[2];
            pos[1] = pos[1] + car->forward[1] * v->velocity[2];
            pos[2] = pos[2] + car->forward[2] * v->velocity[2];
            pos[0] = pos[0] + car->up[0] * D_8011744C[D_8014A250[slot].type];
            pos[1] = pos[1] + car->up[1] * D_8011744C[D_8014A250[slot].type];
            pos[2] = pos[2] + car->up[2] * D_8011744C[D_8014A250[slot].type];
            break;
        case 6:
            if (D_801392D8[slot] & 0x1000) { D_801392D8[slot] &= ~0x1000; break; }
            D_801392D8[slot] |= 0x1000; v->flags |= 1;
            break;
        case 7:
            if (D_801392D8[slot] & 0x2000) { D_801392D8[slot] &= ~0x2000; break; }
            D_801392D8[slot] |= 0x2000; v->flags |= 1;
            break;
        }
        if (type < 4) {
            scale = -2.0f - (f32)i * spacing;
            pos[0] = car->forward[0] * scale;
            pos[1] = car->forward[1] * scale;
            pos[2] = car->forward[2] * scale;
            pos[0] = pos[0] + car->wheel[type][0];
            pos[1] = pos[1] + car->wheel[type][1];
            pos[2] = pos[2] + car->wheel[type][2];
            pos[0] = pos[0] + car->right[0] * (func_8008B2E4(2.0f) - 1.0f);
            
            pos[1] = pos[1] + car->right[1] * (func_8008B2E4(2.0f) - 1.0f);
            
            pos[2] = pos[2] + car->right[2] * (func_8008B2E4(2.0f) - 1.0f);
             pos[1] += D_8014A250[slot].suspension[type]; 
            D_8012E700[v->handle.word].scaleA = func_8008B2E4(0.75f) + 0.25f;
        }
        if (type >= 6) {
            pos[0] = car->pos[0];
            pos[1] = car->pos[1];
            pos[2] = car->pos[2];
            pos[0] = pos[0] + car->up[0] * offset[1];
            pos[1] = pos[1] + car->up[1] * offset[1];
            pos[2] = pos[2] + car->up[2] * offset[1];
            pos[0] = pos[0] + car->forward[0] * offset[2];
            pos[1] = pos[1] + car->forward[1] * offset[2];
            pos[2] = pos[2] + car->forward[2] * offset[2];
            pos[0] = pos[0] + C_80123994 * car->forward[0];
            pos[1] = pos[1] + C_80123994 * car->forward[1];
            pos[2] = pos[2] + C_80123994 * car->forward[2];
            if (type == 6) {
                pos[0] = pos[0] + car->right[0] * offset[0];
                pos[1] = pos[1] + car->right[1] * offset[0];
                pos[2] = pos[2] + car->right[2] * offset[0];
                pos[0] = pos[0] + C_80123998 * car->right[0];
                pos[1] = pos[1] + C_80123998 * car->right[1];
                pos[2] = pos[2] + C_80123998 * car->right[2];
            } else {
                pos[0] = pos[0] + car->right[0] * offset[0];
                pos[1] = pos[1] + car->right[1] * offset[0];
                pos[2] = pos[2] + car->right[2] * offset[0];
                pos[0] = pos[0] + C_8012399C * car->right[0];
                pos[1] = pos[1] + C_8012399C * car->right[1];
                pos[2] = pos[2] + C_8012399C * car->right[2];
            }
            pos[0] = pos[0] + 0.5f * car->up[0];
            pos[1] = pos[1] + 0.5f * car->up[1];
            pos[2] = pos[2] + 0.5f * car->up[2];
            if (first) {
                if (type == 6) {
                    if (!(D_801392D8[slot] & 0x4000)) D_801392D8[slot] |= 0x4000;
                    else { previous[0] = D_8013C098[slot][0]; previous[1] = D_8013C098[slot][1]; previous[2] = D_8013C098[slot][2]; }
                    D_8013C098[slot][0] = pos[0]; D_8013C098[slot][1] = pos[1]; D_8013C098[slot][2] = pos[2];
                } else if (type == 7) {
                    if (!(D_801392D8[slot] & 0x8000)) D_801392D8[slot] |= 0x8000;
                    else { previous[0] = D_8013C098[slot][3]; previous[1] = D_8013C098[slot][4]; previous[2] = D_8013C098[slot][5]; }
                    D_8013C098[slot][3] = pos[0]; D_8013C098[slot][4] = pos[1]; D_8013C098[slot][5] = pos[2];
                }
            } else {
                delta[0] = previous[0] - pos[0];
                delta[1] = previous[1] - pos[1];
                delta[2] = previous[2] - pos[2];
                distance = func_8008E0B8(delta);
                if (distance != 0.0f) {
                    scale = (f32)i * (distance / (f32)count);
                    pos[0] = pos[0] + scale * delta[0];
                    pos[1] = pos[1] + scale * delta[1];
                    pos[2] = pos[2] + scale * delta[2];
                }
            }
            D_8012E700[v->handle.word].scaleA = func_8008B2E4(C_801239A0) + C_801239A4;
        }
         model_transform_setup(v->handle.word, first = 0, 15); 
        if (car->viewport >= 0 && (car->visibility == 0 || car->visibility == 1))
            model_data_load(v->handle.word, 0, 1 << car->viewport);
        if (type >= 6) {
            switch (D_80111299[slot * 13 + model->type]) {
            case 0: { s16 h = v->handle.word; D_8012E700[h].rgba.word = color1; } break;
            case 1: { s16 h = v->handle.word; D_8012E700[h].rgba.word = color2; } break;
            case 2: { s16 h = v->handle.word; D_8012E700[h].rgba.word = color3; } break;
            }
        } else if (type == 4) {
            D_8012E700[v->handle.word].rgba.byte[0] = D_8013A020[slot];
            D_8012E700[v->handle.word].rgba.byte[1] = D_8013A020[slot];
            D_8012E700[v->handle.word].rgba.byte[2] = D_8013A020[slot];
            D_8012E700[v->handle.word].rgba.byte[3] = 255;
            D_8013A020[slot] -= 3;
            if (D_8013A020[slot] < 53) D_8013A020[slot] = 53;
        } else if (type == 5) { s16 h = v->handle.word; D_8012E700[h].rgba.word = color0; }
        else if (surface == 1) { s16 h = v->handle.word; D_8012E700[h].rgba.word = color5; }
        else { s16 h = v->handle.word; D_8012E700[h].rgba.word = color4; }
        v->pos[0] = pos[0];
        v->pos[1] = pos[1];
        v->pos[2] = pos[2];
        i++;
    } while (count != i);
}

static Car *get_car(s16 slot) { return &D_80152818[slot]; }
static s16 get_mph(Car *c) { return c->mph >> 2; }
