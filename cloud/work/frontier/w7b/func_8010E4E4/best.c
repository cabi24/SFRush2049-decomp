/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_8010E4E4 (historical label): falling-debris object callback (Obj *, s16 mode), sibling of the
 * matched func_8010D85C (same Obj/Model records, same `mode == 0` -> entity_transform_apply(obj, 1)
 * unlink and D_801170FC pause test). Each frame the model's physics record (m->phys, +0x6C) gets
 * velocity V += gravity * 0.15 (gravity = D_80121DDC = {0, -0.25, 0}) and position RWR (+0x38) moves by
 * V + gravity * 0.15; the orientation UV (+0x14) is rotated by W * frame time through
 * sound_position_set (= arcade rotateuv, see battle_mode_setup.c); frame time D_8002EB94 is volatile
 * (re-read for every use). obj->timer (+0x10) counts down by the frame time; at <= 0 the debris is
 * removed: if its kind record D_80117530[m->type] (0x30 bytes) has flag 0x2000, entity_spawn_callback
 * (m->id, 0, 0); unlink the model from the list D_80143FC8 (func_800AFA84) and free the object.
 * No arcade ancestor (N64 object system). Own literal 0.15f (0x3E19999A at 0x801249CC), verified.
 * Shaping quirks: the removal block is an inlined static helper taking the kind-record pointer (retail
 * forms &D_80117530[type] in a register and loads +0x12 from it; it also supplies 8 bytes of the frame);
 * the unused `f32 a[6]` gives the 104-byte frame with temp at sp+60 and m's spill home at sp+96
 * (INFERRED filler, like func_8010D85C's unused buf[16]); `v = p->V` (retail hoists p+12).
 * -O3 only (-O2 is 103/108 off).
 */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef float f32;
typedef struct Phys {
    f32 W[3];
    f32 V[3];
} Phys;
typedef struct Model {
    u8 pad0[0xE];
    s16 id;
    s16 type;
    u8 pad12[2];
    f32 UV[3][3];
    f32 RWR[3];
    u8 pad44[0x6C - 0x44];
    Phys *phys;
} Model;
typedef struct Obj {
    u8 pad0[0xC];
    Model *model;
    f32 timer;
    void (*fn)();
} Obj;
typedef struct Kind { u8 pad0[0x12]; u16 flags; u8 pad14[0x30 - 0x14]; } Kind;
extern s32 D_801170FC;
extern f32 D_80121DDC[3];
extern volatile f32 D_8002EB94;
extern Kind D_80117530[];
extern u8 D_80143FC8[];
void entity_transform_apply(void *node, s32 unlink);
void sound_position_set(f32 *rv, void *uvs);
void entity_spawn_callback(s16 arg0, s32 arg1, s32 arg2);
void func_800AFA84(void *list, void *node);

static void debris_remove(Obj *obj, Model *m, Kind *k) {
    if (k->flags & 0x2000) {
        entity_spawn_callback(m->id, 0, 0);
    }
    func_800AFA84(D_80143FC8, m);
    entity_transform_apply(obj, 1);
}

void func_8010E4E4(Obj *obj, s16 mode) {
    Phys *p;
    Model *m;
    f32 a[6];
    f32 temp[3];
    f32 *v;
    s16 i;

    if (mode == 0) {
        entity_transform_apply(obj, 1);
        return;
    }
    if (D_801170FC != 0) {
        return;
    }
    m = obj->model;
    p = m->phys;
    v = p->V;
    for (i = 0; i < 3; i++) {
        v[i] += D_80121DDC[i] * 0.15f;
        m->RWR[i] += v[i] + D_80121DDC[i] * 0.15f;
    }
    temp[0] = p->W[0] * D_8002EB94;
    temp[1] = p->W[1] * D_8002EB94;
    temp[2] = p->W[2] * D_8002EB94;
    sound_position_set(temp, m->UV);
    obj->timer -= D_8002EB94;
    if (obj->timer <= 0.0f) {
        debris_remove(obj, m, &D_80117530[m->type]);
    }
}
