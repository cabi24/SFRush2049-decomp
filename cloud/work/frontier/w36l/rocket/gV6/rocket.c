/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * w5a note (NOT a match): this is the r5_e first pass (cloud/work/ipa-groups/drone_ai_update)
 * with the 25 fake `extern f32 D_80123A00..A60` replaced by their literal values (bits checked:
 * entity_tick_main owns 0x80123A00..0x80123A5C, drone_ai_update starts at 0x80123A60 = 0.025f).
 * Score: blob_unit --tag w5a score drone_ai_update entity_tick_main --with best.c
 *        --internal entity_tick_main  -> both FAIL (structurally complete bodies).
 * Findings for the next pass are in ../RESULTS.md (frame 368 with col at 340 copied as a
 * struct through `at`, v[3] at 324, x+x not 2*x).
 */
/*
 * drone_ai_update (0x80093B20, 820 words) and its only callee entity_tick_main
 * (0x800930A4, 671 words, receives the player index in $s5). By content this is
 * a per-car light/palette/model-slot updater (the "DoDrones" name is a guess).
 *
 * HAND-WRITTEN FIRST PASS (cloud round 5, agent r5_e) from the assembly and the
 * m2c seeds in cloud/work/bigfish/seeds/. Not a match; see STATUS.md.
 *
 * Every other callee is ABI and lives outside this group:
 *   model_data_load, model_transform_setup, func_8008B32C, func_80092FE0,
 *   string_copy_format, func_80092B80, matrix_scale_apply, math_utility.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

float sqrtf(float);
float fabsf(float);
#pragma intrinsic (sqrtf)
#pragma intrinsic (fabsf)

typedef union {
    u8 b[4];
    u32 w;
} Color;

/* 3x3 matrix followed by a translation; math_utility copies m[9] */
typedef struct Node {
    /* 0x00 */ f32 m[9];
    /* 0x24 */ f32 t[3];
} Node;

/* table at 0x8012E700, 0x44 bytes each; flags bit 31 = hidden */
typedef struct ModelSlot {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 pad4[4];
    /* 0x08 */ Node *node;
    /* 0x0C */ u8 padC[8];
    /* 0x14 */ u16 tex;
    /* 0x16 */ s16 child;
    /* 0x18 */ s16 sibling;
    /* 0x1A */ u8 pad1A[0x22];
    /* 0x3C */ u32 colA;
    /* 0x40 */ u32 colB;
} ModelSlot;

/* table at 0x80139320, 0x40 bytes each: four model-slot ids per car */
typedef struct CarModels {
    /* 0x00 */ s32 id[4];
    /* 0x10 */ s32 wheelA;
    /* 0x14 */ s32 wheelB;
    /* 0x18 */ s32 pad18[10];
} CarModels;

/* player_array element (0x3B8 bytes) */
typedef struct Car {
    /* 0x000 */ u8 pad0[0xE8];
    /* 0x0E8 */ u32 flagsE8;
    /* 0x0EC */ u8 padEC[0xF8 - 0xEC];
    /* 0x0F8 */ s16 sF8;
    /* 0x0FA */ u8 padFA[0x344 - 0xFA];
    /* 0x344 */ u16 pal[4];
    /* 0x34C */ u32 irColor;
    /* 0x350 */ u32 prevIRColor;
    /* 0x354 */ u8 pad354[4];
    /* 0x358 */ u8 pad358[2];
    /* 0x35A */ s8 fade;
    /* 0x35B */ u8 pad35B;
    /* 0x35C */ s8 lightIdx;
    /* 0x35D */ s8 lightMode;
    /* 0x35E */ u8 pad35E;
    /* 0x35F */ s8 nameSet;
    /* 0x360 */ u32 alpha;
    /* 0x364 */ u8 pad364[0x374 - 0x364];
    /* 0x374 */ f32 scale[3];
    /* 0x380 */ u8 pad380;
    /* 0x381 */ u8 b381[3];
    /* 0x384 */ s8 b384;
    /* 0x385 */ u8 pad385[0x38C - 0x385];
    /* 0x38C */ u32 f38C;
    /* 0x390 */ u8 pad390[0x3A1 - 0x390];
    /* 0x3A1 */ u8 b3A1;
    /* 0x3A2 */ u8 pad3A2[0x3B8 - 0x3A2];
} Car;

/* D_8014A250 element (0x808 bytes) */
typedef struct CarState {
    /* 0x000 */ u8 pad0[8];
    /* 0x008 */ u8 mode;
    /* 0x009 */ u8 pad9[0x3D0 - 9];
    /* 0x3D0 */ f32 f3D0;
    /* 0x3D4 */ u8 pad3D4[0x5F4 - 0x3D4];
    /* 0x5F4 */ f32 dist;
    /* 0x5F8 */ u8 pad5F8[0x6C4 - 0x5F8];
    /* 0x6C4 */ s16 s6C4;
    /* 0x6C6 */ u8 pad6C6[0x7D0 - 0x6C6];
    /* 0x7D0 */ s16 s7D0;
    /* 0x7D2 */ u8 pad7D2[0x808 - 0x7D2];
} CarState;

/* the entity drone_ai_update is called with */
typedef struct Entity {
    /* 0x00 */ u8 pad0[6];
    /* 0x06 */ s16 slot;
    /* 0x08 */ s16 car;
    /* 0x0A */ u8 padA[0x14 - 0xA];
    /* 0x14 */ s32 field14;
} Entity;

extern ModelSlot D_8012E700[];
extern CarModels D_80139320[];
extern Car D_80152818[];
extern CarState D_8014A250[];
extern s32 D_8014A110;          /* gameplay_mode */
extern s32 D_8011735C;          /* rng state */
extern Color D_8011B464;
extern s32 D_801174B4;
extern s8 D_80140418;
extern s8 D_8013FECD;
extern s8 D_8013F1D8;
extern volatile u8 D_80140BDC;
extern u16 D_80142998;
extern s8 D_8014978C;
extern s16 D_80151AD0;
extern u16 D_801427C0[];
extern u8 D_801234A4[];
extern u8 *D_8011AF90[];        /* palette base per track */
extern s32 D_8011B468[];         /* stride 4 */
extern s32 D_8011ADC0[][2];

void model_data_load(s32, s32, u32);
void model_transform_setup(s32, s32, u32);
s32 matrix_scale_apply(Entity *,s32,s32);
void math_utility(f32 *, f32 *);
void func_8008B32C(f32 *, f32 *, f32);
void func_80092FE0(s16, u32 *);
void func_80092BF4(s16, Color *, Color *);
u16 func_80092B80(s16, u8);
s32 string_copy_format(char *, s8, s8, s8);

f32 func_8008B2E4(f32 max);

Node *func_8008B3A0(s16 objnum);

void func_8008E06C(s16 objnum, Color *color) { D_8012E700[objnum].colA = color->w; }

void entity_tick_main(s16 ipa_s5, s32 flag);

/* Reference helper boundaries. No unknown N64 symbol attribution is made. */
static void ZOID_SetObjectDef(s16 objnum, u16 def) { D_8012E700[objnum].tex = def; }
u32 func_8008AE64(s16 objnum) { return D_8012E700[objnum].flags; }
void func_8008AE48(s32 objnum, u32 flags) { D_8012E700[objnum].flags = flags; }


/* MAT3 GetCarsIRColor retained partial, adapted to N64 native no-smoothing
 * output. Its accepted caller proves the helper boundary, not helper equality.
 * Native index loads establish four evaluated palette indexes; each local is
 * used. The GC-only weights and temporary column array are not transplanted. */
static Color GetCarsIRColor(Car *car, CarState *m)
{
    Color clr;
    int u6;
    Color sclr2;
    unsigned char rgb[3];
    float d1;
    float d2;
    unsigned int a;
    unsigned int b;
    unsigned int hobj;
    unsigned int fourth;
    unsigned int sum;
    int u7;
    int u8,u9,u10,u11,u12,u13,u14,u15;
    hobj=(car->pal[0]&0xf800)>>11;
    fourth=(car->pal[1]&0xf800)>>11;
    a=(car->pal[2]&0xf800)>>11;
    b=(car->pal[3]&0xf800)>>11;
    sum = ((Color *)D_8011AF90[D_8014978C])[hobj].b[0]+((Color *)D_8011AF90[D_8014978C])[fourth].b[0]+((Color *)D_8011AF90[D_8014978C])[a].b[0]+((Color *)D_8011AF90[D_8014978C])[b].b[0];
    rgb[0] = sum >> 2;
    sum = ((Color *)D_8011AF90[D_8014978C])[hobj].b[1]+((Color *)D_8011AF90[D_8014978C])[fourth].b[1]+((Color *)D_8011AF90[D_8014978C])[a].b[1]+((Color *)D_8011AF90[D_8014978C])[b].b[1];
    rgb[1] = sum >> 2;
    sum = ((Color *)D_8011AF90[D_8014978C])[hobj].b[2]+((Color *)D_8011AF90[D_8014978C])[fourth].b[2]+((Color *)D_8011AF90[D_8014978C])[a].b[2]+((Color *)D_8011AF90[D_8014978C])[b].b[2];
    rgb[2] = sum >> 2;
    if (m->dist >= 20.0f) {
        d1 = 0.025f * (m->dist - 20.0f);
        if (d1 > 1.0f)
            return ((Color *)D_8011AF90[D_8014978C])[D_8011B468[D_8014978C]];
        if (d1 > 0.0f) {
            sclr2.w = ((Color *)D_8011AF90[D_8014978C])[D_8011B468[D_8014978C]].w;
            d2 = 1.0f - d1;
            a = rgb[0];
            b = sclr2.b[0];
            rgb[0] = a * d2 + b * d1;
            a = rgb[1];
            b = sclr2.b[1];
            rgb[1] = a * d2 + b * d1;
            a = rgb[2];
            b = sclr2.b[2];
            rgb[2] = a * d2 + b * d1;
        }
    }
    clr.b[0] = rgb[0];
    clr.b[1] = rgb[1];
    clr.b[2] = rgb[2];
    clr.b[3] = 255;
    return clr;
}

/* Accepted MAT3 AnimateFrame declaration/entry order with native-only gates.
 * Unused rear/tobj/eobj are donor locals, not inferred frame-padding bytes. */
void drone_ai_update(Entity *ent, s16 alive)
{
    int hulk;
    s16 u0;
    s16 slot;
    CarState *m;
    Car *car;
    int def;
    u32 flags;
    u32 hobj;
    int u1, u2;
    u32 sobj;
    Color clr;
    Color sclr1;
    Color sclr2;
    int u3, u4, u5;
    Color nextclr;

    slot = ent->car;
    m = &D_8014A250[slot];
    car = &D_80152818[slot];
    def = D_8012E700[ent->slot].tex;
    if (alive == 0) {
        if (ent->slot >= 0) {
            model_data_load(ent->slot, 1, 15);
        }
        ent->field14 = 0;
        ent->slot = -1;
        return;
    }
    hulk = (car->flagsE8 & 0x10) != 0;
    sobj = D_80139320[slot].wheelB;
    hobj = D_80139320[slot].wheelA;
    if (m->mode == 2) {
        entity_tick_main(slot, hulk);
    }

    nextclr = GetCarsIRColor(car, m);
    clr.w = nextclr.w;
    car->prevIRColor = car->irColor;
    car->irColor = clr.w;
    sclr1.w = D_8011ADC0[D_8014978C][0];
    sclr2.w = D_8011ADC0[D_8014978C][1];

    if (car->fade != 0) {
        if (car->alpha < 0x20) {
            car->alpha = 0;
        } else {
            car->alpha -= 0x19;
        }
    } else {
        if (car->alpha >= 0xE1) {
            car->alpha = 0xFF;
        } else {
            car->alpha += 0x19;
        }
    }
    sclr1.b[3] = car->alpha;
    sclr2.b[3] = car->alpha;
    if (D_8014A110 == 2 && slot > 0) {
        clr.b[3] = 0x80;
    } else if (D_8014A110 == 6 && (car->f38C & 1)) {
        clr.b[3] = car->b3A1;
        sclr1.b[3] = car->b3A1;
        sclr2.b[3] = car->b3A1;
    } else {
        clr.b[3] = 0xFF;
    }
    func_80092FE0(slot, &clr.w);
    func_80092BF4(slot, &sclr1, &sclr2);

    if (hulk) {
        car->nameSet = 0;
        if (func_80092B80(slot, 13) != def) {
            ZOID_SetObjectDef(ent->slot, func_80092B80(slot, 13));
        }
        model_data_load(hobj, 1, 15);
        model_data_load(sobj, 1, 15);
        matrix_scale_apply(ent,1,car->lightIdx);
        return;
    }
    if ((D_801174B4 & 0x100) && D_8013FECD != 0 && car->nameSet == 0) {
        D_80142998 = string_copy_format((char *)D_801234A4, 0, D_80140BDC - 1, 0);
        car->nameSet = 1;
    }
    if ((D_801174B4 & 0x100) && D_8013FECD == 0 && car->nameSet != 0) {
        car->nameSet = 0;
    }
    if (car->nameSet != 0) {
        if (def != D_80142998) {
            ZOID_SetObjectDef(ent->slot, D_80142998);
        }
        model_data_load(hobj, 1, 15);
        model_data_load(sobj, 1, 15);
        matrix_scale_apply(ent,car->lightMode>=2,car->lightIdx);
        return;
    }
    if (!(D_801174B4 & 0x100)) {
        matrix_scale_apply(ent,car->lightMode>=2,car->lightIdx);
        if (car->fade != 0) {
            if (car->alpha < 0x11) {
                model_data_load(sobj, 1, 15);
            } else {
                sclr1.b[3] = car->alpha;
                sclr2.b[3] = car->alpha;
                func_80092BF4(slot, &sclr1, &sclr2);
            }
        } else {
            if ((D_8014A110 == 6 || D_8014A110 == 4) && D_80151AD0 >= 3) {
                model_data_load(sobj, 1, 15);
            } else if ((D_801174B4 & 0x7C0000) && D_8013F1D8 != 0) {
                model_data_load(sobj, 1, 15);
            } else if (car->lightIdx < 0 || car->lightMode >= 2) {
                model_transform_setup(sobj, 0, 15);
            } else {
                model_data_load(sobj, 1, 1 << car->lightIdx);
            }
        }
        if (func_80092B80(slot, m->mode) != def) {
            ZOID_SetObjectDef(ent->slot, func_80092B80(slot, m->mode));
        }
        if (D_8014A110 == 6 && m->s6C4 >= 0) {
            D_8012E700[ent->slot].flags |= 0x80000000;
        } else if (D_80140418 != 0 || (car->flagsE8 & 8)) {
            D_8012E700[ent->slot].flags ^= 0x80000000;
        } else {
            D_8012E700[ent->slot].flags &= 0x7FFFFFFF;
        }
        if (car->lightMode != 1 || D_80151AD0 >= 2) {
            model_data_load(hobj, 1, 15);
            return;
        }
        {
            flags = func_8008AE64(hobj);
            if ((int)D_8012E700[hobj].tex != (int)D_801427C0[(s16)(slot * 3 + 1)]) {
                ZOID_SetObjectDef(hobj, D_801427C0[(s16)(slot * 3 + 1)]);
            }
            if (flags & (0x100 << car->lightIdx)) {
                model_transform_setup(hobj, 0, 1 << car->lightIdx);
            }
            if (D_80140418 != 0 || (car->flagsE8 & 8)) {
                D_8012E700[hobj].flags ^= 0x80000000;
                return;
            }
            D_8012E700[hobj].flags &= 0x7FFFFFFF;
        }
    }
}

/* Accepted MAT3 full Rocket source shape, N64 field offsets/constants.
 * GC-only extra camera visibility calls omitted: absent from N64 target.
 * The donor's unreferenced float r is retained as provenance, not a frame claim.
 */
void entity_tick_main(s16 slot, int hulk)
{
{
    CarState *m;
    Car *car;
    Node *mat;
    Node *flames[3];
    Color clr = { {255,255,255,255} };
    float scale1;
    float r;
    float pos[3];
    int mph;
    int b;
    m=&D_8014A250[slot];
    car=&D_80152818[slot];
    if(hulk) {
        model_data_load(D_80139320[slot].id[1],1,15);
        model_data_load(D_80139320[slot].id[2],1,15);
        model_data_load(D_80139320[slot].id[3],1,15);
        return;
    }
    model_transform_setup(D_80139320[slot].id[1],0,15);
    model_transform_setup(D_80139320[slot].id[2],0,15);
    model_transform_setup(D_80139320[slot].id[3],0,15);
    if(D_8014A110==2 && slot>0) clr.b[3]=128;
    else if(D_8014A110==6 && (car->f38C&1))
        clr.b[3]=car->b3A1;
    func_8008E06C(D_80139320[slot].id[1], &clr);
    func_8008E06C(D_80139320[slot].id[2], &clr);
    func_8008E06C(D_80139320[slot].id[3], &clr);
    mat=func_8008B3A0(D_80139320[slot].id[0]);
    pos[0]=mat->t[0]+2.0f*mat->m[3];
    pos[1]=mat->t[1]+2.0f*mat->m[4];
    pos[2]=mat->t[2]+2.0f*mat->m[5];
    pos[0]=pos[0]+mat->m[6]*-5.6f;
    pos[1]=pos[1]+mat->m[7]*-5.6f;
    pos[2]=pos[2]+mat->m[8]*-5.6f;
    flames[0]=func_8008B3A0(D_80139320[slot].id[1]);
    math_utility(mat->m,flames[0]->m);
    flames[0]->t[0]=pos[0];
    flames[0]->t[1]=pos[1];
    flames[0]->t[2]=pos[2];
    flames[1]=func_8008B3A0(D_80139320[slot].id[2]);
    math_utility(mat->m,flames[1]->m);
    flames[1]->t[0]=pos[0];
    flames[1]->t[1]=pos[1];
    flames[1]->t[2]=pos[2];
    flames[2]=func_8008B3A0(D_80139320[slot].id[3]);
    math_utility(mat->m,flames[2]->m);
    flames[2]->t[0]=pos[0];
    flames[2]->t[1]=pos[1];
    flames[2]->t[2]=pos[2];
    b=0;
    if(D_8014A110==6 && car->b384==3) {
        b=1;
        car->scale[0]=0.1f;
        car->scale[1]=0.1f;
        car->scale[2]=0.2f;
    } else if((car->sF8>>2)<0) {
        car->scale[0]=0.25f-func_8008B2E4(0.15f);
        car->scale[1]=0.25f-func_8008B2E4(0.15f);
        car->scale[2]=0.55f-func_8008B2E4(0.1);
    } else if(m->f3D0<0.1f) {
        if(car->scale[0]<0.28f)
            car->scale[0]=0.25f-func_8008B2E4(0.15f);
        else {
            car->scale[0]-=func_8008B2E4(0.025f)+0.01f;
            if(car->scale[0]<0.2f) car->scale[0]=0.2f;
        }
        if(car->scale[1]<0.28f)
            car->scale[1]=0.25f-func_8008B2E4(0.15f);
        else {
            car->scale[1]-=func_8008B2E4(0.025f)+0.01f;
            if(car->scale[1]<0.2f) car->scale[1]=0.2f;
        }
        if(car->scale[2]<0.58f)
            car->scale[2]=0.55f-func_8008B2E4(0.1);
        else {
            car->scale[2]-=func_8008B2E4(0.025f)+0.01f;
            if(car->scale[2]<0.5f) car->scale[2]=0.5f;
        }
    } else {
        mph=m->s7D0;
        scale1=0.2f+(1.35f*mph)/10000.0f;
        if(scale1>1.0f) scale1=1.0f;
        car->scale[0]=scale1-func_8008B2E4(0.15f);
        car->scale[1]=scale1-func_8008B2E4(0.15f);
        scale1=0.5f+mph/10000.0f;
        if(scale1>1.0f) scale1=1.0f;
        car->scale[2]=scale1-func_8008B2E4(0.1);
    }
    flames[0]->m[6]*=car->scale[0];
    flames[0]->m[7]*=car->scale[0];
    flames[0]->m[8]*=car->scale[0];
    flames[1]->m[6]*=car->scale[1];
    flames[1]->m[7]*=car->scale[1];
    flames[1]->m[8]*=car->scale[1];
    if(b)
        func_8008B32C(flames[2]->m,flames[2]->m,
                car->scale[2]);
    else {
        flames[2]->m[0]*=car->scale[2];
        flames[2]->m[1]*=car->scale[2];
        flames[2]->m[2]*=car->scale[2];
        flames[2]->m[3]*=car->scale[2];
        flames[2]->m[4]*=car->scale[2];
        flames[2]->m[5]*=car->scale[2];
    }
    if(D_8014A110==6 && m->s6C4>=0) {
        D_8012E700[D_80139320[slot].id[1]].flags |=0x80000000;
        D_8012E700[D_80139320[slot].id[2]].flags |=0x80000000;
        D_8012E700[D_80139320[slot].id[3]].flags |=0x80000000;
    } else if(D_80140418 || (car->flagsE8&8)) {
        D_8012E700[D_80139320[slot].id[1]].flags ^=0x80000000;
        D_8012E700[D_80139320[slot].id[2]].flags ^=0x80000000;
        D_8012E700[D_80139320[slot].id[3]].flags ^=0x80000000;
    } else {
        D_8012E700[D_80139320[slot].id[1]].flags &=0x7fffffff;
        D_8012E700[D_80139320[slot].id[2]].flags &=0x7fffffff;
        D_8012E700[D_80139320[slot].id[3]].flags &=0x7fffffff;
    }
}
}
