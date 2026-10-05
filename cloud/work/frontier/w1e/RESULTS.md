# Frontier wave 1, agent w1e (2026-10-04)

Scorer: `tools/cloud/score.py` in the builder copy `watchman2:~/rush2049/scratch/frontier/w1e`
(`IDO_DIR=$HOME/rush2049/cache/toolkits/796ae99a…/ido`). Every command below was run as
`python3 tools/cloud/score.py fn cand/NAME.c NAME --flags '-g0 -O3 -mips2 -G 0 -non_shared'`; the output is quoted
exactly. Nothing was committed, spliced or locked; nothing outside `cloud/matches/` (four new files) and this
directory was written.

| # | function | bytes | state | flags | scorer output |
|---|---|---:|---|---|---|
| 1 | physics_float_calc | 1,176 | **not matched**, 210/294 | -O3 | `210/294 words differ (12 extra words (nonzero beyond target length); 2 section-relative relocations unverified: .rodata+0x0 at +0x1e0, .rodata+0x0 at +0x1e8)` |
| 2 | func_800F769C | 1,020 | **strict MATCH** | -O3 (also -O2) | `func_800F769C:  MATCH` |
| 3 | func_800FC9F8 | 1,016 | **strict MATCH** | -O3 (also -O2) | `func_800FC9F8:  MATCH` |
| 4 | func_800D0424 | 996 | **strict MATCH** | -O3 (also -O2) | `func_800D0424:  MATCH` |
| 5 | func_800E114C | 948 | **code identical, own-rodata unverified** (not strict) | -O3 (same at -O2) | `MATCH (6 section-relative relocations unverified: .rodata+0x0 at +0x10, .rodata+0x0 at +0x74, .rodata+0x4 at +0x8c, .rodata+0x4 at +0x90, .rodata+0x8 at +0x170, .rodata+0x8 at +0x1b0)` |
| 6 | func_800AB18C | 924 | **strict MATCH** | **-O3 only** (-O2: `99/231 words differ`) | `func_800AB18C:  MATCH` |

Strict: 4 functions, 3,956 bytes. Code-identical-unverified: 1 function, 948 bytes. All six are ordinary kept
functions (no register parameters, no unsaved callee-saved registers); none needed a group.

## 2. func_800F769C — strict MATCH — `cloud/matches/func_800F769C.c`

Adds one object's 1,780-byte statistics block into five global total arrays. Layout came from
`cloud/work/game_C38` (65 words off). Two levers closed it:
- the one-record loop is written `i != 1` (gives the `bne ptr,end` test; `i < 1` gives `sltu`/`bnez`);
- the last loop has its own counter `k` (reusing `i` swaps the destination and end pointer registers `v1`/`a0`).

Types: `StatsA` 96 bytes ×12 at `D_80150F88` (f32 total at 0x40, u16 count[9] at 0x44, u32 sum at 0x58, u16 flags at
0x5C); `StatsB` 64 ×4 at `D_80151410`; `StatsC` 12 ×8 at `D_80151578`; `StatsD` 24 ×1 at `D_801515F8`;
`StatsE` 28 ×4 at `D_80151618`. Source block = 140-byte prefix + the same five arrays, reached through
`object->model(+0)->holder(+0x2C)->statistics(+0)`.

## 3. func_800FC9F8 — strict MATCH — `cloud/matches/func_800FC9F8.c`

First compile. Scrolls four point lists by a per-frame, per-slot velocity and wraps them. Prior attempt
`cloud/work/tiny_A69` was 235/254 with a raw `s16` list and a byte-offset macro; typed structs matched directly.
- `PointList`: s16 count at 0, 20-byte points from 0x14 (s16 x, y first). `D_80114628[4]` list pointers.
- `D_80114264[frame - 1]`: 224-byte frame = 4 × 56-byte slot {0x10 f32 dx, 0x14 f32 dy, 0x18 f32 xs[4], 0x28 f32 ys[4]}.
- `D_80114638`: pointer, u16 width at 0x10, height at 0x12. `D_8011463C[frame - 1]` s32 dirty flag. `D_80151AD0` s16 frame.
- Quirk: the two "no shift" stores are int `0`, the initialisers are `0.0f` (two zero registers in retail).

## 4. func_800D0424 — strict MATCH — `cloud/matches/func_800D0424.c`

Arcade `positions(MODELDAT *m)` (`reference/repos/rushtherock/game/drivsym.c`), proven by structure. Callees:
`func_8009E820` = `bodtorw`, `func_800A61B0` = `rwtobod`, `sound_position_set` = `rotuv`, `func_8008B424` =
reciprocal length. Prior attempt `cloud/work/near_miss_B15`: 171/249.
- N64 `MODELDAT` offsets seen here: 0x000 car-type pointer (TIRER[4][3] at +0x70 of that record), 0x028 A, 0x040 V,
  0x04C W, 0x0F4 BODYR[4][3], 0x214 RWA, 0x220 RWV, 0x22C RWR, 0x238 previous RWR, 0x244 TIRERWR[4][3],
  0x274 BODYRWR[4][3], 0x2E0 angle accumulator[3], 0x2EC UV (3×3), 0x5DC tpcomp[4], 0x634 dt, 0x710 frame counter.
  `D_8014A110` s32 mode (== 4 enables the angle accumulator), `D_801427A1` s8.
- Quirks: the three-float copy is one comma expression; sums written `temp + RWR`, `rwp + RWR`, `bp[1] + tpcomp`;
  the unused arcade locals stay declared (unused arrays do take frame space) minus two scalars, for the 176-byte frame.

## 5. func_800E114C — code identical, own-rodata unverified — `func_800E114C/best.c`

Arcade `velocities(MODELDAT *m)` with N64 additions. Every text word equals retail; the six unverified words are the
HI16/LO16 pairs of the function's own three float literals. The literals are `0.1f`, `0.025f`, `.025f` in that order =
retail 0x801243B4/B8/BC (3DCCCCCD, 3CCCCCCD, 3CCCCCCD), the addresses the retail words encode. Closes when the scorer
and splicer own rodata (plan workstream B). Cannot be made strict with externs: `extern f32 D_801243B8` is reloaded
after the store to `V[0]` (195 words differ); the other two can be externs without changing code
(`unverified` 6 -> 2), which does not help.
- Prior attempts `cloud/work/ipa-groups/codex_velocities_*`: 202/237.
- Levers, each needed: (a) the two 0.025 constants are **spelled differently** (see "Generalises"); (b) the contact
  test uses int `0`, all other zeros are `0.0f`; (c) arcade operand order `V + temp`, `W + temp`; (d) the drag factor
  is built in `speed` in two statements with an if/else coefficient, and is not the clamp's `velfact` variable.
- More `MODELDAT` offsets: 0x034 AA, 0x2FC "up" component (UV[1][1]), 0x3D0 brake, 0x3D4 throttle, 0x3F0 magvel,
  0x5EC contact[4], 0x640 s8 sliding, 0x650 damping, 0x7C6 s16 car index. `D_80152818[car].mode` s8 at +0x359.
  `D_80114184`/`D_80114188` are `.data` f32 drag coefficients (1e-05, 0.0005).

## 6. func_800AB18C — strict MATCH — `cloud/matches/func_800AB18C.c`

Per-view fog/clip setup with fog regions. The prior source (`codex_init_region_countdown_a160`, reported 187/231) is
38 words off when compiled alone: it had been scored as an internal member of a group. Levers:
- typed struct fields instead of `*(T *)((u8 *)p + off)` (36 words of FP temp order);
- the view pointer through an integer cast, `(View *) ((s32) D_8017A510 + index * 72)`: retail reloads `D_8013F1D8`
  and `D_8014A108` after each store to the view; `&D_8017A510[index]` does not (228 words differ), `(u32)` on the
  finished address is 65 off. The original spelling is unknown; this is a shaping quirk.
- `nearest = region;` before `closest = dist;`.
Types: view 72 bytes at `D_8017A510` (f32 fog at 0x3C, u16 near at 0x40, u16 far at 0x42); region 12 bytes
{s16 x, z, radius, near, far, fog}, list per track at `D_8011E76C[D_8014978C]`, terminated by radius 0;
`D_8011E810`/`D_8011E7F0` s16 [track][4], `D_8011E7A8` f32 [track][4], `D_80151AA0` f32.

## 1. physics_float_calc — not matched — `physics_float_calc/best.c`, `physics_float_calc/NOTES.md`

Residual lanes: structure (loop-2 statement placement, `mask` held in memory across the switch), then temp-ring width
and stack layout. About 800 scripted variants; the last 300 did not move the loop-2 residual, so it was stopped.
Established on the way (details and the kept experiment sources are in NOTES.md):
- the function saves `ra`/`s0` without containing a call because a call sits under a condition that only uopt folds;
- the zone search is an inlined static with pointer out-parameters (first loop then equals retail), and the caller-less
  stub `func_8009EBB8` in front of it is that static's deleted body;
- the six tables are 16-byte (128-bit) visibility mask rows per zone, one table per track.
Best next hypothesis: the whole body is the static, called once with a constant-0 debug flag from a thin wrapper;
the dead call is between the switch and loop 2 (NOTES.md).

## Generalises

1. **uopt identifies float constants by spelling.** `0.025f` twice is one web (loaded once, reloaded after a call);
   `0.025f` and `.025f` are two webs with two rodata words. Retail `func_800E114C` has two equal words (0x801243B8,
   0x801243BC) for exactly this reason. Equal adjacent literal words in a function's rodata window are a cue to
   spell them differently. (Tested on this function only.)
2. **A call that only uopt can prove dead leaves `sw ra`, callee-saved registers and an un-unrolled loop behind.**
   `s32 dbg = 0; if (dbg) foo(x);` — cfe removes a literal `if (0)` and leaves nothing. This is a second source of
   "internal-looking" allocation in a kept function. Signature: `ra` saved, no `jal`.
3. **A comma expression of assignments emits its loads and stores in reverse order** with the temps numbered in
   source order (`a[0] = b[0], a[1] = b[1], a[2] = b[2];` loads `b[2]` first into the highest temp). This is the
   "descending loads then stores" pattern without any inlined callee (`func_800D0424`).
4. **Operand order of `a + b` is not free when one side is a memory field and the other a stack temp**: the arcade's
   `vecadd(a, b, r)` order (`a[i] + b[i]`) matched in both drivsym functions. But with byte-offset cast macros cfe
   reorders by operand complexity, and no source order reaches retail: use typed fields first.
5. **A loop counter reused across consecutive loops changes pointer/end register assignment** in later loops; a
   separate counter for one loop fixed `func_800F769C`. `i != N` vs `i < N` selects `bne` vs `sltu` on the
   strength-reduced pointer test.
6. **Unused local arrays do take frame space** (removing the unused arcade `yvect/ftroll/rtroll/temp2` from
   `func_800D0424` changes the frame); a declared-but-unused float scalar did not change `func_800E114C`'s frame.
7. A stored group result can be worse than the same source alone: `func_800AB18C` was 187/231 as an internal group
   member and 38/231 alone. Re-score "group" near-misses standalone at `-O3` before reading their residual.
8. Three of the five solved functions also match at `-O2`; `func_800AB18C` is `-O3` only.

## Helper scripts in this directory

`sc.sh` (score one file), `aq.sh` (score at -O3 and -O2), `ad.sh` + `adiff.py` (aligned instruction diff with
relocations resolved, `--full` for the whole listing), `batch.sh` + `bscore.py` (score a directory of variants;
`--tail N` restricts the aligned count to the last N retail words), `dump.sh`.
