# Thirteenth frozen cut: three additional runtime bodies

This cut contains 17 fresh whole-function attempts / 10,124 B: three independently
reviewed strict matches / 1,504 B and fourteen complete nonmatches / 8,620 B.
Every selected C source preserves its recorded peer-approved commit bytes.
Current spatial callers, emitter, sequence dispatch and later work are excluded.

| Packet | Matches | Complete nonmatches |
|---|---:|---:|
| BT03-high-runtime-followon | 2 / 940 B | 4 / 1,828 B |
| BT06-matrix-inverse | 1 / 564 B | 0 |
| BT03-low-insert-pair | 0 | 2 / 1,084 B |
| BT03-low-final-resource | 0 | 2 / 1,120 B |
| BT02-audio-final | 0 | 3 / 2,432 B |
| C11-rate-convert | 0 | 1 / 380 B |
| BT03-runtime-singleton | 0 | 1 / 840 B |
| BT03-chain-dispatch | 0 | 1 / 936 B |

## Actual source and domains

The 564-byte affine-matrix inverse has two genuine pointers to 48-byte objects,
with a 3x3 matrix followed by three translation floats. Its complete141-word O2
body matches exactly. One pinned CC0 family expression control restores native
IDO floating operation order; no additional source tuning was done. The caller
supplies distinct real objects. Finite nonsingular matrices and ordinary FP state
are the intended inverse domain; no singularity fallback is fabricated. Tests
also preserve native alias overwrite order and bounded signed-zero/nonfinite
observations without claiming that aliasing still computes a mathematical inverse.
The peer replayed all2,492 sanitizer executions over623 binary32 fixtures and
added a native instruction-order model over1,246 disjoint/alias fixtures.

The two runtime matches preserve full-word identifiers rather than narrowing a
packed child load to a byte. Keeping the original input handle while reusing the
real parameter for translation closes register allocation naturally. All26 packet
rows, exact ELF extents, native-width proofs and six sanitizer groups /70,276 cases
were independently checked. Four companion bodies remain complete nonmatches.

The parent directly reviewed both insertion/resource pairs and the final rate
converter against full native bodies. Packed8-/12-byte record copies, reference
wrap, unlock order, live counts, bucket updates, real payload pointers and
untouched bytes are explicit. The rate converter used exactly one natural source
with O2/O1 and diagnosis, then stopped the already known byte-home plateau.
External float-table contents and overall extents are not fabricated. Coherent
synthetic views, valid supplied indices, nonzero rate and finite conversion ranges
are test domains, not proof of original data or a universal producer invariant.

Audio, event and chain-dispatch research preserves every real argument and
callback/callee effect. The six-slot chain signature includes a genuinely unused
vertical-pan argument supported by three callers and the pinned source lead;
no argument was invented to manipulate the frame. Added narrowing instructions
and native/candidate frame differences remain visible in receipts. The public
source's extra parameter-info loop is absent from N64 and is omitted. Peer tests
include callback mutation, poisoned caller registers, callee-save/full-memory
checks and optimized-host negative mutations. Each complete residual receives
zero matching credit regardless of its native-like extent or low excess count.

## Strict proof

Every accepted source has full relocated word equality and exactly one correctly
named ELF function at text offset zero with the complete native size. Remaining
text-section words must be zero alignment outside that body. Masks, unresolved
or unverified fields, relocation errors and nonzero excess are rejected. Research
rows report actual function-symbol size separately from nonzero excess so zero
body words cannot hide an overlong function. Protected targets and scorer are
unchanged.

## Exact parent, totals and verification

This explicitly stacks on [#72](https://github.com/cabi24/SFRush2049-decomp/pull/72)
head `1ef54c2e507e6d29cc22d041bf5d99dc2173bfd8`, tree
`f26b129a53d5ae7299da71ac3a1b584b15f56cb1`. Fresh read-only Git-data and PR
metadata confirm that exact head/tree and parent. Exact
[Verify 37172540204](https://github.com/cabi24/SFRush2049-decomp/actions/runs/37172540204)
passed; `../wave12/ci.json` carries the proof. Local checkpoint
`ad26279fe31cae86374b357d41ffcce23afe112f` has that identical tree and is used as
the local direct-tree comparison base. No history substitution or graft is used.

Prior new CI-verified bodies total231 /23,756 B. This cut adds three /1,504 B,
yielding234 unique new bodies /25,260 B if its own exact-head CI passes. The
historical12-byte getter is separate. Across thirteen cuts,383 distinct attempted
addresses comprise234 matching candidates,148 complete nonmatches /39,304 B and
the unchanged96-byte SOURCE-LEAD /needs-rodata-proof at80021548. Repeated controls
add no targets. See reviewed_sources.json, verification.json and unique_totals.json.

Fresh aggregate replay, the exact three-source canonical submission gate, central
28 tests, deterministic ledger/opcode checks, all161 locks, source hashes, allowed
paths and whitespace are checked. Host/model tests complement native equality;
they do not establish hardware/concurrency, cartridge coverage or acceptance.

Only allowed cloud boot-tail sources and work artifacts differ from #72. D10 is
byte-identical; its blocked update is excluded. No ROM/image bytes, raw native
dumps, objects, credentials, protected input/scorer, shared types/layouts/locks,
symbols, runtime-image/farm or production-gate edits are included. The draft
targets master for existing CI with prior drafts as explicit dependencies.
Leave merging to the owner's independent checker.
