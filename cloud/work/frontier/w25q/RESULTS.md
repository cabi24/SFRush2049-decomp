# w25q: integration of Astra's PRs #353–#358 (2026-10-12)

Branch `w25q` from `wave25` `eb5bc4b2`. Not pushed. #355–#358 opened after the lane brief and are included.
#322/#325/#339/#342 have no commit after their last comment (all 15:57Z, Astra's own; last commits earlier): untouched.

## Outcome

| PR | What | Verdict | Bytes | Gate (`splice.log`) |
|---|---|---|---:|---|
| #353 | B `func_80391650` status-scene helper + 4 B owned literal, new group `dot_runtime_b_status_scene_20261012` | LAND | 532 | `image B: group spliced ['func_80391650']; image gate byte-identical` |
| #354 | A `func_8039D300` car-stat product, superseding `dot_cleanup_colortrace_20261011` (5 locked + 1) | LAND | 404 | `image A: group spliced [... 6 ...]; image gate byte-identical` |
| #355 | A `func_803AD0D4` track-stats renderer + 56 B switch table, superseding `runtime_a_stats_menu_20261006` (9 locked + 1) | LAND | 1,184 | `image A: group spliced [... 10 ...]; image gate byte-identical` |
| #356 | B `func_8038D798` radius effect (accepted `volatile s16 D_801543CA` view), new group `dot_runtime_b_radius_effect_20261012` | LAND | 736 | `image B: group spliced ['func_8038D798']; image gate byte-identical` |
| #357 | A `func_803A9088` profile deletion renderer, 15/917 (stack-offset homes only) | IMPORT-ONLY | 0 | — |
| #358 | B `func_80390B10`/`func_80390D38` (552 B each) + stubs `80390B00`/`80390B08` (8 B each), new group `dot_runtime_b_resource_callbacks_20261012`; parent 80390F60 5/247 kept as context | LAND | 1,120 | `image B: group spliced ['func_80390B00', 'func_80390B08', 'func_80390B10', 'func_80390D38']; image gate byte-identical` |

Overlay A 75/193 (43,112 B, 22.21%) → **77/193 (44,700 B, 23.03%)**; overlay B 14/49 (5,964 B, 13.59%) →
**20/49 (8,352 B, 19.03%)**. Total +3,976 B (A +1,588, B +2,388). Static 617/667 (139,624 B) and game 997/1216
(329,104 B) unchanged.

Gates after all splices (`ovl_rom.log`): `ovl_layout build` (A 77 locked C bodies, B 20, both "byte-identical"),
`ovl_splice check` ("ovl lock: all 97 runtime-image entries intact"), `ovl_rom rom`: both streams `== ROM`, "ROM build
(game blob + both runtime-image streams composed): SHA-1 EXACT", "ROM matches!", `MAKE=0 TEST=0`.

## Dependency map

| Locked group on wave25 | Superseded by | New members |
|---|---|---|
| `dot_cleanup_colortrace_20261011` (5: D05C, D494, ED20, E200, E3BC) | #354 (6) | D300 (was context) |
| `runtime_a_stats_menu_20261006` (9) | #355 (10) | AD0D4 (was context) |
| `runtime_a_profile_menu_20261006` (3) | #357 would re-carry the same 3; not installed (A9088 NONMATCH) | — |
| (new) | #353 B, #356 B, #358 B | |

Both superseding packets claim every member (no `--claims all`), carry every locked member, and the image gate kept the
inherited bodies byte-identical. #353's `group.json` names `runtime_b_hud_scenes_20261006`, but `ovl_splice group`
installs under the packet directory name (`dot_runtime_b_status_scene_20261012`), as with #334. No B target was locked
before; no migration.

## Packet verifiers (trap check: before and after installation)

All six `verify.py` ran on rocky with the toolchain before install (`--score`: members MATCH, context bodies listed as
NONMATCH) and after install (`--deep`, #356 has no `--deep`: default mode + `python3 -O`): all exit 0, e.g.
"PASS: D300 404 new bytes; five exact old members 2264 bytes preserved", "PASS: 1184 new exact candidate bytes; 7512
prior bytes preserved", "PASS: 184/184 words, full GNU body/data, accepted count view and both causal controls",
"PASS: 1104 new helper bytes + 16 deleted-stub bytes; parent NONMATCH". No verifier compares against the pre-install
production source this time; no receipt was re-recorded.

## Notes

- #356 relies on `extern volatile s16 D_801543CA`, the declaration five accepted production sources already use
  (EC914, 108DA8, 108F40, 109A60, world_physics_tick); its verifier pins those sources against the lock.
- #358's two 8-byte stubs are separate scanned targets (`asm/us/ovl_b/extents.json`) and count as functions (B +4).
- #357: residual is A9088 at 15/917 (all lw/lh/sw stack offsets, frame 272 matches); with A9EEC 281/315 still open,
  recorded on Astra's CLAIMS row.

## Tests

- rocky, `REQUIRE_TOOLCHAIN=1` on the six new packet tests (before and after install): 6 passed.
- rocky `REQUIRE_TOOLCHAIN=1 make test-fast JOBS=16` (at `af5f899b`): conveyor 2324 passed, 52 skipped; cloud 948
  passed, 1 skipped; exit 0.
- Pi `make test-fast JOBS=4`: conveyor 1584 passed, 792 skipped; cloud 770 passed, 179 skipped; exit 0.

## Setup notes

As w25p: `cp baserom.us.z64`, copy `build/game_code.bin` + `build/blob/obj`, submodules `--reference` the main checkout,
`ovl_splice rebuild` A/B, `blob_layout derive`, `blob_rom produce` (stream byte-identical; its rewrites of
`asm/us/blob/{SHA256SUMS,symbols.json}` and `src/blob/blob.ld` reverted). `OVL_BUILDER_DIR=/tmp/ovlsplice-w25q`. Rocky
clone `~/rush2049/scratch/w25q` (copied from w25p, `receive.denyCurrentBranch=updateInstead`).
