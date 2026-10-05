# Particle-system knot — structure (w6a, 2026-10-05)

Supersedes `cloud/work/frontier/w5b/particle_unit/STRUCTURE.md` (left unchanged). Names are historical labels.
Source: `groups/particle_knot/group.c` (assembled from `dev/part_*.c` by `dev/build.sh`); the header comment of
that file lists every shaping fact. Scores are in `RESULTS.md`.

## Call graph and IPA facts

```
Input_ProcessGameplayPad (unmatched; loop over views, D_80146204 = count)
 `-> func_8009F058(view, -, -, &D_80150B70[view])     a1/a2 never set by the caller (dead params)
       |-> physics_float_calc(view, eye->pos, 0)      (unmatched, ABI)
       |-> track_collision_wall(&D_801497C8, NULL, root, view, 0, 1, 0, NULL)   x3, recursive
       |     `-> particle_system: obj in s7, view in t0, gfx** passed in the a0 home slot, 7th arg on stack
       |           |-> render_display_list: gp s4, tex s1, rect s0, clip s3, pals s5; clobbers s2
       |           |     `-> func_80099B30 (TLUT load)
       |           |-> func_80099B30 x2
       |           `-> func_8009C3F8(x in $f16, flag in a0)   <- camera_update_c (asinf), select_screen_update
       |                                                         (acosf), camera_follow_path, func_800BFD8C
       `-> render_display_list (2D poly textures)
```

* particle_system and func_8009F058 save only `ra`: every s-register they write is IPA "unsaved"; their callers
  re-materialise or reload around the call (track_collision_wall reloads s0..s8 constants after each
  particle_system call).
* Parameter order is the order the callers set the registers up, not the register numbers:
  `render_display_list(Gfx **gp, Texture *tex, TexRect *rect, TexRect *clip, Palette *pals)` (w5b's
  `(tex, rect, clip, gp, pals)` gives the same callee but the wrong set-up order in particle_system), and
  `func_8009C3F8(f32 x, s32 flag)` (the wrappers then emit `mov.s $f16,$f12` before `move a0,zero`).

## Types recovered

```c
typedef struct { u8 pad0[16]; u16 width, height; u8 siz, fmt; s16 palette; void *image; u32 flags; u32 unk20; } Texture; /* 36, D_80151AE8[] tables */
typedef struct { u16 x0, y0, x1, y1; } TexRect;                  /* 10.5 fixed point */
typedef struct { u8 pad0[16]; u8 first, last; u8 pad12[2]; s32 table; } Palette; /* 24 */
typedef struct { s16 unk0; s16 count; u16 tex; u16 flags; f32 dist; Gfx *dl; } LodLevel; /* level k at +20+16k */
typedef struct { u8 pad0[20]; LodLevel lv[4]; u8 pad54[4]; } LodSet;  /* 88 bytes; count = lv[0].count (+22) */
typedef struct Node {                 /* D_8012E700[], 0x44 */
    u32 flags;        /* 0x00 bit31 skip, 0x80000 billboard, 0x400000 abs matrix, 0x10000 scaled view matrix,
                         0x8000 billboard matrix, 0x1000 vertex, 0x80 no matrix, 0x40000/0x800000 deferred
                         layers, 0x100<<view hidden-in-view, 0x10/0x20 cull box, 8 force distance, 4 forced LOD */
    u32 lights;       /* 0x04 &7 = light count, (&0x1FF8)>>3 = index into D_80157248 (Vtx) */
    View *obj;        /* 0x08 rot[3][3] + pos[3] */
    f32 scale;        /* 0x0C */
    f32 w;            /* 0x10 matrix w */
    u16 model;        /* 0x14 >>10 table, &0x3FF entry (D_801161F4 LodSet / D_80138670 palettes) */
    s16 child, next;  /* 0x16, 0x18 */
    s16 mode;         /* 0x1A 0/1/2 = SetLights0/1/2 from `light` */
    Lights2 *light;   /* 0x1C */
    Texture *tex[4];  /* 0x20 per LOD */
    TexRect *rect;    /* 0x30 */
    TexRect *clip;    /* 0x34 (particle_system points it at a stack TexRect for billboards, then clears it) */
    Palette *pal;     /* 0x38 */
    u8 prim[4], env[4]; /* 0x3C, 0x40 */
} Node;
typedef struct { f32 rot[3][3]; f32 pos[3]; u8 pad30[0x68]; } View;     /* D_80150B70[], 0x98 */
typedef struct { Vp vp[4]; Vtx vtx[4]; Mtx proj[4]; Mtx look[4]; LookAt la[32]; Mtx ident; } GfxBuf; /* *D_8015B250 */
typedef struct {                       /* D_8017A510[], 0x48, per-view camera */
    Vp *vp; u16 *scissor; s32 ortho; u8 pad0C[4]; f32 fov; u8 pad14[0x10];
    f32 w, h, x, y, aspect, near, far; u16 fogmin, fogmax; u8 fog[4];
} Camera;
typedef struct { f32 pos[3]; s16 tc[2]; u8 cn[4]; } PolyVtx;            /* 20 */
typedef struct {                       /* D_8015B268[], 0x58, overlay polygons, D_801613B4 = count */
    s16 count; u16 flags; u16 tex; s16 z;
    union { PolyVtx v[4]; struct { f32 x0, y0; u8 pad10[8]; u8 col[4]; f32 x1, y1; } rect; } u;
} Poly;
```

Globals: `D_801497C8` Gfx* (main list), `D_8015B250` GfxBuf*, `D_8015B254` s16 scene root, `D_8015B260` Mtx*
(matrix allocator), `D_80161434` Vtx* (vertex allocator), `D_80161430` Gfx* (patch point of the cull branch),
`D_8017A638` TLUT mode cache (0 none, 1 RGBA16, -1 unknown), `D_8017A4B0` TLUT address cache, `D_8017A508`
last 2D texture, `D_80124EE8` f32 LOD distance (.bss), `D_80124EF0`/`D_80124F30`/`D_801403D8` MtxF (view,
billboard, scaled view), `D_80124F78` f32[3] current node position, `D_80124F84` s8 LookAt slot,
`D_80124FC4`/`D_80124FE0` deferred-layer pass flags, `D_80124FCC`/`D_80124FE2` first deferred root (s16, -1 none),
`D_80151AA0` f32 light cull distance, `D_80151AD8` s8 (rotated look-at), `D_80140A04` s8 mirror mode,
`D_801613B8` f32 view scale, `D_801613F0` MtxF look-at, `D_8014A108` s16 player/view count.

## render_display_list (0x80099BFC, 2559 words) — texture loader. MATCH

w5b's body with two changes: parameter order `(gp, tex, rect, clip, pals)` and the tail's clip/rect tile-size
rewrite written int-typed:
`_g->words.w0 = (G_SETTILESIZE << 24) | (((uls) & 0xFFF) << 12) | ((ult) & 0xFFF)`. With the SDK `_SHIFTL`
form the 0xF2000000 constant of the tail and of the nine tile sections is one unsigned constant web; uopt gives
it `s2` once `masks` dies (saving one `lui` per section) — exactly w5b's 1-word residual. The traced allocator
showed it (`p1:w1056 save=0.09 color->s2`); forcing that web to split (`gforce.sh … p1:w1056=s`) reproduced
retail exactly, and the int-typed tail is the source form that does the same.

## func_80099B30 (204 B) — TLUT load. MATCH (agentB body; `D_80161430` typed `Gfx *`)

## func_8009C3F8 (452 B) — Cody–Waite asin/acos kernel. MATCH, own .rodata verified

`f32 func_8009C3F8(f32 x, s32 flag)`; |x| < 2.3e-10 → x; |x| ≥ 1 → π/2; |x| > 0.5 → `g = ((0.5f - y) + 0.5f)
/ 2.0f; y = -(2·sqrt g)`, i = 1 - flag; else g = y²; `r = (P(g)·g / Q(g)) * y + y`; tables `D_8011F010` (a[2], b[2]).
Shaping: `/ 2.0f` (uopt turns it into `* 0.5` after CSE, so the 0.5 is a fresh constant — retail rematerialises
it), one `return r` (all three exits end in `mov.s $f0,$f12`), `(a[i] + r) + a[i]` operand order.
`camera_update_c(x) = func_8009C3F8(x, 0)` (asinf) and `select_screen_update(x) = func_8009C3F8(x, 1)` (acosf)
match as kept members once the kernel is internal with `x` in $f16.

## particle_system (0x8009C8F0, 725 words) — draw one scene node. MATCH, own .rodata verified

`s32 particle_system(Gfx **gp, Node *parent, s32 count, s32 flag, Node *node, s32 view, Palette *pal)`.
1. LOD: `set = &D_801161F4[model >> 10].sets[model & 0x3FF]; lod = set->lv[0].count - 1;` forced LOD
   (`flags & 4`) sets the cached distance from `set->lv[lod - 1].dist`; otherwise the distance to the view is
   recomputed when `(lv.dist != 0 && !hidden && !flag) || (flags & 8)` from `x/y/z` locals
   (`d[i] = parent->obj->pos[i] + node->obj->pos[i]` for count 1, minus the view position).
2. Reject when hidden in this view or beyond `lv.dist * scale`; pick the LOD (`else if (lod > 0) while (lod != 0 &&
   D < set->lv[lod - 1].dist * node->scale) lod--;`); `dl = set->lv[lod].dl`.
3. Billboard (`flags & 0x80000`): `camera_update_d` (LookAt) from the view to `D_80124F78`, LookAt/geometry/cycle
   commands, texture scale, then yaw = `func_8008C768(dir[0], dir[2])`, pitch = `func_8009C3F8(dir[1], 0)` →
   a stack TexRect `{-yaw·4096/π + 6144, -pitch·4096/π + 2048, +4064, +2016}` stored in `node->clip`.
4. Palette, texture (`render_display_list` with the node's texture or `(id & 0x3FF) + D_80151AE8[id >> 10].tex`),
   or a SetTileSize rewrite from clip/rect; prim colour with distance fade (alpha · (80 − D) / 56 between 24 and 80),
   env colour, `gSPDisplayList(dl)`, TLUT restore, second palette load, billboard state restore.
Frame: `gfx, set, lod, rect, dl, v, d[3], drawn, dir[3]`, then pals/alpha/x/y/z/yaw/pitch (yaw/pitch supply 8
bytes that retail's uopt reserves as spill-temp homes, see RESULTS "frame").

## track_collision_wall (0x8009DD88, 678 words) — scene-graph walk (recursive). MATCH

`s32 track_collision_wall(Gfx **gp, Node *parent, Node *node, s32 view, s32 count, s32 more_in, s32 flags6,
Palette *pal)`. Sibling loop over `node->next`; per node: skip (bit 31); deferred layers 0x40000/0x800000 record
their first index in `D_80124FCC`/`D_80124FE2` unless that pass is active; `track_collision_edge` visibility;
then (unless a hidden leaf) optional vertex load, matrix (`func_8009D99C` with view, scaled or billboard matrix,
`func_8009D708`, `func_8009D45C`) pushed with `more` = (has sibling or caller asked), lights (SetLights0/1/2),
optional cull branch (`D_80161430` points at the BranchList operand; light-box distance cull against
`D_80151AA0`), `particle_system`, recursion into the child with `count + (more == 1)`, cull-branch patch or
rewind, PopMatrix or rewind.
Shaping (each tested): every early exit is an explicit `goto next` (an else-if chain makes PRE insert the
`node->next` load in a join block); `if (child >= 0 || !(flags & bit)) {…} else more = 0;`; `scale != 1.0f` arm
first; the scale loop's inner counter is the same `j` as the next loop (a private counter gets unrolled by 2);
two separate filter `if`s at the loop bottom; `s32 pushed = 0, total = 0, saved = 0;` (one line: as1 orders the
two stack stores by line); 16 bytes of frame filler (`s32 pad[4]`).

## func_8009F058 (0x8009F058, 1307 words) — per-view scene setup. 22 words off (frame only)

`void func_8009F058(s32 view, s32 arg1, s32 arg2, View *eye)`: physics_float_calc; viewport copy + scale;
scissor (u16 locals read before the viewport command), fog colour/position, cycle type, fog mode, depth
source, TLUT; ortho or perspective projection; look-at (three forms: rotated-uv with mirror, mirrored, plain)
and the inverse view matrix `D_80124EF0` (scaled by `D_801613B8` when ≠ 1), billboard matrix, vertex 0 =
view-matrix column × 480 (or 0 when more than one view); fixed-point look-at matrix, identity (X mirrored unless
`D_80140A04`); scene walk from `D_8015B254`; overlay polygons (`D_8015B268`): fill rectangles (flag 0x4000) or
3/4-vertex polygons in view space (range-checked against ±32768), fog/cycle and render-mode/combiner state
machine (`mode` −1/0/1/2/3, `fog`, `depth`), textures through `render_display_list`, triangle lists; TLUT/depth/fog
restore; deferred layers FCC then FE2.
Shaping facts found (each moved code): `Camera *cam = &D_8017A510[view];` at its declaration (a later assignment
lets ugen emit `.noalias $16,$sp`, and as1 then reorders the ortho arm and hoists `lw s1`); scissor values in
u16 locals; SDK `gSPFogPosition` (128000); `57.295776f`; `uv[3][3]`, `perspNorm`, `p[3]`, `tex` declared in the
blocks that use them (frame offsets 276/322/212/180 follow textual declaration order); the projection offset and
`&D_80150B70[view]` are expressions (CSE temps: s1, t0); `fog = 1; poly = D_8015B268; mode = -1;`; `i =
D_8014A108; if ((i < 2) != 0)`; the inner scale loop on one line (`for (j…) D_80124EF0[i][j] *= D_801613B8;`);
FillRectangle with `(s16)` coordinates and the y term ORed before the x term; the texture address recomputed as
the call argument (s1 param + v1 copy spilled to `tex`'s home); triangle lists (0x1000: (0,2,1)(2,0,1) and (2,0,3)(0,2,3);
mirror: (1,0,2) or (1,0,2)(0,3,2); else (2,0,1) or (2,0,1)(0,2,3)); the deferred-layer node pointer in a block
local before the two flag stores (`Node *n = &D_8012E700[root];` — with the pointer computed in the call, as1
hoists `lui at`/`move a0`/`move a1` into the test block instead of the index chain); one `s32 pad1` in the
ortho block (slot accounting, see RESULTS).
