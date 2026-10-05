typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef float f32;
typedef struct Model {
    u8 pad0[4];
    u8 flags;
    u8 pad5[9];
    s16 id;
    u8 pad10[0x5A - 0x10];
    s16 f5A;
    u8 pad5C[0x6C - 0x5C];
    char *name;
} Model;
typedef struct Obj {
    u8 pad0[4];
    s16 team;
    u8 pad6[6];
    Model *model;
    f32 f10;
    void (*fn)();
} Obj;
typedef struct Ent { u8 pad0[0x38]; s32 sound; u8 pad3C[8]; } Ent;
extern s32 D_801170FC;
extern char *D_80118E08[];
extern u8 D_80140BDC;
extern Ent D_8012E700[];
void entity_transform_apply(void *node, s32 unlink);
char *func_800A464C(char *s, char *find);
void audio_channel_setup();

void func_8010D85C(Obj *obj, s16 mode) {
    Model *m;
    char *p;
    s32 kind;
    u16 handle;

    if (mode == 0) {
        entity_transform_apply(obj, 1);
        return;
    }
    if (D_801170FC != 0) {
        return;
    }
    m = obj->model;
    if (!(m->flags & 1)) {
        m->f5A = 10;
        if (func_800A464C(m->name, "BLU") != 0) {
            kind = 2;
        } else if (func_800A464C(m->name, "GRN") != 0) {
            kind = 1;
        } else {
            kind = 0;
        }
        D_8012E700[m->id].sound = sound_bank_load(D_80118E08[kind], &handle, 0, (s8)(D_80140BDC - 1), 1);
        p = func_800A464C(m->name, "_OFF");
        if (p != 0) {
            obj->team = (s8)(p[4] - '0');
        } else {
            obj->team = 0;
        }
        m->flags |= 1;
    }
    obj->fn = audio_channel_setup;
    obj->f10 = 0.0f;
}
