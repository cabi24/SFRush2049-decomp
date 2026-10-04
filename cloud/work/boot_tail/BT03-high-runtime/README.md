# BT03-high runtime state and spatial helpers

Four strict O2 matches, **680 B / 170 words**; two complete NONMATCHs, **352 B**.
Parent activation at2026-10-03T23:56:23Z followed a fresh no-overlap scan of
central20621601. Branch `dot/boot-tail-bt03-high-runtime` uses exact master
`301d9e7552ad4fd7f54a38796db84671e1000d35` in its own sparse /tmp object store.

| Function | Bytes | Final O2 | Final O1 |
|---|---:|---|---|
| 8001D5C0 |160|MATCH|39/40 +23 excess|
| 8001B8C4 |164|MATCH|40/41 +14 excess, unresolved HI16|
| 80019BE4 |168|MATCH|41/42 +6 excess|
| 80018A30 |188|MATCH|46/47 +1 excess|
| 8001897C |180|14/45 +1 excess|38/45 +6 excess, unresolved HI16|
| 800202C4 |172|26/43|41/43 +3 excess|

All four selected O2 bodies have full relocated equality and zero nonzero
excess, unresolved symbols, unverified references or errors. Failed O1 controls
remain honestly recorded, including unpaired high relocations; they are not
alternative matches. No target boundary, scorer, relocation mask or tool changes.

## Actual source and ABI

- `1D5C0` takes one spatial-state pointer. It calls real three-vector cross
  product24D04 with output+48 and inputs+60/+36. A genuine12-float local assembles
  their three basis columns and the position at+12, then passes all48 meaningful
  bytes to real two-pointer24D74 with output at+72. Its72-byte native frame is
  reproduced without local padding. The caller contains no floating literal or
  invented float formal; both external math bodies remain untouched.
- `1B8C4` gates a key lookup through genuine1EDF4(u32), then walks packed416-byte
  records using low-byte indices. It sets flag8 only when the full stored ID at
  +96 agrees, but follows the next ID at+16 even after a stale ID. Result is0 if
  any record matched and-1 otherwise. Registered handles must select allocated
  records and the chain must terminate; malformed indices/cycles are not made
  safe by an invented guard. Reusing the genuine input key as the advancing ID
  and naming its real low-byte index reproduce the complete32-byte frame.
- `19BE4` conditionally resets packed words+140/+148 from words+144 and byte+193
  according to flag0x80000, mode byte+152 and flag0x2000. Regardless of that reset,
  a non255 channel at+74 calls20FDC with the actual three byte inputs: channel,
  set+75 and1. The callee's three byte homes/masks were audited.
- `18A30` consumes timed8-byte entries while their unsigned time is at most
  context high+half, stopping atFFFFFFFF. It forwards the value assigned to
  rate+292 to real19A60(u32,u8), using the low byte of the channel word. After the
  call it increments the cursor of the **current global context**, then rereads
  that context and cursor. A callback may replace the context; the source and
  tests preserve that behavior rather than retaining an obsolete local pointer.
  Native cursor+3948 is a32-bit pointer. Host tests validate semantics with wider
  pointers, not that native pointer-containing structure's host offsets.
- `1897C` is complete fixed-point rate arithmetic. It preserves a low32 left
  shift, signed division, low32 unsigned products, unsigned division by60 and
  logical shifts before splitting the result into fraction/whole/half fields.
  Signed conversion follows the N64 two's-complement convention. Valid division
  excludes zero denominator and INT_MIN/-1; native trap behavior is not replaced
  by a silent fallback. This natural source remains14/45 plus one excess word.
- `202C4` sets a real query flag, calls actual no-argument146AC, or accumulates
  real1467C(int) byte predicates over the live global channel count, resets the
  flag, and returns the inverse boolean. The canonical146AC body returns zero,
  but is declared rather than inlined or stubbed. The source preserves the whole
  native alternative branch. Narrow-byte copies and global address formation
  remain unmatched; no speculative volatile/global-type trick is introduced.

Packed and naturally aligned views model only genuine observed fields. Object
gaps are unexamined object bytes, not local padding. No extra formal, keeper,
forced register, inserted assembly or callee definition is present.

## Bounded diagnosis

All seeds use O2 first, then O1. `diagnosis.json` records the unchanged workbench
before refinements and at improved checkpoints; raw objects/native dumps are
not committed. Strict relocated scoring overrides raw relocation-layout counts.

`1D5C0` and `19BE4` match first form. For1B8C4, a genuine index local improves
36/41 to18/41 but creates a40-byte frame. Reusing the input as the advancing key
then closes the frame and registers: three compiled forms total. For18A30,
forwarding the assignment's value removes two spurious context/value reloads
and closes the initial23/47: two forms. For1897C, inlining the real quotient
improves27/45 to14/45; ordinary compound-arithmetic staging does not improve it,
so the simpler expression is retained: three forms. Reversing202C4's natural
predicate improves35/43 to26/43, then stops: two forms. No declaration, register,
K&R, volatile, padding or flag sweeps were pursued.

`verification.json` binds twelve final rows; `controls_verification.json` binds
fourteen rejected-control rows. Neither O1 duplicates nor controls receive body
credit. Source hashes, target manifest and exact compiler flags are retained.

## Tests and repository checks

Six strict-C89 ASan/UBSan tests check actual candidate bodies: all twelve matrix
entries and call ordering; finite chains containing stale/current IDs; reset
flag/mode combinations; timed cursor progression including global-context
replacement; independent wider-word fixed-point arithmetic; and live-count query
mutation. Helpers are synthetic contracts, not replacement native algorithms.
LeakSanitizer alone is disabled under ptrace; harnesses allocate no heap.

```sh
python3 cloud/work/boot_tail/BT03-high-runtime/verify.py
python3 cloud/work/boot_tail/BT03-high-runtime/verify_controls.py
python3 -m unittest discover -s cloud/work/boot_tail/BT03-high-runtime -p 'test_*.py' -v
```

Fresh manifests, all439 extents/99,120B and the existing getter pass. A new full
checkout exhausted /tmp before any candidate edits; only that incomplete new
checkout was removed and replaced by a sparse clone. Baseline-tracked lock/storage
proof dependencies were materialized unchanged so the normal161-lock guard runs;
no frozen checkout, protected source or Git object was pruned or edited.

Only four matching submissions and this owned packet change. Central STATUS/D10,
prior sources, targets/scorer, symbols/layout/locks, runtime-image/farm and banned
helper work remain unchanged. No ROM, raw assembly, objects, credentials or
private data are published. Independent source/ABI review and exact aggregate
CI precede checker-owned merging; local matches are not cartridge coverage.

Independent paired review PASS at source commit
`d193e055e26e5741d3d153306855911d41ac4fc0`, tree
`6e29861754e86d6dc0b05da4781abe0ad19662fe`. All twelve final and fourteen rejected
rows replayed independently, all six sanitizer tests passed, and complete
source/native/helper contracts were audited. The four selected O2 matches have
clean full-word relocation proof; rejected O1 failures remain explicit. Actual
matrix storage, stale-chain continuation and global-context replacement were
approved. `independent_review.json` binds exact source hashes; no source changes
followed that checkpoint.
