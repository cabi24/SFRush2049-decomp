# w10e results (wave 10, mid-size groups)

Assignment: func_800DA2C0 (2,316 B), sound_bank_unload (1,248 B), func_800B59F0 (1,372 B), func_800D6914 (1,260 B),
race_countdown_display (1,120 B). Flags everywhere `-g0 -O3 -mips2 -G 0 -non_shared`. All scoring in the
whole-program unit (`blob_unit --tag w10e score … --neighbours`); no locked body differed in any run.

**No strict MATCH this lane, and nothing to splice.** `cloud/matches/` has no new files and no group claims anything.

| Function | Bytes | State | Exact unit output | Residual lane |
|---|---:|---|---|---|
| race_countdown_display | 1,120 | near-miss, 7 aligned rows (was 60/280) | `FAIL race_countdown_display: 34 of 280 words differ; compiled body is 279 words, target 280` | one as1 delay-slot choice |
| func_800B59F0 | 1,372 | **provisional, EQUAL with stand-in callers, but only with one unproven quirk** (`equal_quirk.c`); 2/343 without the quirk (`best.c`) | `EQUAL func_800B59F0: 343 words (internal, c_equal_quirk.c)` / `FAIL func_800B59F0: 2 of 343 words differ` | v0/v1 colour of one web |
| func_800D6914 | 1,260 | provisional lane, near-miss | `FAIL func_800D6914: 282 of 315 words differ; compiled body is 321 words, target 315; …` | s7/s8 address-vs-constant colour, FP constant colour |
| func_800DA2C0 | 2,316 | provisional lane, first draft | `FAIL func_800DA2C0: 494 of 579 words differ; compiled body is 574 words, target 579; …` | colouring (s8, FP constants) |
| sound_bank_unload | 1,248 | near-miss, unchanged from w3b | `FAIL sound_bank_unload: 134 of 312 words differ` (`EQUAL func_800B0EA0`, `EQUAL func_800B0F60`) | temp-ring phase, colour, as1 store order |

Lane tools (all tag `w10e`, Pi-side): `tools/q.sh NAME "unit args" FILE…` (unit verdict and aligned rows per file),
`tools/u.sh`, `tools/udiff.py` (aligned diff of the unit object), `tools/ulist.sh` (ugen `-l` listing of the stage),
`tools/ctrace.sh` / `pdiff.sh` / `force.sh` (traced uopt colouring, built in
`watchman2:~/rush2049/scratch/frontier/w10e/uopt` from the workbench `globalcolor` profile), `tools/udiag.sh`.

---

## race_countdown_display: 34/280 words (7 aligned rows)

`race_countdown_display/best.c`. Semantics (from the dot packet and code): checkpoint crossing for one model (arcade
`game/checkpoint.c:PassedCP`); next checkpoint via the locked kept `func_800B930C`, lap count, finish detection,
lap/finish sounds through the inlined `func_800B61A8` wrapper, lap-remaining announcements through `car_stats_display`.

```
python3 -m tools.conveyor.pipeline.blob_unit --tag w10e score race_countdown_display \
    --with cloud/work/frontier/w10e/race_countdown_display/best.c --neighbours
  FAIL race_countdown_display: 34 of 280 words differ; compiled body is 279 words, target 280
       defined by c_best.c (kept)
       umerge inlined into it: func_800B61A8, func_800B930C
  locked bodies that differ in this unit: 0
```

Started from `cloud/work/frontier/dot_checkpoint_crossing_20261006/group/` (60/280 in the unit). What moved it:
- **Comparison operand order sets temp order.** IDO evaluates the left operand of `==` first here, so
  `m->last_cp == D_80151CE8.before_finish` (−2 words), `m->laps + 1 == D_80152734` (−20) and
  `m->laps == D_80152734` at both finish tests (−4) reproduce retail's t3/t4 and t7/t8/t9 order.
- Calling the **locked kept `func_800B930C`** (get_next_checkpoint) instead of a private static copy compiles
  identically and needs no duplicate definition.
- The direct-return `__inline func_800B61A8` must be defined in the same file. With only the locked
  `src/blob/func_800B61A8.c` (it has a `new_var` local) umerge still inlines it, but the result is 72/280.

Residual (lane: as1). At +0x3e0 retail has `lb v0,0(a1); li at,1; beql a2,v0,epilogue+4; lw ra` and ours has
`lb v0,0(a1); beq a2,v0,epilogue; li at,1`. Retail hoists the `li at,1` of the next block's `bne v0,1` macro above the
branch and fills the slot from the target. Ours puts it in the delay slot. That is one word, and the
epilogue branches shift by 4 after it. The ugen listings agree up to that point. Tried about 25 variants with no
movement: line layout of the else-if (single line, split, braces, `;`), `!=`/`==` operand order, swapped arms, `switch`,
early `return`, `else { if }`, `s8`/`s16` old_remaining (much worse), using `old_remaining` in the clamp (worse).
Workbench diagnose run: it only reports the 1-word length delta and relocation-symbol noise.
Next hypothesis: an as1 trace (w6d has a traced as1, `asr_remote.sh`) of why the load-use hoist of `li at` does
not fire for us. Something in the `.alias $5,$sp` / `.loc` sequence after the `beq` may block it.

**Integration (if it ever closes):** the file defines `func_800B61A8` as `__inline` direct-return; the locked
`src/blob/func_800B61A8.c` must become that form, or the unit needs a `prefer_definition` entry for it.

## func_800B59F0: provisional, EQUAL with stand-ins via one quirk; 2/343 naturally

Real caller `physics_sym` (L8, unmatched), so provisional at best. The function writes s0–s8 without saving them.
Pause-menu buttons (semantics in w6d RESULTS): four 64-byte records at D_8011A994 plus shadow copies at [i+4] and a
spinning cursor at [8]. Visibility D_8011AD40[i] chooses model_data_load or model_transform_setup.

```
python3 -m tools.conveyor.pipeline.blob_unit --tag w10e score func_800B59F0 --internal func_800B59F0 \
    --keep zz_caller --keep zz_caller2 --block func_800B59F0 \
    --with cloud/work/frontier/w10e/func_800B59F0/equal_quirk.c --neighbours
  EQUAL func_800B59F0: 343 words (internal, c_equal_quirk.c)
  locked bodies that differ in this unit: 0
… --with cloud/work/frontier/w10e/func_800B59F0/best.c
  FAIL func_800B59F0: 2 of 343 words differ
```
The 12 own-literal references are unverified in the aligned diff. Their bits match retail at
0x80123DC4..0x80123DD0: π/2 3FC90FDB, 4π 41490FDB, π 40490FDB, 2π 40C90FDB. The strings "BUTTON_SELECT" and
"BUTTON" are own data.

From w6d's 86/343 (stand-in group):
1. **`D_80152678` is not volatile.** Retail's reloads after the `?:` stores come from the store-then-join pattern.
   With volatile, every later use reloads (−3 words).
2. **Retail recomputes `|d|·60/10` in the then-arm because the source writes `fabsf(y - old[1])` twice.** uopt CSEs
   the `abs.s` but not the mul/div that follow (83 → 4 words). This was w6c's and w6d's open "PRE/no-CSE" residual.
   `func_800D6914` and `func_800DA2C0` have the same pattern.
3. **The visibility byte is a named local** (`vis = D_8011AD40[i]`): retail loads it into a uopt register (v1),
   not an ugen temp. Two quirks remain. The new local takes w6d's spare `f32 k` filler slot, so the frame is unchanged.
   The temp-ring phase of the later `D_8011AC98` test is then right (4 → 2 words).
4. Residual: retail colours `vis` **v1**, ours v0. The traced uopt shows equal cost for every caller-saved register,
   so v0 wins unless something in v0 interferes. `equal_quirk.c` makes the second `func_8008D870` call "return" into
   `vis` before the real assignment (prototype changed to `s32`). The dead def puts the call's v0 in vis's web, and
   everything is then EQUAL. That is a shaping quirk with a false prototype: `func_8008D870` is `void` in its locked source.
   Tried and not moving: register/u32/s8/s16 types, declaration positions, `!vis`, `(vis = …) == 0`, switch, an
   inlined getter or empty helper for the stub `func_800B59E8`. Each of those adds 8 frame bytes at the bottom, and
   the frame layout then no longer fits.
   Best next hypothesis: whatever really made v0 busy at that point. Two candidates: a real value-returning call
   whose result is discarded into the same variable, or the deleted static `func_800B59E8` with a return value.

## func_800D6914: provisional, 282/315 (compiled 321)

Real caller `func_800D7634` (L8). Main-menu sibling of func_800B59F0 (records at D_801102B4, selected D_80110638,
snap D_801105B8, speed D_801543D0, scale D_801105BC). `best.c` is the B59F0 source with the globals changed, no
visibility branch, `model_transform_setup` twice and an unconditional `y -= 40`.
- **Frame:** 144 bytes with `old` at 76, `sel_y` 68 and `tex` 62. It needs `s32 i, name` declared *above* a
  `f32 cur[12]` filler, plus the `d`/`k` filler words around `sel_y`. Scalars declared last take bottom slots.
- Residual: retail colours `&D_801105B8` s7 and constant `1` s8, and does not keep `&tex`. Ours colours `1` s7 and
  `&tex` s8, and `&D_801105B8` gets no register. That costs the 6 extra `lui` words. The trace shows three tied webs at
  save 2.2222 (`1`, `&tex`, one more), and the `D_80140BDC` address web (tot 32, nocs 15) just below them. FP
  constants: retail f22=180, f24=π, f26=0; ours f26=180, f22=π, f24=0. Next: find a block-count or tie-order
  change that ranks `&D_801105B8` first; w9 "tied priorities resolve by first appearance" did not apply directly.

## func_800DA2C0: provisional, first draft 494/579 (compiled 574)

Real caller `func_800DB1E0` (L8). `best.c` is a full draft, recovered from the disassembly:
- n = count of nonzero flags D_80116D14[0..cur), where cur = D_80116D9C;
- cursor record [28] spins 2π/s; y = 75 − n·165/(D_80149D9C − 1), eased with speed D_8015273C, clamped to [180, 540];
- 14 records (+14 shadows) at D_8011650C; the selected one (i == cur) turns to π at 4π/s, the rest to 0, snapped by
  D_80116D10;
- y starts at (s32)(sel_y·1.33 + 40n) and steps −40 per visible item; per-item easing uses D_80152740, and a local
  flag (sp+119) clears it at the end if nothing moved;
- shadow at (150, y·31/30, 310); textures "BUTTON_SELECT"/"BUTTON";
- alpha (u8 at +0x3C) is 0 outside [−160, 140] and fades over 40 units above 100 / below −120 (float→unsigned
  conversion);
- alpha goes into a copy of the Color D_80116DB0, which is stored into D_8012E700[handle].+0x3C for both models;
- invisible or transparent items get model_data_load, the rest model_transform_setup.

What it showed: the count loop needs **its own index variable** (`j`). Sharing `i` with the main loop merges the
two flag-pointer IVs into one web (s2 instead of retail's v0 pointer) and shuffles every s-register. With `j`, s0–s7
are as retail. The frame (152; `old` 124, flag 119, `tex` 102, color 96) needs fillers that are not final yet.
Residual: `yi` (retail s8) is spilled because `&D_80116D10` (save 1.55) outranks it (1.50). Ours hoists 255.0f
where retail hoists 4π. Not iterated further (breadth).

## sound_bank_unload: 134/312 (unchanged)

```
python3 -m tools.conveyor.pipeline.blob_unit --tag w10e score sound_bank_unload func_800B0EA0 func_800B0F60 \
    --with cloud/work/frontier/w10e/sound_bank_unload/best.c --internal func_800B0EA0 --internal func_800B0F60 --neighbours
  FAIL sound_bank_unload: 134 of 312 words differ
       umerge inlined into it: func_800B0F60
  EQUAL func_800B0EA0: 48 words (internal, c_a.c)
  EQUAL func_800B0F60: 2 words (internal, c_a.c)
  locked bodies that differ in this unit: 0
```
`best.c` is w3b's best, unchanged. The `preserved: a0,a3,t0–t4` signature is this function relying on its internal
callee `func_800B0EA0` (already EQUAL). No unmatched caller is needed, so it is genuinely matchable.
This pass tried about 25 more variants, none of which moved the count:
- Ramp loops: `i`-indexed, `i*8`, `i<<3`, `*cursor = …; cursor++`, for-init order, a running cursor across
  all three ramps.
- Brightening bodies: GPACK macro, named r/g/b, `u16`/`s32` element local, pointer form, lower clamp.
- A traced colour run: ratio web w97 → a3 (save 41), cursor w100 → t0 (31). Retail is the reverse.

Retail also stores `addiu a3,2; sh v0,-2(a3)` (store in the delay slot), where ours has `sh; addiu` in the slot. That
is as1 again. The `u16 *p` unroll hack is still needed. Next hypothesis: the ramp is an inlined helper
`(dst, colour, count)` (its own webs would reorder ratio/cursor). A plausible source for the 2× unroll is still
missing.

---

## What generalises
1. **Operand order of `==` sets ugen temp order** when one side is a global load and the other a field
   expression. Swap the operands before blaming ring phase. That gave 26 of the 60 words in race_countdown_display.
2. **`if (g < f(x)*k) g = f(x)*k;` with `f` written twice defeats CSE of the arithmetic** while still sharing `f`
   (`abs.s`) and the constants. That is the source form behind "retail recomputes the expression in the then-arm" (w6c/w6d).
3. **A loop-carried address/IV web shared by two loops changes every s-register.** Give a pre-pass loop its own
   index variable (func_800DA2C0).
4. **A dead definition from a call puts the call's v0 in a variable's web.** This is a v0→v1 lever. When retail
   colours a short-lived local v1 with nothing visible in v0, look for a discarded call result in the original.
5. uopt trace force specs (`p1:wN=cM`) use colour index c1=v0, c2=v1 … c12=t5, c14=s0 … c22=s8. Forcing a *split*
   web is silently declined.

## Permission denials
None.
