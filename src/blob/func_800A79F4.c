/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800A79F4 (0x800A79F4, 240 bytes): display-slot allocator. Scans the 32-byte slot table
 * D_80140BF0 (200 entries) for a free slot (state == 2) below the in-use count D_801613AC; returns -1
 * when the index reaches 200, grows the count when the slot is past it, raises the high-water mark
 * D_8013C234, then initialises the slot (id, two pointers, a/b, width/height and width-1/height-1,
 * alpha 255, state/flags 0) and returns its index. No arcade ancestor identified. Field names are labels.
 *
 * w15d (traced): the residual of earlier drafts was the order of the two ugen ring temps for h-1/w-1
 * and the param registers t1 (w) / t2 (h). Retail needs three things at once:
 *  - w's web numbered before h's (phase-2 colouring takes webs in first-appearance order): `s->w = w`
 *    before `s->hm1 = h - 1`;
 *  - both w and h live in the procedure's last basic block, so the return value's v0 is forbidden to
 *    them (otherwise the first gets v0);
 *  - h - 1 evaluated before w - 1 (h - 1 takes ring temp t6).
 * SHAPING DEVICE (disclosed): the empty `do {} while (0);` (an empty debug-macro expansion; it emits no
 * code) ends a uopt basic block, so the -1 stores and the state clears form the last block. Equivalent
 * compiled-out forms also match (`if (i) {}`, `if (s == 0) {}`, `if (h <= 0) {}`); an unused label does not.
 * No volatile, no unused locals, no pad arrays.
 */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32;

typedef struct {
    void *p0;   /* 0x00 */
    void *p4;   /* 0x04 */
    s16 id;     /* 0x08 */
    s16 a;      /* 0x0A */
    s16 b;      /* 0x0C */
    s16 e;      /* 0x0E */
    s16 w;      /* 0x10 */
    s16 h;      /* 0x12 */
    u8 alpha;   /* 0x14 */
    u8 f15;     /* 0x15 */
    s8 state;   /* 0x16 */
    s8 f17;     /* 0x17 */
    s16 x18;    /* 0x18 */
    s16 x1A;    /* 0x1A */
    s16 hm1;    /* 0x1C */
    s16 wm1;    /* 0x1E */
} Slot;

extern Slot D_80140BF0[];
extern s32 D_801613AC;
extern s32 D_8013C234;

s32 func_800A79F4(s32 id, void *p4, void *p0, s32 a, s32 b, s32 w, s32 h)
{
    Slot *s;
    s32 i;

    for (i = 0; i < D_801613AC; i++) {
        if (D_80140BF0[i].state == 2) {
            break;
        }
    }
    if (i >= 200) {
        return -1;
    }
    s = &D_80140BF0[i];
    if (i >= D_801613AC) {
        D_801613AC++;
    }
    if (D_8013C234 < D_801613AC) {
        D_8013C234 = D_801613AC;
    }
    
    s->b = b;
    s->p4 = p4;
    s->p0 = p0;
    s->id = id;
    s->a = a;
    s->e = 0;
    s->alpha = 255;
    s->x18 = 0;
    s->x1A = 0;
    s->w = w;
    s->h = h;
    do {} while (0);
    s->hm1 = h - 1;
    s->wm1 = w - 1;
    s->state = 0;
    s->f15 = 0;

    return i;
}
