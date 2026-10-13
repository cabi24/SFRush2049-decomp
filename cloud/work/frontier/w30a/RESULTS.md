# w30a — results / options screens (wave 30), IDO 5.3 -O3

Status: FINAL (wave 30). Unit tag `w30a`; builder scratch `watchman2:~/rush2049/scratch/frontier/w30a` deleted at the end.
Unit flag files used for the measurements: `uargs.txt` (results-table family), `uargs5480.txt`, `uargs_au.txt`, `uargs_ct.txt`
(blob_unit `--internal` lists; run `blob_unit --tag w30a score NAME --with FILE $(cat uargsX.txt)`).

| function | bytes | state | before -> after (unit) | draft_check | residual class |
|---|---|---|---|---|---|
| func_80102980 | 1,456 | **MATCH in group** `frontier_results_table` (superseding packet) + EQUAL in unit | 171 rows (102 words) -> EQUAL / MATCH | not needed (match) | none; devices: w29a's `if (previous) {}` holder + four-line colour inits; cell width through the existing local `other` |
| func_80100E58 | 2,732 | draft `func_80100E58/best.c` (group file + E58 body) | 180 rows (534 words), body 682/683 -> unchanged | not run (unchanged draft) | live-range split of `columns` (uopt f_addadjacents acceptance inequality), traced, no source lever yet |
| func_80105480 | 1,780 | complete draft `func_80105480/best.c` (GC animText@highscorescr.c) | not started -> 79 rows (ops 9), 440/445 words, frame 584/688 | blocked: N64Recomp `No function found for jal target: 0x80104704` (unregistered head) | callee-dependent (FP saves, func_80104704 IPA param s5) + frame (inline areas) |
| func_80103D28 | 2,516 | draft (w29a body, inside `func_80105480/best.c`) measured INTERNAL for the first time | kept-mode 103 ops -> internal 75 ops, 614/629 words, frame 192/264 | blocked (same build) | frame (per-category row pointers spilled in retail) + x/y narrowing |
| game_results_render | 1,164 | complete draft `audio_options/best.c` (GC animText@audioscr.c) | not started -> 114 rows (ops 32), 291/291 words | **EQUIVALENT** 1,273 calls / 3 scripts (audio_options 434, options_menus 540, pause_menus 299), draft sha 635bcc93 | loop LICM/strength-reduction difference (menuRect.x kept in memory) |
| game_results_update | 1,172 | complete draft `audio_options/best.c` (GC animButtonText@audioscr.c) | not started -> 112 rows (ops 16), 289/293 words | covered by the game_results_render run (same draft file defines and calls it) | frame 200/240 + colour |
| func_80100564 | 1,576 | complete draft `controls/best.c` (GC animText@controlscr.c) | not started -> 378 rows (ops 66), 411/394 words | **EQUIVALENT** 1,949 calls / 2 scripts (controls_frontend 1110, pause_menus 839), draft sha ae40327d | p kept in memory in retail, constant 10 web, exit call convention |
| game_results_exit | 1,916 | complete draft `controls/best.c` (GC animButtonText@controlscr.c) | not started -> 234 rows (ops 63), 488/479 words | covered by the func_80100564 run (same file) | goto-end tail (&D_80118E20 in t0), frame 520/544 |
| func_80104B14, func_80105EA8 | 2,404 / 2,508 | not started (time); survey below | — | — | — |

## Progress log
- func_80102980: **EQUAL in unit** (`func_80102980/y1_unit.c`, = installed results_table.c + the 2980 body): every cell-width
  60 in the 2980 body written through `other` (`other = 60;` already present): `160-columns*other/2` in the fill loop and both
  header blocks, `x=column*other` in the headers and `column*other-columns*other/2+190` in the final state_utility.
  Cause (pretrace): uopt emits hoisted header expressions in expression-bit order; literal `160-columns*60/2` got bit 58 (fill
  loop) ahead of &input_rec0[col] (bit 123), retail has the address first -> 171 -> 17 rows; then x (column*60, bit 132) vs
  total (bit 182) tied at save 175 and the lower bit took s0 (force.sh p1:w132=c15,p1:w182=c14 -> 4 rows); making every
  column*60 propagated renumbers x after total -> EQUAL.
- func_80102980 packaged: `groups/frontier_results_table/` (superseding, same name; members = the 6 installed + func_80102980;
  claims = members; keep unchanged func_80102F30; ctx_gr3_b.c byte-identical to credits_scroll_grp/gr3_b.c). Builder
  `score.py group cand/frontier_results_table`: all 7 members MATCH (F30 own .rodata 0x80124864..0x80124880, drawMain
  0x80124848..0x80124864), rc=0; `--claims` rc=0. Pi unit: 8/8 EQUAL, `locked bodies that differ in this unit: 0`.
- func_80100E58: lever (1) traced, not closed. The "two separate columns loads" are a live-range-split shape: columns
  (var sp+464, w17) is split; its text-loop piece (w299, start bb39) in retail covers the column==0 header block (bb45: uses
  v0 for columns*60) and NOT the cell-tail then-block (bb110, reload t9 because 60 sits in v0); ours rejects bb45 and adds
  bb110, so v0 is forbidden (60 constant) and the piece takes v1. Rejection rule read from uopt f_addadjacents (split
  oracle bin/uopt_spl from w16d/patch_spl.py): add block iff shared < old_regsleft && 2*regsleft >= numintf + shared.
  bb45: regsleft 12, numintf 23, shared 4 -> 24 < 27 reject. Retail needs numintf <= 20 at that point (or 2 more free regs
  in bb45). Variants without effect: the `other` cell-width lever (text only), q1 no holder (worse), total placement,
  tail as one named x. Fill-loop `other` lever makes the body 683/683 but worse rows (206).
- func_80105480 = GC `animText@highscorescr.c` (NOT DrawScores): switch (gHighScoreState) 1..5 via jump table 0x801248A0 =
  DrawUnlocked (func_80103D28) / DrawCircuitCode (inlined) / per-player DrawName loop (`if (D_80143F18[p] == 1)
  func_80104704(p)`, so func_80104704 = GC DrawName(p)) / DrawGhost (= func_80104B14, not a _PrintScores sibling) /
  DrawScores (inlined); render_helper(0.0f) before, render_helper(-1.0f) after, return 1. Complete draft
  `func_80105480/best.c` (= v/h1.c; also carries the w29a func_80103D28 body and non-static helpers du_*, hs_font_set,
  hs_CenterText, hs_RightText, DrawCircuitCode, DrawScores). Unit (`--internal func_80103D28 du_gfx_lock du_gfx_unlock
  du_font_set DrawCircuitCode DrawScores hs_CenterText hs_RightText hs_font_set`, 5480 kept):
  `FAIL func_80105480: 438 of 445 words differ; compiled body is 440 words, target 445 | words 79 ops 9 | frame 584/688`
  (was 366 rows with the first form). Levers: GC TB_CenterText/TB_RightText as inlined helpers (`x - (w>>1)`, `x - w`);
  the colour argument as one ternary call `dispatch_handler(c == -1 ? 1 : D_801146AC[c])` (248 -> 79 rows); font helper
  with one nested lock level (hs_font_set: lock helper + osSendMesg written out). Residual: (a) retail saves $f20/$f22
  (+16 B frame, 4 words): an internal callee (func_80104704 or func_80104B14, both still asm) clobbers them -- needs their C;
  (b) case 3 `move s5,a0` + `li at,1` in the loop: func_80104704 takes p in s5 as an IPA register parameter (needs
  func_80104704 registered (w30n) and drafted); (c) frame: retail has NO inline areas for the 6 font sites and 5 text
  helper sites (DrawScores area = exactly i, numWid, str[96], label[256] = 360 B at 136..496; DrawCircuitCode area =
  GC's x, y, i, length, str[64] at 496..576; p at 580); every helper shape tried costs 8 B per inlined call (lock/unlock
  helpers, font helper, text helpers), written-out font sites lose the `move s0,v0` -> open: which source shape
  inlines with no area.
- func_80103D28 measured INTERNAL for the first time (real caller present; umerge keeps it out of line now):
  `FAIL func_80103D28: 575 of 629 words differ; compiled body is 614 words | words 543 ops 75 | frame 264/192`
  (w29a kept-mode: 619 words, 103 ops, frame 264/232). Residual: frame 72 B short (retail homes 36..76 hold seven
  per-category row pointers `&D_80150xxx[p]` spilled at 52..76 -- ours keeps them in a0..a3/v1), x/y narrowing.
- AUDIO-OPTIONS screen (GC audioscr.c) -- names proposed: game_results_render (0x800FF298, kept) = GC `animText@audioscr.c`
  (propose `audio_animText`), game_results_update (0x800FEE04, internal) = GC `animButtonText@audioscr.c` (propose
  `audio_animButtonText`). Evidence: buttonsInited (D_801105B4) == 1 -> animButtonText, else title (CONTROLLER %d %s when
  gGameState & 0x7C0000 && gControllerRemoved), y = 2*TB_GetFontLineHeight()+10, 4 options (gAudioMenuSelect D_80110638
  colour 22/1, TB_RightText(name, menuRect.x-5), value switch: "%d" gOptions+13 / +12, gSong (D_80110640) 12/13/gSongNames
  D_80110030, music type), the toupper loop, HAL_SetTextTranslucency(-1) = func_800ED66C, TB_FlashTextColors = func_800BEA3C.
  animButtonText: ZOID_ProjectWindow = brake_light_update(0, &buttonInfo..., &gCamera D_80150B70, 0, screenPos), button
  angles vs 0.698/2.443/0.349/2.793/1.571 (D_80124828..38 = own .rodata), buttonInfo D_801102B4 (64-B entries, angle +8,
  pos +48), gOptions = D_80146108, flashColors = D_801146BC. Draft `audio_options/best.c` (both functions + non-static
  helpers au_*; and a GC-typed func_800BEA3C body, see below). Unit (`--internal game_results_update au_gfx_lock
  au_gfx_unlock au_font_set au_CenterText au_RightText`):
  `FAIL game_results_render: 156 of 291 words differ | words 114 ops 32 | frame 288/280` (length 291/291);
  `FAIL game_results_update: 154 of 293 words differ; compiled body is 289 words | words 112 ops 16 | frame 240/200`.
  Levers found: TB_FlashTextColors' real parameter type is GC `union __anon_21da {u8 c[4]; u32 rgba;}` -- a body
  `D_80118E28.rgba = first.rgba; D_80118E2C.rgba = second.rgba;` is EQUAL for func_800BEA3C itself in the unit (7 words,
  kept) and gives retail's single-load/two-store inlined copy (render 135 -> 114 rows); `x = screenPos[0]; y =
  screenPos[1];` named in animButtonText (155 -> 112 rows). NOTE: locked src/blob/func_800BEA3C.c uses a byte struct
  Color4; this draft redefines it (its definition wins in the unit) -- integration would need func_800BEA3C's file to
  switch to the union form (re-lock; it stays EQUAL) or the draft to carry it as a group member. Residual (render): the
  loop keeps menuRect.x in memory (280(sp)) and recomputes (s16)(x+5)/(s16)(x-5) and i*4 at the loop head (retail does no
  LICM/strength reduction there; ours hoists both into spill homes) -- cause not traced; frame 280 vs 288.
- CONTROLS screen (GC controlscr.c): func_80100564 (kept) = GC `animText@controlscr.c`, game_results_exit (internal) =
  GC `animButtonText@controlscr.c(signed char p)` (propose `controls_animButtonText`). Retail passes p to
  game_results_exit only through the callee's argument home (`sw t6,0(sp)` in the jal delay slot, callee `lb a0,547(sp)`),
  an IPA convention that needs both functions in the unit. Draft `controls/best.c` (func_80100564 + helpers ct_*;
  game_results_exit still extern): `FAIL func_80100564: 392 of 394 words differ; compiled body is 397 words | words 405
  ops 85 | frame 360/304`. GC-derived: gNumWindows D_80151AD0, buttonsInited[p] D_80154618, controlScrActive[p]
  D_8015418C, gPlayerRects (24-B s32 form) D_80113EE0[win][4] (y +4, w +8, h +12, x +16, +20), gControllerData D_80156CF0
  (16 B), gControlFirst D_8014A100[p], gControlSelect D_8014A0F8[p], gControlMenuSize D_8014A10A, PS_PakRumble(pak) =
  D_80144030[pak].b6 (s8; same record as billboard's pak_slot_b6), validOption(i) inlined as `i != 8` (N64 has 12
  items, GC 13), TB_DrawTextScrollBox = camera_auto_follow(x, y, w/2, h, -1, 0, fixed[38]), TB_EllipseText =
  race_position_update(pName, name+20, 69). Residual: p lives in memory in retail (home 352; `p + 1` for the sprintf not
  CSE'd with the increment), constant 10 in s8, the exit call convention, FP saves (+48 B: game_results_exit clobbers
  $f20-$f30 -- needs its C).

## Group packet `groups/frontier_results_table/` (supersedes the installed group of the same name)
Files: `results_table.c` = installed `src/blob/groups/frontier_results_table/results_table.c` + the func_80102980 body
(between drawMain and rt_font_set_b) + header comment updates; `ctx_gr3_b.c` byte-identical (cmp) to the installed copy
and to `credits_scroll_grp/gr3_b.c`. group.json: members = the 6 installed members + func_80102980; claims = members;
context and keep unchanged (keep = func_80102F30).
Builder (`python3 tools/cloud/score.py group cand/frontier_results_table`, IDO 5.3, scratch synced from wave30 1b34f797):
```
func_80102F30: MATCH  own .rodata verified at 0x80124864..0x80124880
drawMain:      MATCH  own .rodata verified at 0x80124848..0x80124864
func_80101904: MATCH
func_80101D84: MATCH
func_80102448: MATCH
func_80100D5C: MATCH
func_80102980: MATCH
Context (informational): slot_state_setup 2/58, object_create 28/28, func_800B669C 5/5 (same pattern as installed)
rc=0; --claims rc=0
```
Pi unit (`G=cloud/work/frontier/w30a/groups/frontier_results_table; python3 -m tools.conveyor.pipeline.blob_unit --tag w30a
score func_80102980 func_80102F30 drawMain func_80101904 func_80101D84 func_80102448 func_80100D5C slot_state_setup --with
$G/results_table.c --with $G/ctx_gr3_b.c --internal func_80102980 --internal func_80101904 --internal func_80100D5C --internal
func_80101D84 --internal drawMain --internal func_80102448 --internal rt_gfx_lock --internal rt_gfx_unlock --internal rt_font_set
--internal rt_font_set_b --neighbours`):
```
  EQUAL func_80102980: 364 words (internal, c_results_table.c)
  EQUAL func_80102F30: 890 words (kept, c_results_table.c)
  EQUAL drawMain: 137 words (internal, c_results_table.c)
  EQUAL func_80101904: 288 words (internal, c_results_table.c)
  EQUAL func_80101D84: 294 words (internal, c_results_table.c)
  EQUAL func_80102448: 334 words (internal, c_results_table.c)
  EQUAL func_80100D5C: 63 words (internal, c_results_table.c)
  EQUAL slot_state_setup: 58 words (internal, c_ctx_gr3_b.c)
  locked bodies that differ in this unit: 0
blob_unit score: 8/8 equal
```
Integration: `PYTHONPATH=. python3 cloud/work/frontier/tools/install_group.py cloud/work/frontier/w30a/groups/frontier_results_table
frontier_results_table` (same name supersedes; every old member kept; no `--revert-single`: func_80102980 was not locked).
unit_overrides: add `force_internal` func_80102980 if the group rule does not already make non-keep members internal (as for
the other five). No data-symbol changes, no own .rodata in func_80102980, provisional.json: nothing. func_80100E58 stays asm
(F30 calls it as an extern, unchanged).

## Survey / leads for the next lane
- func_80104B14 = GC `DrawGhost` (highscorescr.c; from func_80105480's jump table: state 4), not a _PrintScores sibling. GC
  locals i, y, str, str1[128], time[16], gFreePages. Internal (unsaved s0..s8) and probably the function that clobbers
  $f20/$f22 for func_80105480. Calls the pane box camera_auto_follow (= TB_DrawTextScrollBox) and fcvt "%01d:%02d.%03d".
- func_80104704 (unregistered head, w30n) = GC `DrawName(p)`; func_80105480 passes p in s5 (IPA) and a0 is only spilled.
- func_80105EA8 = GC `animResults` (hud.c): the head counts gNumPlayersFinished (s4) / gAPlayerFinished (s5) over the cars
  exactly as GC; kept, saves s0..s8 + $f20; register parameter t9 (component 104).
- func_80100E58: the open lever is the split acceptance at bb45 (`2*regsleft >= numintf + shared` must hold with
  regsleft 12, shared 4 -> numintf <= 20 at the piece start bb39, ours 23). Oracle: `bin/uopt_spl` (copied from w25d;
  w16d/tools/patch_spl.py) with `SPL_REJ`; a "force-accept" oracle hook for f_addadjacents would prove it before searching
  for a source form that removes three interferers at the text-loop row head.

## What generalises
- **Hoisted expressions are emitted in expression-bit order** (pretrace bit numbers): when uopt moves several
  expressions into one block (here above a call), ugen emits them by bit number, and as1 keeps that order when the
  dependency chains allow. A literal sub-expression that first appears in an earlier loop (`160 - columns*60/2` in the
  fill loop) is numbered early; writing the cell width through a variable everywhere (`other = 60`) defers every such
  expression to copy propagation, so later-written expressions (&input_rec0[col]) come first (171 -> 17 rows), and the
  same deferral flips a save tie between two propagated webs (17 -> 0). Same family as w29a's `cell = 60` lever.
- **uopt's split growth rule** (f_addadjacents, read from the recompiled uopt.c): a neighbour block joins a split piece iff
  `shared < old_regsleft && 2*regsleft >= numintf + shared` (else rejected; regsleft==0 always rejects). numintf is the
  piece's interference count so far (already ~all live ranges at the start block), so pieces starting in a busy loop head
  cannot absorb a block with 4+ coloured interferers.
- **GC types fix copy idioms**: TB_FlashTextColors takes GC `union {u8 c[4]; u32 rgba;}`; with the union and `.rgba`
  word moves the inlined copy becomes retail's one load / two stores (the byte-struct form gives lwl/lwr + `at` block moves).
- **Inline areas**: every inlined helper call costs 8 B of frame in our compiles (lock/unlock helpers, font helper, text
  helpers); retail func_80105480 shows none for 6 font sites + 5 text-helper sites -> some helper shape inlines with no area.
- A `x - (w >> 1)` / `x - w` pair = GC TB_CenterText / TB_RightText inlined (w29 lesson) also holds in highscorescr.c,
  audioscr.c and controlscr.c; the colour choice `c == -1 ? 1 : playerTextColor[c]` must be ONE call with a ternary
  argument (248 -> 79 rows in func_80105480).

## draft_check commands
`python3 -m tools.recomp.draft_check --remote rocky game_results_render cloud/work/frontier/w30a/audio_options/best.c --scripts audio_options,options_menus,pause_menus`
-> `VERDICT: EQUIVALENT over 1273 calls / 3 scripts (audio_options 434, options_menus 540, pause_menus 299)`.
`python3 -m tools.recomp.draft_check --remote rocky func_80100564 cloud/work/frontier/w30a/controls/best.c --scripts controls_frontend,pause_menus`
-> `VERDICT: EQUIVALENT over 1949 calls / 2 scripts (controls_frontend 1110, pause_menus 839)`.
`... func_80105480 cloud/work/frontier/w30a/func_80105480/best.c --scripts player_finish,ghost_save,race_finish,two_player_finish`
-> `ERROR: repl build: N64Recomp failed: No function found for jal target: 0x80104704` (rerun once w30n registers it).
EQUIVALENT is behaviour only, not coverage. Both EQUIVALENT drafts carry a redefinition of func_800BEA3C (GC union body,
EQUAL in the unit); disclose if those drafts are ever integrated.
