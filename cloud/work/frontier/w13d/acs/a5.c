/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef float f32;
typedef struct Model {
    u8 pad0[0xE];
    s16 id;
    s16 type;
    u8 pad12[0x50 - 0x12];
    s16 cur;
    u8 pad52[0x58 - 0x52];
    s16 base;
    s16 count;
} Model;
typedef struct Obj {
    u8 pad0[4];
    s16 frame;
    u8 pad6[6];
    Model *model;
    f32 timer;
} Obj;
typedef struct Ent { u8 pad0[0x14]; u16 tex; u8 pad16[0x44 - 0x16]; } Ent;
typedef struct Kind { u8 pad0[0x12]; u16 flags; u8 pad14[0x30 - 0x14]; } Kind;
extern s32 D_801170FC;
extern volatile f32 D_8002EB94;
extern Kind D_80117530[];
extern u16 D_801427C0[];
extern Ent D_8012E700[];
void entity_transform_apply(void *node, s32 unlink);
void entity_spawn_callback(s16 idx, s32 freeChildren, s32 freeSiblings);

void audio_channel_setup(Obj *obj, s16 mode)
{
    Model *m;
    Kind *kind;
    s16 f;
    s16 id;
    u16 tex;

    if (mode == 0) {
apply:
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
    obj->frame++;
    obj->timer = 0.0625f;
    if (obj->frame >= m->count) {
        kind = &D_80117530[m->type];
        if (kind->flags & 0x1000) {
            goto apply;
        }
        if (kind->flags & 0x2000) {
            entity_spawn_callback(m->id, 0, 0);
            goto apply;
        }
        obj->frame = 0;
    }
    f = m->base + obj->frame;
    if (f != m->cur) {
        id = m->id;
        tex = D_801427C0[f];
        D_8012E700[id].tex = tex;
        if (id) {}
        m->cur = f;
    }
}
