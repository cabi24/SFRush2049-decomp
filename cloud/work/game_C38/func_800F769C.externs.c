/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;
typedef float f32;
typedef struct StatsA {u8 pad0[64]; f32 total; u16 count[9]; u8 pad86[2]; u32 sum; u16 flags; u8 pad94[2];} StatsA;
typedef struct StatsB {u32 first, max0, max1, total; u16 count0, count1; u32 sum[10]; u16 flags; u8 pad62[2];} StatsB;
typedef struct StatsC {u32 first; u16 count[4];} StatsC;
typedef struct StatsD {u32 first; f32 minimum, total; u16 count[5]; u8 pad22[2];} StatsD;
typedef struct StatsE {u32 first; u16 count[6], maximum; u8 pad18[6]; u32 total;} StatsE;
typedef struct Statistics {u8 pad0[140]; StatsA a[12]; StatsB b[4]; StatsC c[8]; StatsD d[1]; StatsE e[4];} Statistics;
typedef struct Holder {Statistics *statistics;} Holder;
typedef struct Model {u8 pad0[44]; Holder *holder;} Model;
typedef struct Object {Model *model;} Object;
extern StatsA D_80150F88[];
extern StatsB D_80151410[];
extern StatsC D_80151578[];
extern StatsD D_801515F8[];
extern StatsE D_80151618[];
void func_800F769C(Object *object) {
    Statistics *s;
    s32 i, j;
    if (!object->model->holder) return;
    s = object->model->holder->statistics;
    for (i = 0; i < 12; i++) {
        D_80150F88[i].total += s->a[i].total;
        D_80150F88[i].count[0] += s->a[i].count[0];
        D_80150F88[i].count[1] += s->a[i].count[1];
        D_80150F88[i].count[2] += s->a[i].count[2];
        D_80150F88[i].count[3] += s->a[i].count[3];
        D_80150F88[i].count[4] += s->a[i].count[4];
        D_80150F88[i].count[5] += s->a[i].count[5];
        D_80150F88[i].count[6] += s->a[i].count[6];
        D_80150F88[i].count[7] += s->a[i].count[7];
        D_80150F88[i].count[8] += s->a[i].count[8];
        D_80150F88[i].sum += s->a[i].sum;
        D_80150F88[i].flags |= s->a[i].flags;
    }
    for (i = 0; i < 4; i++) {
        if (D_80151410[i].max0 < s->b[i].max0) D_80151410[i].max0 = s->b[i].max0;
        if (D_80151410[i].max1 < s->b[i].max1) D_80151410[i].max1 = s->b[i].max1;
        D_80151410[i].total += s->b[i].total;
        D_80151410[i].count0 += s->b[i].count0;
        D_80151410[i].count1 += s->b[i].count1;
        for (j = 0; j < 10; j++) D_80151410[i].sum[j] += s->b[i].sum[j];
        D_80151410[i].flags |= s->b[i].flags;
    }
    for (i = 0; i < 8; i++) {
        D_80151578[i].count[0] += s->c[i].count[0];
        D_80151578[i].count[1] += s->c[i].count[1];
        D_80151578[i].count[2] += s->c[i].count[2];
        D_80151578[i].count[3] += s->c[i].count[3];
    }
    for (i = 0; i < 1; i++) {
        if (s->d[i].minimum > 0.0f && (D_801515F8[i].minimum == 0.0f || s->d[i].minimum < D_801515F8[i].minimum)) D_801515F8[i].minimum = s->d[i].minimum;
        D_801515F8[i].total += s->d[i].total;
        D_801515F8[i].count[0] += s->d[i].count[0];
        D_801515F8[i].count[1] += s->d[i].count[1];
        D_801515F8[i].count[2] += s->d[i].count[2];
        D_801515F8[i].count[3] += s->d[i].count[3];
        D_801515F8[i].count[4] += s->d[i].count[4];
    }
    for (i = 0; i < 4; i++) {
        D_80151618[i].count[0] += s->e[i].count[0];
        D_80151618[i].count[1] += s->e[i].count[1];
        D_80151618[i].count[2] += s->e[i].count[2];
        D_80151618[i].count[3] += s->e[i].count[3];
        D_80151618[i].count[4] += s->e[i].count[4];
        D_80151618[i].count[5] += s->e[i].count[5];
        if (D_80151618[i].maximum < s->e[i].maximum) D_80151618[i].maximum = s->e[i].maximum;
        D_80151618[i].total += s->e[i].total;
    }
}
