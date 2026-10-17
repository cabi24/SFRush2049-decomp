# PlayGame / render_viewport_init: strict N64 group match

Deliverable: `groups/frontier_car_checkpoints/` in this packet. New game-code
match: **render_viewport_init, 0x800FA9B4, 924 bytes / 231 words**. The historical
name is misleading: the function is the per-frame race update related to MAT3
`game.c`'s `PlayGame`, not viewport setup or the smaller `DoDrones` routine.

Base: `58425072b49e95f0fbe5c4c239988fb4885f7c5c`. Stock IDO 5.3,
`-g0 -O3 -mips2 -G 0 -non_shared`, with the scorer's standard `as1 -r4300_mul`.
All **25 members and claims** are strict MATCH, including all 24 inherited
locked functions. No unresolved or unverified relocation, own-data error, or
nonzero excess word remains. The native and candidate frames are both 192 B.
The exact command output, without `--claims`, is in `score.txt`.

```sh
python3 tools/cloud/score.py group cloud/work/frontier/dot_mat3_race_tail_20261017/groups/frontier_car_checkpoints
python3 cloud/work/frontier/dot_mat3_race_tail_20261017/verify.py
python3 -m pytest -q tests/cloud/test_mat3_race_tail.py
```

The full-group replay and its seven negative controls passed against current
master inputs. The single pytest entry passed in 2.34 seconds in a fresh replay
root using current-master tools, targets, and conftest. The isolated research
checkout predates the `gCamera` rename, so its stale symbol map is intentionally
not used as the current-master verification basis.

## Real context and installation boundary

This packet preserves the existing `frontier_car_checkpoints` group name and
all its old members and keep entries. It also includes the full genuine locked
`render_large_objects`, `frontier_path_D169C`, `frontier_camera_scene_manager`,
and `frontier_camera_target_track` closures, plus `camera_collision_check` and
`func_800CB718`. All defined already-locked functions are members and claims,
not silently omitted context. The coordinator should merge/deduplicate these
closures on installation and retain the corresponding existing group contracts.
No private-ABI stand-in, artificial helper, target-byte patch, unused local, or
stack-padding array was used. The structural byte arrays describe measured
native field offsets, not invented frame filler.

Only FA9B4 adds coverage. Whole-program installation, nonregression, and ROM
identity remain the independent coordinator's gates. This packet does not edit
production sources or lock files and does not claim those integration checks.

## Source-supported changes and disclosed shaping

The older w27a draft replayed at 176/231 differing words plus two extra words,
with a 112-byte frame. Genuine source context was required, rather than treating
flattened helper bodies as the original source:

- Restoring actual kept sound wrappers `func_800B61A8` (PlayGameSound) and
  `camera_collision_check` (EmitGameSound) supplies 64 missing frame bytes and
  the native inlined early-return branch/argument shape. Both remain exact.
- Restoring the real matched `func_800CB718(s16)` call supplies the last 16
  frame bytes. Its current source takes the address of the indexed smoke flag
  and clears bit 0x20, exactly the operation required at the donor's
  InitSmokeBlast site. Its source is copied unchanged, not invented as padding.
- The old Cam152 definition actually totaled 156 bytes. The trailing field is
  now 8 bytes, making the observed native stride 152 and saved-position offset
  132. `cpak_read` now uses its actual signed-short argument prototype, rather
  than the draft's signed-byte prototype; `race_setup_1` is correctly void.
- The model position is a float[3] at 0x22c. EmitGameSound's direction is a
  `const float *`, consistent with use and the donor signature; its own locked
  text is still exact. Camera view/save-view stores precede the position copy.
- The accepted MAT3 PlayGame donor motivated two explicit volatile restart-byte
  reads. These retain the two native loads; they are reconstruction choices,
  not proof that original N64 C used `volatile`. A named restart local colors
  an extra web on N64 and is omitted here. Either individual volatile read can
  be removed without changing the strict score, but removing both fails.
- A used `Car952 *car` assigned before loading the model slot restores the
  native order of the two loop-address induction values, closing six remaining
  scheduling words. Function-scope and block-scope pointer forms both matched.
- The two `1` float spellings are intentionally distinct: StartBlast retains
  `1.f`, while EmitGameSound uses `1.0f`. Normalizing them merges an IDO constant
  web and fails. Native sound ID 45 is retained rather than MAT3's ID 9.

The MAT3 donor was the accepted 684-byte PlayGame reconstruction at 0x80015344,
not the stale canonical TODO. Its full compiled source SHA256 was
`b35e5ade1142a55ff758e76cf09710ce347baca0481a06721d549932bdc3cf27`;
its native object SHA256 was
`91f56cd5866ef58d6564ebbc7ae43f2efd2e867735f629fc6a4030d93208673c`.
N64 offsets, symbols, native sound IDs, and IDO behavior were verified separately;
PowerPC compiler assumptions were not transferred.

## Trace and controls

The observation-only uopt trace of the six-word near-match was byte-identical
to its complete stock object (SHA256
`9e944103c3d5fab2f58253a2da3a2b1f0ac749e2f7d6a6fd7428546928eec379`).
No register forcing was used. The remaining six words only reversed the births
and increments of the player/model loop-address values. The meaningful car
pointer source choice closed them without changing the frame or body length.

Fresh controlled mutations are replayed by verify.py. Every control leaves all
24 inherited locked functions exact and rejects the new function:

| Control | New-function strict result |
|---|---|
| Remove used car pointer, directly index | 6/231 differing words |
| Flatten the actual smoke-reset call | 11/231 |
| Remove both volatile restart reads | 119/231 |
| Restore incorrect camera stride 156 | 2/231 |
| Restore incorrect signed-byte slot argument | 1/231 |
| Normalize StartBlast `1.f` to `1.0f` | 231/231 plus 2 extra words |
| Substitute GC sound ID 9 | 2/231 |

receipt.json pins the delivered source/group inputs, each of the 25 target-word
rows, and verify.py. It does not pin tools, locks, build files, or assembly text.
No ROM data, raw assembly, compiler objects, credentials, or scratch traces are
included.
