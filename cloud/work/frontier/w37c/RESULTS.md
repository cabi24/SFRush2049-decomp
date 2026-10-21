# w37c RESULTS (wave 37, game flow; SHORT wave)

Builder scratch: watchman2:~/rush2049/scratch/frontier/w37c (from base, master c12837976). Trace toolkit installed
(reused wtk). All unit runs: `TAG=w37c cloud/work/frontier/tools/trace/us.sh NAME FILE` (blob_unit --tag w37c).

## Status

| function | GC identity | bytes | state | best file |
|---|---|---|---|---|
| RaceStateMachine_Update | updatePauseScreen@pausescr.c (w36p confirmed) | 2148 | 13 words short (unchanged; re-scored in current unit, voice_stop landed) | w36p/RaceStateMachine_Update/best.c (= w37c/RaceStateMachine_Update/v/r0.c) |
| ghost_race_setup | GhostEnd@ghost.c (w36w2 confirmed) | 440 | 3 words (unchanged) | w36w2/ghost_race_setup/best.c |
| display_list_flush | PrePlay@game.c (w36p confirmed) | 2356 | 55 op rows (unchanged); frame mechanism traced (below) | w36p/display_list_flush/best.c |
| playgame_state_change | updateGameState+initGameState@state.c | 2544 | not re-attempted (w36p: ops 2 with blockers; umerge budget) | w36p/playgame_state_change/best.c |
| explosion_effect | animSpeedText@hud.c (w36o confirmed) | 1460 | not attempted (w36x3: 37 rows) | w36x3/explosion_effect/best.c |
| physics_float_calc | WorldCullObjects@world.c (w36u confirmed) | 1176 | 39 rows (unchanged); frame/home layout decomposed (below) | w36x3/physics_float_calc/best.c |

## Notes

### RaceStateMachine_Update (updatePauseScreen) -- 13 words short, not closed
Re-score of w36p best.c in the current unit (voice_stop now landed in frontier_pausescr_state):
`r0.c: FAIL RaceStateMachine_Update: 498 of 537 words differ; compiled body is 524 words, target 537 | words 47 ops 26 norm 26 | frame 272/272`.
Traced (ctrace on snapshot r0): the constant-2 web is `p1 w177 save=2.25 nocs=4 tot=9 best=4.0 color -> t0` (PauseTargets
+ end chain only). Retail's li placement says its t0 range is the end block plus ALL its immediate predecessors (dispatch
block, every case's last block, the case-1 inner-switch join, the --sel/++sel joins with the call block before them),
but NOT case 1's test blocks, the inner-switch case blocks, case 6's test/call blocks or the case 7/8 interior:
the li sits after the last call of each predecessor (or at the start of a call-free predecessor such as 800dbd28).
Experiments (v/):
- a1/a2/a3 (`if (D_801170FC == 2) {}` / `2 == ...` before the switch, either side of SaveServer): the constant's
  range grows to the whole switch (`p1 w168 nocs=32 tot=11 best=40.25 split`, t0-t5 not available), both split pieces
  get negative save (`save=-0.46 nocs=13`, `save=-0.20 nocs=20`) -> no colour, 216 words / ops 28.
- force oracle on a1 (`force.sh a1 RaceStateMachine_Update p1:w168=c7`): `want 537 words, got 536; differing rows 63`
  -- retail-like li's at every case exit but ALSO in the inner-switch cases (and `move a0,t0` for setPauseState(2)):
  retail's range is narrower than "whole switch", so a holder at the top is not the source form.
- h1 (holder right after the switch): no change (47); h2 (inside `if (D_801170FC)`): 85; h3 (top + after): 216;
  p1-p4 (holder at case 0 head / pause_game head / before / after PauseTargets): 52 / 91 / 49 / 47;
  q1-q4 (`== 2` / `!= 2` holders on gControllerRemoved / playerInPause, after the switch or at the top): 58 / 66 / 216 / 58.
Conclusion: the missing piece is a range shape "end block + one level of predecessors"; none of 17 holder placements
produce it. Next hypothesis: an occurrence of 2 at the end of each case (e.g. the N64 end-of-case statement being a
macro/inline that compares or stores 2 and compiles out), or an oracle with w36z/tools/patch_acc.py (SPL_ACC) to test
whether a split piece with exactly those blocks reproduces retail before searching the source.

### ghost_race_setup (GhostEnd) -- 3 words, not closed
Re-score: `g0.c: FAIL ghost_race_setup: 3 of 110 words differ | words 3 ops 2 norm 2 | frame 64/64` (w36w2 best.c).
Tried address-only holders to make uopt place `&gGameCar[i]` in the main block before the header load (the w36w2 listing
oracle o7.s): k1 `if (&D_80152818[i]) {}` before `g = ...` 26 words, k2 after 26, k3 `&D_80152818[i].ready` 19,
k4 `D_80152818 + i` 36 (all renumber the base-address webs; the address computation stays in the else arm).

### display_list_flush (PrePlay) -- frame mechanism traced, not closed
Re-score (w36p best.c, `UARGS="--internal InitBattlePoints"`): `d0.c: FAIL display_list_flush: 499 of 589 words differ ...
| words 395 ops 55 norm 55 | frame 256/88` (same with or without --internal AudLock/AudUnlock/AudSend and the countdown
group files, which are installed now).
- Merged-ucode Mmt dump (w33a/tools/mmt.py on unit/w37c/stage/merged): our merged display_list_flush has `Udef Mmt 0 90`
  = 144 B of local area (i at -4 with 56 refs, inline areas down to -140), yet the frame is 88: ugen trims the local
  area to the deepest slot still USED after colouring (only i's home fp-4 = sp+84 survives here).
- Retail's frame 256 = i's home at fp-4 (sp+252, `sw zero,252(sp)` / `lw s1,252(sp)` around the internal
  init_state_continue, same as ours at 84) plus two SPILL TEMPS deep below the Mmt area: sp+92 (= fp-164, InitStunt
  loop pointer) and sp+84 (= fp-172, gGameCar pointer). So the 168-B frame gap is entirely a consequence of retail
  spilling those two webs (w36p: our callee-save cost lets them have s-registers); with the spills, our homes would sit
  just below a 144-B Mmt area, i.e. ~16 B (two inline slots) higher than retail's -- so the Mmt area is also ~16 B short.
- `--keep` on every inlinee (func_800FAF50 x2, AudKillSong, AudStopAllEffects, player_flag_clear_process,
  InitBattlePoints, effect_system_init, func_800A14FC, AudLock/AudUnlock/AudSend): umerge still inlines all of them,
  `words 393 ops 53 norm 53 | frame 256/88` (2 rows better, frame unchanged). ZOID_Reset non-static (d1): no longer
  inlined (budget), body 569 words, ops 78.

### physics_float_calc (WorldCullObjects) -- 39 rows, not closed; frame layout decomposed
Re-score: `f0.c: FAIL physics_float_calc: 52 of 294 words differ | words 39 ops 10 norm 10 | frame 136/136` (w36x3 best.c).
Retail homes (fp = sp+136): dlindex fp-4, windex fp-8, dist fp-12, best fp-16, ptr fp-36, WorldDistance's dv[0..2]
fp-76/-72/-68 (sp 60/64/68), nothing referenced below sp+60.
- The `(ptr_no_clip ? mask & ptr_no_clip[w] : 0)` clause's cfe temp is a 4-B slot in the CALLER's Mmt area, so umerge's
  inline base for WorldDistance (align8(caller locals + parm area), w33a Fact A) moves 8 B deeper: that is exactly
  w36x3's "dv homes 52/56/60 vs 60/64/68".
- f1 (the `ptr_no_clip && (...)` spelling, no temp): every home is fp-exact (dv fp-76..-68) but the frame is 128:
  `f1.c: ... compiled body is 293 words ... | words 271 ops 10 norm 10 | frame 136/128` -> retail has 8 more bytes at the
  BOTTOM of the local area (below the inlined callee), not above it.
- f7 (oracle: f1 + an unused third float local `e` at the end of WorldDistance's locals): frame and every home exact,
  `f7.c: FAIL physics_float_calc: 164 of 294 words differ; compiled body is 293 words, target 294 | words 236 ops 10 norm 10 |
  frame 136/136`; mask/i now v0/v1 as retail. Remaining: every allocated register is ONE LOWER than retail from the
  inlined callee on (callee ring `lh t4` vs retail `lh t5`; parts t0/ptr t1/ptr copy a3/i*68 IV a1 vs retail
  t1/t2/t0/a3) and 1 word short. So retail = "no caller-area temp + 8 B at the callee bottom + one more web live
  through the function". A ?: (or other cfe temp) inside WorldDistance would supply both the bottom slot and the
  extra web; not found yet. f10 (f0 + e): frame 144, 69 words; f3/f11 (clause removed): body 291, 200 words.
- Static ShowCull helper (f4-f6, empty/K&R/holder body): umerge then stops inlining WorldDistance (frame 80, ops 28).

### playgame_state_change, explosion_effect -- not attempted (short-wave time box spent on the four above)

## What generalises
- **ugen sizes the frame from the deepest slot still used after colouring, not from umerge's merged Mmt size.**
  display_list_flush's merged Mmt is 144 B (`Udef Mmt 0 90`) but the frame is 88 because only i's home (fp-4) is used;
  retail's 256 comes from two spill temps placed BELOW the whole Mmt area. A large retail frame with only a few deep
  homes = spills (a colouring difference), not missing locals; `--keep` on inlinees does not change it.
- **A `?:` in value context costs a 4-B cfe temp in the caller's Mmt area, which moves every umerge-inlined callee's
  locals 8 B deeper** (align8 base). Spelling it with `&&` removes the temp (physics_float_calc: all homes fp-exact).
- **Holder placement for a constant's range**: a compiled-out `if (x == K) {}` before a switch extends the constant's
  live range to the whole switch (nocs 4 -> 32) and makes it split; one after the switch adds nothing. When retail's
  constant reloads sit exactly at the end of every predecessor of one block, the range is "that block + one level of
  predecessors", which no single holder produces (RaceStateMachine_Update, 17 placements).
- Read frames fp-relatively when comparing homes (`want - got` frame offsets shift every sp offset).

## Identities proven
(none new)

## Research receipts
No deliverable this wave; nothing touches research-packet tests.

## Permission denials
none
