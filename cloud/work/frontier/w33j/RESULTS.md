# w33j — game-side integration of w33c (wave 33, worktree, branch `w33j`)

Worktree `.claude/worktrees/w33j`, branch `w33j` from origin/master `58425072b` (= wave33). w33c's uncommitted lane
dir copied from the main checkout and committed first (`w33c: lane results`: RESULTS.md, blast/best_effects.c,
entity_iterate/cand.c, groups/frontier_read_path_data/{group.json,physics_response.c,audio_effect_apply.c}; the
`v/` and per-function scratch dirs are git-ignored). `cloud/matches/entity_iterate.c` and `cloud/matches/func_800B9740.c`
copied from the main checkout. build/ inputs copied from the main checkout (build/{blob,ovl,us,m2c_asm},
build/*.json, game_code.bin/.sym, blob_unit/cache; baserom symlinked); `blob_splice rebuild-stale --builder-dir
~/rush2049/scratch/frontier/w33j/bs --group-builder-dir .../bg` rebuilt 56 singles + 48 groups first
(`rebuild_stale.log`; dry run afterwards: 0 stale). The copied `build/blob_unit.json` was stale (Oct 12):
`blob_unit manifest` regenerated it after the install. Builder scratch watchman2:~/rush2049/scratch/frontier/w33j
(copy of base + rsync of src/blob, src/rom, include, tools/cloud, tools/conveyor, asm/us/blob, asm/us/boot_tail,
blob_matched.lock.json; `IDO_DIR` = toolkit 796ae99a IDO 5.3; unit remote dir `rush2049/scratch/frontier/w33j/unit`)
— deleted at the end.

| Step | Change | New bytes | Game after (`blob_rom coverage`) |
|---|---|---:|---|
| (start) | — | — | 1068/1216, 397,668 B (61.46%) |
| 1 | singles func_800B9740 (408 B) + entity_iterate (224 B) | 632 | 1070/1216 (image gate True) |
| 2 | NEW group `frontier_read_path_data`: audio_effect_apply (696 B, kept) + physics_response (920 B, internal) | 1,616 | **1072/1216, 399,916 B (61.80%)** |

Total +2,248 B, as w33c predicted. Static (632/667, 151,176 B, 94.06%), overlay A (81/193, 51,060 B) and overlay B
(21/49, 9,020 B) unchanged. `coverage_before.log`, `coverage_after.log`.

## Scores before install
Builder (`builder.log`), from the worktree's sources: `score.py fn cand/<fn>.c <fn> --flags "-g0 -O3 -mips2 -G 0 -non_shared"`
→ `entity_iterate: MATCH`, `func_800B9740: MATCH` (rc=0; `--claims` is a `group`-only option, `fn` rejects it with
rc=2 — the singles have no claims to check). `score.py group cand/frontier_read_path_data` with and without `--claims`:
```
physics_response:
  MATCH
    own .rodata verified at 0x80123DF4..0x80123DF8
audio_effect_apply:
  MATCH
Context (informational; excluded from exit status):
func_800B930C:
  MATCH
rc=0          (x2)
```
Unit (`unit_pre.log`), all four candidates together:
```
default derivation:
  EQUAL entity_iterate: 56 words (kept, c_entity_iterate.c); stub tail: 2 words of deleted-procedure stubs follow (NOT as in the image)
  EQUAL func_800B9740: 102 words (kept, c_func_800B9740.c)
  FAIL audio_effect_apply: 158 of 174 words differ; compiled body is 162 words, target 174
  FAIL physics_response: 229 of 230 words differ; compiled body is 247 words, target 230   (kept by default: expected)
  locked bodies that differ in this unit: 0
--internal physics_response:
  EQUAL entity_iterate / func_800B9740 / audio_effect_apply: 174 words (kept) / physics_response: 230 words (internal)
  locked bodies that differ in this unit: 0 | blob_unit score: 4/4 equal
```
entity_iterate's "stub tail ... NOT as in the image" note is the unit-ordering artefact (w13f/w16d): the image gate
after splicing entity_iterate alone was byte-identical (`produce_singles.log`: `image 647072 B (1070 spliced) ...
byte-identical to the ROM's stream: True`), and `blob_unit check` after the install is 1072/1072 equal.

## Install and gates
```
python3 - (remove the "physics_response" row from cloud/work/frontier/provisional.json; indent 1 kept, diff = 4 removed lines)
PYTHONPATH=. python3 cloud/work/frontier/tools/splice_singles.py func_800B9740 entity_iterate
python3 -m tools.conveyor.pipeline.blob_rom produce            # image gate after the singles
PYTHONPATH=. python3 cloud/work/frontier/tools/install_group.py cloud/work/frontier/w33c/groups/frontier_read_path_data frontier_read_path_data
python3 -m tools.conveyor.pipeline.blob_group check
python3 -m tools.conveyor.pipeline.blob_splice check
python3 -m tools.conveyor.pipeline.lock check
python3 -m tools.conveyor.pipeline.blob_unit --tag w33j --remote-dir rush2049/scratch/frontier/w33j/unit check
python3 -m tools.conveyor.pipeline.blob_unit manifest
python3 -m tools.conveyor.pipeline.blob_layout derive
python3 -m tools.conveyor.pipeline.blob_tu symbols
python3 -m tools.conveyor.pipeline.blob_rom rom
python3 -m tools.conveyor.pipeline.blob_rom produce
python3 -m tools.conveyor.pipeline.blob_rom coverage
python3 cloud/work/frontier/dot_slot_contract_audit_20261006/audit.py --write
```
- Singles (`install_singles.log`): `refused {}`, `image_ok True locked 1070`. func_800B9740 spliced before the group.
- Group (`install_group.log`): `spliced frontier_read_path_data: 2 members, new: ['audio_effect_apply', 'physics_response']`,
  `image coverage: 1072/1216 functions, 399916/647072 bytes (61.80%)`. No group superseded, no single reverted.
- Gates (`gates.log`): `group lock: 0 problems`; `blob lock: 1072 entries, 0 problems`; `all 635 locked functions intact`
  (static lock); `blob_unit: 1072 locked bodies, 1072 equal to the image in one 939-file unit, 0 differ [1033 plain;
  stub tail 35, own bss 5; 463 own .rodata/.data references in 142 bodies checked against image bytes] (957 kept, 332 internal)`.
- unit_overrides: **none added.** With the group locked, the regenerated manifest derives physics_response internal
  (`internal` contains it, `keep` does not, audio_effect_apply on keep) and `unused_overrides []`, as w33i found for
  audio_priority_find. (w33c's note asking for `force_internal += physics_response` applied only while it was unlocked.)
- Lock diff vs the start: exactly 4 added entries (entity_iterate, func_800B9740, audio_effect_apply, physics_response);
  0 removed, 0 changed — no locked body changed.
- Image (`produce.log`): `blob_rom: image 647072 B (1072 spliced) -> blob 326180 B sha256 9e632d21b7d0d293…`,
  `byte-identical to the ROM's stream: True`.
- ROM (`rom.log`): `blob objects: 0 stale singles, 0 stale groups`, **`ROM build with the source-built blob: SHA-1 EXACT`**,
  `build/us/rush2049.us.z64: OK`, `ROM matches!`, `MAKE=0 TEST=0`.
- Layout/symbols (`layout_symbols.log`): `functions: 1216 561860 bytes (86.8%)`; symbols.json and blob.ld UNCHANGED (all four
  names already in the symbol table); the asm diff is four `/* compiled from src/blob/... */` provenance comments
  (blob_800b7360.s: physics_response, func_800B9740, audio_effect_apply; blob_800c46d0.s: entity_iterate) + SHA256SUMS.
  Slot-contract audit `--write`: no diff. No data-symbol changes.

## Tests
Pi, worktree, `make test-fast JOBS=4` (`test_fast.log`; tools/mips_to_c and tools/decomp-permuter symlinked to the main
checkout's for the run, empty dirs restored afterwards): conveyor `1 failed, 1591 passed, 800 skipped`; cloud
`854 passed, 207 skipped`; rc=2.

**The failure is a receipt that pins entity_iterate as unlocked** —
`tests/conveyor/test_entity_vector_boundaries.py::test_native_metadata_and_call_contract`:
`cloud/work/frontier/dot_entity_vector_boundaries_20261005/verify.py:214  assert FN not in locks and len(targets[FN])*4==224`.
This is the same trap commit 68d3c6a67 fixed for masked_random (`if FN in locks: assert locks[FN]['source']=='src/blob/'+FN+'.c'`).
I made that edit, but the next step (updating the receipt's frozen `native_metadata.locked` and the verify.py sha256 in
`verification.json` source_bindings) was **denied by the permission classifier as test removal**; per the lane rules I
did not retry in another form and reverted the verify.py edit. **Left for the coordinator:** re-pin the receipt
(verify.py: accept FN locked from its own source only; verification.json: `native_metadata.locked` → true and the new
verify.py hash on line 1069; nothing else pins that hash — `grep -r d99d6522c82423ee` finds only verification.json).

## Bookkeeping
- `cloud/work/frontier/provisional.json`: physics_response row removed.
- `cloud/work/CLAIMS.md`: "wave 33 landed (w33c/w33j)" row added (four functions, devices, GC rename leads entity_iterate =
  CameraCollision, func_800B9740 = CalcResPathBounds, audio_effect_apply = ReadPathData, physics_response =
  CalcCheckpointTimes, physics_friction_apply = SyncAltPaths; both w17h audio_effect_apply blockers cleared; the open
  receipt re-pin); the w33i row's "func_800B9740 remains" knock-on updated. No other row listed these as open.
- physics_response.c keeps w17h's disclosure of the two `do {} while (0);` devices (file byte-identical to w17h's).
- Builder scratch watchman2:~/rush2049/scratch/frontier/w33j (incl. unit/) deleted. `install_group.py`/`splice_singles.py`
  and `blob_rom rom` use their fixed builder dirs (/tmp/blobgroup, /tmp/blobsplice, ~/rush2049/repo) as in every
  integration lane.
- Permission denials: one (the receipt re-pin above).
