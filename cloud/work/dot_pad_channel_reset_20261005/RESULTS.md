# Player flag / pad-configuration callback

Status: **COMPLETE-NONMATCH**, not a match submission or coverage claim.
Baseline: `cf10b339`, target `audio_channel_reset`, `[0x80094FE8,0x8009508C)`.

## Result

The natural indexed scan in `group.c` compiles to the exact **164-byte ELF
function extent**, with **15 of 41 fully relocated words differing**. Its
opcode sequence and instruction schedule agree with the native body; the
remaining differences rotate the count/status, player cursor, and car-index
allocation. There are no unresolved, unverified, masked, or erroneous
relocations. Both real context functions remain strict matches with exact ELF
extents: `Input_ApplyPadConfig` (192 bytes) and `Input_InitPadHandlers` (64 bytes).

The unchanged accepted source is a literal prefix of this packet's source.
`verify.py` checks that relationship, compiler fingerprints, actual ELF symbol
sizes, and every relocated instruction across each complete function extent.
The canonical scorer is unchanged and also records 15/41 for the target.
No production sources, locks, symbols, flags, protected targets, or scorer
were changed. No ROM/image acceptance was attempted.

## Native contract and source provenance

The historical audio name is misleading. The function starts an aggregate
boolean at one, scans every player when the signed-half count is positive,
and clears the aggregate when a selected car has a zero signed-byte flag.
A changed aggregate replaces the signed byte at Sprite offset 26 and invokes
`Input_ApplyPadConfig` with the same pointer. It always returns one. The loop
has no early exit after finding a zero.

Recovered layout facts are limited to the accesses: 76-byte player records
with an unsigned car index at offset 1, and 772-byte car records with a signed
flag at offset 6. The complete Sprite declaration comes from the accepted
`src/blob/groups/frontier_pad_config/group.c`. These are ordinary ABI calls;
no values survive the callee call. The protected image has no direct `jal`
callers for this callback; this does not rule out data-driven callers.
The arcade reference checkout was absent, so no arcade donor is claimed.

## Bounded experiment record

`controls.json` contains 33 local compile controls, including the two old
baseline flagsets. The main sequence was:

1. Historical pointer/count source: 39/41 at both O2 and O3. Its real ELF size
   is **168 bytes**, not 164; a trailing zero instruction is invisible to the
   scorer's extra-nonzero count. This corrects the historical exact-extent
   description without altering that source or the scorer.
2. Exact accepted Sprite type plus unchanged real O3 callee context does not
   improve the historical body. Both context functions remain strict matches.
3. An indexed scan recovers 164 bytes and the native schedule: 27/41. Naming
   the actual car-index load reaches 26/41; naming the actual signed-flag load
   reaches 15/41. These locals correspond to real native loads, not dummy
   pressure or extra accesses.
4. Workbench `diagnose` confirms the allocation residual. Its relocation
   caution is expected because the reconstructed private native object uses
   absolute words; the full relocated verifier is authoritative.
5. Natural pointer/count, loop, local-scope, condition, and narrow-type forms
   do not improve 15/41. A cached count causes real loop unrolling; it is not
   retained. No source-layout search, fake arguments, dead keepers, extra
   accesses, padded body, inline assembly, or stand-in function was used.

Stop here until new genuine source/compilation evidence explains the
three-web priority order. An instrumented allocator trace could establish
that mechanism; blind syntax or physical-layout permutations are not a
justified next step.

## Reproduce

With the pinned IDO/toolchain environment available:

```sh
python3 cloud/work/dot_pad_channel_reset_20261005/verify.py
python3 tools/cloud/score.py group cloud/work/dot_pad_channel_reset_20261005
python3 -m pytest -q tests/cloud/test_dot_pad_channel_reset.py
```

The canonical group command exits **1**, intentionally: its sole member is a
nonmatch. `claims` is empty. The verifier exits 0 only for the recorded
nonmatch and unchanged strict context, not for a promoted/matching status.

Behavior tests pass: 192 directed cases plus 1,000 deterministic randomized
cases and their idempotent repeat calls. Coverage includes negative/zero
counts, maximum signed-half count, signed/noncanonical flag values, unchanged
versus changed configuration, original pointer/field preservation, actual
callee record updates, callback arguments, and callback ordering. Host
behavior tests are separate evidence and are not native matching proof.
