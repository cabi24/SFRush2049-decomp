# Third cut: 30 more peer-reviewed strict matches

Frozen cut: 30 additional strict matching C bodies / 1,864 B, seven complete
nonmatches / 624 B, from 37 whole-function attempts. No later candidates are
included. The 31-body second cut now has completed exact-head CI; its status
receipt is carried in `../wave2/ci.json` without changing that frozen source PR.

| Packet | New local matches | Complete nonmatches |
|---|---:|---:|
| BT03-high-medium | 2 /140 B | 4 /328 B |
| BT03-high-api | 6 /496 B | 0 |
| BT03-low-C | 6 /200 B | 1 /48 B |
| BT07-small | 7 /280 B | 0 |
| BT05-medium | 3 /296 B | 2 /248 B |
| BT03-high-gates | 6 /452 B | 0 |

Every source byte equals its exact paired-reviewed commit recorded in
`reviewed_sources.json`; all 30 matches and seven residuals are independently
reproducible through `scripts/verify_wave3.py --check`. Matching bodies require
full relocated equality with no masked/unresolved/unverified fields or nonzero
excess words. Source and ABI review are both required; a score alone is not an
excuse to omit actual arguments. Bounded complete nonmatches receive no credit.

Unique source accounting in `unique_totals.json` is derived from all three exact
manifests and checked against every boot-tail match filename: first two cuts 73
new bodies / 3,060 B with exact CI; this cut 30 / 1,864 B pending its exact CI;
combined 103 / 4,924 B if this cut passes. The pre-existing getter is separately
12 B. All 135 attempted rows are unique; 32 nonmatches / 2,184 B remain research.
No cartridge or maintainer integration percentage is reported.

## Publication and verification

This draft explicitly stacks on [#62](https://github.com/cabi24/SFRush2049-decomp/pull/62)
at `09ac2a2890c669b12c4e37398fa66b44baadef05`, tree
`f7d5a6fa6aa4a36e403c94dff46dd8830fbd5daa`. That exact source head passed
[Verify 37154187199](https://github.com/cabi24/SFRush2049-decomp/actions/runs/37154187199).
Master was freshly confirmed at 301d9e75; no merge is performed. The draft targets
master to trigger CI. Only the 30 additional source files count as this cut's gain.

```sh
bash tools/cloud/setup.sh
python3 cloud/work/boot_tail/scripts/verify_wave1.py --check
python3 cloud/work/boot_tail/scripts/verify_wave2.py --check
python3 cloud/work/boot_tail/scripts/verify_wave3.py --check
python3 tools/cloud/check_submissions.py --base 09ac2a2890c669b12c4e37398fa66b44baadef05 --head HEAD
python3 cloud/work/boot_tail/scripts/generate.py --check
python3 -m unittest discover -s cloud/work/boot_tail/tests -v
```

Paired reviews inspect actual native bodies, caller/callee contracts, packed
fields and source hashes; per-packet receipts retain O2/O1 evidence and stated
host-test limits. The central strict gate, source-hash preservation, status
population, target manifest and allowed-path checks are repeated after assembly
of the cut. Exact new-head CI remains required before final completion.

`dot_handoff.md` remains exactly equal to the published parent. The denied full
handoff update is excluded; status and proof are kept in permitted cloud files.
No ROM/image bytes, raw native dumps, objects, credentials, protected targets or
scorer, locks/layout/symbol/shared-type edits, runtime-image/farm work or
production gates are added. Leave merging to the owner's independent checker.
