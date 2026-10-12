# w26p: integration of Astra's PRs #359–#366 (2026-10-12)

Branch `w26p` from `wave26` `46780d3e`. Not pushed. Of #322/#325/#339/#342/#357 only #357 has a commit after
2026-10-12T14:00Z (`2b3ff865`, 16:44Z), and its packet/test are byte-identical to what w25q imported (`git diff HEAD
pr357` empty); the others' last commits are 10-11T23:14Z, 10-12T01:02Z, 11:59Z, 12:11Z. Nothing re-imported.

## Outcome

| PR | What | Verdict | Bytes | Gate (`splice.log`) |
|---|---|---|---:|---|
| #359 | A `func_80394B00` availability reset, superseding `dot_runtime_a_availability_refresh_20261007` (2 locked + 1), `--claims all` | LAND | 728 | `image A: group spliced ['func_80394564', 'func_80394834', 'func_80394B00']; image gate byte-identical` |
| #360 | A `func_803ABEC8` aggregate statistics + 28 B switch table, superseding `runtime_a_stats_menu_20261006` (10 locked + 1) | LAND | 1,128 | `image A: group spliced [... 11 ...]; image gate byte-identical` |
| #361 | B `func_8038A408` projected-surface ring, 105/305 | IMPORT-ONLY | 0 | — |
| #362 | B `func_80391864` kind HUD scene + 40 B owned data, superseding `dot_runtime_b_status_scene_20261012` (1 locked + 1) via `--name` | LAND | 668 | `image B: group spliced ['func_80391650', 'func_80391864']; image gate byte-identical` |
| #363 | A `func_803AA3D8` stats profile selection, 2/717 (frame 192 vs 176 only) | IMPORT-ONLY (near-landing) | 0 | — |
| #364 | B `func_8038C910` callback initializer, 13/69 | IMPORT-ONLY | 0 | — |
| #365 | A `func_80397FFC` availability root, 15/268 | IMPORT-ONLY | 0 | — |
| #366 | A `func_803A7B58` profile-selection renderer, superseding `runtime_a_profile_menu_20261006` (3 locked + 1); 96EE8/96F44 kept as singles | LAND | 3,196 | `image A: group spliced ['func_80395E24', 'func_803A7B58', 'func_803A87D4', 'func_803A8B74']; image gate byte-identical` |

Overlay A 77/193 (44,700 B, 23.03%) → **80/193 (49,752 B, 25.63%)**; overlay B 20/49 (8,352 B, 19.03%) →
**21/49 (9,020 B, 20.55%)**. Total +5,720 B (A +5,052, B +668). Static 617/667 (139,624 B, 86.87%) and game
997/1216 (329,104 B, 50.86%) unchanged (`make progress`).

Gates after all splices (`ovl_rom.log`): `ovl_layout build` (A 80 locked C bodies, B 21, both "byte-identical"),
`ovl_splice check` ("ovl lock: all 101 runtime-image entries intact"), `ovl_rom rom`: both streams `== ROM`, "ROM build
(game blob + both runtime-image streams composed): SHA-1 EXACT", "ROM matches!", `MAKE=0 TEST=0`. A fresh
`ovl_splice rebuild` of A and B from the locked sources (objects deleted first) is byte-identical (80/21 bodies).

## Dependency map and install order

| Locked group on wave26 | Superseded by | New members |
|---|---|---|
| `dot_runtime_a_availability_refresh_20261007` (2: 94564, 94834) | #359 (3) | 94B00 |
| `runtime_a_stats_menu_20261006` (10) | #360 (11) | ABEC8 (was context) |
| `dot_runtime_b_status_scene_20261012` (1: 91650) | #362 (2), installed with `--name` | 91864 (was context) |
| `runtime_a_profile_menu_20261006` (3: 95E24, A87D4, A8B74) | #366 (4), `--claims` without 96EE8/96F44 | A7B58 (was context) |
| research | #365 carries #359's 94B00 (verified after #359); #363 carries the pre-#360 stats group (10) | — |

Order: #359 → #360 → #362 → #366, each gated and committed separately, so coverage rose monotonically
(A 77 → 78 → 79 → 80, B 20 → 21). Then research imports #363, #361, #364, #365 (one commit each).

## Notes

- #362: the packet directory is `dot_runtime_b_kind_scene_20261012` but it re-carries 80391650, locked in
  `dot_runtime_b_status_scene_20261012`; installing under the packet name fails the clash check, so it was installed
  with `--name dot_runtime_b_status_scene_20261012 --claims all` (91650 body textually identical to the locked one).
- #366: the packet claims 96EE8/96F44 (92 B each), which are locked as standalone O2 singles
  (`src/ovl_a/func_80396EE8.c`, `func_80396F44.c`), and its RESULTS asks the checker to reconcile ownership rather than
  unlock them. Installed with `--claims func_80395E24,func_803A87D4,func_803A8B74,func_803A7B58`: the singles keep their
  locks, the group carries them as byte-identical context (the packet proves the emitted bodies exact). No coverage lost.
- Verifier trap (w25p #340/#346 again): #366 and #363 `verify.py` used `src/ovl_a/groups/<group>/group.c` as BASELINE
  and bound its hash, so they would fail once #366/#360 are installed. Both now read a pre-install snapshot
  `baseline_group.c` in the packet (byte-identical to the wave26 source; for #363 the pre-#360 stats group). Receipts
  re-recorded `--record --deep` on rocky: only the input path label (same sha256) and the `verify.py` hash changed.
- #359's and #360's verifiers compare against packet-local baselines (no production source); no change needed.
- #363 AA3D8 2/717: every native instruction except frame allocation/release (emitted 192 vs native 176 B); all 32
  owned literal/switch bytes verify. Put on Astra's CLAIMS row as a near-landing; a landing packet must now carry
  ABEC8 (stats group is 11).
- #357's verifier (`dot_runtime_a_delete_profile_20261012`) had the same trap against the profile group; fixed the
  same way (see Tests). `dot_runtime_a_writer_bounds_20261012` and `dot_runtime_a_capacity_evidence_20261012` also
  read production group sources, but groups this lane did not touch (menu_renderers, seven_option_group).
- #365 is research only; its packet preserves #359's 94B00 and verifies after #359's install.
- Shaping devices disclosed by the packets: #366 has one compiled-out `if (label) {}` read (no instruction emitted).

## Packet verifiers (before and after installation)

Before install (wave26 base + packets), rocky `REQUIRE_TOOLCHAIN=1 pytest -n 8 --dist loadgroup` on the eight new
packet tests: 8 passed. After install, every `verify.py --deep` exit 0:
"PASS: 728 new bytes; 1120 preserved; complete GNU/extent/selective proof",
"PASS: 1128 new exact candidate bytes; 8696 prior bytes preserved; no accepted-image/ROM credit",
"PASS complete GNU/native members, owned literals/table, caller, installer and receipt",
"PASS A7B58 stock exact3196B;2508 inherited bytes preserved" (also `python3 -O`),
"PASS: AA3D8 two-word NONMATCH; 8696 inherited bytes preserved; zero new coverage" (also `python3 -O`),
"Verified empty-claims research: baseline 268/305; stock candidate 105/305; no coverage credit",
"PASS: empty-claims research, 260 -> 15 / 268 words; zero new coverage.", #364 receipt JSON (13/69, 8192 sanitizer
fixtures). The eight packet tests again after install: 8 passed.

## Tests

- rocky, `REQUIRE_TOOLCHAIN=1` on the eight new packet tests: 8 passed before and 8 passed after installation.
- rocky `REQUIRE_TOOLCHAIN=1 make test-fast JOBS=16` (at `dcbb7f82`): conveyor 2324 passed, 52 skipped; cloud 956
  passed, 1 skipped; exit 0.
- Pi `make test-fast JOBS=4` (at `dcbb7f82`): conveyor 1584 passed, 792 skipped; cloud 770 passed, 187 skipped; exit 0.
- The first test-fast run (at `e671ff1f`) failed one test on both hosts: `test_runtime_a_delete_profile` (#357's
  packet, on master since w25q) — "unrelated source changed", the same production-baseline trap, triggered by #366's
  install. Fixed in `dcbb7f82` (snapshot `baseline_group.c`, receipt re-recorded `--deep` on rocky; `verify.py --deep`
  and `python3 -O ... --deep` print "PASS: A9088 NONMATCH 15/917; zero new exact bytes; 2324 preserved").

## Setup notes

As w25q: `cp baserom.us.z64`, copy `build/game_code.bin` + `build/blob/obj`, submodules `--reference` the main
checkout, `ovl_splice rebuild` A/B, `blob_layout derive`, `blob_rom produce` (stream byte-identical; its rewrites of
`asm/us/blob/{SHA256SUMS,symbols.json}` and `src/blob/blob.ld` reverted). `OVL_BUILDER_DIR=/tmp/ovlsplice-w26p`.
Rocky clone `~/rush2049/scratch/w26p` (renamed from w25q). Rocky packet tests need MIPS binutils on PATH: source
`~/rush2049/scratch/w26p.env` (ci.env's `CI_PATH` + civenv), otherwise REQUIRE_TOOLCHAIN fails on "MIPS binutils missing".
