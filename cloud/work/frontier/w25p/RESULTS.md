# w25p: integration of Astra's PRs #335–#352 (2026-10-12)

Branch `w25p` from `wave24` `7eb1ae51`. Not pushed. #352 opened during the lane (head of the statistics-panel chain)
and is included. #322/#325 have no commit after 2026-10-12T04:00Z (nothing re-imported).

## Outcome

| PR | What | Verdict | Bytes | Gate (`splice.log`) |
|---|---|---|---:|---|
| #335 | A `func_803A5658` standings renderer, superseding `dot_runtime_a_text_helpers_20261006` (12 locked + 1) | LAND | 736 | `group spliced [... 13 ...]; image gate byte-identical` |
| #336 | A `func_80394564` availability producer, superseding `dot_runtime_a_availability_refresh_20261007` | LAND | 720 | `group spliced ['func_80394564', 'func_80394834']; image gate byte-identical` |
| #337 | A `func_803ADDA8` option callback, superseding `dot_runtime_a_option_eligibility_20261006` | LAND | 1,048 | `group spliced ['func_8038F744', 'func_803ADDA8', 'func_803AE1C0']; image gate byte-identical` |
| #338 | B `func_8038D054` position initializer (inherited lean match, never locked) | LAND | 340 | `group spliced ['func_8038D054']; image gate byte-identical` |
| #339 | B `func_80392894` stunt callback, 2/468 | IMPORT-ONLY | 0 | — |
| #340 | A `func_803A6B50` projected options, superseding `dot_runtime_a_seven_option_group_20261007` | LAND | 964 | `group spliced ['func_80399394', 'func_803A6B50']; image gate byte-identical` |
| #341 | A statistics panels, new group `runtime_a_stats_menu_20261006` (6 members; 3 inherited unlocked) | LAND | 4,548 | `group spliced [... 6 ...]; image gate byte-identical` |
| #342 | A `func_803A5358` scrolling settings renderer, 58/192 | IMPORT-ONLY | 0 | — |
| #343 | B `func_8038DDDC` nearest-target steering | LAND | 684 | `group spliced ['func_8038DDDC']; image gate byte-identical` |
| #344 | A `func_803AB5FC` extra-car panel, stats group (7) | LAND | 588 | `group spliced [... 7 ...]; image gate byte-identical` |
| #345 | A `func_803A8B74` keyboard renderer, new group `runtime_a_profile_menu_20261006` (+ inherited A87D4) | LAND | 2,220 | `group spliced ['func_803A87D4', 'func_803A8B74']; image gate byte-identical` |
| #346 | A `func_803A7180` file-menu renderer, superseding `dot_runtime_a_menu_renderers_20261011` | LAND | 1,184 | `group spliced [... 4 ...]; image gate byte-identical` |
| #347 | B `func_8038CB20` trail callback + owned data | LAND | 1,332 | `group spliced ['func_8038CB20']; image gate byte-identical` |
| #348 | A `func_803AE63C` settings-bar callback | LAND | 772 | `group spliced ['func_803AE63C']; image gate byte-identical` |
| #349 | A `func_803AB848` unlock panel, stats group (8) | LAND | 1,664 | `group spliced [... 8 ...]; image gate byte-identical` |
| #350 | A `func_80395E24` profile owner, profile group (3) | LAND | 104 | `group spliced ['func_80395E24', 'func_803A87D4', 'func_803A8B74']; image gate byte-identical` |
| #351 | A `func_803AECE4` settings values | LAND | 2,656 | `group spliced ['func_803AECE4']; image gate byte-identical` |
| #352 | A `func_803ACA94` cumulative panel, stats group (9) | LAND | 712 | `group spliced [... 9 ...]; image gate byte-identical` |

Overlay A 56/193 (25,196 B, 12.98%) → **75/193 (43,112 B, 22.21%)**; overlay B 11/49 (3,608 B, 8.22%) → **14/49
(5,964 B, 13.59%)**. Total +20,272 B (A +17,916, B +2,356). Static 614/667 (137,484 B) and game 986/1216 (326,688 B)
unchanged.

Gates after all splices (`ovl_rom.log`): `ovl_layout build` (A 75 locked C bodies, B 14, both "byte-identical"),
`ovl_splice check` ("ovl lock: all 89 runtime-image entries intact"), `ovl_rom rom`: both streams `== ROM`, "ROM build
(game blob + both runtime-image streams composed): SHA-1 EXACT", "ROM matches!", `MAKE=0 TEST=0`.

## Dependency map and install order

| Locked group on wave24 | Superseded by | New members |
|---|---|---|
| `dot_runtime_a_text_helpers_20261006` (12) | #335 (13) | A5658 |
| `dot_runtime_a_availability_refresh_20261007` (1) | #336 (2) | 94564 |
| `dot_runtime_a_option_eligibility_20261006` (2) | #337 (3) | ADDA8 |
| `dot_runtime_a_seven_option_group_20261007` (1) | #340 (2) | A6B50 |
| `dot_runtime_a_menu_renderers_20261011` (3) | #346 (4) | A7180 |
| (new) `runtime_a_stats_menu_20261006` | #341 (6) ⊂ #344 (7) ⊂ #349 (8) ⊂ #352 (9) | |
| (new) `runtime_a_profile_menu_20261006` | #345 (2) ⊂ #350 (3) | |
| (new) one-member groups | #338 B D054, #343 B DDDC, #347 B CB20, #348 A E63C, #351 A ECE4 | |

No PR target was locked on wave24 (incl. #338's 8038D054), no migration or `--claims all` was needed: every packet
claims all its members and each superseding packet carries every locked member. Chains installed in order
(#341 → #344 → #349 → #352, #345 → #350), each step gated, so coverage rose monotonically. #337/#340 and #348/#351/#342
touch disjoint functions. Overlay code keeps the address names the locked overlay groups already use (e.g.
`func_80004990` for sprintf); no libultra/ll helper is referenced.

## Notes

- #340, #346 verifiers compared the packet with the *pre-install* production source (`src/ovl_a/groups/...`), so they
  failed once installed (missing `group.c`; "change exceeds sole bounded capacity edit"). Patched both to accept the
  installed state too, re-recorded their receipts `--deep` on rocky: only the `verify.py` input hash changed.
- #341's 4,548 B include 2,692 B (AAF0C, AC330, ACF5C) matched in older packets but never locked; #345's 2,220 B
  include inherited A87D4 (920 B). #338 is likewise an older lean match, first locked here.
- Residuals put on CLAIMS: #339 B 80392894 2/468 (sentinel constant / pending-count store swapped at +0x398/+0x39c;
  frame 176 vs 184 needs two unused locals); #342 A 803A5358 58/192 (rematerialized text address, temp-register ring,
  frame 128 vs 136); A9EEC 281/315 (frame 280 vs 288, notes in #350's packet). 8039A4D4 stays paused.

## Tests

- rocky, `REQUIRE_TOOLCHAIN=1` on the 18 new packet tests: 18 passed.
- rocky `REQUIRE_TOOLCHAIN=1 make test-fast JOBS=16`: conveyor 2311 passed, 53 skipped; cloud 942 passed, 1 skipped;
  exit 0.
- Pi `make test-fast JOBS=4`: conveyor 1568 passed, 796 skipped; cloud 770 passed, 173 skipped; exit 0.

## Setup notes

As w24p: `cp baserom.us.z64`, copy `build/game_code.bin` + `build/blob/obj`, `ovl_splice rebuild` A/B, `blob_layout
derive`, `blob_rom produce` (stream byte-identical; its rewrites of `asm/us/blob/{SHA256SUMS,symbols.json}` and
`src/blob/blob.ld` reverted). `OVL_BUILDER_DIR=/tmp/ovlsplice-w25p`. Rocky clone `~/rush2049/scratch/w25p` (renamed
from w24p). Packet tests need `pytest --dist loadgroup` when run with `-n`.
