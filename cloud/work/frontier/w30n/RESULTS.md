# w30n: register func_80104704 + apply the 10 deferred w29n renames (RESULTS)

Lane w30n, wave 30, worktree branch `w30n` from `wave30` = 1b34f797. Coverage unchanged by design (population
and names only): game 1041/1216 functions locked.

## Job 1: the unregistered head func_80104704

**Registered: func_80104704** (0x80104704..0x80104B14, 260 words, 1,040 B, frame 136). Path: the
refresh-game-context skill's first step, `closure run`, on a sqlite COPY of the live Conveyor DB
(`build/w30n_data`, blob store hard-linked so nothing is added to `~/.conveyor`). It discovers 0x80104704 from
the `jal` at 0x80105730 in func_80105480 (registered since 2026-10-01 as a switch head, so closure had never
been re-run over it), the 005 extent scanner gives 260 words, and closure's suffix rule marks the inventory row
`highscore_entry_anim` (0x80104A58, 47 words, `extent_repaired`) **`extent_conflict:func_80104704`**. A second
`closure run` registers 0 (fixpoint; the 30 other candidates are `invalid: outside_image`, calls into static
code). `targets.relocate_extracted(names=['func_80104704'])`: upgraded to a reloc-aware object.

Why it had been refused (specs/012 findings): `closure heads` bounds a head by the next known extent, and the
`bne`/`b` at 0x80104A48/0x80104A50 enter `highscore_entry_anim`'s start. That row is not a function: the code
before it sets `a0` (16 or 22) on both paths and the label's first instruction is the `jal` that consumes it; no
`jal 0x80104A58` (0x0C041296) and no pointer word 0x80104A58 exists in the game image or in runtime images A/B.

**highscore_entry_anim: retired** as a function (inventory `extent_conflict:func_80104704`, layout/asm/blob.ld
section gone). The NAME stays defined in `symbol_addrs.us.txt` as an interior label, exactly like the earlier
tail labels `game_over_screen`, `name_entry_screen`, `achievements_check`, so it remains an address in
`asm/us/blob/symbols.json`/`blob.ld` PROVIDE (removing it would re-pin the owned-context hashes for no gain;
the ~410 generated prelude declarations of it in src/blob are unused). Frozen overlay/boot_tail symbols.json
untouched. Recorded on CLAIMS.md (new row + the results-family row).

Regenerated through the tools (never by hand): `blob_layout --data build/w30n_data derive` (1216 functions,
561,860 B, 86.8%; opaque 11 runs), `blob_splice rebuild-stale` (the main checkout's copied objects were stale),
`blob_tu generate`: `asm/us/blob/blob_80100d24.s` (opaque run 0x801046FC shrinks 860 -> 8 B, section
`.text.func_80104704` replaces `.text.highscore_entry_anim`), `src/blob/blob.ld`, `symbols.json` (+1 name),
`SHA256SUMS`. Replayable: `sh cloud/work/frontier/w30n/register.sh` (copy DB, closure x2, derive,
rebuild-stale, generate, re-pins).

Re-pins (`cloud/work/frontier/w30n/tools/repin_heads.py`, each proved before writing; the proof undoes the
registration on today's parsed target table — head removed, retired label restored with the head's words from
0x80104A58 — see `heads_undo.py`, data in `heads.json`):
- symbols.json pins of dot_nearest_point_donor_20261005 / dot_target_orientation_20261005: today's file is
  exactly the HEAD table plus `func_80104704` (2152d6ca… -> b84a0560…);
- census digests (w29n rebind.py CENSUS_PINS): dot_static_10450/12234 (6ab5d1ff… -> 120deace…),
  dot_runtime_b_hud_teardown/radial_impulse (f9ee4063… -> b2f05ba4…): digest(undo(today)) = pinned value;
- dot_slot_contract_audit_20261006: `audit.verify()` over undo(today) accepts the saved evidence; evidence
  regenerated (= `audit.py --write`): func_80104704 is a new slot_state_setup caller (site 0x80104724, s0
  capture, other unaccepted call state_utility) -> test counts 65/170/167 -> 66/171/168, captures 161 -> 162,
  unaccepted caller bytes 56,236 -> 57,276 (func_80105480's dependency list gains func_80104704);
- dot_source_boundary_scout_20261005: native_summary over undo(today) = receipt; word_count 140,252 -> 140,465
  (+213 = 260 - 47); function_count stays 1216;
- tools/recomp/census/census.json: the `highscore_entry_anim` entry removed (its probe sat on the label, so its
  counts were passes through the label, not calls; draft_check runs every script for func_80104704 until the
  next census refresh probes it — w30r).
Test edits (inputs): tests/cloud/test_ipakit_heads.py (no audited head remains; 0x80104A58 is inside
func_80104704; no registered tail label left), tests/cloud/test_ipakit_deps.py (the TAIL case has no live label
left: asserts highscore_entry_anim is gone and func_80104704 classifies normally).

Other heads in `cloud/work/unregistered-heads.md`: every "Confirmed" head and 0x8010C2E4 are registered (45 by
`closure heads`, six switch heads 62f8b2f3, func_80104704 now). **Left**: the 2-word `jr ra; nop` stubs (the
file's own "Uncertain" table). `closure orphans` (dry run) accepts 13 today — 0x80102440, 0x80103D18,
0x80103D20, 0x801046FC, 0x80105478, 0x8010A534, 0x8010C024 (the seven listed) and 0x8010C7E4, 0x8010C7EC,
0x8010E8AC, 0x8010F210, 0x8010FBA4, 0x8010FBAC — registering them moves the game denominator 1216 -> 1229, an
owner/coordinator decision (`closure --data DIR orphans --apply`, then the same derive/generate/re-pin chain).
Status section added at the top of unregistered-heads.md.

Context refresh (skill), on the DB copy, build/ only (not committed): `datasyms generate` (2,207 symbols),
`protos generate` x2 (byte-stable: see below), histogram (see below). NOTE: the main checkout's
`build/m2c_datasyms.json` is stale (2,185 entries, still lists the five data names w29n moved into
GAME_SYMBOLS); the committed symbols.json/blob.ld here were generated with that same main copy, as w29n did, so
`blob_tu symbols` in the main checkout reproduces them. Refreshing datasyms in main adds ~22 generated `D_`
names to symbols.json/blob.ld (independent of this lane; re-pin the two symbols.json receipts after it).

Skill instruments run here on the DB copy (build/ only): `datasyms generate` 2,207 symbols; `protos generate`
(first pass 39 min, `declarations=1150 omitted=145 datasyms_externs=2207`; the byte-stability second pass was
stopped when the DB copy was re-made for Job 2, so stability is NOT claimed here); the population histogram
(`autodecomp clusters --population extracted --limit 0`) was NOT run (multi-hour m2c sweep) — the coordinator
runs the four skill commands in the main checkout after the live DB update.

Job 1 gates (commit b1181667): `blob lock: 1041 entries, 0 problems`; `group lock: 0 problems`;
`blob_unit: 1041 locked bodies, 1041 equal to the image in one 925-file unit, 0 differ`; `blob_rom rom`:
`ROM build with the source-built blob: SHA-1 EXACT`, `ROM matches!`, `MAKE=0 TEST=0`.

## Job 2: the 10 deferred w29n renames (batch w30n)

| Address | Old | New | Fixture that blocked it -> how it moved |
|---|---|---|---|
| 0x800BB9B0 | func_800BB9B0 | LoadTrack | dot_node_pool_init (receipt + test caller name): packet token rename |
| 0x800FF724 | game_results_input | animControls | d05_hud_context_audit (evidence key + parametrised test): packet token rename; locked single moved `src/blob/animControls.c`, lock key re-keyed |
| 0x80151690 | D_80151690 | gTotalNames | dot_name_cache (+ its byte-equal tiny_A68 copy), dot_billboard_context/helpers: token rename, pins re-hashed |
| 0x801161F4 | D_801161F4 | gRomObjDefPtrs | dot_material_selector (+ its archived seed near-miss/func_8008B000/base.c): token rename |
| 0x80151AE8 | D_80151AE8 | gTexDefPtrs | src/blob/func_800B24EC.c pins in blit_family / new_multiblit / availability_root: path-aware re-pin |
| 0x80138670 | D_80138670 | gPalDefPtrs | tests/conveyor/test_disasm.py fixture: token rename |
| 0x80150B70 | D_80150B70 | gCamera | dot_steady_camera (+ its registered submission ipa-groups/dot_steady_camera): token rename |
| 0x80151AD0 | D_80151AD0 | gNumWindows | frame_dispatch, vector_blend, slot_setter_boundary packets; radius_effect pins |
| 0x80152032 | D_80152032 | gCountDown | dot_frame_dispatch (Effects_UpdateEmitters.c pin, name lookup): token rename |
| 0x80140BDC | D_80140BDC | gNumModels | availability_root's `extern volatile u8` text check (exact edit), slot_setter_boundary, resource_init(ializer) packets |

Single source of truth: `cloud/work/frontier/w29n/renames.json` — every accepted row now carries `batch`
(w29n: 27 rows, w30n: these 10; `was_deferred` keeps w29n's blocking note). names.md: the deferred table became
"Applied in wave 30 by lane w30n". Method = w29n's: `RENAME_BATCH=w30n sh cloud/work/frontier/w29n/replay.sh`
on the inputs commit, output committed untouched (9312cb9a). Replay changes made for batches
(inputs commits 26c9bef1..0de5b8af):
- `tools/batch.py` writes the batch table; inventory/symbol_addrs/sources/json/lock/CLAIMS steps get only the
  batch (earlier batches' old names no longer exist; sources keep stale prelude declarations of them, so a
  whole-table permute would break the pure-rename proof), token-only tools the whole table;
- step 1 runs `closure run` on the DB copy before `db_rename` (the copy otherwise lacks Job 1's head); the
  blob store is a hard-linked copy (a symlink would let registration write into `~/.conveyor/blobs`);
- per-batch owned-context rebind dir (`w30n/rebind/`; rom_owned_data.json pointed at w29n's proof file);
- `rebind.py --map` (census digests undo only the batch); `rebind_packets.py --map`;
- new `rebind_edits.py`: exact game-side edits + path-aware re-pins in mixed fixtures (runtime-image packets
  whose overlay sources keep old spellings; HUD-bar packets whose harness derives addresses from `D_<addr>`
  names in cloud/matches copies). Re-pins are by PATH, not hash: cloud/matches/func_800EF62C.c is
  byte-identical to the src/blob source and a hash-only rule re-pinned it wrongly (found and fixed);
- `followups.py`: census function keys renamed structurally in the generator's format. **Fixes a w29n bug**:
  its word rename turned the census SCRIPT `race_finish` into `animPlace` (census.json script list, 128
  per-function script keys, race_finish.json `script`), which made draft_check pick a nonexistent script;
  proof: today's structural rename == the pre-w29n census (07b9bf89) structurally renamed, all 30 files.
Replay result: 2 inventory rows (copy), 1 symbol_addrs definition, 77 src/blob + 17 lane-draft sources
renamed, 1 lock key re-keyed, 151 entries re-pinned (49 singles, 20 groups,
`w29n/blob_lock_migration_w30n.json`), 42 + 6 fixture files rebound.
Job 2 gates: `blob lock: 1041 entries, 0 problems`; `group lock: 0 problems`; `blob_unit: 1041 locked bodies,
1041 equal to the image in one 925-file unit, 0 differ`; `ROM build with the source-built blob: SHA-1 EXACT`
(image bf7da3fa…).

## Integration (coordinator)

Live Conveyor DB (`/home/cburnes/.conveyor/conveyor.db`), from the main checkout after merging w30n, in order:
```
python3 -c "import sqlite3; s=sqlite3.connect('/home/cburnes/.conveyor/conveyor.db'); d=sqlite3.connect('/home/cburnes/.conveyor/conveyor.pre-w30n.db'); s.backup(d); d.close()"
python3 -m tools.conveyor.pipeline.closure run                      # registers func_80104704, retires highscore_entry_anim
python3 -m tools.conveyor.pipeline.closure run                      # must report registered=0
python3 -c "from pathlib import Path; from tools.conveyor.coordinator import db; from tools.conveyor.coordinator.store import BlobStore; from tools.conveyor.pipeline import targets; d=Path.home()/'.conveyor'; print(targets.relocate_extracted(db.connect(d/'conveyor.db'), BlobStore(d/'blobs'), names=['func_80104704']))"
python3 cloud/work/frontier/w29n/tools/batch.py cloud/work/frontier/w29n/renames.json w30n build/rename_batch_w30n.json
python3 cloud/work/frontier/w29n/tools/db_rename.py build/rename_batch_w30n.json --apply   # LoadTrack, animControls
python3 -m tools.conveyor.pipeline.blob_layout derive
python3 -m tools.conveyor.pipeline.blob_tu symbols                  # must leave asm/us/blob/symbols.json unchanged
```
(relocate needs the decomp-permuter submodule.) Then the skill refresh in the main checkout: `datasyms
generate`, `protos generate` (x2), `autodecomp clusters --population extracted --limit 0`. Main's
build/m2c_datasyms.json is stale (pre-w29n); after `datasyms generate` there, `blob_tu symbols`/a splice adds
~22 generated `D_` names to symbols.json/blob.ld — re-pin the two symbols.json receipts then
(`repin_heads.py` handles only the registration; the sha swap is the w29n step-7 sed).
In-flight drafts using old names: `python3 -m tools.conveyor.pipeline.static_rename source
build/rename_batch_w30n.json FILE.c` (lane drafts w2[89]?/w3??/*/best.c were renamed by the replay).
Not changed: overlay/boot_tail symbols.json and overlay sources (frozen, old spellings), cloud/matches,
include/game_types.h, highscore_entry_anim's symbol_addrs line (interior label like game_over_screen).
If wave30 moves: `git checkout -B w30n wave30; git cherry-pick` the input commits, `sh
cloud/work/frontier/w30n/register.sh`, commit, then `RENAME_BATCH=w30n sh cloud/work/frontier/w29n/replay.sh`,
commit, run both test-fasts (fixtures the replay does not know only show up there).

Post-replay fix (found by rocky): dot_steady_camera's fresh IDO object differs from its receipt only in
.strtab/.symtab (`gCamera` is 3 bytes shorter than `D_80150B70`); proof on rocky: the fresh .strtab with the
name renamed back hashes to the pinned d688083c…, the .symtab with st_name remapped to it to the pinned
4249206b…, every other section equal. Recorded as `rebind_edits.py` OBJECT_PINS (commits 31b1b06b, 7d3d1856).
Merged wave30 (931baefa, lane dirs only) at cc9aa998. **The w30a–w30d packets use old names** (w30a
audio_options/controls/func_80105480, w30b func_800DB1E0/func_80108AB0/func_8010AEAC + groups
credits_scroll_grp/func_8010AEAC, w30d LoadTrack/animPlace + groups/credits_scroll_grp: D_80150B70,
D_80151AD0, D_80140BDC, D_80151AE8, func_800BB9B0): before installing any of them on top of w30n run
`python3 -m tools.conveyor.pipeline.static_rename source build/rename_batch_w30n.json FILE…` (batch table from
`tools/batch.py`), or `--with` unit runs fail to link.

## Tests (at cc9aa998)
- Pi worktree `make test-fast JOBS=4`: conveyor `1590 passed, 804 skipped`; cloud `808 passed, 202 skipped`; rc=0.
- rocky `REQUIRE_TOOLCHAIN=1 make test-fast JOBS=16` (CI venv): conveyor `2342 passed, 52 skipped`; cloud
  `1005 passed, 5 skipped, 8 warnings, 27 subtests passed`; rc=0.
- Gates at the replay output (9312cb9a): `blob lock: 1041 entries, 0 problems`, `group lock: 0 problems`,
  `blob_unit: 1041 locked bodies, 1041 equal to the image in one 925-file unit, 0 differ`,
  `ROM build with the source-built blob: SHA-1 EXACT`; re-checked at cc9aa998: lock/group 0 problems.

## Builder / hosts
watchman2 scratch `~/rush2049/scratch/frontier/w30n` (+ `unit/w30n`) deleted at the end. Pi worktree: the
main checkout's tools/decomp-permuter and tools/mips_to_c contents were copied into the worktree's empty
submodule dirs (symlinks break `git status`) for relocate_extracted and the tests, removed at the end.
rocky clone `~/rush2049/scratch/w30n` (copied from w29n's clone, branch from a bundle, origin/master =
1b34f797). `blob_rom rom` uses the builder gate repo `~/rush2049/repo` as designed (rsync of blob/streams).

## Permission denials
None.

## Re-application series (coordinator request: hold until w30i merges)

Branch `w30n` is now three INPUT commits on wave30, plus generated-output commits on top:
1. `w30n inputs 1/3` — register.sh, heads.json, tools/repin_heads.py + heads_undo.py, the ipakit test edits,
   the CLAIMS row, unregistered-heads.md;
2. `w30n inputs 2/3` — renames.json (batches), replay.sh (batch-aware; now also renames every tracked C/H file
   under `cloud/work/frontier/w3[0-9]?/`, i.e. wave-30 drafts and group packets incl. ctx_*.c), w29n tools
   (batch.py, rebind_edits.py, rebind/rebind_packets --map, followups census fix, claims, symaddr), names.md;
3. `w30n inputs 3/3` — this RESULTS.md.
Outputs, each produced by a script on a clean tree and committed untouched:
`sh cloud/work/frontier/w30n/register.sh` (Job 1), then `RENAME_BATCH=w30n sh cloud/work/frontier/w29n/replay.sh`
(Job 2; installed src/blob groups/singles are covered by its src/blob user scan).
To re-apply on a merged wave30 W: `git checkout -B w30n W; git cherry-pick <inputs 1..3>; sh
cloud/work/frontier/w30n/register.sh; git add -A …; git commit; RENAME_BATCH=w30n sh
cloud/work/frontier/w29n/replay.sh; git add -A …; git commit`; then both test-fasts (new fixtures pinning a
renamed name show up only there: extend the replay.sh rebind list / rebind_edits.py and redo the replay).

## Final application on W = faaaee20 (w30i installs merged)

Series: inputs cf697ad9 / 6fe7fcc5 / ba51ded0 (cherry-picks; CLAIMS conflict resolved by re-adding the w30n row
and marking func_80104704 registered in the results-family row), Job 1 output 97d7b777 (`register.sh`), Job 2
output 44bb2f76 (`replay.sh`, RENAME_BATCH=w30n). dot_steady_camera's group func_800E92C8 is untouched by w30i
(`git diff 931baefa faaaee20 -- src/blob/groups/func_800E92C8` empty), so the OBJECT_PINS proof stands.
Job 1: slot-contract counts 66/171/168, captures 162, unaccepted caller bytes 53,504 (w30i landed callers).
Job 2: 79 src/blob + 42 draft/packet sources renamed (incl. menu_save_options ctx files, credits_scroll_grp
gr3_7af4.c and the 8AB0 file, func_8010B5D0, func_8010DCFC and the w30a-d lane dirs), 1 lock key re-keyed,
153 entries re-pinned.
Gates: `blob lock: 1050 entries, 0 problems`; `group lock: 0 problems`; `blob_unit: 1050 locked bodies, 1050
equal to the image in one 925-file unit, 0 differ`; `ROM build with the source-built blob: SHA-1 EXACT`.
Tests at 44bb2f76: Pi `make test-fast JOBS=4` conveyor `1590 passed, 799 skipped`, cloud `808 passed, 202
skipped`, rc=0 (main checkout's submodule contents copied into the worktree's empty submodule dirs; still there,
untracked by git — remove `tools/decomp-permuter/*` and `tools/mips_to_c/*` in the worktree by hand);
rocky `REQUIRE_TOOLCHAIN=1 make test-fast JOBS=16` conveyor `2337 passed, 52 skipped`, cloud `1005 passed,
5 skipped, 27 subtests passed`, rc=0.
