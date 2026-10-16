STATUS (2026-10-15): DONE (jobs 1-3; job 4: no package). Final wave tree wave33 1fe5fe8c0 merged into w33r. Recomp Phase 13, branch w33r (worktree, from origin/master 58425072b). Not a matching lane:
no src/, asm/, lock or coverage change. Docs: tools/recomp/README.md "Phase 13"; plan section 22.

# 1. dc.args for the five --all-lanes ERROR unit dirs

| dir | dc.args | verdict (rocky, `--remote rocky`, service tree as w32r left it) |
|---|---|---|
| w31a/hs | `--unit highscore_animText,DrawName,DrawGhost,DrawUnlocked --scripts ghost_pak,unlock,player_finish,race_finish` | `VERDICT: EQUIVALENT over 2473 calls / 4 scripts (ghost_pak 1144, unlock 308, player_finish 863, race_finish 158)` |
| w32a/hs | same | `VERDICT: EQUIVALENT over 2473 calls / 4 scripts (ghost_pak 1144, unlock 308, player_finish 863, race_finish 158)` |
| w31a/f5ea8 | `--name animResults --params arg0=a0` (byte-identical to w31a/func_80105EA8, same args) | `VERDICT: EQUIVALENT over 26166 calls / 6 scripts (stunt_end 9336, pause_menus 3736, two_player_finish 3390, ghost_save 3268, race_finish 3218, unlock 3218)` |
| w31a/audio | `--unit audio_animText,audio_animButtonText --scripts audio_options,options_menus,pause_menus` | MATCHED (both members in blob_matched.lock.json; not run) |
| w32b/audio | same | MATCHED |

`lane_drafts` parses all five (NAME = the unit list / animResults). In the units DrawName, DrawGhost and DrawUnlocked
are reached only through highscore_animText (draft_check NOTE "never called from outside the unit").
The hs and f5ea8 rows were run one by one (`draft_check --remote rocky ...`, inbox copy only); the `--all-lanes`
table with them comes with job 3.

# 2. Spill-aware save clause in the conveyor (`frontier.unsaved_callee_writes`)

The conveyor scanner already had w32r's first clause (a stack store after the register's first write is a spill;
landed by w32n, eac877ec5). Added the second (regmap_verify Interproc, w32r job 3): in a body that makes a call
(`jal`/`jalr`), a store of s0-s8 to the stack is a save only if the body reloads that register from that slot
(`sw`->`lw`, `sd`->`ld`); a call-free body keeps the old answer. Test:
`tests/conveyor/test_frontier.py::test_a_save_in_a_calling_body_needs_its_reload`.

Facts regenerated in the worktree from copies of the Pi main checkout's build/blob_layout.json, build/game_code.bin
(sha256 bf7da3fa...a1d) and build/ipa_members.json (no DB: `frontier scan` does not read it), old code vs new code
on identical inputs:
- `unsaved_callee_regs` over all 1,216 game functions: exactly Interproc's eight grow (= w32r's list):
  UNMATCHED audio_doppler_calc +s0, entity_tick_main +s5, render_replay_ui +s4; locked save_validate +s3,
  func_800B0A88 +s1,s2, best_times_display +s2, control_cell_text2 +s8, func_800E398C +s7.
- build/frontier.json (unmatched only) changes in exactly three facts:
  audio_doppler_calc 0x800B6788 s1-s8 -> s0-s8; entity_tick_main 0x800930A4 s0-s4,s6,s7 -> s0-s7;
  render_replay_ui 0x800D7D40 s0-s3 -> s0-s4. All three were already recipe `unit`; no recipe, layer or top-level
  field changes. `frontier calibrate`: unsaved detector 0/692 standalone locks (unchanged), 79/376 group members.
- The main checkout's live build/frontier.json (Oct 14 15:06) predates w33i: matched 1066 vs 1068 now
  (audio_mixer_main/audio_priority_find), so its rescan also drops those two.

Coordinator, after merging w33r into the main checkout:
```bash
python3 -m tools.conveyor.pipeline.frontier scan          # writes build/frontier.json; no DB read
# (ipa_members.json is unaffected by this change; refresh it with `ipa scan` only if the wave needs it)
scp build/frontier.json build/ipa_members.json rocky:rush2049/recomp/dc/repo/build/
```

# 3. --all-lanes + census on the final wave tree (wave33 1fe5fe8c0, game 1077/1216)

Rocky service tree `~/rush2049/recomp/dc/repo` rsynced by the coordinator from wave33 1fe5fe8c0, with its freshly
scanned build/frontier.json (new clause, 1077 matched) and build/ipa_members.json. Every row recomputed with
`--refresh` (the cache is ignored, rows rewritten; same effect as an epoch bump without editing the service tree's
code, which this lane was told not to sync). Command (on rocky, in the service tree):
`RUSH_DC_BUILD_JOBS=8 python3 -m tools.recomp.draft_check --all-lanes --refresh --json ~/rush2049/recomp/dc/lanes/w33r-all.json`
Log/JSON: rocky `~/rush2049/recomp/dc/lanes/w33r-all.{log,json}`.

`ALL LANES w2[8-9]*,w3*: 115 drafts: 40 EQUIVALENT, 2 ERROR, 73 MATCHED`; DIVERGES 0, FAULT 0.
- The five formerly-ERROR dirs: w31a/hs and w32a/hs EQUIVALENT 2,473 calls / 4 scripts; w31a/f5ea8 (animResults)
  EQUIVALENT 26,166 calls / 6 scripts; w31a/audio and w32b/audio MATCHED.
- w33a/hs (its own dc.args, `--unit ... --params DrawName:p=s5`, default scripts): EQUIVALENT over 4,921 calls /
  6 scripts (ghost_pak 1144, ghost_resave 1144, two_player_finish 496, ghost_save 900, player_finish 863,
  ghost_delete_fail 374). DrawName/DrawGhost/DrawUnlocked are locked now; highscore_animText is not, so it runs.
- The 2 ERRORs were new w33a unit dirs whose NAME is not the directory: `w33a/ar` (dc.args had only
  `--params arg0=a0`) and `w33a/f0e58` (no dc.args). Fixed here: `ar/dc.args` `--name animResults --params arg0=a0`,
  `f0e58/dc.args` `--name func_80100E58`. Run by hand on rocky (drafts already in the service tree):
  `draft_check animResults cloud/work/frontier/w33a/ar/best.c --params arg0=a0` ->
  `VERDICT: EQUIVALENT over 26166 calls / 6 scripts (stunt_end 9336, pause_menus 3736, two_player_finish 3390, ghost_save 3268, race_finish 3218, unlock 3218)`;
  `draft_check func_80100E58 cloud/work/frontier/w33a/f0e58/best.c` ->
  `VERDICT: EQUIVALENT over 3405 calls / 6 scripts (saves 1395, race_finish 480, ghost_pak 420, ghost_resave 420, unlock 420, player_finish 270)`.
  With those dc.args the table would read 42 EQUIVALENT, 0 ERROR, 73 MATCHED once the service tree has them.
- Newly MATCHED this wave (rows w33d control_settings, w33p entity_iterate, plus older lanes' drafts of the wave-33
  matches) - no draft check needed.

Census (rocky, same tree, 39 scripts; logs `~/rush2049/recomp/dc/lanes/w33r-census{,-ovl}.log`; files copied back to
tools/recomp/census/ and committed): **139 unmatched: 136 reached (152,616 B), 3 never (632 B)**:
world_effect_update (456), fp_call_wrapper (104), object_bytes23_sum (72), unchanged since wave 31 (no static
reference). Was 147 of 150 at wave 32. Overlays (`--overlays`): image A 99 of 193 reached (unmatched 45 of 112,
92,504 of 101,892 B), image B 35 of 49 (unmatched 19 of 28, 28,696 of 29,688 B): unchanged (no overlay matches in
the census tree's ovl lock change their reach).

# Checks

Worktree, tools/mips_to_c and tools/decomp-permuter symlinked to the main checkout's submodules during the run
(restored to empty dirs after), `-n 4 --dist loadgroup`: tests/conveyor + tests/recomp `1744 passed, 819 skipped`;
tests/cloud -k "recomp or frontier" `75 passed, 10 skipped`.

## Permission denials
- `git ls-files | rsync -a --files-from=- . rocky:rush2049/recomp/dc/repo/` (refresh the shared rocky service tree
  from this worktree) was denied (Modify Shared Resources). Not retried; the job-1 rows ran through
  `draft_check --remote rocky` (per-run inbox) against the service tree as w32r left it. Job 3 needs the service
  tree refreshed to the final wave tree: the coordinator runs that rsync, or grants it.
