# Wave 11, lane w11g — results

Assignment: `func_800E7FA0` (provisional internal) + its caller `func_800E847C`; then the E4300 chain
(`func_800E451C` → `func_800E4B58`, `func_800E398C`) if blockers allow.

**Outcome: no strict match.** No change to the best scores, which are still w10a's. All scores come from
the Pi with `python3 -m tools.conveyor.pipeline.blob_unit --tag w11g score …` (whole-program unit),
using flags `-g0 -O3 -mips2 -G 0 -non_shared`.

| Function | Bytes | State | Flags | Scorer output |
|---|---:|---|---|---|
| `func_800E847C` | 2,100 | 232 words off (unchanged) | `-O3` group, E7FA0 internal | `FAIL func_800E847C: 232 of 525 words differ` |
| `func_800E7FA0` | 1,244 | 35 words off (unchanged; still provisional) | same group | `FAIL func_800E7FA0: 35 of 311 words differ` |
| `func_800E451C` | 1,588 | not attempted: provisional at best (caller E4B58 is blocked) | — | PR #128 packet: 389 + 3 extra / 397 |
| `func_800E4B58` | 2,268 | not attempted: blocked by `state_utility` (itself blocked by `menu_input_process`) | — | packet: 528 / 567 (533 compiled) |
| `func_800E398C` | 2,396 | not attempted: provisional at best (sole caller E4B58 is blocked) | — | packet: 592 / 599 (567 compiled) |

```
python3 -m tools.conveyor.pipeline.blob_unit --tag w11g score func_800E847C func_800E7FA0 --internal func_800E7FA0 \
    --with cloud/work/frontier/w11g/func_800E847C/best_group.c
  FAIL func_800E847C: 232 of 525 words differ
  FAIL func_800E7FA0: 35 of 311 words differ
```
`func_800E847C/best_group.c` is w10a's best, unchanged. `func_800E847C/lead_segment_first.c` scores 232 / 36.
It is not better, but it is the only variant that gave segment retail's colour (t2). See below.

There is nothing to integrate: no matches, and no unit_overrides or group changes.

## func_800E7FA0 — what moved and what did not (~150 variants, all in `e7/v1`..`e7/v10`, generators `e7/gen*.py`)

Retail's segment loop wants these phase-2 colours: segment t2, hi = `level[4]` t3, `lo[0]` t4, the car->level
induction pointer t5, then `lo` s0 and `lo[1]` s1. Phase 2 visits webs in ascending web number, so retail
needs the web order **segment < hi < lo[0] < IV-car < lo < lo[1]** (w10a; confirmed in the CDX of `runs/alo`,
where every `p2dec` has `bestcost=0`).

New findings:
1. **Web numbers split into two ranges.** In the trace (`runs/alo/report.txt`), webs 2–209 are numbered in
   source statement order at read time. Webs 218 and up are created later by copy propagation (`car` and
   `curve` substituted into expressions), and they are numbered after every source web.
   - IV-car is web 270, a propagated `ixa(ixa(car), i*4)`, so it is always late.
   - For retail's order, `lo` and `lo[1]` must therefore be compiler-created webs numbered after 270.
   - `lo[0]` must be a source-numbered web.
   - A named pointer `lo` can never meet this: its variable web takes its number where `lo` first appears,
     and `lo[0]` / `lo[1]` (ilod of lo) always come after it.
2. **The indexed form gets the order nearly right but the structure wrong.** The indexed form is
   `curve->level[segment + 1]` in the test and `curve->level[segment]` after the loop (`e7/v3/hi_cv.c`,
   trace `runs/hicv`).
   - Order: the copy-propagated SR pointer is web 292 (s-reg side), `lo[1]` 296, and IV-car 267 comes before
     them.
   - Structure: uopt puts the pointer's initialisation (`move IV,curve`) in the join block after
     `if (hi < sample) sample = hi;`, as ugen's listing shows (`e7`, o3s). as1 then fills the clamp branch with
     `beqzl` plus a duplicated target instruction, which makes 312 words. Retail initialises in the block
     before the clamp (`move t2,zero; move s0,t0` before `slt`). Its ugen order also puts segment's
     initialisation before lo's, which is what an SR initialisation appended to that block would give.
   - So retail looks like an SR pointer whose preheader is the clamp's own block. No source shape tried
     reproduces that.
3. **A `segment = 0;` written before `lo = curve->level`** (the for's own init is then redundant and is
   removed) moves segment's web ahead of lo. Segment takes retail's t2 (`e7/v2/a_lo.c`, 36 words; best is 35).
   Placed before the switch it moves ahead of curve and sample instead (57–67 words).

Tried without success, each scored in the unit:
- **Loop forms:**
  - indexed, with and without a named `hi` / `lo` base: 106–110 words, 312 compiled;
  - `segment = 0` before the clamp combined with `for (;…)` or `while`: 313–320 compiled, because segment·4
    no longer constant-folds into the SR initialisation;
  - comma-for initialisation: 110 words / 312 compiled.
- **The clamp:**
  - ternary, inverted and `>` spellings of the clamp: inert;
  - the clamp hoisted before `magnitude` / the `D_8014A110` test / the compare: 115–122 words / 312 compiled.
- **The if/else:** inverting the outer if/else (decay arm first): 106–113 words.
- **Inert edits:**
  - six alternative `lo` initialisers (`D_80120EEC[i].level`, casts, `&…[0]`, `+ 0`): all inert;
  - `^ 0` at each use and definition (workbench L87): inert, except two that changed structure;
  - inner-block declarations of lo, hi and segment: inert.
- **Tiny harness (`tiny/t*.c`):** five loop spellings. Only the condition-in-for form puts an initialisation
  before the clamp, and it drops SR.

Next hypothesis: find the uopt case where the SR initialisation lands in the immediate dominator, not in a
created preheader. One example would be a clamp that uopt does not treat as a join: a pattern it folds, or a
compiled-out check that changes block structure. Alternatively, accept that `lo` is a source variable and look
for a mechanism that renumbers it. Candidates are live-range splitting under pressure, or a variable whose only
surviving definition is created by copy propagation. Workbench L83/L87 have the relevant laws.

E847C is unchanged at 232. w7d's analysis stands: its s-register rotation follows from E7FA0's callee-saved
count (retail E7FA0 uses s0–s5; ours uses s0–s4). Nothing in E847C can be fixed independently.

## E4300 chain — not attempted (blockers)

`frontier show`: `func_800E4B58` is blocked by `state_utility` (396 B), which `menu_input_process` blocks in
turn. E4B58 is also the only real caller of both `func_800E451C` and `func_800E398C`. So each of the three can
be at most provisional this wave.

The PR #128 packet (`cloud/work/ipa-groups/codex_path_search_a7`, frozen NONMATCH) records:
- the starting distances: E451C 389 + 3 extra, E398C 592, E4B58 528;
- size mismatches: 400/397, 567/599 and 533/567 words;
- unresolved homes and frame (E451C 184 vs retail 232).

None of this is a near-miss lane. The budget went to E7FA0 / E847C, the only pair that could produce a strict
match.

## What generalises

- **Two numbering ranges.** uopt numbers source-read webs in statement order. It numbers webs created by copy
  propagation (pointer locals substituted into expressions, e.g. `car->level[i]` → `ixa(ixa(D,…),…)`) after
  all of them. A phase-2 order in which a value depending on a named local comes *before* the local is
  therefore impossible. The value with the late number must be compiler-created: a copy-propagated
  expression or SR pointer.
- **A for loop's constant init is redundant if the same store comes earlier.** Writing `segment = 0;` earlier
  in the same block keeps the earlier store, so segment's web number moves to that position. This is a cheap
  way to renumber one variable without changing code shape. It is not a dead store: the for's init is the one
  removed.
- **`beqzl` plus a duplicated instruction in ours where retail has `beqz`** means uopt put a value in the join
  block that retail computed before the branch (here an SR preheader initialisation).

## Notes

- Tools: w10a's toolset copied to `tools/` and retargeted to w11g. The builder scratch is
  `~/rush2049/scratch/frontier/w11g` (copied from base, with src/blob, include, tools/cloud, asm/us/blob and
  the lock synced; uopt binary copied from w10a's build). At most 1–2 jobs.
- No permission denials. Nothing committed, spliced or edited outside this lane directory.
