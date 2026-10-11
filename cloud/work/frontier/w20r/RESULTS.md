# w20r: recomp Phase 0 (tooling lane, worktree branch w20r)

Status: done (2026-10-11). Not a matching lane: no src/, asm/, lock or coverage change.

## Result

N64Recomp (ffb39cda, rocky) consumes the generated config with no hand edits, exit 0, 0.2 s:

| Population | Inventory | Emitted | Not emitted |
|---|---:|---:|---|
| game (0x80086A50) | 1216 | 1216 | 0 |
| image A (0x8038A400) | 193 | 193 | 0 |
| image B (0x8038A400) | 49 | 49 | 0 |
| static (boot segment) | 668 | 578 (+10 provisional) | 86 on N64Recomp's libultra lists (47 reimplemented, 39 ignored), 4 ignored by patches.json |

Plus `static_1_80104704` (a game function missing from the inventory, recovered by N64Recomp).
All 41 output files compile with clang 21 (`-Wno-error=implicit-function-declaration`; 20 ignored
libultra internals are called but undeclared: the Phase 1 shim list).

## Files (branch w20r)

- tools/recomp/gen_config.py, check_output.py, extra_functions.json, patches.json, README.md, VERSIONS.md
- tests/recomp/test_gen_config.py (18 tests; ROM-free except one e2e that skips without the ROM)
- docs/plans/2026-10-12-recomp-port-plan.md §8 "Phase 0 findings"

## Commands

See tools/recomp/README.md "Commands (rocky)". Generated output: build/recomp/ (private, ignored).

## Integration notes

Merge branch w20r as is. Nothing generated is committed. `extra_functions.json` extents are
PROVISIONAL (recompiler only) and must not feed layout/coverage; they answer nothing about the
BT08 census boundary question beyond "these addresses are code".
