/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef float f32;
typedef struct Model {
    u8 pad0[0xE];
    s16 id;
    s16 type;
    u8 pad12[0x50 - 0x12];
    s16 frame;
    u8 pad52[0x58 - 0x52];
    s16 base;
    s16 count;
} Model;
typedef struct Obj {
    u8 pad0[4];
    s16 step;
    u8 pad6[6];
    Model *model;
    f32 timer;
    void (*fn)();
} Obj;
typedef struct Kind { u8 pad0[0x12]; u16 flags; u8 pad14[0x30 - 0x14]; } Kind;
typedef struct Resource68 { u8 prefix[20]; u16 object; u8 tail[46]; } Resource68;
extern s32 D_801170FC;
extern volatile f32 D_8002EB94;
extern Kind D_80117530[];
extern Resource68 D_8012E700[];
extern u16 D_801427C0[];
void entity_transform_apply(void *node, s32 unlink);
void entity_spawn_callback(s16 arg0, s32 arg1, s32 arg2);

static void resource_set_object(s16 index, u16 object)
{
    D_8012E700[index].object = object;
}

void audio_channel_setup(Obj *obj, s16 mode) {
    Model *m;
    Kind *k;
    s16 f;

    if (mode == 0) {
remove:
        entity_transform_apply(obj, 1);
        return;
    }
    if (D_801170FC != 0) {
        return;
    }
    m = obj->model;
    obj->timer -= D_8002EB94;
    if (obj->timer > 0.0f) {
        return;
    }
    obj->step++;
    obj->timer = 0.0625f;
    if (obj->step >= m->count) {
        k = &D_80117530[m->type];
        if (k->flags & 0x1000) {
            goto remove;
        }
        if (k->flags & 0x2000) {
            entity_spawn_callback(m->id, 0, 0);
            goto remove;
        }
        obj->step = 0;
    }
    f = m->base + obj->step;
    if (f != m->frame) {
        u16 o = D_801427C0[f];
        resource_set_object(m->id, o);
        m->frame = f;
    }
}
