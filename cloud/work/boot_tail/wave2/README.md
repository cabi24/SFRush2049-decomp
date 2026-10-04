# Second boot-tail cut: 31 additional peer-reviewed strict matches

Source-frozen publication cut: **31 new strict local matches / 1,728 B**, plus
**17 complete nonmatches / 1,156 B**. All 48 attempts total 2,884 B. Later packets
remain outside this cut, even where peer review has since completed. These 31
matches are claimed pending exact-head CI; prior first-wave source proof remains
42 new verified bodies / 1,332 B. Combined proposed matching source is 73 bodies /
3,060 B, separate from the pre-existing 12-byte getter. No cartridge or maintainer
acceptance credit is claimed.

| Packet | Attempts | New matches | Complete nonmatches | Exact peer-reviewed source head |
|---|---:|---:|---:|---|
| BT05-controls | 9 /396 B | 9 /396 B | 0 | `e71c375bfab44e6ec257bdd96c87a495ff488eb2` |
| BT03-high-next | 9 /328 B | 7 /248 B | 2 /80 B | `f114f529b0e32f6e7ef0a495a14bad0cd37e0adf` |
| BT02-medium | 10 /952 B | 8 /680 B | 2 /272 B | `696db6aa0fa19ffb4cb6161166f2a937f419e5c3` |
| BT03-low-B | 8 /284 B | 2 /24 B | 6 /260 B | `a176191d4db38bb6a62fb2d3577667fca73e3ea2` |
| C13-medium | 6 /672 B | 1 /228 B | 5 /444 B | `2e6bd41116ca0d64f2cd580cf3ef5dccb77b5792` |
| BT05-wrappers | 6 /252 B | 4 /152 B | 2 /100 B | `6315540b19d55d528eff090e8c3f726590203357` |

Every integrated C file is byte-identical to its frozen, independently reviewed
source head. `reviewed_sources.json` binds the whole source, chosen flags, native
extent, peer and exact expected outcome. `verification.json` recompiles all 48
actual sources through the unchanged strict scorer. All 31 submissions have full
relocated word equality, zero masked/unresolved/unverified relocations and no
nonzero excess words. All 17 archives reproduce their exact nonzero residuals.

## Base and review boundaries

The packet workers used fresh master `301d9e7552ad4fd7f54a38796db84671e1000d35`.
Publication explicitly stacks on first-wave draft [PR #61](https://github.com/cabi24/SFRush2049-decomp/pull/61),
receipt head `61c880a011f974a40c6a5a8566d8c6330e50607e` and tree
`a1f6542ead12fd1a2d730b7332c11dc96cbcafc5`. Master was freshly confirmed unchanged
before the cut. The PR targets master to obtain exact-head repository CI; its
first-wave content is an explicit dependency. Nothing was merged.

Paired source/ABI/native review was performed by BT03-high/BT05, BT03-low/BT02,
and BT03-high/C13 peers as recorded in the source manifest and packet receipts.
Important ABI control: the initial no-argument zero-score `80020528` candidate
was rejected. The final source forwards its genuine pointer through the existing
call chain; only that hash-reviewed complete ABI source is submitted.

The common narrow-formal coalescing residual is archived rather than forced:
several native byte-formal homes/masks stay in original argument registers while
natural O2 introduces a copy. Prototype, real result and C89 spelling controls
were bounded; no fake formals, padding, keeper calls, assembly or target/scorer
changes were used. Frozen attempts are not automatically reopened by later workers.

## Reproduce

```sh
bash tools/cloud/setup.sh
python3 cloud/work/boot_tail/scripts/verify_wave1.py --check
python3 cloud/work/boot_tail/scripts/verify_wave2.py --check
python3 tools/cloud/check_submissions.py --base 61c880a011f974a40c6a5a8566d8c6330e50607e --head HEAD
python3 cloud/work/boot_tail/scripts/generate.py --check
python3 -m unittest discover -s cloud/work/boot_tail/tests -v
python3 -m unittest discover -s cloud/work/boot_tail/C13-medium -v
```

Per-packet repro commands include O2/O1 controls and host semantics. Local
aggregate checks validate all prior 42 and new 31 matching sources, all 25 archived
nonmatch residuals across both cuts, generated status, allowed paths, static
locks and ordinary whitespace. No production image/ROM gate is run. Exact-head
CI status is carried on the PR; source-local equality alone is not final CI.

Only allowed cloud matching/work files change relative to the first-wave parent.
`dot_handoff.md` is deliberately byte-identical to the already-published first-wave
file: its final proof text could not be updated without republishing inherited
unrelated metadata, so the denied file remains untouched. STATUS.csv, this README,
the source manifests and PR checks carry the current evidence. No inherited
handoff content is removed and no alternative write route is used for that file.
