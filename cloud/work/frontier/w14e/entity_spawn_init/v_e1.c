/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Native N64 smoke/effect spawning, descended in behavior from arcade StartSmoke. */
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
extern u32 D_8011735C;
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

void entity_spawn_init(s16 slot, u32 type, s32 fast, f32 *offset)
{
    Effect *v;
    Car *car;
    f32 pos[3];
    f32 delta[3];
    f32 spacing;
    f32 previous[3];
    u16 surface = D_8014A250[slot].surface[type];
    s16 mph;
    s16 first;
    s32 count;
    s32 i;
    s32 color0 = D_8011749C;
    s32 color1 = D_801174A0;
    s32 color2 = D_801174A4;
    s32 color3 = D_801174A8;
    s32 color4 = D_801174AC;
    s32 color5 = D_801174B0;
    Model *model;
    f32 distance;
    f32 scale;
    s32 handle;
    u16 resource;

    if (fast == 77) func_8008B2E4(1.0f);
    if (!D_80143F14 && type < 4 && !fast) return;
    i = 0;
    car = &D_80152818[slot];
    count = 1;
    mph = (s16)fabsf((f32)(s16)(car->mph >> 2));
    if ((s16)surface == 8 && type < 5) return;
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
    model = &D_8014A250[slot];
    do {
        v = func_8008E3C0(D_8013F1E0);
        if (!v) return;
        v->flags = type << 17;
        v->slot = slot;
        v->timestep = C_80123960;
        if (type == 5) {
            v->object = -1;
            handle = func_8008E26C(D_80142958, v->basis, -1, 0x52000);
            v->handle.word = handle;
            D_8012E700[handle].scaleB = C_80123964;
        } else {
            if (D_80156994 == 0 && D_8014978C >= 0 && D_8014978C < 6) {
                v->object = 0;
                handle = func_8008E26C(D_80142946, v->basis, -1, 0x52000);
            } else {
                v->object = (s32)func_8008B2E4(8.0f);
                handle = func_8008E26C(D_80142948[v->object], v->basis, -1, 0x52000);
            }
            v->handle.word = handle;
        }
        if (type >= 6) v->lifetime = func_8008B2E4(C_80123968) + C_8012396C;
        else if (count == 1) v->lifetime = func_8008B2E4(1.5f) + 0.5f;
        else if (type == 5) v->lifetime = func_8008B2E4(2.25f) + 0.5f;
        else v->lifetime = func_8008B2E4(0.25f) + 0.5f;
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
            if (D_801392D8[slot] & 1) D_801392D8[slot] &= ~1;
            else { D_801392D8[slot] |= 1; v->flags |= 1; }
            break;
        case 1:
            if (D_801392D8[slot] & 2) D_801392D8[slot] &= ~2;
            else { D_801392D8[slot] |= 2; v->flags |= 1; }
            break;
        case 2:
            if (D_801392D8[slot] & 4) D_801392D8[slot] &= ~4;
            else { D_801392D8[slot] |= 4; v->flags |= 1; }
            break;
        case 3:
            if (D_801392D8[slot] & 8) D_801392D8[slot] &= ~8;
            else { D_801392D8[slot] |= 8; v->flags |= 1; }
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
            pos[0] += (func_8008B2E4(5.0f) - 2.5f) * car->right[0];
            pos[1] += (func_8008B2E4(5.0f) - 2.5f) * car->right[1];
            pos[2] += (func_8008B2E4(5.0f) - 2.5f) * car->right[2];
            pos[0] += (func_8008B2E4(7.0f) - 3.5f) * car->forward[0];
            pos[1] += (func_8008B2E4(7.0f) - 3.5f) * car->forward[1];
            pos[2] += (func_8008B2E4(7.0f) - 3.5f) * car->forward[2];
            scale = D_8011744C[model->type];
            pos[0] += scale * car->up[0];
            pos[1] += scale * car->up[1];
            pos[2] += scale * car->up[2];
            pos[1] += 0.75f;
            D_8012E700[v->handle.word].scaleA = func_8008B2E4(0.5f) + 0.75f;
            break;
        case 5:
            D_8012E700[v->handle.word].scaleA = C_80123990;
            resource = D_8013F380[i];
            func_8008D870(v->handle.part.id, D_80151AE8[resource >> 10].data + (resource & 0x3ff) * 36, -1);
            pos[0] = car->pos[0];
            pos[1] = car->pos[1];
            pos[2] = car->pos[2];
            v->velocity[0] = func_8008B2E4(4.0f) - 2.0f;
            v->velocity[2] = func_8008B2E4(8.0f) - 4.0f;
            pos[0] += v->velocity[0] * car->right[0];
            pos[1] += v->velocity[0] * car->right[1];
            pos[2] += v->velocity[0] * car->right[2];
            pos[0] += v->velocity[2] * car->forward[0];
            pos[1] += v->velocity[2] * car->forward[1];
            pos[2] += v->velocity[2] * car->forward[2];
            scale = D_8011744C[model->type];
            pos[0] += scale * car->up[0];
            pos[1] += scale * car->up[1];
            pos[2] += scale * car->up[2];
            break;
        case 6:
            if (D_801392D8[slot] & 0x1000) D_801392D8[slot] &= ~0x1000;
            else { D_801392D8[slot] |= 0x1000; v->flags |= 1; }
            break;
        case 7:
            if (D_801392D8[slot] & 0x2000) D_801392D8[slot] &= ~0x2000;
            else { D_801392D8[slot] |= 0x2000; v->flags |= 1; }
            break;
        }
        if (type < 4) {
            scale = -2.0f - (f32)i * spacing;
            pos[0] = car->forward[0] * scale;
            pos[1] = car->forward[1] * scale;
            pos[2] = car->forward[2] * scale;
            pos[0] = car->wheel[type][0] + pos[0];
            pos[1] += car->wheel[type][1];
            pos[2] += car->wheel[type][2];
            pos[0] += (func_8008B2E4(2.0f) - 1.0f) * car->right[0];
            pos[1] += (func_8008B2E4(2.0f) - 1.0f) * car->right[1];
            pos[1] += D_8014A250[slot].suspension[type];
            pos[2] += (func_8008B2E4(2.0f) - 1.0f) * car->right[2];
            D_8012E700[v->handle.word].scaleA = func_8008B2E4(0.75f) + 0.25f;
        }
        if (type >= 6) {
            pos[0] = car->pos[0];
            pos[1] = car->pos[1];
            pos[2] = car->pos[2];
            pos[0] += offset[1] * car->up[0];
            pos[1] += offset[1] * car->up[1];
            pos[2] += offset[1] * car->up[2];
            pos[0] += offset[2] * car->forward[0];
            pos[1] += offset[2] * car->forward[1];
            pos[2] += offset[2] * car->forward[2];
            pos[0] += car->forward[0] * C_80123994;
            pos[1] += car->forward[1] * C_80123994;
            pos[2] += car->forward[2] * C_80123994;
            if (type == 6) {
                pos[0] += offset[0] * car->right[0];
                pos[1] += offset[0] * car->right[1];
                pos[2] += offset[0] * car->right[2];
                pos[0] += car->right[0] * C_80123998;
                pos[1] += car->right[1] * C_80123998;
                pos[2] += car->right[2] * C_80123998;
            } else {
                pos[0] += offset[0] * car->right[0];
                pos[1] += offset[0] * car->right[1];
                pos[2] += offset[0] * car->right[2];
                pos[0] += car->right[0] * C_8012399C;
                pos[1] += car->right[1] * C_8012399C;
                pos[2] += car->right[2] * C_8012399C;
            }
            pos[0] += car->up[0] * 0.5f;
            pos[1] += car->up[1] * 0.5f;
            pos[2] += car->up[2] * 0.5f;
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
                    pos[0] += delta[0] * scale;
                    pos[1] += delta[1] * scale;
                    pos[2] += delta[2] * scale;
                }
            }
            D_8012E700[v->handle.word].scaleA = func_8008B2E4(C_801239A0) + C_801239A4;
        }
        first = 0;
        model_transform_setup(v->handle.word, 0, 15);
        if (car->viewport >= 0 && (car->visibility == 0 || car->visibility == 1))
            model_data_load(v->handle.word, 0, 1 << car->viewport);
        if (type >= 6) {
            switch (D_80111299[slot * 13 + model->type]) {
            case 0: D_8012E700[v->handle.part.id].rgba.word = color1; break;
            case 1: D_8012E700[v->handle.part.id].rgba.word = color2; break;
            case 2: D_8012E700[v->handle.part.id].rgba.word = color3; break;
            }
        } else if (type == 4) {
            D_8012E700[v->handle.word].rgba.byte[0] = D_8013A020[slot];
            D_8012E700[v->handle.word].rgba.byte[1] = D_8013A020[slot];
            D_8012E700[v->handle.word].rgba.byte[2] = D_8013A020[slot];
            D_8012E700[v->handle.word].rgba.byte[3] = 255;
            D_8013A020[slot] -= 3;
            if (D_8013A020[slot] < 53) D_8013A020[slot] = 53;
        } else if (type == 5) D_8012E700[v->handle.part.id].rgba.word = color0;
        else if ((s16)surface == 1) D_8012E700[v->handle.part.id].rgba.word = color5;
        else D_8012E700[v->handle.part.id].rgba.word = color4;
        v->pos[0] = pos[0];
        v->pos[1] = pos[1];
        v->pos[2] = pos[2];
        i++;
    } while (i != count);
}
