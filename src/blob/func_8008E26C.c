/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_8008E26C: allocate a 0x44-byte scene record in D_8012E700[] and link it under `parent`.
 *   Takes the first slot whose id (+0x14) is 0xFFFF, else appends one (count D_80156990, high-water
 *   mark D_801569A8); fills it (word +0 = d, +4 = 0, +8 = owner pointer b, scale 1.0/1.0, id = a,
 *   the three links +0x16/+0x18/+0x1A = -1, nine zero words) and calls render_mode_select(index,
 *   parent). Returns the index narrowed to s16. No arcade ancestor identified.
 *
 * STRICT MATCH (frontier wave 3, w3a). Shaping quirks (compile-affecting):
 *   - the byte offset is its own local computed right after the search loop (`off = i * sizeof(Ent)`)
 *     and the base is added after the two count updates: that is retail's i*68 in a3 at the loop
 *     exits and `addu v0,a3,t6` after the ifs (`&D_8012E700[i]` anywhere costs 59-68 words);
 *   - the compiled-out index check after the link call (`if (i < 0 || i >= D_80156990) DEBUG_PRINT`)
 *     keeps i live across the call: that is retail's `sw v1,32(sp)` home store that is never
 *     reloaded, while the returned (s16)i is the narrowed argument spilled at sp+24. A check against
 *     a constant does not work (uopt rewrites it onto `off`); `return (s16)i` (not a saved idx local);
 *   - declaration order `e, i, off` puts i's home at sp+32.
 *   Earlier attempts (w2f) reproduced the dead store with an address-taken index written through an
 *   inlined static's out-parameter; that form cannot hoist the offset without splitting the index web.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

#define DEBUG_PRINT(args)

typedef struct Ent {
    /* 0x00 */ s32 w0;
    /* 0x04 */ s32 w4;
    /* 0x08 */ void *owner;
    /* 0x0C */ f32 scaleA;
    /* 0x10 */ f32 scaleB;
    /* 0x14 */ u16 id;          /* 0xFFFF = free */
    /* 0x16 */ s16 link[3];     /* -1 = none */
    /* 0x1C */ s32 w28[9];
    /* 0x40 */ s32 w64;
} Ent; /* size 0x44 */

extern Ent D_8012E700[];
extern s32 D_80156990;  /* records in use */
extern s32 D_801569A8;  /* high-water mark */

void render_mode_select(s16 index, s16 parent);

s32 func_8008E26C(s32 a, void *b, s16 parent, s32 d) {
    Ent *e;
    s32 i;
    s32 off;

    for (i = 0; i < D_80156990; i++) {
        if (D_8012E700[i].id == 0xFFFF) {
            break;
        }
    }
    off = i * sizeof(Ent);
    if (i == D_80156990) {
        D_80156990++;
    }
    if (D_801569A8 < D_80156990) {
        D_801569A8 = D_80156990;
    }
    e = (Ent *)((u8 *)D_8012E700 + off);
    e->w0 = d;
    e->w4 = 0;
    e->owner = b;
    e->scaleA = 1.0f;
    e->scaleB = 1.0f;
    e->id = a;
    e->link[0] = -1;
    e->link[1] = -1;
    e->link[2] = -1;
    e->w28[0] = 0;
    e->w28[1] = 0;
    e->w28[2] = 0;
    e->w28[3] = 0;
    e->w28[4] = 0;
    e->w28[5] = 0;
    e->w28[6] = 0;
    e->w28[7] = 0;
    e->w28[8] = 0;
    render_mode_select(i, parent);
    if (i < 0 || i >= D_80156990) {
        DEBUG_PRINT(("scene record %d out of range\n", i));
    }
    return (s16)i;
}
