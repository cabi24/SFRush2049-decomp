# w32i — game-side integration (wave 32, worktree, branch `w32i`)

Merged wave32 `eb3ccd01` into w32i (fast-forward from d1c53ab2). Worktree build/ copied from the main checkout
(build/{blob,ovl,us,m2c_asm}, build/*.json, game_code.bin/.sym, blob_unit/cache, baserom); `blob_splice rebuild-stale
--builder-dir ~/rush2049/scratch/frontier/w32i/bs --group-builder-dir .../bg` rebuilt 101 stale objects first (`rebuild_stale.log`).
Builder scratch watchman2:~/rush2049/scratch/frontier/w32i (copy of base + rsync; `IDO_DIR` = toolkit 796ae99a ido).

| Step | Change | New bytes | Game after (`blob_rom coverage`) |
|---|---|---:|---|
| (start) | — | — | 1056/1216, 384,828 B (59.47%) |
| 1 | `menu_save_options` superseded in place (w32c): + func_800CC50C (CompressGhost), + props_render (GhostStart) | 2,940 | **1058/1216, 387,768 B (59.93%)** |

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
