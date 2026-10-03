# Seventh frozen cut: eight additional runtime bodies

Exactly 16 fresh whole-function attempts / 1,764 B are frozen: eight
independently reviewed strict matches / 824 B and eight complete nonmatches /
940 B. Later low-eight, macro-five, initialization, variables and dispatch
packets are excluded, regardless of their current local results.

| Packet | Matches | Complete nonmatches |
|---|---:|---:|
| BT03-high-helpers | 6 / 584 B | 0 |
| BT05-conditions | 0 | 5 / 688 B |
| BT03-low-followon | 2 / 240 B | 3 / 252 B |

Every source in `reviewed_sources.json` is byte-identical to its exact final
peer-reviewed packet commit. The six helpers matched the first natural O2 form;
actual list/pointer/mixed-float ABI and all O1 controls were independently
reviewed. The two low wrappers retain their genuine five-input and release
contracts. The three low residuals and five conditional-transfer residuals
remain complete NONMATCH with no credit. Known narrow-formal coalescing
plateaus were bounded rather than forced with invented formals, keepers or
protected-tool changes. The conditions packet retains its documented pointer
domain qualification and does not claim identity with a different middleware
platform's source.

## Parent proof and unique totals

This cut explicitly stacks on [#66](https://github.com/cabi24/SFRush2049-decomp/pull/66)
head `e22b0c2d2437c16a2448c048240f3a5ec2d7b33c`, tree
`5b6ff3d3e11960d0879e623c2d1de886e86fff62`. Exact
[Verify 37159760954](https://github.com/cabi24/SFRush2049-decomp/actions/runs/37159760954)
passed; this source cut carries its proof in `../wave6/ci.json` and promotes
those nine unchanged source rows to VERIFIED-BODY. Prior new CI-verified
bodies total 157 / 11,236 B. This cut adds eight / 824 B, yielding 165 unique
new bodies / 12,060 B if its own exact-head CI passes. The existing 12-byte
getter stays separate.

Across seven source cuts there are 224 distinct attempted addresses: 165
matching candidates, 58 complete nonmatches / 6,684 B, and the one previous
96 B SOURCE-LEAD / needs-rodata-proof at 80021548. Its table mapping remains
unproved and receives no complete-native or matching credit. Repeated donor
probes add no unique targets. See `unique_totals.json`.

## Checks and scope

The aggregate checker recompiles exact source hashes, validates native extents
and reproduces all 16 selected outcomes. Every match has full relocated equality,
no masked/unresolved/unverified fields or relocation errors, and no nonzero
excess words. Source/native/caller/callee ABI, flag controls and recorded
C89/host/layout/sanitizer tests were peer-reviewed. All 30 low-followon controls,
six low-followon sanitizer tests and 631 existing regressions passed in its
isolated source packet; other packet evidence is preserved alongside its sources.

```sh
python3 cloud/work/boot_tail/scripts/verify_wave7.py --check
python3 tools/cloud/check_submissions.py --base e22b0c2d --head HEAD
python3 cloud/work/boot_tail/scripts/generate.py --check
python3 -m unittest discover -s cloud/work/boot_tail/tests -v
```

Only allowed cloud boot-tail source/work paths differ from #66. D10 is
byte-identical; its blocked update stays excluded. No ROM/image bytes, raw
native dumps, objects, credentials, protected target/scorer, shared types,
layout/lock/symbol, runtime-image/farm or production-gate edits are included.
The draft targets master for CI, with prior drafts as explicit dependencies.
No merge, cartridge coverage or maintainer acceptance is claimed. Leave merging
to the owner's independent checker.
