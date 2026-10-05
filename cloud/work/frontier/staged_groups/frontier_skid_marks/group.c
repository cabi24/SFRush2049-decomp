/*
 * ---- unit: cpak_init + func_800AF8C0 + save_validate (frontier wave 1, agent w1g) ----
 * Skid marks, N64 port of the arcade code in reference/repos/rushtherock/game/visuals.c (labels are
 * historical; nothing here is Controller Pak or save code):
 *   cpak_init      = the per-car skid driver: arcade DoSkid() for each of the four tires, written as one loop
 *   func_800AF8C0  = UpdateSkid(ns, slot, tire, color)
 *   save_validate  = StartSkid(ns, slot, tire, color), with GetSkid() inlined
 *   func_800AFD54  = ContinueSkid() -- inlined into cpak_init; retail keeps its `jr ra` stub at 0x800AFD54
 *   func_800AFB30  = GetSkid()      -- inlined into save_validate; stub at 0x800AFB30 (0x800AFB28 is a
 *                                      second deleted static next to it, not identified)
 *   func_800AF844  = StopSkid(ns), func_800AFA84 = ReleaseSkid-style list unlink (context)
 * WheelSlot is the arcade NewSkid (obj=skid, start, pos=end, lenSq, dir, quad/left/right = vert[4][3]);
 * EffectObj is the arcade Skid (pool link, f8=lastTime as f32, dl=objnum, pos, colour instead of xlu).
 * This is a CLOSED unit: every callee is present as context (unchanged copies of the locked sources), and
 * there are no stand-in callers.  func_800AF844 must NOT be in `keep`: only then does IPA know it leaves
 * $t0-$t4 alone, which is what puts cpak_init's loop state in $t0/$t2/$t3/$t4 instead of $s1/$s4/$s5.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

float sqrtf(float);
#pragma intrinsic (sqrtf)

typedef struct EffectObj {
    /* 0x00 */ struct EffectObj *next;   /* pool list link */
    /* 0x04 */ u8 pad4[4];
    /* 0x08 */ f32 f8;
    /* 0x0C */ void *dl;                 /* display handle */
    /* 0x10 */ f32 pos[3];
    /* 0x1C */ u8 color[4];
    /* 0x20 */ s32 f20;
} EffectObj;

typedef struct {
    /* 0x00 */ EffectObj *obj;
    /* 0x04 */ f32 start[3];
    /* 0x10 */ f32 pos[3];
    /* 0x1C */ f32 lenSq;
    /* 0x20 */ f32 dir[3];
    /* 0x2C */ f32 quad[6];             /* 0x2C..0x40: previous edge */
    /* 0x44 */ f32 left[3];
    /* 0x50 */ f32 right[3];
} WheelSlot;                            /* 0x5C */

typedef struct {
    /* 0x000 */ u8 pad0[0x74];
    /* 0x074 */ f32 wheelPos[4][3];
    /* 0x0A4 */ u8 padA4[0xE8 - 0xA4];
    /* 0x0E8 */ u32 wheelFlags;
    /* 0x0EC */ u8 padEC[0x3B8 - 0xEC];
} Car;                                  /* 0x3B8 */

typedef struct {
    /* 0x000 */ u8 pad0[0x5A0];
    /* 0x5A0 */ u16 surface[4];
    /* 0x5A8 */ u8 pad5A8[0x61C - 0x5A8];
    /* 0x61C */ u16 terrain[4];
    /* 0x624 */ u8 pad624[0x6C4 - 0x624];
    /* 0x6C4 */ s16 airborne[4];
    /* 0x6CC */ u8 pad6CC[0x75C - 0x6CC];
    /* 0x75C */ f32 wheelHeight[4];
    /* 0x76C */ u8 pad76C[0x808 - 0x76C];
} CarState;                             /* 0x808 */

typedef struct {
    /* 0x0 */ u8 pad0[2];
    /* 0x2 */ u16 flags;
    /* 0x4 */ u8 pad4[0x18 - 4];
} Surface;                              /* 0x18 */

extern Car player_array[8];
extern CarState D_8014A250[];
extern WheelSlot D_80155290[][4];
extern s16 active_player_count;
extern s32 state_word_a;
extern u32 D_8011743C[4];               /* per-wheel flag masks */
extern u8 D_80117438[];                 /* default mark colour */
extern u8 D_8011AD8C[];                 /* colour on terrain 0 */
extern f32 D_8011AD90[];
extern Surface *D_801497F8;
extern f32 D_80123C08;
extern f32 D_80123C0C;                  /* minimum travel before a mark starts */
extern f32 D_8002EB90[];
extern char D_80155220[];               /* EffectObj pool */
typedef struct {
    /* 0x00 */ f32 uvs[3][3];
    /* 0x24 */ f32 pos[3];
} CamXform;
extern CamXform D_80150B70;             /* camera matrix + position (pos = old D_80150B94) */
extern u16 D_80161378;

extern EffectObj *func_8008E3C0(void *pool);
extern void func_800AFA84(void *pool, EffectObj *obj);
extern void func_8008D0C0(void *dl);
extern void *func_800A78BC(s32, f32 *, u16, u8 *, s32, s32);
extern void func_8008C074(void *dl, s32 n, f32 *quad, s32, u8 *color, s32, s32);
extern void vector_copy_scale(f32 *in, f32 *out);
extern void func_800AF844(WheelSlot *slot);

void cpak_init(s32 slot);
void func_800AF8C0(WheelSlot *slot, s16 player, u32 wheel, u8 *color);
void save_validate(WheelSlot *slot, s16 player, u32 wheel, u8 *color);

/* func_800AF8C0 (0x800AF8C0, 113 words) = arcade UpdateSkid: place the skid's leading edge across the
 * axle (tires tire|1 and tire&2), 0.75 either side of the tire, lifted 0.8; stamp time and colour; update
 * the polygon.  IPA parameters: ns $s0, slot $t1, tire $s4, color $s5.
 * Quirk for strictness: the 0.8f lift is this function's own .rodata (0x80123C08).  It is read through the
 * extern into a local `up` declared above the vectors (same code as the natural `+= 0.8f`, which scores
 * "2 relocations unverified"); `car` must stay unnamed and `m` named for the frame and registers.
 */
void func_800AF8C0(WheelSlot *slot, s16 player, u32 wheel, u8 *color)
{
    CarState *m = &D_8014A250[player];
    EffectObj *obj = slot->obj;
    s16 i;
    f32 up;
    f32 orth[3];
    f32 dir[3];
    f32 wid;

    orth[0] = player_array[player].wheelPos[wheel | 1][0] - player_array[player].wheelPos[wheel & 2][0];
    orth[1] = player_array[player].wheelPos[wheel | 1][1] - player_array[player].wheelPos[wheel & 2][1];
    orth[2] = player_array[player].wheelPos[wheel | 1][2] - player_array[player].wheelPos[wheel & 2][2];
    orth[1] += m->wheelHeight[wheel | 1] - m->wheelHeight[wheel & 2];
    vector_copy_scale(orth, dir);
    for (i = 0; i < 3; i++) {
        wid = dir[i] * 0.75f;
        slot->pos[i] = player_array[player].wheelPos[wheel][i];
        slot->left[i] = slot->pos[i] - wid;
        slot->right[i] = slot->pos[i] + wid;
    }
    up = D_80123C08;
    slot->left[1] += up;
    slot->right[1] += up;
    obj->f8 = *(f32 *) ((u32) &D_8002EB90[0]);
    obj->color[0] = color[0];
    obj->color[1] = color[1];
    obj->color[2] = color[2];
    obj->color[3] = 0xC0;
    func_8008C074(obj->dl, 4, slot->quad, 0, obj->color, 0, 0);
}

/* func_800AFB30 = arcade GetSkid: take a skid from the pool, or steal the one farthest from the camera.
 * Not `static`, not in `keep`: umerge inlines it into its only caller and leaves the retail `jr ra` stub. */
EffectObj *func_800AFB30(u8 *color)
{
    s32 i;
    EffectObj *s;
    EffectObj *fars;
    f32 d;
    f32 dsq;
    f32 fardsq;

    s = func_8008E3C0(D_80155220);
    if (s == 0) {
        fardsq = -1.0f;
        fars = ((EffectObj **) D_80155220)[4];
        for (s = ((EffectObj **) D_80155220)[4]; s->next; s = s->next) {
            for (dsq = 0.0f, i = 0; i < 3; i++) {
                d = s->pos[i] - D_80150B70.pos[i];
                dsq += d * d;
            }
            if (dsq > fardsq) {
                fardsq = dsq;
                fars = s;
            }
        }
        s = fars;
        func_8008D0C0(s->dl);
        func_800AFA84(D_80155220, s);
        s = func_8008E3C0(D_80155220);
    }
    s->f20 = 0;
    s->color[0] = color[0];
    s->color[1] = color[1];
    s->color[2] = color[2];
    s->color[3] = 0xC0;
    s->dl = func_800A78BC(4, D_8011AD90, D_80161378, color,
                          ((state_word_a & 0x100) ? 1 : 15) | 0x1200, 1);
    if (s->dl == 0) {
        func_800AFA84(D_80155220, s);
        return 0;
    }
    return s;
}

/* save_validate (0x800AFB38, 135 words) = arcade StartSkid.  IPA parameters: ns $s6, slot $s3, tire $s4,
 * color $s5.  The three veccopy() macros are written out element by element as in vecmath.h. */
void save_validate(WheelSlot *ns, s16 slot, u32 tire, u8 *color)
{
    EffectObj *s;

    s = func_800AFB30(color);
    if (s == 0) {
        return;
    }
    ns->obj = s;
    ns->lenSq = 0.0f;
    func_800AF8C0(ns, slot, tire, color);
    ns->start[0] = ns->pos[0]; ns->start[1] = ns->pos[1]; ns->start[2] = ns->pos[2];
    ns->quad[0] = ns->right[0]; ns->quad[1] = ns->right[1]; ns->quad[2] = ns->right[2];
    ns->quad[3] = ns->left[0]; ns->quad[4] = ns->left[1]; ns->quad[5] = ns->left[2];
    func_8008C074(s->dl, 4, ns->quad, 0, 0, 0, 0);
}

#define magsq(v) (((v)[0] * (v)[0]) + ((v)[1] * (v)[1]) + ((v)[2] * (v)[2]))

/* func_800AFD54 = arcade ContinueSkid: extend the current skid, or end it and start a new one when the
 * tire has left the skid's line by more than 1.0 (squared) or the skid got shorter.  Inlined into cpak_init.
 * No named `car` here and magsq as a macro: with either as a named local / inline function the frame is one
 * word too big.  0.1f is cpak_init's own .rodata (0x80123C0C, hoisted into $f26): natural literal,
 * reported unverified by the scorer (an extern is not hoisted out of the loop). */
void func_800AFD54(WheelSlot *ns, s16 slot, u32 tire, u8 *color)
{
    f32 vec[3];
    f32 dirpos[3];
    f32 lensq;
    f32 len;
    f32 ds;
    f32 invlen;

    func_800AF8C0(ns, slot, tire, color);

    vec[0] = player_array[slot].wheelPos[tire][0] - ns->start[0];
    vec[1] = player_array[slot].wheelPos[tire][1] - ns->start[1];
    vec[2] = player_array[slot].wheelPos[tire][2] - ns->start[2];
    lensq = magsq(vec);

    if (ns->lenSq > 0.0f) {
        len = sqrtf(lensq);
        dirpos[0] = ns->dir[0] * len;
        dirpos[1] = ns->dir[1] * len;
        dirpos[2] = ns->dir[2] * len;
        vec[0] = ns->pos[0] - dirpos[0];
        vec[1] = ns->pos[1] - dirpos[1];
        vec[2] = ns->pos[2] - dirpos[2];
        ds = (vec[0] * vec[0]) + (vec[1] * vec[1]) + (vec[2] * vec[2]);
        if ((ds > 1.0f) || (lensq < ns->lenSq)) {
            func_800AF844(ns);
            save_validate(ns, slot, tire, color);
        }
    } else if (lensq > 0.1f) {
        invlen = 1.0f / sqrtf(lensq);
        ns->dir[0] = vec[0] * invlen;
        ns->dir[1] = vec[1] * invlen;
        ns->dir[2] = vec[2] * invlen;
        ns->lenSq = lensq;
    }
}

/* cpak_init (0x800AFD5C, 265 words): for each tire, start / continue / stop its skid (arcade DoSkid).
 * `slot` is int here (no entry sll/sra; the s16 callee parameters are read back with `lh` from its home).
 * Quirks: colour chosen with ?: (the if-form drops the `b` and swaps $t0/$t2); `flags & mask[i]` operand
 * order; `ns` walks with the loop. */
void cpak_init(s32 slot)
{
    s32 i;
    CarState *m;
    WheelSlot *ns;
    s32 on;
    s32 laston;
    u8 *color;

    if (active_player_count >= 3) {
        return;
    }
    m = &D_8014A250[slot];
    ns = D_80155290[slot];
    for (i = 0; i < 4; i++, ns++) {
        on = ((player_array[slot].wheelFlags & D_8011743C[i]) != 0) && (m->airborne[0] == -1) &&
             (m->terrain[i] != 8) && ((D_801497F8[m->surface[i]].flags & 0x30) == 0);
        laston = (ns->obj != 0);
        color = (m->terrain[i] == 0) ? D_8011AD8C : D_80117438;

        if (!laston && on) {
            save_validate(ns, slot, i, color);
        } else if (laston && on) {
            func_800AFD54(ns, slot, i, color);
        } else if (laston && !on) {
            func_800AF844(ns);
        }
    }
}

