# w30i — game-side integration (wave 30, worktree, branch `w30i`)

Merged wave30 `c5512ac1` into w30i (Job 1). Worktree build/ copied from the main checkout (build/{blob,ovl,us,…},
blob_unit cache, baserom); `blob_splice rebuild-stale` rebuilt 5 stale copied group objects before any install.

| Step | Install | New bytes | Game after (`make progress` / splice coverage) |
|---|---|---:|---|
| (start) | — | — | 1041/1216, 370,952 B (57.33%) |
| 1 | `menu_save_options` (frontier_menu_save_options) superseded by w30c's packet + merge of menu_cc040_20261004 and codex_func_800C7200_2 | 2,848 | 1045/1216, 373,800 B (57.77%) |
| 2 | `credits_scroll_grp` superseded in place (w30d): + animPlace (gr3_7af4.c) | 1,000 | 1046/1216, 374,800 B (57.92%) |
| 3 | `frontier_results_table` superseded in place (w30a): + func_80102980 | 1,456 | 1047/1216, 376,256 B (58.15%) |
| 4.1 | single func_8010DCFC (w30b) | 660 | 1048/1216, 376,916 B |
| 4.2 | NEW group func_8010B5D0 (w30b) | 556 | 1049/1216, 377,472 B (58.34%) |
| 4.3 | NEW group func_8010AEAC (w30b) — REFUSED by blob_group (own .rodata), not installed | 0 | — |
| 4.4 | credits_scroll_grp superseded as the UNION of w30d (animPlace) + w30b (func_80108AB0) | 760 | **1050/1216, 378,232 B (58.45%)** |

## Job 1: w30c save packet
New coverage: draw_speedometer 1,704, func_800C7EC8 628, draw_ui_element 240, func_800CBF2C 276 = 2,848 B. The three
stubs (func_800C7EC0, func_800C7300, func_800CD0FC) were already locked singles (8 B each), so w30c's 2,872 B figure
counts 24 B that were already coverage; they and the three pak getters move from singles into the group
(`--revert-single` x6, same bytes).

**One definition of func_800CBF2C / SaveServer.** First tried the clean route — superseding packets of
`menu_cc040_20261004` (slot_release) without its func_800CBF2C draft and of `codex_func_800C7200_2` without its
SaveServer draft (`groups/v/`, all members kept). Builder `score.py group --claims`: menu_item_value_get 17/17 differ
(slot_release program without the caller), func_800C7200 64/64 (umerge inlines it into its sole remaining caller).
Not installable. Instead the four members of those two groups (AdjustSteer, menu_transition, menu_item_value_get —
kept; func_800C7200 — internal) became members of menu_save_options, where w30c's ctx_slot.c / ctx_newghost.c already
compile them (MATCH as context), and both old groups were reverted by `install_group.py --revert-group`. Keep/internal
status of every function unchanged; group.c/save.c/ctx files are w30c's byte-for-byte; only group.json changed
(4 context -> members/claims, note). Packet: `groups/frontier_menu_save_options/`.

- Unit (Pi, `blob_unit --tag w30i score` of 22 names with w30c's four files `--with`, the 7 new internals + `_ReadGhostHeader`
  `--internal`, `--neighbours`): `blob_unit score: 22/22 equal`, `locked bodies that differ in this unit: 0` (`unit_packet.log`).
- Builder (watchman2 scratch w30i, IDO 5.3 796ae99a): `score.py group cand/w30i/frontier_menu_save_options --claims`
  rc=0, all 20 members MATCH (`claims_score.log`); context unmatched as in w30c: SaveServer 176/225, CreatePlayer
  246/272, func_800CC040 29/224, menu_back 3/33, func_800CC50C 117/190.
- Install: `install_group.py cloud/work/frontier/w30i/groups/frontier_menu_save_options menu_save_options --revert-group
  menu_cc040_20261004 --revert-group codex_func_800C7200_2 --revert-single func_800A361C/func_800A1A3C/func_80094FC4/
  func_800C7EC0/func_800C7300/func_800CD0FC` -> `spliced menu_save_options: 20 members, new: ['draw_speedometer',
  'draw_ui_element', 'func_800C7EC8', 'func_800CBF2C']`.
- Gates: `group lock: 0 problems`; `blob lock: 1045 entries, 0 problems`; `blob_unit: 1045 locked bodies, 1045 equal to
  the image in one 920-file unit, 0 differ`; `ROM build with the source-built blob: SHA-1 EXACT`,
  `build/us/rush2049.us.z64: OK`.
- Unit manifest: SaveServer, func_800CBF2C, func_800C7200, menu_item_value_get not in `duplicates`; `unused_overrides: []`.
  No force_keep needed (no locked single is context: draw_number, format_string_parse, audio_* are group members;
  func_800CC040/func_800CC50C/menu_back are unlocked).
- symbols.json / blob.ld unchanged (only SHA256SUMS + provenance comments): no donor-receipt re-pin. Slot-contract
  audit `audit.py --write`: no diff; test passes. Scout receipt: none of the functions is a scout target.
- provisional.json: no entries for these functions. No data-symbol changes.

## Job 2: w30d animPlace (merge of wave30 5cfc8433/e4e580f3 first)
- Packet check: all 10 old files byte-identical to src/blob/groups/credits_scroll_grp (cmp); group.json differs only in
  members/keep/files += animPlace/gr3_7af4.c, claims, provenance.
- Unit (`blob_unit --tag w30i score animPlace animElapsedTimerText animLapTime slot_state_setup --with gr3_7af4.c
  --internal` the four *_7AF4 helpers `--neighbours`): `blob_unit score: 4/4 equal`, `locked bodies that differ in this unit: 0`
  (animPlace 250 words kept).
- Builder `score.py group cand/w30d/credits_scroll_grp --claims`: rc=0, animPlace MATCH; without --claims rc=0, all 9
  members MATCH (`builder_job2.log`).
- Install: `install_group.py cloud/work/frontier/w30d/groups/credits_scroll_grp credits_scroll_grp` -> `spliced
  credits_scroll_grp: 9 members, new: ['animPlace']`.
- Gates: `group lock: 0 problems`; `blob lock: 1046 entries, 0 problems`; `blob_unit: 1046 locked bodies, 1046 equal to
  the image in one 921-file unit, 0 differ` (internal 271 -> 275: the four *_7AF4 helpers, no override needed);
  `ROM build with the source-built blob: SHA-1 EXACT`, `build/us/rush2049.us.z64: OK`.
- All seven ctx_gr3_b.c copies (func_8010BC84, physics_sym, frontier_results_table, func_8010B7FC, func_8010B9C8,
  func_80106B3C, frontier_billboard_render) byte-identical to credits_scroll_grp/gr3_b.c.
- Slot-contract audit `audit.py --write`: credits_scroll_grp/group.json re-pinned, animPlace accepted,
  unaccepted_caller_bytes 56236 -> 55236; frozen list/count in tests/cloud/test_slot_contract_audit.py updated.
  symbols.json/blob.ld unchanged. Not a scout target. No provisional/data/unit_overrides changes.

## Job 3: w30a func_80102980
- Packet check: ctx_gr3_b.c byte-identical to the installed copy; results_table.c = installed file + the 2980 body + header
  comment; group.json differs only in members (+func_80102980), claims, provenance.
- Unit (w30a's command, tag w30i): `blob_unit score: 8/8 equal`, `locked bodies that differ in this unit: 0`
  (func_80102980 364 words internal, F30 890 kept).
- Builder `score.py group cand/w30a/frontier_results_table`: with and without `--claims` rc=0, all 7 MATCH (F30 own .rodata
  0x80124864..0x80124880, drawMain 0x80124848..0x80124864).
- Install: `install_group.py cloud/work/frontier/w30a/groups/frontier_results_table frontier_results_table` -> `spliced
  frontier_results_table: 7 members, new: ['func_80102980']`.
- Gates: `group lock: 0 problems`; `blob lock: 1047 entries, 0 problems`; `blob_unit: 1047 locked bodies, 1047 equal to
  the image in one 921-file unit, 0 differ`; `ROM build with the source-built blob: SHA-1 EXACT`, `build/us/rush2049.us.z64: OK`.
- func_80102980 is internal in the unit manifest by the group rule (not on keep): no force_internal; `unused_overrides: []`.
  All seven ctx_gr3_b.c copies still byte-identical to credits_scroll_grp/gr3_b.c.
- Slot-contract audit re-written: func_80102980 accepted, unaccepted_caller_bytes 55236 -> 53780; test list/count updated.
  symbols.json/blob.ld unchanged; not a scout target; func_800BEA3C untouched; no provisional/data/unit_overrides changes.

## Job 4: w30b (merge of wave30 931baefa first)
Unit confirmations on the current tree (tag w30i, each `--neighbours`, `locked bodies that differ in this unit: 0`):
`EQUAL func_8010DCFC: 165 words (kept)`; `EQUAL func_8010B5D0: 139 words (kept)` (5 *_B5D0 `--internal`);
`EQUAL func_8010AEAC: 413 words (kept)`, `EQUAL func_8010A8D0: 375 (internal)`, `EQUAL func_8010A7A4: 75 (internal)`;
union credits packet: `blob_unit score: 6/6 equal` (func_80108AB0 190 kept + own zero-initialised data consistent,
animPlace, animElapsedTimerText, animLapTime, slot_state_setup, credits_scroll).
Builder (scratch w30i, recreated and deleted): `score.py fn` func_8010DCFC MATCH; func_8010B5D0 group MATCH rc=0 with and
without --claims; func_8010AEAC group rc=1: AEAC `NOT VERIFIED (4 section-relative relocations unverified ... 0x80121018
for +0x0, 0x801248C0 for +0x5c)`, A8D0 MATCH (own .rodata 0x801248D8..0x8012491C), A7A4 MATCH (0x801248D4..0x801248D8);
union credits_scroll_grp rc=0 all 10 MATCH, `--claims` func_80108AB0 MATCH (`own .bss placed at 0x8013F2F8`, own .rodata
0x801248C4..0x801248CC). (`builder_job4.log`)

- 4.1 `splice_singles.py func_8010DCFC` -> `image_ok True locked 1048`. No overrides.
- 4.2 `install_group.py cloud/work/frontier/w30b/groups/func_8010B5D0 func_8010B5D0`; force_internal += gfx_lock_B5D0,
  gfx_unlock_B5D0, font_set_B5D0, item_refresh_B5D0, TB_CenterText_B5D0 (as instructed; they are internal by the group
  rule anyway, `unused_overrides: []` either way).
- 4.3 **NOT INSTALLED.** `install_group.py ... func_8010AEAC` -> `FAILED: func_8010AEAC: own .rodata: this function's
  references disagree on the section's image address (0x80121018 for +0x0, 0x801248C0 for +0x5c) ...; independent regions
  refused: .rodata: relocated words (a jump table) need the one-base or jump-table proof [per-reference placement tried
  because the whole-section placement was refused: .rodata: unmapped data between jump table entries]`; lock, sources and
  image restored (`image after restore: OK`), the force_internal entries for it reverted. Cause: the TU's .rodata mixes
  the string/literal objects with AEAC's jump table, so neither the one-base proof, the independent-regions proof (which
  refuses relocated words) nor the jump-table-windows proof (which requires the section to be table entries only)
  applies. Also retail interleaves func_80108AB0's literals (0x801248C4..CC) between AEAC's 0x801248C0 and A7A4's
  0x801248D4, a hint that this unit shares a TU with the hud.c siblings in credits_scroll_grp. Coordinator decision:
  package the three into credits_scroll_grp next to gr3_8ab0.c (and re-score), or a blob_group change (tools/, not mine).
- 4.4 Union packet `groups/credits_scroll_grp/` (w30i): installed group (8 old members + animPlace) + w30b's
  gr3_8ab0.c; members/files/keep += func_80108AB0/gr3_8ab0.c, context unchanged; all 12 .c files byte-identical to their
  sources (old files cmp-checked against w30b's copies too). force_internal += gfx_lock_8AB0, gfx_unlock_8AB0,
  font_select_8AB0, font_set_8AB0, TB_SetJust_8AB0, TB_GetFlashTextCycle_8AB0, HAL_SetFont_8AB0 (`unused_overrides: []`).
  `spliced credits_scroll_grp: 10 members, new: ['func_80108AB0']`. D_8013F2F8: only extern DECLARATIONS exist (6 locked
  files, include/game_globals.h), no definition to drop; symbols.json/blob.ld unchanged by the splice (no receipt re-pin).
  All eight ctx_gr3_b.c copies byte-identical to credits_scroll_grp/gr3_b.c.
- Gates after 4.1/4.2/4.4: `group lock: 0 problems`; `blob lock: N entries, 0 problems`; `blob_unit: N locked bodies, N
  equal to the image ..., 0 differ` (N = 1048, 1049, 1050); `ROM build with the source-built blob: SHA-1 EXACT`,
  `build/us/rush2049.us.z64: OK`. After the refused 4.3: lock 1049 entries, unit 1049/0 differ, SHA-1 EXACT.
- Slot-contract audit re-written at 4.2 (B5D0 accepted, 53780 -> 53224) and 4.4 (credits_scroll_grp/group.json re-pinned,
  80108AB0 accepted, 53224 -> 52464); frozen list/count in the test updated. 4.1: no audit change. No scout targets.

## Tests (branch w30i at 810bed26)
- Pi `make test-fast JOBS=4` (main checkout's tools/mips_to_c and tools/decomp-permuter symlinked in temporarily, removed
  afterwards): conveyor `1589 passed, 800 skipped`; cloud `808 passed, 202 skipped`; rc=0.
- rocky clone `~/rush2049/scratch/w30i` (copy of the w29i clone, branch from bundles, `origin/master` = 1b34f797, CI venv),
  `REQUIRE_TOOLCHAIN=1 make test-fast JOBS=16`: conveyor `2337 passed, 52 skipped`; cloud `1005 passed, 5 skipped, 8
  warnings, 27 subtests passed`; rc=0. (An earlier run at 6b1c5725 under load avg ~45 had 2 failures, both
  `subprocess.TimeoutExpired` after 180 s in the static boot-tail verifiers audio_record_contracts / macro_wrapper_contracts
  — host load, not these changes; they pass in the final run.)
- Builder scratch watchman2:~/rush2049/scratch/frontier/w30i deleted.
