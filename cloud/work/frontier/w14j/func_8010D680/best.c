/* w14j round 1 template for func_8010D680 (base: w10f b.c). Choice points marked  (vbatch). */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct {
    u8 pad0[4];
    u8 flags;
    u8 pad5[7];
    s32 handle;
    u8 pad10[4];
    f32 pos[3];
    u8 pad20[0x50 - 0x20];
    s16 type;
    u8 pad52[0x6C - 0x52];
    f32 *vel;
} Thing;
typedef struct {
    u8 pad0[0xC];
    Thing *thing;
} Holder;
typedef struct {
    u32 flags;
    u8 pad[0x40];
} Rec;
typedef struct {
    u8 pad[900];
    s8 index;
    u8 pad2[952 - 901];
} Car;

extern s32 D_801170FC;
extern volatile f32 D_8002EB94;
extern Rec D_8012E700[];
extern s16 active_player_count;
extern Car D_80152818[];
void entity_transform_apply(void *arg0, s32 arg1);
void sound_position_set(void *arg0, void *arg1);
void model_transform_setup();
void model_data_load();

static s32 model_visible(s16 id)
{
    return D_8012E700[id].flags & 0x80000000;
}

void func_8010D680(Holder *h, s16 on) {
    Thing *t;
    /*@{pad*/s32 pad;
    s32 index;
    f32 v[3];
    s32 ok;
    s32 i;
    f32 *vel;

    if (on == 0) {
        entity_transform_apply(h, 1);
        return;
    }
    if (D_801170FC != 0)
        return;
    t = h->thing;
    if (t->flags & 2) {
        vel = t->vel;
        v[0] = vel[0] * D_8002EB94;
        v[1] = vel[1] * D_8002EB94;
        v[2] = vel[2] * D_8002EB94;
        sound_position_set(v, t->pos);
        return;
    }
    if (model_visible(t->handle)) {
        switch (t->type) {
        case 350: index = 0; break;
        case 351: index = 1; break;
        case 352: index = 2; break;
        case 355: index = 3; break;
        case 356: index = 4; break;
        case 357: index = 5; break;
        case 358: index = 6; break;
        case 360: index = 7; break;
        default: index = -1; break;
        }
        if (index != -1) {
            ok = 1;
            for (i = 0; i < active_player_count; i++) {
                if (index == D_80152818[i].index)
                    ok = 0;
            }
            if (ok) {
                model_transform_setup(t->handle, 0, 15);
                t->flags |= 2;
            }
        }
    } else {
        model_data_load(t->handle, 0, 15);
    }
}
