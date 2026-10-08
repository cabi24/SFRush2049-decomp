# w14r results (wave 14 lesson: inline named locals / name repeated subexpressions)

No function matched. No cloud/matches file, group, claim or splice was produced. Nothing committed.
Scoring is standalone bscore (vbatch) unless noted. No top candidate beat its control, so no whole-program
`blob_unit` run was needed (none qualified to confirm). No permission denials.

## Summary

| function | bytes | prior | this lane (standalone strict) | state | residual |
|---|---:|---|---|---|---|
| particle_lifetime_set | 732 | 59/183 (w11d, unit) | 59 (control reproduced) | unchanged, plateau | register/temp ring; as1 temp choice (see w11d) |
| drone_target_update | 496 | 69/124 unit (w14h) | 70 standalone with bp/cnt removed (same as before removal) | unchanged, plateau | three record pointers (retail s4/s5/s6) |
| func_800AC9BC | 224 | 25/56 (w14h) | 25 (control reproduced) | unchanged, plateau | beqzl delay-slot forms; q+=2 branch vs ori conflict |
| race_position_update | 484 | 114/121 (w14k) | 114 (control reproduced) | unchanged, plateau | width web / s5 web (w14k force.sh) |
| func_800B9740 | 408 | 83/102 (w14k b4 v0015) | 83 (control reproduced) | unchanged, plateau | retail keeps one callee-saved loop reg |

Scorer lines (bscore, `vbatch.sh`, control rows):
- `strict 59 mnem-missing 0 aligned-missing 58 size +0 v0000.c` (particle_lifetime_set, b1)
- `strict 70 mnem-missing 18 aligned-missing 49 size +0 best.c` (drone_target_update/c, bp/cnt removed)
- `strict 25 mnem-missing 11 aligned-missing 22 size +0 v0000.c` (func_800AC9BC, b1)
- `strict 114 mnem-missing 24 aligned-missing 56 size -13 v0004.c unverified=4` (race_position_update, b1; v0000 ties at 114)
- `strict 83 mnem-missing 24 aligned-missing 65 size +2 v0000.c` (func_800B9740, b2 control = recovered w14k b4 v0015)

## Lesson check (particle_lifetime_set first)

The inline-versus-name axes did not reproduce the w14 place_cars_in_order effect here.
- b1 (800 of 1024): choices for right/bottom/left/top (named versus inline), scale (three forms in verts),
  list (named versus `D_80114628[i]`), a `p` launder for the flags store, the `name` copy (in or out of the loop),
  and declaration order. Best 59 (v0000, v0149, v0165, v0314). No strict improvement.
- b2i (800 of 1024): named index local `idx = D_80151AD0 - 1`. Best 172. **Worse by 113.** Naming the index
  moves the value out of the temp ring. This is the reverse-direction case from the brief.
- b2s (800 of 1024): named `Motion *slot` pointer on top of idx. Best 164. Worse.
- b3 (800 of 1536): the four assignment statements in all 24 orders, plus scale hoisting and `name` hoisting,
  with the right/bottom/left/top inline choices fixed to named. Best 59 (v0000, v0535). No improvement.
- g1 (wbgen, 980 one-edit neighbours of best.c): best 59, control 59. No improvement.
- Diagnose (`tools.conveyor.pipeline.diagnose one particle_lifetime_set --source best.c`), run on the Pi, gave
  `verdict mixed(constant:2, structural:3, register:57)`, `lever none-known`, lanes pool none, temp slot 1,
  shared slot 4, fp-pool/fp-temp none, `strict_words_differing 62`. Note: diagnose compiled with `-g0 -O2`
  (its default); the lane flags are `-O3`, so its counts differ from vbatch. The verdict is register-class.
- Stop: 3 consecutive batch rounds (b1, b2, b3) with no strict improvement, plus a wbgen round.

## drone_target_update

- Unused locals removed first (`bp`, `cnt`, and the `bp = record;` store). The cleaned control scores 70 standalone,
  the same as the file with them (70). They cost nothing in standalone, so they are not a frame residual.
  `cloud/work/frontier/w14r/drone_target_update/best.c` is the cleaned file.
- t/t1.c (BND, DO, DC, SEL, IFX, END, CFG, CH, CM, UNR, BND on two stores): b1 512 variants, best 70 (control,
  many ties). The single-pointer form does not reach retail's three-pointer shape; that needs explicit text.
- wbgen g1 (78 one-edit neighbours): best 70, no improvement.
- Caught a bug in my first template (store line linked to SEL): fixed before the scored round.
- Stop: 2 batch rounds with no improvement after the cleanup.

## func_800AC9BC

- t/t1.c (DL, DX, DM, QT, QO, NC on the best draft): 64 variants, best 25 (v0000, v0001).
- t/t2.c (adds WH loop form, EQ compare order, DO/QT/LT2-free): b2 256 variants, best 25. No improvement.
  The first t2 had nested choice markers (SMX inside DX, ORD dropping `*quad`); both removed before the scored round.
- wbgen g1 (189 neighbours): best 25. No improvement.
- Stop: 2 batch rounds + wbgen with no improvement. Residual unchanged from w14h (delay-slot placement, branch
  versus `ori` for `q += 2`).

## race_position_update

- t/t1.c (QQ unused decl, NI `n = len*0-1` inline, WS, LT, LE, SP, IX, WC, DO width/wide order): 512 variants,
  best 114 (v0004..v0007 and the control). No improvement.
- `room` inline was not possible: the `while` condition needs it and the call has side effects. Not tested.
- wbgen g1 (189 neighbours): best 114. No improvement.
- Stop: 2 rounds + wbgen, no improvement.

## func_800B9740

- Recovered the 83-strict file: w14k `b4` was not kept on disk, so I regenerated it with `w14k/func_800B9740/t4.c`
  (`vgen --max 400`, 288 variants). `v0015.c` scores `strict 83` again. The w14k `best.c` (91 standalone) is a
  different file; w14r `best.c` is the 83 version.
- t/t1.c (EL duplicated-body form, RG range pointer inline, reversed `for`-less form): 16 variants, best 83 control.
  EL made a dead `if (0)` shaping construct in some options; treat that template as exploratory.
- t/t2.c (built from the 83 file; BRK break/no-break, VA/VB vertex forms, AX axis order, MN/MX min/max forms,
  EQ1 `eligible` test, PC primary-count compare, DO decl order): 800 of 1152 variants, best 83 (v0000, v0022, v0036,
  v0041). No improvement.
- wbgen g1 (147 neighbours): best 83. No improvement.
- Stop: b2 and g1 with no improvement after the regenerated 83 control.

## Counts (scored this lane)

particle_lifetime_set ~4,180 (b1 800, b2i 800, b2s 800, b3 800, g1 981); drone_target_update 591 (b1 512, g1 79);
func_800AC9BC 510 (b1 64, b2 256, g1 190); race_position_update 702 (b1 512, g1 190); func_800B9740 1,252
(b4x 288, b1 16, b2 800, g1 148). Every round's score.txt is kept under the lane dir.

## Permission denials
None.

## Integration notes
None. No overrides, group supersession or cloud/matches files. All five best.c files are local drafts.
