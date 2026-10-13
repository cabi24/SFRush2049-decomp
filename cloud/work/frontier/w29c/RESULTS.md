# w29c — camera / race end / attract handlers (wave 29)

Status: FINAL. Branch wave29 (main checkout), nothing committed/spliced. Builder scratch
watchman2:~/rush2049/scratch/frontier/w29c (toolkit reused from wtk + debug as1 from w12b/tools/build_as1dbg.sh)
deleted at the end. Flags `-g0 -O3 -mips2 -G 0 -non_shared` (IDO 5.3). Unit = `blob_unit --tag w29c`
(trace toolkit `us.sh`). No permission denials.

**Landable: 5 functions, 5,644 B** — func_800E56F8 + func_800E6AF8 (group packet, 2,772 B), lap_complete (packet
superseding credits_scroll_grp, 1,232 B), func_800EB028 (cloud/matches single, 1,640 B).

### Integration notes (exact)
1. `groups/func_800E56F8/` — NEW group `func_800E56F8`: members/claims func_800E56F8, func_800E6AF8; keep
   func_800E6AF8. unit_overrides `force_internal`: func_800E56F8, update_drone_models, update_model (the last two
   are non-static inlined helpers, not members, not keep; they leave no stub — locked stubs func_800E6AF0 and
   func_800E7030 untouched). No data-symbol changes. Devices disclosed in group.c (`^ 0`, one `do {} while (0);`,
   unused locals curtime/n/result as exact frame residual, one-line `rec = input_rec0; do {`).
2. `groups/credits_scroll_grp/` — supersedes the locked group IN PLACE (same name, all 7 old members, all old
   files byte-identical, context unchanged) + `gr3_761c.c` (lap_complete). members/keep += lap_complete; claims
   [lap_complete]. Helpers gfx_lock_761C/gfx_unlock_761C/font_set_761C/pair_set_761C non-static, not on keep (as
   the *_EDC helpers; add force_internal only if the shadow unit reports them). No devices.
3. `cloud/matches/func_800EB028.c` — plain single (splice_singles); own .rodata verified by score.py. No devices.

| Function | Bytes | Before (w28d) | After | Verdict | Residual |
|---|---:|---|---|---|---|
| func_800E56F8 | 1,436 | 4/359, rows 3 | **group MATCH** | unit EQUAL, `score.py group` MATCH | — |
| func_800E6AF8 | 1,336 | 117/334, rows 106, frame 128/160 | **group MATCH** | unit EQUAL, `score.py group` MATCH | — |
| race_finish | 1,000 | 186/250, rows 100, ops 26 | 40/250, rows 40, ops 6, frame exact (1 device) | **EQUIVALENT 18,271 calls / 6 scripts** | colour/schedule |
| **lap_complete** | 1,232 | 288/308, rows 111 (w27b) | **group MATCH** (credits_scroll_grp + gr3_761c.c), no devices | unit EQUAL | — |
| func_800E543C | 700 | 152/175, rows 180, ops 24 | unchanged (w28d best stands) | w28d: EQUIVALENT 122 calls | 3 missing words = index split stores |
| **func_800EB028** | 1,640 | 163/410, rows 175 (w27b) | **MATCH** standalone (`cloud/matches/func_800EB028.c`), no devices | unit EQUAL | — |
| control_settings | 2,552 | untouched (m2c context in codex_control_settings_a4) | first draft: 603/638, ops 164, frame 1904/1896 | never called by any script | logic drafted; row/col not register-allocated |
| attract_mode_handler | 264 | EQUAL with stand-in | **EQUAL with the control_settings draft as caller** (still provisional) | never called | — |
| attract_video_handler | 692 | rows 17 | rows 17, ops 2 (with real caller draft) | never called | as w28d |
| attract_demo_handler | 820 | rows 35 | rows 35, ops 5 (with real caller draft) | never called | as w28d |

## func_800E56F8 + func_800E6AF8 — group `groups/func_800E56F8/` (READY TO INSTALL)

```
python3 -m tools.conveyor.pipeline.blob_unit --tag w29c score func_800E56F8 func_800E6AF8 \
  --with cloud/work/frontier/w29c/groups/func_800E56F8/group.c --internal func_800E56F8 \
  --internal update_drone_models --internal update_model --neighbours
  EQUAL func_800E56F8: 359 words (internal, c_group.c)
  EQUAL func_800E6AF8: 334 words (kept, c_group.c)
  locked bodies that differ in this unit: 0
blob_unit score: 2/2 equal
ssh watchman2 '... python3 tools/cloud/score.py group cand/func_800E56F8'
func_800E56F8:
  MATCH
func_800E6AF8:
  MATCH
```
Integration: new group `func_800E56F8` (members/claims func_800E56F8, func_800E6AF8; keep func_800E6AF8).
unit_overrides `force_internal`: func_800E56F8 (IPA s4 parameter; only caller func_800E6AF8), update_drone_models,
update_model (non-static inlined helpers, not members, not keep, leave no stub; the locked stubs func_800E6AF0 and
func_800E7030 stay untouched). No data-symbol changes. func_800E543C (callee of E56F8, unmatched) stays separate.

**E56F8 last 3 rows (as1, traced):** retail hoists `lui t7,%hi(D_8013FECB)` from the join block two diamonds down
into the `lb 2027(s0)` load-delay slot; ours stopped one block lower. Debug as1 (`w12b/tools/build_as1dbg.sh`,
AS1DBG=8) showed the `lb` block had xbb weight 65534 (never a MOVETO target), because the early-return arm ended
`b $exit` followed by six `.alias` lines (address registers of D_80153F40/D_8015274C/D_80153F08 still noalias at
the `b`) — the w12b pattern. Deleting any subset short of all six from the listing kept 3 rows; deleting all six
gave 0. Source lever: a compiled-out `do {} while (0);` after the last store before `return;` closes the regions
before the `b` (`goto ret; ret:;` also works). Disclosed device; the `^ 0` device stays (without it 1 word).

**E6AF8 (106 rows → 0):** it is arcade mdrive.c `model_iteration()` — the drone part is `update_drone_models()`
inlined (retail: `drone_sched[sched_to_use][++sched_index] == -1` with the incremented s16 kept in a register,
`if (num == 0) return;`, `S16 i` loop narrowed with sll/sra, `num` homed at 100(sp)) with a nested inlined
`update_model(car)`. Levers in order: drone part as an inlined helper (106 → 24); update_model as a nested inlined
helper with one more local (bottom +8: i/j/r/t homes 124..112 as retail); two named locals before `i` (top +8,
frame 160); speed loop `fz = vel[2]` assigned first (FP webs: fz→$f2, fx→$f12, fy→$f14); `car->dt = t;` before
`car->lastTick += 1;` (retail loads `t` above the lastTick store; as1 reorders the two s7 stores); `rec =
input_rec0; do {` on one line; drone helper defined before func_800E6AF8 and update_model after it (the reload of
`num` then carries a later `.loc` and wins the bnez delay slot; this matches the locked stub positions
func_800E6AF0 before / func_800E7030 after).
Names (arcade mdrive.c, recorded for reconciliation): D_80120E74 = drone_sched (6-byte rows), D_80153E84 =
sched_to_use, D_80153F24 = sched_index, D_80152808 = our_drones, D_80153F40 = cur_drone, D_8015274C = drone count
(wrap bound), D_80153F08 = next schedule; GC: no correlation candidate for E6AF8 (E56F8: weak `_UpdateModel`/`GhostUpdate`).
Devices: see group.c header (`^ 0`, `do {} while (0);`, 3 unused locals as exact frame residual with arithmetic,
one-line `rec = input_rec0; do {`).

## lap_complete — MATCH via `groups/credits_scroll_grp/` (READY TO INSTALL; supersedes the locked group in place)
lap_complete = GC hud.c `animElapsedTimerText` (static int (void); locals t, digits[9], str[2], digitW, colonW,
fixedWidth, x, y, digitIdx). GC-derived names (recorded): D_801543CC = gModelTime, D_8015723C = gShowElapsed,
D_80115C88 = elapsedTimerPos, object_byte9_set = HAL_SetFontFixedWidth, camera_shake_update = HAL_GetCharWidth,
render_helper = HAL_SetTextDepth, D_80118E20 = TB_SetJust's (just, ?) pair, dispatch_handler = TB_SetTextColor,
state_utility = TB_DrawText (both coordinates narrowed: prototype (s16, s16, char *), as gr3_a53c.c).
```
python3 -m tools.conveyor.pipeline.blob_unit --tag w29c score lap_complete \
  --with cloud/work/frontier/w29c/groups/credits_scroll_grp/gr3_761c.c --neighbours
  EQUAL lap_complete: 308 words (kept, c_gr3_761c.c)
  locked bodies that differ in this unit: 0
ssh watchman2 '... python3 tools/cloud/score.py group cand/credits_scroll_grp'   (no --claims)
  credits_scroll / func_80100B8C / slot_state_setup / world_trigger_check / func_8010A53C / func_80107EDC /
  func_80108154 / lap_complete: MATCH (context informational as before)
```
`score.py fn` standalone: 159/308 (needs the unit: slot_state_setup takes s2) → group, not cloud/matches.
Steps 108 → 79 → 0 rows: GC local list and statement forms (index loop, frame 176 → 184 exact) → state_utility
(s16, s16) + s32 y (retail's `lw; sll; sra` y) → compare through `(str[0] = digits[digitIdx]) != ':'` (ops 9) →
`str[1] = 0;` first (ops 5) → **the TB_SetJust pair**: an inlined `pair_set(s32 first, s32 second)` storing
`.second` then `.first` on an s32 pair (the matched sibling func_80107EDC writes `.b = 3; .a = 1;`): the s32 1
now joins the `D_80151AD0 == 1`/osRecvMesg constant piece in v0 (u32 parameters kept it a separate constant, the
cause of the "one register ahead" temp ring in both lap_complete and race_finish) → `y` assigned before `x` (as1
line order of the address and `digitW * 5`): EQUAL.
Packet: `groups/credits_scroll_grp/` = the locked group's files unchanged + `gr3_761c.c`; group.json members +=
lap_complete, keep += lap_complete, files += gr3_761c.c, claims [lap_complete]. Helpers gfx_lock_761C,
gfx_unlock_761C, font_set_761C, pair_set_761C are non-static and not kept (w17j; no stubs). No unit_overrides
expected (as for *_EDC); if the shadow unit complains, force_internal those four names. No data-symbol changes.

## race_finish (`race_finish/best.c`) — GC hud.c `animPlace`, 40 rows (from 100)
GC name/locals (recorded): race_finish = GC `static int animPlace(void)` (hud.c, locals digits[3], str[2],
digitW, x, y, digitIdx, slot); func_800CF604 = AreWeSolid, D_80115CA8 = placePos, D_80116168 = playerColor,
D_8016137C = gShowPlace. GC declaration order alone changes nothing.
`FAIL race_finish: 40 of 250 words differ | words 40 ops 6 norm 6 | frame 280/280` (us.sh, unit).
Levers: (1) lap_complete's TB_SetJust lever: inlined `pair_set(s32, s32)` with a `Pair *p` local storing
`.second` then `.first` on an s32 pair → length 250 (was 251) and the early v0 constant piece as retail
(without the `Pair *p` local the frame is 272); (2) state_utility(s16, s16, char *); (3) **device (disclosed)**
compiled-out `if (i) {}` at the end of the outer loop body: ctrace showed the int-1 web (save 14.07) taking s8
and `i` (11.8) splitting; with the device i is in s8 as in retail (without it 88 rows). Placement in the inner
loop swaps s7/s8.
Residual (colour/schedule, 40 words): player_array[i] address in v1 (retail a0), y in a0 (retail v1), outer block
loads x before y (retail y, place, x, w), spill homes of (s16)(y+1)/(s16)y 72/68 (retail 76/72: one more spill
temp or one fewer named home before them). Statement permutations of y/x/place: 40–51 rows; y as an expression:
worse. force.sh swapping the two v1/a0 expression webs (w68/w60): 42 rows, so not that pair. Next: spcensus.sh
on the homes (which extra coloured expression web precedes (s16)(y+1) in retail). draft_check: w28d body EQUIVALENT
17,029 calls). Re-run on best.c: `VERDICT: EQUIVALENT over 18271 calls / 6 scripts (pause_menus 3736,
two_player_finish 3390, ghost_save 3268, race_finish 3218, player_finish 2633, backward 2026)`.

## func_800E543C (no new best; w28d/func_800E543C/best.c stands)
The 3 missing words are exactly retail's spill code for `index` (t0): `sh t0,78(sp)` before func_800C4F68 and
before stunt_combo_display, `lh t0,78(sp)` after func_800C4F68 (78 = the s16 param home). So retail's `index`
live range reaches the stunt_combo_display block (stored there, never reloaded): some occurrence of `index`
survives PRE in that block. ctrace: ours `index` = w1, 5 blocks, a2; `action` (w6) splits to memory in both.
Tried, no effect: `set_cam(index, cam)` helper form; dead reads `if (index) {}` before/after the stunt call
(removed); an inlined `clear_car(index)` / `do_stunt(index)` helper after the call (frame 80, still no spill).
Next: pretrace.sh on `index` occurrences in the stunt block (which expression keeps it), or a helper whose
parameter copy survives (the caller-less stub func_800E5434 still says one inlined static with 4 sites).

## func_800EB028 — MATCH (`cloud/matches/func_800EB028.c`)
```
ssh watchman2 '... python3 tools/cloud/score.py fn cand/func_800EB028.c func_800EB028 --flags "-g0 -O3 -mips2 -G 0 -non_shared"'
func_800EB028:
  MATCH
    own .rodata verified at 0x8012451C..0x80124558
python3 -m tools.conveyor.pipeline.blob_unit --tag w29c score func_800EB028 --with cloud/matches/func_800EB028.c --neighbours
  EQUAL func_800EB028: 410 words (kept, c_func_800EB028.c)
  locked bodies that differ in this unit: 0
```
The named lever ("an existing f32 local to carry pos_out[1]") turned out to be the opposite: remove a local. GC
`update_camera` (camera.c) has locals i, j, pos_in, pos_out, m, camera (an **int index**), rear, view — no camera
pointer. With the w27b `Camera *cam` local replaced by the expression `D_80150B70[slot]` (macro CAM) the pointer's
home slot is gone and uopt promotes pos_out[1] into $f0 in cases 0/1 exactly as retail, with the frame still 96
(175 → 20 rows, ops 0, the rest operand order). `CAM.pos[i] = CAM.pos[i] + pos_out[i];` (not `+=`, and not
`pos_out[i] + CAM.pos[i]`, which is canonicalised) gives retail's add operand order → EQUAL. No devices.
Integration: plain single (splice_singles). Own .rodata (jump table + constants) verified by the scorer.
GC-derived: camera index naming from GC update_camera; arcade ancestor camera.c setcamview (w27b).

## control_settings + attract_mode/demo/video_handler (`control_settings/best.c`, draft; NOT installable)
Retail-read facts (all from the retail disassembly; no GC/arcade counterpart — GC correlation candidates are
weak size-only matches): control_settings(row, &sel1, &sel2, &done, poll) steps one row D_80153FD8[row] of a
two-cell modal dialog (44-byte Cell: done +0, sel1 +1, sel2 +2, state[4] +4, mode[4] +8, alt +0x10, type
+0x14, x/y/w/h s16 +0x18..+0x1E, buttons +0x20, held +0x24, timeout +0x28): the active cell is found col 1 → 0
(an idle unfinished cell finishes at once), the pad words D_8015694C/D_80156944 are latched, the row number is
sprintf'd (D_80121060) and centred, then a 15-way jump table on `type` (cases 1–15 as in the m2c context of
codex_control_settings_a4, now with real names) and the done tail publishes sel1/sel2 and applies the cell's
per-player state/mode (player_state_set/player_mode_set ×4, process_inputs). D_8017A4E0 = the language string
table {+4 msg[], +12 lang (u16 indices at +48/+50/+52), +16 strings[]} (same table as gr3_a53c.c/gr3_edc.c).
IPA conventions confirmed at the call sites: attract_mode_handler(row=s5, col=s6, text=s7);
attract_video_handler(sel=s5, row=s6, col=s7, a=sp+12, b=sp+16, text=s8) — the callee never reads `a`/`b`
(it reads msg[944/4] and strings[lang[25]] itself), so the prototype carries two unused stack parameters;
attract_demo_handler(sel1=s5, row=s6, text..s3 = sp+8..sp+20, col=s7, sel2=s8) = w28d's order;
func_800DD0C0 takes (row, col) in its caller's 0(sp)/4(sp).
Unit (all four in one file, handlers + helpers `--internal`): `EQUAL attract_mode_handler: 66 words (internal,
c_csD3.c)` with the real caller draft (w28d needed a stand-in); video rows 17 / demo rows 35 unchanged;
control_settings `603 of 638 words differ | rows 663 ops 164 | frame 1896/1904`.
Frame findings (measured): each nested font_set site (gfx_lock/gfx_unlock helpers) reserves 24 B, a
single-level one is not inlined at more than one site; with the lock calls written directly at the four sites
and pair_set inlined (u32 first/second, first stored first — the w28d form; this TU differs from the HUD TU) the
frame is 1904 (retail 1896); `c` and `&D_80153FD8[row]` are expression webs in retail (spill homes 88/92), not
named locals; named locals are col (home 1892), the sprintf buffer [76] and six text buffers [296, 328, 272,
264, 264, 264] in that order.
Residual: retail keeps row in s3 and col in s4 (saved to their homes around the IPA calls); ours splits both to
memory (ctrace: row web save 1.0 vs callee-saved cost 36.4). Next: find what raises row/col's weight (retail's
case bodies may reload `row`/`col` through the handler-call copies), then the 8-byte frame residual. All four
need to be one group with func_800DCD58/func_800DCCE0 (locked in codex_control_settings_a4, whose context copy
of control_settings this supersedes).

## What generalises
- **A pointer local can be the residual, not a missing variable**: func_800EB028 closed by deleting the
  `Camera *cam` local (GC keeps an int `camera` index) — the freed home let uopt promote pos_out[1] into $f0 as
  retail (175 → 0 rows). Before adding an f32 local to "carry" a value, try turning a named pointer into the
  indexed expression.
- **A `.alias` directive after the unconditional `b` of an early return** (address registers still noalias at the
  `b`) gives the following region xbb weight 65534 in as1 and blocks its cross-block hoist of a `lui` two blocks
  up (debug as1 AS1DBG=8: the blocked block never appears as a MOVETO target). A compiled-out `do {} while (0);`
  (or `goto ret; ret:;`) after the last store before `return;` closes the regions before the `b`.
- **Arcade inlined helpers explain frame and schedule**: func_800E6AF8 = arcade model_iteration with
  update_drone_models and a nested update_model inlined; definition order of the helpers (before/after the
  caller) changes the `.loc` lines as1 uses for tie-breaks and matches the locked stub positions.
- **The TB_SetJust pair type is per TU**: the HUD TU (lap_complete, race_finish, matched sibling func_80107EDC)
  stores an s32 pair `.second` then `.first` (the s32 1 joins the D_80151AD0 == 1 / osRecvMesg constant piece
  in v0); the dialog TU (attract_mode_handler) needs the locked u32 func_800B669C form.
- **IPA callees can carry unused stack parameters**: attract_video_handler's caller stores two strings at
  sp+12/sp+16 that the callee never reads.
- `x = x + E` vs `x += E` (w15e) closed func_800EB028's last rows; `pos_out + x` is canonicalised.
