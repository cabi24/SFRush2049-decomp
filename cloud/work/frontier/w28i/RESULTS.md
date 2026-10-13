# w28i — game-side integration (wave 28, worktree, branch `w28i` from wave28 `264bab69`)

## Job 1: pak write cluster (w28b) — installed

| Group | Members | Bytes | Game coverage (`make progress`) |
|---|---|---:|---|
| `pak_write_cluster` | func_800A1BB4 184, func_800A1C6C 360, func_800A1DD4 192, track_data_decompress 652, track_collision_setup 468, MaxPathZeroControls 396, slot_state_lookup 188 | 2,440 | 1010/1216, 341,056 B (52.71%) -> **1017/1216, 343,496 B (53.08%)** |

Static (632/667, 151,176 B, 94.06%), overlay A (81/193, 51,060 B) and overlay B (21/49, 9,020 B) unchanged.

### Confirmation
- Pi, `blob_unit --tag w28i score func_800A1BB4 func_800A1C6C func_800A1DD4 track_data_decompress
  track_collision_setup MaxPathZeroControls slot_state_lookup --with cloud/work/frontier/w28b/groups/pak_write_cluster/group.c
  --internal func_800A1BB4 --internal func_800A1C6C --internal func_800A1DD4 --internal track_data_decompress
  --internal pfs_queue_init --internal pfs_lock --internal pfs_unlock --neighbours`:
  ```
  EQUAL func_800A1BB4: 46 words (internal, c_group.c)
  EQUAL func_800A1C6C: 90 words (internal, c_group.c)
  EQUAL func_800A1DD4: 48 words (internal, c_group.c)
  EQUAL track_data_decompress: 163 words (internal, c_group.c)
  EQUAL track_collision_setup: 117 words (kept, c_group.c)
  EQUAL MaxPathZeroControls: 99 words (kept, c_group.c)
  EQUAL slot_state_lookup: 47 words (kept, c_group.c)
  locked bodies that differ in this unit: 0
  blob_unit score: 7/7 equal
  ```
- watchman2 scratch `w28i` (IDO 5.3, `IDO_DIR` = the 796ae99a toolkit), `score.py group ... --claims`: all seven
  members `MATCH`, rc=0. Scratch deleted afterwards.

### Integration
- `src/blob/unit_overrides.json` `force_internal`: func_800A1BB4, func_800A1C6C, func_800A1DD4, track_data_decompress.
- `PYTHONPATH=. python3 cloud/work/frontier/tools/install_group.py cloud/work/frontier/w28b/groups/pak_write_cluster pak_write_cluster`
  -> `spliced pak_write_cluster: 7 members`, `image coverage: 1017/1216 functions, 343496/647072 bytes (53.08%)`.
  No group superseded, no single reverted.
- `blob_group check`: `group lock: 0 problems`; `blob_splice check`: `blob lock: 1017 entries, 0 problems`.
- `blob_unit --tag w28i check`: `1017 locked bodies, 1017 equal to the image in one 902-file unit, 0 differ`.
- `blob_rom rom`: `ROM build with the source-built blob: SHA-1 EXACT`, `build/us/rush2049.us.z64: OK`
  (3f99351d7bb61656614bdb2aa1a90cfe55d1922c).
- `cloud/work/frontier/provisional.json`: MaxPathZeroControls entry removed (it is locked now with its real body).
- The splice changed only `asm/us/blob/SHA256SUMS` and `blob_8009dd18.s` (provenance comments); `symbols.json` and
  `blob.ld` are unchanged, so the nearest_point_donor / target_orientation receipts need no re-pin, and
  `dot_slot_contract_audit_20261006/audit.py --write` produced no diff (no slot caller among the members).
- CLAIMS.md: cluster row moved to "landed (wave 28)"; resource-loader part kept as its own open row; new w28b drafts row.
- The w28b packet (groups/pak_write_cluster, pak_write_cluster/cluster.c, RESULTS.md) is committed on this branch as
  provenance; w28b's other drafts (AdjustSpeed, func_800CD104, func_800C813C) are still uncommitted in the main checkout.

### Worktree notes
- The worktree had no `build/`: copied `build/{blob,ovl,us,game_code.bin,blob_layout.json,blob_unit.json,...}` and the
  blob_unit preprocess cache from the main checkout, plus a real copy of `baserom.us.z64` (not a symlink).
- The worktree has empty submodule dirs. Pi `make test-fast JOBS=4` first run: tests/cloud `790 passed, 200 skipped`;
  tests/conveyor 41 failed + 2 collection errors, all `src.scorer`/m2c imports (decomp-permuter, mips_to_c absent).
  Rerun of tests/conveyor with the MAIN checkout's submodules symlinked in temporarily (removed afterwards):
  `1589 passed, 802 skipped`, rc=0.
- rocky, clone `~/rush2049/scratch/w28i` (copy of ciclone, branch fetched from a bundle, `origin/master` =
  e3b1f926 set), `REQUIRE_TOOLCHAIN=1 make test-fast JOBS=16`: conveyor `2339 passed, 52 skipped`; cloud
  `986 passed, 4 skipped, 8 warnings, 27 subtests passed`; rc=0.
