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
