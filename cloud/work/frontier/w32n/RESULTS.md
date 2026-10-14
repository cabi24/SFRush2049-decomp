# w32n: rename batch w32n + conveyor register/clobber facts (RESULTS)

Lane w32n, wave 32, worktree branch `w32n` from `wave32` = d1c53ab2. Names and facts only: coverage unchanged
(game 1056/1216, image bf7da3fa…, ROM SHA-1 EXACT).

## Job 1: rename batch w32n (19 functions)

Single source of truth: `cloud/work/frontier/w29n/renames.json`, rows with `batch: w32n`; reader's table in
`docs/external/gc/names.md` ("Applied in wave 32"). Naming rule (stated in both files): a GC file-static name that
recurs in several GC source files (`_InitScreen` x3, `animText` x17, `animButtonText` x8, `printOption` x2) gets the
N64 screen-file prefix (`highscore_`, `options_`, `audio_`, `controls_`) and keeps GC's spelling after it (the same
CamelCase as the file's PrintScores/SetHighScoreState/FindGhosts); a GC name unique in the GC build keeps its
spelling without the CodeWarrior underscore. (The coordinator's example `highscore_init_screen` was not used, so
the four highscorescr.c-prefixed names read alike: highscore_InitScreen / highscore_animText.)

| Address | Old | New | GC source | Locked? |
|---|---|---|---|---|
| 0x800F5F90 | track_texture_load | highscore_InitScreen | highscorescr.c _InitScreen | yes (frontier_billboard_render) |
| 0x80104704 | func_80104704 | DrawName | highscorescr.c DrawName(p) | no |
| 0x80104B14 | func_80104B14 | DrawGhost | highscorescr.c DrawGhost | no |
| 0x80103D28 | func_80103D28 | DrawUnlocked | highscorescr.c DrawUnlocked | no |
| 0x80105480 | func_80105480 | highscore_animText | highscorescr.c animText | no |
| 0x80105EA8 | func_80105EA8 | animResults | hud.c animResults | no |
| 0x800CC040 | func_800CC040 | SaveGhost | save.c SaveGhost | yes (menu_save_options) |
| 0x800CC50C | func_800CC50C | CompressGhost | save.c CompressGhost | no |
| 0x80102980 | func_80102980 | drawBattleScore | gameoverscr.c drawBattleScore | yes (frontier_results_table) |
| 0x80108AB0 | func_80108AB0 | animWrongWay | hud.c animWrongWay | yes (credits_scroll_grp) |
| 0x8010DCFC | func_8010DCFC | StartCone | targets.c _StartCone | yes (single; `git mv` -> src/blob/StartCone.c) |
| 0x8010AEAC | func_8010AEAC | options_animText | optionscr.c animText | yes (group func_8010AEAC) |
| 0x8010A8D0 | func_8010A8D0 | options_animButtonText | optionscr.c animButtonText | yes (group func_8010AEAC) |
| 0x8010A7A4 | func_8010A7A4 | options_printOption | optionscr.c printOption | yes (group func_8010AEAC) |
| 0x800FF298 | game_results_render | audio_animText | audioscr.c animText | no |
| 0x800FEE04 | game_results_update | audio_animButtonText | audioscr.c animButtonText | no (w29a candidate row accepted in place) |
| 0x80100564 | func_80100564 | controls_animText | controlscr.c animText | no |
| 0x800FFDE8 | game_results_exit | controls_animButtonText | controlscr.c animButtonText(signed char p) | no |
| 0x800F6AB8 | props_render | GhostStart | ghost.c GhostStart | no at d1c53ab2; w32c matched it, lands in menu_save_options via w32i (coordinator request) |

symbol_addrs.us.txt defined `game_results_exit = 0x800FFDF8`, 16 B inside the function (no jal or pointer to
0x800FFDF8 anywhere in the image; the only call is `jal 0x800FFDE8` at 0x80100604 in controls_animText). The row
carries `symbol_addrs_was: 0x800FFDF8`; symaddr_rename.py renames that definition and moves it to 0x800FFDE8 (blob.ld
PROVIDE follows; the section symbol already was at DE8). Group directory/file names stay (ids:
`groups/func_8010AEAC/`, `track_texture_load.c` inside frontier_billboard_render), as do lane directory names
(draft_check maps them through renames.json). `tools/recomp/identity.json` maps static recompiler names only: no
game name, no change.

### Replay changes (inputs commit 2/3; all in `cloud/work/frontier/w29n/`)
- `replay.sh`: step 1 now derives the layout from the DB copy and runs `blob_splice --data COPY rebuild-stale` on
  the PRE-rename tree (the main checkout's copied objects were stale for 4 singles + 13 groups -> `LockedBodyError`
  in the lock step otherwise); drafts step covers every tracked C/H of `w2[89]?/` too (w28c's `dc_static.c`, which
  the rocky e2e test runs); new `tools/lane_rename.py` (dc.args argument lines; lane group.json name values, proved
  value-only on hand-formatted files); rebind list + phase-11/ipakit tests, dot_menu_row_caller, dot_menu_options_root,
  dot_cone_start_donor, dot_speed_flags_c9210, `ipa-groups/billboard_render/br.c`,
  `heads_B14/func_8010DCFC_singleformal.c` (the cone packet compiles it as a control).
- `rebind_packets.py --users-only` (replay passes it): a packet's C/H files are renamed only when one of them USES a
  batch name (users.py, the src/blob rule) — dot_steady_camera's submission group.c must stay byte-equal to the
  untouched src/blob group plus its candidate.
- `rebind.py`: an untouched SOURCE_PINS file is skipped (its prelude may declare a batch name); LOOKUPS of another
  batch are skipped. `rebind_edits.py`: EDITS of another batch skipped; new edit: test_recomp_phase9's draft_check
  name -> options_animText (its `w28c/func_8010AEAC/` path stays); new DERIVED pins: control sources a verifier
  derives by text replacement from a renamed source (dot_menu_row_caller's 3 rejected controls, dot_cone_start_donor's
  authentic_scale_macro/genuine_basis_36) — proof: base source + the verifier's replacements hashes to the pinned
  value, live = that text renamed; the new values equal what verify.py produced on rocky. `followups.py`: census/README.md table cells;
  `overlays.json` (frozen overlay names, new since w31r) skipped — the census step crashed on it before.
- `symaddr_rename.py`: `symbol_addrs_was` (above).

### Replay result (output commit, nothing by hand)
19 inventory rows (DB copy), 4 symbol_addrs definitions, 9 src/blob sources + 60 lane C/H files, 3 dc.args + 9 lane
group.json, 5 src/blob group.json + unit_overrides, 8 lock keys re-keyed, 50 entries re-pinned (1 single, 5 groups;
`w29n/blob_lock_migration_w32n.json`), 39 + 3 fixture files rebound, CLAIMS.md, ipa_callees.json (func_80104704 ->
DrawName), 34 census files + README, correlation. Gates (in the replay):
- `image matches (647072 bytes) bf7da3fa6283428a97372250cd4076d15e9eae10f9d5709c0387fe0742d43a1d`
- `blob lock: 1056 entries, 0 problems`; `group lock: 0 problems`
- `blob_unit: 1056 locked bodies, 1056 equal to the image in one 930-file unit, 0 differ`
- `ROM build with the source-built blob: SHA-1 EXACT`

## Job 2: conveyor facts (inputs commit 1/3)

Three corrections, all in the scanner behind `build/ipa_members.json` and `build/frontier.json`:
1. `ipa.py` stack-argument rule (w31r): a callee-saved store to the stack is a save only when the body reloads that
   slot or makes no call. (w31r's `ipa_detect` also counts "the body writes the register" as a save; with that clause
   control_cell_text2 keeps s5,s6,s7 — it passes s8 as a stack argument, then reuses s8 unsaved. Without it the whole
   image changes in exactly the two functions w31r reported. Suggest the same one-line change in ipa_detect.)
2. `frontier.unsaved_callee_writes` spill rule (w32r, coordinator request): a store is a save only before the first
   write of the register. `frontier calibrate`: unsaved detector still 0/693 standalone locks.
3. `ipa.parse` (coordinator request: drop animResults' t9): the cause was the DERIVED ASM, not the function. The m2c
   asm renders a load/store off a pointer register as a synthetic labelled `lui $r,%hi(sym)` + the real unlabelled
   `op $x,%lo(sym)($r)` (824 pairs in the image). Read literally, the fake lui writes $r and pushes the real
   instruction out of a delay slot: animResults 0x80105F2C `bnel a3,t8,F3C; lh t9,0x7C6(s3)` (words 54F80003
   867907C6) became `bnel; lui s3` so the taken path never loaded t9. parse() now drops such a lui (labels move to
   the real instruction). This also removes lap_count_select t8 and split_time_display t4 — the three callees
   tools/recomp/README already listed as "missed" by ipa_detect (they were artifacts).
Tests: test_ipa.py (+5; one fixture gained the `ldc1 $f20` restore every IDO save has), test_frontier.py (+1).

Facts diff, pre-w32n scanner (d1c53ab2's ipa.py/frontier.py) vs now, same tree and DB copy, all 1,216 functions:
- register params: control_cell_text s5,s6 -> s5,s6,s7; control_cell_text2 s5,s6,s7 -> s5-s8; animResults t9 ->
  none; lap_count_select t8 -> none; split_time_display t4 -> none.
- unsaved (integer only; the conveyor never tracked f20-f30): func_80097164 s1,s3 -> s0-s4; track_data_decompress
  +s7,s8; func_800C7EC8 +s1,s2,s8; func_800D6914 +s8; control_cell_text3 +s1; control_cell_text2 +s3;
  func_800DD0C0 +s0,s1,s3 (= s0-s8); func_800E398C +s0; func_800E92C8 +s2; SetHighScoreState +s8;
  countdown_handler +s0,s1; func_80100E58 +s1,s3,s6; func_80101904 +s1; func_80101D84 +s1,s6; DrawGhost +s2;
  options_animButtonText +s1. 16 functions = the integer part of w32r's list as I reproduce it with ipa_detect (23;
  the other 7 are FP-only: func_8009F058, input_process_controller, func_800B59F0, camera_look_at_point,
  mode_select_handler, func_800E0050, func_800E451C). DrawName (+s5 in w32r's list) is in neither diff: the conveyor
  already had s0-s8 for it. w32r's hand table has func_80097164 s0-s7; the scan says s0-s4.
- preservers (parse fix): Input_ProcessGameplayPad -t7, animResults -t8, camera_target_track -a0,
  physics_friction_apply -t7; removed entirely: car_stats_display, controls_animButtonText, controls_animText,
  drone_collision_avoid, func_800B4FB0, func_800F1114, func_800F45F8, minimap_render, physics_velocity_clamp,
  process_inputs; new: func_800B7438 a0 (the fake lui had hidden a read of a0 after a call).
- callers: func_800D2FA8 (split_time_display) gone; stunt_combo_display loses lap_count_select.
- members: +func_800B7438; -car_stats_display, func_800D2FA8, func_800F1114, func_800F45F8, lap_count_select,
  minimap_render, physics_velocity_clamp, process_inputs, split_time_display (internal: same minus func_800F1114).
- frontier recipes among unmatched: only animResults unit [t9] -> group [] (it still writes callee-saved/ring).
- Not changed, flagged: control_cell_text2's s8 clobber (stored as an argument BEFORE its first write, so the spill
  rule counts it as saved; the same in w32r's Interproc).
No committed receipt or test pins ipa_members/frontier contents (build/ products): nothing to re-pin. Regenerated in
the worktree: `build/ipa_members.json` (360 members, 91 callees, 123 unsaved), `build/frontier.json`.

## Integration (coordinator)

Re-application on the merged commit W (lanes w32a–d use the OLD names; this branch goes on last):
```
git checkout -B w32n W
git cherry-pick c4cdd4154 0b7528d63          # inputs 1/3 (facts), 2/3 (rename table + replay)
RENAME_BATCH=w32n sh cloud/work/frontier/w29n/replay.sh   # clean tree; copies build/ from the main checkout
git add -A -- . ':!cloud/work/frontier/w32n/RESULTS.md'; git commit   # output, untouched
git cherry-pick <RESULTS commit>
```
then both test-fasts (a new fixture pinning a renamed name shows up only there: extend the replay.sh rebind list /
rebind_edits.py in inputs 2/3 and redo). New w32a–d packets/drafts under `cloud/work/frontier/w32?/` are renamed by
the replay (C/H, dc.args, group.json); packets installed in src/blob by w32i are covered by the users.py scan.

Live Conveyor DB (Pi `/home/cburnes/.conveyor/conveyor.db`), from the main checkout after merging:
```
python3 -c "import sqlite3; s=sqlite3.connect('/home/cburnes/.conveyor/conveyor.db'); d=sqlite3.connect('/home/cburnes/.conveyor/conveyor.pre-w32n.db'); s.backup(d); d.close()"
python3 cloud/work/frontier/w29n/tools/batch.py cloud/work/frontier/w29n/renames.json w32n build/rename_batch_w32n.json
python3 cloud/work/frontier/w29n/tools/db_rename.py build/rename_batch_w32n.json --apply
python3 -m tools.conveyor.pipeline.blob_layout derive
python3 -m tools.conveyor.pipeline.blob_tu symbols          # must leave asm/us/blob/symbols.json unchanged
python3 -m tools.conveyor.pipeline.ipa scan                 # Job 2: build/ipa_members.json with the new rules
python3 -m tools.conveyor.pipeline.frontier scan            # build/frontier.json
```
In-flight drafts with old names: `python3 -m tools.conveyor.pipeline.static_rename source build/rename_batch_w32n.json FILE…`.
Recomp (rocky service tree): `build/frontier.json` there should be regenerated the same way after the merge.

## Tests (at the output commit 4ad453b3)
- Pi worktree `make test-fast JOBS=4`: conveyor `1592 passed, 799 skipped`; cloud `841 passed, 205 skipped`; rc=0.
- rocky `REQUIRE_TOOLCHAIN=1 make test-fast JOBS=16` (clone `~/rush2049/scratch/w32n`, CI env): conveyor `2339 passed,
  52 skipped`; cloud `1041 passed, 5 skipped, 8 warnings, 27 subtests passed`; rc=0. (The first rocky run found the
  dot_menu_row_caller / dot_cone_start_donor derived-control pins -> DERIVED proofs above.)

## Builder / hosts
watchman2 scratch `~/rush2049/scratch/frontier/w32n` (replay compiles) deleted at the end. Pi worktree: the main
checkout's `tools/mips_to_c` and `tools/decomp-permuter` were symlinked into the worktree's empty submodule dirs for
the test runs only, and removed afterwards. rocky clone `~/rush2049/scratch/w32n` (copied from w31r's clone, branch
from a bundle, origin/master = d1c53ab2).

## Permission denials
None.

## Final application on W = 7d835eb4 (w32i installs merged; game 1066/1216)

Series: inputs 08656246 (facts) / ad4c02fc (rename table + replay) cherry-picked, output 8b92a0ef
(`RENAME_BATCH=w32n sh cloud/work/frontier/w29n/replay.sh`, untouched), RESULTS 4dca3668 + this section.
Replay: 19 inventory rows (DB copy), 4 symbol_addrs definitions, 11 src/blob sources (incl. the newly installed
menu_save_options props.c, codex_control_settings_a4, frontier_audioscr, frontier_optionscr_buttons files that use a
batch name) + 73 lane C/H files (w28*–w32*), 12 lock keys re-keyed, 55 entries re-pinned; derived-control pins as
before; no tracked src/blob C/H still USES an old name (users.py; only prose notes keep them).
Gates: `image matches (647072 bytes) bf7da3fa6283428a97372250cd4076d15e9eae10f9d5709c0387fe0742d43a1d`;
`blob lock: 1066 entries, 0 problems`; `group lock: 0 problems`; `blob_unit: 1066 locked bodies, 1066 equal to the
image in one 934-file unit, 0 differ`; `ROM build with the source-built blob: SHA-1 EXACT`.
Tests: Pi `make test-fast JOBS=4` conveyor `1591 passed, 799 skipped`, cloud `841 passed, 205 skipped`, rc=0 (main
checkout submodules symlinked temporarily, removed); rocky `REQUIRE_TOOLCHAIN=1 make test-fast JOBS=16` (ci.env on
PATH) conveyor `2338 passed, 52 skipped`, cloud `1041 passed, 5 skipped, 27 subtests passed`, rc=0.
