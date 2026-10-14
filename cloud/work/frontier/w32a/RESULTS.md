# w32a — high-score / results (wave 32), IDO 5.3 -O3

Status: FINAL (wave 32). Unit tag `w32a`; builder scratch `watchman2:~/rush2049/scratch/frontier/w32a` (deleted at
the end). No standalone match, no group packet (func_80105480 did not close, so the high-score group cannot be
claimed: its two EQUAL members are provisional on it). One new unit-EQUAL member: func_80104B14 (DrawGhost).

| function | bytes | state | unit (us.sh / blob_unit) | draft_check | residual class |
|---|---|---|---|---|---|
| func_80105480 (= GC animText@highscorescr.c) | 1,780 | draft `func_80105480/best.c` (= `hs/best.c`) | `FAIL func_80105480: 27 of 445 words differ` (words 33 ops 2, frame 584/704) — unchanged | **EQUIVALENT** 2,473 calls / 4 scripts (unit, w32a run) | frame only (inline-area layout, §2) |
| func_80104704 (= GC DrawName) | 1,040 | `EQUAL func_80104704: 260 words (internal, c_best.c)` | EQUAL (provisional: caller 5480 unmatched) | EQUIVALENT (inside 5480) | none |
| func_80104B14 (= GC DrawGhost) | 2,404 | **`EQUAL func_80104B14: 601 words (internal, c_best.c)`** (was 17/601) | us.sh `words 6 ops 0` = the 6 unverified own-.rodata rows | EQUIVALENT (inside 5480) | none; provisional on 5480 |
| func_80103D28 (= GC DrawUnlocked) | 2,516 | unchanged w31a draft (same file) | `FAIL func_80103D28: 588 of 629 words differ` (words 340 ops 48, frame 264/264) | EQUIVALENT (inside 5480) | colour/hoisting of the per-category "before" base addresses (§5) |
| func_80105EA8 (= GC animResults@hud.c) | 2,508 | unchanged w31a draft `func_80105EA8/best.c`, `dc.args` = `--params arg0=a0` | `562 of 627` (words 358 ops 41, frame 400/400) | EQUIVALENT (w31a) | PRE moves `playerIdx*16` above the font-height call + colouring (§3) |
| func_80100E58 (= GC drawRaceStandings) | 2,732 | `func_80100E58/best.c` = w30a body REBASED onto the current `frontier_results_table/results_table.c` | `534 of 683 words differ; compiled body is 682 words` (words 180 ops 9, frame 528/528) | not run | split acceptance of `columns` at bb45 (§4) |

Unit command (Pi): `python3 -m tools.conveyor.pipeline.blob_unit --tag w32a --jobs 2 score func_80105480 func_80104704
func_80104B14 func_80103D28 --with cloud/work/frontier/w32a/hs/best.c $(cat cloud/work/frontier/w32a/hs/uargs_hs.txt) --neighbours`
```
  FAIL func_80105480: 27 of 445 words differ
  EQUAL func_80104704: 260 words (internal, c_best.c)
  EQUAL func_80104B14: 601 words (internal, c_best.c)
  FAIL func_80103D28: 588 of 629 words differ
  locked bodies that differ in this unit: 0
```
draft_check: `python3 -m tools.recomp.draft_check --remote rocky --unit func_80105480,func_80104704,func_80104B14,func_80103D28
cloud/work/frontier/w32a/hs/best.c --params func_80104704:p=s5 --scripts ghost_pak,unlock,player_finish,race_finish` ->
`VERDICT: EQUIVALENT over 2473 calls / 4 scripts (ghost_pak 1144, unlock 308, player_finish 863, race_finish 158)`.

## 1. func_80104B14 (DrawGhost) — closed in the unit (lever: "force the register map first, then the source lever")
- force.sh `p1:w276=c1` (gGhostStatus[pak] PRE web -> v0) is declined with `forbidden=0x40000000` = v0 ONLY, and v0
  never appears among the web's p1cost candidates; forcing every earlier v0 holder elsewhere (w134 -> v1/t0, w356)
  does not free it. So it is not an interferer: uopt excludes v0 for the whole web because one occurrence (the PRE
  reload of `gGhostStatus[gGhostPak]` at the end of the status-1/2 arm) sits in the same block as, after, the
  fcvt_wrapper call (a call's v0 definition in a block of the web's range).
- Source lever: end that block — `do {} while (0);` after the arm's time sprintf, before `break;` ->
  `EQUAL func_80104B14: 601 words` (`if (i) {}` there works too). DISCLOSED SHAPING DEVICE (compiled-out block end).
- Frame: the unused GC local `s16 gFreePages` stays (exact frame residual 568 -> 576, one slot between `str` and
  `str1`; disclosed). Tested alternative: the arm's time format as an inlined helper `gh_PrintTime(buf, t)` gives frame
  576 without gFreePages, but the 4 B land below the locals instead of between `str` and `str1`, so every buffer is 4 off
  (`h1.c`, 25 words): the slot is a real named local, not an inline area.

## 2. func_80105480 (animText) frame — lever tried, not closed (one lane-hour + analysis)
Retail anchors (frame-top-relative, T = 584): p T-4; DrawCircuitCode x T-12, i T-20, str[64] T-88 (all as ours); the
DrawScores sprintf buffer T-192; the ellipse-name buffer (race_position_update dst) T-448; the last font site's
`old` (sw v0 after slot_state_setup in the times loop) T-460 = label - 12; frame 584. Ours (base): 704; tool
`tools/anchors.sh FILE [UARGS]` prints both.
Facts established (synthetic IDO 5.3 -O3 tests on the builder, `cc -K` + merged-ucode Mmt dump):
- Every umerge-inlined call adds >= 8 B to the caller's Mmt (no-parameter void helper: +8 per site; s16+pointer
  params: 8; nested call +8), whether the helper is static, `__inline` (cfe does NOT inline it at -O3), `register`
  params, in another file (uld cross-file inlining: identical), in a switch arm, or in a loop; areas are never shared
  between sites. The return-value vreg is one per (caller, callee) at the top of the homes. The frame keeps the whole
  Mmt whenever any local needs memory (a function without memory locals drops it: the "no local area" rule).
- With retail's anchors the budget does not fit 8 B per helper site: retail keeps the sprintf buffer adjacent to the
  ellipse buffer (T-192 = label + 256) and has at most ~32 B of homes below the label, the first of them the times
  loop's font site (old at label-12).
Variants (all complete, `hs/v/`): `e1.c` = GC structure — DrawScores locals `i, numWid, str[16]` (GC) and an inlined
`hs_DrawEllipseText(x, y, width, text)` helper owning `char label[256]` (GC TB_DrawEllipseText): `24 of 445 words
differ | words 31 ops 6 | frame 584/640`, reproduces retail's label->old relation (old = label-12) and the s0 spill
home at 92; `e4.c` = e1 + the title (font 13 + centred table name) drawn in animText's case 5 before DrawScores():
frame 648 but the sprintf buffer lands exactly at retail's T-192 (the remaining gap is the 72 B of loop-1/2 font sites,
DS vregs and the ellipse helper's parameters between str and label). Written-out font/centre sites (macros, named
`previous` holders, s16 temps: `m1.c`, `w1.c`, `e2.c`, `e3.c`) lose the inlined-return `move` artifacts (300+ rows):
the helpers ARE inlined calls in retail. Defining DrawScores/DrawCircuitCode after animText (`a1.c`): no change.
Next hypothesis: start from `e4.c` and find which of the loop-1/2 helper sites (font 5/11/10, the ellipse parameters)
retail does not pay for; check `old` first — a spill temp, not a home, would mean retail's Mmt ends at the label
(T-448) and all helper homes are absent, which umerge never does in our tests (so: a different helper boundary).

## 3. func_80105EA8 (animResults)
- regmap_verify (lever 3 step 1): `python3 -m tools.recomp.regmap_verify --remote rocky func_80105EA8 'a0=arg0:ptr'
  --static-only` -> `VERDICT: CONSISTENT: the claimed registers are exactly the live-in set (static evidence only).`
  (live-in = a0 only; 66,916 dynamic calls, all through jalr, a0 a pointer 0x80146510..0x801478D0). The frontier fact
  `register_params t9` is REFUTED (with `:int` the run says `REFUTED ... a0 claimed int, seen ptr`); the real
  signature takes a pointer (`s32 func_80105EA8(void *arg0)` style; the draft's `s32 arg0` is behaviourally equal).
  For w32n/facts: drop t9.
- pretrace (snapshot ea0): `playerIdx*16` is bit 130 `mpy.J(varM(928-4)r,16)`, `antloc 45 | DELETE 45 | INSERT 44`:
  uopt ends a block at each call, block 44 = the font-height call (object_bytes_sum_global), 45 = the pane arguments +
  ambient_sound_set, 46 = tournament_unlock_check. PRE moves p*16 into block 44 (before the call) together with
  `&D_80116148[p]` (bit 132) and `&D_80116148[p].y` (bit 133); retail has the last two there too (s0, spill 92(sp)) but
  computes p*16 after the call in t6. The p*16 web then gets s3 (tot 20.0 vs callee cost 19.25, a near tie) and pushes
  `x` (w97, tot 40 < best 120) out of the s-registers.
- Oracle: force.sh `p1:w130=s` (split p*16) -> 358 -> **228 rows**; p*16 is then spilled at 116(sp) and still computed
  before the call, so retail's p*16 is not a moved web at all: the remaining lever is a source form in which p*16 is
  not anticipated at the end of block 44 (or the expression differs). Tried by w31a: `<< 4`, assignment in the argument
  list, height over two statements, pane block as a helper.

## 4. func_80100E58 (drawRaceStandings)
- Rebased: the w30a body inserted into the CURRENT `src/blob/groups/frontier_results_table/results_table.c`
  (`func_80100E58/best.c`, unit flags `func_80100E58/uargs.txt` = w30a's): same `534 of 683 words differ; compiled
  body is 682 words` (words 180 ops 9, frame 528/528) and `locked bodies ... 0`. Without `--internal func_80100E58` it
  compiles kept (s-saves, 255 rows); with --internal but without the w30a list umerge inlines it into drawMain.
- Oracle rebuilt (`watchman2:.../w32a/bin/uopt_acc` from w16d patch_spl.py + w31a patch_acc.py; driver
  `tools/acc.sh`): `SPL_ACC=45,112 SPL_SYM=17` -> 31 rows (reproduced). SPL log: piece lr 299 (columns, start bb39) at
  bb45 `regsleft=12 old=19 shared=4 numintf=23` -> reject (needs 2*regsleft >= numintf+shared: 24 < 27); bb45's busy
  set f0=1c017e00 = a0-a2, s1, s3-s8.
- Row-head variants (one each): `total` folded into the call (`b1.c` 201 rows), header font site without its
  `if (previous) {}` holder (`b2.c` 210), `x` before `y` (`b3.c` 180): none helps. Next: list the 23 interferers of
  piece 299 (extend patch_spl's adj print with the interference set) and compare with retail's register map at the
  row head (retail keeps row/rows/columns in memory: 460/468/464(sp)).

## 5. func_80103D28 (DrawUnlocked) — not advanced
No lane-hour left after levers 1, 3 and 4. Checked: retail's font site saves the old font (`move s0,v0`) before the
unlock argument set-up while ours fills the osSendMesg delay slot with it; replacing du_font_set's unlock helper by a
direct osSendMesg loses the 8 B area (`u1.c`, frame 256/264); a void SetFont wrapper is not inlined there (`u2.c`).
Residual unchanged: retail keeps &D_801426A0 (tracks "before") in s8 for the whole function, rematerialises the cars
"before" base as %hi/%lo folded into the load with the index added first (`base + (p*13 + i)`), and hoists the others
per loop; ours hoists every base into v0 per loop. Next: ctrace the constant-address webs (type 2) and compare their
(save, tot, best) with the s8 / folded outcomes.

## Group packaging (not delivered — 5480 open)
When func_80105480 matches: package func_80105480 (keep) + func_80104704 + func_80104B14 (+ func_80103D28 when it
matches; until then it must stay asm and the group must declare it extern — check that umerge still keeps it out of
line) from `hs/best.c`. The helpers du_gfx_lock/du_gfx_unlock/du_font_set/hs_*/gh_*/DrawCircuitCode/DrawScores are
non-static, internal, not on keep (force_internal entries like the w29b/w30b helpers). Group choice: the locked
highscorescr.c functions live in `frontier_billboard_render` (PrintScores, SetHighScoreState, FindGhosts,
track_texture_load = _InitScreen) but none of them calls the animText unit (it is reached through the screen's function
pointer), so a NEW group `highscore_anim` is the simpler packet; context `ctx_gr3_b.c` (slot_state_setup's s2 IPA
parameter, byte-identical copy as in frontier_results_table). Run `score.py group --claims` on the builder before
claiming.

## Proposed names / GC-derived facts (for names.md)
Unchanged from w31a (func_80104704 DrawName, func_80104B14 DrawGhost, func_80103D28 DrawUnlocked, func_80105480
highscore animText, func_80105EA8 animResults). New: race_position_update = the N64 ellipsis-text builder used by GC
TB_DrawEllipseText@TB_text.c (DrawScores' name column: `TB_DrawEllipseText(69, y, 160, name)` in GC DrawScores);
GC DrawScores locals are `i, numWid, str[16]` (functions.json) — the w30a `str[96]` was sized from the frame only.
func_80105EA8 takes a pointer in a0 (screen object 0x80146510..0x801478D0; regmap_verify), not t9.

## Integration notes
Nothing to integrate (no claimable member). Files: `hs/best.c` (= `func_80105480|func_80104704|func_80104B14|
func_80103D28/best.c`, unit flags `hs/uargs_hs.txt`), `func_80105EA8/best.c` + `dc.args`, `func_80100E58/best.c` +
`uargs.txt`, tools `tools/acc.sh` (split oracle driver), `tools/anchors.sh` (animText frame anchors). Shaping devices in
`hs/best.c` (header): non-static inlined helpers, void SetFont wrappers, hs_SetJust, the unused `s16 gFreePages`
(DrawGhost, exact frame residual 568 -> 576), and NEW the compiled-out `do {} while (0);` in DrawGhost's status-1/2 arm.
No permission denials.

## What generalises
- **A web that cannot take v0 although no interferer holds it** (force declined with forbidden = v0 only, no p1cost
  line for colour 1): one of its occurrences shares a block with a call that precedes it — typically a PRE reload
  inserted after a call. A compiled-out block end (`do {} while (0);`, `if (x) {}`) between the call and the reload
  gives it back v0.
- **uopt ends a block at every call; PRE moves single-occurrence argument expressions up into the call's block**
  (`DELETE 45 | INSERT 44`), where they live across the call (callee-saved) — the "computed before the call" shape.
- **umerge always charges >= 8 B per inlined call** in a function with memory locals (static, `__inline`, register
  params, cross-file all equal; no sharing between sites). A frame that cannot afford its helper sites means a different
  helper boundary, not a different helper spelling.
