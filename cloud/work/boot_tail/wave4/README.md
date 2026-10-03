# Fourth cut: 21 more strict bodies and an explicit proof gap

Frozen cut: 21 additional peer-reviewed strict matching bodies / 2,012 B from 32
attempts. Ten complete nonmatches / 1,240 B are archived. One additional 96 B
function,80021548, is a **SOURCE-LEAD / needs-rodata-proof**, not a complete native
reconstruction. Later work and source-led rechecks are excluded from this cut.

| Packet | New local matches | Complete nonmatches | Blocked lead |
|---|---:|---:|---:|
| BT02-remaining | 8 / 628 B | 0 | 0 |
| BT07-medium | 5 / 632 B | 1 / 152 B | 0 |
| BT03-high-service | 5 / 424 B | 1 / 120 B | 0 |
| C13-tail | 1 / 100 B | 5 / 692 B | 1 / 96 B |
| BT05-medium-next | 2 / 228 B | 3 / 276 B | 0 |

The canonical 21548 metadata names a six-entry table address but does not supply
its six case-to-block entries. The candidate mapping comes from a pinned licensed
MusyX source-family lead, not independently proved native data. Both recorded
flag levels have unverified local-table relocation evidence. No target/scorer
repair, table-byte inspection or proof-gap substitution was used. Caller semantic
tests isolate the translator as an external contract. This target receives no
match credit and is excluded from the next-candidate queue.

All 32 integrated sources are byte-identical to their independently reviewed
packet commits. `reviewed_sources.json` retains their flags, extents, outcomes
and provenance. `verification.json` recompiles all 21 strict matches and the 11
nonacceptance cases; it keeps the blocked lead's proof state distinct. Every
submitted match has full relocated equality without masked/unresolved/unverified
fields, relocation errors or nonzero excess words.

## Unique counts and explicit parent

This cut stacks on [#63](https://github.com/cabi24/SFRush2049-decomp/pull/63)
head `aeba70fccebf2631955c82272c43a466a6cade52`, exact tree
`1605a13a44fee1dbd0941471222e448bdd1954b1`. Its successful exact-head
[Verify 37155038665](https://github.com/cabi24/SFRush2049-decomp/actions/runs/37155038665)
receipt is now in `../wave3/ci.json`. Prior published new matches are 103 / 4,924 B;
this cut adds 21 / 2,012 B. The union is 124 new bodies / 6,936 B if this cut's CI
passes, plus the separately counted pre-existing 12-byte getter. All 167 attempted
addresses are unique;42 complete nonmatches / 3,424 B and the 96-byte proof gap
remain outside matching credit. See `unique_totals.json`.

The draft targets master for the existing CI workflow; earlier drafts are
explicit dependencies. No merge or cartridge promotion is performed. Only the 21
new source files count as this cut's gain.

## Verification

```sh
bash tools/cloud/setup.sh
python3 cloud/work/boot_tail/scripts/verify_wave4.py --check
python3 tools/cloud/check_submissions.py --base aeba70fccebf2631955c82272c43a466a6cade52 --head HEAD
python3 cloud/work/boot_tail/scripts/generate.py --check
python3 -m unittest discover -s cloud/work/boot_tail/tests -v
python3 -m unittest discover -s cloud/work/boot_tail/C13-tail -v
```

Paired reviews cover actual source/native/ABI behavior, all flag controls,
source hashes and per-packet C89/host tests. Stronger host controls are retained
where run (including ASan/UBSan, unsigned boundaries and token index 1 checks),
with their native pointer/layout limitations stated. Host tests do not replace
native equality or solve the missing switch mapping. The strict aggregate,
allowed-path guard, static locks, unchanged manifest and whitespace are checked
again after integration; exact new-head CI remains required.

`dot_handoff.md` is byte-identical to the published parent. The denied handoff
update is excluded; current proof is in the allowed ledger/work files and PR.
No raw native dumps, ROM/image bytes, objects, credentials, target/scorer,
lock/layout/symbol/shared-header, runtime-image/farm or production-gate edits.
Leave merging to the owner's independent checker.
