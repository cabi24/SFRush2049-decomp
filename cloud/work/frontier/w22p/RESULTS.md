# w22p: integration of Astra's PRs #323–#330 (2026-10-12)

Branch `w22p` from `wave22` = origin/master `3bc25c0c`. Not pushed.

## Outcome

| PR | What | Landed | Bytes | Gate |
|---|---|---|---:|---|
| #323 | CLAIMS reservations (Phase 2 recomp, overlay lanes) | rows reconciled by hand (PR's CLAIMS not taken) | 0 | — |
| #324 | A `func_8038F648` input-sequence gate (single) | yes | 236 | `spliced 1, refused 0, locked 50; image gate byte-identical` |
| #325 | recomp Phase 2: RDRAM accessors + o32 wrapper generator | accessors + fixtures only | 0 | `tests/recomp`: 72 passed, 2 skipped (Pi) |
| #326 | A `func_8039C140` in superseding `dot_runtime_a_text_helpers_20261006` (10 members) | yes, after migration | 1,984 | `group spliced [... 10 ...]; image gate byte-identical` |
| #327 | B `func_803908D0` initializer (single) | yes | 560 | `spliced 1, refused 0, locked 10; image gate byte-identical` |
| #328 | A `func_8039B560` literal webs | yes, merged into the #326 group (11 members) | 936 | `group spliced [... 11 ...]; image gate byte-identical` |
| #329 | A `func_8038C93C` object initializer (group, one claim) | yes, group `dot_runtime_a_c93c_closure_20261012` | 936 | `group spliced ['func_8038C93C']; image gate byte-identical` |
| #330 | A `func_803A6B50` research (3/241 words, frame 160 vs 144) | packet + test only, no coverage | 0 | — |

Overlay A 49/193 (19,272 B) → **53/193 (23,364 B, 12.04%)**; overlay B 9/49 (2,804 B) → **10/49 (3,364 B, 7.66%)**.
Total +4,652 B (A +4,092, B +560). Static and game code unchanged (586/668, 982/1216).

Gates after all splices: `ovl_layout build` (both images byte-identical), `ovl_splice check` ("all 63 runtime-image
entries intact"), `ovl_rom rom`: both streams `== ROM`, "ROM build ... SHA-1 EXACT", "ROM matches!", `MAKE=0 TEST=0`
(`ovl_rom.log`).

## Migration (#326, #328)

#326's packet is a 10-member group under the locked name `dot_runtime_a_text_helpers_20261006` (4 members). Five of
its other members were locked in four single-member/two-member groups, so the installer refused them. Reverted with
`ovl_splice revert-group`: `dot_runtime_a_config_caller_group_20261006` (B120), `..._config_parent_group_20261006`
(A448, B214), `..._scene_init_group_20261006` (BE48), `..._cleanup_group_20261006` (B00C); then installed the #326
group under the same name. Every reverted member is a member of the new group (coverage 46 → 52, no loss).

#328's group (A448/B120/B214/B560/BE48 with C140 as non-matching context) conflicts with #326 (C140 matched, B560
absent). Merged instead: #326's `group.c` with #328's B560 block appended verbatim (its two duplicate typedefs,
`MenuObjectSlot` and `Color`, dropped; identical definitions already in the file), B560 added to members and claims.
All 11 members MATCH in the installer gate including B560's owned `.rodata` (owndata against the retail image) and the
whole-image gate. Packet: `merge_b560/dot_runtime_a_text_helpers_20261006/`. Bare `score.py group` on rocky: all 11
MATCH, exit 1 only for B560's 16 section-relative `.rodata` relocations "no retail data available" (no standalone
image-A data artifact in a clone; same as #328 reports).

## #325 (recomp)

Imported: `tools/recomp/rdram.h`, `tools/recomp/game_rdram.h`, `tests/recomp/test_rdram.py`,
`tests/recomp/fixtures/rdram_{accessors,runtime_contract}.c`, and a README section. w22r's design (plan §10) compiles
replacements with IDO into a fifth rdram section; its `native` mode has generated o32 wrappers but no accessor layer,
which is what these headers add (optional; nothing uses them yet).
Not imported: `gen_wrapper.py`, `o32_abi.h`, `tests/recomp/test_wrappers.py`, `fixtures/wrapper_runtime/recomp.h`
(second o32 wrapper generator duplicating `replace.py`'s `o32_layout`/`render_wrapper`; its guest-pointer mode should
become an option of `replace.py` native mode), `.github/workflows/recomp-fixtures.yml` (GitHub CI is the owner's call),
and the PR's README "o32 replacement wrappers" section.

## Tests

- Pi, new packet tests without IDO: 6 skipped cleanly. `tests/recomp`: 72 passed, 2 skipped.
- rocky, `REQUIRE_TOOLCHAIN=1` on the six new packet tests: 6 passed.
- Pi `make test-fast JOBS=4`: conveyor 1524 passed, 796 skipped; cloud 770 passed, 150 skipped; exit 0.
- rocky `REQUIRE_TOOLCHAIN=1 make test-fast JOBS=16`: conveyor 2269 passed, 51 skipped; cloud 919 passed, 1 skipped;
  exit 0 (the first run failed 2 tests only because the fresh clone had no `origin/master` ref).

## Notes for the coordinator

- Fresh worktree setup for the ROM gate: `cp baserom.us.z64`, `ovl_splice rebuild` for both images, copy
  `build/game_code.bin` and `build/blob/obj` from the main checkout (objects are provenance-checked by `blob_splice
  check`), `blob_layout derive`, `blob_rom produce` (stream byte-identical to the ROM). `blob_rom produce` rewrote
  `asm/us/blob/symbols.json` and `src/blob/blob.ld` (2,240 lines dropped) in this worktree; reverted, not committed.
- `ovl_splice` used the default builder dir `/tmp/ovlsplice`; concurrent lanes splicing overlays should set
  `OVL_BUILDER_DIR`.
