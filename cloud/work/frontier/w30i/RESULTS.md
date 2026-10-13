# w30i — game-side integration (wave 30, worktree, branch `w30i`)

Merged wave30 `c5512ac1` into w30i (Job 1). Worktree build/ copied from the main checkout (build/{blob,ovl,us,…},
blob_unit cache, baserom); `blob_splice rebuild-stale` rebuilt 5 stale copied group objects before any install.

| Step | Install | New bytes | Game after (`make progress` / splice coverage) |
|---|---|---:|---|
| (start) | — | — | 1041/1216, 370,952 B (57.33%) |
| 1 | `menu_save_options` (frontier_menu_save_options) superseded by w30c's packet + merge of menu_cc040_20261004 and codex_func_800C7200_2 | 2,848 | 1045/1216, 373,800 B (57.77%) |

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
