# w7a results — particle-system knot, stand-ins removed (2026-10-05)

Builder scratch `~/rush2049/scratch/frontier/w7a` (copied from `w6a`, `src/` and the lock refreshed from `base`),
unit tag `w7a`. Nothing committed or spliced; nothing under `src/`, `tools/`, `include/`, `asm/` or `tests/`
touched; `func_80087110` and `stat_race_update`/`func_800FE5B0` not touched.

Deliverable: `groups/particle_knot/` (`group.json`, `group.c`, `score.txt`, `unit_score.txt`). `group.c` is
assembled by `dev/mk.sh` from w6a's parts plus `dev/part_ipgp.c` (Input_ProcessGameplayPad) and `dev/tail.c`
(the two kept asinf/acosf wrappers). Sweeps: `runs/*` (Input_ProcessGameplayPad), `runs/f/*` (func_8009F058),
`runs/sw1/*`. Tools: `tools/README.md` (w6a's set retargeted, plus the spill-home tracer below).

## Table

| Function | Bytes | State |
|---|---:|---|
| `Input_ProcessGameplayPad` | 2,720 | **strict MATCH** (new; kept member, real caller of the chain) |
| `render_display_list` | 10,236 | MATCH, EQUAL in unit |
| `particle_system` | 2,900 | MATCH (own .rodata verified), EQUAL in unit |
| `track_collision_wall` | 2,712 | MATCH, EQUAL in unit |
| `func_8009C3F8` | 452 | MATCH (own .rodata verified), EQUAL in unit |
| `func_80099B30` | 204 | MATCH, EQUAL in unit |
| `camera_update_c` / `select_screen_update` | 36 / 36 | MATCH, EQUAL in unit (kept) |
| `func_8009F058` | 5,228 | **22/1307 words off** — unchanged: only the slot of the `&D_80150B70[view]` spill home (68 vs 72) |

Flags `-g0 -O3 -mips2 -G 0 -non_shared` (whole-program group). The group has **no stand-ins** any more.
`claims: []`; see "What can be claimed" below.

### Scorer output (exact)

```
cloud/work/frontier/w7a/tools/grp.sh cloud/work/frontier/w7a/groups/particle_knot
Members:
render_display_list:
  MATCH
func_80099B30:
  MATCH
func_8009C3F8:
  MATCH
    own .rodata verified at 0x80123ABC..0x80123AEC
camera_update_c:
  MATCH
select_screen_update:
  MATCH
particle_system:
  MATCH
    own .rodata verified at 0x80123AFC..0x80123B00
track_collision_wall:
  MATCH
func_8009F058:
  22/1307 words differ          (all `sw/lw t0,68(sp)` vs retail `72(sp)`; full list in score.txt)
    own .rodata verified at 0x80123B60..0x80123B68
Input_ProcessGameplayPad:
  MATCH

python3 -m tools.conveyor.pipeline.blob_unit --tag w7a score render_display_list func_80099B30 particle_system \
  track_collision_wall func_8009F058 func_8009C3F8 camera_update_c select_screen_update Input_ProcessGameplayPad \
  --with cloud/work/frontier/w7a/groups/particle_knot/group.c --internal render_display_list --internal func_80099B30 \
  --internal particle_system --internal track_collision_wall --internal func_8009F058 --internal func_8009C3F8 --neighbours
  EQUAL render_display_list: 2559 words (internal, c_group.c)
  EQUAL func_80099B30: 51 words (internal, c_group.c)
  EQUAL particle_system: 725 words (internal, c_group.c)
  EQUAL track_collision_wall: 678 words (internal, c_group.c)
  FAIL func_8009F058: 22 of 1307 words differ
       defined by c_group.c (internal)
  EQUAL func_8009C3F8: 113 words (internal, c_group.c)
  EQUAL camera_update_c: 9 words (kept, c_group.c)
  EQUAL select_screen_update: 9 words (kept, c_group.c)
  EQUAL Input_ProcessGameplayPad: 680 words (kept, c_group.c)
  locked bodies that differ in this unit: 0
blob_unit score: 8/9 equal; object build/blob_unit/w7a/unit.o (4.5s)
```

## Blocker 2 closed: Input_ProcessGameplayPad (0x800A04C4)

**Dependence on func_80087110:** none. `frontier show` lists it as a `group`-class function (blockers
func_80087110, func_8009F058); its only non-ABI facts come from func_8009F058 (which saves only `ra`, so this
caller saves s0–s8 and $f20–$f24 it never uses). func_80087110 is called twice through the plain ABI (a0–a3 plus
two stack words); its body is absent from the unit (unlocked, external) and Input_ProcessGameplayPad is EQUAL
there, so its code does not depend on it. (`frontier`'s `preserved: a1, a2, t7` is an artefact of the
likely-branch delay slots, not a real dependency.)

**Semantics** (names are labels): per frame — reset the TLUT caches (`D_8017A4B0`, `D_8017A638 = -1`,
`D_8017A508`), clear lighting/texgen, `D_80124F84 = 0`, load lights 0/1/2 from `*D_801406B8` by
`D_80140618`, pick the vertex buffer `D_80161434 = D_80161498[D_8015F72D]` (3200 Vtx each), draw every view
(`func_8009F058(i, &D_80150B70[i])` for `i < D_80146204`), then 2D state (no fog, pipe sync, full-screen
scissor, pixel depth, 1-cycle, point filter); unless `arg0`, walk the 2D sprite list `D_80140BF0[D_801613AC]`:

```c
typedef struct {            /* D_80140BF0[], 32 bytes, count D_801613AC */
    Palette *pal;           /* 0x00 palette (textured) or u8 *colour (fill rect); arg of the callback */
    Texture *tex;           /* 0x04 Texture (w6a layout), or a callback when flags & 0x80 */
    u16 model;              /* 0x08 >> 10 = D_80138670 palette table */
    s16 x, y;               /* 0x0A */
    u16 mode;               /* 0x0E func_8008A644 argument, else func_8008705C(16) */
    s16 w, h;               /* 0x10 */
    u8 color;               /* 0x14 255 = func_8008705C(32), else func_8008A38C(color) */
    u8 flags;               /* 0x15 0x80 callback, 4, 8 (clear = flipped strips), 1 */
    s8 hidden;              /* 0x16 */
    u8 pad17;
    s16 t0, s0, t1, s1;     /* 0x18 texel window */
} Sprite;
```
Untextured: `func_8008A3E4` (clip) + `func_8008A46C` fill. Textured: clip with the height doubled when
`D_8002AFC4 > 240`, strip height from `D_8011ED0C[width >> (4 - siz)]` (halved for fmt 2/5; whole-TMEM
`4096/2048/1024 / width` and `tile = 1` when < 2 lines or width not a multiple of `16 >> siz`), palette from the
sprite, `D_80138670[model >> 10].pal[tex->palette]`, the 1-entry TLUT `D_8011ED08` (fmt 3, alpha = colour) or the
table's first palette; then strips `object_render(...)` + `func_80087110(...)`, rows bottom-up when flipped.
Globals: `D_8002AFC0/C4` s32 screen w/h, `D_801406B8` Lights2*, `D_80140618` s16, `D_8015F72D` volatile s8,
`D_80146204` u8, `D_801613AC` s32, `D_8011ED08` u8[4], `D_8011ED0C` u16[].

**Shaping facts** (each changed code; ~45 compiles, `runs/i1`…`q3`):
1. `func_8009F058(s32 view, View *eye)` — two parameters. With w6a's `(view, arg1, arg2, eye)` the caller has to
   store the dead a1/a2 into their outgoing stack words (`sw zero,4(sp)`); retail stores nothing. eye still
   arrives in a3 (IPA choice); func_8009F058's own code is unchanged by this.
2. `if (D_80140618 >= 0) switch (...) { case 0/1/2 }` — gives retail's `bltz` before the compare chain (a plain
   switch, an if-chain, `default:`, `case -1:` or reversed cases do not).
3. `D_8015F72D` volatile (`lui; addiu; lb 0(reg)`).
4. Frame (256): only `i` at function level; the sprite-loop body declares its own 14 locals (block scope), `e`
   first, `x` 8th, `tile` 10th — the three spilled ones land at 180/152/144 exactly as in retail; three of the 14
   are unused (each named local costs a slot, the 17 GBI-macro `_g` locals sit between `i` and `e`).
5. Variable sharing (one variable = one register, and only a call-crossing variable gets an s-register):
   `lines` also holds the doubled height and the fill-rect y1 (retail `s3`); `flip` also holds the texture
   width and the fill-rect x1 (retail `s0`).
6. The callback test re-reads `e->tex` (retail loads it into v1 and copies to `tex`'s s5).
7. `flip = 1;` before `func_800878E0(8)` (else as1 cannot hoist `li a0,8`).
8. `if (e->t0 < 0) t = 0; else t = e->t0;`.
9. Strip loops with no `next` variable: `if (e->t1 < t + lines) lines = ...; ... t += lines;` — a `next`
   variable made uopt propagate `t + lines` into `next - 1` and add a copy.
10. y advance: `if (D_8002AFC4 > 240) y += lines; t += lines; y += lines;` (gives retail's s0 = y + lines, the
    conditional add on s0, then `move s1,s2; move s4,s0`).
11. Flipped strip's ult written `tex->height - (t + lines)`: with `tex->height - t - lines` uopt shares
    `height - t` with lrt's `tex->height - t - 1`, that temp takes v0 and shifts the whole colouring.
12. Palette pointer: `pal = e->pal` *inside* the first branch, `pal = &D_80138670[..].pal[tex->palette]` and
    `pal = D_80138670[..].pal` in the others (retail colours these v1/v0/v0 as a variable, not as temps).

## func_8009C3F8's other callers

`camera_follow_path` (1,484 B; blocked by the unmatched `camera_trigger_check`) and `func_800BFD8C` are absent
from the group and from the unit run; func_8009C3F8 is EQUAL as an internal function with only its real chain
caller (particle_system) and the two kept wrappers, so they are **not needed for its code**. For the
integrator: when either of them is matched it must join this group (it calls the kernel with x in `$f16`).

## Blocker 1 still open: func_8009F058's spill slot (new measurements)

Built an instrumented uopt (`tools/patch_home2.py`, `patch_confl.py`, `patch_gt2.py` on top of w6a's
`patch_area2.py`/`patch_sp.py`; `tools/sp.sh`). Findings:
- The `t0` slot is **not** an `f_gettemp` save slot; it is the `f_spilltemps` home of the expression
  `&D_80150B70[view]` (bit 201, `ixa(ldaS(19), view*152)`). Homes are given out greedily in bit order: a
  temp takes the lowest home not used by a lower-numbered homed temp that shares a block set (+0x15c).
  Ours: bit 5 `view*16` (s3) → home 0, bit 11 `&D_8017A510[view]` → home 1, bit 170 `view*64` → home 2,
  **bit 201 → home 1** (it meets bit 5, not bit 11), then D_801613B8 (FP) → 3, the scale loop's row/element
  addresses → 4, 5. Six homes; the busiest blocks are the `D_801613B8` scale loop
  `{5, 170, 201, 231, 240, 242}` (`runs/g1/sp.log`, CONFL lines).
- Named locals are fixed by the referenced offsets: with frame 400, perspNorm at 322 needs 78 bytes of named
  locals above it, i.e. one more 4-byte local declared textually before it (w6a's `pad1`; removing it gives
  `runs/f/v0`: frame 392, t0 64, 95 words off, and no aligned frame puts perspNorm back at 322). So retail's
  named area is 324 too, and t0 at 72 means **bit 201 holds home 0 in retail**: either `view*16` is not a homed
  temp in retail, or `&D_80150B70[view]` is numbered before it, or they do not share a block set. The code
  itself shows s3 = view*16 set once at entry and used at the vtx stores, so the difference must be in how
  uopt numbers/scopes those expressions, not in the instructions.
- Tried without effect (no pad / with pad): five scale-loop spellings (`1.0f` compare, braces, `x = x * s`,
  `s * x`, one-line nest). w6a's list of non-effects still applies.
**Best next hypothesis:** make `&D_80150B70[view]` the first-numbered homed temp or keep `view*16` out of the
homed set without changing code: look for a source form where the vp/vtx `view*16` is reached through a
different expression tree (e.g. a `Vp *`/`Vtx *` computed differently) or where the view-matrix pointer appears
earlier; check each candidate with `tools/sp.sh` (HOME lines: bit 201 must print `home=0`) before scoring.

## What can be claimed

Strictly under the brief: nothing yet — func_8009F058 is a real member but not a match, so `claims: []`.
However, unlike w6a's state, every other member is now pinned by real code only: the group has no stand-ins,
func_8009F058's residual is a stack offset (its registers, call sites, parameter and unsaved-register facts are
byte-identical to retail), and all eight others are EQUAL in the whole-program unit with 0 locked bodies
broken. If the owner accepts an unclaimed real context member whose only residual is a spill-slot offset,
these are claimable now (listed in `group.json` as `claimable_if_unmatched_context_member_accepted`):
render_display_list, func_80099B30, func_8009C3F8, camera_update_c, select_screen_update, particle_system,
track_collision_wall, Input_ProcessGameplayPad — **19,296 bytes**. func_8009F058 (5,228) stays out until its
slot closes. Landing note: the group must be spliced with func_8009F058 compiled in as an unclaimed member
(not context-stripped), and camera_follow_path/func_800BFD8C must later join this group.

## What generalises

1. **A dead parameter in an IPA-internal callee still costs the caller a stack store.** If the callee never
   reads a1/a2 and retail's caller does not set them, the callee simply has fewer parameters; IPA kept the
   remaining pointer in a3 anyway.
2. **`if (x >= 0) switch (x)`** reproduces a `bltz` in front of IDO's case-compare chain.
3. **Block-scoped declarations decide the frame**: a loop body that declares its own locals puts them below
   every GBI-macro `_g` of the code before it; three spilled locals' offsets pinned the order and count.
4. **A variable assigned twice with the same expression (`next = t + lines` before and inside an `if`) gets
   copy-propagated into later uses** (`next - 1` → `(t+lines) - 1` plus a copy). Writing the loop without the
   variable (`t += lines`) or making the defs different removes the copy.
5. **A shared sub-expression between two call arguments steals a register for the whole block** (here
   `height - t` took v0 and shifted four other colours); `h - (t + lines)` is reassociated by uopt but not
   CSE'd against `h - t - 1`.
6. **Spill-home slots are greedy colours over block sets in expression-number order**; `tools/sp.sh` prints
   which expression owns each home and which lower-numbered temps blocked it, so a "frame filler" or "one slot
   off" residual can be traced to a specific expression instead of guessed.
