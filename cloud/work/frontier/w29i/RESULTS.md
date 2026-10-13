# w29i — game-side integration (wave 29, worktree, branch `w29i`)

Merged wave29 `199ad39b` (Job 1) and `09496631` (Job 2) into w29i. Static 632/667 (151,176 B, 94.06%), overlay A
81/193 (51,060 B) and overlay B 21/49 (9,020 B) unchanged throughout. Game coverage from `make progress`:

| Step | Install | Bytes | Game after |
|---|---|---:|---|
| (start) | — | — | 1024/1216, 352,068 B (54.41%) |
| 1a | NEW group `frontier_results_table` (w29a): func_80102F30 (kept) 3,560, func_8010221C 548, func_80101904 1,152, func_80101D84 1,176, func_80102448 1,336, func_80100D5C 252 | 8,024 | 1030/1216, 360,092 B (55.65%) |
| 1b | `frontier_billboard_render` superseded in place (w29a): func_800F0F44 context -> members | 464 | 1031/1216, 360,556 B (55.72%) |
| 2a | NEW group `func_800E56F8` (w29c): func_800E56F8 1,436 (internal), func_800E6AF8 1,336 (kept) | 2,772 | 1033/1216, 363,328 B (56.15%) |
| 2b | `credits_scroll_grp` superseded in place (w29c): + lap_complete (gr3_761c.c) | 1,232 | 1034/1216, 364,560 B (56.34%) |
| 2c | single func_800EB028 (w29c, cloud/matches) | 1,640 | **1035/1216, 366,200 B (56.59%)** |

Total: 11 functions, 14,132 B.

## Confirmation before installing
- Pi unit, both w29a packets together (every packet file `--with`, ctx_gr3_b.c once, `--internal` for the five
  results members, rt_*, hs_*, pak_slot_b1/b5/b6, `--neighbours`): `blob_unit score: 10/10 equal`,
  `locked bodies that differ in this unit: 0` (F30 890 kept, 221C 137, 1904 288, D84 294, 2448 334, D5C 63,
  F0F44 116, F1210 454, F0914 396 internal, billboard_render 244 kept).
- Pi unit, all w29c pieces together (group.c, gr3_761c.c, func_800EB028.c; `--internal func_800E56F8
  update_drone_models update_model`): `blob_unit score: 4/4 equal` (E56F8 359 internal, E6AF8 334 kept,
  lap_complete 308 kept, EB028 410 kept), `locked bodies that differ in this unit: 0`.
- watchman2 scratch `w29i` (IDO 5.3 796ae99a, 2 cores; deleted afterwards), `score.py group … --claims`:
  frontier_results_table 6 MATCH rc=0 (own .rodata verified F30 0x80124864..0x80124880, 221C 0x80124848..0x80124864);
  frontier_billboard_render func_800F0F44 MATCH rc=0; func_800E56F8 group both MATCH rc=0; credits_scroll_grp all 8
  members MATCH rc=0 (without --claims) and lap_complete MATCH with --claims; `score.py fn` func_800EB028 MATCH, own
  .rodata verified at 0x8012451C..0x80124558.

## Gates after each step (all identical in form)
`blob_group check`: `group lock: 0 problems`; `blob_splice check`: `blob lock: N entries, 0 problems`;
`blob_unit --tag w29i check`: `N locked bodies, N equal to the image …, 0 differ` (N = 1030, 1031, 1033, 1034,
1035); `blob_rom rom`: `ROM build with the source-built blob: SHA-1 EXACT`, `build/us/rush2049.us.z64: OK`.

## Integration details
- unit_overrides: `force_internal` added ONLY for func_800E56F8, update_drone_models, update_model (2a). Everything
  else the prompts listed (the five results members, rt_gfx_lock/rt_gfx_unlock/rt_font_set/rt_font_set_b,
  pak_slot_b1/b5/b6, the *_761C helpers) is internal by the group rule (not on keep): verified in the tag manifest
  (`internal` lists) and the unit counts (internal 219 -> 228 -> 231 -> 234 -> 238), `unused_overrides: []`.
- Context-copy trap: ctx_gr3_b.c in frontier_results_table is covered by the existing `force_keep func_800B669C`;
  object_create/slot_state_setup already internal; `blob_unit check` clean. After 2b, credits_scroll_grp/gr3_b.c is
  still byte-identical to both ctx_gr3_b.c copies (cmp).
- symbols.json / blob.ld unchanged by every splice (only SHA256SUMS and the provenance comments in blob_*.s): no
  receipt re-pin. Slot-contract audit `audit.py --write`: 1a (+5 accepted callers, unaccepted_caller_bytes 68260 ->
  60488), 1b (F0F44 off a call list, counts unchanged), 2b (group.json pin, lap_complete accepted, 60488 -> 59256);
  frozen counts in tests/cloud/test_slot_contract_audit.py updated; 2a/2c no diff.
- provisional.json: no entry for any of the 11 functions (nothing retired). No other stand-in record found.
- CLAIMS.md: new "landed wave 29" row with all 11 functions and every disclosed device; the w27c/w28a results rows
  merged into one open row (w29a drafts E58/2980/3D28 + not started); the w27b row rewritten as the open
  race/world/attract row (w29c race_finish/control_settings drafts; the attract handler unit must land as one group
  superseding codex_control_settings_a4).

## Worktree notes
- The worktree had no `build/`: copied build/{blob,ovl,us,game_code.bin,blob_layout.json,blob_unit.json,…} and the
  blob_unit cache plus baserom.us.z64 from the main checkout. The copied objects for 5 locked groups/singles were
  stale (first install_group refused and restored cleanly); `blob_splice rebuild-stale` fixed it.
- Pi tests ran with the MAIN checkout's tools/mips_to_c and tools/decomp-permuter symlinked in temporarily
  (removed afterwards).

## Tests (branch w29i at d81ca464 + RESULTS)
- Pi `make test-fast JOBS=4` (main checkout's submodules symlinked in temporarily, removed afterwards): tests/conveyor
  `1 failed, 1588 passed, 804 skipped`; tests/cloud `806 passed, 202 skipped`; rc=2.
- rocky clone `~/rush2049/scratch/w29i` (ciclone copy, branch from a bundle, `origin/master` = 8c74d8b6, CI venv),
  `REQUIRE_TOOLCHAIN=1 make test-fast JOBS=16`: tests/conveyor `1 failed, 2340 passed, 52 skipped`; tests/cloud `1004 passed, 4 skipped, 8 warnings, 27 subtests passed`; rc=2.
- The ONE failure on both hosts: `tests/conveyor/test_dot_substantial_scout.py::test_native_metadata_replays_complete_current_extents`.
  The D02 scout receipt `cloud/work/dot_substantial_scout/native_audit.json` freezes `accepted_name_lock: false`
  for every target and the test asserts no target is locked; func_80102F30 is one of its targets and is now locked
  (`accepted_name_lock False -> True`, `accepted_interval_overlaps [] -> ['func_80102F30']`; nothing else changes).
  That receipt and test are outside this lane's edit list, so the fix is NOT committed: `scout_receipt.patch` in this
  directory regenerates native_audit.json (preflight.py output) and changes the assertion to
  `accepted_name_lock == (target in {'func_80102F30'})`; with it applied the file passes 5/5.

## Job 3: w29b packets (merge of wave29 1fededc2 first, incl. coordinator's scout-receipt commit 92aa7850)

| Step | Install | Bytes | Game after |
|---|---|---:|---|
| (start) | — | — | 1035/1216, 366,200 B (56.59%) |
| 3.1 | single game_results_input (= GC animControls) | 1,732 | 1036/1216, 367,932 B (56.86%) |
| 3.2 | physics_sym superseded in place: + sound_loop_set (slp.c) + ctx_gr3_b.c | 332 | 1037/1216, 368,264 B (56.91%) |
| 3.3 | new group func_80106B3C | 600 | 1038/1216, 368,864 B (57.01%) |
| 3.4 | new group func_8010BC84 | 928 | 1039/1216, 369,792 B (57.15%) |
| 3.5 | new group func_8010B7FC | 460 | 1040/1216, 370,252 B (57.22%) |
| 3.6 | new group func_8010B9C8 | 700 | **1041/1216, 370,952 B (57.33%)** |

Job 3 total: 6 functions, 4,752 B. Wave 29 total (Jobs 1–3): 17 functions, 18,884 B, game 1024 -> 1041.

- Unit, all six together (`--with` game_results_input.c, slp.c, f6b3c.c, fbc84.c, fb7fc.c, fb9c8.c, `--internal` every
  helper + sound_loop_set, `--block sound_loop_set`, `--neighbours`): `blob_unit score: 9/9 equal` (GRI 433 kept,
  sound_loop_set 83 internal, physics_sym 313, B5688 49, B59F0 343, 6B3C 150, BC84 232, B7FC 115, B9C8 175),
  `locked bodies that differ in this unit: 0`.
- Builder (scratch w29i, deleted): physics_sym group B55F4/B5688/B59E8/physics_sym/sound_loop_set MATCH (sound_loop_set
  own .rodata verified at 0x80122614..0x80122624), func_800B59F0 `NOT VERIFIED (12 section-relative relocations …)` = the
  known two-region case, rc=1 as for the locked group; `--claims` sound_loop_set MATCH rc=0. The four single-member groups:
  MATCH, rc=0, with and without --claims. `score.py fn game_results_input`: MATCH, own .rodata verified at 0x80120234..0x80120240.
- Gates after each step: `group lock: 0 problems`, `blob lock: N entries, 0 problems`, `blob_unit: N locked bodies, N
  equal …, 0 differ` (N = 1036..1041), `ROM build with the source-built blob: SHA-1 EXACT`, `build/us/rush2049.us.z64: OK`.
- unit_overrides: inline_blockers += sound_loop_set; force_internal += sound_loop_set, gfx_lock_slp, gfx_unlock_slp,
  font_select_slp, font_set_slp, the six *_6B3C, six *_BC84, five *_B7FC and five *_B9C8 helpers (`unused_overrides: []`).
- All seven ctx_gr3_b.c copies are byte-identical to credits_scroll_grp/gr3_b.c; func_800B669C stays kept via force_keep.
- symbols.json/blob.ld unchanged (no receipt re-pin). Slot-contract audit re-written at 3.2–3.6 (sound_loop_set,
  6B3C, BC84, B7FC, B9C8 accepted; unaccepted_caller_bytes 59256 -> 56236); frozen counts in the test updated each time.
- CLAIMS: six functions appended to the wave-29 landed row with devices; w28c menu row rewritten as the open w28c/w29b
  menus row; w27d row points B5D0 there and notes B7FC/B9C8 landed.
- Tests at 0cc13fa9 (Job 3): Pi `make test-fast JOBS=4` (main checkout's submodules symlinked in temporarily, removed):
  conveyor `1589 passed, 805 skipped`; cloud `806 passed, 202 skipped`; rc=0. rocky `REQUIRE_TOOLCHAIN=1 make test-fast
  JOBS=16` (clone ~/rush2049/scratch/w29i, origin/master 8c74d8b6): conveyor `2342 passed, 52 skipped`; cloud `1004 passed,
  4 skipped, 8 warnings, 27 subtests passed`; rc=0. (The scout-receipt failure of Jobs 1–2 is fixed by 92aa7850.)
