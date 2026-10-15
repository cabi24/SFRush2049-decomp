# w33i — game-side integration (wave 33, worktree, branch `w33i`)

Worktree `.claude/worktrees/w33i`, branch `w33i` from origin/master `c22612328`. w33p's lane commit `adfd2ceaf`
cherry-picked (adds only `cloud/work/frontier/w33p/`: RESULTS.md, `entity_iterate/best.c`, logs, the packet). The
PR #379 files (`gc_informed_n64_unscored_20261015`) are NOT brought over (they arrive by merging the PR).
build/ inputs copied from the main checkout (build/{blob,ovl,us,m2c_asm}, build/*.json, game_code.bin/.sym,
blob_unit/cache; baserom symlinked); `blob_splice rebuild-stale --builder-dir ~/rush2049/scratch/frontier/w33i/bs
--group-builder-dir .../bg` rebuilt 56 singles + 47 groups first (`rebuild_stale.log`; dry run afterwards: 0 stale).
Builder scratch watchman2:~/rush2049/scratch/frontier/w33i (copy of base + rsync of src/blob, src/rom, include,
tools/cloud, tools/conveyor, asm/us/blob, asm/us/boot_tail, blob_matched.lock.json; `IDO_DIR` = toolkit 796ae99a
IDO 5.3; unit remote dir `rush2049/scratch/frontier/w33i/unit`) — deleted at the end. Permission denials: none.

| Step | Change | New bytes | Game after (`blob_rom coverage`) |
|---|---|---:|---|
| (start) | — | — | 1066/1216, 396,504 B (61.28%) |
| 1 | NEW group `frontier_route_cross` (w33p, from Astra's PR #379 `routes/unsigned_above.c`): audio_mixer_main (732 B, kept) + audio_priority_find (432 B, internal) | 1,164 | **1068/1216, 397,668 B (61.46%)** |

Static (632/667, 151,176 B, 94.06%), overlay A (81/193, 51,060 B) and overlay B (21/49, 9,020 B) unchanged.
`coverage_before.log`, `coverage_after.log`.

## Packet and renames
- `groups/frontier_route_cross/` = w33p's packet with the PR's private helper names replaced:
  `gc_lane_FindPathDistances` -> **`FindPathDistances`** (GC FindPathDistances, static void, the distance pass);
  `gc_lane_GetNextCheckpoint` -> **`route_next_checkpoint_u`** (GC's unsigned `GetNextCheckpoint(unsigned int)`;
  a distinct name because the locked func_800B930C is N64's GetNextCheckpoint with a different `index+1 == count`
  body). Both are static and inlined (`umerge inlined into it`), so no symbol is created. No collision:
  `grep -rn` over src, include, asm/us (symbols.json), build/*.json (inventory, frontier, protos), tools/recomp: no hits.
- Header: the stale "PRIVATE, UNSCORED SOURCE EXPERIMENT" line replaced by the install provenance and the rename note;
  the w33p disclosure (two unused `s32` locals unused1/unused2 = exact frame residual: retail 80, 72 without, 2 slots,
  8 B; coordinator-accepted under the owner's unused-locals rule) kept verbatim. `prerename/` keeps the copy as
  received. group.json: note/provenance updated; members, keep, claims, flags unchanged.

## Scores
Builder, `score.py group` (`builder_prerename.log`, `builder_renamed.log`), both copies, with and without `--claims`:
```
audio_priority_find:
  MATCH
audio_mixer_main:
  MATCH
rc=0          (x4: pre-rename --claims / plain, renamed --claims / plain)
```
Unit before install (`unit_pre.log`, renamed copy):
```
default derivation:  FAIL audio_mixer_main: 107 of 183 words differ | FAIL audio_priority_find: 63 of 108 words differ
--internal audio_priority_find --keep audio_mixer_main:
  EQUAL audio_mixer_main: 183 words (kept, c_group.c)
  EQUAL audio_priority_find: 108 words (internal, c_group.c)
  locked bodies that differ in this unit: 0 | blob_unit score: 2/2 equal
```
unit_overrides: **none added.** Once the group is locked the manifest derives audio_priority_find as internal (a
locked group defines it without keeping it): `build/blob_unit.json` internal contains it, `unused_overrides []`, and
`blob_unit check` is 1068/1068 equal with no override (w33p's `force_internal` was needed only while it was unlocked).

## Install and gates
```
python3 - (remove the "audio_priority_find" row from cloud/work/frontier/provisional.json; 1-space indent kept, diff = 4 removed lines)
PYTHONPATH=. python3 cloud/work/frontier/tools/install_group.py cloud/work/frontier/w33i/groups/frontier_route_cross frontier_route_cross
python3 -m tools.conveyor.pipeline.blob_group check
python3 -m tools.conveyor.pipeline.blob_splice check
python3 -m tools.conveyor.pipeline.blob_unit check
python3 -m tools.conveyor.pipeline.lock check
python3 -m tools.conveyor.pipeline.blob_layout derive
python3 -m tools.conveyor.pipeline.blob_tu symbols
python3 -m tools.conveyor.pipeline.blob_rom rom
python3 -m tools.conveyor.pipeline.blob_rom produce
python3 -m tools.conveyor.pipeline.blob_rom coverage
```
- Install (`install.log`): `spliced frontier_route_cross: 2 members, new: ['audio_mixer_main', 'audio_priority_find']`,
  `image coverage: 1068/1216 functions, 397668/647072 bytes (61.46%)`. No group superseded, no single reverted (neither
  function was locked; the w2b evidence group was never installed).
- Gates (`gates.log`): `group lock: 0 problems`; `blob lock: 1068 entries, 0 problems`; `blob_unit: 1068 locked bodies,
  1068 equal to the image in one 935-file unit, 0 differ [1029 plain; stub tail 35, own bss 5; 462 own .rodata/.data
  references in 141 bodies checked against image bytes] (954 kept, 331 internal ...)`; `all 635 locked functions intact`
  (static lock); `unused_overrides []`.
- Image (`produce.log`): `blob_rom: image 647072 B (1068 spliced) -> blob 326180 B sha256 9e632d21b7d0d293…`,
  `byte-identical to the ROM's stream: True`.
- ROM (`rom.log`): `blob objects: 0 stale singles, 0 stale groups`, `ROM build with the source-built blob: SHA-1 EXACT`,
  `build/us/rush2049.us.z64: OK`, `ROM matches!`, `MAKE=0 TEST=0`.
- Layout/symbols (`layout_symbols.log`): `functions: 1216 561860 bytes (86.8%)`; symbols.json and blob.ld UNCHANGED (both
  names were already in the symbol table); the asm diff is the two provenance comments in blob_800b7360.s + SHA256SUMS.
  The lock diff is exactly the two new entries (group frontier_route_cross). No receipt re-pin. Slot-contract audit
  (`dot_slot_contract_audit_20261006/audit.py --write`): no diff. No data-symbol changes.

## Tests
- Pi, worktree, `make test-fast JOBS=4` (`test_fast.log`; the empty tools/mips_to_c and tools/decomp-permuter submodule
  dirs replaced by symlinks to the main checkout's for the run and restored afterwards, as w32i did): conveyor
  `1592 passed, 798 skipped`; cloud `854 passed, 207 skipped`; rc=0. `tests/conveyor/test_dot_route_tracker.py`
  (pins the two extents and the w2b provisional claim layout) passes.

## Bookkeeping
- `cloud/work/frontier/provisional.json`: audio_priority_find row removed.
- `cloud/work/CLAIMS.md`: no existing row named either function; a wave-33 landed row added (packet, helper names,
  device, GC names SyncCheckpointsToPaths / FindAltPathIntersect as rename leads, and the w17h knock-on:
  audio_effect_apply (GC ReadPathData) was blocked by audio_mixer_main and func_800B9740 — the first is cleared,
  func_800B9740 (83/102, w14r) remains).
- Handoff: not edited — it has no rename-pending list (the wave-32 "Names pending" list lives in the lane brief),
  so the GC names are recorded in CLAIMS and here only.
- Builder scratch watchman2:~/rush2049/scratch/frontier/w33i (incl. unit/) deleted. Note: `install_group.py` and
  `blob_rom rom` use their fixed builder dirs (/tmp/blobgroup, ~/rush2049/repo) as in every integration lane.
