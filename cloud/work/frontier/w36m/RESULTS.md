# w36m RESULTS (wave 36, game code, results/hud.c 0x801xxxxx)

Builder scratch: watchman2:~/rush2049/scratch/frontier/w36m (from base, master e68aea6e1).

| function | GC identity | bytes | state | best file |
|---|---|---|---|---|
| func_80100E58 | drawRaceStandings@gameoverscr.c (CONFIRMED: DRONE, single[208]/[210]/[22], gRankedPlayer colours) | 2732 | 537/683 (words 189, ops 19, frame exact) typed port; w33a draft 534/683 (180 rows) | f0e58/best_port.c |
| animResults (0x80105EA8) | animResults@hud.c (confirmed w31a-w33a) | 2508 | **MATCH in group** (unit EQUAL, from w33a 150/627) | groups/credits_scroll_grp/gr3_5ea8.c (= ar/animResults.c) |
| highscore_animText (0x80105480) | animText@highscorescr.c (confirmed w30a/w33a) | 1780 | not advanced: 27/445 frame-only (w33a hs/best.c, installed as group context) | src/blob/groups/highscore_anim/highscore.c |
| func_80109F54 | animFlag@hud.c (CONFIRMED: Hidden x2, gCheckpoints finishPt, "%d" laps, animID%5) | 1504 | **MATCH in group** (unit EQUAL; `score.py group` MATCH, own .rodata verified) | groups/credits_scroll_grp/gr3_9f54.c (= flag/animFlag.c) |
| func_801084D4 | animLapTimeText@hud.c (CONFIRMED: address between animLapTime and animWrongWay; body = GC) | 1500 | **MATCH in group** (unit EQUAL, no devices) | groups/credits_scroll_grp/gr3_84d4.c (= laptext/animLapTimeText.c) |
| func_80106874 | NOT animTach: N64-only hud.c title pane (companion of locked func_80106B3C; GC CheckResults pane idiom) | 712 | **MATCH in group** (unit EQUAL) | groups/credits_scroll_grp/gr3_6874.c (= pane/func_80106874.c) |

## Log
- session start 2026-10-20: scratch created, RESULTS.md created.
- func_80109F54 = animFlag: a1 (GC port) 291/376, frame 128/136. Levers: N64 has two extra named locals for the
  map-centre offsets (`xoff = (kMapSize - 8 - w) / 2 + 4; yoff = ...;` computed before mapPos: frame 136 exact,
  49 words); operand order `(pos - min) * w / width` and `gFrameSeconds * 1000.0f`; TB_SetJust stores .h then .v
  (2 words); multu operand order `height` first needs the variable side to be an expression: `(0 + height) *
  (kMapSize - 8)` -> `EQUAL func_80109F54: 376 words (kept, c_d4.c)`.
- func_801084D4 = animLapTimeText: a1 (GC port + 761C sibling helpers) 55 rows, frame 232/224. Levers: unused
  `s32 arg0` parameter (retail stores a0 to its home, as the sibling animElapsedTimerText); `remaining` assigned
  AFTER the `<= 0 / place_locked` continue test (retail's `mov.s $f2,$f0`; that also removes the 8-B frame excess);
  the loop's zero spelled `0.f` (compare + clear) so it is a different constant from `render_helper(0.0f)`
  (retail: $f20 = 0.f hoisted, render_helper's 0.0f rematerialised `mtc1 zero,$f12`): `EQUAL func_801084D4: 375 words`.

- func_80106874: N64-only (GC hud.c has nothing between animResults and animOdometer = locked func_80106D94);
  companion of locked func_80106B3C (same StrTable +0x388 text, font 11/12 limit test, D_80144030[ctl].b6). a1 51 words
  colour-only (ops 0, frame exact); the b6 test through an inlined getter (w29b lever) -> `EQUAL func_80106874: 178 words`.

- animResults: w33a `ar/best.c` re-scored in the current unit = 150/627 (words 228 ops 15). Levers (ar/v/):
  a1 ar_font_set defined after the caller (as1 .loc tie, w29b lever): ops 9; b1 block-scoped pane heights `h`;
  c1..c8 GC block scoping (str1/str2/place/min/sec/thou in the default block) + time statements `min = (u32)t;
  min = min / 60 % 100; sec = (u32)t % 60; thou = (u32)(1000.0f * t); thou %= 1000;` -> 53/627 (words 65, ops 2);
  d1 GC's two extra TB_Sprintf varargs on the laps text (`model[..].laps, gNumLaps`; retail keeps the compare
  operands in a2/a3) -> `EQUAL animResults: 627 words (kept, c_animResults.c)` / `locked bodies that differ in this
  unit: 0` (unit args `--internal ar_font_set --internal ar_SetJust --internal ar_SetFont`).

### Packet: superseding group `credits_scroll_grp` (cloud/work/frontier/w36m/groups/credits_scroll_grp/)
Both functions call slot_state_setup with its private-IPA register parameter (s2), so they match only in the unit /
a group with slot_state_setup as member (standalone `score.py fn`: 247/375 differ). Superseded IN PLACE: every old
file byte-identical (copied from src/blob/groups/credits_scroll_grp/), group.json = old + members/files/keep
func_801084D4 (gr3_84d4.c), func_80109F54 (gr3_9f54.c), func_80106874 (gr3_6874.c), `claims` = [func_801084D4,
func_80109F54, func_80106874] (3,716 B).
Unit (Pi): `python3 -m tools.conveyor.pipeline.blob_unit --tag w36m score func_801084D4 func_80109F54 --with
cloud/work/frontier/w36m/laptext/animLapTimeText.c --with cloud/work/frontier/w36m/flag/animFlag.c --neighbours`
```
  EQUAL func_801084D4: 375 words (kept, c_animLapTimeText.c)
  EQUAL func_80109F54: 376 words (kept, c_animFlag.c)
  locked bodies that differ in this unit: 0
```
Builder `score.py group cand/credits_scroll_grp` (without --claims): all 12 members `MATCH` (credits_scroll,
func_80100B8C, slot_state_setup, world_trigger_check, func_8010A53C, func_80107EDC, animLapTime,
animElapsedTimerText, animPlace, animWrongWay, func_801084D4, func_80109F54), rc=0. With `--claims`:
```
Members:
func_801084D4:
  MATCH
func_80109F54:
  MATCH
    own .rodata verified at 0x80120E48..0x80120E50
```
Disclosed device (gr3_9f54.c): `(0 + width)` / `(0 + height)` on the variable side of the two map-aspect multiplies
(no code; uopt puts a plain variable second in a commutative op, retail has the variable first: `multu t2,a0`).
Tried and rejected natural forms: `height * (K-8)`, `(K-8) * height`, `(s32)` casts, `-8 + K`, declaration orders
(2 words), `w = K - 8; h = height * w / width` (structure changes: 320 words), a temp `t` (25-38 words), inline
`(gPathMax[2] - gPathMin[2])` (321 words). xoff/yoff are N64-only named locals (frame 136 proves two more slots
than GC's six; the evaluation order proves they are computed before mapPos).
Final group run (3 claims, builder): `score.py group cand/credits_scroll_grp --claims` -> func_801084D4 MATCH,
func_80109F54 MATCH (`own .rodata verified at 0x80120E48..0x80120E50`), func_80106874 MATCH, rc=0; without --claims
all 13 members MATCH (old members unchanged: credits_scroll ... animWrongWay all MATCH), rc=0. Unit with all three:
`EQUAL func_801084D4: 375 words` / `EQUAL func_80109F54: 376 words` / `EQUAL func_80106874: 178 words` /
`locked bodies that differ in this unit: 0`.
Group run with animResults added (4 claims, 6,224 B): `--claims` -> func_801084D4 MATCH, func_80109F54 MATCH (own
.rodata verified at 0x80120E48..0x80120E50), func_80106874 MATCH, animResults MATCH (`own .rodata verified at
0x80120DBC..0x80120DC4`, `0x80120DC8..0x80120DEC`, `0x80120DF0..0x80120E08`), rc=0; without --claims all 14 members
MATCH, rc=0.

## func_80100E58 (drawRaceStandings) — fresh MAT3 typed port (in progress)
Baseline (w33a f0e58/best.c in current unit): `534 of 683 words differ; compiled body is 682 words` (words 180 ops 9).
Typed port (f0e58/v/p1..p8; the whole results-table file with only func_80100E58 replaced; uargs = w33a's +
`--internal rs_DrawEllipseText --internal rs_SetJust`):
- p1 GC verbatim (block locals, inlined TB_DrawEllipseText(s16 x, s16 y, s16 w, char *) with a buffer, TB_SetJust
  helper, `volatile s16 gNumCars` as MAT3): 630/683, frame 504.
- N64 layout facts: retail anchors str T-16, str1[40] T-56, rows T-60, cols T-64, row T-68, c[4] T-84, ellipse buffer
  at sp+208. p2: function-level `x, y, bgY` before str, `rows, cols` / `row, col` pairs, two f32 `time` slots before c:
  every top anchor exact (frame 496). p6: TB_DrawEllipseText buffer `char str[160]` -> frame 528 exact with the buffer
  at sp+208 (every frame anchor matches retail). p7: loop 1 computes x and y inside the inner loop before the
  `row == 0 && col == 0` continue (as GC loop 2) -> loop 1 ops-exact. p8: `time * 1000.0f`.
- p8: `537 of 683 words differ; compiled body is 682 words` (words 189, ops 19, frame 528/528). NEW vs w33a: the
  cols live-range split now happens (w30a's blocker), with the loop piece in v1 (retail v0): ctrace `p1 w325 cols
  piece -> v1`; force `p1:w325=c1` is declined (v0 forbidden: the constant-60 web w289 in the final draw block
  interferes because our piece extends into the final `state_utility(x + 30, y + 7, str)` block; retail reloads cols
  from memory there, `lw t9,464(sp)` after `li v0,60`).
- Split oracle (bin/uopt_spl copied from w25d, w16d patch_spl.py; `w36m/tools/oracle.sh`, `tools/splvar.sh`): the
  text-loop cols piece (`split lr=325 sym=15 start bb=39`) rejects the column==0 header block exactly as w30a found:
  `adj bb=45 regsleft=12 old=19 shared=4 numintf=21` -> reject (needs `2*regsleft >= numintf + shared`, i.e.
  numintf <= 20). **The typed port cut the gap from w30a's 3 interferers (numintf 23) to 1 (21).** bb110 (the final
  draw block) is then added (`regsleft=14 shared=1 numintf=23`), so the piece meets the constant-60 web and takes v1.
  Variants that leave numintf at 21 (diagnostic): loop-2 x/y as block locals (q1), no `rows` variable (q2), `str = 0`
  before the text loop (x1), no font call in the row==0 block (x2).
- Next for func_80100E58: find the one extra live range at the text-loop head (bb39..44) — an oracle that prints the
  piece's interference list at bb45 (extend patch_spl.py at L46e2e0 to walk the lr's interference bit vector), then
  the source form that removes it. Best file f0e58/best_port.c (= v/p8.c, the whole results-table file with
  func_80100E58 replaced; unit args = w33a f0e58/uargs.txt + `--internal rs_DrawEllipseText --internal rs_SetJust`).
  Its N64 layout facts (exact frame 528, all anchors) are solid even though the 3 top locals (x, y, bgY) and the two
  f32 slots (time, time2) are layout-only names: `bgY` and `time2` are unused (exact frame residual, 2 slots).
- Interference oracle (NEW, `w36m/tools/patch_spl2.py` = w16d patch_spl.py + an `[SPLI]` dump of the old live range's
  interference list at every adjacency decision; built as watchman2:~/rush2049/scratch/frontier/w36m/bin/uopt_spl2;
  run `UOPTB=uopt_spl2 w36m/tools/oracle.sh LABEL func_80100E58 "SPLI=1"`). At bb45 the cols piece has counted
  (`*`) 21 interferers: col (lr24, s5), str (lr123, s6), &D_801461D0 (lr282, s7), 12 more undecided global-address
  webs (type 1: lr272, 281, 284, 285, 293, 296, 299, 304-307), and the undecided constants 1 (piece 316), 3 (piece
  324), 76, 160, 14, 2056 (lr286/288/290/308), plus one null node; bb45 adds the four header-block webs s1/s3/s4/s8
  (lr34, 269, 64, 33) as `shared`. w33a's draft counts 23 with the same structure. So the missing lever removes one
  loop-spanning constant/address live range (or keeps one of s1/s3/s4/s8 uncoloured until after the cols split, which
  would give regsleft 13). Not closed.

## highscore_animText — not advanced
Reviewed w33a Facts A-C and the retail layout again (DS label at sp+136, str[96] at sp+392, i/numWid at 492/488, the
times-loop font site's `old` stored at sp+124 because slot_state_setup clobbers s0 there): the DS/DCC areas have no
nested helper areas in retail and one 8-B helper area sits right below the label, i.e. the helper sites inside DS were
inlined into animText AFTER DS (areas appended at the caller's end) -- a umerge processing-order effect that w33a
showed no source shape reproduces in our unit. No new variant run; it stays the installed EQUIVALENT context draft.

## Identities proven (for w36n / w36y)
| N64 | MAT3 / GC | evidence |
|---|---|---|
| func_80109F54 | animFlag@hud.c | GC body: two Hidden tests, gCheckpoints.points[finishPt].pos[0/2] scaled into the map, "%d" laps-left digit with shadow, animDTA -= gFrameSeconds*1000, animID = (animID+1) % 5, SelectBlit(blt, animID, width, width); unit + group MATCH |
| func_801084D4 | animLapTimeText@hud.c | address between animLapTime (0x80108154) and animWrongWay (0x80108AB0) = GC order; body = GC (digits loop, gCheckPtTimeout fade, colour 22); unit + group MATCH |
| func_80106874 | (none: N64-only hud.c Blit AnimFunc) | between animResults and animOdometer (= locked func_80106D94); companion of locked func_80106B3C; GC CheckResults pane idiom; MATCH |
| D_80152014 | gNumLapsLeft (s8) | animFlag |
| D_80156BDC | gShowTrackMap (s8) | animFlag (also locked animTrackMap) |
| D_801407B4 / D_801407D4 | gPathMax / gPathMin (s16[3]) | animFlag |
| D_801161C4 | kMapSize (int) | animFlag |
| D_801160A8 | mapPos[4][2] (int) | animFlag |
| D_80140A04 | gMirrorWaves (s8) | animFlag |
| D_80151CE8 | gCheckpoints (finishPt +4, points[] +0xC, 80 B each) | animFlag |
| D_8002EB94 | gFrameSeconds (volatile float) | animFlag, animLapTimeText (per-use lui/addiu + lwc1 0(reg)) |
| D_801170FC | gPauseState | animFlag, animLapTimeText, func_80106874 |
| D_80142740 / D_80142770 | gCheckPtTimeout[4] / gLapTime[4] (float) | animLapTimeText (and locked animLapTime) |
| D_80115EA8 | lapTimePos[4][4][2] | animLapTimeText |
| D_80154350 | resultPane[5] (GC hud.c `static tBlitSet *resultPane[5]`) | animResults ([4] = header pane) and func_80106874 ([i]) |
| ambient_sound_set / tournament_unlock_check / sound_handles_array_clear | InitPane / ChangePaneColor / RemovePane | animResults, func_80106874 (GC CheckResults usage) |
| object_manager_update / object_bytes_sum_global | HAL_GetTextWidth / TB_GetFontLineHeight | func_80106874 |
| music_tempo_adjust | TB_Printf(s16 x, s16 y, fmt, ...) | animFlag |

## What generalises
- **The GC typed port closes big hud/results functions in one or two lever rounds** (4 of 6 here, 6,224 B): GC block
  scoping of locals (animResults str1/str2/place/min/sec/thou and the pane heights), GC statement splits
  (`thou %= 1000;` separate), and GC's "extra vararg" calls (`TB_Sprintf(str2, fmt, laps, gNumLaps)`: retail keeps the
  compare operands in a2/a3) were each worth tens of rows.
- **Define-after-use for the lock/font helper** (w29b lever) is general for the TB_SetJust-store vs osRecvMesg-argument
  as1 tie (animResults ops 15 -> 9).
- **N64 versions add named locals where GC has inline expressions** (animFlag xoff/yoff: frame +8 and evaluation
  order) and drop GC terms; the frame + statement order show it.
- **Operand order of commutative multiplies follows variable-vs-expression kind**: a plain variable is put second; a
  retail `multu var, expr` needs the variable side to be an expression (`(0 + height)`, disclosed device).
- **Float zero spelling separates hoisted constants**: `0.f` in a loop vs `0.0f` in render_helper(0.0f) gives retail's
  one hoisted loop zero and a rematerialised `mtc1 zero,$f12` (animLapTimeText).
- **Split-oracle interference dump** (`w36m/tools/patch_spl2.py`): prints which live ranges count against a split
  piece's growth (f_addadjacents), turning "remove N interferers" into a named list.
