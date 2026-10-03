# func_800D1248 / func_800D11BC: round 12 near-miss work (2026-10-03)

**Status:** NONMATCH research. No lock, splice or coverage claim.
Scored with `tools/cloud/score.py fn` (IDO 5.3, toolkit `796ae99a…`, default
`-g0 -O2 -mips2 -G 0 -non_shared` + `-Wab,-r4300_mul`) on watchman2.

The owner's pause on func_800D1248 applies to one other worker only. The owner
cleared this work for Claude on 2026-10-03.

## func_800D1248 (81 words): 14 → **9** words differ

`func_800D1248.c` is self-contained. Change from the previous best
(`cloud/work/near-miss/func_800D1248/base.c`): the final table index is a
separate `s16 idx` local, loaded and then used. That moves the `lh` into a
uopt-colored register (`v0`), matching the target's tail
(`lh v0 / sll t6,v0,8 / addu t6,t6,v0`). It removes 5 of the 14 differing words.
Reusing the loop counter `i` for it gives `a0`, not `v0` (12 words).

**Remaining residual:** words 7–14 and 16, `a2→a0` for `arg0` in the
then-block before the first call. Both objects emit `move a2,a0` at entry, and
the workbench reports identical uopt colorings (pool 14/14, temp 7/7). The
difference is ugen's reuse of the still-valid incoming `a0` inside the
then-block. The target does not reuse it. Workbench lever: `none-known`.

None of these moved it (each stayed at 9 words, except as noted):
- early return / `if (== -2) {}` / `if (!= -2) {} else {}` / `goto` /
  `switch` single case / `do { if break } while (0)` / trailing `return`
- local copy `p = arg0` (`void *`, `s8 *`, used everywhere, or everywhere but the check)
- parameter typed `s8 *`, `s32` cast to pointer, K&R definition
- all four orderings of the zero, −1 and float-copy statements
- locals declared inside the then-block scope
- `func_800D11BC` defined earlier in the same file (real reconstruction or empty stub)
- `-g1`/`-g2`/`-g`: 80 words; `-g3`: 15 words

Reopen only with new evidence about ugen's parameter-register tracking, for
example a ugen trace (`DKWB_UGEN_TRACE`) showing where it invalidates `a0`.

## func_800D11BC (35 words): 7 words differ, unchanged

`func_800D11BC.c` is a full reconstruction. Its residual is as1 ordering
before `jal func_800CF06C`. The target keeps the stores in source order
(three `swc1 0`, `li t6,1`, `swc1 1.0`, `sb`) and has `move a0,s0` in the
delay slot. Ours hoists the `sb` above the float stores and puts `swc1 72C` in
the delay slot. These were invariant at 7 words: all store orderings,
chained assignments, `0` against `0.0f`, `u8` against `s8`, `-O3`, and
`-Wo,-loopunroll,0`. `-O1`, `-mips1` and `-g*` are far worse. Both functions
are adjacent (same TU). A shared TU-level cause, such as a different as1/ugen
setting, has not been found.
