/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * Historical label only. Accumulates one object's statistics block
 * (object->model->holder->statistics, 1,780 bytes: 140-byte prefix, 12 x 96,
 * 4 x 64, 8 x 12, 1 x 24 and 4 x 28 byte records) into the global totals at
 * D_80150F88 / D_80151410 / D_80151578 / D_801515F8 / D_80151618: u16 counters
 * and u32/f32 sums are added, flag words are OR-ed, maxima are kept, and the
 * single D record keeps the smallest positive time (0 = not yet set).
 * No arcade ancestor identified (N64 statistics screen data).
 * Layout recovered by cloud/work/game_C38 (65 words off there); the two levers
 * that close it:
 *   - the one-record D loop is written `i != 1` (gives the `bne` pointer test;
 *     `i < 1` gives `sltu`/`bnez`);
 *   - the last loop uses its own counter `k` (with `i` reused, the destination
 *     and end pointers swap v1/a0).
 * Also MATCH at -O2 with the same source.
 */
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
extern StatsA D_80150F88[12];
extern StatsB D_80151410[4];
extern StatsC D_80151578[8];
extern StatsD D_801515F8[1];
extern StatsE D_80151618[4];
void func_800F769C(Object *object) {
    Statistics *s;
    s32 i, j, k;
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
    for (i = 0; i != 1; i++) {
        if (s->d[i].minimum > 0.0f && (D_801515F8[i].minimum == 0.0f || s->d[i].minimum < D_801515F8[i].minimum)) D_801515F8[i].minimum = s->d[i].minimum;
        D_801515F8[i].total += s->d[i].total;
        D_801515F8[i].count[0] += s->d[i].count[0];
        D_801515F8[i].count[1] += s->d[i].count[1];
        D_801515F8[i].count[2] += s->d[i].count[2];
        D_801515F8[i].count[3] += s->d[i].count[3];
        D_801515F8[i].count[4] += s->d[i].count[4];
    }
    for (k = 0; k < 4; k++) {
        D_80151618[k].count[0] += s->e[k].count[0];
        D_80151618[k].count[1] += s->e[k].count[1];
        D_80151618[k].count[2] += s->e[k].count[2];
        D_80151618[k].count[3] += s->e[k].count[3];
        D_80151618[k].count[4] += s->e[k].count[4];
        D_80151618[k].count[5] += s->e[k].count[5];
        if (D_80151618[k].maximum < s->e[k].maximum) D_80151618[k].maximum = s->e[k].maximum;
        D_80151618[k].total += s->e[k].total;
    }
}
