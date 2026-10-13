# w28a — results / high-score / records screens (wave 28), IDO 5.3 -O3

Status: final (wave 28). Builder scratch `watchman2:~/rush2049/scratch/frontier/w28a`; unit tag `w28a`.

| function | bytes | state | score before → after | draft_check | residual | GC evidence |
|---|---|---|---|---|---|---|
| func_800F0914 | 1,584 | **MATCH in group** `frontier_billboard_render` (superseding) + EQUAL in unit | context copy 288/396 → `MATCH` | — (match) | none | `_PrintScores` (highscorescr.c, confident) |
| func_800F1210 | 1,816 | **MATCH in group** `frontier_billboard_render` (superseding) + EQUAL in unit | context copy 398/454 → `MATCH` | — (match) | none | `_SetHighScoreState` (highscorescr.c, confident) |
| func_800F0F44 (extra, not in pool) | 464 | draft `func_800F0F44/best.c` | 68/116 → 8/116 (9 rows, frame 72/72) | not run (no script reaches it) | layout/colour tie (1 vs 2 constants) | `_FindGhosts` (highscorescr.c) |
| func_80102F30 | 3,560 | complete draft `fam/fam_best.c` (= `func_80102F30/best.c`) | not started → 140/890 words (145 rows, ops 43), frame 640/672 | **EQUIVALENT** 1,350 calls (race_finish 750, two_player_finish 240, player_finish 360) | frame (inline-area accounting) + colour | weak (`drawRaceScore`/`drawRaceStandings` 0.9); layout from sibling 2448 |
| func_8010221C | 548 | draft `fam/fam_best.c` | 96/137 → 11/137 (13 rows, ops 0), frame 24/24 | **EQUIVALENT** 1,180 calls (race_finish, two_player_finish, player_finish, stunt_short) | colour-only (4-web tie) | none |
| func_80100D5C / func_80101904 / func_80101D84 | 252 / 1,152 / 1,176 | EQUAL in unit, now with the REAL caller chain (F30 → 221C → 1904/D84/E58) and no stand-in | unchanged (EQUAL) | (w27c: D84 EQUIVALENT) | lands when F30 + 221C match | — |
| func_80102448 / func_80102980 / func_80100E58 | | unchanged w27c drafts (18 / 325 / 633 words) | — | — | — | — |
| func_80103D28, func_80105EA8, func_80104B14, func_80105480, func_80100564, game_results_update/render/exit | | not started (time); survey below | — | — | — | — |

## Group packet: `groups/frontier_billboard_render/` (supersedes the locked group of the same name)

Members: billboard_render, func_800F207C, func_800F1930 (old) + **func_800F0914, func_800F1210** (moved from
context). Claims: func_800F0914, func_800F1210. Context: func_800F0F44, func_800F2718 (old) + byte-identical
copies `ctx_gr3_b.c` (= locked `credits_scroll_grp/gr3_b.c`: slot_state_setup, object_create, func_800B669C) and
`ctx_func_800D50E4.c` (= locked single `src/blob/func_800D50E4.c`, rand_r). Keep unchanged (billboard_render).

Why the context copies: the group compile otherwise lacks the unit's IPA facts — slot_state_setup's `$s2`
register parameter, object_create inlined by umerge into func_800F0914 (its inlined return gives retail's
`move s0,v0`), and rand_r (func_800D50E4) inlined three times into func_800F0914. Without them func_800F0914 is
340/396 in the group (frame 96: real calls to rand_r force callee saves).

Builder (`python3 tools/cloud/score.py group cand/frontier_billboard_render`, IDO 5.3):
```
billboard_render:   MATCH  (own .rodata verified at 0x80124604..0x80124618)
func_800F207C:      MATCH
func_800F1930:      MATCH
func_800F0914:      MATCH  (own .rodata verified at 0x801245B4..0x801245B8)
func_800F1210:      MATCH  (own .rodata verified at 0x801245B8..0x801245E4)
Context: func_800F0F44 68/116 words differ; func_800F2718 MATCH; slot_state_setup 2/58; object_create 28/28;
func_800B669C 5/5; func_800D50E4 10/10 (context copies are inlined/deleted, informational)
```
`--claims`: func_800F0914 MATCH, func_800F1210 MATCH, rc=0.

Pi unit (`G=cloud/work/frontier/w28a/groups/frontier_billboard_render; W=""; for f in $G/*.c; do W="$W --with $f"; done;
python3 -m tools.conveyor.pipeline.blob_unit --tag w28a score func_800F1210 func_800F0914 billboard_render func_800F207C
func_800F1930 func_800F2718 slot_state_setup $W --internal hs_font_set --internal hs_gfx_lock --internal hs_gfx_unlock --neighbours`):
```
  EQUAL func_800F1210: 454 words (internal, c_func_800F1210.c)
  EQUAL func_800F0914: 396 words (internal, c_func_800F0914.c)
  EQUAL billboard_render: 244 words (kept, c_billboard_render.c)
  EQUAL func_800F207C: 421 words (internal, c_func_800F207C.c)
  EQUAL func_800F1930: 245 words (internal, c_func_800F1930.c)
  EQUAL func_800F2718: 92 words (internal, c_func_800F2718.c)
  EQUAL slot_state_setup: 58 words (internal, c_ctx_gr3_b.c)
  locked bodies that differ in this unit: 0
blob_unit score: 7/7 equal
```

Integration notes:
- `PYTHONPATH=. python3 cloud/work/frontier/tools/install_group.py cloud/work/frontier/w28a/groups/frontier_billboard_render frontier_billboard_render`
  (same name supersedes; every old member kept). No `--revert-single` (neither function was locked).
- unit_overrides: keep the existing `prefer_definition` func_800F1210 → `src/blob/groups/frontier_billboard_render/func_800F1210.c`
  (func_800F2718_grp still defines a partial F1210 as context). Add `force_internal` for hs_font_set, hs_gfx_lock,
  hs_gfx_unlock if the locked-group rule does not already make them internal (they are not on keep; the unit run
  above passed them with `--internal`). The ctx copies duplicate locked definitions byte for byte
  (slot_state_setup/object_create/func_800B669C in credits_scroll_grp, func_800D50E4 single): check `blob_unit check`
  picks either copy cleanly; if it complains, add `prefer_definition` entries pointing at the original files.
- provisional.json: nothing (neither function was listed).
- No data-symbol changes; func_800F0914 declares `extern void *D_80149778` (title pane, GC namePanes+0x1b0) and
  the strings D_8012060C/D_8012061C.

## func_800F0914 = GC `_PrintScores` (no devices)
GC evidence: `reference/gc/disasm/highscorescr.c/_PrintScores.s`, functions.json locals (i, s = float*, p = player***,
defTime f31, seed on stack, width), types.h `__anon_2dfd` (race stats 0x60: checksum, scores[3][5] at +4) and
`__anon_3bdf` (player record, name[13] at +0x14). Names used (GC → N64): gBackwards D_80152570, gTrackNum D_8014978C,
gTable D_801148D0, gThisTrack D_80142528, gTotalNames D_80151690[track][3][5], gTotalRaceStats D_80150F88[12],
gDefStartTimes D_8002E870, gDefaultNames D_801147C8 (55 on N64, 58 on GC), gTempNames D_80151968[20][13], name
strings D_80143F20[5][10], name pointers D_80143FE0[5], gText = countdown_state (strings[indices->h54 + gTable]),
title pane D_80149778 (GC namePanes+0x1b0), rand_r = func_800D50E4 (locked, inlined), HAL_SetFont = object_create
(inlined), HAL_GetTextWidth = object_manager_update, TB_GetFontLineHeight = object_bytes_sum_global,
ChangePaneSize = crowd_cheer_play.
Levers found: the else arm converts `(u32)defTime` twice in the sprintf args and stores `seed = defTime` before
(the GC form `seed = defTime; sprintf(seed/60, seed%60)` was 83 rows); `s++` before `p++`; width kept un-halved
and halved at the call (as retail's `move s0,v0`); temp-name test `(u32)*p <= 20` with `(s32)*p` index.

## func_800F1210 = GC `_SetHighScoreState(newState)` (no devices)
GC evidence: `_SetHighScoreState.s` (two switches, RemovePane/InitPane, gPlayerRects, SetCountdownTimer 600/30/15),
functions.json (newState param; block locals p,p,i,i,p,p,pak,i,width×3). GC → N64 names: gHighScoreState
D_80140B20, blit D_801148CC, RemoveBlit sound_stop, NewMultiBlit sound_control (mblit D_801148A8), namePanes
D_80149418[], codePane D_80146208, ghost panes D_80149798[3], score panes D_80149778[2] (GC has 3), gNumPlayers
active_player_count, gNumWindows D_80151AD0, gPlayerRects D_801148E8, gRace D_8014A110, gGhostMenuSelect
D_80144DA0 (N64 sets 0, GC 2), gGhostPak D_801460C0, _UninitScreen func_800F1114, _FindGhosts func_800F0F44,
SetCountdownTimer = **viScheduleTick** (label wrong), InitPane = ambient_sound_set, RemovePane =
sound_handles_array_clear. N64 lacks GC's PS_LockFile loop and the third score pane.
Levers found (traced): (1) the second switch is `switch (D_80140B20)` read back after the store — uopt then keeps
&D_80140B20 in s3 and PRE-reloads the global per case for the final `if (D_80140B20 != 0)` (54 → 4 rows);
(2) umerge does not inline a one-piece font wrapper `{osRecvMesg; slot_state_setup; osSendMesg}` with 4 call
sites (nor object_create with 5 unit-wide sites), but does inline the split form font_set{gfx_lock(); ...;
gfx_unlock();} — the gr3_d.c shape; (3) frame: block-scoped `pak` plus one function-level `p` for the cleanup
loops puts the s8 spill at sp+44 with frame 72.

## func_800F0F44 = GC `_FindGhosts(pak)` (464 B; outside the pool, context of the same group) — near-miss
Draft `func_800F0F44/best.c` (from the GC structure + w15f/ipa-groups draft). Unit, with the group files and
`--internal pak_slot_b1 --internal pak_slot_b5 --internal pak_slot_b6`: from 68/116 (context copy) to
`FAIL func_800F0F44: 8 of 116 words differ | words 9 ops 5 | frame 72/72`.
GC → N64: gPakGhosts D_80146198[pak] (`ghosth`, spilled at sp+24 as retail), gGhostStatus D_801461C0, gGhostPlayer
*D_8014A160 (`player`, s2), gGhostFiles list D_80152028, ghost record: next +0, q +4 (`(*q)->id` +0x10, name +0x12),
track +8, backwards +9, pos +10, id0 +24, id1 +28; player: qq +8, id +12/+16, pos +20. strcmp(…,14) = func_800950AC.
The N64 status checks differ from GC: -4 controller-pak absent (D_80156CF0[pak].b0), -1 slot byte 1 == 0, -2 byte 5,
-3 byte 6 (D_80144030[pak], 772 B), then 0; ghost-race only (gRace == 2) walk; 1 = same ghost, 2 = different.
Levers found: the three slot-byte tests are **inlined helpers taking pak** (pak_slot_b1/b5/b6, non-static
internal): they give exactly retail's 72-byte frame (24 B of inline areas) and the `v0` status-byte copies;
signed `== 2` pools the constant-2 web with the stores (s7) and puts pak in s8 (`2U` gives s7/pak wrong).
Residual (traced with ctrace): webs 2 (s7) and 1 (s6) tie at save 2.0 with the two address webs (s4/s5); the
lower web number wins, and the number follows the order of the `= 1`/`= 2` stores in the loop. Source with `= 1`
first (best.c) has retail's colours but the then-arm block first (9 rows, layout only); source with `= 2` first
(`if (fail) 2 else 1`, v/e.c) has retail's block layout but 2 in s4 (12 rows, colour only). Next lever: a form
whose u-code puts the 2-store block first while the 1 constant is numbered first (e.g. a compiled-out use of 1
before the loop, or the force oracle to confirm then a prio_patch run).

## Results-table family (fam/fam_best.c; w27c fam_best.c + this lane's changes)
Unit flags: `--internal func_80101904 --internal func_80100D5C --internal func_80101D84 --internal func_80100E58
--internal func_80102448 --internal func_80102980 --internal func_8010221C --keep standin_1904` (+ F30 once present).

### func_8010221C (548 B): 96/137 → 11/137 words (13 rows), frame 24/24 (was 56)
`FAIL func_8010221C: 11 of 137 words differ | words 13 ops 0 norm 0 | frame 24/24` (v/q.c = fam_best.c).
Levers (each measured): `s16 x, y` (retail keeps the raw byte and narrows at each state_utility call; with s32 x
uopt CSEs the narrowing); the font switches are the file's inlined `font_set` (gives retail's `move s0,v0`, no
frame cost here); `if (item == D_801391E0) color = 22; else color = 1;`; a compiled-out `do {} while (0);` after the
item loop (DEVICE) adds one live block to the &gameplay_mode web (save 2.2 → 1.83) so it is split as in retail and
x gets s6 (frame 56 → 24). Residual: colour-only tie of four loop constants/addresses at save 2.0 (ctrace):
ours 4→s2, 6→s3, &D_801391E0→s7, 7→s8; retail 7→s2, 4→s3, 6→s7, &sel→s8 (the loop bound 7 must be numbered first).
`item <= 6`, `item != 7`, `7 > item` do not renumber it. Next: force.sh to confirm 0 rows, then a source form that
introduces the 7 earlier (or prio_patch oracle).

### func_80102F30 (3,560 B) — results dispatcher + standings table, new complete draft
`s32 func_80102F30(s32 arg0)` (kept, saves s0–s8/$f20–$f24, returns 1): `D_80138870 == 1` → func_8010221C();
`== 2` → switch (gameplay_mode) {4: func_80102448(); 6: func_80102980(); 0,2,3,5: table; 1: nothing} (jump table
0x80124864). The table has `D_80152734 + 3` rows × `active_player_count + 1` columns, row height 16, y =
row*16 - rows*16/2 + 110 (+8 from row 3 on, with an 8-px separator bar of width columns*60 at row 3); fill pass
via func_80100D5C (colours by column: 1 blue, 2 red, 3 yellow, 4 green), then font 11 text pass: header = player
names (D_801146AC colour, `input_rec0[c-1].name[0]+20` through race_position_update into a 252-byte label), row
labels fixed[208]/fixed[210]/`fcvt(fixed[205], row-2)`, cells = best lap D_80152908[car].best (if
D_80144018[car] == D_80152734), race time D_80144DA4[col] (if D_80144018[car] > 0), lap times
D_80149A78[col-1][row-3] (if row-3 < D_80144018[car]) in `%d:%02d.%03d` (D_80120360/70/80) with font 3, else
fixed[215] with font 11. Unit: `FAIL func_80102F30: 140 of 890 words differ | words 145 ops 43 | frame 640/672`.
Frame analysis (spcensus): all nine font switches inlined = 216 B of inline areas (24 B each; all written out:
frame 456). Retail: label..top 12 B larger, inline area 172 B (≈7×24+4). All 36 "two sites written out with a
holder" variants were worse (body 872–888 words). Next: identify which helper shape gives the 172 B (e.g. a
one-piece object_create-style wrapper at some sites), then the header-block colour (s6/s8 swap).
With F30 present the w27c stand-in is no longer needed: D5C/1904/D84 stay EQUAL with the real chain
(`blob_unit ... score func_80102F30 func_8010221C ... --with fam/fam_best.c --internal <all but F30> --neighbours`
→ `EQUAL func_80101904: 288 words`, `EQUAL func_80101D84: 294 words`, `EQUAL func_80100D5C: 63 words`,
`locked bodies that differ in this unit: 0`).
draft_check: `python3 -m tools.recomp.draft_check --remote rocky func_80102F30 cloud/work/frontier/w28a/fam/fam_best.c
--scripts race_finish,two_player_finish,player_finish --callee 's32 slot_state_setup(s32 font)=s2/s0,s1,s3'` →
`VERDICT: EQUIVALENT over 1350 calls / 3 scripts (race_finish 750, two_player_finish 240, player_finish 360)`.
func_8010221C with the same file and `--scripts race_finish,two_player_finish,player_finish,stunt_short` →
`VERDICT: EQUIVALENT over 1180 calls / 4 scripts`.

## Not started — survey for the next lane
- func_80103D28 (GC `DrawUnlocked`, 2,516 B vs GC 2,416; locals x, y, i, p, str[64], stuffCount): internal
  (no saves), only caller func_80105480 (+0x6C) which is unmatched → provisional at best; caller-less stubs at
  0x80103D20 and 0x801046FC (inlined statics). No script reaches it (ghost-save branch).
- func_80105480 (weak GC `DrawScores`): calls 3D28, 4B14 and the unregistered head func_80104704
  (highscore_entry_anim is its tail; needs refresh-game-context first).
- func_80105EA8 (GC `animResults` [hud.c], 2,508 vs 1,392 B; GC locals playerIdx, x, y, str[128],
  gNumPlayersFinished, gAPlayerFinished, min/sec/thou, str1/str2[32], place): kept, saves; calls slot_state_setup →
  test in the unit or package with ctx_gr3_b.c like func_800F0914.
- Unit pairs: game_results_render (kept) → game_results_update (internal); func_80100564 (kept) → game_results_exit
  (internal). GC leads only weak (audioscr.c/controlscr.c via shared tables).

## What generalises
- GC confident pairs were worth it: both highscorescr.c functions went from context copies to MATCH in one
  session each; the GC disassembly gave the statement order, the loop variables (functions.json block locals) and
  the struct layouts (race stats 0x60, player record name at +0x14); instruction order still came from the N64.
- **umerge inlining is decided by the callee's pre-inlining size times something per site**: a one-piece font
  wrapper `{osRecvMesg; slot_state_setup; osSendMesg}` is inlined at 1 site (func_800F0914) but not at 4
  (func_800F1210, nor object_create with 5 unit-wide sites); the split form font_set{gfx_lock(); slot_state_setup;
  gfx_unlock();} is inlined everywhere — that is why every HUD file carries the 3-helper shape.
- A switch on the global just stored (`g = v; switch (g)`) instead of on `v` changes PRE: the global's value is
  available at the switch, so uopt reloads it per case for a later `if (g)` (F1210, 54 → 4 rows).
- Inline areas (24 B per inlined font_set) do not always cost frame: func_8010221C with two inlined font_sets has
  retail's 24-byte frame once register pressure stops the spills.
- Packaging a match whose callees have IPA conventions: byte-identical context copies of the callee files (here
  credits_scroll_grp/gr3_b.c and the rand_r single) restored the unit behaviour in `score.py group`.

Builder scratch `~/rush2049/scratch/frontier/w28a` deleted at the end.
