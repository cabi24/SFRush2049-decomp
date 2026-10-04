# Eighth frozen cut: eighteen additional runtime bodies

This cut freezes 29 fresh whole-function attempts / 4,036 B: eighteen
peer-reviewed strict matches / 2,200 B and eleven complete nonmatches /
1,836 B. Current resource, lookup, parameter, control-caller and final-small
packets are excluded. All sources retain the exact reviewed bytes recorded
in `reviewed_sources.json`.

| Packet | Matches | Complete nonmatches |
|---|---:|---:|
| BT03-low-eight | 8 / 1,196 B | 0 |
| BT05-macro-five | 0 | 5 / 888 B |
| BT03-high-init | 4 / 416 B | 2 / 200 B |
| BT05-variables | 1 / 124 B | 4 / 748 B |
| BT03-low-dispatch | 5 / 464 B | 0 |

The low-eight packet uses genuine list storage and an IDO unaligned halfword
input for packed parameters. Natural source refinements preserve real pointer
initialization, attach/detach and saved-head behavior. It adds no invented
formals, keepers, padding or assembler. Native layout and actual-source C89/UBSan
checks supplement strict equality.

The initializer packet preserves its stated valid link-count range, N64 pointer
layout and callback-copy limits; independent tests include live count growth,
mutation and untouched-tail preservation. The dispatch packet's five real
one-u16 helpers and caller were independently inspected in full. Forward walkers
reload after each helper; the reverse walker preserves its initial sentinel scan
and pre-decrement. Its valid interior resource-pointer contract includes the
observed eight-byte prefix; arbitrary standalone arrays, invalid offsets and
missing terminators are not claimed safe.

Macro-five remains entirely NONMATCH. Its actual comparison caller restricts
modes to zero or one; unsupported native scratch behavior is not fabricated
away. Global variable indexing uses the real observed base/range. All 68
compiler rows, native layout and five host groups are preserved. Variables
records one match and four bounded residuals; no unchanged narrow-formal plateau
was forced with fake declarations. None of these residuals receives byte credit.

## Parent proof and unique totals

This cut stacks on [#67](https://github.com/cabi24/SFRush2049-decomp/pull/67)
head `3dd49752e74dda49a581db52efe291782b80521b`, tree
`a7fdcd245bdf1d56f332c309a14c36cd61ca8b40`. Exact
[Verify 37160610106](https://github.com/cabi24/SFRush2049-decomp/actions/runs/37160610106)
passed; its receipt is carried in `../wave7/ci.json`. Prior new CI-verified
bodies total 165 / 12,060 B. This cut adds eighteen / 2,200 B, yielding
183 unique new matching bodies / 14,260 B if its own exact-head CI passes.
The pre-existing 12-byte getter is separate.

Across eight cuts there are 253 distinct attempted addresses: 183 matching
candidates, 69 complete nonmatches / 8,520 B and the earlier 96 B
SOURCE-LEAD / needs-rodata-proof at 80021548. That table mapping remains unproved.
Repeated research never adds duplicate targets. See `unique_totals.json`.

## Verification and limits

The aggregate checker validates source hashes/native extents and independently
recompiles all 29 selected outcomes. Each match has full relocated equality,
zero masked/unresolved/unverified fields, zero relocation errors and zero nonzero
excess words. O1 controls and natural O2 signatures remain per function. Every
source packet has independent actual-source/native/ABI review, with recorded
host, sanitizer and layout checks. Dispatch alone exercises 336,775 actual-source
wrapper calls, including all non-sentinel identifiers and live mutations; host
mocks are contracts, not implementations of the actual callees.

```sh
python3 cloud/work/boot_tail/scripts/verify_wave8.py --check
python3 tools/cloud/check_submissions.py --base 3dd49752 --head HEAD
python3 cloud/work/boot_tail/scripts/generate.py --check
python3 -m unittest discover -s cloud/work/boot_tail/tests -v
```

Only allowed cloud boot-tail source/work paths differ from #67. D10 is
byte-identical; its blocked update remains excluded. No ROM/image bytes, raw
native dumps, object files, credentials, protected input/scorer, shared types,
layout/lock/symbol, runtime-image/farm or production-gate edits are included.
The draft targets master for CI with earlier drafts as explicit dependencies.
No merge, cartridge coverage or maintainer acceptance is claimed. Leave merging
to the owner's independent checker.
