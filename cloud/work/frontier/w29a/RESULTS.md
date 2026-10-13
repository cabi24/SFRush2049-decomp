# w29a — results / high-score / records screens (wave 29), IDO 5.3 -O3

Status: FINAL (wave 29). Unit tag `w29a`; builder scratch `watchman2:~/rush2049/scratch/frontier/w29a` deleted at the end.

| function | bytes | state | before → after | draft_check | residual |
|---|---|---|---|---|---|
| func_80102F30 | 3,560 | **MATCH in group** `frontier_results_table` (kept) + EQUAL in unit | 140/890 → MATCH | EQUIVALENT on the final file: 1,350 calls (race_finish 750, two_player_finish 240, player_finish 360) | none; devices: 2 unused locals (exact frame residual), helper-shape hypothesis |
| func_8010221C | 548 | **MATCH in group** + EQUAL in unit | 11/137 → MATCH | EQUIVALENT (w28a, 1,180 calls) | none, no device |
| func_80101904 | 1,152 | **MATCH in group** + EQUAL in unit (real callers) | EQUAL (provisional) → MATCH | — | w27c devices unchanged |
| func_80101D84 | 1,176 | **MATCH in group** + EQUAL in unit | EQUAL (provisional) → MATCH | EQUIVALENT (w27c, 690 calls) | w27c devices unchanged |
| func_80100D5C | 252 | **MATCH in group** + EQUAL in unit | EQUAL (provisional) → MATCH | — | none |
| func_80102448 | 1,336 | **MATCH in group** + EQUAL in unit | 16 rows (18 words) → MATCH | — | one variable reuse (`left = 60`) + w27c holder |
| func_80100E58 | 2,732 | draft `func_80100E58/best.c` (whole family file) | 267 rows (633 words) → 180 rows (534 words), body 682/683, frame exact | — | structure: `columns` reload in the cell tail + colour |
| func_80102980 | 1,456 | draft `func_80102980/best.c` (whole family file) | 176 rows (325 words) → 171 rows (102 words), body and frame exact | — | header-block schedule + colour |
| func_800F0F44 | 464 | **MATCH in group** `frontier_billboard_render` (superseding) + EQUAL in unit | 8/116 → MATCH | not reached by a script | none; one device (`do {} while (0);`) |
| func_80103D28 | 2,516 | complete draft `func_80103D28/best.c` (GC DrawUnlocked), provisional at best | not started → kept-mode 103 ops rows (incl. 18 save/restore) | no script reaches it | internal measurement blocked (umerge inlines it into any C stand-in); needs func_80105480 |
| func_80105480, func_80104B14, func_80105EA8, func_80100564, game_results_update/render/exit | | not started (time); survey below | — | — | — |

Total landing with the group: 3,560 + 548 + 1,152 + 1,176 + 1,336 + 252 = **8,024 B** (6 functions); plus
func_800F0F44 (464 B) via the superseding billboard packet = **8,488 B, 7 functions**.

## Group packet `groups/frontier_results_table/` (NEW group; supersedes nothing)

Files: `results_table.c` (func_80100D5C, func_80101904, func_80101D84, func_80102448, func_8010221C, func_80102F30 and the
non-static helpers rt_gfx_lock, rt_gfx_unlock, rt_font_set, rt_font_set_b), `ctx_gr3_b.c` (byte-identical copy of
locked `src/blob/groups/credits_scroll_grp/gr3_b.c`: slot_state_setup's `$s2` IPA parameter; same as w28a's
billboard packet). Members = claims = func_80102F30, func_8010221C, func_80101904, func_80101D84, func_80102448, func_80100D5C.
Context: slot_state_setup, object_create, func_800B669C. Keep: func_80102F30 (only externally called member).
func_80100E58 and func_80102980 are NOT in the packet (stay asm; F30/221C call them as externs —
nothing is live across those calls, and they call func_80100D5C with the same register convention).

Builder (`python3 tools/cloud/score.py group cand/frontier_results_table`, IDO 5.3):
```
Members:
func_80102F30:
  MATCH
    own .rodata verified at 0x80124864..0x80124880
func_8010221C:
  MATCH
    own .rodata verified at 0x80124848..0x80124864
func_80101904:
  MATCH
func_80101D84:
  MATCH
func_80102448:
  MATCH
func_80100D5C:
  MATCH
Context (informational; excluded from exit status):
slot_state_setup: 2/58 words differ; object_create: 28/28 words differ; func_800B669C: 5/5 words differ
```
(context rows are the same informational pattern as w28a's frontier_billboard_render packet: the copies are
inlined/deleted in the group compile). `--claims`: rc=0, 6 MATCH.

Pi unit (`G=cloud/work/frontier/w29a/groups/frontier_results_table; python3 -m tools.conveyor.pipeline.blob_unit --tag
w29a score func_80102F30 func_8010221C func_80101904 func_80101D84 func_80102448 func_80100D5C slot_state_setup --with
$G/results_table.c --with $G/ctx_gr3_b.c --internal func_80101904 --internal func_80100D5C --internal func_80101D84
--internal func_8010221C --internal func_80102448 --internal rt_gfx_lock --internal rt_gfx_unlock --internal rt_font_set --internal
rt_font_set_b --neighbours`):
```
  EQUAL func_80102F30: 890 words (kept, c_results_table.c)
  EQUAL func_8010221C: 137 words (internal, c_results_table.c)
  EQUAL func_80101904: 288 words (internal, c_results_table.c)
  EQUAL func_80101D84: 294 words (internal, c_results_table.c)
  EQUAL func_80102448: 334 words (internal, c_results_table.c)
  EQUAL func_80100D5C: 63 words (internal, c_results_table.c)
  EQUAL slot_state_setup: 58 words (internal, c_ctx_gr3_b.c)
  locked bodies that differ in this unit: 0
blob_unit score: 7/7 equal
```
(no stub-tail warning: the helpers are non-static, w17j.)

### Integration notes
- `PYTHONPATH=. python3 cloud/work/frontier/tools/install_group.py cloud/work/frontier/w29a/groups/frontier_results_table frontier_results_table`
  (new group name; no `--revert-single`: none of the five was locked).
- unit_overrides `force_internal`: func_8010221C, func_80101904, func_80101D84, func_80102448, func_80100D5C (all internal in
  retail; not on keep, so the group rule may already cover them — check) and the helpers rt_gfx_lock,
  rt_gfx_unlock, rt_font_set, rt_font_set_b (caller-less after inlining; as w28a's hs_* helpers).
- ctx_gr3_b.c duplicates locked definitions (slot_state_setup/object_create/func_800B669C) byte for byte, exactly
  as frontier_billboard_render's ctx_gr3_b.c does; the existing `force_keep` func_800B669C entry covers the w28i
  trap. Check `blob_unit check` picks either copy cleanly.
- provisional.json: nothing listed (checked).
- Data symbols: no definitions; `extern s32 D_80118E20[2]` (defined as `Pair` in credits_scroll_grp/gr3_d.c).
- Own .rodata: two jump tables (221C 0x80124848, F30 0x80124864), verified by score.py.

## func_8010221C (GC `drawMain`, gameoverscr.c) — the lever
w28a residual: colour-only tie of the loop constants 4, 6, &D_801391E0, 7 at save 2.0 (force.sh with
`p1:w87=c16,p1:w83=c17,p1:w85=c21,p1:w86=c22` → 2 rows = the unverified jump-table relocations only).
Traced cause (pretrace `makelivranges`): constant/address register candidates are numbered by the FIRST BLOCK
holding them, in block order (4 in block 15, &gameplay_mode 15, 6 16, &sel 18, 7 25 = the loop-test block).
Retail numbers 7 first. Variants: while-loop, do-while, `if (item>=7) goto` pre-test, compiled-out `if (item<7) {}`
in/before the body: no change or much worse. **`if (!func_800F84B0(item)) continue;`** (an early forward reference
to the loop-test block) numbers 7 first: EQUAL, and w28a's `do {} while (0);` device is no longer needed.
GC drawMain confirms locals i, x (s16), y (s16), str and the validOption loop.

## func_80102F30 — four levers, all traced
1. **Frame (named lever: "helper shape giving retail's 172 B")**. sp census: retail and ours share every offset from
   `label` up to `y`; retail has 3 more words above `y` and 44 B less below `label`. Measured per-site inline areas:
   rt_font_set (lock helper + slot_state_setup + unlock helper) = 24 B; one nested helper only (either side
   written out) = 16 B and identical code; a flat one-piece wrapper is not inlined (frame 456), written-out sites
   lose one word each (body 888). With the six font-11 sites on the 16-B shape and the three font-3 sites on the
   24-B shape plus three words at the top of the frame: frame 640/640 and 145 → 60 rows. One of the three top words
   is a real local (`cell`, item 3); the other two are unused (exact residual: 632 without, 632/89 words with one).
2. **`text = value` after the sprintf** (GC drawRaceScore order `sprintf(str1, …); str = str1;`): as1 then fills
   the first call's delay slot with the `addiu s5,sp,576` as retail (60 → 43 rows).
3. **Header block order** `font_set(11); dispatch_handler(color[col-1]); text = name; left = …; x = …;` (the
   addresses are then numbered before x/left): 43 → 17 rows, colour-only left (force.sh
   `p1:w39=c22,p1:w123=c20,p1:w136=c21` → 7 rows incl. 2 jump-table relocs).
4. **The s6/s8 swap**: pretrace shows x = `mpy(column,60)` is entered at read time (bit 39), left =
   `160 - columns*60/2` only after copy propagation (bit 136), the colour-table address at bit 123. Retail order
   ptr < left < x means x is also a propagated expression: `cell = 60; x = column*cell;` (one site, fill loop) →
   1 row. The last row (skip of cell (0,0) jumping past the `columns` reload) closed with
   `if (row == 0 && column == 0) continue;` in the text loop → EQUAL.
GC evidence: drawRaceScore (gameoverscr.c) locals y, str, str1[40], rows, cols, row, col, c[4], time; the row/col
loops, 160 - cols*60/2, 112/110 base, drawRect colours and the `%d:%02d.%03d` cells agree with N64.
GC names: func_80102F30 ≈ (gameover draw dispatch + drawRaceScore), func_8010221C = drawMain, func_80100D5C =
drawRect, func_800F84B0 = validOption, D_801391E0 = gMainSelect, D_80114724 = titlePos, D_801146AC =
playerTextColor, func_80102448 = drawStuntScore?, func_80102980 = drawBattleScore? (dispatch on gRace 4/6),
func_80101D84 = drawStuntStandings, func_80101904 = drawBattleStandings, func_80100E58 = drawRaceStandings.

## func_800F0F44 (GC `_FindGhosts`) — superseding packet `groups/frontier_billboard_render/`
Packet = the installed group (all files byte-identical) with `func_800F0F44.c` replaced by the w29a body and
func_800F0F44 moved context → members; `"claims": ["func_800F0F44"]`; keep unchanged (billboard_render).
Trace: in the w28a `= 1`-first form the colours are retail's but the block layout is not (9 rows); in the
`= 2`-first form (w28a v/e.c) the layout is retail's and force.sh `p1:w116=c21,p1:w119=c18,p1:w120=c19,p1:w121=c20`
gives 0 rows (colour-only). ctrace: save = tot/nocs; the three loop webs (&D_8014978C, &D_80152570, const 1) have
tot 10 / nocs 5 = 2.0 and the constant-2 web (born at the `D_8014A110 == 2` compare, block 11, so lowest number)
also 2.0 → 2 wins s4. In the 1-first form the loop has fewer blocks (nocs 4 → 2.5) so 2 (nocs 5) lost the tie.
Lever: lengthen only the 2-web: one compiled-out `do {} while (0);` between the gRace compare and the loop
(d1: EQUAL; two: EQUAL; three/four: 9 rows; `node = D_80152028; do {} while (0); for (; …)` also EQUAL). GC has
a GhostFileAvailable() status check at exactly this point. Variants without effect: `!(a && b && c)`, ternaries,
`= 1` default then `= 2`, goto form (worse), `== 2U` (worse, 14 rows).
Builder `score.py group cand/frontier_billboard_render`: billboard_render MATCH (own .rodata verified
0x80124604..0x80124618), func_800F207C MATCH, func_800F1930 MATCH, func_800F0914 MATCH (0x801245B4..0x801245B8),
func_800F1210 MATCH (0x801245B8..0x801245E4), func_800F0F44 MATCH; `--claims` rc=0.
Pi unit (`G=cloud/work/frontier/w29a/groups/frontier_billboard_render; W=""; for f in $G/*.c; do W="$W --with $f"; done;
blob_unit --tag w29a score func_800F0F44 func_800F1210 func_800F0914 billboard_render func_800F207C func_800F1930
func_800F2718 slot_state_setup $W --internal hs_font_set --internal hs_gfx_lock --internal hs_gfx_unlock --internal
pak_slot_b1 --internal pak_slot_b5 --internal pak_slot_b6 --neighbours`): 8/8 EQUAL (func_800F0F44 116 words
internal), `locked bodies that differ in this unit: 0`. Both w29a packets together: 10/10 EQUAL, 0 differing.
Integration: `install_group.py cloud/work/frontier/w29a/groups/frontier_billboard_render frontier_billboard_render`
(same name supersedes; every old member kept); add `force_internal` pak_slot_b1, pak_slot_b5, pak_slot_b6 (non-static
helpers, inlined, caller-less) unless the group rule already covers them. No data-symbol changes (the body
declares D_80146198 as `Ghost **[]` and D_80156CF0 as a 16-byte struct array; declarations only).

## func_80102448 (stunt records table) — w27c's tie closed with F30's lever
w27c residual (16 rows): webs x = column*60 and `160 - width/2` tie, lower web number takes s0, retail the other way.
Same cause as F30 (x numbered at read time, left only after copy propagation). Fixes: `text = value` after each
sprintf (16 → 12 rows), then `left = 60;` before the loops and `x = column*left;` in the fill loop (H.c; the same
with the `previous` slot renamed `cell`, G.c, also EQUAL) → `EQUAL func_80102448: 334 words`. 2448's frame was
exact, so the cell width reuses an existing local instead of a new one (disclosed variable reuse).
`x = column; x *= 60;` and `width = 60; x = column*width; width *= columns;` do NOT work (folded at read time).

## func_80100E58 (race standings, GC drawRaceStandings) — draft, 267 → 180 rows
`func_80100E58/best.c` (family file; E58 body). Levers that worked: `*(volatile s16 *)&D_801543CA == 1` (retail
reads it through an address register; head 0x00–0x84 now identical; disclose as a volatile read or declare the
global `extern volatile`), `text = value` after the sprintf, `total = 160 - columns*60/2` named (retail adds
`(left + x) + 30`). Not working: the cell-width lever via `other`/`total` (no change), continue forms. Residual:
the cell tail — retail branches past a `columns` reload (`beqzl s5,…; addiu s5,s5,1`) and reloads columns again
for `columns*60` (`lw t9,464(sp)`), ours CSEs one load; plus the x/left/row colours (row in v1 vs a0). Next: find the
source form that keeps two separate `columns` loads (e.g. the loop bound read through another variable/`rows`
style local, or `column*60 - columns*60/2 + 190` written with a named temp), then force.sh on the colours.

## func_80102980 (battle head-to-head matrix, GC drawBattleScore?) — draft, 325 → 102 words
`func_80102980/best.c` (family file). Levers: `text = value` after the sprintf (176 → 132 rows); the pre-loop font
site as `gfx_lock(); previous = slot_state_setup(11); osSendMesg(…); if (previous) {}` — one inlined helper level
= retail's missing 8 B below `label` (frame 472 → 480, every sp offset right); `total = 160 - columns*60/2`
named in both header blocks (body 362 → 364 = retail length); `other = 60; x = column*other` in the fill loop
(fill-loop colours right); four-line colour inits in the switch arms. All 24 orders of the header statements
(dispatch/text/left/x) measured: none better. Residual: header-block schedule (retail computes &input_rec0[col]
before column*60 and left) and the remaining colours. Next: ctrace the header webs, then force.sh.

## func_80103D28 (GC `DrawUnlocked`, highscorescr.c) — complete draft, provisional at best
`func_80103D28/best.c`. Per player p with D_80142694[p] (gStuffUnlocked): stuffCount = 3 + items whose "now"
flag is set and "before" flag clear over seven categories (19 tracks D_80150DD8/D_801426A0, 13 cars
D_80150E30/D_801426F0, then 4/8/9/5/6 entries: D_80150E88/D_80142750, D_80150EB8/D_80142780, D_80150ED8/D_80142AD8,
D_80150F00/D_80142D78, D_80150F40/D_80142DC0), resize the name pane D_80149418[p] around the list
(crowd_cheer_play = ChangePaneSize, rect = D_801148E8[gNumWindows-1][p], centre at Rect+8/+10), draw fixed[55],
fixed[56] (colour 1), then colour 22 and one centred line per new item (track titles D_8011A8D8, car titles
D_80110D70, the other five via `fcvt_wrapper(str, fmt, strings[indices->h[k] + i], …)`). Kept-mode unit score
(the only measurable form): `FAIL func_80103D28: 619 of 629 words differ; compiled body is 631 words, target
629 | words 573 ops 103 | frame 264/232` — the ops rows are the 9 callee saves/restores (retail is internal and
saves nothing), x/y narrowing copies and spill homes; control flow and every call's arguments line up.
Measuring it internal failed: umerge inlined it into every C stand-in tried (1, 2, 6 sites; blob_unit's dead
`if (0)` block inside it; a 600-byte stand-in frame; stand-in defined first). In retail it stays out of line in
func_80105480, so test it together with a draft of that caller. Next: draft func_80105480 (needs the
unregistered head func_80104704 in the inventory first, refresh-game-context), then score both internal.

## Survey of the rest of the pool (not started)
- func_80105480 (1,780; weak GC `DrawScores`): calls 3D28, 4B14, func_800BE744 and the unregistered head
  func_80104704 (highscore_entry_anim is its tail) — inventory refresh first.
- func_80104B14 (2,404; internal: unsaved s0,s1,s3–s8; weak GC `_PrintScores` via "%01d:%02d.%03d").
- func_80105EA8 (2,508; GC `animResults` [hud.c] 2.8 via "%s\n%s"; IPA register param t9, component 104).
- **Wrong-label leads for w29n**: game_results_update (0x800FEE04) and game_results_render pair with GC
  audioscr.c animButtonText/animText (string table "Flier", "Garage", "Night" = music track names, floats 2.79253
  and 0.349066) — they are the audio-options screen, not results; game_results_exit and func_80100564 pair with
  controlscr.c (animButtonText/animControls, "CONTROLLER %d %s").

## What generalises
- **Constant/address register candidates are numbered by the first block that holds them** (pretrace
  `makelivranges`, block order), and ties at equal save go to the lower number. A `continue` that jumps forward
  to the loop-test block early renumbers the loop bound (221C), and a compiled-out block between a constant's
  first use and the loop lengthens only that web and lowers its save (F0F44, save = tot/nocs).
- **Copy-propagated expressions are numbered after everything written literally**: `x = column*60` is entered at
  read time, `160 - columns*60/2` only after propagation. Writing the multiplier through a variable
  (`cell = 60; x = column*cell;`) renumbers x after the other webs — this closed the x/left swap in F30 and
  2448 (the "x = column*60 vs 160 - width/2" tie w27c saw in all three tables). `x *= 60` does not (folded early).
- **Inline-area size is a property of the helper shape, not the local count**: lock+set+unlock helper = 24 B per
  site, one nested level = 16 B (or 8 B for a single inlined lock at a written-out site), identical code; mixing
  the two shapes reproduced F30's 640-B frame and the 8 B missing in 2980.
- **GC statement order matters for as1**: `sprintf(str1, …); str = str1;` (assignment after the call) puts
  `addiu sN,sp,…` in the first call's delay slot as retail (F30, 2448, 2980, E58).
- A final "skip jumps past a reload" row = the cell-skip written as `if (row == 0 && column == 0) continue;`.
- `*(volatile s16 *)&G` (or `extern volatile`) explains a global read through an address register (E58's head).
