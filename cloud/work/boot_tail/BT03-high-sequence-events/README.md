# Recovered sequence event dispatcher research

`func_80017D38` remains **COMPLETE-NONMATCH, 872 native bytes, zero matching
credit**. Fresh pinned IDO replay gives O2 **173/218 differing words plus two
nonzero excess words**, with an 880-byte ELF function. O1 gives **217/218 plus
96 excess**, with a 1,276-byte function and 1,280-byte padded text section.
Both complete text sections relocate without masks, unresolved/unverified
references or errors. Neither compiler row is accepted.

This recovery branch starts at published PR74 checkpoint
`496a72b0edd683bd7d46e8c21f8323ab43629494`, tree
`b116b8282af4c082fa699b5186d3505292aa3b82`. The parent reassigned this packet
exclusively to `verify_boot_tail_matches` after the filesystem reset. Changes
remain confined to this packet; the central integrator owns status and claims.

## Recovery and current verification scope

The complete retained C was recovered from the independent reviewer's complete
visible tool output. Its SHA256 is exactly the pre-reset value
`ad5137579da173b15c903c8aa35c69cd26416d51def3b65267045062543bff65`.
No source refinement or additional compiler experiment was performed.

The complete 90-line semantic test and ABI/preflight scripts were also
transcribed from visible output. Their original individual hashes were not
printed, so original-hash equality is not claimed. The recovered test is now
freshly verified and bound to SHA256
`52f4f790b12cd27558a6cea5d7d9af6becff7133f9101933a3eb6ea99e77200e`.
The fresh verifier and supplemental runner explicitly pin these recovered inputs.

The original reviewed commit `ca6a32ca2497c95804adabd7686d73e7e295887b`, tree
`ae184b11c16f57fbb165721cd2cf7180867f3567`, and old review receipt bytes are
unavailable. Their identifiers remain historical provenance; they are not
claimed as recovered Git objects. `recovery/transcription_manifest.json`
describes the transcript-only recovery before fresh testing. Its statements
that fresh tests had not yet run refer to that earlier recovery stage.

Only the retained source's O2/O1 rows are freshly replayed here. The former
four-form/eight-row control archive is **historical evidence only**: three
rejected source bodies and the complete control receipts were not recoverable.
The whole-timestamp control was historically identical to the retained source,
but no missing archive is recreated. The old control verifier is deliberately
absent from the fresh packet. `recovery/historical_review_evidence.json` and
`recovery/historical_diagnosis.json` preserve limited historical results without
claiming they can be replayed. No old object, raw native listing or ROM data is
included. One future hypothesis would require authentic source context for the
remaining scheduling/frame/coalescing differences; repeated declaration sweeps
are not justified by the current evidence.

## Whole-body interface and native behavior

The genuine interface is `u8 func_80017D38(void)`. Complete caller 198C8 invokes
it without arguments and consumes the byte active result. The result records
that a stream existed when its track was visited, including a stream whose
terminator this call consumes; it is not a final stream count.

The dispatcher visits 64 tracks through a live external context. Native tracks
are 40 bytes: fraction/whole timestamp +0/+4, event time +8, event cursor +12,
two other cursor/value/time pairs and channel/signed offsets/track number +36..39.
The context is 4,088 bytes: enabled masks +272, lookahead +288, 64 group bytes
+1320, tracks +1384, 16 program words +3968, a 40-byte request +4040, result
pointer +4080 and pending byte +4084. The request matches the reviewed 19490
interface. IDO asserts every used native offset and whole-object size; host
LP64 pointer-bearing layouts are not substituted for N64 layouts.

Events have aligned halfword time deltas and key/velocity bytes. Commands and
zero/zero no-ops advance four bytes; note attempts advance six even if disabled,
unmapped or allocation fails. FF/FF clears the cursor without advancing it.
Unsigned due, lookahead and note-end additions wrap at 32 bits. Future events
leave the event time and cursor unchanged. The captured due value survives
helper calls; live context, cursor, duration and timestamp reloads follow the
native body.

High-key velocity 0/1 calls the program/controller helpers. Two high bytes
select a controller or control 104's pending request. High keys with other low
velocities still follow the native note path; newer family command rejection
is not imported. Pending dispatch passes the embedded request, result pointer
and byte1, then clears the current context's pending byte after the call.

Notes use channel 0..15 and track number 0..63 for the program, enable and group
maps. Signed-byte note and velocity adjustments clamp to 0..127. Allocation
returns a real 24-byte linked note. A270 consumes ten genuine inputs:
`u32,u8,u8,u8,u8,u8,u16,u16,u8,s16`. Zero is a successful identifier;
only FFFFFFFF invokes the one-pointer free helper. The program, channel, track
number and due time are captured at native points. Current globals and group
mapping are read after allocation; current duration and the whole two-word
timestamp are read after construction.

All seven callees (173B4,17410,17720,177EC,19490,20610,A270), caller 198C8 and
initializer 178B0 were fully inspected during the independent pre-reset review.
Fresh ABI verification proves all ten canonical native inputs are byte-identical
to those reviewed targets. The larger initializer remains a read-only layout
reference. No callee definition is reconstructed in this packet.

D_8004BE7B is the big-endian low byte of the word at D_8004BE78. Host fixtures
keep separate stand-in globals coherent; they do not assert native linker
placement. Pinned public CC0 `seq.c` is only a family lead. Its newer architecture,
additional commands and expanded constructor differ. Exact provenance is in
`references.json`; no original external data values are invented.

## Bounded tests and replay

Fresh actual-source C89 ASan/UBSan runs pass **134,531 calls**. The recovered
O1 harness checks 65,536 key/velocity pairs, 3,200 live-context/failure/timing
cases, 192 multi-event cases and three active/future checks (68,931 total).
The recovered supplemental O2 runner adds all 65,536 signed-offset pairs with
direct clamp assertions and 64 simultaneous four-track cases (65,600 total).
Tests compare complete contexts, node storage, result words, helper ordering,
all ten constructor arguments and live globals against a separate bounded model.
Only LeakSanitizer is disabled. Helpers are interface/effect fixtures.

The domain requires finite, aligned, sufficiently allocated u16-backed streams;
complete reached headers and note durations; valid whole contexts and nodes;
channel/track bounds; and helper changes that preserve valid pointers and eventual
bounded traversal. Partial-object aliases, malformed/cyclic streams, concurrent
mutation and universal downstream safety are not claimed. Whole timestamp copy
is a real object copy within this domain. Bounded regression agreement and strict
binary matching are distinct; these tests earn no matching credit.

From repository root, with the pinned IDO toolchain available:

```
python3 cloud/work/boot_tail/BT03-high-sequence-events/verify.py --check
python3 cloud/work/boot_tail/BT03-high-sequence-events/abi_proof.py --check
python3 cloud/work/boot_tail/BT03-high-sequence-events/verify_semantics.py --check
```

`preflight.py` reruns setup, all target-manifest checks, the 439-function / 99,120 B
census and the existing strict getter match. Fresh preflight receipts are included.
The changed-submission gate must find zero new matching sources. Protected paths,
static locks and whitespace are checked before handoff. Independent review of
source commit `a675c9e7e7cf96def91c728b389e60da1fbbbd46` passed with no blocking
findings. `REVIEW.json` binds that source tree, both retained compiler rows, all
134,531 sanitizer calls and the complete native/interface audit. This final
receipt update changes documentation and metadata only. Central owns publication
and hosted CI; the independent checker alone merges. No cartridge coverage is
claimed.
