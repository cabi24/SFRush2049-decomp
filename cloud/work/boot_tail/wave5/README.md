# Fifth frozen source cut: 24 additional strict bodies

Exactly 26 fresh whole-function attempts / 3,884 B are frozen here: 24
peer-reviewed strict matches / 3,548 B and two complete nonmatches / 336 B.
Later control, low-medium, stream and source-led research packets are excluded.

| Packet | Matches | Complete nonmatches |
|---|---:|---:|
| BT03-high-state | 6 / 560 B | 0 |
| BT02-followon | 6 / 548 B | 0 |
| BT07-followon | 5 / 804 B | 1 / 128 B |
| BT02-larger | 5 / 1,084 B | 1 / 208 B |
| BT07-larger | 2 / 552 B | 0 |

Every source byte matches its exact independently reviewed packet commit in
`reviewed_sources.json`. The final BT07 follow-on uses the existing opaque
`OSMesgQueue_s` tag, as required by peer review. Source/ABI reviews checked the
actual callers, callbacks, queue semantics, fields and genuine formals, rather
than relying only on matching instructions. The larger BT02 task initializer
80011910 remains complete NONMATCH at 47/52; BT07 boolean query 800255F0 remains
complete NONMATCH at 2/32. Neither receives match credit.

## Parent proof and unique totals

This cut explicitly stacks on [#64](https://github.com/cabi24/SFRush2049-decomp/pull/64)
head `cbd048e6d03710fe828c8844de4d9d010da19ae0`, tree
`39d6072013ea5000b231911534bbb590ae493775`. Its exact source-head
[Verify 37156783599](https://github.com/cabi24/SFRush2049-decomp/actions/runs/37156783599)
passed; the receipt is carried in `../wave4/ci.json` and those 21 rows are now
VERIFIED-BODY. Prior published new bodies total 124 / 6,936 B. This cut adds
24 / 3,548 B, yielding 148 unique new matches / 10,484 B if its own exact-head
CI passes. The pre-existing getter is separate: one body / 12 B.

Across five disjoint source cuts there are 193 unique attempted addresses,
44 complete nonmatches / 3,760 B and the previously documented 96 B
SOURCE-LEAD / needs-rodata-proof at 80021548. Its six-case table mapping is still
unproved and not a complete native reconstruction. See `unique_totals.json`.

The draft targets master for the existing CI workflow, with earlier drafts as
explicit dependencies. No merge, cartridge coverage or maintainer acceptance is
claimed. Prior source and verification artifacts stay unchanged.

## Checks

`verification.json` recompiles the 26 exact source hashes with their recorded
IDO flags, verifies extents and reproduces both match and residual states.
Every match has full relocated word equality, no masked/unresolved/unverified
fields or relocation errors, and only zero alignment beyond its whole body.
O1 controls, pinned toolchain/input checks, actual-source C89 behavior,
sanitisers and native-width layout assertions are recorded per packet. Host
behavior tests supplement rather than replace native equality.

```sh
python3 cloud/work/boot_tail/scripts/verify_wave5.py --check
python3 tools/cloud/check_submissions.py --base cbd048e6d03710fe828c8844de4d9d010da19ae0 --head HEAD
python3 cloud/work/boot_tail/scripts/generate.py --check
python3 -m unittest discover -s cloud/work/boot_tail/tests -v
```

Only allowed cloud boot-tail source/work files differ from the published parent.
`dot_handoff.md` is byte-identical; the blocked handoff update remains excluded.
Current counts/proof are in the allowed ledger and receipts. No target/scorer,
layout/lock/symbol/shared-header, runtime-image/farm or production-gate edits;
no ROM/image bytes, native dumps, objects, credentials or unrelated private data.
Leave merging to the owner's independent checker.
