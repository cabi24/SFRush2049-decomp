# w38a RESULTS (wave 38, game image; branch wave37 @ fc6281e8b)

Builder scratch: watchman2:~/rush2049/scratch/frontier/w38a (copied from base; trace toolkit reused from wtk).
Overlay functions and `cloud/work/frontier/w38b/` are lane w38b's; not touched.

## Triage (51 unmatched game functions, docs/dot_handoff_targets.csv)

Excluded: func_80087110, camera_transform (Astra keep-off); held for owner with code already identical:
car_shadow_render (AudioInitSys, D_00039B40), debug_stats + debug_collision (demo pair, ipakit floor),
time_result_display (stand-in callers). Provisional-only (real caller unmatched): func_800A3640 (_RemovePak,
caller PS_PakServer), func_800B0EA0 (caller sound_bank_unload), func_800B66B0 (_TextLen, caller HAL_DrawText; both matched later this wave),
func_80098FB8 (AudioMain, caller entity_ai_pathfind), display_enable (caller playgame_state_change),
world_velocity_integrate (HAL_FinishText, IPA partner _DrawText context), brake_force_apply (align override).

Ranked work list (closest first; gap from the latest lane RESULTS):

| # | function | bytes | identity | last gap | source |
|---|---|---:|---|---|---|
| 1 | object_bytes_sum_global | 84 | TB_GetFontLineHeight | 4 words (&gFontHeader t2 vs t4) | w36w/text, f2/tb |
| 2 | object_bytes23_sum | 72 | TB_GetFontHeight | same cause | - |
| 3 | ghost_race_setup | 440 | GhostEnd@ghost.c | 3 words | w36w2, f1 |
| 4 | func_8010E8B4 | 340 | animPanes@videoscr.c | 3 words | w32b, f1 |
| 5 | drone_collision_avoid | 852 | calcMenuRect@audioscr.c | 5 words (one bottom-area slot) | w37b/rect, f2 |
| 6 | world_effect_update | 456 | InitGraphics@Init.c | 12 words | w19e |
| 7 | RaceStateMachine_Update | 2148 | updatePauseScreen@pausescr.c | 13 words short | w36p, w37c |
| 8 | car_lod_select | 324 | PS_InitPakSys | 19/81 | w37a |
| 9 | func_8010D680 / func_8010D3C0 | 476/704 | _AnimateWeaponIcon / _StartWeaponIcon | 22 / 23 rows | w36w |
| 10 | audio_frame_update | 600 | ? | 25/150 | w26e/w16f |
| 11 | world_trigger_activate | 464 | N64 gfx msg handler | 25 words | w36w3 |
| 12 | camera_look_at_point | 796 | _PlayMoveSound@targetpaths.c | 38/199 | w36v, w11c |
| 13 | explosion_effect, physics_float_calc, LoadTrack, sound_position_update | | | 37-40 rows | |
| 14 | track_render_process | 3200 | PS_PakServer | 706/800 | w37a |

## Per-function results

| function | identity | bytes | state | best file |
|---|---|---:|---|---|
| car_lod_select | PS_InitPakSys@paksysdc.c | 324 | **group MATCH + unit EQUAL** (w37a_paksys superseded in place, claims [car_lod_select]) | groups/w37a_paksys/group.c |
| LoadTrack | LoadTrack@tracks.c | 2104 | **group MATCH + unit EQUAL** (NEW group w38a_loadtrack, claims [LoadTrack]; no shaping) | groups/w38a_loadtrack/loadtrack.c |
| camera_look_at_point | _PlayMoveSound@targetpaths.c | 796 | **group MATCH + unit EQUAL** (camera_aspect_ratio superseded in place, claims [camera_look_at_point]; no shaping) | groups/camera_aspect_ratio/group.c |
| drone_collision_avoid | calcMenuRect@audioscr.c | 852 | **group MATCH + unit EQUAL** (audio_options_menu superseded in place, claims [drone_collision_avoid]; 1 disclosed unused local) | groups/audio_options_menu/rect_w38a.c |
| world_velocity_integrate | HAL_FinishText@HAL_text.c | 712 | **group MATCH + unit EQUAL** (codex_defaults_a16 superseded in place; final packet claims [world_velocity_integrate, menu_input_process, func_800B66B0]; inline blockers in a context copy, disclosed) | groups/codex_defaults_a16/finishtext.c |
| menu_input_process | HAL_DrawText@HAL_text.c | 1280 | **group MATCH + unit EQUAL** (same codex_defaults_a16 packet, claims += menu_input_process; own .bss statics; 1 disclosed empty `else ;` + uniform `& 255` byte masks) | groups/codex_defaults_a16/hal_drawtext.c |
| func_800B66B0 | _TextLen@HAL_text.c | 152 | **group MATCH + unit EQUAL** (same packet, claims += func_800B66B0; MAT3 body, no shaping; was provisional only for want of HAL_DrawText) | groups/codex_defaults_a16/hal_drawtext.c |
| audio_frame_update | StartCar@visuals.c | 600 | **group MATCH + unit EQUAL** (audio_frame_update group superseded in place, claims [audio_frame_update]; 1 disclosed cast) | groups/audio_frame_update/startcar.c |
| object_create | HAL_SetFont@HAL_text.c | 112 | **code EQUAL as kept, but provisional**: retail keeps it out of line only because its unmatched caller playgame_state_change calls it by jal | setfont/o1.c |
| state_utility | TB_DrawText@TB_text.c | 396 | 10 words (first draft; MAT3 form with the real locked helpers); residual = the inlined TB_GetFontHeight address web | tbdraw/t1.c |
| object_bytes_sum_global | TB_GetFontLineHeight | 84 | 4 words (unchanged; address web t2 vs retail t4) | ../w36w/text/best_object_bytes_sum_global.c |
| object_bytes23_sum | TB_GetFontHeight | 72 | 4 words (MAT3 `A() + D()`; same t2/t4 cause) | tb/d1.c |
| world_effect_update | InitGraphics@Init.c | 456 | 8 words (volatile msgq) / 14 (plain); unchanged | ../w19e/world_effect_update/best_volatile.c |

### world_velocity_integrate = HAL_FinishText@HAL_text.c (712 B): MATCH (group + unit)
- The w27b draft (141/178) and the codex_defaults_a16 context body (161/178) predate _DrawText's match (w36t,
  audio_doppler_calc, installed in batch 13). A direct typed port of MAT3 HAL_FinishText with the real callees
  (`finishtext.c`: _SetDefaults func_800B6748, _HAL_SetFont slot_state_setup, HAL_SetFontLeading/Spacing
  mode_byte2_set/mode_byte_set, HAL_SetTextPos func_800B70EC (umerge-inlined), HAL_SetTextTranslucency func_800ED66C,
  HAL_SetTextColor func_800B7360, _DrawText audio_doppler_calc) is EQUAL on the first compile. No shaping in the body.
- Unit (Pi): `python3 -m tools.conveyor.pipeline.blob_unit --tag w38a score world_velocity_integrate audio_doppler_calc
  func_800B6748 mode_byte2_set mode_byte_set --with cloud/work/frontier/w38a/groups/codex_defaults_a16/finishtext.c
  --neighbours` -> `EQUAL world_velocity_integrate: 178 words (kept, c_finishtext.c)`, audio_doppler_calc /
  func_800B6748 / mode_byte2_set / mode_byte_set EQUAL, `locked bodies that differ in this unit: 0`.
- Group compile needed context the unit has: slot_state_setup's IPA s2 parameter (ctx_gr3_b.c, else `end` lands in
  s1, retail s4), HAL_SetTextPos's body to inline (ctx_func_800B70EC.c), and the unit's two inline blockers:
  retail calls mode_byte2_set/mode_byte_set by jal (unit_overrides.inline_blockers), but the group compile inlines
  them (14 extra words) unless their copies carry the dead `if (0) switch` that the unit's staging adds.
- Builder: `score.py group cand/groups/codex_defaults_a16` rc=0: func_800B6748 MATCH, audio_doppler_calc MATCH,
  `world_velocity_integrate: MATCH` (`own .rodata verified at 0x80124588..0x801245A4`, the opcode jump table);
  `--claims` rc=0. gproof: `group_bodies OK: audio_doppler_calc, func_800B6748, world_velocity_integrate`, all 0 differ.
- **Integration note:** supersedes installed `src/blob/groups/codex_defaults_a16` in place (same name, old members
  func_800B6748 and audio_doppler_calc unchanged). Files: group.c = installed with the old world_velocity_integrate
  context body removed (one comment line); drawtext.c byte-identical; NEW finishtext.c (member); NEW context copies
  ctx_func_800B70EC.c, ctx_func_800ED66C.c, ctx_func_800B7360.c (= src/blob/func_800B70EC.c, func_800ED66C.c,
  func_800B7360.c; added to keep, as they are kept singles) and ctx_gr3_b.c (= src/blob/groups/frontier_pausescr_state/
  ctx_gr3_b.c; slot_state_setup/object_create/func_800B669C as context, not kept, as in frontier_optionscr_buttons);
  slot_state_setup's home keep status is unchanged by this. DISCLOSED: slot_sound.c (context copy) is byte-identical
  to src/blob/groups/slot_sound/group.c except the two inline blockers appended to mode_byte2_set and mode_byte_set
  (the unit_overrides.inline_blockers device made explicit for the group compile; integrator may prefer a blob_group
  option instead). group.json: members += world_velocity_integrate, context -= it, context += func_800B70EC,
  func_800ED66C, func_800B7360, slot_state_setup, object_create, func_800B669C, mode_byte2_set, mode_byte_set; claims
  [world_velocity_integrate] (the FINAL packet adds HAL_DrawText + _TextLen on top: see the next entry; claims become
  [world_velocity_integrate, menu_input_process, func_800B66B0]). No new unit_overrides for FinishText. Not provisional: HAL_FinishText is kept (its caller
  world_collision_response is unmatched but does not shape a kept callee); every callee is matched.

### menu_input_process = HAL_DrawText@HAL_text.c (1,280 B) + func_800B66B0 = _TextLen (152 B): MATCH (group + unit)
- Source: `groups/codex_defaults_a16/hal_drawtext.c` (unit form `drawtext/best.c`, identical code with `dt_*` helper
  names). MAT3 HAL_DrawText/_TextLen, typed against the matched callees (_DrawText = audio_doppler_calc, _SetDefaults =
  func_800B6748, strcpy func_800A473C, strncpy func_80092DCC, memcpy). What closed it, in order:
  1. MAT3's seven function-local `static` last-state variables (they are referenced only by HAL_DrawText) instead of
     externs; own .bss, verified by address (0x80153F60/80/D0, 0x80154180/84/94/9C).
  2. The semaphore as MAT3's `_GetSemaphore`/`_ReleaseSemaphore` helpers (frame 376 exact). In the group they are
     non-static and not on keep (w17j convention, `_GetSemaphore_DrawText`/`_ReleaseSemaphore_DrawText`): `static`
     versions leave two 2-word deleted-inline stubs in the group object, and score.py then reports func_800B6748 and
     func_800B70EC as `1 extra words (nonzero beyond target length)`.
  3. MAT3 statement order (`last = g` before the buffer writes; position block as MAT3: assign, write, (re)assign).
  4. The address-web tie. ctrace: the gFontIndex, gLeading and gSpacing address webs tie at save 0.2 (nocs 5) and
     only t4/t5 are free, so the lowest web number (gFontIndex) took t4 and gSpacing was split; retail splits
     gFontIndex. `force.sh ... p1:w161=s` gave 0 ops rows, which proved the tie was the only structural residual.
     An empty `else ;` on the first size test (`if (lastFontIndex != gFontIndex) size = 2; else ;`) adds a
     zero-footprint uopt block to that web's live range only: nocs 5 -> 6, save 0.2 -> 0.167, and it loses the tie.
     `if (0) {}`, `do { } while (0)`, an explicit `goto`, and `if (size) {}` after the test do the same; `? :`
     and `if/else size = 0` change the code (5 ops rows).
  5. Ring phase: every computed byte stored into gBuffer is masked `& 255` (both position bytes of each coordinate
     and all four translucency bytes). Only the all-masked spelling matches; the other 15 subsets of each block
     differ (18-66 words). MAT3 masks only the high position bytes.
- Unit (Pi): `python3 -m tools.conveyor.pipeline.blob_unit --tag w38a score menu_input_process func_800B66B0
  world_velocity_integrate --with cloud/work/frontier/w38a/groups/codex_defaults_a16/hal_drawtext.c --with
  cloud/work/frontier/w38a/groups/codex_defaults_a16/finishtext.c --internal func_800B66B0 --internal
  _GetSemaphore_DrawText --internal _ReleaseSemaphore_DrawText --neighbours` -> `EQUAL menu_input_process: 320
  words (kept, c_hal_drawtext.c); own zero-initialised data (addresses consistent, inside game .bss, no overlaps)`,
  `EQUAL func_800B66B0: 38 words (internal, c_hal_drawtext.c)`, `EQUAL world_velocity_integrate: 178 words`,
  `locked bodies that differ in this unit: 0`, `3/3 equal` (`drawtext/unit_group_form.txt`).
- Builder: `score.py group cand/groups/codex_defaults_a16` rc=0, every member MATCH (func_800B6748,
  audio_doppler_calc, world_velocity_integrate, menu_input_process with seven `own .bss placed at ...` notes,
  func_800B66B0); `--claims` rc=0, claims world_velocity_integrate / menu_input_process / func_800B66B0 all MATCH.
  Context unchanged from the installed packet (world_trigger_check 2/54, object_create and func_800B669C stand-in
  bodies; informational). Scores: `groups/score_codex_defaults_a16{,_claims}.txt`.
- gproof: `group_bodies OK: audio_doppler_calc, func_800B66B0, func_800B6748, menu_input_process,
  world_velocity_integrate`, all 0 differ, `5/5 members image-identical` (own .bss placed by the per-reference path).
- **Integration note:** this is the SAME `codex_defaults_a16` packet as the HAL_FinishText entry above (one supersede
  in place carries both waves of changes; the FinishText-only intermediate is kept for reference in
  `drawtext/prev_codex_defaults_a16_finishtext_only/`). On top of the FinishText changes: group.c also loses the old
  m2c context body of menu_input_process (one comment line in its place; the stale `s32 func_800B66B0(u8 *,s16);`
  prototype in group.c is a separate TU and harmless); NEW member file hal_drawtext.c; group.json members +=
  menu_input_process, func_800B66B0; context -= menu_input_process; claims = [world_velocity_integrate,
  menu_input_process, func_800B66B0]; keep unchanged (menu_input_process was already kept; func_800B66B0 and the
  two helpers are not on keep). unit_overrides: force_internal += func_800B66B0, _GetSemaphore_DrawText,
  _ReleaseSemaphore_DrawText (if blob_unit does not derive them). Own .bss: seven objects (address-only proof,
  the usual caveat: sizes/zero init not provable). Not provisional: HAL_DrawText is kept; its callers are state_utility
  (TB_DrawText, unmatched, calls it by jal after spilling) and camera_auto_follow (locked, unchanged: 0 locked
  bodies differ); _TextLen's only caller is HAL_DrawText. DISCLOSED shaping: the empty `else ;` and the uniform
  `& 255` spelling (both above).

### LoadTrack = tracks.c LoadTrack (2,104 B): MATCH (group + unit)
- Base: w31d best (= w30d; `39 of 526 words differ`, rows 40, frame 168/168).
- Lever 1 (ring): the Dreamcast decomp's tracks.c keeps three empty checks after the gRace tests,
  `if (gGameState & 8) {} else if (gGameState & 8) {}` and `if (gGameState & 8) {}`; added verbatim they compile to
  nothing and supply the one missing t6-t9 ring pop (trackInited load t6 as retail): 39 -> 10 words (12 rows).
  The single `if (x & 8) {}` alone does nothing (39); the if/else-if pair is the part that pops.
- Lever 2 (helpers): NewWorld is the LOCKED single wheel_torque_apply (0x800AC668; its out-parameter store is what
  gives retail's separate `lui at`/`lui a0` WorldBase store and reload) and the N64 `D_80110680[0..1] = 0;
  InitCheckpointData()` pair is the LOCKED single mode_flags_clear; calling both (umerge inlines them) replaces the
  draft's private w29_NewWorld/w29_FindWorld/w30_Clear copies (and its volatile WorldBase): 10 -> 0. On the way,
  defining the private copies after LoadTrack (world.c's higher source lines) fixed the Worlddlidx store schedule
  (as1 line tie), but the real functions make that moot.
- The final condition is the natural `if (D_80156994 || track >= 6)`; uopt itself produces retail's compare of the
  spilled `&gWorldNames[track]` against `&gWorldNames[6]` (the draft spelled the pointer compare).
- Unit (Pi): `python3 -m tools.conveyor.pipeline.blob_unit --tag w38a score LoadTrack player_flag_clear_process
  wheel_torque_apply mode_flags_clear --with cloud/work/frontier/w38a/groups/w38a_loadtrack/loadtrack.c --neighbours`
  -> `EQUAL LoadTrack: 526 words (kept, c_loadtrack.c)`, player_flag_clear_process / wheel_torque_apply /
  mode_flags_clear EQUAL, `locked bodies that differ in this unit: 0`.
- Builder: `score.py group cand/groups/w38a_loadtrack` rc=0: Members `LoadTrack: MATCH`; context func_80096288,
  slot_value_get, display_list_alloc, player_state_get, slot_deactivate, sound_update_channel, mode_byte2_set,
  object_type_byte2_get, object_type_byte3_get, mode_byte_set, wheel_torque_apply MATCH, mode_flags_clear
  `MISMATCH (1 extra words (nonzero beyond target length))` (context only: the deleted stub of
  wheel_torque_apply.c's static wheel_find_node lands after it in the group object; not spliced). `--claims` rc=0:
  `LoadTrack: MATCH`. Outputs: groups/score_w38a_loadtrack.txt, groups/score_w38a_loadtrack_claims.txt.
- **Integration note:** NEW group `w38a_loadtrack` (supersedes nothing; LoadTrack had no installed copy). Files:
  loadtrack.c (member) + byte-identical context copies ctx_slot_sound.c (= src/blob/groups/slot_sound/group.c),
  ctx_wheel_torque_apply.c (= src/blob/wheel_torque_apply.c), ctx_mode_flags_clear.c (= src/blob/mode_flags_clear.c).
  keep = LoadTrack + every function slot_sound keeps that ctx_slot_sound.c defines + wheel_torque_apply +
  mode_flags_clear, so the lock derivation turns nothing internal (func_80096288 stays internal, as in slot_sound).
  claims [LoadTrack]. No unit_overrides, extern_data, force_internal or prefer_definition needed. Not provisional:
  callers player_flag_clear_process (locked) and display_list_flush (unmatched, but a caller does not shape a kept
  callee); every callee is locked except the image-B call.

### camera_look_at_point = targetpaths.c _PlayMoveSound (796 B): MATCH (group + unit)
- Base: the installed context body in camera_aspect_ratio (w12f/w16a, `tt` holder for retail's empty `bgez v1`,
  38/199). Replaced by the Dreamcast decomp's _PlayMoveSound text, which keeps two N64-era empty statements that
  GC/MAT3 dropped: `if ((maxVel < 0 && curVel > 0) || (maxVel > 0 && curVel < 0)) ;` (this is retail's empty
  `bgez v1`, no holder needed) and `if ((int)ratio < 0 || (int)ratio > 1) ;` inside the clamp. With path_abs as a
  macro: 1 word; MAT3's `static inline int path_abs(int)` as a function: 168 words. Last word: `0.75f * ratio`
  (retail `mul.s $f10,$f6,$f0`; DC spells `ratio * 0.75f`). 38 -> 0, no shaping devices.
- Unit (Pi): `python3 -m tools.conveyor.pipeline.blob_unit --tag w38a score camera_look_at_point camera_update
  camera_aspect_ratio --with cloud/work/frontier/w38a/pms/g_dc3.c --internal camera_look_at_point --neighbours` ->
  `EQUAL camera_look_at_point: 199 words (internal, c_g_dc3.c)`, `EQUAL camera_update: 719 words`, `EQUAL
  camera_aspect_ratio: 50 words`, `locked bodies that differ in this unit: 0`. (g_dc3.c = the group.c below.)
- Builder: `score.py group cand/groups/camera_aspect_ratio` rc=0: all eight old members MATCH (camera_aspect_ratio,
  camera_fov_control, camera_build_view_matrix, func_800C15FC, camera_process_input, camera_update,
  camera_free_look, camera_track_spline, own data verified as before) and `camera_look_at_point: MATCH`; `--claims`
  rc=0: `camera_look_at_point: MATCH`. Outputs: groups/score_camera_aspect_ratio.txt, ..._claims.txt.
- **Integration note:** supersedes installed `src/blob/groups/camera_aspect_ratio` in place (same name, one file).
  group.c differs only in camera_look_at_point's body plus the path_abs macro and its comment right above it;
  group.json: members += camera_look_at_point, context -= camera_look_at_point, claims [camera_look_at_point],
  keep unchanged (the existing `__standin_camera_look_at_point` stays, like the group's other three stand-ins; the
  unit check above has the real caller camera_update). camera_look_at_point stays internal (IPA s0 parameter, off
  keep, as before); no new overrides. Not provisional: its only caller camera_update is a member.

### drone_collision_avoid = calcMenuRect@audioscr.c (852 B): MATCH (group + unit)
- Base: w37b rect/v/a0.c (5 words: menuIdx IV home sp+28 vs retail sp+32, i.e. retail's inline area is 4 B bigger).
- Levers: (1) validOption is the group's own member func_800D690C (w22e's body `s32 v; v = 1; return v;` = the
  Dreamcast decomp's `signed char validOption(int option) { signed char valid = 1; return valid; }` shape), called and
  umerge-inlined instead of a private `return 1;` copy: its named local is the missing 4-byte step of the inline
  area (IV home sp+32); (2) HAL_SetFont in the MAT3 two-level shape called directly (`cmr_font_set(10)`: lock,
  slot_state_setup = _HAL_SetFont, unlock), no extra void wrapper; (3) cmr_gfx_unlock defined AFTER cmr_font_set with
  a prototype before (as1 source-line tie, the highscore_anim precedent).
- Tested and rejected: an empty inlined call (the caller-less stub func_800D6E74 right before calcMenuRect, w38b's
  8-byte-per-inlined-call rule) instead of u0: frame 144 but the upper locals 4 B high (18 words). func_800D6E74 is
  more likely initScreen's deleted initButtons (w38b layout rule: initButtons stub, calcMenuRect, initScreen); claiming
  it would mean restructuring the locked drone_pathfind_main, not needed here.
- DISCLOSED: one unused local `u0` kept from w36q/w37b. Frame arithmetic: retail 144; without u0 136 (every home 8 B
  low); with u0 144 and every sp offset retail's.
- Unit (Pi): `python3 -m tools.conveyor.pipeline.blob_unit --tag w38a score drone_collision_avoid drone_pathfind_main
  func_800D7634 --with cloud/work/frontier/w38a/rect/rect_w38a.c --internal drone_collision_avoid --internal
  cmr_gfx_lock --internal cmr_gfx_unlock --internal cmr_font_set --neighbours` (scoring also func_800D6914) ->
  `EQUAL drone_collision_avoid: 213 words (internal, c_rect_w38a.c)`, `EQUAL drone_pathfind_main: 281 words`, `EQUAL
  func_800D7634: 451 words`, `EQUAL func_800D6914: 315 words`, `locked bodies that differ in this unit: 0`. As a kept function it is 209/213: it must be
  internal (its sole caller drone_pathfind_main is internal in the same group).
- Builder: `score.py group cand/groups/audio_options_menu` rc=1 only because of the pre-existing
  `func_800D6914: NOT VERIFIED (12 section-relative relocations ...)` (two-region own .rodata, documented in the
  group's note; installs via blob_group); members func_800D690C, func_800D7634, drone_pathfind_main, credits_screen
  MATCH and `drone_collision_avoid: MATCH` (own .rodata verified at 0x8011F6E8..0x8011F6F0). Without the slot_state_setup
  context the group compile passed font 10 in a0 instead of slot_state_setup's IPA s2 (5 words), hence ctx_gr3_b.c.
  `--claims` rc=0: `drone_collision_avoid: MATCH`. Outputs: groups/score_audio_options_menu*.txt.
- **Integration note:** supersedes installed `src/blob/groups/audio_options_menu` in place (same name; all eleven old
  files byte-identical). New files: rect_w38a.c (member drone_collision_avoid + helpers cmr_gfx_lock, cmr_gfx_unlock,
  cmr_font_set; it calls the member func_800D690C) and ctx_gr3_b.c (byte copy of src/blob/groups/frontier_pausescr_state/ctx_gr3_b.c, the
  same context copy frontier_optionscr_buttons uses). group.json: members += drone_collision_avoid, context +=
  slot_state_setup, object_create, func_800B669C, claims [drone_collision_avoid]; keep unchanged.
  unit_overrides force_internal (if not derived): drone_collision_avoid, cmr_gfx_lock, cmr_gfx_unlock,
  cmr_font_set. Not provisional (sole caller drone_pathfind_main is a member).

### audio_frame_update = StartCar@visuals.c (600 B): MATCH (group + unit)
- Identity from w36n (callees StartFrame/StartSparks/StartWing/StartWingFlame); the old draft (w17i/w26e f2.c, 25-27
  words) predates it. Rewritten in MAT3 shape: StartCar calls StartHeadLights/StartBrakeLights (statics) and
  `for (j = 0; j < 4; j++) StartTire(slot, j);` (static, `u32 tire`, visuals[tire + 6]); the N64 condition
  `t = state_word_a & 8; if (t == 0 || (t != 0 && D_80156994 != 0))` around the wing calls (retail's double beqz).
- Levers: (1) the helpers defined AFTER StartCar (MAT3 file order): as1 then schedules the brake-light stores
  after the hoisted tire-loop invariants (6 words with the helpers first; any order of the three after the caller,
  or StartHeadLights before, is EQUAL). (2) DISCLOSED cast: `v = (tVisual *)((u32)&player_array[slot] + 0x110);`
  in StartBrakeLights, so `&gGameCar[slot]` is one web shared with the tire loop (retail a2) and visuals[0] is a2+272
  in v0; every pointer spelling folds the 272 into the offsets and recomputes slot*952 (26-32 words: `.visuals`,
  `&...visuals[0]`, car local, (u8 *) offset, inline getter, `if (v) {}` holder).
- Deleted statics: AnimateTire/AnimateBrakeLights/D_8011B5AC are referenced only by this function (xref), so the
  helpers have no out-of-line copies. In the group object their three unnamed deleted-procedure stubs land at the
  start, before func_800B08FC, exactly where retail has four caller-less stubs (func_800B08DC..F4, locked singles);
  blob_group's extra-word check only looks after each member, so they are not attributed to anything.
- Unit (Pi): `python3 -m tools.conveyor.pipeline.blob_unit --tag w38a score audio_frame_update func_800B0A88
  func_800B08FC music_control --with cloud/work/frontier/w38a/groups/audio_frame_update/startcar.c --neighbours` ->
  `EQUAL audio_frame_update: 150 words (kept, c_startcar.c)`, `EQUAL func_800B0A88: 112 words`, `EQUAL func_800B08FC:
  99 words`, `EQUAL music_control: 248 words`, `locked bodies that differ in this unit: 0`.
- Builder: `score.py group cand/groups/audio_frame_update` rc=0: Members func_800B0A88 MATCH, audio_frame_update
  MATCH; context func_800B08FC `4/99 words differ` (the installed group gives the same 4/99 for it, and
  `29/150` for the old audio_frame_update context body). `--claims` rc=0: `audio_frame_update: MATCH`.
  Outputs: groups/score_audio_frame_update.txt, groups/score_audio_frame_update_claims.txt.
- **Integration note:** supersedes installed `src/blob/groups/audio_frame_update` in place (same name). group.c is the
  installed file with only the old audio_frame_update definition removed (one comment line in its place; the
  prototype block and the two stand-ins are unchanged); new file startcar.c holds StartCar + its three static
  helpers; group.json: files += startcar.c, members [func_800B0A88, audio_frame_update] (old member kept), context
  [func_800B08FC], keep unchanged (audio_frame_update was already kept), claims [audio_frame_update]. No unit_overrides,
  extern_data, force_internal or prefer_definition needed; the four locked stub singles func_800B08DC..F4 stay as they
  are. codex_audio_frame_a5 still carries an old audio_frame_update text as context; it does not need to change
  (unit check above uses its func_800B08FC). Not provisional: every callee (StartFrame, StartSparks, StartWing,
  StartWingFlame) is matched; the image-B StartWeapon call is an external jal.

### car_lod_select = PS_InitPakSys (324 B): MATCH (group + unit)
- Lever: the free-handle loop written as MAT3/GC's indexed `for (i = 0; i < 64; i++) { D_80144C60[i].file =
  &D_80144DC0[i]; func_80091FBC(&D_801460E0, &D_80144C60[i], D_801460E0.head); }` instead of w36w3's pointer
  do-while (`fileh`/`psfile` locals, `while (psfile < D_801460C0)`). With pointers, the free-list address web
  (save 12) is coloured before the end-address web (save 5) and takes s3; with the indexed loop uopt builds the
  two induction pointers and the end bound itself and retail's s4 list / s3 end follows, including the `lw a2,8(s4)`
  in the jal delay slot. 19/81 -> 0. No shaping.
- Unit (Pi): `python3 -m tools.conveyor.pipeline.blob_unit --tag w38a score car_lod_select track_render_process
  PS_CreateFile no_catchup func_800A3640 func_800A3724 --with cloud/work/frontier/w38a/pak/unit_init.c --neighbours`
  -> `EQUAL car_lod_select: 81 words (kept, c_unit_init.c)`, PS_CreateFile / no_catchup / func_800A3640 /
  func_800A3724 EQUAL, `FAIL track_render_process: 706 of 800 words differ` (unchanged context),
  `locked bodies that differ in this unit: 0`. (`pak/unit_init.c` = the group's declarations + this body only;
  passing a full copy of group.c with `--with` breaks PS_CreateFile's inlining in the harness, also for the
  unmodified copy.)
- Builder: `score.py group cand/groups/w37a_paksys` (no --claims) rc=0: Members PS_CreateFile MATCH, no_catchup
  MATCH, func_800A3724 MATCH, car_lod_select MATCH; context func_800A1E94 / func_800A370C / func_800A44E8 /
  func_8008A704 MATCH, func_800A3640 `29/51 words differ`, track_render_process `779/800 words differ` (as before).
  `--claims` rc=0: `car_lod_select: MATCH`. Outputs: groups/score_w37a_paksys.txt, groups/score_w37a_paksys_claims.txt.
- **Integration note:** supersedes installed `src/blob/groups/w37a_paksys` in place (same name). ctx_ll.c and
  ctx_lock.c byte-identical; group.c differs only in PS_InitPakSys's body, the header comment and the removed
  now-unused `extern tPS_File D_801460C0[]`; group.json: members += car_lod_select, context -= car_lod_select,
  claims [car_lod_select], note/provenance updated. car_lod_select was already on keep; no new unit_overrides,
  extern_data, force_internal or prefer_definition needed (the existing w37a_paksys overrides stay). All old members
  still MATCH. Provisional caveat unchanged for the group as a whole: PS_PakServer (track_render_process) remains
  unmatched caller context of _ClearPak/_RemovePak (accepted in batch w37-3); car_lod_select itself has no callers
  by jal and its callees (_ClearPak, InitLL, InsertLLE, func_800A43FC, osCreateThread/osStartThread) are matched.

## Splice-proof replay (all six packets)

`cloud/work/frontier/w38a/tools/gproof.py GROUP_DIR` (read-only copy of w37b's: blob_group.builder_script build in
`~/rush2049/scratch/frontier/w38a/gp/<group>`, then blob_group.group_bodies with the relocation/own-data proofs, then
a byte compare with the image; log `groups/gproof_all.log`):
- w38a_loadtrack: `group_bodies OK: LoadTrack`, `LoadTrack: 526 words, 0 differ from the image`, `1/1`.
- w37a_paksys: `group_bodies OK: PS_CreateFile, car_lod_select, func_800A3724, no_catchup`, all 0 differ, `4/4`.
- audio_frame_update: `group_bodies OK: audio_frame_update, func_800B0A88`, both 0 differ, `2/2`.
- camera_aspect_ratio: all nine members incl. camera_look_at_point 0 differ, `9/9`.
- audio_options_menu: all six members incl. drone_collision_avoid 0 differ, `6/6`.
- codex_defaults_a16 (final, with HAL_FinishText + HAL_DrawText + _TextLen): `group_bodies OK: audio_doppler_calc,
  func_800B66B0, func_800B6748, menu_input_process, world_velocity_integrate`, all 0 differ, `5/5`.

Combined unit run (five bodies, before HAL_FinishText/HAL_DrawText):
`python3 -m tools.conveyor.pipeline.blob_unit --tag w38a score LoadTrack car_lod_select audio_frame_update
camera_look_at_point drone_collision_avoid camera_update drone_pathfind_main func_800B0A88 func_800B08FC PS_CreateFile
func_800A3724 --with .../w38a_loadtrack/loadtrack.c --with .../pak/unit_init.c --with .../audio_frame_update/startcar.c
--with .../pms/g_dc3.c --with .../audio_options_menu/rect_w38a.c --internal camera_look_at_point --internal
drone_collision_avoid --internal cmr_gfx_lock --internal cmr_gfx_unlock --internal cmr_font_set --neighbours` ->
`blob_unit score: 11/11 equal`, `locked bodies that differ in this unit: 0`.

Final combined unit run (all eight claimed bodies of the six packets together, `groups/unit_all_w38a.txt`):
`python3 -m tools.conveyor.pipeline.blob_unit --tag w38a score LoadTrack car_lod_select audio_frame_update
camera_look_at_point drone_collision_avoid world_velocity_integrate menu_input_process func_800B66B0 camera_update
drone_pathfind_main func_800B0A88 func_800B08FC PS_CreateFile func_800A3724 audio_doppler_calc func_800B6748 --with
.../w38a_loadtrack/loadtrack.c --with .../pak/unit_init.c --with .../audio_frame_update/startcar.c --with
.../pms/g_dc3.c --with .../audio_options_menu/rect_w38a.c --with .../codex_defaults_a16/finishtext.c --with
.../codex_defaults_a16/hal_drawtext.c --internal camera_look_at_point --internal drone_collision_avoid --internal
cmr_gfx_lock --internal cmr_gfx_unlock --internal cmr_font_set --internal func_800B66B0 --internal
_GetSemaphore_DrawText --internal _ReleaseSemaphore_DrawText --neighbours` -> `blob_unit score: 16/16 equal`,
`locked bodies that differ in this unit: 0`.

## Not matched (investigated this wave)

### object_create = HAL_SetFont@HAL_text.c (112 B): code EQUAL when kept, provisional on playgame_state_change
- `setfont/o1.c` (MAT3 HAL_SetFont: `old = slot_state_setup(fontID)` between the D_801461D0 recv/send):
  `blob_unit --tag w38a score object_create --with cloud/work/frontier/w38a/setfont/o1.c --keep object_create` ->
  `EQUAL object_create: 28 words (kept, c_o1.c)`. Without `--keep` it is inlined everywhere and deleted (2-word body).
- With `--keep`, PrintScores (locked, frontier_billboard_render) stops inlining it: `locked bodies that differ in this
  unit: 1: PrintScores`. So retail's HAL_SetFont is INTERNAL; it survives out of line only because
  playgame_state_change (initGameState, unmatched) calls it 13 times and umerge's per-caller budget runs out (w36p:
  retail inlines only the last 3). Lands with playgame_state_change; not a match now.

### state_utility = TB_DrawText@TB_text.c (396 B): 10 words, first draft
- `tbdraw/t1.c`: MAT3 body with the real locked helpers: TB_CenterText = sound_pitch_diff_halved, TB_RightText =
  sound_pitch_diff_calc, TB_GetFontHeight = object_bytes23_sum (unit context body), HAL_SetTextPos = func_800B70EC,
  HAL_DrawText = menu_input_process, gJust = D_80118E20 {h, v}. Unit: `FAIL state_utility: 14 of 99 words differ |
  words 10 ops 4` (us.sh). The only residual is the inlined TB_GetFontHeight: ours colours the &gFontHeader address
  (t4) in one arm, retail rematerialises `lui/lw` in both arms. Same mechanism as the two TB getters below.
- Callees are kept and called after everything live is spilled (x/y homes), so HAL_DrawText being unmatched does not
  shape this body; it would land in a group with sound_update_channel's IPA t0 (codex_sound_channel_extra-like context).

### object_bytes_sum_global / object_bytes23_sum = TB_GetFontLineHeight / TB_GetFontHeight: 4 words each
- Both out-of-line copies colour &gFontHeader in t4 (retail) where ours takes t2; in TB_DrawText (t2/t3 = x/y) retail
  leaves it uncoloured. Consistent reading: TB_GetFontHeight's inlined getters carry two more webs that cross the
  second _SetupFontData call and take the two lowest free t-registers (t2/t3 out of line, t4/t5 in TB_DrawText).
  sound_update_channel's kill set is not the cause (retail TB_DrawText keeps x/y in t2/t3 across it).
- New facts: HAL_GetFontLeading is the locked func_800B3D0C (`return D_80149B60`) and calling it is identical to the
  static `lead()` (4 words); `A() + D()` in TB_GetFontHeight evaluates A first (d1.c, 4 words), while the 3-term
  LineHeight evaluates D first (umerge). Tried: 6 TB_GetFontHeight spellings (h_1..h_6), 6 nested/typed getter forms
  (n_1..n_6) paired with TB_DrawText in one unit run: never below 4 / 10. Next: find the two codeless webs (the
  N64 HAL_GetFontAscent/Descent bodies are the suspects; their out-of-line copies are locked and fixed).
- ctrace (late wave 38, `tb/d1.c` and `tbdraw/t1.c`): standalone, the &gFontHeader web (save 0.5) sees t2..t5 all at
  cost 0 and takes the lowest, t2; retail took t4. In TB_DrawText the two inlined copies' webs are w101 (save 0.67,
  coloured t4: t2/t3 hold x/y) and w100 (split); retail splits both. Retail _SetupFontData (sound_update_channel)
  writes only t0, t6-t9, a0-a3, v0/v1, so its kill set cannot explain t2/t3. Both retail colourings fit exactly
  ONE model: two extra webs, coloured before &gFontHeader and live across the second _SetupFontData call, that
  occupy the two lowest free t-registers (t2/t3 standalone, t4/t5 inside TB_DrawText) and emit no code. Not found.

### world_effect_update = InitGraphics@Init.c (456 B): unchanged (8 words with volatile msgq, 14 plain)
- Helper forms (static/_InitGfx, non-static func_800EE8AC = the stub before it, `D_801497F4 = msgq_ptr` inside or
  outside the helper, empty inlined calls at the top): all identical to w19e (the block boundary after the prologue
  is still missing; empty inlined calls leave no block).

### particle_lifetime_set = createBG@front.c (732 B): 51 words, unchanged
- The private ZOID_ShowPoly copy can be the locked func_8008D0B0 (`flags &= 0x7fff`), no change. Ring trace
  (`ringsim.py` over ugt.sh): the ring is identical up to the gBackCamera load; retail's load takes a NEW temp (t3)
  where ours reuses its base register (t1), i.e. retail is one pop ahead there and does not load in place. Not solved.

### controls_animText (1,576 B): Dreamcast goto loop + `menuY` local tried, worse (174-377 rows; base 151)

### RaceStateMachine_Update = updatePauseScreen (2,148 B): 13 words short, unchanged
- Re-scored w36p best; the missing words are all `li t0,2` (constant 2 for the end chain) placed after the last call
  of every predecessor of the end block, plus one `li t9,7`. The Dreamcast pausescr.c is the DC-specific pak version
  and has no extra N64-era statement in the end chain. Not attempted further (w37c's analysis stands).

### race_position_update = TB_EllipseText (484 B): 106/121 (frame now exact), not matched
- HAL_GetFontSpacing is the locked func_800BDEB0 (no change). An inlined width wrapper for the loop's
  HAL_GetTextWidth gives retail's frame (120, one inlined call = 8 B, the w38b rule) but uopt still folds the -1;
  retail keeps -1 in s4 and narrows it each iteration, saves s5 with no code and keeps dbl in s6. TB_RightText
  (sound_pitch_diff_calc) for `ew`: frame 128, worse.

### func_8010E8B4 = animPanes@videoscr.c (340 B): calling the real HAL_SetFont (object_create) leaves it out of line
  (jal, IPA s2 = 13; 61 words); w32b's private 3-level font_set stays the best (3 words, disclosed u0).

### explosion_effect = animSpeedText@hud.c (1,460 B): 37 -> 33 rows (not matched)
- `ee/x2.c` = w36x3 u27.c with the real TB_GetFontHeight (`object_bytes23_sum() - 4`, umerge-inlined unit body)
  instead of the private _Speed copy: `FAIL explosion_effect: 282 of 365 words differ | 33 rows` (unit, same
  --internal flags as w36x3). The real TB_SetJust (locked func_800B669C, pointer-local body) costs +8 frame (296,
  218 rows); the real HAL_SetFont (object_create) is not inlined (315 words). Residual unchanged in kind:
  &gGameCar web t0 vs v0, constants 1/10 one register off, one extra `move a2,t1`.

### playgame_state_change (2,544 B): unchanged
- The Dreamcast state.c keeps two empty `if (p > 1) {}` checks in the controller-off loops; added verbatim: no change
  (80 rows, frame 232/224). MAT3's empty loading-effect calls (le*) as inlined empties blow umerge's budget (frame 96).

### CreatePlayer (1,088 B): unchanged; layout hint
- Two caller-less stubs precede it (func_800CD0F4, func_800CD0FC). By w38b's post-order rule they are the deleted
  statics _Pow (first, as a callee of the name helper) and the unique-name helper (second); retail's `move a0,s7` /
  `move a3,s2` before the filter loop are that helper's parameter copies. A static helper with the
  memset/prefix/filter/digits body is not inlined by our umerge (v1/v2/v5: 177-194 words), so the original helper
  must be smaller or split differently. Not solved.

## What generalises

- **A tie on save is decided by a zero-footprint block.** When ctrace shows equal-save webs (same nocs) and retail
  colours a different one, an empty `else ;` (or `if (0) {}`, `do {} while (0)`, `if (x) {}`, a `goto` around the
  statement) adds a uopt block to ONE web's live range: nocs 5 -> 6 drops its save below the tie and it is coloured
  last (HAL_DrawText: gFontIndex vs gLeading/gSpacing address webs, 14 ops rows -> 0). Prove it first with
  `force.sh LABEL NAME p1:wN=s` (0 ops rows), then place the empty block where only that web is live.
- **Ring phase from masks: sweep the whole subset space, the answer may be "all".** Every computed byte stored into
  a byte buffer was `& 255` in retail HAL_DrawText (4 position + 4 translucency bytes); 15 of 16 subsets per block
  differ by 18-66 words. A redundant mask costs a ring pop (L65), so a uniform macro-like spelling is a lead.
- **MAT3 typed ports with the REAL locked callees beat older m2c drafts.** HAL_FinishText (EQUAL on first compile),
  LoadTrack (wheel_torque_apply = NewWorld, mode_flags_clear), calcMenuRect (validOption = func_800D690C),
  HAL_DrawText (_DrawText, _SetDefaults): drafts that predate a partner's match close fast when rewritten.
- **MAT3 indexed loops beat pointer-walk reconstructions** (PS_InitPakSys).
- **Dreamcast text keeps N64-era empty statements** that supply ring pops or empty branches (LoadTrack's empty
  `gGameState & 8` checks, _PlayMoveSound's empty ifs).
- **Helpers defined after the caller** (MAT3 file order) settle as1 line ties (StartCar's static helpers,
  calcMenuRect's cmr_gfx_unlock).
- **Function-local statics** for globals that only one function references (HAL_DrawText's seven last-state
  variables): own .bss, address-verified by score.py and placed by blob_group's per-reference path.
- **Group compiles need the unit's context**: IPA parameter conventions (slot_state_setup's s2 via ctx_gr3_b.c), the
  unit's inline blockers, every inlined context body listed in group.json `context`, and unique non-static helper
  names not on keep (w17j). `static` helpers leave 2-word deleted-inline stubs in the group object that make
  neighbouring members score `1 extra words (nonzero beyond target length)`.
