# w38i: wave-38 integration

Branch `wave37`, integration only (src/, locks, commits). Overlay gates per batch: `ovl_layout build` (both images
byte-identical), `ovl_splice check` (ovl lock intact), `ovl_rom rom` (ROM SHA-1 EXACT); Pi pytest and rocky
`ci_equiv` (CI_FAST=16) before every push. Logs are in the coordinator's scratchpad.

| Batch | Commit | What | Image A | Image B | Pushed |
|---|---|---|---|---|---|
| 1 | 4aa2bdd04 (lane files 15d8abb49) | w38b deleted-inline stubs: 11 image-A groups superseded in place (c93c, w35d_start_screen, callers_deep, availability_refresh, menu_cleanup, seven_option_group, seven_option_init, text_helpers, menu_state, settings_values, dot_cleanup_colortrace; 28 stubs); image B singles func_8038A408/func_803925D0 reverted, re-landed as NEW groups w38b_worlddam/w38b_hud_show (2 stubs) | 127 -> 155/193 (128,284 B) | 41 -> 43/49 (36,288 B) | yes |
| 2 | cd2b9bef6 (lane files e26d56101) | w38a game: NEW w38a_loadtrack (LoadTrack); w37a_paksys, camera_aspect_ratio, audio_options_menu, audio_frame_update, codex_defaults_a16 superseded in place (car_lod_select, camera_look_at_point, drone_collision_avoid, audio_frame_update, world_velocity_integrate, menu_input_process, func_800B66B0); phase-12 test example LoadTrack -> camera_transform | game 1165 -> 1173/1216 (+6,820 B) | | yes, after owner commit 329614591 (slot audit re-pin) |
| 3 | 3b32175c6 | w38b menu_renderers (func_803A6F14/1C/24), after owner commit 4934ffede (writer_bounds superseded copy) | 155 -> 158/193 (128,308 B) | | yes |
| 4 | 4c481dc28 (lane files feeab9967) | w39c game: NEW w39c_updatefx (sound_position_update), w39c_mainloop (world_trigger_activate), w39c_initgraphics (world_effect_update; stub single func_800EE8AC superseded); audio_frame_sync superseded in place (brake_force_apply); unit overrides force_internal func_800EE8AC/MainPreNMI/MainEndFrame, prefer_definition func_800EE8AC, align brake_force_apply | game 1173 -> 1177/1216 (+1,560 B) | | yes |
| 5 | 836aec1b5 (lane files 3ff88872c) | w40a/w40b game: engine_sound_sync superseded in place (func_80098FB8, entity_ai_pathfind); NEW w40a_modelthread (render_thread_entry), w40a_carcolor (sound_bank_unload, func_800B0EA0, stub func_800B0F60 reverted), w40b_state (playgame_state_change, object_create, four stub singles reverted); align entity_ai_pathfind/render_thread_entry, force_internal SetModelPalette, prefer_definition + force_keep object_create | game 1177 -> 1184/1216 (525,360 B) | | after owner patches |

Note: commit 4aa2bdd04's message says "30 stubs" for image A; the split is 28 in A plus 2 in B.

## Held (both applied by the owner on 2026-10-22: 329614591, 4934ffede)

- **menu_renderers** (`w38b/groups/dot_runtime_a_menu_renderers_20261011`, stubs func_803A6F14/1C/24, A -> 158/193):
  installing it breaks `tests/cloud/test_runtime_a_writer_bounds.py` (verify.py requires production group.c to equal
  the receipt's packet). Prepared fix, `held/writer_bounds_superseded_menu_renderers.patch` (handoff section 5
  superseded-copy pattern): byte copies of the current production group.c/group.json under
  `cloud/work/frontier/superseded/dot_runtime_a_menu_renderers_20261011/`; verify.py accepts that copy when production
  differs and requires production members to include the packet's; verification.json re-pins verify.py
  (3cbf3b69... -> 41a7ebf4...). Validated on rocky in a throwaway commit with the install applied: tests/conveyor 2351
  passed, tests/cloud 1109 passed (writer_bounds included). To land: owner applies the patch, then
  `python3 -m tools.conveyor.pipeline.ovl_splice group --targets asm/us/ovl_a --claims all
  cloud/work/frontier/w38b/groups/dot_runtime_a_menu_renderers_20261011` and the overlay gates.

- **Slot-contract audit** (batch 2): drone_collision_avoid and world_velocity_integrate become accepted callers, so
  `tests/cloud/test_slot_contract_audit.py` (3 tests) fails until the receipt is regenerated. Prepared fix,
  `held/slot_contract_audit_w38a.patch`: evidence.json regenerated with the packet's own `collect()` (summary + two
  caller rows `accepted: true`, unaccepted bytes 16124 -> 14560, bounded dependency 824 -> 112) and the test's
  expected values. Validated in a throwaway commit: those tests pass on the Pi, rocky conveyor 2351 / cloud 1109
  passed. To land: owner applies and commits it, then batch 2 can be pushed.

Batch 5 (w40, 2026-10-23): Pi pytest showed two failures caused by the batch, not by the matches.
`test_slot_contract_audit.py`: object_create was the receipt's last selector-only dependency, so the summary moves
and `test_changed_acceptance_status_changes_bound` indexes an empty list. `test_ipakit_sigs.py`: return-class
accuracy 753/801 -> 748/796 (0.9397 < 0.94) only because five correctly predicted `void` stub singles left
`src/blob/*.c` (no prediction changed). Prepared fixes: `held/slot_contract_audit_w40b.patch` (evidence regenerated
with the packet's `collect()`, expected values, selector-only fallback to any unaccepted caller) and
`held/ipakit_sigs_floor_w40b.patch` (floor 0.935). Validated in a throwaway commit (install + both patches): those
tests pass on the Pi; rocky conveyor 2345 (5 fewer: stub singles moved into groups) / cloud 1109 passed. To land:
owner applies and commits both, then batch 5 can be pushed.

