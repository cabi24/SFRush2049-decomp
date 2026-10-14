# w31i — game-side integration + blob_group jump-table rule (wave 31, worktree, branch `w31i`)

Merged wave31 `882c9f77` into w31i (fast-forward). Worktree build/ copied from the main checkout (build/{blob,ovl,us,m2c_asm},
*.json, game_code.bin/.sym, blob_unit/cache, baserom); `blob_splice rebuild-stale --builder-dir ~/rush2049/scratch/frontier/w31i/bs
--group-builder-dir .../bg` rebuilt 101 objects bound to other source (copied objects were stale) before any install (`rebuild_stale.log`).

| Step | Change | New bytes | Game after (`blob_rom coverage`) |
|---|---|---:|---|
| (start) | — | — | 1050/1216, 378,232 B (58.45%) |
| 1a | blob_group `_independent_regions` jump-table branch + tests (tools change) | — | — |
| 1b | splicing-docs paragraph (tools/conveyor/README.md, section 008) | — | — |
| 1c | NEW group `func_8010AEAC` (w30b packet, unchanged) | 3,452 | **1053/1216, 381,684 B (58.99%)** |

## Job 1a: the rule (tools/conveyor/pipeline/blob_group.py)
`_independent_regions(..., address=None)` (verify_own_data passes the member's image address). Old: any relocation in the
section refused (`relocated words (a jump table) need the one-base or jump-table proof`) — still the message when no address
is given. New branch:
- `_own_jump_table_entries`: every relocation of the section must be an aligned R_MIPS_32 against the function's text
  section (section symbol or a function symbol in it); else `relocation type N is not a jump-table entry ...` /
  `relocated word against X is not a jump-table entry into the function's text`.
- `_relocate_own_jump_table`: each entry inside one of THIS function's windows must target [lo, hi) of the function
  (`jump-table entry targets .text+0x.., outside f (...)`) and is rewritten to `address + (target - lo)`; the window
  is then compared whole against retail as before; a mismatch in a relocated window refuses with `(jump-table window,
  N entries relocated) differs from retail 0x... at +0x.. after relocation (retail .., got ..)` (word-aligned).
- Entries outside this function's windows (another member's table, e.g. func_8010A8D0's in the same .rodata) are not
  compared through this function's windows; that member's own proof covers them (A8D0 passes owndata's one-base rule).
- Trailing-zero trimming of the last window happens after relocation, so an entry with a zero addend is never trimmed away.
- One-base path (owndata) untouched; every other independent-regions rule unchanged.
Tests (tests/conveyor/test_owndata.py; the old `test_rodata_regions_refuse_jump_tables` became the passing case):
`test_rodata_regions_accept_a_jump_table_into_the_function` (also: function-symbol entries; no address -> old refusal),
`..._refuse_a_jump_table_entry_outside_the_function`, `..._refuse_a_jump_table_that_differs_after_relocation`,
`..._refuse_other_relocations` (type 4; against .data). `tests/conveyor/test_owndata.py`: all pass.

## Job 1c: func_8010AEAC group (w30b `groups/func_8010AEAC/`, installed unchanged)
- Unit (`unit_job1.log`): `blob_unit --tag w31i score func_8010AEAC func_8010A8D0 func_8010A7A4 --with faeac.c --internal`
  A7A4, A8D0 and the six *_AEAC helpers `--neighbours` -> `EQUAL func_8010AEAC: 413 words (kept, c_faeac.c)`,
  `EQUAL func_8010A8D0: 375 words (internal, c_faeac.c)`, `EQUAL func_8010A7A4: 75 words (internal, c_faeac.c)`,
  `locked bodies that differ in this unit: 0`, `blob_unit score: 3/3 equal`.
- Builder (watchman2 scratch w31i, IDO 5.3 796ae99a, `builder_job1.log`) `score.py group cand/w31i/func_8010AEAC --claims`
  and without: rc=1 only from AEAC's expected line `NOT VERIFIED (4 section-relative relocations unverified: .rodata+0x0
  at +0xd0, .rodata+0x0 at +0xe8, .rodata+0x5c at +0x208, .rodata+0x5c at +0x210; own .rodata: this function's references
  disagree on the section's image address (0x80121018 for +0x0, 0x801248C0 for +0x5c) ...)`; `func_8010A8D0: MATCH`
  (own .rodata 0x801248D8..0x8012491C), `func_8010A7A4: MATCH` (0x801248D4..0x801248D8).
- unit_overrides force_internal += func_8010A7A4, func_8010A8D0, gfx_lock_AEAC, gfx_unlock_AEAC, font_set_AEAC,
  TB_FlashTextColors_AEAC, TB_SetJust_AEAC, TB_CenterText_AEAC (before install).
- Install (`install_job1.log`): `install_group.py cloud/work/frontier/w30b/groups/func_8010AEAC func_8010AEAC` ->
  `spliced func_8010AEAC: 3 members, new: ['func_8010A7A4', 'func_8010A8D0', 'func_8010AEAC']`,
  `image coverage: 1053/1216 functions, 381684/647072 bytes (58.99%)`.
- Gates (`gates_job1.log`, `rom_job1.log`): `group lock: 0 problems`; `blob lock: 1053 entries, 0 problems`;
  `blob_unit: 1053 locked bodies, 1053 equal to the image in one 927-file unit, 0 differ`; `unused_overrides []`;
  `ROM build with the source-built blob: SHA-1 EXACT`, `build/us/rush2049.us.z64: OK`.
- symbols.json / blob.ld unchanged (asm diff = 3 provenance comments + SHA256SUMS): no receipt re-pin.
  Slot-contract audit `audit.py --write`: func_8010A8D0 and func_8010AEAC accepted, unaccepted_caller_bytes 53504 -> 50352;
  frozen list/count in tests/cloud/test_slot_contract_audit.py updated.
- provisional.json: func_8010A7A4 retired (its real caller A8D0 is now locked). All nine ctx_gr3_b.c copies
  byte-identical to credits_scroll_grp/gr3_b.c. Not a scout target. No data-symbol changes.
- CLAIMS: wave-31 landed row added; menus open row now carries w31b's func_800DA2C0 202/579 + DB1E0 pair, func_8010E8B4 3/85.

## Receipt rebind (commit fb90f65f)
The blob_group.py change moves its sha256 (b8faa781 -> a2d1bce8), pinned as `tools_sha256` by four receipts:
dot_material_selector_20261006, dot_billboard_context_20261005, ipa-groups/dot_random_range_b2e4_20261005,
ipa-groups/dot_masked_random_b23e0_20261006 (only that hash changed; precedent eae0c646). Their four test files: 35 passed on rocky.

## Tests
- Pi `make test-fast JOBS=4` at c4c94e49 (main checkout's tools/mips_to_c and tools/decomp-permuter symlinked in temporarily,
  removed afterwards): conveyor `1593 passed, 799 skipped`; cloud `820 passed, 204 skipped`; rc=0.
- rocky clone `~/rush2049/scratch/w31i` (copy of the w30i clone, branch from a bundle, `origin/master` = 880e729e, submodules
  populated, CI venv) `REQUIRE_TOOLCHAIN=1 make test-fast JOBS=16`: at c4c94e49 2 failures (the two receipt tests above,
  tools_sha256 of blob_group.py) -> rebind; at fb90f65f conveyor `2340 passed, 52 skipped`; cloud `1019 passed, 5 skipped,
  8 warnings, 27 subtests passed`; rc=0.
- Builder scratch watchman2:~/rush2049/scratch/frontier/w31i and unit/w31i deleted.

## Job 2: w31d track_texture_load via frontier_billboard_render (merge of wave31 0237fa4c first)
- Packet check: all 9 old files cmp-identical to src/blob/groups/frontier_billboard_render; ctx_vsync_a145.c cmp-identical
  to codex_vsync_a145/group.c; group.json differs only in members (+track_texture_load), context (+speed_set,
  speed_mode0_wrapper, speed_mode1_wrapper, continue_prompt, vsync_wait — all locked members of codex_vsync_a145), files, claims, provenance.
- Unit (`unit_job2.log`, force_internal added first): `EQUAL track_texture_load: 337 words (internal, c_track_texture_load.c)`,
  `EQUAL billboard_render: 244 words (kept)`, `locked bodies that differ in this unit: 0`, `2/2 equal`.
- Builder (`builder_job2.log`) `score.py group cand/w31i/frontier_billboard_render` with and without `--claims`: rc=0, all
  8 members MATCH (track_texture_load, billboard_render, func_800F207C, func_800F1930, PrintScores, SetHighScoreState,
  FindGhosts, func_800F2718); context informational (speed_set MATCH).
- Install: `spliced frontier_billboard_render: 7 members, new: ['track_texture_load']`.
- FORCE_KEEP TRAP hit: first unit check `1054 locked bodies, 1049 equal ..., 5 differ` — continue_prompt, speed_mode0_wrapper,
  speed_mode1_wrapper, vsync_wait emptied to 2 words (context in the packet makes them internal), plus func_800D7634 as a
  knock-on. force_keep += the four (speed_set stays fine) -> `blob_unit: 1054 locked bodies, 1054 equal to the image in one
  929-file unit, 0 differ`; `group lock: 0 problems`; `blob lock: 1054 entries, 0 problems`; `unused_overrides []`;
  `ROM build with the source-built blob: SHA-1 EXACT`, `build/us/rush2049.us.z64: OK`.
- force_internal += track_texture_load, AudKillSong_TTL, AudSetMusicVol_TTL, AudPlaySong_TTL, SaveFlush_TTL.
- symbols.json / blob.ld unchanged (1 provenance comment + SHA256SUMS): no re-pin. Slot-contract audit: no change.
  ctx_gr3_b.c copies all identical. Not provisional, not a scout target. No data-symbol changes.
- Coverage: 1053 -> **1054/1216, 381,684 -> 383,032 B (58.99% -> 59.19%)**.
- CLAIMS: track_texture_load (+ wrong-name note: = GC _InitScreen@highscorescr.c, rename next batch, + devices) on the
  wave-31 landed row; w31d's control_settings/func_800DD0C0/codex_control_settings_a4 packet state, LoadTrack,
  func_800E543C (track pool row) and SetupWaveModel/func_80097164 (resource loader row) with next levers + the
  unsaved-callee-saved draft_check lesson.

## Job 3: w31c SaveServer + func_800CC040 via menu_save_options (merge of wave31 f4a9ddd8 first)
- Packet check: ctx_hash.c, save.c cmp-identical to the installed group; ctx_strncmp.c cmp-identical to src/blob/func_800950AC.c;
  ctx_newghost.c differs by the one prototype line (`void SaveServer(s32 forceUpdate, s32 rumbleOnly);`); group.json: members
  (first 20 identical, in order) + SaveServer, func_800CC040; context - those two + func_800950AC; keep + func_800950AC;
  files + ctx_strncmp.c; same name frontier_menu_save_options.
- force_internal += ghost_heap_lock, ghost_heap_unlock, cart_free_direct, DeleteGhost, GhostDirName (before install).
- Builder (`builder_job3.log`) `score.py group cand/w31i/menu_save_options --claims`: rc=0, all 22 members MATCH;
  SaveServer `own .data verified at 0x8011742C`, `0x80117430`, `0x80117434 (4 bytes, all zero ...)`.
- Install: `spliced menu_save_options: 22 members, new: ['SaveServer', 'func_800CC040']`.
- Gates (`gates_job3.log`, `rom_job3.log`): `group lock: 0 problems`; `blob lock: 1056 entries, 0 problems`;
  `blob_unit: 1056 locked bodies, 1056 equal to the image in one 930-file unit, 0 differ`; `unused_overrides []`;
  func_800950AC: kept, duplicates chooses `s_func_800950AC.c` (locked single), so no force_keep;
  `ROM build with the source-built blob: SHA-1 EXACT`, `build/us/rush2049.us.z64: OK`.
- symbols.json / blob.ld unchanged (2 provenance comments + SHA256SUMS): no re-pin. Slot-contract audit re-written:
  only SaveServer leaves one caller's other_unaccepted list (counts unchanged; test passes). Not provisional.
- Coverage: 1054 -> **1056/1216, 383,032 -> 384,828 B (59.19% -> 59.47%)**.
- CLAIMS: SaveServer, func_800CC040 (+ devices/hypotheses) on the wave-31 landed row; save row rewritten with
  func_800CC50C 9/190 (= CompressGhost), pak_delete_file_retry (colour only, `--callee` a1,a2/s0,s1), CreatePlayer 241/272.
