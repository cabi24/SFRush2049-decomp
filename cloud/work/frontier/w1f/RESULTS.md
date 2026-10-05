# Frontier wave 1, agent w1f (2026-10-04)

Scorer: `tools/cloud/score.py` in the builder copy `watchman2:~/rush2049/scratch/frontier/w1f`
(`IDO_DIR=$HOME/rush2049/cache/toolkits/796ae99a.../ido`). `sc.sh SRC NAME [args]` in this directory copies the
source to `cand/NAME.c` and runs `python3 tools/cloud/score.py fn cand/NAME.c NAME [args]`; the commands below are
that call. Nothing was committed, spliced or locked. All seven were re-scored together at the end of the pass.

| # | function | bytes | state | flags |
|---|---|---:|---|---|
| 1 | camera_follow_target | 812 | **strict MATCH** | **-O3** (-O2: 38 words) |
| 2 | camera_update_d | 784 | code identical, own-rodata unverified (8 relocations, 4 literal words) | **-O3**, needs `func_8008B3F4` in the unit (inlined twice) |
| 3 | race_setup_2 | 908 | code identical, own jump table unverified (2 relocations) | -O3 (same words at -O2) |
| 4 | func_800A1644 | 716 | **strict MATCH** | -O3 (also -O2) |
| 5 | func_800A7E10 | 704 | **strict MATCH** | -O3 (also -O2) |
| 6 | func_8009D99C | 692 | **strict MATCH** | **-O3** (-O2: 170 words, s0 frame) |
| 7 | func_800A557C | 456 | **strict MATCH** | -O3 (also -O2) |

Strict: 5 functions, 3,380 bytes. Code-identical but not strict: 2 functions, 1,692 bytes.

## Strict matches (`cloud/matches/`)

Each: `score.py fn cand/NAME.c NAME --flags '-g0 -O3 -mips2 -G 0 -non_shared'` -> `NAME:  MATCH`.

- **camera_follow_target** = arcade `dotireforce` (`reference/repos/rushtherock/game/tires.c`); callee `camera_dolly`
  is `frictioncircle`, `func_8009E820` is `bodtorw`. Earlier state: 177/203 (`cloud/work/ipa-groups/codex_tireforce_a124`).
  What closed it, each checked by removing it: the arcade's four unused locals `wheelalpha, spd, temp, maxnorm`
  (16 frame bytes; without them 20 words); arcade literal types — `= 0` (int) for the three zero stores against
  `0.0f` in comparisons (166 words with `0.0f` stores) and int `- 1` in the N64 traction term (61 words with `1.0f`).
  Six own literals as `extern f32 D_80123E10..D_80123E24`.
  Layout recovered: model `+4` ptr (`+0x18`, `+0x1C` floats), `+9/+0xB/+0xC` s8 indices, `+0x3F4` s16,
  `+0x430` `Tire[4]` (0x5C each, `sideforce +0x50`, `traction +0x54`), `+0x5B0`, `+0x5B4`, `+0x5BC` weight,
  `+0x5C0` mass, `+0x638` idt, `+0x720`; tables `f32 D_80111130[][3]`, `f32 D_80110F80[][6]`, `s8 D_80142DB0`.
- **func_800A1644** (code-string encoder through `u16 D_8011EAEC[]`; callers `track_process_main`,
  `track_render_process`). Earlier state: 165/179 (`cloud/work/near_miss_B105`). What closed it: both pointer
  parameters are copied to locals at the top and only the copies are used (`dst = out; src = in;`). With the
  parameters used directly uopt moves `out` to `t0` (or spills it at -O2) and gives `a0` to a strength-reduction
  temp; every later register follows. Also: no byte temporary (`src[i]`/`buf[i]` re-read in source) and `& 0xFF`
  on the low-byte store (one temp-ring step). `u8 buf[64]` from the 104-byte frame.
- **func_800A7E10** (box corner vertices into `Vtx D_80157248[]` at `s32 D_80156CE0`, high-water `s32 D_80156D30`,
  `D_8012E700[idx].flags |= (first << 3) | 7`). Earlier state: 66 words. What closed it: `vtx++` after each of the
  first seven vertices instead of `vtx[n]` (IDO folds to one `addiu v0,v0,112` and negative offsets).
- **func_8009D99C** (float 3x4 rows + position -> fixed-point `Mtx`). Sibling of locked `func_8009D45C`; its source
  with `f32 (*m)[4]` and a separate `f32 *pos` matched first compile at -O3. Earlier state: 64/173
  (`cloud/work/tiny_A125`). `func_8009D45C` is locked at -O2; this one needs -O3.
- **func_800A557C** (single-precision tangent, Cody & Waite form; calls `modff` at 0x80002A64). Earlier state:
  70/114 (`cloud/work/near_miss_B29`). What closed it: the second `modff` writes its integer part into `g`, the same
  variable later assigned `f * f`. Inferred mechanism (from the register outcome, not from a uopt trace): a variable
  whose live range touches the block holding a call result cannot take `$f0`, so `g` takes `$f2` and the other four float variables take the retail registers; with a separate
  integer-part variable `g` is coloured `$f0` and five registers rotate. Also `f = xnum;` before the test and
  declaration order `xnum, g, xn, y, f, xden`. Twelve own literals as `extern f32 D_80123B98..D_80123BC4`.

## Code identical, not strict (`<name>/best.c`)

- **camera_update_d** — `score.py fn cand/camera_update_d.c camera_update_d --flags '-g0 -O3 -mips2 -G 0 -non_shared'` ->
  `MATCH (8 section-relative relocations unverified: .rodata+0x4 at +0x44, .rodata+0x4 at +0x54, .rodata+0x8 at +0x58, .rodata+0x8 at +0x70, .rodata+0xc at +0x88, .rodata+0xc at +0xf8, .rodata+0x10 at +0x100, .rodata+0x10 at +0x118)`.
  It is a cut-down libultra `guLookAtReflectF` (LookAt only, Up not re-normalised). The two normalisations are
  inlined copies of the locked `func_8008B3F4` (`if (v < 0.0001f) v = 0.0001f; return 1.0f / sqrtf(v);`), which has no
  `jal` callers in the image. The four `0.0001f` words at 0x80123AEC..0x80123AF8 (all 0x38D1B717) are therefore this
  function's own rodata, and they cannot be spelled as externs: the inlined copies would reference the helper's
  `D_80123884`. Writing the clamp out by hand with externs does not reproduce the code (20 words shorter, 6
  callee-saved FP registers), so it closes only with workstream B or a group splice.
  Real two-body group prepared: `groups/camera_update_d/` (`score.py group` -> `func_8008B3F4: MATCH (2 ... unverified:
  .rodata+0x0 at +0x0, .rodata+0x0 at +0x4)`, `camera_update_d: MATCH (8 ... unverified ...)`; claims left empty).
  Other shaping facts: Up assigned back to the `xUp/yUp/zUp` parameters as in the SDK (expressions used directly:
  106 aligned rows); SDK `FTOFRAC8` with `& 0xFF` (12 words without); declaration order `len, xRight, yRight, zRight,
  xLook, yLook, zLook` (SDK order: 6 words). About 100 variants before the inlining was recognised.
- **race_setup_2** — `score.py fn cand/race_setup_2.c race_setup_2 --flags '-g0 -O3 -mips2 -G 0 -non_shared'` ->
  `MATCH (2 section-relative relocations unverified: .rodata+0x0 at +0x2c4, .rodata+0x0 at +0x2cc)`.
  The two relocations are the function's own six-entry jump table. Hand check: the object's `.rodata` is six
  `R_MIPS_32 .text` words `+0x2d8 x3, +0x2e8 x3` = 0x800BED78 x3, 0x800BED88 x3, equal to retail 0x80123E5C..0x80123E70.
  Earlier state: 152/227 (`cloud/work/game_C51`). What closed the code: the quadtree box test encloses the split
  test (`if (inside) { if (flags & 1) {...} else break; } else return;`), which lets the `pos[]` loads be shared;
  `D_80149770[tree->child2].maxz` directly instead of a named sub-node pointer (1 word); `f32 pos[3]` filled by
  three scalar assignments. **Not original source:** `s32 unusedA[6]` and `s32 unusedB[9]` only reproduce the
  136-byte frame (24 untouched bytes above `pos`, 36 below); their real declarations are unknown.
  Layout recovered: 28-byte `Tree` (`u8 flags +3`, `f32 minx, maxx, minz, maxz +4..`, `u16 count, first, child2, child3
  +0x14..`) at `Tree *D_80149770`; `Object **D_801497C0`; object `u8 flags +4`, `s16 type +0x10`, `+0x44`, `+0x54`,
  `s8 player +0x5C`; 48-byte `ObjType D_80117530[]` (`onHit +8`, `s16 shape +0x10`); `HitFunc D_80117518[6]`;
  pool `D_80151AA8` (`active +0x10`); car `pos +8` in `D_80152818[]` (0x3B8).

## What generalises

1. **Check for an inlined callee before fighting float allocation.** In `camera_update_d` the signature was not the
   `ra, t5, t4` load pattern but: values kept in memory across a clamp-and-`sqrt`, `1/sqrt` landing in `$f0` before a
   separate `neg`, and only three callee-saved FP registers where plain C used six. `frontier stubs`-style lookup
   that works: `callers.py` on small matched neighbours — a matched function with no `jal` callers is a candidate.
2. **Variable reuse is visible to uopt.** One C variable used for two unrelated values is one live range
   (`func_800A557C`: `g`). When a float variable avoids `$f0` for no visible reason, look for an earlier value that
   could have lived in the same variable in a block that holds a call result.
3. **Local copies of pointer parameters** (`dst = out; src = in;`) keep the parameters in `a0`/`a1` and push
   strength-reduction temps to `a3`/`t0`. Symptom: candidate starts with `move t0,a0` or spills a parameter.
4. **SDK macros verbatim.** `FTOFRAC8`'s `& 0xff` and a redundant `& 0xFF` on a byte store each cost one temp-ring
   step and no code; a ring that skips one register per statement is this.
5. **`p++` between element groups, not `p[n]`**, when retail has one up-front `addiu` and negative offsets.
6. **Arcade unused locals and literal types are load-bearing** (`dotireforce`): copy the arcade declaration list and
   the int/float spelling of each literal before anything else.
7. Tool notes: `full.py` here passes `-z` to objdump (the agentC copy crashes on runs of `nop`); `bscore.py` here has
   `--lead` (index of the first differing word), which is a better progress meter than the aligned count when an
   unrolled loop amplifies one register swap, and uses 2 workers.
