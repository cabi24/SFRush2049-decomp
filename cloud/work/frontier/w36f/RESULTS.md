# w36f RESULTS (wave 36, overlay image A, recordscr)

Status table (updated after every function):

| function | GC identity | bytes | state | best file |
|---|---|---|---|---|
| func_803923B0 | updateButtons@recordscr.c + inlined updateViewButtons (CONFIRMED) | 2936 | **MATCH in group** (2 disclosed unused locals; own .data/.rodata verified vs private image) | groups/dot_runtime_a_callers_deep_20261011/ |
| func_803AA3D8 | drawSelect@recordscr.c (CONFIRMED) | 2868 | **MATCH in group** (1 disclosed unused local; own .rodata verified vs private image) | groups/runtime_a_stats_menu_20261006/ |
| func_803AD57C | animText@recordscr.c + inlined drawView + nested-inlined drawStuntTotals | 2092 | near-miss: strict 497/523 (1-word dispatch shift), 30 non-equal-run words (w35e: 36); spill homes now retail | func_803AD57C/best/ |
| func_80399D10 | updateModeScreen@modescr.c (CONFIRMED; lane lead was none) | 1340 | **MATCH in group** (first typed port; own .rodata verified) | groups/dot_runtime_a_seven_option_group_20261007/ |
| func_803993EC | updateButtons@modescr.c (CONFIRMED; NOT videoscr/audioscr) | 1228 | **MATCH in group** (3 disclosed unused f32; own .rodata verified) | groups/dot_runtime_a_seven_option_group_20261007/ |

Builder scratch: watchman2:~/rush2049/scratch/frontier/w36f (copied from base, master e68aea6e1).

## Identities proven

- A:func_80399D10 = updateModeScreen@modescr.c (GC 0x80073A38); A:func_803993EC = updateButtons@modescr.c (GC 0x800745F0);
  A:func_80399394 = optionValid@modescr.c; A:func_803998C0 = initScreen@modescr.c with initButtons inlined (stub 0x803998B8);
  stubs 0x80399384/0x8039938C = deleted uninitScreen/uninitButtons; A:func_803A6B50 = animText@modescr.c (structure of the
  installed member: title, "%d %s" players line, 7 option labels with flash colours). Evidence: MATCH.
  Data: buttonInfo[13] D_803B42F4, validOption table at D_803B4679 (installed optionValid's indexing kept as is),
  gButtonScale..cursorZ D_803B463C..D_803B4650, cursorSpeed D_803BAC18,
  buttonsInited D_803B4634, buttonsJustInited D_803B4638, inited D_803B4678, blit D_803B467C, gModeMenuSelect D_803B469C,
  exitModeScreen D_803B46A0.

- A:func_803AA3D8 = drawSelect@recordscr.c (GC 0x8008126C); evidence: MATCH (string/table indices, pak-status switch N64-only).

- A:func_803923B0 = updateButtons@recordscr.c (GC 0x80084EE8) with updateViewButtons (GC 0x80085998) inlined; evidence: MATCH.
- A:func_803915E8 = updateSelectButtons@recordscr.c (GC 0x80084F38) (called in case kRecordStateSelect; w36c's function).
- A:func_80394058 = updateRecordScreen (calls func_80393B3C = initRecordScreen with initButtons inlined, stub 0x80393B34;
  controlView inlined as its case 1); func_8039338C = controlSelect; func_803932E4 = uninitRecordScreen (+ inlined uninitButtons;
  stubs 0x803932D4/DC = uninitButtons + controlView); func_803931C0 = setRecordState; func_80393004 = updateSelectInfo;
  func_80392F28 = updateViewInfo; func_803930B8 = updatePakInfo (structure, globals and call graph of the installed members;
  not separately scored here).
- A:0x803ACA8C (caller-less stub) = deleted drawStuntTotals (nested-inlined into animText via drawView).
- Image A recordscr data: buttonInfo[23] = D_803B5F28 (0x40), gButtonScale..cursorZ = D_803B64E8..D_803B6500 (buttonX at
  D_803B64F0), buttonsInited D_803BA7D0, buttonsJustInited D_803BA7D4, cursorSpeed D_803BA7D8, scrollSpeed D_803BA7DC,
  gViewMenuVisible D_803BA7FE[2], gRecordState D_803BA804, gViewMenuSelect D_803BA810, "BUTTON_SELECT" D_803B8C60, "BUTTON" D_803B8C70.

## Notes per function

### func_803923B0 = recordscr updateButtons + inlined updateViewButtons -- MATCH (group)

- Identity: root func_80394058 (updateRecordScreen, kept) calls it last; body = `if (buttonsInited) switch (gRecordState)
  { case 0: updateSelectButtons() [= jal func_803915E8]; case 1: updateViewButtons() [inlined] }`. The installed group
  `dot_runtime_a_callers_deep_20261011` already had the recordscr family as members (setRecordState = 931C0,
  updateSelectInfo = 93004, updateViewInfo = 92F28, updatePakInfo = 930B8, uninitRecordScreen = 932E4) and 923B0 as context.
- First typed port: 8/734 (frame 136 vs 144 + one delay-slot pick). Levers:
  1. DWARF local ORDER (rush_d_dwarf.c, not the MAT3 C, which moved clearScrollSpeed up for CW): clearScrollSpeed after
     selectedPos puts it at sp+91 (retail).
  2. `s8 clearScrollSpeed = 1;` keeps its initialiser, `gViewMenuCount = 0; whichItem = 0;` are statements after the
     declarations: the dispatch `beq` delay slot then takes `li t7,1` (retail), not `move a1,zero`.
  3. Frame: 8 B above the inlined callee's locals that nothing explains (one stub only, 0x803915E0 = the deleted
     updateViewButtons). Two unused s32 in updateButtons (disclosed in the header; two unused int params of
     updateViewButtons give the identical object). static vs non-static and define-before/after change nothing.
- Exact scorer output (builder, `python3 tools/cloud/score.py group lane/groups/dot_runtime_a_callers_deep_20261011 --targets asm/us/ovl_a --claims`, exit 0):
  `func_803923B0:` / `  MATCH (12 section-relative relocations unverified: .data+0x0 at +0x48, .data+0x0 at +0x4c, .rodata+0x0 at +0x148, ... .rodata+0x10 at +0x3f0)`
  then `not verified: ... no retail data available` (expected for overlays). Plain mode: all 7 members MATCH, exit 1 (unverified own data only).
- Own data verified locally (w35a owncheck.py with build/ovl/a/image.bin): `own .data verified at 0x803B65B0 (4 bytes)`,
  `own .rodata verified at 0x803B9440..0x803B9454` (2pi, pi/2, 0.4, 4pi, pi).
- Integration: supersede `src/ovl_a/groups/dot_runtime_a_callers_deep_20261011` in place: members + func_803923B0, claims
  [func_803923B0], context [func_8039338C, func_80393B3C], keep unchanged [func_80394058]. Only shared change:
  `extern volatile u8 D_80140BDC` (also retail's form in context 93B3C). `updateViewButtons` is an extra non-member symbol
  (deleted stub), not on keep.

### func_803AA3D8 = recordscr drawSelect -- MATCH (group)

- Typed port of MAT3 drawSelect with the DWARF locals (i, x, y, cx, str, str1[64], screenPos[2]); str1[64] (GC DWARF) is
  what PR #363 / #371 lacked (their text[80] gave frame 192 vs 176; #371's "stored-name bound" debate is moot for matching:
  the GC DWARF fixes the array at 64).  First port 153/717 -> MATCH in 5 steps:
  1. drop GC's dead block-scope tRGBA c1/c2 pairs (frame 208 -> 176);
  2. one of player/playerIdx block-scope keeps the homes (151 -> 137);
  3. `str = taba[tab[50] + 1]; TB_DrawText(x, y, str)` instead of passing the expression (137 -> 77; w35e lesson 4);
  4. `player = gStatPlayer;` before `if (!player)`, else-loop has no `*player` test (77 -> 8);
  5. unused `s32 pad` first (cfe area 104 -> 108: spill home sp+56 -> sp+52, traced with tools/trace/spill.sh: the
     spilled web is home 3 at -120) and the list step after the loop body (as1 line tie-break) (8 -> 0).
- Exact scorer output (builder, `python3 tools/cloud/score.py group lane/groups/runtime_a_stats_menu_20261006 --targets asm/us/ovl_a --claims`, exit 0):
  `func_803AA3D8:` / `  MATCH (8 section-relative relocations unverified: .rodata+0xb4 at +0x458, ... .rodata+0xc0 at +0x93c)`;
  plain mode: all 12 members MATCH (exit 1 only for unverified own data); context func_803AD57C 348/523 (installed root).
  Also MATCH inside w35e's improved stats group (AD57C 497/523 there).
- Own data (private image): `own .rodata verified at 0x803B98AC..0x803B98CC` (pi/2, 2.443461, 0.6981317, 5-entry jump table).
- Integration: supersede `src/ovl_a/groups/runtime_a_stats_menu_20261006` in place: members + func_803AA3D8, claims
  [func_803AA3D8], context [func_803AD57C], keep [func_803AD57C] unchanged; the eleven old member bodies untouched.

### func_803AD57C = recordscr animText (drawView inlined, drawStuntTotals nested inline) -- near-miss

- Base: w35e best group with my matching func_803AA3D8 (no change to AD57C: 36 non-equal-run words, strict 497/523).
- Spill homes (w35e residual (d), 8 words) FIXED: traced with tools/trace/spill.sh. The spilled webs take uopt homes in
  itable order below the cfe area; ours had area 174 (drawView 166 + 8 B inline reservation), retail needs <= 172.
  drawView's 166 = 88 fn locals + 16 (two GC c1/c2 pairs) + 56 hand-inlined drawStuntTotals locals + 4 (cfe temp of the
  `gRecordTrack > 12 ? .. : ..` ?:, allocated after all locals) + 2 (cfe temp for a call result used inside an expression,
  `y += TB_GetFontLineHeight()` / `96 + TB_GetFontLineHeight() * 5`; a plain `v = f()` creates none; the prototype's
  return type does not matter).  Making drawStuntTotals a REAL non-static function (nested umerge inline) and keeping
  GC's dead `Color4 c1, c2;` in ONE arm only gives area 170, frame 312, every home/local at retail's offsets
  (screenPos 216, str1 220, x/y homes 188/192, text 144, spill homes 124/132).  -> 30 non-equal-run words.
- Remaining (all as1 scheduling, structure equal): (a) dispatch: as1's cross-block pass hoists the case-1 compare's
  `li at,1` into the `lw v0` stall and leaves a nop in the beqz delay slot (retail: li at,1 in the delay slot; one-word
  shift => strict 497).  as1 trace (build/trace/w36f/as1r_n3.func_803AD57C.log, 2nd pass) shows li at,1 entering the
  first block's DAG; `.livereg` marking $at live at either branch does not stop it (asm oracle tools/asmo.sh).
  The matched func_803923B0 has the SAME hoisted pattern in retail, so this is context-dependent, not a fixed rule.
  (b) loop head `addiu s0,s2,752` / `lui at,0x4f80` one slot early; (c) `lw a2,0(t9)` placement; (d) a `move a1` in
  a jal delay slot vs before it.
- as1 oracle result (tools/asmo.sh on the ugen listing of best/, func_803AD57C/asm/*.s): inserting a LABEL between the
  dispatch's `lw $2, D_803BA804` and `beq $2, 0, $277` (ec.s), or `b $999; $999:` (eh.s, as1 deletes the jump but keeps
  the block boundary), gives retail's dispatch exactly: strict 498 -> 16, non-equal-run 27 -> 14.  So retail's as1 input
  had a block boundary there (the dispatch block had a second predecessor or uopt left the switch's `UJP` to its compare
  chain unmerged).  (b) and the `lui at,0x4f80` of (b) are the same cross-block hoist (ours pulls the successor's first
  instruction into a load stall, retail does not), so retail probably had block boundaries there too.  `.livereg`, `.loc`
  and 10..300 dummy blocks elsewhere change nothing (no as1 size threshold).  Source shapes tried without effect: named
  `state` local, `do {} while (0);`/`if (x) {}` before the switch, default arms (first/last), case order, goto-based arms,
  switch inside an inlined helper taking the state as parameter, if-chain (layout differs).  Next: find the C construct
  that leaves the switch's dispatch as a separate uopt block (candidates: a loop/goto target at the switch head with the
  load hoisted above it, or a cfe switch-temp form); then (b)-(d) may follow.
- uopt view (tools/trace/pretrace.sh n3): the cfe switch temp varM(158-4) is stored in uopt block 1 (`str temp =
  gRecordState`) and compared in block 2 (`equ temp,1`, cg1) -- they ARE separate uopt blocks in ours too; ugen just emits
  block 2 without a label because nothing branches to it any more.  Retail's ugen output evidently still had a reference
  to the dispatch label.  (b) is different in kind: retail places `addiu s0,s2,752` (&buttonInfo[i+11].uvs[3]) in the
  u32->float conversion block where our compile has `lui at,0x4f80`; in the matched drawSelect (same file) retail DOES
  hoist that `lui at` before the bgez, so this is placement of the address computation (uopt/ugen), not an as1 rule.
- best/ is also installed as the (non-claimed) context of the stats deliverable group (strict 497/523 there).
- as1 `-Olimit` is not the cause: the listing assembled with -Olimit 115..5000 gives the same 27 words; below 111 as1 drops
  other optimisations (77 words worse) without ever producing retail's dispatch (tools/asol.sh).

### func_803993EC + func_80399D10 = modescr updateButtons + updateModeScreen -- MATCH, MATCH (group)

- Identity: the seven-option screen is MAT3 modescr.c (7 race modes, validOption[4][7], gRace 0..6, game state 0x4000 for
  mode 3); the lane lead (videoscr/audioscr) was wrong.  The installed group `dot_runtime_a_seven_option_group_20261007`
  already had optionValid (99394) and animText (A6B50) as members and both functions as context.
- updateModeScreen: the first typed port MATCHES (uninitScreen/uninitButtons as non-static functions defined after the
  root; umerge inlines them three times, retail stubs 0x80399384/0x8039938C).
- updateButtons: first port 21/307 -> cursor stores in order [0], [1], [2] (GC: [2], [0], [1]) -> 9 -> three unused f32
  declared first: MATCH.  Frame arithmetic in the header (88 -> 96; one slot fixes the homes but not the frame; the w35d
  startscr updateButtons sibling has the identical three-slot residual).  N64 differences: unselected buttons settle to
  0.0f (GC 0.17453292f); `cursorSpeed < fabsf(d) * 60.0f / 10.0f`; volatile gFrameSeconds / gNumModels.
- Exact scorer output (builder, `python3 tools/cloud/score.py group lane/groups/dot_runtime_a_seven_option_group_20261007 --targets asm/us/ovl_a --claims`, exit 0):
  `func_803993EC:` / `  MATCH (8 section-relative relocations unverified: .rodata+0x14 at +0x1c, ... .rodata+0x20 at +0x2d8)`
  `func_80399D10:` / `  MATCH (2 section-relative relocations unverified: .rodata+0x24 at +0x410, .rodata+0x24 at +0x418)`
  then `not verified: ... no retail data available` lines; plain mode: all 4 members MATCH (exit 1 only for unverified own data).
- Own data (private image): `own .rodata verified at 0x803B951C..0x803B952C` (993EC: pi/2, pi, 4pi, 2pi; 180.0 etc. are
  lui-immediates), `own .rodata verified at 0x803B9560..0x803B957C` (99D10's 7-entry jump table).
- Integration: supersede `src/ovl_a/groups/dot_runtime_a_seven_option_group_20261007` in place: members [99394, A6B50,
  993EC, 99D10], claims [993EC, 99D10], context [] (both former context entries are now members), keep unchanged
  [func_80399D10, func_803A6B50]; renderer.c byte-identical; eligibility.c's optionValid unchanged; D_80140BDC and
  D_8002EB94 are now volatile (only the new bodies use them).  The context copies of 993EC/99D10 in
  `dot_runtime_a_seven_option_init_20261007` are stale context and can be refreshed from this group (not required).

## Integration summary (three superseding groups, all in place, same names, old members unchanged)

| group | members (new in bold) | claims | context | keep |
|---|---|---|---|---|
| dot_runtime_a_callers_deep_20261011 | 92F28 93004 930B8 931C0 932E4 94058 **923B0** | 923B0 | 9338C 93B3C | 94058 |
| runtime_a_stats_menu_20261006 | AAF0C AB5FC AB848 ABEC8 AC330 AC54C AC7F4 ACA94 ACD5C ACF5C AD0D4 **AA3D8** | AA3D8 | AD57C (improved, not claimed) | AD57C |
| dot_runtime_a_seven_option_group_20261007 | 99394 A6B50 **993EC 99D10** | 993EC 99D10 | (none) | 99D10 A6B50 |

All three: `allow_unverified: true` (no public image-A data tree); every claim's own .data/.rodata verified locally against
build/ovl/a/image.bin with w35a's owncheck.py (tools/own.sh).  Extra non-member symbols (deleted stubs of inlined helpers,
off keep, given unique names so they cannot clash with other groups' globals): recordscr_updateViewButtons;
recordscr_drawView, recordscr_drawStuntTotals; modescr_uninitScreen, modescr_uninitButtons.  (w35d's startscr group uses
`static` uninitScreen/uninitButtons; renaming changed no code: all three groups rescored MATCH after the rename.)  No overrides, no lock edits.
Bytes newly claimed: 2936 + 2868 + 1340 + 1228 = 8372 (image A).

## What generalises

1. Use the GC DWARF local list (rush_d_dwarf.c), not the MAT3 C declaration order: MAT3 reorders declarations for
   CodeWarrior (recordscr updateViewButtons: clearScrollSpeed after selectedPos in DWARF; that order gave retail's homes).
   Initialisers matter separately from declaration order: keep an initialiser where retail's first statement needs it,
   write the rest as statements (as MAT3's sibling functions do).
2. GC's dead block-scope `tRGBA c1, c2;` are NOT a constant: drawSelect has none on the N64, drawView keeps one pair.
   Measure with tools/trace/spill.sh: the cfe area (`AREA f_readnxtinst`) sets where uopt's spill homes start; a home
   off by 4 is a 4-byte difference in the local area (locals, block locals, cfe temps), not a colouring problem.
3. cfe temps sit at the END of the local area: a `?:` (4 B, shared with switch scrutinees) and a call result used inside
   an expression (`y += f()`, `96 + f() * 5`: 2 B; a plain `v = f()` creates none; the prototype's return type does
   not matter).
4. Stubs locate inlined statics, including nested ones: drawStuntTotals' stub is 0x803ACA8C (between its GC neighbours).
   A real nested inline (non-static, off keep) instead of hand-inlining fixed the frame/homes.
5. The N64 screen-button functions carry 1-3 unexplained 4-byte slots in their local area (recordscr updateButtons 2,
   modescr updateButtons 3, startscr updateButtons 3 in w35d, drawSelect 1): disclose them as frame residuals.
6. as1 residuals can be tested on the ugen listing (tools/asmo.sh): a label between a switch's scrutinee load and its
   compare chain turns the "hoisted li at,1 + nop" into retail's "li at,1 in the beqz delay slot".
7. Lane leads can be wrong at the file level: the 0x80399xxx pair is modescr.c (7 race modes), not videoscr/audioscr.

## Permission denials
none
