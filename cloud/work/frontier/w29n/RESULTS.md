# w29n: game symbol renames + GameCube reference tooling (RESULTS)

Lane w29n, wave 29, worktree branch `w29n` from `wave29` = 8c74d8b6 (wave29 merged in, last at 09496631).
Coverage unchanged (renames only): game 1024/1216, 352,068 B; image sha256 bf7da3fa…, ROM SHA-1 EXACT.

## Job 1: renames

### Applied (17 functions, 5 data symbols)

Table: [`renames.json`](renames.json) (static_rename format + `kind`, `source`, `evidence`; accepted rows only).
Reader's table with GC sources: [`docs/external/gc/names.md`](../../../../docs/external/gc/names.md).

| Address | Old | New | GC source / reason |
|---|---|---|---|
| 0x800DC88C | attract_mode_handler | control_cell_text | wrong label; one-line control-settings menu cell (w28d) |
| 0x800DCDF4 | attract_video_handler | control_cell_text2 | wrong label; cell text + two option strings |
| 0x800DC99C | attract_demo_handler | control_cell_text3 | wrong label; cell text + three option strings |
| 0x800A2680 | AdjustSpeed | pak_delete_file_retry | wrong label; osPfsDeleteFile with retry (lead: GC PS_DeleteFile/_DeleteFile) |
| 0x800EDDC0 | attract_or_transition | gfx_frame_begin | wrong label; per-frame display-list prologue (locked single, file moved) |
| 0x800B61FC | audio_doppler_full | ShowCountdown | countdown.c (locked group `audio_doppler_full`) |
| 0x800F0914 | func_800F0914 | PrintScores | highscorescr.c _PrintScores (locked) |
| 0x800F1210 | func_800F1210 | SetHighScoreState | highscorescr.c _SetHighScoreState (locked) |
| 0x80108154 | func_80108154 | animLapTime | hud.c (locked) |
| 0x800CD104 | func_800CD104 | CreatePlayer | save.c |
| 0x800C813C | func_800C813C | SaveServer | save.c |
| 0x80096CA8 | func_80096CA8 | SetupWaveModel | waves.c _SetupWaveModel |
| 0x800B574C | sound_loop_set | TitlePane | _TitlePane |
| 0x800A2D4C | track_process_main | PS_CreateFile | paksysdc.c |
| 0x800A2504 | slot_state_lookup | PS_WriteFile | paksysdc.c (locked, pak_write_cluster) |
| 0x800A2378 | MaxPathZeroControls | PS_FlushFile | paksysdc.c (locked) |
| 0x800A21A4 | track_collision_setup | PS_FlushPak | paksysdc.c (locked) |
| 0x80140B20 | D_80140B20 | gHighScoreState | highscorescr.c |
| 0x80149418 | D_80149418 | namePanes | highscorescr.c |
| 0x80146208 | D_80146208 | codePane | highscorescr.c |
| 0x8002E870 | D_8002E870 | gDefStartTimes | highscorescr.c/waveinfo.c |
| 0x80156D38 | D_80156D38 | gWaveInstance | waves.c |

Deferred (status `deferred` with the blocking fixture in renames.json; names.md lists them): LoadTrack
(func_800BB9B0; test_node_pool_init_match records it as caller), animControls (game_results_input;
test_d05_hud_context_audit parametrises on it), gTotalNames (dot_name_cache), gRomObjDefPtrs
(dot_material_selector), gTexDefPtrs (func_800B24EC.c pinned by availability_root, multiblit, blit_bar),
gPalDefPtrs (test_disasm fixture), gCamera (dot_steady_camera looks it up), gNumWindows, gCountDown,
gNumModels (pinned sources / a declaration-text check). Each goes with its fixture in a later rename.
Not applied (weaker or contested; names.md "not applied yet"): FindGhosts, UninitScreen, animText/
animButtonText/printOption, animWrongWay, animResults, DrawUnlocked, InitPlayer, PS_PakServer, PS_ReadFile,
the ShowCountdown callees, HAL_SetFont (w28a/w28c disagree), rand_r, the static vi* pair, ~40 data names.
Names chosen: GC spelling without the CodeWarrior underscore; neutral snake_case for N64-only code.

### How (all tools in `tools/`, rerunnable: `sh tools/apply.sh DATA_DIR`)

Game function names are Conveyor inventory target ids (`n64_target`, population `extracted`) → blob layout
→ asm/us/blob; `targets_rename` handles static rows only and `GAME_SYMBOLS` only names data, so:
1. `db_rename.py` renames the 17 rows (5 keyed tables, temp ids, refusals as targets_rename) — **run on a copy
   of the DB only** (live DB untouched: the wave's lanes still use it); `blob_layout --data COPY derive`
   differs from the old layout by exactly the 17 names.
2. `GAME_SYMBOLS` (disasm.py) gets the 5 data names (+ comment listing the deferred ones).
3. `symaddr_rename.py`: 10 game definitions in symbol_addrs.us.txt renamed (others are not defined there);
   `static_rename owned rebind/empty_renames.json --base HEAD --rebind-dir w29n/rebind` re-pins
   sdk_initialize's context (rom_owned_data.json) and src/rom/storage_overrides.mk follows (generated).
4. `users.py` + `static_rename source`: the 47 src/blob files that USE a renamed name (outside the generated
   prelude declarations) rewritten by permute_c; the other ~860 files keep stale prelude prototypes of old
   names (no code/link effect; rewriting them would re-pin ~1000 lock entries). `json_rename.py`: group.json /
   unit_overrides.json name values; `git mv src/blob/attract_or_transition.c src/blob/gfx_frame_begin.c`.
   Group dir/file names kept (ids, e.g. `groups/audio_doppler_full/`).
5. `rename_resplice.py apply` (w23g's tool + lock re-keying + JSON proof + stale-object rebuild): every changed
   lock entry's base text holds its pin and the live text is exactly the base renamed; 8 keys renamed, 98
   entries re-pinned (36 sources), recompiled on watchman2, image gate; proof in `blob_lock_migration.json`.
6. `blob_tu symbols`; receipts: nearest_point_donor/target_orientation symbols.json pins, slot-contract audit
   `--write` + test (accepted callers PrintScores/SetHighScoreState/animLapTime; counts unchanged 68260/6/4520).
7. Receipts the rename moves (proved, then written): `rebind.py` (census digests of dot_static_10450,
   dot_static_12234, dot_runtime_b_hud_teardown, dot_runtime_b_radial_impulse recomputed with the rename undone
   = recorded; source pins of dot_frame_dispatch and dot_runtime_a_resource_cache; frame_dispatch's
   audio_doppler_full binding; test_ipakit_heads reads renames.json) and `rebind_packets.py` (w23g rules:
   token rename + pin re-hash in encoded_string_mapping, dot_runtime_a_writer_bounds, dot_audio_bus_reopen,
   dot_billboard_context and test_billboard_save_input_packet). These packets/tests are outside the
   remit list in the lane prompt; any game-function rename breaks them, so they are one separate commit.

### Gates (worktree, after the last apply)
- `image matches (647072 bytes) bf7da3fa6283428a97372250cd4076d15e9eae10f9d5709c0387fe0742d43a1d`
- `blob lock: 1024 entries, 0 problems`; `group lock: 0 problems`
- `blob_unit: 1024 locked bodies, 1024 equal to the image in one 909-file unit, 0 differ`
- `blob_rom rom`: `ROM build with the source-built blob: SHA-1 EXACT`, `build/us/rush2049.us.z64: OK`
- pre-commit `all 635 locked functions intact` (static locks).

### Integration (coordinator)
1. Merge `w29n`. Before any gate on the main checkout, rename the LIVE inventory (backup first):
   `python3 cloud/work/frontier/w29n/tools/db_rename.py cloud/work/frontier/w29n/renames.json --apply --backup ~/.conveyor/conveyor.pre-w29n.db`
   then `python3 -m tools.conveyor.pipeline.blob_layout derive`; `blob_tu symbols` must leave
   asm/us/blob/symbols.json unchanged. Until then main's layout and lock disagree (blob_splice check fails).
2. After merging w29r: `python3 cloud/work/frontier/w29n/tools/followups.py --write` (ipa_callees.json key
   func_80096CA8 → SetupWaveModel + its phase-8 test line, census names; proved on this branch:
   tests/recomp + phase 8/9 tests pass with it) — or regenerate the census.
3. In-flight drafts and lane packets (w29a's frontier_billboard_render/results_table groups, w29b/c/d drafts)
   use old names: `python3 -m tools.conveyor.pipeline.static_rename source cloud/work/frontier/w29n/renames.json FILE…`
   before scoring/installing; `--with` unit runs otherwise fail to link (`D_80140B20` etc. are gone).
4. Not changed: asm/us/ovl_{a,b}/symbols.json and overlay sources (Astra's; frozen, still old spellings — a
   future `ovl_targets generate` would need the table), boot_tail symbols.json, cloud/matches, target objects
   in the DB (old symbol inside; `targets --relocate-extracted` before a farm search on a renamed row),
   build/m2c_datasyms/protos (regenerate in the main checkout), include/game_types.h (pinned by the
   integration_B24 static context; renaming it needs that re-pin).
5. `identity.json` maps static recompiler names only: no change.

## Replay (coordinator request: renames go on last)

The branch is now three commits on wave29 09496631, so it can be rebuilt on any later wave29:
1. `gcref: struct-offset fingerprint channel` (Job 2; independent of the rename),
2. `w29n: rename inputs and replay` (renames.json, tools/, replay.sh, names.md, README; inputs only),
3. `Apply the w29n game symbol rename (replay.sh output …)` — produced by
   `sh cloud/work/frontier/w29n/replay.sh` on commit 2, nothing by hand (plus this RESULTS commit).

`replay.sh [MAIN_CHECKOUT]` (header documents the 9 steps) needs a clean tree; it copies build/ from the main
checkout when absent, renames a sqlite backup COPY of the live DB (never the live DB), discovers every
src/blob source that uses an old name (so groups installed after the base are covered), and ends with the
gate chain. Proof: a fresh scratch worktree at commit 2 with build/ copied from the main checkout, replay
run, `git add -A; git diff --cached <commit 3>` = **0 lines**; its gates: `image matches (647072 bytes)
bf7da3fa…`, `blob lock: 1024 entries, 0 problems`, `group lock: 0 problems`, `blob_unit: 1024 locked bodies,
1024 equal … 0 differ`, `ROM build with the source-built blob: SHA-1 EXACT` (2.5 min). Determinism fixes:
storage_overrides.mk regenerated explicitly (`owned_storage generate`), correlation run with
PYTHONHASHSEED=0 (equal-score ties followed set order).
To re-apply on a merged wave29 W: `git checkout -B w29n W; git cherry-pick <commit 1> <commit 2> <RESULTS>;
sh cloud/work/frontier/w29n/replay.sh; git add -A; git commit`. Packets that the replay does not know
(new receipts pinning renamed names) show up only in the test suites, not in the gates: run both test-fasts.
Later-lane names: `renames.json` `later_lane_names` and status-`candidate` rows (FindGhosts, drawMain,
model_iteration, animElapsedTimerText, animPlace, the audio-options wrong-label lead), names.md last section.
Tests at commit 3: Pi `make test-fast JOBS=4` conveyor `1589 passed, 803 skipped`, cloud `808 passed, 202
skipped`, rc=0; rocky `REQUIRE_TOOLCHAIN=1 make test-fast JOBS=16` conveyor `2340 passed, 52 skipped`, cloud
`1005 passed, 5 skipped, 27 subtests passed`, rc=0. followups.py is now part of the replay (ipa_callees key
SetupWaveModel, census names, phase-8 test).

## Job 2: struct-offset channel (`tools/gcref/fields.py`, `correlate.py --fld-*`)
Keys `offset/width` of loads/stores off pointer registers (GC D-form words from rush_d.elf with relocated
words, r0/r1/r2/r13 and negative displacements skipped; N64 MIPS with `%lo` bases, sp/gp/zero skipped),
offsets >= 8, IDF-weighted cosine, >= 4 shared keys; added as `fld` (weight 1.0 x similarity, top 3,
similarity >= 0.5), re-ranking only (confident pairs decided without it; `--fld-confident` opt-in).
Held-out (22 string-anchored pairs, strings masked): **top-1 8 → 12/22, top-3 11 → 15/22**, confident
4/4 (recall 4/22) unchanged, 72 confident pairs (same set), functions with a candidate 746 → 841. Tried:
df caps (fld-alone top-1 1–5/22), min offset 0/16, min shared 2, weights 2–3, `--fld-confident` (weight 1:
129 confident pairs, weight 2: 171, 4/4 and 6/6 on the small truth set but implausible propagated pairs).
Outputs regenerated with the new names (`correlation.md/json`), README documents channel + numbers.
Tests: test_gcref.py 8 passed (2 new).

## Job 3: rock.elf / offroad_d.elf (not fetched)
rock.elf (PowerPC, 8,881 symbols, 1.4 MB debug) is very likely a CodeWarrior GameCube build of Rush the Rock,
i.e. a console port of the arcade source we already have: little new logic, but DWARF struct layouts with
offsets, params/locals for arcade-ancestor functions (declaration order, field checks), console-side names
bridging arcade ↔ 2049-GC names, and a second fld corpus; probably none of the N64-only screens/pak code.
Needed: the ISO + Astra's gcm.py/extract_syms.py (not in the repo) to extract it; `dwarf1.py --elf` should
work unchanged if it is CodeWarrior DWARF v1; disasm.py needs SDA bases read from `_SDA_BASE_` symbols and to
tolerate a missing symbols CSV; correlate needs per-reference `--gc-features/--elf` (exists). ~2–4 h after
the ISO is available. offroad_d.elf is another game sharing only Midway's platform/library layers: low value.

## Tests
- Pi worktree (main checkout's tools/mips_to_c and decomp-permuter symlinked in temporarily, removed after),
  `make test-fast JOBS=4` at 724e197e and again at dc66f2e7 (wave29 = 09496631 merged): conveyor `1589 passed,
  803 skipped`; cloud `808 passed, 202 skipped`; rc=0.
- rocky clone `~/rush2049/scratch/w29n` (from w28i's clone, branch from a bundle, origin/master = 8c74d8b6),
  `REQUIRE_TOOLCHAIN=1 make test-fast JOBS=16` at 724e197e: conveyor `2340 passed, 52 skipped`; cloud
  `1005 passed, 5 skipped, 8 warnings, 27 subtests passed`; rc=0. (0939c4b1 adds only the regenerated
  storage_overrides.mk, present in the Pi run's tree; the later merges add only cloud/work/frontier/w29a,w29c lane files.)
- Builder scratch `watchman2:~/rush2049/scratch/frontier/w29n` and `unit/w29n` deleted.

## Permission denials
None.
