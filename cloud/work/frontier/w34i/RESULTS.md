# w34i — wave 34: integration of PR packets #382–#418 (2026-10-20)

Branch `wave34` from master `89aa61118` (wave 33 + the entity_iterate receipt re-pin). The PRs were drafted
on `58425072b` (before wave 33 was pushed); #380/#381/#402 duplicate wave-33 landings and were not imported.
Each PR's files were checked out from its head unchanged (`ab152638f`), except `cloud/work/CLAIMS.md`
(their diffs are `claims_prN.diff` here). Merges of the PRs themselves are left to the owner.

## Outcome

| PR | Function | Bytes | Installed as | Batch |
|---|---|---:|---|---|
| #382 | func_800E543C = MAT3 Init_MDrive | 700 | NEW `frontier_init_mdrive` | 1 |
| #397 | brake_light_update = ZOID_ProjectWindow | 448 | NEW `frontier_project_window` (replay `--materialize`) | 1 |
| #399 | arb_rate_set = ZOID_InitializeWindow | 312 | NEW `frontier_window_init` (replay `--materialize`) | 1 |
| #400 | func_800A7480 = ZOID_SetFog | 136 | NEW `frontier_set_fog` (replay `--materialize`) | 1 |
| #385 | func_800DE45C settings/race reset | 1,020 | `codex_vsync_a145` superseded in place | 1 |
| #386 | entity_anim_texture = AnimateWingFlame | 636 | `codex_visual_tire_module_20261002` superseded in place | 1 |
| #387 | hud_setup = sound-system init | 1,048 | `menu_save_options` superseded in place | 1 |
| #406 | entity_render_setup = AnimateArrow | 2,048 | NEW `frontier_animate_arrow` | 2 |
| #418 | image A func_803A3DA4 carscr slider | 912 | `src/ovl_a/func_803A3DA4.c` (overlay A single) | 2 |
| #383 | func_800D5A04 race transition | 428 | `frontier_slot18_alloc` superseded in place | 3 |
| #384 | render_viewport_init = PlayGame race tail | 924 | `frontier_car_checkpoints` superseded in place | 3 |
| #412 | func_800D6160 = _StartGameMusic | 304 | NEW `frontier_game_music` | 3 |
| #390 | camera_play_script = car-body surface collision | 3,520 | `func_800AD4C8` superseded in place | 3 |
| #394 | input_deadzone_apply = ShotToWorldCollision | 3,580 | NEW `frontier_shot_world_collision` (replay `--materialize`, before #390) | 3 |

Game 1077 → **1090/1216** (408,612 → 423,716 B, 63.15% → **65.48%**, +15,104 B). Overlay A 81 → **82/193**
(51,060 → 51,972 B). Static and overlay B unchanged. Every function was a separate PR's strict cloud match;
all 14 landed. Batches 1–2 were pushed (`13277db1c`); batch 3 follows.

## How the tangled packets were installed: already-locked members as context

#383, #384, #412 and #394 listed as members functions that are locked elsewhere (11 singles; four groups and
five singles; three groups and three singles; six members of `func_800AD4C8`), and #390/#394 both superseded
`func_800AD4C8`. Reverting all of those into ever-larger unions was not needed: in the installed `group.json`
the members are the superseded group's old members plus the one new function, and every other locked
function moved to `context` (compiled for IPA, not spliced — 79 locked groups already do this). Sources,
flags and `keep` are the packets' own; the installed copies are byte-identical to the packet files. The
whole-program unit is the gate that the context is consistent:

- `src/blob/unit_overrides.json` `extern_data` D_80152818 in `frontier_car_checkpoints/frontier_path_D169C_path.c`
  (#384's byte copy of `frontier_path_D169C/path.c`; uld saw the `Car D_80152818[6]` definition twice).
- `prefer_definition` input_process_controller → `frontier_shot_world_collision/group.c`: #394's materializer
  reorders the unmatched callee's parameters (`Poly *poly, p1, p2, out, vcOut, mat, flag, outIdx, rad2`), and
  input_deadzone_apply matches only against that order (742/895 words differ against func_800AD4C8's copy).
  Its only retail caller is input_deadzone_apply; func_800AD4C8's copy is called only by all-zero stand-ins.

## Tooling

- `blob_group`: a LO16 without a HI16 of its own pairs with its symbol's previous HI16 (IDO reads two
  words of AnimateArrow's own `.data` through one `lui`; refusal was ".data LO16 at .text+0x588 has no HI16").
  Fail-closed as before; new test `test_two_lo16_sharing_one_hi16_are_paired_and_verified`.
- `tests/conftest.py`: a missing-IDO diagnostic re-raised as RuntimeError is a toolchain skip
  (test_mat3_race_tail on the Pi); still a failure under REQUIRE_TOOLCHAIN=1.
- Overlay objects on the Pi were stale (A: 19 bodies in three groups, B: 2); `ovl_splice rebuild --image a/b`
  rebuilt 81/21 bodies byte-identical before #418.

## Gates (logs here)

Each batch: `blob_group check` 0 problems; `blob_splice check`; `lock check` all 635 static locks intact;
`ovl_splice check`; `blob_unit check` all locked bodies equal in one unit (batch 3: 1090/1090, 997 files);
blob stream byte-identical; `ovl_layout build` + `ovl_rom rom` both streams == ROM; **ROM SHA-1 EXACT**,
MAKE=0 TEST=0; `blob_layout derive`, `blob_tu symbols` (provenance comments + SHA256SUMS only), slot-contract
audit `--write` (accepted callers and unaccepted bytes unchanged).

## Research-packet fixes (handoff §5)

- Lock / manifest pins: the imported verifiers bound `blob_matched.lock.json` (two also `SHA256SUMS`);
  they no longer bind them, and receipts were refreshed by a script that refused any change other than the
  dropped keys and the verifier's own hash (rocky diffed fresh bindings against every receipt first).
- Superseded context groups (pre-install copies, byte-identical): `superseded/menu_save_options` (#387),
  `superseded/frontier_slot18_alloc` (#383), `superseded/func_800AD4C8_w34` (#390, #394; `superseded/func_800AD4C8`
  is the older wave-10 copy another packet binds).
- `blob_group.py` hash rebound in the four receipts that pin it (w31i precedent).

## Rename leads (not applied; next rename batch)

func_800E543C = Init_MDrive, brake_light_update = ZOID_ProjectWindow, arb_rate_set = ZOID_InitializeWindow,
func_800A7480 = ZOID_SetFog, entity_anim_texture = AnimateWingFlame, entity_render_setup = AnimateArrow,
render_viewport_init = PlayGame, func_800D6160 = _StartGameMusic, input_deadzone_apply = ShotToWorldCollision,
camera_play_script = car-body surface collision (name WRONG), hud_setup = sound-system init (name WRONG).
