# w31a — results / high-score + audio/controls screen units (wave 31), IDO 5.3 -O3

Status: FINAL (wave 31). Unit tag `w31a` (sub-forks used `w31ag`, `w31ae`); builder scratch
`watchman2:~/rush2049/scratch/frontier/w31a` (and `w31ag`) deleted at the end. No match this wave; no group packet
(see "Packaging decision"). Everything below is EQUIVALENT-or-better drafts with traced residuals.

| function | bytes | state | before -> after (unit rows / words) | draft_check | residual class |
|---|---|---|---|---|---|
| game_results_render (= GC animText@audioscr.c) | 1,164 | draft `game_results_render/best.c` (= `audio/best.c`) | 114 rows -> **14 rows** (`60 of 291 words differ; compiled body is 292 words`; 6 of the rows are unverified own-.rodata string relocs), frame 288/288 | **EQUIVALENT** 1,273 calls / 3 scripts | one extra `move v1,v0`: `cvt.J(*charPtr)` gets its own PRE web (bit 140) beside the loop-carried load (bit 139) |
| game_results_update (= GC animButtonText@audioscr.c) | 1,172 | same file | 112 rows -> **23 rows** (`56 of 293 words differ`, 14 of them unverified .rodata rows), frame 240/240 | covered by the render run (internal callee) | same cvt-web `move` + as1 places the first SetJust stores after the osRecvMesg arg moves |
| func_800BEA3C (TB_FlashTextColors) | 28 | locked single; GC union body EQUAL in the unit (`EQUAL func_800BEA3C: 7 words (kept, c_a3.c)`) | — | — | packaging only (not done: no new member matches) |
| func_80100564 / game_results_exit (controls) | 1,576 / 1,916 | unchanged w30a draft `cloud/work/frontier/w30a/controls/best.c` | 378 / 234 rows (not re-worked) | EQUIVALENT (w30a) | p in memory + spilled outer-loop IVs (register pressure), exit call convention |
| func_80104704 (= GC DrawName(p)) | 1,040 | **EQUAL in the unit** (provisional: caller unmatched), `func_80104704/best.c` (= `hs/best.c`) | new draft -> `EQUAL func_80104704: 260 words (internal, c_best.c)` | EQUIVALENT (unit run below, exercised inside 5480) | none; device: void `hs_SetFont` wrapper (+8 B) |
| func_80104B14 (= GC DrawGhost) | 2,404 | draft, same file | new -> `17 of 601 words differ` (6 unverified own-.rodata rows), ops 0, frame 576/576 | EQUIVALENT (unit) | colour only (gGhostStatus[pak] web v1 vs v0; force declined by a precoloured interferer) |
| func_80105480 (= GC animText@highscorescr.c) | 1,780 | draft, same file | 79 rows (440/445) -> `27 of 445 words differ` (33 rows, 15 with `--nosp`), body length exact, $f20/$f22 saves now present | **EQUIVALENT** 2,473 calls / 4 scripts (unit) | frame 704 vs 584: helper inline areas inside the inlined DrawScores/DrawCircuitCode |
| func_80103D28 (= GC DrawUnlocked) | 2,516 | draft, same file | 575/629 (614 words, frame 192/264, 75 ops) -> `588 of 629 words differ` (340 rows, 48 ops), body length and frame exact | EQUIVALENT (unit) | colour/split of the per-category row-pointer webs and constant-address webs (&D_801426A0 in s8 in retail) |
| func_80100E58 (= GC drawRaceStandings) | 2,732 | w30a draft unchanged (`func_80100E58/best.c`); force-accept oracle built | 180 rows; oracle `SPL_ACC=45,112` -> 31 rows | not run | split acceptance of `columns` at bb45 proven dominant (180 -> 31); source form not found; plus switch-arm-exit reloads + a byte-store swap |
| func_80105EA8 (= GC animResults@hud.c) | 2,508 | NEW complete draft `func_80105EA8/best.c` (= `f5ea8/best.c`, sub-fork) | not started -> `562 of 627 words differ` (358 rows, 41 ops), frame 400/400 | **EQUIVALENT** 9,241 calls / 3 scripts (`--params arg0=a0`) | callee-saved colouring (`playerIdx*16` computed before the font-height call takes an s-register, x is split) + SetJust store order |

## Packaging decision (lever 1)
The audio unit is the near one (render 14 rows = one instruction + unverified string relocs; update 23 rows), but no
screen function reaches MATCH, so a group packet would only move the locked func_800BEA3C (re-typed to GC's
`union {u8 c[4]; u32 rgba;}`, bytes identical) into a group with zero new coverage. Not delivered. When the audio
pair matches: group = `audio/best.c` with members game_results_render (keep), game_results_update, func_800BEA3C
(`--revert-single func_800BEA3C`, the pak-getter precedent), helpers au_gfx_lock/au_gfx_unlock/au_font_set/
au_CenterText/au_RightText/au_validOption/au_SetJust/au_SetFont as non-static internal (force_internal if the group
rule does not cover them); func_800BEA3C must stay KEPT (it has other callers) -- check `score.py group` keeps its
out-of-line copy. The controls unit would join the same group only if it shares func_800BEA3C's definition (it does:
both drafts define it); package both into ONE group then, as the brief says.

## Proposed names (GC-derived, for names.md)
game_results_render -> `audio_animText`, game_results_update -> `audio_animButtonText` (GC audioscr.c);
func_80100564 -> `controls_animText`, game_results_exit -> `controls_animButtonText` (GC controlscr.c);
func_80104704 -> `DrawName`, func_80104B14 -> `DrawGhost`, func_80103D28 -> `DrawUnlocked`, func_80105480 ->
`highscore_animText` (GC highscorescr.c); func_80105EA8 -> `animResults` (GC hud.c). Data: D_80117420 gPlayerNameChars,
D_80144010 gNameSelect, D_80143A50 gScoreName, D_801149A8/AC/B0 DrawName statics "DEL"/"END"/"SPC" (GC
del$1225/end$1226/spc$1227); DrawGhost and animResults data names in `hs/ghost/NOTES.md` and `f5ea8/NOTES.md`.

## Integration notes
Nothing to integrate (no MATCH). Files: `game_results_render/best.c`, `game_results_update/best.c` (same file),
`func_80105480|func_80104704|func_80104B14|func_80103D28/best.c` (same file = `hs/best.c`; unit flags
`hs/uargs_hs.txt`), `func_80105EA8/best.c` (+ `dc.args` = `--params arg0=a0`; the frontier fact `register_params t9`
for func_80105EA8 looks spurious -- retail homes a0 like the other screen functions; a w31r/regmap_verify item),
`func_80100E58/best.c` (w30a draft, unchanged), oracle tools `tools/patch_acc.py`, `tools/acc.sh`.
Shaping devices in the drafts (all disclosed in headers): non-static inlined helpers (au_*, hs_*, gh_*, ar_*), void
SetFont wrappers, SetJust helpers, one unused GC local `s16 gFreePages` (DrawGhost, exact frame residual 568 -> 576),
two variable reuses in animResults.

## What generalises
- **An inlined constant `validOption()` test turns off loop optimisation**: GC's `if (!validOption(i)) continue;`
  (inlined `return 1`) left retail's audio loop without LICM or strength reduction (menuRect.x reloaded each
  iteration, `i*4` recomputed); writing it closed 57 of 114 rows. Look for it wherever retail recomputes invariants.
- **Two stores in both arms** of a toupper-style update = `if (c in range) *p = *p ^ 0x20; else *p = *p;` (or a
  ternary store); a single conditional store gives one `sb`.
- **Missing frame below the locals = inlined text helpers**: DrawUnlocked's 72 B were 9 `hs_CenterText` areas (the
  s16 return is visible as `sra; move a0`); DrawName's last 8 B a void `HAL_SetFont` wrapper.
- **The SetJust-store schedule residual recurs** (audio update, animResults): with TB_SetJust inlined before a font
  site, as1 puts our stores after the osRecvMesg argument moves, retail before; Pair vs array typing and store order
  do not change it -- worth one as1t trace for all three.
- **uopt f_addadjacents**: the acceptance inequality applies only when the byte at 0x1001eb10 is set; force-accept
  oracle (`tools/patch_acc.py`) proves split-piece hypotheses before searching for source forms.
- Sub-forks with their own blob_unit tags parallelise fresh drafts well (DrawGhost, animResults each ~10 min to a
  complete EQUIVALENT draft).

## Progress log
- AUDIO unit (start: w30a `audio_options/best.c`, render 114 rows, update 112 rows). Current `audio/best.c`:
  render `60 of 291 words differ; compiled body is 292 words` = 14 rows (8 of them the 6 unverified own-.rodata
  string relocations + 1 extra instruction), frame 288/288; update `56 of 293 words differ` = 23 rows (14 unverified
  .rodata relocation rows), frame 240/240. Levers (all GC-derived):
  1. `if (!au_validOption(i)) continue;` (GC validOption@audioscr.c = `return 1`, inlined): retail's loop has NO LICM and
     NO strength reduction (menuRect.x reloaded from its home each iteration, `i*4` recomputed) -- the inlined constant
     test makes the loop body non-anticipatable for uopt's PRE (114 -> 57 rows).
  2. `strings[gOptions.musicType + indices->musicTypes]` operand order (GC order) (-> 35 rows).
  3. toupper loop written as two stores `if (c in a..z) *p = *p ^ 0x20; else *p = *p;` (retail stores in both arms).
  4. frame: a void `au_SetFont(font)` wrapper at the font-13 site (+8 B, GC HAL_SetFont) in both functions and
     `au_SetJust(h, v)` (GC TB_SetJust) inlined at the two update sites: both frames exact.
  5. update: `x -= 55; state_utility(x, …)` / `x -= 35` (retail adds into x's register).
  Residual (both): `cvt.J(ilod.L *charPtr)` gets its own PRE web (pretrace bit 140, v1) separate from the
  loop-carried load (bit 139, v0) -> one `move v1,v0`; retail has one register. Update also: as1 schedules the
  first SetJust stores after the osRecvMesg argument moves (retail before).
- func_80104704 (= GC DrawName(p)@highscorescr.c, 260 words, NEW draft from GC + N64 disasm, in `hs/v/best_h1.c` (superseded by `hs/best.c`)):
  unit (`blob_unit --tag w31a score func_80105480 func_80104704 func_80103D28 --with hs/v/h1.c $(cat w30a/uargs5480.txt)
  --internal func_80104704 --internal hs_SetFont`): `EQUAL func_80104704: 260 words (internal, c_h1.c)`. First form
  was 5 rows = frame 128/136 only; the void `hs_SetFont(12)` wrapper (GC HAL_SetFont) closed the 8 B. p arrives in s5
  as retail (IPA, from the C caller). GC names: gPlayerNameChars = D_80117420 (char *), gNameSelect = D_80144010 (s16[p]),
  gScoreName = D_80143A50 (char[p][13]), DrawName statics del$1225/end$1226/spc$1227 = D_801149A8 "DEL" / D_801149AC
  "END" / D_801149B0 "SPC", gPlayerRects D_801148E8 (y +2, cx +8). PROVISIONAL: its only caller func_80105480 is unmatched.
- func_80105480 with DrawName in C: still `438 of 445 words differ` (69 rows), frame 584/688; the $f20/$f22 saves
  wait for func_80104B14 (DrawGhost, draft in progress by a sub-fork under `hs/ghost/`). Writing DrawScores /
  DrawCircuitCode bodies into the switch arms (`hs/v/i1.c`) only moves the frame 688 -> 672.
- func_80103D28 (internal, real caller func_80105480 in the file): w30a `575 of 629 words; compiled body is 614 words |
  words 543 ops 75 | frame 264/192`. Lever: the 72 B frame gap is 9 inlined `hs_CenterText` areas (8 B each) -- retail
  narrows each centred x through the helper's s16 return (`sra; move a0`); writing the 9 sites as `hs_CenterText(s, x)`
  (`hs/v/d3.c`): `588 of 629 words differ | words 340 ops 48 | frame 264/264` (body length and frame exact). Residual =
  colouring of the constant-address webs: retail keeps &D_801426A0 (tracks "before") in s8 for the whole function,
  folds the cars/engines bases into the load (%hi/%lo, not coloured) and hoists the others into v0 per loop; ours puts
  every base in v0 per loop, so p lands in s6 (retail s5) and str in s5 (retail s6).
- func_80105480 frame (584/688) tests: (a) DrawScores/DrawCircuitCode bodies written into the switch arms with
  block-scoped GC locals (`hs/v/i1.c`): frame 672 and every local block lands at retail's top-relative offset (+-4) --
  the whole excess is helper inline areas at the bottom (92 B vs retail 12 B); (b) helper definitions moved after
  their users / to the end of the file (`hs/v/j1.c`, `j2.c`): no change; (c) `register` params on the helpers: no change
  (`register s32 old` stops umerge inlining font_set). Retail evidently inlines font/centre/right helpers into the
  inlined DrawScores/DrawCircuitCode with NO areas while DrawName/DrawUnlocked (out-of-line) keep 8 B per site --
  open: which inline order/shape gives area-free nested inlining (GC: the helpers live in TB_text.c / hal.c, i.e.
  another source file; a separate `--with` helper file was not linked by blob_unit -- untested).
- func_80100E58 force-accept oracle (lever (4)): `tools/patch_acc.py` (on top of w16d patch_spl.py) adds `SPL_ACC=bb,..`
  (+`SPL_SYM`) to uopt f_addadjacents (jumps to the add path L46e34c before the acceptance test); built as
  `watchman2:~/rush2049/scratch/frontier/w31a/bin/uopt_acc`, driver `tools/acc.sh LABEL PROC "ENV"`. Fidelity: no env
  -> 180 rows (= stock). Note from the recompiled source: the inequality is only applied when the byte at 0x1001eb10
  is set (otherwise only `regsleft == 0` rejects). Results on the w30a best (`func_80100E58/best.c`, unchanged copy) for `columns` (sym 17, piece
  lr 299 from bb39): `SPL_ACC=45` -> 40 rows; `SPL_ACC=45,112` -> **31 rows** (678/683 words); accepting every block
  40..125 -> 35 rows. So the bb45 split is the dominant lever (180 -> 31) but not the whole residual: retail also
  reloads `columns` (`lw v0,464(sp)` in the branch delay slots) at every switch-arm exit into the join block, and
  two byte stores (444/445) are swapped. Source form not found (no time left for the 3-interferer search).
- AUDIO draft_check (`audio/best.c` (was a3.c)): `python3 -m tools.recomp.draft_check --remote rocky game_results_render
  cloud/work/frontier/w31a/audio/best.c --scripts audio_options,options_menus,pause_menus` ->
  `VERDICT: EQUIVALENT over 1273 calls / 3 scripts (audio_options 434, options_menus 540, pause_menus 299)`.
  Alternative frame source tested: the toupper loop as an inlined `au_strupr(str)` helper with no SetFont wrapper
  (`audio/v/s1.c`) gives the identical 14 rows -- the 8 B are one inlined area either way; the cvt-web `move` stays.
- CONTROLS unit: not advanced (time went to the high-score unit; 378/234 rows need the outer-loop IV/spill structure:
  retail keeps p in memory and spills &buttonsInited[p] / p*4 IVs at 148/152(sp) -- register pressure from s0..s8 all
  in use). Draft unchanged: `cloud/work/frontier/w30a/controls/best.c` (EQUIVALENT per w30a).
- HIGH-SCORE unit draft_check (`hs/best.c`): `python3 -m tools.recomp.draft_check --remote rocky --unit
  func_80105480,func_80104704,func_80104B14,func_80103D28 cloud/work/frontier/w31a/hs/best.c --params func_80104704:p=s5
  --scripts ghost_pak,unlock,player_finish,race_finish` -> `VERDICT: EQUIVALENT over 2473 calls / 4 scripts (ghost_pak
  1144, unlock 308, player_finish 863, race_finish 158)`; the three internal members are exercised only inside
  func_80105480's compared calls (draft sha256 fc5e7b9eccc5d875).
- func_80104B14 (DrawGhost) drafted by a sub-fork (`hs/ghost/NOTES.md`): 17 of 601 words (6 unverified own-.rodata
  rows), ops 0, frame 576/576; colour only (gGhostStatus[pak] PRE web v1 vs retail v0; force declined by a precoloured
  interferer). With it in C, func_80105480 gains retail's $f20/$f22 saves and its exact body length (27 of 445 words,
  33 rows, of which only 15 survive `--nosp`): the remaining 5480 residual is the frame (704 vs 584).
- func_80105480 frame measurements (`hs/v/n1.c`, `n2.c`): text helpers (CenterText/RightText) cost 8 B per inlined
  site, the nested font helper 16 B per site; with every DrawScores/DrawCircuitCode helper written as a macro the
  frame is 560 (24 B UNDER retail) -> retail spends exactly 24 B (three 8-B areas) on the 12 helper sites inside the
  two inlined screens. Which three is open.
- func_80105480 "which three 8-B areas" sweep (`hs/v/c00..c38.c`, `hs/v/combos_scores.txt`): every 3-text-site and
  1-font+1-text-site helper/macro mix -> frame 592/600 and 246+ rows (vs 33 with all helpers): the macro hypothesis is
  wrong (written-out sites change the code far more than the frame). Open: area-free nested inlining.
- func_80105EA8 draft_check: `python3 -m tools.recomp.draft_check --remote rocky func_80105EA8
  cloud/work/frontier/w31a/f5ea8/best.c --params arg0=a0 --scripts race_finish,player_finish,two_player_finish` ->
  `VERDICT: EQUIVALENT over 9241 calls / 3 scripts (race_finish 3218, player_finish 2633, two_player_finish 3390)`.
  Without `--params`: `ERROR: func_80105EA8 takes register parameters (t9, build/frontier.json facts)`.
- Builder scratch `w31a` and `w31ag` deleted.
