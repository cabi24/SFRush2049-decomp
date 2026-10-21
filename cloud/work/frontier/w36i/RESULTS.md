# w36i: wave-36 integration

Branch `wave35`, integration lane only (src/, locks, commits). Every batch (log `gates_batchN.log`): overlay
batches `ovl_layout build`, `ovl_splice check`, `ovl_rom rom` (image gates byte-identical, ovl lock intact, ROM
SHA-1 EXACT); game batches `blob_unit check` (all locked bodies equal), `blob_group check`, `blob_splice check`,
`lock check`, `ovl_splice check`, `blob_rom rom` (ROM SHA-1 EXACT with the source-built blob). Tests on rocky
(clean clone, authoritative) and the Pi before every push; the Pi's only failures were the known
untracked-lane-file receipts.

## Batches

| Batch | Commit | What | Bytes | Image A | Image B | Pushed |
|---|---|---|---|---|---|---|
| 1 | 2bc8a8558 | w36a updateCarScreen, showCarSelect, _InitScreen, createGridMap (dot_cleanup_colortrace_20261011) | +15,000 | 90 -> 94/193 | 25/49 | yes (a14da0b24) |
| 2 | ecd144bde | w36g circuitscr + cheat screen, w36f recordscr/modescr/stats | +18,208 | 94 -> 107/193 | 25/49 | no (see batch 3) |
| 3 | a5a4ccc1a | back out w36f's dot_runtime_a_seven_option_group_20261007 supersession (func_803993EC, func_80399D10) | -2,568 | 107 -> 105/193 | 25/49 | yes (a5a4ccc1a) |
| - | 8754d42bc | w36y research commit (MAT3 -> N64 data-symbol map, struct layouts; not applied) | 0 | | | |
| 4 | 0ce59e845 | w36j image B: animStuntPoints, animHealth, newWorldDamage, ShowHUDExtra (standalone) + UpdateBattlePowerUps (dot_runtime_b_resource_callbacks_20261012 superseded in place) | +6,120 | 105/193 | 25 -> 30/49 | yes (0ce59e845, with 8754d42bc) |
| 5 | cc56165ba | w36c updateSelectButtons/controlSelect@recordscr + w36b initRecordScreen (+stub) merged into dot_runtime_a_callers_deep_20261011 with the installed w36f revision; w36c controlSelect@playerscr (dot_runtime_a_availability_refresh_20261007); w36b setGhosts/updateCarComponents (dot_cleanup_colortrace_20261011) | +14,824 | 105 -> 112/193 | 30/49 | yes (cc56165ba) |
| - | 0cad8af34 | owner: research receipt re-pins for the three held items | 0 | | | with batch 6 |
| 6 | dabdcf959 | formerly held: w36f updateButtons/updateModeScreen@modescr (seven_option group), w36j RemoveHUDExtra (B), w36m animResults/animFlag/animLapTimeText/func_80106874 (game credits_scroll_grp) | +2,568 A, +380 B, +6,224 game | 112 -> 114/193 | 30 -> 31/49 | yes (dabdcf959, with 0cad8af34) |
| 7 | 65d3be09b | w36e trackscr (dot_runtime_a_c93c_closure_20261012: 11 new claims incl. w36d's BBB4/DFEC) + startscr initScreen (w35d_start_screen); w36h shots (NEW group w36h_shots, image B, 10 members) | +10,376 A, +12,360 B | 114 -> 126/193 | 31 -> 41/49 | yes (fb77b6f9e) |
| 8 | fb77b6f9e | game: w36l StartEnvironment (music_control) + AnimateNewBlast (entity_update_callback) singles; w36o InitSystem (game_late_init, codex_heap_release_a25 superseded; force_internal HAL_InitText_Init, DeallocateMemory_Init); w36u func_8009F058 (frontier_particle_knot superseded) | +9,224 game (1094 -> 1098) | | | yes (fb77b6f9e) |
| 9 | 82c7fe44c | game: w36s Resurrect cluster (NEW w36s_resurrect, 5 fns); w36v TB_DrawTextScrollBox single + CylinderInSurf (frontier_shot_world_collision, provisional entry retired); w36q initScreen@audioscr (audio_options_menu); w36t AnimateShadow + InitWaves singles, LoadCars (NEW w36t_loadcars), SavePlayerOption/_SaveRaceSetting (NEW w36t_save_option) | +17,056 game (1098 -> 1110) | | | yes (bf8210efa, with w36x) |
| 10 | e013f7ac4 | game: w36w SetControls (drone_target_update single), _ProcessSequences (engine_sound_sync superseded), VectorToSurfEdge (func_800AD4C8 claim only) | +1,140 game (1110 -> 1113) | | | yes (e3faa54a1, with w36x2) |
| 11 | dbcc64cd5 | game: w36x4 StartNewBlast/AnimateBlast/StartBlast (NEW w36x4_blast) | +3,040 game (1113 -> 1116) | | | yes (dbcc64cd5) |
| 12 | 559503fb7 | game: w36p _UpdateGame + _CalcPolePositions (NEW frontier_update_game; provisional entry retired); w36x3 update_game_data + _UpdateTireAppearance (NEW w36x3_update_game_data; provisional entry retired) and FindWinner (single); w36w demo pair (backed out in 12b) | +9,188 game (1116 -> 1123) | | | yes (bb34a8d5e) |
| 12b | bb34a8d5e | back out w36w_demo_pair (ipakit sigs accuracy floor; held) | -1,064 game (1123 -> 1121) | | | yes (bb34a8d5e) |
| 13 | cfe5af449 (pushed in 5eb7d857d) | game, the formerly held selector-caller set after 96e39b502: w36o ShowHUD/animGear/animCountdownTimer (NEW groups; w36o_countdown from w36i's copy, context += camera_shake_update), w36q particle_collision/particle_render (physics_sym) + setPauseState (NEW frontier_pausescr_state), w36z drawRaceStandings (frontier_results_table), w36t _DrawText (codex_defaults_a16, w36i keep fix), w36w animPanes/animElapsedTimer (credits_scroll_grp merge), w36k controlscr (NEW w36k_controlscr, 8 members incl. drone_throttle_calc via the window proof; CLAIMS row) | +19,720 game (1121 -> 1138) | | | yes (5eb7d857d) |
| 14 | 019281a78 (pushed in 5eb7d857d) | game: w36l AnimateRocketFlame + AnimateFrame (codex_visual_tire_module_20261002 superseded; six locked singles re-spliced with struct forms, same bytes), w36x4 CreateCar (music_seq_load single); image A: w36e LoadSelectCar (func_80390BC0 single) | +7,392 game (1138 -> 1141), +364 A (126 -> 127) | 126 -> 127/193 | 41/49 | yes (5eb7d857d) |
| 15 | 427cb7673 (pushed in 5eb7d857d) | game, after the owner's ca46e75ea / a28e8705f: w36x3 animCoins (NEW w36x3_coins; second extern_data D_80118E20 entry, blob_unit now keys extern_data on (name, file)); w36x5 SetupWaveModel, _ReadWave, LoadWave, ReplaceWave, fp_call_wrapper + 3 stub bodies (audio_frame_sync superseded) | +3,876 game (1141 -> 1147) | | | yes (5eb7d857d) |
| 16 | bb6b2f054 (pushed in 5eb7d857d) | game: w36w3 func_800E7038 + func_8010FBE0 (singles), func_800E7134 (controller_poll superseded), uninitScreen@audioscr (credits_screen, audio_options_menu superseded) | +1,352 game (1147 -> 1151) | | | yes (5eb7d857d) |
| 17 | (this) | game, after 82000793f: w36w2 voice_stop (frontier_pausescr_state superseded), func_800F857C/audio_update_d/func_800F87A0/finish_state_alt (NEW w36w2_gameover), game_loop + countdown_handler (codex_input_aux superseded) with frontier_update_game refreshed (w36i copy: _EndGame stub func_800FBE20 as member, single reverted); w36w3 dust_cloud_effect (NEW w36w3_dust) | +4,416 game (1151 -> 1159) | | | |

## Held for owner

### (resolved) receipt follow-ups for batches 13-16 — owner applied them in 5eb7d857d; pushed

Rocky on 427cb7673 (batches 13-15) fails exactly these, all receipt/test bindings:
- `tests/conveyor/test_dot_substantial_scout.py::test_native_metadata_replays_complete_current_extents`: the scout's
  `native_audit.json` records accepted_name_lock false for func_800D91A0 / func_80100E58 (my earlier
  substantial_scout_held.patch only updated the test's landed set, not the receipt).
- `tests/cloud/test_slot_contract_audit.py::test_changed_acceptance_status_changes_bound`: its synthetic
  acceptance of func_800D9058 changes nothing now that func_800D9058 is locked.
  Fix for both: `held/batch13_receipt_followups.patch` (native_audit.json regenerated by the scout's own audit();
  the synthetic test picks the first caller the receipt still lists as selector-only).
- `tests/cloud/test_mat3_shot_match.py` (3 tests): `dot_mat3_shot_match_20261017/proof.json` binds the hash of
  tools/conveyor/pipeline/blob_unit.py, which batch 15 changed (extern_data keyed on (name, file), needed for
  w36x3_coins' second D_80118E20 entry). Fix: `held/shot_match_blob_unit_rebind.patch` (tooling hash rebound,
  handoff section 5 precedent).

### (resolved) w36w2 x3 and w36w3 dust — owner re-pin 82000793f; landed in batch 17

w36w2 frontier_pausescr_state + voice_stop, NEW w36w2_gameover (func_800F857C, audio_update_d, func_800F87A0,
finish_state_alt), codex_input_aux superseded (game_loop, countdown_handler) with the refreshed
frontier_update_game copy; w36w3 NEW w36w3_dust (dust_cloud_effect). voice_stop, func_800F857C, countdown_handler and
dust_cloud_effect are slot_state_setup callers. Fix: `held/slot_contract_audit_w36w2_w36w3_followup.patch`
(evidence simulated with all eight claims locked; stacks after batch13_receipt_followups.patch). ipakit floor with
w36w2's func_800FBE20 stub superseded: 754/802 (passes).

### Summary: the selector-caller set (one owner patch, one landing) — LANDED in batch 13 after the owner's 96e39b502

`held/slot_contract_audit_held_callers.patch` regenerates the slot contract audit evidence (and the test's
accepted list / counts) for ALL of these landed together; `held/substantial_scout_held.patch` adds func_80100E58
and func_800D91A0 to that scout's landed set. Apply both, then land in one batch:

| packet | command | bytes |
|---|---|---|
| w36o_showhud / w36o_animgear / w36o_countdown | `install_group.py cloud/work/frontier/w36o/groups/<g> <g>` (overrides from w36o/RESULTS.md first, incl. extern_data D_80118E20 for w36o_countdown/countdown.c) | 3,444 |
| w36q physics_sym (particle_collision, particle_render) | `install_group.py cloud/work/frontier/w36q/groups/physics_sym physics_sym` | 1,460 |
| w36q frontier_pausescr_state (func_800B4FB0) | `install_group.py cloud/work/frontier/w36q/groups/frontier_pausescr_state frontier_pausescr_state` | 1,472 |
| w36z frontier_results_table (func_80100E58) | `install_group.py cloud/work/frontier/w36z/groups/frontier_results_table frontier_results_table` | 2,732 |
| w36t codex_defaults_a16 (audio_doppler_calc) | `install_group.py cloud/work/frontier/w36i/groups/codex_defaults_a16 codex_defaults_a16` (+ force_internal audio_doppler_calc) | 1,124 |
| w36w credits_scroll_grp merge (game_timer_display, checkpoint_hit) | `install_group.py cloud/work/frontier/w36i/groups/credits_scroll_grp credits_scroll_grp` | 932 |
| w36k controlscr (7 claims; drone_throttle_calc only if blob_group's window proof accepts) | `install_group.py cloud/work/frontier/w36k/groups/controlscr controlscr --revert-single func_800D8154` (+ force_internal gfx_lock_CS, gfx_unlock_CS, font_set_CS; CLAIMS.md landed row crediting Astra PR #322) | 7,532 (+1,024) |

The evidence was simulated with every claim above locked (identical with or without drone_throttle_calc) and
with the credits_scroll_grp group.json exactly as install_group writes it from w36i's merged packet. Each packet
still gets the normal unit check at landing (context copies must keep what their home groups keep; see the
codex_defaults_a16 note).

### Other held items (details below)

| item | blocker | prepared |
|---|---|---|
| w36l codex_visual_tire_module_20261002 (AnimateRocketFlame, AnimateFrame; 5,964 B) + six locked-single source replacements, then w36x4 music_seq_load (1,428 B) | two receipts pin the singles' current sources | `held/w36l_locked_singles_superseded.patch` |
| w36u w36u_audio_init_sys (AudioInitSys, 836 B) | new absolute ROM-offset symbol D_00039B40 (owner decision on naming/route) | note below |
| w36w w36w_demo_pair (revised; 1,064 B) | ipakit sigs accuracy floor (removing 4 stub singles makes return class exactly 0.94) | note below |
| w36x3 NEW w36x3_coins (particles_update = animCoins, 1,464 B) | another slot_state_setup caller: changes the audit census again on top of the owner's re-pin | `held/slot_contract_audit_w36x3_coins_followup.patch` (applies AFTER slot_contract_audit_held_callers.patch) |
| w36x5 audio_frame_sync superseded (SetupWaveModel, LoadWave, ReplaceWave, fp_call_wrapper, func_80097164 + 3 stub bodies; 2,412 B) | `test_runtime_a_resource_cache::test_complete_frozen_replay`: the packet binds src/blob/groups/audio_frame_sync/group.c (helper_contracts source_sha256) | `held/resource_cache_superseded_audio_frame_sync.patch` (superseded copy + resolver; verify.py hash re-pinned). ipakit floor checked: 753/801 = 0.94007 after it, also with the held batch (no margin left for further stub supersessions) |
| w36e func_80390BC0 (LoadSelectCar, 364 B) | resource-cache receipt asserts the cloud/matches file absent | `held/resource_cache_allow_promoted_80390BC0.patch` |

### w36k: NEW group controlscr (updateControlScreen func_800D91A0, func_800D816C, func_800D9058, ...)

- Not landed: func_800D9058 is a slot_state_setup caller (slot contract audit) and func_800D91A0 is a
  dot_substantial_scout target (`test_dot_substantial_scout.py`: accepted_name_lock must equal the landed set).
  Both are covered by the two held patches above.

The owner applied the three receipt patches below himself (0cad8af34) and the items landed in batch 6.

### w36f: func_803993EC (updateButtons@modescr.c) + func_80399D10 (updateModeScreen)

- Failing test (rocky): `tests/cloud/test_runtime_a_capacity_evidence.py::test_runtime_a_capacity_evidence`
  -> `ValueError: inherited genuine group source differs from production`
  (`cloud/work/frontier/dot_runtime_a_capacity_evidence_20261012/verify.py:262-265`). The receipt compares
  its inherited eligibility.c with `src/ovl_a/groups/dot_runtime_a_seven_option_group_20261007/eligibility.c`;
  w36f's revision extends that file in place.
- Batch 3 re-installed the pre-batch-2 group from
  `cloud/work/frontier/superseded/dot_runtime_a_seven_option_group_20261007/` (byte-identical to a14da0b24;
  README added), so production matches the receipt again.
- Prepared fix (not applied): `cloud/work/frontier/w36i/held/capacity_evidence_superseded_seven_option.patch`.
  verify.py accepts the superseded copy when production differs (eligibility.c and renderer.c must equal
  the packet's, and production must still lock every member the packet claims); verification.json
  re-pins verify.py (0c95de4b... -> 1074fae3...). Handoff section 5 "superseded context group" pattern.
- To land after the patch: `git apply cloud/work/frontier/w36i/held/capacity_evidence_superseded_seven_option.patch`,
  then `python3 -m tools.conveyor.pipeline.ovl_splice group --targets asm/us/ovl_a --claims all
  cloud/work/frontier/w36f/groups/dot_runtime_a_seven_option_group_20261007`, gates, tests (+2,568 B, A +2).

### w36j: func_8039244C (RemoveHUDExtra@hudextra.c, 380 B, standalone strict MATCH)

- Failing test if landed: `tests/cloud/test_runtime_b_teardown_packet.py::test_packet_is_research_only`
  asserts `cloud/matches/ovl_b/func_8039244C.c` does not exist (frozen NONMATCH research packet
  cloud/work/runtime_b_teardown_20261006, 34 words).
- Not spliced; `cloud/matches/ovl_b/func_8039244C.c` stays untracked where w36j left it (this is also why the Pi
  run fails that test; rocky's clean clone does not see it).
- Prepared fix (not applied): `cloud/work/frontier/w36i/held/teardown_packet_allow_promoted_8039244C.patch`
  (handoff section 5 "function not yet locked" pattern: the file may exist once func_8039244C is locked from
  src/ovl_b/func_8039244C.c).
- To land after the patch: `python3 -m tools.conveyor.pipeline.ovl_splice splice cloud/matches/ovl_b/func_8039244C.c`,
  gates, tests (+380 B, B +1).

### w36m: credits_scroll_grp superseded in place (animResults, func_80109F54, func_801084D4, func_80106874; 6,224 B)

- Not landed. Failing test if landed: `tests/cloud/test_slot_contract_audit.py::test_fresh_source_bound_census`
  (`cloud/work/frontier/dot_slot_contract_audit_20261006/audit.py` binds the sha256 of
  `src/blob/groups/credits_scroll_grp/group.json`, and its summary's `unaccepted_caller_bytes` /
  `bounded_dependency_bytes` move when slot_state_setup callers land: 36820 -> 30596, 4520 -> 3016).
- Prepared fix (not applied): `cloud/work/frontier/w36i/held/slot_contract_audit_w36m_credits_scroll.patch`
  (evidence.json regenerated by the audit's own collect() against the landed state, simulated: group.json as
  install_group writes it without --provenance, the four names locked; test accepted list and counts updated).
  Land with `PYTHONPATH=. python3 cloud/work/frontier/tools/install_group.py
  cloud/work/frontier/w36m/groups/credits_scroll_grp credits_scroll_grp`, then the game gates and `blob_rom rom`.

### w36e: func_80390BC0 (LoadSelectCar@selectcarload.c, 364 B, standalone strict MATCH)

- Not spliced (`cloud/matches/ovl_a/func_80390BC0.c` stays untracked). Failing test if landed:
  `tests/cloud/test_runtime_a_resource_cache.py::RuntimeCachePacket::test_frozen_artifact_hashes_and_honest_claim`
  (asserts the file is absent; frozen NONMATCH packet dot_runtime_a_resource_cache_20261006).
- Prepared fix (not applied): `cloud/work/frontier/w36i/held/resource_cache_allow_promoted_80390BC0.patch`
  (promoted state allowed: the file may exist once func_80390BC0 is locked from src/ovl_a/func_80390BC0.c).
- To land after the patch: `python3 -m tools.conveyor.pipeline.ovl_splice splice cloud/matches/ovl_a/func_80390BC0.c`.

### w36l: codex_visual_tire_module_20261002 superseded (entity_tick_main = AnimateRocketFlame, drone_ai_update = AnimateFrame)

- Not landed. The packet needs six locked singles' sources replaced (w36l/singles/; same bytes, struct forms that
  umerge inlines). `splice_singles.py NAME=PATH` re-splices a locked single through `blob_splice.splice()` (which
  supports re-splicing: the old entry is dropped and the new object gated), so the tooling is there, but two
  receipts pin the current sources:
  `tests/conveyor/test_dot_slot_setter_boundary_20261006.py::test_receipt_and_input_hashes` (input_hashes of
  src/blob/func_8008E06C.c and src/blob/func_80092BC8.c) and
  `tests/conveyor/test_dot_source_boundary_scout.py` (`scout.check_inputs`: source_inputs of
  src/blob/func_8008AE48.c and src/blob/func_8008AE64.c).
- Prepared fix (not applied): `cloud/work/frontier/w36i/held/w36l_locked_singles_superseded.patch` (handoff
  section 5 superseded-copy pattern: byte copies of the four current sources in
  cloud/work/frontier/superseded/w36l_singles/, and both packets resolve a recorded src/blob path there when
  production differs; recorded paths unchanged; verify.py hashes re-pinned where a receipt binds them).
- To land after the patch: `PYTHONPATH=. python3 cloud/work/frontier/tools/splice_singles.py
  func_8008B3A0=cloud/work/frontier/w36l/singles/func_8008B3A0.c ...` (all six), then
  `install_group.py cloud/work/frontier/w36l/groups/codex_visual_tire_module_20261002 codex_visual_tire_module_20261002`,
  `blob_unit check` (w36l: 12/12 EQUAL), `blob_rom rom` (+5,964 B).
- Depends on it: w36x4's standalone `cloud/matches/music_seq_load.c` (CreateCar, 1,428 B) is unit EQUAL only with
  w36l's struct-form func_8008E06C/func_80092BC8/func_80092BF4 (256/357 with the current singles); land it right
  after the w36l batch (`splice_singles.py music_seq_load`).

### w36o: NEW groups w36o_showhud (skid_mark_render = ShowHUD), w36o_animgear (func_800EF288 = animGear), w36o_countdown (emitter_update = animCountdownTimer)

- Not landed. All three are slot_state_setup callers, so landing them moves
  `tests/cloud/test_slot_contract_audit.py::test_fresh_source_bound_census` (summary unaccepted_caller_bytes /
  bounded_dependency_bytes; skid_mark_render leaves the only-selector dependency list) and the counts in
  `test_counts_are_bounded_dependency_surface_not_matches`.
- Prepared fix (not applied): `cloud/work/frontier/w36i/held/slot_contract_audit_w36o.patch` (evidence.json
  regenerated by the audit's collect() with the three names locked; accepted list and counts in the test).
- To land after the patch: create the three groups with `install_group.py` from cloud/work/frontier/w36o/groups/,
  add the unit_overrides from w36o/RESULTS.md first (force_internal *_ShowHUD/*_Gear/*_Countdown helpers;
  extern_data D_80118E20 for src/blob/groups/w36o_countdown/countdown.c), then the game gates (+3,444 B).
  w36o's codex_heap_release_a25 (InitSystem) is not a selector caller and landed in batch 8.

### w36u: NEW group w36u_audio_init_sys (car_shadow_render = AudioInitSys, 836 B)

- Not landed: the body references a new absolute symbol D_00039B40 (asset ROM offset 0x39B40, `lui 0x4; addiu
  -0x64C0`). The only ROM-offset symbol the game link knows is gRomCompressedData (symbol_addrs.us.txt +
  `PROVIDE` in the generated src/blob/blob.ld, from the static boot code); there is no game-code precedent, and adding
  one means regenerating asm/us/blob/symbols.json / blob.ld through the context tools, which many receipts bind.
  Owner decision needed: name and route for asset ROM-offset symbols in game code (symbol_addrs.us.txt +
  GAME_SYMBOLS + context refresh), then land with `install_group.py cloud/work/frontier/w36u/groups/w36u_audio_init_sys`.

### w36q: physics_sym superseded (particle_collision, particle_render) and NEW frontier_pausescr_state (func_800B4FB0); w36z: frontier_results_table superseded (func_80100E58 = drawRaceStandings)

- Not landed: particle_render, func_800B4FB0 and func_80100E58 are slot_state_setup callers (same slot contract
  audit tests as w36o). func_80100E58 also trips `tests/conveyor/test_dot_substantial_scout.py`
  (`landed = {'func_80102F30'}`: accepted_name_lock must stay False for func_80100E58).
- Prepared fix (not applied): one combined patch for all six held selector callers (w36o x3, w36q x2, w36z x1) and w36t's _DrawText:
  `cloud/work/frontier/w36i/held/slot_contract_audit_held_callers.patch` (replaces slot_contract_audit_w36o.patch)
  plus `cloud/work/frontier/w36i/held/substantial_scout_held.patch`. Landing order after the owner applies
  them: all seven must land together (the regenerated evidence assumes all seven locked).
- particle_collision (not a selector caller) shares physics_sym with particle_render; it waits with it.

### w36t: codex_defaults_a16 superseded (audio_doppler_calc = _DrawText, 1,124 B): packaging fix, then held

- Packaging: the lane's packet copies slot_sound/group.c as slot_sound.c without slot_sound's keep entries;
  installed, the derivation (a function a group defines and does not keep is internal everywhere) turned eight
  kept slot_sound functions internal and the unit failed 15 locked bodies. w36i's copy
  (`groups/codex_defaults_a16/`) adds the eight to keep (as w36u_audio_init_sys does); builder: both members
  MATCH (`score_codex_defaults_a16_keepfix.txt`), unit 1111/1111 equal. Land from that copy.
- Held: landing _DrawText leaves world_velocity_integrate (HAL_FinishText, unaccepted) with no other unaccepted
  callee, so it enters the slot contract audit's only-selector dependency list and
  `test_fresh_source_bound_census` fails. Backed out of batch 9 (blob_group revert, installed group re-spliced, lock
  entry restored exactly, force_internal audio_doppler_calc removed); it is included in
  `slot_contract_audit_held_callers.patch` (the regenerated evidence assumes it landed with the six callers).

### w36w: credits_scroll_grp merge (game_timer_display = animPanes@audioscr.c, checkpoint_hit = animElapsedTimer@hud.c)

- Merged onto the installed w36m revision in `groups/credits_scroll_grp/` (w36w's gr3_d.c and gr3_73d8.c; claims
  game_timer_display + checkpoint_hit only; time_result_display stays context/keep and gr3_trd.c is dropped, per the
  coordinator). Builder, no --claims: all 16 members MATCH (`score_merged_credits_scroll.txt`).
- Held: both are slot_state_setup callers and the audit binds credits_scroll_grp/group.json. Folded into
  `slot_contract_audit_held_callers.patch` (its evidence uses the group.json hash install_group writes from this
  packet). Land with `install_group.py cloud/work/frontier/w36i/groups/credits_scroll_grp credits_scroll_grp`.

### w36w: NEW group w36w_demo_pair (debug_collision = updateDemoGame, debug_stats = playDemoGame + four stub bodies)

- Installed, then backed out (blob_group revert; stub singles and lock entries restored from HEAD; overrides
  removed): the packet's context copy frontier_path_D169C_path.c defines D_80152818, which the locked
  frontier_path_D169C/path.c already defines, so the shadow unit fails (`uld saw a name defined twice ...
  D_80152818: multiply defined`). The established fix is an `extern_data` override for the copy, but
  frontier_car_checkpoints' copy already holds the only D_80152818 entry and `blob_unit.validate_overrides`
  rejects a second entry for the same name ("extern_data lists D_80152818 twice").
- Revised by w36i (`groups/w36w_demo_pair/`): the context copy declares D_80152818 `extern` instead (it is
  context, not a member; func_800D169C is on keep, so the members do not depend on its lui sharing). Builder: all
  six members MATCH (`score_w36w_demo_pair_extern.txt`); unit 1123/1123 equal. Landed in batch 12, backed out in
  12b: removing the four stub singles drops `tests/cloud/test_ipakit_sigs.py::test_validation_accuracy_floor`'s
  return-class ratio to exactly 0.94 (752/800 on a clean clone; the test needs > 0.94). The stubs are four correct
  `void f(void)` predictions in ipakit's src/blob ground truth. Needs the owner (floor or ground-truth rule) or
  more correctly predicted singles landing first; the revised packet is ready.

## Batch 5 merge (dot_runtime_a_callers_deep_20261011)

Packet `groups/dot_runtime_a_callers_deep_20261011/`: the installed group.c (w36f: func_803923B0 member) with
w36c's 9338C region and 915E8 block and w36b's 93B3C/93B34 region taken verbatim (three-way base a14da0b24;
the only shared-declaration edit, volatile D_80140BDC / D_8002EB94, is identical in all three). Builder
(`score.py group`, no --claims): all 11 members MATCH; `--claims` rc=0 (`score_merged_callers_deep_claims.txt`).
The builder also rescored w36c's availability_refresh revision (9 members MATCH, context 9842C 2/451), w36b's
carscr revision (14 members MATCH) and the unchanged runtime_a_profile_menu_20261006 (4 members MATCH;
96FA0 is a stand-in context there): `score_batch5_groups.txt`.

## Wave 37 (branch wave37, from c12837976)

| Batch | Commit | What | Bytes | Pushed |
|---|---|---|---|---|
| w37-1 | f7a89ab92 | w37d func_800E73D8 (standalone; disclosed `do {} while (0);`) | +596 game (1159 -> 1160) | yes (f7a89ab92) |
| w37-2 | 0adb25284 | after 78b60b453: w37d func_800979A0 (slot_sound superseded; disclosed holder `if (D_8011EAA0) {}`), w37b replay_save_prompt = initScreen@optionscr.c (frontier_optionscr_buttons superseded; disclosed unused `u0`) | +1,832 game (1160 -> 1162) | yes (0adb25284) |
| w37-3 | (this) | w37a Controller Pak (NEW w37a_paksys from w36i's copy, coordinator decision for the owner): PS_CreateFile, no_catchup (single reverted), func_800A3724 = _ClearPak; PS_PakServer/PS_InitPakSys/_RemovePak stay unmatched context. D_00039B40 for AudioInitSys skipped: adding it means regenerating asm/us/blob/symbols.json/blob.ld, which several receipts bind | +1,840 game (1162 -> 1164) | |

Formerly held, landed in w37-2 after the owner's 78b60b453 (patches were applied with `git apply <patch>` from the repo root, then commit as the owner):
- w37d slot_sound superseded with func_800979A0 (292 B): breaks `dot_sequence_start_979a0_20261006`
  (`test_verification.py::test_frozen_receipt_replays`: verify.py requires the packet group to extend exactly the
  installed slot_sound). Patch: `held/sequence_start_979a0_superseded_slot_sound.patch` (byte copy of the current
  slot_sound in superseded/slot_sound/, verify.py resolves it there, verification.json re-pins verify.py).
  Then land: `install_group.py cloud/work/frontier/w37d/groups/slot_sound slot_sound`. Landing 979A0 alone keeps
  the slot contract audit valid (simulated).
- w37b frontier_optionscr_buttons superseded with replay_save_prompt (= initScreen@optionscr.c, 1,540 B): a new
  accepted slot_state_setup caller. Patch: `held/slot_contract_audit_w37b_replay_save_prompt.patch` (evidence +
  test counts; simulated identical with or without func_800979A0 landed, so the two patches are independent).
  Then land: `install_group.py cloud/work/frontier/w37b/groups/frontier_optionscr_buttons frontier_optionscr_buttons`
  (+ force_internal per w37b/RESULTS.md).
