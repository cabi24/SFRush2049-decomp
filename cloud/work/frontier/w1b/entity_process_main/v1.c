/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

extern f32 fabsf(f32);
#pragma intrinsic(fabsf)

typedef struct Visual {
    /* 0x00 */ u8 pad0[6];
    /* 0x06 */ s16 objnum;
    /* 0x08 */ s16 slot;
    /* 0x0A */ u8 padA[0x14 - 0x0A];
    /* 0x14 */ void (*func)(struct Visual *v, s16 op);
} Visual;

typedef struct {
    /* 0x00 */ s16 active;
    /* 0x02 */ u16 flags;
    /* 0x04 */ u8 pad[0x54];
} Poly;

typedef struct {
    /* 0x000 */ u8 pad0[0x74];
    /* 0x074 */ f32 dr_tirepos[4][3];
    /* 0x0A4 */ u8 padA4[0xE8 - 0xA4];
    /* 0x0E8 */ u32 flags;
    /* 0x0EC */ u8 padEC[0x35C - 0xEC];
    /* 0x35C */ s8 unk35C;
    /* 0x35D */ s8 unk35D;
    /* 0x35E */ s8 unk35E;
    /* 0x35F */ s8 unk35F;
    /* 0x360 */ u8 pad360[0x3B8 - 0x360];
} CarData;

typedef struct {
    /* 0x000 */ u8 pad0[8];
    /* 0x008 */ u8 body_type;
    /* 0x009 */ u8 pad9[0x6C4 - 9];
    /* 0x6C4 */ s16 moving_state;
    /* 0x6C6 */ u8 pad6C6[0x76C - 0x6C6];
    /* 0x76C */ f32 airdist[4];
    /* 0x77C */ u8 pad77C[0x7DF - 0x77C];
    /* 0x7DF */ s8 hide_car;
    /* 0x7E0 */ u8 pad7E0[0x808 - 0x7E0];
} ModelDat;

extern CarData D_80152818[];
extern ModelDat D_8014A250[];
extern Poly D_8015B268[];
extern s8 D_80140418;
extern f32 D_8011F914[][4];
extern u8 D_8011AD8C[4];

extern void func_8008D0C0(Poly *p);
extern void func_8008C074(Poly *record, s32 count, f32 (*positions)[3], u16 parameter, u8 *color, u16 flags, s32 indexed);

#define rng(x, lo, hi) (((x) < (lo)) ? (lo) : (((x) > (hi)) ? (hi) : (x)))

void entity_process_main(Visual *v, s16 op) {
    f32 pos[3], mat[3][3], xyz[4][3], dist[4], d1, d2;
    s16 i, whl;
    CarData *car = &D_80152818[v->slot];
    ModelDat *m = &D_8014A250[v->slot];
    f32 wscale, fscale, bscale, len, offset;
    f32 *scale;

    if (op == 0) {
        if (v->objnum >= 0) {
            func_8008D0C0(&D_8015B268[v->objnum]);
        }
        v->func = 0;
        v->objnum = -1;
        return;
    }

    d1 = m->airdist[3] - m->airdist[1];
    d2 = m->airdist[2] - m->airdist[0];
    wscale = m->airdist[1] - m->airdist[0];
    fscale = m->airdist[3] - m->airdist[2];
    if (fabsf(d1) > 20.0f || fabsf(d2) > 20.0f || fabsf(wscale) > 20.0f || fabsf(fscale) > 20.0f ||
        car->unk35F != 0 || m->moving_state > -1 || (car->flags & 8) || D_80140418 != 0 || m->hide_car) {
        D_8015B268[v->objnum].flags |= 0x8000;
        return;
    }

    for (d1 = i = 0; i < 4; i++) {
        d1 += (dist[i] = m->airdist[i]);
        if (dist[i] < 0.0f) {
            dist[i] = 0.0f;
        }
    }
    d1 *= 0.25f;

    for (i = 0; i < 4; i++) {
        whl = (i < 2) ? i : 5 - i;
        offset = (dist[whl] > 10.0f) ? 2.0f : dist[whl] * 0.1f + 1.0f;
        xyz[i][0] = car->dr_tirepos[whl][0];
        xyz[i][1] = car->dr_tirepos[whl][1] - dist[whl] + offset;
        xyz[i][2] = car->dr_tirepos[whl][2];
    }

    if (car->flags & 0x10) {
        scale = D_8011F914[13];
    } else {
        scale = D_8011F914[m->body_type];
    }

    wscale = scale[0];
    fscale = scale[1];
    for (i = 0; i < 3; i++) {
        len = xyz[0][i] - xyz[1][i];
        xyz[0][i] += len * wscale;
        xyz[1][i] -= len * wscale;

        len = xyz[3][i] - xyz[2][i];
        xyz[3][i] += len * fscale;
        xyz[2][i] -= len * fscale;
    }

    fscale = scale[2];
    bscale = scale[3];
    for (i = 0; i < 3; i++) {
        len = xyz[0][i] - xyz[3][i];
        xyz[0][i] += len * fscale;
        xyz[3][i] -= len * bscale;

        len = xyz[1][i] - xyz[2][i];
        xyz[1][i] += len * fscale;
        xyz[2][i] -= len * bscale;
    }

    D_8015B268[v->objnum].flags &= 0x7FFF;
    D_8011AD8C[3] = 255.0f - rng(d1 * 8.0f, 20.0f, 255.0f);
    if (D_8011AD8C[3] > 192) {
        D_8011AD8C[3] = 192;
    }

    if (car->unk35C >= 0 && (car->unk35D == 0 || car->unk35D == 1)) {
        D_8015B268[v->objnum].flags &= ~(1 << car->unk35C);
    } else {
        D_8015B268[v->objnum].flags |= 0xF;
    }
    func_8008C074(&D_8015B268[v->objnum], 4, xyz, 0, D_8011AD8C, 0, 0);
}
