# w36z RESULTS (wave 36, game-code near-misses left by finished lanes)

| function | GC identity | bytes | state | best file |
|---|---|---|---|---|
| func_80100E58 | drawRaceStandings@gameoverscr.c (confirmed w36m) | 2732 | **MATCH in group** (unit EQUAL 8/8; `score.py group` MATCH with/without --claims) | groups/frontier_results_table/results_table.c (= f0e58/v/n7.c) |
| highscore_animText | animText@highscorescr.c (confirmed w30a/w33a) | 1780 | not advanced: 27/445 frame-only (584 vs 704), EQUIVALENT draft unchanged | src/blob/groups/highscore_anim/highscore.c (= w33a hs/best.c) |

Builder scratch: ~/rush2049/scratch/frontier/w36z (copied from base 2026-10-21).

## Log
- started; scratch + lane dir created.
- func_80100E58: w36m p8 re-scored in my unit: `FAIL func_80100E58: 537 of 683 words differ; compiled body is 682 words, target 683 | words 189 ops 19 norm 19 | frame 528/528`.
- NEW oracle `w36z/tools/patch_acc.py` (on top of w36m patch_spl2 output; binary watchman2 w36z/bin/uopt_acc):
  `SPL_ACC=bb,..` forces a block into the split piece; `SPL_DEC=n` evaluates the growth test with numintf-n (SPL_SYM=sym).
  * SPL_ACC=45 SPL_SYM=15 (force the header block only): 152 rows, 679 words — bb57/bb112 are then rejected
    (numintf 25 > 2*12), so forcing bb45 alone is NOT retail.
  * SPL_DEC=1 SPL_SYM=15 (one fewer interferer): **683 words, 121 rows** (structure = retail; only bb45's evaluation
    order and the temp-register rotation it causes remain). Confirms w36m's numintf<=20 target AND shows a second residual:
    retail evaluates s4 = &D_80143F54[row] (expr bit 64, INSERT 45) BEFORE 160 - cols*60/2 (bit 33) in the header block.
- **The 21st interferer is the constant 160 in the final draw block (bb110).** Labelled interferer list (tools/names.py:
  SPLI dump + pretrace report): 3, 1 (pieces), 2056, &D_8014AA16, &D_80152906, &D_80120334, &D_80144DA8, &D_80120324,
  &D_80152908, &D_80152734, 14, **160**, 76, &input_rec0, &D_801146AC, &D_801461D0 (s7), &D_80118E20, &active_player_count,
  str (s6), col (s5), rows (lr 0, printed `0/0` by w36m's dump: it is live range #0 = rows, not a null node).
  w33a's draft (23) had no 160 (its final call was written `column*60-columns*60/2+190`) but still counted
  &D_80143F54 and the constant 2. All loop-invariant single-use constants/addresses with save 5 are hoisted to the loop-2
  preheader bb39 and still undecided when the cols piece splits; each one counts.
  * s2 = p8 with the final call `state_utility(col * 60 - cols * 60 / 2 + 190, row * 14 + 43, str)` (diagnostic form,
    same bb110 code): numintf 20, bb45 accepted -> `FAIL func_80100E58: 119 of 683 words differ | words 121 ops 14`
    (from 537). String literals instead of the D_801203xx externs: no change (still lda webs). x as `col*60+160-...`
    (s3/s4/s5): numintf 20 but uopt canonicalises differently, loop 1 and bb45 change (552/683).
  * Remaining in s2: bb45 evaluation order (retail computes s4 = &D_80143F54[row] before s8/s1) and the s1 spill home
    (retail 40(sp), ours 48(sp)). spill.sh: homes go in itable order (33 s8, 34 s1, 40 t3, 64 s4 reuse, 65, 274 s3);
    retail's s1 home = 4th slot => col*60 (bit 34) is numbered AFTER D_80143F54[row-1] (64/65) in retail, i.e. in
    loop 1 `col * 60` first appears after the switch (x added at the drawRect call). Testing t1-t3.
- t1/t3 (loop 1 drawRect x written inline after the switch): loop 2 fully matches (19 rows, all loop-1 head scheduling).
  **u3 = loop 1 `func_80100D5C(160 - cols * 60 / 2 + col * 60, 36 + row * 14, 60, 14, c)` (both inline, w33a's loop-1 form)
  + s2 final call -> `FAIL func_80100E58: 4 of 683 words differ | words 2 ops 0 norm 0 | frame 528/528`** (f0e58/best.c).
  Remaining 2 rows: loop 1 else-branch `c.a = 192` store order (retail stores c[3] last after c[0..2]).
- **func_80100E58 EQUAL in the unit**: u3 + loop-1 else-branch colour init on two lines as MAT3's sibling
  drawStuntStandings writes it (`c.r = 0; c.g = 0; c.b = 0;` / `c.a = 192;`):
  `u5.c: EQUAL func_80100E58: 683 words (internal, c_u5.c) | words 0 ops 0 norm 0 | frame 528/528` (four lines, u4: also EQUAL).
  best = f0e58/best.c (= v/u5.c). Next: naturalness pass (final call form, time2/bgY), group packaging.

### Packet: superseding group `frontier_results_table` (cloud/work/frontier/w36z/groups/frontier_results_table/)
Superseded IN PLACE: same name, ctx_gr3_b.c byte-identical, results_table.c = installed file + func_80100E58's C and its
two inlined helpers (rs_DrawEllipseText, rs_SetJust: non-static, internal) + `extern volatile s16 D_801543CA` (only
func_80100E58 uses it); every old member's body unchanged. group.json: members += func_80100E58, claims = [func_80100E58],
keep unchanged (func_80102F30), context unchanged. Overrides: none new (func_80100E58 was plain asm, no locked stub).
Unit (Pi): `python3 -m tools.conveyor.pipeline.blob_unit --tag w36z score func_80100E58 func_80102F30 drawMain
func_80101904 func_80101D84 func_80102448 func_80100D5C drawBattleScore --with
cloud/work/frontier/w36z/groups/frontier_results_table/results_table.c $(cat cloud/work/frontier/w33a/f0e58/uargs.txt)
--internal rs_DrawEllipseText --internal rs_SetJust --neighbours`
```
  EQUAL func_80100E58: 683 words (internal, c_results_table.c)
  locked bodies that differ in this unit: 0
blob_unit score: 8/8 equal
```
Builder `score.py group cand/frontier_results_table` (no --claims): all 8 members MATCH (func_80102F30 `own .rodata
verified at 0x80124864..0x80124880`, drawMain `own .rodata verified at 0x80124848..0x80124864`, func_80101904,
func_80101D84, func_80102448, func_80100D5C, drawBattleScore, func_80100E58), rc=0. With `--claims`:
`func_80100E58: MATCH`, rc=0 (context slot_state_setup/object_create/func_800B669C informational as before).
Source forms (header of results_table.c): loop-1 cell position `x = ...; bgY = 36 + row * 14;` computed right before
drawRect; final text x spelled `col * 60 - cols * 60 / 2 + 190` (= GC x + 30; `x + 30` makes the constant 160 a
21st interferer: n2 532/683); loop-1 else colour init on two lines (as GC drawStuntStandings); two f32 locals
time/time2 for the two time columns. No unused locals (w36m's bgY/time2 residual is gone), no pads.
Variants: n1 (`y + 7` in the final call) EQUAL, n3 (time2 used) EQUAL, n7 (both + bgY used) EQUAL = final;
n8 (bgY only, x inline) 643/683 (one word longer: y numbered first).

## highscore_animText — not advanced (new facts, written up)
Unit harness (NEW, needed since highscore_anim is installed): `hs/hu.sh FILE.c` renames the file's address-less helpers
(`hs/ren.sh`, suffix `_z`) so blob_unit does not rename them `__uK` against the installed group's copies (which left
DrawCircuitCode__u0 as an un-inlined call: 268/445), with `hs/uargs.txt`. Baseline reproduced:
`FAIL highscore_animText: 27 of 445 words differ` / DrawName, DrawGhost, DrawUnlocked EQUAL / 0 locked bodies differ.
Tried (all 12 s unit runs, `hs/v/`):
- e2: helpers hs_CenterText/hs_RightText/hs_font_set/hs_SetFont defined AFTER animText (prototypes first): 27/445.
- e3: DrawCircuitCode/DrawScores defined after animText: 27/445. e4: both: 27/445. e1: animText defined first: 27/445.
  `nm` of the unit object: our umerge emits the file in DFS postorder from the kept root animText, visiting callees in
  REVERSE call order (DS, DG, DN, DCC, DU, animText) whatever the definition order (e1..e4 identical order).
- h1: DCC/DS call the real HAL_SetFont (object_create, locked) instead of the local copy: umerge does not inline it in
  the unit (394 words, calls) -> not usable.
Observation (for the next lane, not a conclusion): retail's emission order differs from the unit's DFS order in
both files: high-score draw file retail DU, [stub], DN, DG, [stub], animText (= animText's call order, FORWARD) vs unit
DS, DG, DN, DCC, DU, animText (reverse); results table retail D5C, E58, 1904, D84, drawMain, 2448, drawBattleScore, F30
(F30's callees forward, drawMain's E58/1904/D84 reverse of its case order) vs unit D5C, drawBattleScore, 2448, 1904,
D84, drawMain, F30 — and every results-table member still MATCHES, so emission order alone is harmless. No single
"callee visiting order" explains retail's sequence; retail's umerge input/keep set differs from the unit's somehow,
which is where w33a's "stale callee size" hypothesis (nested helper areas absent) would have to come from. - d1: DrawScores' label as the local buffer of an inlined hs_DrawEllipseText(s16 x, s16 y, s16 w, char *text) (char[256];
  as func_80100E58's TB_DrawEllipseText): `FAIL highscore_animText: 24 of 445 words differ`, frame 720 (worse), fixes the
  loop-2 `addiu s6,s6,4` as1 row but not `move a0,s8` / `li s8,5`; not kept.
No source-level lever found; draft stays EQUIVALENT context.

## Identities proven
None new: func_80100E58 = drawRaceStandings@gameoverscr.c was already confirmed by w36m; its globals (gNumCars
D_801543CA volatile s16, gCircuitPlayer D_801543D4, gRankedPlayer D_80143F54, gLap D_80144018, gNumLaps D_80152734,
gBestLapTime D_80144DA8, gGameCar score/place/place_locked D_80152908/D_80152906/D_80152907 (stride 952),
model[].slot D_8014AA16 (stride 2056), playerTextColor D_801146AC, gPlayers input_rec0 (stride 76, save at +72)) are
in w36y's map. (TB_DrawEllipseText's N64 buffer is char[160]; TB_SetJust = the D_80118E20 pair.)

## What generalises
- **A split piece's growth test counts every undecided loop-invariant constant/address live range hoisted to the
  loop preheader** (f_addadjacents: add a block only if `2*regsleft >= numintf + shared`). A constant that is the FIRST
  operand of a subtract (`160 - d`, ctx=3) becomes a LIVE-RANGE candidate even when ugen later folds it away
  (`(160-d)+m+30` -> `(m-d)+190`): writing the same value without the constant-first subtract removed exactly the one
  interferer (537 -> 119 words). Check `pretrace.sh` "register candidates" for ctx=3/5 constants in the blocks a
  rejected piece needs.
- **Oracles that prove a split hypothesis before the source hunt** (`w36z/tools/patch_acc.py`, binary uopt_acc):
  `SPL_ACC=bb` forces a block into a piece (showed forcing bb45 alone is NOT retail: 152 rows), `SPL_DEC=n` pretends
  n fewer interferers (683 words, 121 rows: proved the count was the whole structural residual), `SPLFS=1` traces
  f_findsharedintf (showed w36m's "null" counted node is live range #0 = `rows`).
- **Spill-temp homes give the itable (first-appearance) order of coloured expressions**: retail's s1 home one slot lower
  meant `col * 60` is first seen after the switch's gRankedPlayer[row-1] — fixed by computing loop 1's cell position
  right before the drawRect call (119 -> 4 words), and the same reorder fixed the header block's s-register order.
- GC two-line colour init (`c[0..2] = 0;` / `c[3] = 192;`, as GC's sibling drawStuntStandings) is the as1 store-order
  fix (4 -> 0). The unused-local frame residual vanished once the GC's second `time` and the loop-1 `bgY` were used.
- blob_unit runs (25 s for 8 functions) are 3x faster than `us.sh` and also check every neighbour: use them for
  variant loops once the structure is right.
- When a group is already installed in src/, a candidate that redefines its address-less helpers gets them renamed
  `__uK` (and not inlined); rename the candidate's helpers (`w36z/hs/ren.sh`) to score it in the unit.
