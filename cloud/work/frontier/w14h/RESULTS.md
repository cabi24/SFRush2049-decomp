# w14h results

No functions matched. No cloud/matches, group or splice files were written; nothing committed or spliced.
Builder scratch: `~/rush2049/scratch/frontier/w14h` (copied from base; src/blob, include, tools/cloud, asm/us/blob and
blob_matched.lock.json synced from the Pi). Lane dir: `cloud/work/frontier/w14h/`.

Metric note: `vbatch` (bscore, standalone) prints `strict` = differing words (lower is better). The w14b drafts score
32 (func_800AC9BC) and 114 (drone_target_update) on the same standalone metric. Unit numbers are from blob_unit.

## func_800AC9BC (0x800AC9BC, 224 B, 56 words) — FAIL, 25 of 56 words differ (improved from 32)

Best draft: `func_800AC9BC/best.c` (readable form of `func_800AC9BC/b4/v0471.c`).

    FAIL func_800AC9BC: 25 of 56 words differ | words 36 ops 18 norm 18 | frame 0/0      (blob_unit --tag w14h score, --neighbours)
    locked bodies that differ in this unit: 0
    strict  25  mnem-missing  11  aligned-missing  22  size +0  best.c                    (vbatch standalone)

Vbatch rounds (templates `func_800AC9BC/t1.c` … `t4.c`, expansion by `vgen.py`, scoring by `vbatch.sh`; -O3 default flags):

| round | template | variants (scored) | best strict | note |
|---|---|---|---|---|
| r1 | t1.c (8 choice points, product 68040) | 400 | 30 | v0033; w14b best = 32 standalone |
| r2 | t1.c | 20000 (sampled, seed 1) | 30 | plateau |
| r3 | t2.c (adds early leaf-load and q=1 placements, leaf-test forms with `lm`) | 20000 | 25 | v0224; 2007 variants failed to compile (broken `lm` option, fixed) |
| r4 | t2.c (fixed) | 20000 (seed 4) | 25 | v0471 = best.c |
| r5 | t3.c (y0 hoist, s32 q decl) | 20000 | 25 | no gain |
| r6 | t4.c (child-pointer forms, unsigned mask, `mx > x`) | 20000 | 25 | no gain, stop |

Stop: three rounds (r4-r6) without a new best strict count. -O2 on the 40 best files: 57 and worse.

Residual (`dis_cmp` against retail, standalone object of best.c):
- Retail loads y0 (`lh t4,8`) in the delay slot of `bgezl` before the mx sign fix-up and duplicates the `lbu t3,3` leaf
  load into a `beqzl` delay slot. The sweep never reproduced this: all variants load y0/y1 before the fix-up. Looks like as1
  delay-slot filling; source placement of the loads did not move it.
- Retail `q += 2` is a branch (`beqzl` + `addiu a2,a2,2`). The best draft uses `q |= 2` (`ori`) after the sweep preferred
  it, while the branch forms score 32. Branch and branchless forms cannot both be right; this is the open conflict.
- `mflo`/`addu` child-address temp is t4 in retail and t3 in ours: colour-only candidate. Next: `force.sh` on the child
  pointer web (`tools/trace/force.sh`), not more variants.

## drone_target_update (0x800D7E88, 496 B, 124 words) — FAIL, 69 of 124 words differ (improved from 110)

Best draft: `drone_target_update/best.c` (= `drone_target_update/b4/v0038.c` plus a header). It keeps unused locals
`bp`, `cnt` and the `sel` store: these are sweep residue, NOT a disclosed frame residual. Do not splice it.

    FAIL drone_target_update: 69 of 124 words differ; compiled body is 123 words, target 124      (blob_unit --tag w14h score, --neighbours)
    locked bodies that differ in this unit: 0
    strict  70  mnem-missing  18  aligned-missing  49  size +0  best.c                          (vbatch standalone)

Vbatch rounds (`drone_target_update/t1.c`, `t2.c`, `t4.c`, `t5.c`, `t6.c`; `b1`…`b5`):

| round | template | variants | best strict (valid) | note |
|---|---|---|---|---|
| r1 | t1.c: gs, bx-less, sc, ec, pl, tl (729 total) | 729 | 84 | sc=1 and peel-form pl=1 helped |
| r2 | t3.c: adds bp pointer, sel local, cf/sc on sel | 2592 | 70 | 1944 failed: nested choice markers (fixed by t4) |
| r3 | t4.c: no nested choices | 20000 of 23328 | 70 | `sel`-based uses valid only with `sel` placed; the unfiltered best 64 was invalid |
| r4 | t5.c: cnt local and `end` forms via cnt | 20000 of 77760 | 70 | no gain |
| r5 | t6.c: outer do vs for(;;) | 20000 of 51840 | 70 | no gain, stop |

Validity: a variant reads `sel` only when the `sel = record->selector;` placement is on, `cnt` only with the `cnt`
placement, and `bp` only with `bp = record;`. The sweep prints the unfiltered best first; compile failures are counted in
each `all.txt`. Filter by these rules before reading a rank.

Stop: rounds r3-r5 (three in a row) gave no new valid best.

Residual: retail keeps three record pointers (s4 = selector reads, s5 = peel stores and loop test, s6 = unrolled-loop
stores, stepped by 76). The best draft uses one pointer. Retail also loads `active_player_count` as `lui`/`lh` at each
read. Choice points on the single-pointer template cannot express three pointers; they need explicit text. Next
hypothesis: write the three-pointer form as one template (not choice options), then force the colour of the record web.

## Permission denials
None.

## Integration notes
- No overrides, no group supersession, no cloud/matches files.
- Both drafts are NOT matches and not spliceable. Their numbers are in the scorer lines above.

## What generalises
- `vbatch` scores ~20000 variants in about 110 s on watchman2 (2 jobs). Exhaustive product sweeps are cheap; the limit
  is template design, not compile time.
- Nested choice markers do not work: the template text between `/*@{` and `@}*/` may not contain another choice (the
  regex is not nested). Keep choices flat and use separate placement choices with the same name for linked lines.
- `vbatch.sh` passes `$*` unquoted, so `--flags "..."` fails; use the bscore default flags (`-g0 -O3 -mips2 -G 0 -non_shared`).
- Rank by the strict column, not by mnem-missing: the `mnem-missing` order can put a worse strict count first.
