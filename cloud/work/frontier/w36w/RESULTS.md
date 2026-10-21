# w36w results (wave 36, game pool: unassigned smaller game functions)

Lane added after launch. Master at start: a14da0b24. Identities from w36n map_functions.json (verified per
function below). All scores are strict; "rows" = aligned differing rows from `tools/trace/udiff.py`.

## Summary

Matched (game, all with blob_unit confirmation, 0 locked bodies differ): 8 functions, 3,468 B
- groups/credits_scroll_grp/ (superseded in place): game_timer_display 352, time_result_display 332, checkpoint_hit 580
- groups/engine_sound_sync/ (superseded in place): func_800988D8 380
- groups/func_800AD4C8/ (superseded in place, no source change): func_800AD4C8 264
- groups/w36w_demo_pair/ (new group): debug_collision 532, debug_stats 532
- cloud/matches/drone_target_update.c (standalone): drone_target_update 496
Not claimed: object_create 112 (group MATCH only when kept; keeping it breaks PrintScores in the unit).

## Status

| function | GC identity | bytes | state | best file |
|---|---|---|---|---|
| object_create | HAL_SetFont@HAL_text.c (proven, confirmed) | 112 | group MATCH when kept, but blocks shadow unit (PrintScores inline); NOT claimed | object_create/group_with_object_create.json |
| func_8010D3C0 | _StartWeaponIcon@targets.c (confirmed) | 704 | 23 rows (8 = own-rodata relocs; 15 = as1 xbb hoist, 2-word shift) | func_8010D3C0/best.c |
| func_8010D680 | _AnimateWeaponIcon@targets.c (confirmed) | 476 | 22 rows: op narrowed into a1 (coloured) vs t7, two as1 xbb hoists (+2 own-rodata reloc rows); frame now exact with one disclosed unused local | func_8010D680/best.c |
| race_position_update | TB_EllipseText@TB_text.c (confirmed) | 484 | 56-60 rows (loop constant -1 not hoisted, phantom s5; 7 words short) | race_position_update/best.c |
| object_bytes_sum_global | TB_GetFontLineHeight (map: TB_text.c) | 84 | 4 words: colour-only (force.sh p1:w21=c11 -> 0 rows) | text/best_object_bytes_sum_global.c |
| game_timer_display | animPanes@audioscr.c (NEW identity) | 352 | **group MATCH + unit EQUAL** (credits_scroll_grp superseded; 2 disclosed unused locals) | groups/credits_scroll_grp/ |
| time_result_display | _TitlePane@optionscr.c (proven, confirmed) | 332 | **group MATCH + unit EQUAL** (same superseded credits_scroll_grp; body unchanged, off keep, 2 disclosed stand-in callers) | groups/credits_scroll_grp/ |
| checkpoint_hit | animElapsedTimer@hud.c (proven, confirmed) | 580 | **group MATCH + unit EQUAL** (same superseded credits_scroll_grp, new file gr3_73d8.c; 4 disclosed unused locals) | groups/credits_scroll_grp/ |
| func_800988D8 | _ProcessSequences@audiomgr.c (strong, confirmed) | 380 | **group MATCH + unit EQUAL** (engine_sound_sync superseded; Astra #408 body + disclosed `state*1` identity) | groups/engine_sound_sync/ |
| func_800AD4C8 | VectorToSurfEdge@stree.c (proven) | 264 | **group MATCH + unit EQUAL** (group func_800AD4C8 superseded: context -> member, no source change) | groups/func_800AD4C8/ |
| menu_back | _DecodeGhost@save.c (proven, confirmed) | 132 | 3 words (as1 schedule of `move a1,v0` / `lw a0` delay slot) in menu_save_options | (installed ctx_slot.c body) |
| drone_target_update | SetControls@controls.c (lead -> confirmed) | 496 | **standalone MATCH + unit EQUAL** | cloud/matches/drone_target_update.c |
| debug_collision | updateDemoGame@demo.c (strong, confirmed) | 532 | **group MATCH + unit EQUAL** (new group w36w_demo_pair; Astra #413 + holder lever) | groups/w36w_demo_pair/ |
| debug_stats | playDemoGame@demo.c (map said uninitDemoGame/lead; #413: playDemoGame, confirmed) | 532 | **group MATCH + unit EQUAL** (same group) | groups/w36w_demo_pair/ |
| func_8010E8B4 | animPanes@videoscr.c (confirmed: second InitPane(20,136,300,204)) | 340 | 3 words (y narrowed into t0, retail ring temp t9) = w32b best; not improved | ../w32b/func_8010E8B4/best.c, func_8010E8B4/u3/k2.c |

## Notes per function

### object_create = HAL_SetFont (112 B): group MATCH, integration caveat
- Body (already present unchanged as context in src/blob/groups/credits_scroll_grp/gr3_b.c) is
  `osRecvMesg(&D_801461D0,0,1); old = slot_state_setup(fontID); osSendMesg(&D_801461D0,0,0); return old;`.
- `score.py group` of credits_scroll_grp with object_create moved from context to members and added to keep:
  `object_create: MATCH`, every old member still MATCH (credits_scroll, func_80100B8C, slot_state_setup,
  world_trigger_check, func_8010A53C, func_80107EDC, animLapTime, animElapsedTimerText, animPlace, animWrongWay).
  Deliverable: `groups/credits_scroll_grp/` (same name, files unchanged, group.json: members += object_create,
  context -= object_create, keep += object_create, claims [object_create]).
- Whole-program unit: `blob_unit score object_create --keep object_create` -> `EQUAL object_create: 28 words
  (kept, c_v1.c)`, BUT `--neighbours` then reports `locked bodies that differ in this unit: 1: PrintScores`
  (baseline without the keep: 0). Retail INLINES HAL_SetFont into PrintScores and keeps the body only because the
  unmatched playgame_state_change (2544 B) calls it by jal (callers.py: the only game jal). With object_create on
  the keep list umerge stops inlining it into PrintScores (36-39 words). A kept large stand-in caller instead
  (v/s1.c) keeps the body but also stops the PrintScores inline (and the body comes out 20 words). So: the group
  splice is byte-identical, but the shadow-unit gate needs playgame_state_change (or a mechanism for
  "inlined at some sites, out of line at a non-unit site"). Coordinator decision.

### func_8010D3C0 = _StartWeaponIcon (704 B): 23 rows, residual as1 cross-block hoist
- Direct typed port of MAT3 targets.c _StartWeaponIcon. N64 differences: no timeStamp store, no random-weapon
  case (672), N64 type numbers 350..360 in GC source order (jump table checked), picking up the same weapon type
  adds ammo (`if (type == car->battle.type) ammoCnt += ammo; else {type; ammo}`), shield case also clears an
  extra float at battle+0x24 (defense.unknown10), invsTime/fadeTime/shldTime/shldScale literals 30.0f,
  0.666667f (0x3F2AAAB0), 0.0333333f (0x3D088880), 0.05f (0x3D4CCCCD), sqrtf as sqrt.s.
- Layouts recovered: Car stride 952 (appearance u32 @0xE8, battle @0x384), Model stride 0x808 (body_type @8,
  bodyR[2][3] @0xF4 -> [0][2]@0xFC, [1][2]@0x108, colrad @0x654), collsize @0x8011F844 stride 16, targetInfo
  @0x80117530 stride 0x30 (startSnd @0x1C), ammoCnts @0x80121D60, IncBattlePowerUps = image B 0x80391490.
- MAT3 `wep` local kept (= 1 in every weapon case): it moves type/ammo to retail's a0/v1 (59 -> 29 rows).
- Residual: as1's cross-basic-block hoist (f_do_xbb_opt) pulls `li a0,1; lui v1` of the default case into the
  lh->addiu load stall of the switch dispatch block; retail's as1 did not. Proven with the edited-listing oracle
  (asm.sh): a `.livereg` marking BOTH a0 and v1 (or everything) live at the jump-table `j` gives 8 rows (only the
  8 own-rodata relocation rows); a0 or v1 alone gives 21. Same mechanism as w36c (func_803A9EEC) and w36f
  (func_803AD57C). Tried without effect: default first/middle/last, init before switch, wep=0/break forms,
  switch on a local / casts / compiled-out blocks before the switch (all equal or worse), -O2/-O1, six
  preceding procedures in the as1 input (no per-file state effect).

### func_8010D680 = _AnimateWeaponIcon (476 B): 26 rows
- Port of MAT3 body: N64 has no timeStamp/random/demolition logic; cases 350..360 -> wep 0..7 (353/354/359 and
  out-of-range -> -1; jump table checked); gFrameSeconds must be `extern volatile f32 D_8002EB94` (retail per-use
  lui/addiu + lwc1 0(v1)); ZOID_ObjectFlags inlined from D_8012E700 stride 0x44.
- Residuals: (1) retail sign-extends `op` into a1 itself (a coloured web), ours into t7; (2) angVF at sp+56 in
  retail, 60 in ours (one 4-byte slot); (3) as1 xbb hoist of `move a1,zero; move a0,t0` from the
  MBOX_HideObject block into the flags test (retail uses bgezl). 48 vbatch variants (decl order, helper vs
  inline flags test, op type, !op vs op==0) and op-reuse forms: no improvement.
- Later (best.c): one unused `s32 pad` declared between angV and angVF puts angVF at retail's sp+56 (frame 80
  both ways): 26 -> 22 rows. Left: op's sign extension into a1 (retail keeps op as a coloured web), `lui v1`
  of &gFrameSeconds and `move a1,zero; move a0,t0` pulled across blocks by as1's xbb pass (retail: none).

### race_position_update = TB_EllipseText (484 B): 56 rows
- Port of MAT3 TB_EllipseText (same in the Dreamcast tree); HAL_GetFontSpacing inlined as a static returning
  `s8 D_80149B70`. Structure identical except: retail hoists the constant -1 of the loop's
  `HAL_GetTextWidth(d,-1)` into s4 (runtime sll/sra each iteration) and saves s5 (no code) and s6 (dbl), frame
  120 vs 112, plus a `move v1,v0` copy of the eStr width. ctrace: our unit has no -1 constant web at all (cfe folds
  the cast); a `-1` local or an inlined width wrapper is constant-propagated and folded too (frame 120 but still
  no hoist). Next: find the source form that leaves cvt(-1) unfolded in uopt.

### object_bytes_sum_global = TB_GetFontLineHeight (84 B): 4 words, colour-only
- MAT3 form `return HAL_GetFontAscent() + HAL_GetFontDescent() + HAL_GetFontLeading();` with leading as an
  inlined static (`static s8 lead(void) { return D_80149B60; }`) gives retail's call order and v0/v1 (w12e best
  was 8 words). Only the &gFontHeader address web differs: t2 vs retail t4. force.sh `p1:w21=c11` -> 0 rows.
  ctrace: sound_update_channel preserves t1..t5 for the caller; retail must have two codeless webs in t2/t3
  (object_bytes23_sum = TB_GetFontHeight has the same t4). Defining D_801497F0 in the unit: no change.

### game_timer_display = animPanes@audioscr.c (352 B)
- NEW identity: sits right before audio_animButtonText (audioscr.c animButtonText) in address order; body =
  MAT3 audioscr.c animPanes (panesCreated D_80110634, gGameState & 0x7c03fffe, HAL_SetFont(13) inlined,
  gControllerRemoved = !gControllerData[playerInPause].attached, _TitlePane() = credits_scroll,
  y = 10 + 2*TB_GetFontLineHeight(), HAL_SetFont(10), InitPane(menuRect...)). credits_scroll = _TitlePane@audioscr.c.
- Existing context body in credits_scroll_grp/gr3_d.c: `FAIL game_timer_display: 6 of 88 words differ`
  (frame 120/112: y at 116 vs 110; `addu a3,t0,t7` operand order).
- **MATCH.** Deliverable `groups/credits_scroll_grp/` = src/blob/groups/credits_scroll_grp superseded in place:
  same name, every file byte-identical except gr3_d.c (only game_timer_display's body changed), group.json
  members += game_timer_display, context -= game_timer_display, claims [game_timer_display] (it was already on keep).
  `score.py group cand/csg` (no --claims): every old member MATCH (credits_scroll, func_80100B8C,
  slot_state_setup, world_trigger_check, func_8010A53C, func_80107EDC, animLapTime, animElapsedTimerText,
  animPlace, animWrongWay) and `game_timer_display: MATCH`; `--claims`: `Members: game_timer_display: MATCH`.
  Whole-program unit (`blob_unit --tag w36w score game_timer_display func_80100B8C credits_scroll --neighbours
  --with game_timer_display/unit/gr3_d_unit.c`, = the group's gr3_d.c with `Pair D_80118E20` made extern
  because the unit already defines it): `EQUAL game_timer_display: 88 words (kept, c_gr3_d_unit.c)`,
  `EQUAL func_80100B8C`, `EQUAL credits_scroll`, `locked bodies that differ in this unit: 0`.
- Levers: (1) menuRect read as a `{x, w, h}` struct (`((MenuRect *)D_80110650)->h`): the s32[3] array spelling
  gives `addu a3,t7,t0` whatever the source order, the struct gives retail's `addu a3,t0,t7`;
  (2) frame: two disclosed UNUSED locals `s16 x` (declared before y: puts y at sp+116 as retail) and `char buf[8]`
  (the remaining local area). Retail frame 120; 112 with y alone (y at 110); `s16 x,y + s32 a` or four s16 (8 B
  of locals) still give 112; `x,y + s32 a,b` or `x,y + buf[8]` give 120 and EQUAL. A real extra inlined wrapper
  level (_HAL_SetFont static) also makes 120 but moves the first HAL_SetFont home to 100 (retail 92).

### time_result_display = _TitlePane@optionscr.c (332 B): MATCH (group + unit)
- Retail is instruction-for-instruction the locked credits_scroll (_TitlePane@audioscr.c) with optionscr's
  globals; the context copy already in credits_scroll_grp/gr3_c.c is the right body. Why it never matched: it was
  on credits_scroll_grp's keep list (standard ABI, frame 160) and had no caller in the group. Retail: internal,
  private IPA (s0/s1/s2/s4 unsaved), callers func_800DB1E0 (updateOptionScreen, locked group
  frontier_optionscr_buttons) and the unmatched replay_save_prompt (w36q).
- Packaging (in the same superseded credits_scroll_grp): members += time_result_display, context -= it, keep -=
  it, new file gr3_trd.c with two kept stand-in callers (`__standin_time_result_display`, `...2`; disclosed:
  retail's callers are DB1E0 in another group and the unmatched replay_save_prompt). gr3_c.c unchanged.
  `score.py group cand/csg --claims`: `game_timer_display: MATCH`, `time_result_display: MATCH`; without --claims
  every old member MATCH.
- Unit (`blob_unit --tag w36w score time_result_display game_timer_display credits_scroll func_800DB1E0
  func_80100B8C --internal time_result_display --keep __standin_time_result_display --keep
  __standin_time_result_display2 --neighbours --with groups/credits_scroll_grp/gr3_trd.c --with
  game_timer_display/unit/gr3_d_unit.c`): all five `EQUAL` (time_result_display 83 words internal, body from
  g_credits_scroll_grp__gr3_c.c), `locked bodies that differ in this unit: 0`.
- Integration note: the unit needs time_result_display to share gr3_c.c's font_set_g3c with credits_scroll
  (two call sites). A separate-file copy of the helper is renamed per file by the unit manifest (no image address)
  and then credits_scroll loses its `move s0,v0` schedule (4 words); that is why the alternative packaging in
  frontier_optionscr_buttons (time_result_display/alt_frontier_optionscr_buttons/, group MATCH) is NOT the
  deliverable.

### checkpoint_hit = animElapsedTimer@hud.c (580 B): MATCH (group + unit)
- Port of MAT3 hud.c animElapsedTimer in the style of the matched sibling animLapTime (gr3_8154.c: Hidden /
  gfx_lock / gfx_unlock / font_set as non-static inlined helpers). N64 differences: alpha from the colour byte
  D_80116184[3] and half18 = 100 (GC alpha = 128); elapsedTimerPos = D_80115C88 (8-byte {x,y}).
- Levers: alpha store written before the half18 store (13 -> 8 words); frame: four disclosed UNUSED locals
  x, w, show (before win) + y (after fixedWidth) = exact frame residual (retail 144 with digitW at sp+124;
  128 without; x/w/show only: 136 with the right top offsets; char-array forms avoided).
- `score.py group cand/csg --claims`: `checkpoint_hit: MATCH` (with game_timer_display, time_result_display);
  unit: `EQUAL checkpoint_hit: 145 words (kept, c_gr3_73d8.c)` in the combined run below.
- Combined unit run (all three claims): `blob_unit --tag w36w score time_result_display game_timer_display
  checkpoint_hit credits_scroll func_800DB1E0 func_80100B8C animLapTime --internal time_result_display --keep
  __standin_time_result_display --keep __standin_time_result_display2 --neighbours --with
  groups/credits_scroll_grp/gr3_trd.c --with game_timer_display/unit/gr3_d_unit.c --with
  groups/credits_scroll_grp/gr3_73d8.c` -> `7/7 equal`, `locked bodies that differ in this unit: 0`.
- game_timer_display's frame filler is now scalar (`s16 x; s16 y; s32 u1; s32 u2;`, no char array); group and
  unit rechecked after the change (all MATCH / EQUAL).

### func_800988D8 = _ProcessSequences@audiomgr.c (380 B): MATCH (group + unit)
- Built on Astra's research packet refs/pr/408 (1/95: `bne v1,s7` vs retail `bne s7,v1` at +0xB0, the
  `state == 2` test with 2 hoisted into s7). Lever: no-code identity on the variable side, `(state*1)==2`
  (also `state^0`, `state>>0`; `2==state`, `!(state!=2)`, split decl, nested if, dropping the local: no) -
  uopt puts a plain variable second, an expression first (wave-16 rule). Disclosed in the source.
- Deliverable `groups/engine_sound_sync/` = src/blob/groups/engine_sound_sync superseded in place: members +=
  func_800988D8 (context -= it), files += timeout.c (byte copy of locked src/blob/func_80095198.c, context,
  kept, so the 14400-s clock predicate inlines), claims [func_800988D8]; engine_sound_sync.c and
  codex_transform_b109.c unchanged; audio_effect_setup_b132.c changed only in func_800988D8's body.
  `score.py group cand/ess`: engine_sound_sync, audio_effect_setup, func_80098ADC, func_800988D8 all MATCH;
  `--claims`: `func_800988D8: MATCH`.
- Unit: `blob_unit --tag w36w score func_800988D8 audio_effect_setup engine_sound_sync func_80098ADC
  --neighbours --with func_800988D8/grp/audio_effect_setup_b132.c` -> `EQUAL func_800988D8: 95 words
  (internal, ...)`, all 4/4 EQUAL, `locked bodies that differ in this unit: 0`.

### func_800AD4C8 = VectorToSurfEdge@stree.c (264 B): MATCH (group + unit), claim only
- Already a context body (identical text in groups func_800AD4C8, frontier_entity_update,
  frontier_shot_world_collision). Installed group func_800AD4C8 scored as is: `func_800AD4C8: MATCH` (context).
  Its retail callers: func_800C3AD0 (member there) and input_process_controller (real body in that group as
  context, also MATCH; lane w36v's function). Baseline unit: `EQUAL func_800AD4C8: 66 words (internal,
  g_frontier_entity_update__group.c)`, `EQUAL input_process_controller`, `locked bodies that differ: 0`.
- Deliverable `groups/func_800AD4C8/` = installed group superseded in place: func_800AD4C8 moved context ->
  members, claims [func_800AD4C8], no file changed. `score.py group` (no --claims): all 8 members MATCH;
  `--claims`: `func_800AD4C8: MATCH`. Note for the integrator: w36v may want input_process_controller from the
  same group (it is context there and MATCH too).

### menu_back = _DecodeGhost@save.c (132 B): 3 words
- Context body in src/blob/groups/menu_save_options/ctx_slot.c (its caller func_800CBF2C is a member there).
  As a member: `menu_back: 3/33 words differ`: retail `move a1,v0; sw t9,84(s0); jal; lw a0,76(s0)`, ours
  `sw t9; lw a0; jal; move a1,v0` (as1 delay-slot choice for format_string_parse's arguments). Line-splitting
  and `m->count = (n = ...) / 3` variants: unchanged.

### drone_target_update = SetControls@controls.c (496 B): MATCH (standalone + unit)
- Identity confirmed: per-player loop choosing gDefaultControls / gNewControls[controller], gControlsSet test and
  clear, two SavePlayerControls calls (profile, then gJustPlayers when no profile or resetting), then
  SetPlayerControls' table mapping (unrolled by IDO 2 + 2x4 for the N64's 10 control bytes).
- Direct port first try: 111/124 (structure identical; retail hoists &gDefaultControls into s8, ours has a
  codeless s3 web). ctrace: the inlined SetPlayerControls(p, ...) parameter copy is a coloured web; writing the
  table loop in place (a block in the player loop) removes it. `score.py fn cand/drone_target_update.c
  drone_target_update --flags "-g0 -O3 -mips2 -G 0 -non_shared"` -> `MATCH`; `blob_unit --tag w36w score
  drone_target_update --neighbours --with drone_target_update/best.c` -> `EQUAL drone_target_update: 124 words
  (kept, c_best.c)`, `locked bodies that differ in this unit: 0`. Deliverable cloud/matches/drone_target_update.c.

### debug_collision + debug_stats = updateDemoGame + playDemoGame@demo.c (532 + 532 B): MATCH (group + unit)
- Built on Astra's research refs/pr/413 (cloud/work/frontier/dot_mat3_demo_pair_20261017/group): playDemoGame
  already MATCH, updateDemoGame 3/133 (retail `move s0,zero` at the three playDemoGame call sites, ours
  `sw zero,4(sp)`: the second actual argument). Retail passes the second argument of playDemoGame in s0 (private
  IPA register parameter). Lever: give playDemoGame its second formal `x` (the GC source calls it with two
  arguments, DWARF lists one formal) and a compiled-out holder `if (x) {}` (disclosed) placed BEFORE the PrePlay
  test: x becomes a code-free coloured web live across calls -> s0, and the callers load 0 into s0. Holder
  placements: before the PrePlay test or before `func_800FBE60(); D_80114154 = 1;` -> both MATCH; after the
  if/else chain or inside it -> debug_collision MATCH but the holder emits a branch in playDemoGame; at function
  entry -> params shift (a1); `x;` alone or no holder -> 3/133.
- Deliverable: NEW group `groups/w36w_demo_pair/` (all #413 context files unchanged; demo.c = #413's with the
  formal + holder): members [debug_collision, debug_stats, func_800FCDF0, func_800FD00C, func_800FD014,
  func_800FD01C] (the four locked 2-word stubs get real inlined bodies here, wave-12 rule), claims
  [debug_collision, debug_stats]. `score.py group cand/dp`: all six members MATCH, all context MATCH.
- Unit: `blob_unit --tag w36w score debug_collision debug_stats func_800FD01C func_800FD00C func_800FD014
  func_800FCDF0 --internal debug_stats --internal func_800FD01C --internal func_800FD00C --internal func_800FD014
  --internal func_800FCDF0 --neighbours --with groups/w36w_demo_pair/demo.c` -> 6/6 EQUAL (debug_collision kept
  133 words, debug_stats internal 133 words), `locked bodies that differ in this unit: 0`.
- Integration: the four stubs are locked singles with empty bodies; the unit needs this group's definitions
  (prefer_definition) and them + debug_stats internal (force_internal), as tested above.

### func_8010E8B4 = animPanes@videoscr.c (340 B): 3 words
- Identity confirmed (MAT3 videoscr animPanes: HAL_SetFont(13), gControllerRemoved, _TitlePane = TitlePane,
  InitPane(menuRect...), then InitPane(20,136,300,204,176,0,0,0)). w32b best rescored in the current unit:
  `FAIL func_8010E8B4: 3 of 85 words differ` (y narrowed in place into t0; retail keeps the unnarrowed
  2*h+10 in t0 and the narrowed y in ring temp t9, used twice). Struct menuRect (k2) also fixes the a3 operand
  order but the narrowing stays (no-y-variable `(s16)h` casts, `y = h = ...`: same 3 words). A direct MAT3 port
  of mine reaches 6 words (needs w32b's extra font_select nesting for the osSendMesg schedule).

## What generalises

- **Context bodies that already match**: several of my pool functions were correct context copies in installed
  groups that were never promoted (time_result_display, func_800AD4C8; object_create with a keep caveat). Score
  a group with every context function listed as a member (probe) before writing new source.
- **Keep status is part of the match**: time_result_display failed for weeks only because credits_scroll_grp
  had it on keep (standard ABI, frame 160). Retail-internal functions with private IPA must be off keep and
  need real or disclosed stand-in callers in the group.
- **Shared inlined helpers must stay shared in the unit**: two _TitlePanes inlining the same font_set_g3c need
  it to keep both call sites; a per-file copy is renamed per file by the unit manifest (no image address) and
  changes the other caller's schedule.
- **An inlined static helper's parameter can be a coloured web with no code** (SetControls: the inlined
  SetPlayerControls(p, ...) copy of p took a callee-saved register and pushed a hoisted address out). If
  retail has one callee-saved web fewer than you, write the helper's body in place.
- **Array vs struct spelling of a global record changes commutative operand order** (menuRect s32[3] vs
  {x,w,h}: `y + rect.h` gives `addu a3,t7,t0` with the array whatever the source order, retail's
  `addu a3,t0,t7` with the struct).
- **A compiled-out holder on an unused parameter makes it an IPA register parameter**: `if (x) {}` before
  a call keeps the formal as a code-free coloured web across calls, so callers load the argument into the
  callee-chosen s-register (playDemoGame: retail `move s0,zero` at every call site).
- **No-code identity on the variable side of ==** (wave-16 rule) closed _ProcessSequences' last word.
- **Frame residuals of animPanes/animElapsedTimer-style HUD functions**: retail has extra 4-byte local slots
  above and below the GC locals (disclosed unused scalars reproduce them exactly; char arrays avoided).
- **as1 cross-block hoist (f_do_xbb_opt)**: proven again by the edited-listing oracle (_StartWeaponIcon: a
  `.livereg` marking a0+v1 live at the jump-table `j`, or a conservative full mask, gives the retail block;
  one register alone does not). ugen never emits such a .livereg; the source construct is still unknown (same
  open item as w36c/w36f/w26d).

## Identities proven

- debug_stats = playDemoGame@demo.c (w36n map row said uninitDemoGame, lead; #413 and this match: playDemoGame).
- drone_target_update = SetControls@controls.c (w36n: lead): byte-identical match of the MAT3 body (callee
  func_800CCE5C = SavePlayerControls, D_8013C228 gDefaultControls, D_8013C068 gNewControls, D_80139300
  gControlsSet, D_80146150 gJustPlayers, D_801118E4 SetPlayerControls' gControlTable, D_8014A118 gPlayers).

- game_timer_display = animPanes@audioscr.c (address order: immediately before audio_animButtonText =
  animButtonText@audioscr.c; every call/global matches MAT3 animPanes).
- credits_scroll = _TitlePane@audioscr.c (called as `_TitlePane()` from animPanes).

## Permission denials

none
