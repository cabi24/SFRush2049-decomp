# w32i — game-side integration (wave 32, worktree, branch `w32i`)

Merged wave32 `eb3ccd01` into w32i (fast-forward from d1c53ab2). Worktree build/ copied from the main checkout
(build/{blob,ovl,us,m2c_asm}, build/*.json, game_code.bin/.sym, blob_unit/cache, baserom); `blob_splice rebuild-stale
--builder-dir ~/rush2049/scratch/frontier/w32i/bs --group-builder-dir .../bg` rebuilt 101 stale objects first (`rebuild_stale.log`).
Builder scratch watchman2:~/rush2049/scratch/frontier/w32i (copy of base + rsync; `IDO_DIR` = toolkit 796ae99a ido).

| Step | Change | New bytes | Game after (`blob_rom coverage`) |
|---|---|---:|---|
| (start) | — | — | 1056/1216, 384,828 B (59.47%) |
| 1 | `menu_save_options` superseded in place (w32c): + func_800CC50C (CompressGhost), + props_render (GhostStart) | 2,940 | **1058/1216, 387,768 B (59.93%)** |
| 2 | `codex_control_settings_a4` superseded in place (w32d): + control_cell_text, control_cell_text2, control_cell_text3, func_800DD0C0 (provisional on the caller) | 2,684 | **1062/1216, 390,452 B (60.34%)** |
| 3a | NEW group `frontier_audioscr` (w32b): game_results_render + game_results_update; func_800BEA3C re-typed (`--revert-single`) | 2,336 | **1064/1216, 392,788 B (60.70%)** |
| 3b | NEW group `frontier_optionscr_buttons` (w32b): func_800DB1E0 + func_800DA2C0 | 3,716 | **1066/1216, 396,504 B (61.28%)** |

## Job 1: w32c menu_save_options packet
- Packet check: group.c, save.c, ctx_hash.c, ctx_slot.c, ctx_strncmp.c `cmp`-identical to src/blob/groups/menu_save_options;
  ctx_newghost.c changed as w32c describes (CompressGhost body, typed callee prototypes, `extern f32 D_80111754[]`); props.c new.
  group.json vs installed: members + func_800CC50C, props_render; context - func_800CC50C; keep + props_render; files + props.c.
- Builder (`builder_job1.log`) `score.py group cand/w32i/menu_save_options --claims` and without: rc=0 both, all 24 members
  MATCH incl. `func_800CC50C: MATCH`, `props_render: MATCH`; SaveServer `own .data verified at 0x8011742C`, `0x80117430`,
  `0x80117434 (4 bytes, all zero ...)`. Context: CreatePlayer 246/272, menu_back 3/33 (informational, as before).
- force_internal += ghost_free_start, ghost_blocks_from_size, ReadGhostData (needed: without it the unit kept
  ghost_free_start and func_800CC50C was 153/190 — `refers to ghost_free_start, which has no image address`).
- Unit (`unit_job1.log`) `blob_unit --tag w32i score <24 members> func_800950AC --with P/ctx_newghost.c --with P/props.c --neighbours`:
  `EQUAL func_800CC50C: 190 words (kept, c_ctx_newghost.c)`, `EQUAL props_render: 545 words (kept, c_props.c)`,
  `locked bodies that differ in this unit: 0`, `blob_unit score: 25/25 equal`.
- Install (`install_job1.log`): `spliced menu_save_options: 24 members, new: ['func_800CC50C', 'props_render']`,
  `image coverage: 1058/1216 functions, 387768/647072 bytes (59.93%)`.
- Gates (`gates_job1.log`, `rom_job1.log`): `group lock: 0 problems`; `blob lock: 1058 entries, 0 problems`;
  `blob_unit: 1058 locked bodies, 1058 equal to the image in one 931-file unit, 0 differ`; `unused_overrides []`;
  func_800950AC chosen as the locked single (no force_keep); `ROM build with the source-built blob: SHA-1 EXACT`,
  `build/us/rush2049.us.z64: OK`.
- symbols.json / blob.ld unchanged (asm diff = 2 provenance comments + SHA256SUMS): no receipt re-pin. Slot-contract audit
  `audit.py --write`: no diff. Neither function provisional nor a scout target. No data-symbol changes.
- CLAIMS: wave-32 landed row (func_800CC50C = CompressGhost, props_render = GhostStart for the rename batch; devices);
  save row rewritten with w32c's pak_delete_file_retry and CreatePlayer levers.

## Job 2: w32d codex_control_settings_a4 packet (merge of wave32 3d458a4c first)
- Packet check: func_800DCD58 / func_800DCCE0 bodies identical to the installed group.c (brace-matched extraction, 512/214 chars);
  ctx_gr3_b.c `cmp`-identical to src/blob/groups/credits_scroll_grp/gr3_b.c; group.json members + the four, context
  + slot_state_setup, object_create, func_800B669C, files + ctx_gr3_b.c, keep [control_settings] unchanged. The old group.c
  (113,977 B, m2c-era control_settings context) is replaced by w32d's 14,216 B file (control_settings = w32d best.c, context).
- force_internal += control_cell_text, control_cell_text2, control_cell_text3, func_800DD0C0, gfx_lock_DC, gfx_unlock_DC,
  font_set_DC, font_set1_DC, pair_set_DC (before install). func_800B669C was already on force_keep (w28a trap) — no new force_keep.
- Builder (`builder_job2.log`) `score.py group cand/w32i/codex_control_settings_a4` with and without `--claims`: rc=0 both,
  all six members MATCH; context informational (control_settings 536/638, slot_state_setup 2/58, object_create 28/28, func_800B669C 5/5).
- Unit (`unit_job2.log`) `blob_unit --tag w32i score control_settings <6 members> slot_state_setup object_create func_800B669C
  --with P/group.c --neighbours`: the six members EQUAL (66/205/173/227/39/28 words, internal), slot_state_setup and
  func_800B669C EQUAL, control_settings FAIL 534/638 (context, expected), object_create 2/28 (not locked, emptied internal),
  `locked bodies that differ in this unit: 0`.
- Install (`install_job2.log`): `spliced codex_control_settings_a4: 6 members, new: ['control_cell_text', 'control_cell_text2',
  'control_cell_text3', 'func_800DD0C0']`, `image coverage: 1062/1216 functions, 390452/647072 bytes (60.34%)`.
- Gates (`gates_job2.log`, `rom_job2.log`): `group lock: 0 problems`; `blob lock: 1062 entries, 0 problems`;
  `blob_unit: 1062 locked bodies, 1062 equal to the image in one 932-file unit, 0 differ`; `unused_overrides []`;
  `ROM build with the source-built blob: SHA-1 EXACT`, `build/us/rush2049.us.z64: OK`.
- symbols.json / blob.ld unchanged (4 provenance comments + SHA256SUMS): no receipt re-pin. Slot-contract audit
  `audit.py --write`: the four become accepted callers, unaccepted_caller_bytes 50352 -> 47668; frozen list/count in
  tests/cloud/test_slot_contract_audit.py updated (passes). Not in provisional.json (only the CLAIMS note, per the coordinator).
- CLAIMS: the four on the wave-32 landed row, marked PROVISIONAL on control_settings' caller form (coordinator decision);
  race/world row: w32d control_settings 636/638 + measured inline-frame law + lead, LoadTrack stop rule; resource-loader row:
  SetupWaveModel j-loop analysis.

## Job 3 (merge of wave32 e9e1359b first): w32b's two new groups
Builder (`builder_job3.log`, both groups, with and without `--claims`): frontier_audioscr — `game_results_render: MATCH`
(`own .rodata verified at 0x8011F6C4..0x8011F6D4`, `0x8011F6D8..0x8011F6E0`), `func_800BEA3C: MATCH`, `game_results_update:
NOT VERIFIED (14 section-relative relocations unverified ...; own .rodata ... 0x8011F6E0 for +0x0/+0x4, 0x80124804 for +0x24..+0x34)`;
frontier_optionscr_buttons — `func_800DB1E0: MATCH` (`own .rodata verified at 0x80124248..0x80124280`), `func_800DA2C0: NOT VERIFIED
(32 section-relative relocations unverified ...)`, `own .bss placed at 0x8015273C`, `0x80152740`. rc=1 only from the two
expected two-region NOT VERIFIED lines; the install's splice proof (blob_group.group_bodies) is the gate for those.
force_internal is NOT derived for non-keep group members/helpers (without it the unit kept au_font_set: game_results_render 264/291
`refers to au_font_set, which has no image address`), so: += game_results_update, au_gfx_lock, au_gfx_unlock, au_font_set,
au_CenterText, au_RightText, au_SetJust, au_SetFont, au_validOption (3a); += func_800DA2C0, ZOID_SetColor_da, ZOID_SetColorP_da (3b).
Unit (`unit_job3.log`, overrides in place): `EQUAL game_results_render: 291 words (kept, c_audio.c)`, `EQUAL game_results_update:
293 words (internal, c_audio.c)`, `EQUAL func_800BEA3C: 7 words (kept, c_audio.c)`, 0 locked differ, 3/3; `EQUAL func_800DB1E0:
350 words (kept, c_buttons.c)`, `EQUAL func_800DA2C0: 579 words (internal, c_buttons.c); own zero-initialised data (addresses
consistent ...)`, 0 locked differ, 2/2.

### 3a: frontier_audioscr
- ctx_gr3_b.c cmp-identical to credits_scroll_grp/gr3_b.c; func_800B669C already on force_keep.
- Install (`install_job3a.log`): `install_group.py cloud/work/frontier/w32b/groups/frontier_audioscr frontier_audioscr
  --revert-single func_800BEA3C` -> `spliced frontier_audioscr: 3 members, new: ['game_results_render', 'game_results_update']`,
  `image coverage: 1064/1216 functions, 392788/647072 bytes (60.70%)`; src/blob/func_800BEA3C.c removed (body now in audio.c, kept).
- Gates (`gates_job3a.log`, `rom_job3a.log`): `group lock: 0 problems`; `blob lock: 1064 entries, 0 problems`; `blob_unit: 1064
  locked bodies, 1064 equal to the image in one 933-file unit, 0 differ`; `unused_overrides []`; `ROM build with the source-built
  blob: SHA-1 EXACT`, `build/us/rush2049.us.z64: OK`.
- symbols.json / blob.ld unchanged (2 provenance comments + SHA256SUMS): no re-pin. Slot-contract audit: the pair accepted,
  unaccepted_caller_bytes 47668 -> 45332, frozen list/count updated.

### 3b: frontier_optionscr_buttons
- Install (`install_job3b.log`): `spliced frontier_optionscr_buttons: 2 members, new: ['func_800DA2C0', 'func_800DB1E0']`,
  `image coverage: 1066/1216 functions, 396504/647072 bytes (61.28%)`. No revert (neither was locked).
- Gates (`gates_job3b.log`, `rom_job3b.log`): `group lock: 0 problems`; `blob lock: 1066 entries, 0 problems`; `blob_unit: 1066
  locked bodies, 1066 equal to the image in one 934-file unit, 0 differ [... own bss 5 ...]`; `unused_overrides []`;
  `ROM build with the source-built blob: SHA-1 EXACT`, `build/us/rush2049.us.z64: OK`.
- symbols.json / blob.ld unchanged (the two statics D_8015273C/D_80152740 stay as they were; asm diff = 2 provenance comments +
  SHA256SUMS): no receipt re-pin. Slot-contract audit: no change. Not provisional. func_800D816C/func_800D91A0 untouched.
- Lead for the coordinator: provisional `func_800DA0BC` (184 B, EQUAL with a stand-in caller, unlocked) now has its real caller
  func_800DB1E0 locked — a packaging candidate (not touched: provisional.json is edited only to retire entries installed here).
- CLAIMS: both on the wave-32 landed row (devices); menus row (func_8010E8B4 force-oracle finding, videoscr pair draft) and
  results row (audio pair landed, controls pair 253/234 rows + next lever) rewritten from w32b RESULTS.

## Receipt re-pin (Job 3a follow-up, commit 51ebf500)
`--revert-single func_800BEA3C` deleted src/blob/func_800BEA3C.c, which cloud/work/ipa-groups/dot_menu_row_caller_20261005/verify.py
compiled as the "accepted standalone setter" (rocky: `test_dot_menu_row_caller.py::test_toolchain_replays_full_extent_proof` failed).
Fix: `accepted_func_800BEA3C.c` in the receipt = the pre-wave-32 single byte for byte (sha256 ced1420b..., equal to peer_review.json
`accepted_setter_sha256`); verify.py reads it (2 lines + comment); verification.json unchanged; integration.json lists the new
verify.py hash (44c10f2e...) and the copy. TRAP for future `--revert-single`: grep receipts for `src/blob/<fn>.c` first.

## Tests (final head 51ebf500)
- Pi `make test-fast JOBS=4` (main checkout's tools/mips_to_c and tools/decomp-permuter symlinked in temporarily, removed afterwards):
  conveyor `1593 passed, 798 skipped`; cloud `841 passed, 205 skipped`; rc=0.
- rocky clone `~/rush2049/scratch/w32i` (copy of the w32n clone, `git clean -fd`, branch from a bundle, `origin/master` = d1c53ab2,
  submodules populated; env: `. ~/rush2049/scratch/ci.env; PATH=$CI_VENV/bin:$CI_PATH:$PATH` — without it 300+ toolchain/sanitizer
  failures) `REQUIRE_TOOLCHAIN=1 make test-fast JOBS=16`: conveyor `2339 passed, 52 skipped`; cloud `1041 passed, 5 skipped, 8 warnings,
  27 subtests passed`; rc=0 (before the re-pin: 1 failed, the receipt above).
- Builder scratch watchman2:~/rush2049/scratch/frontier/w32i and unit/w32i deleted.
