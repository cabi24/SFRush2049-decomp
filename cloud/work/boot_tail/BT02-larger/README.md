# BT02 larger: transfers, startup and audio state

Five strict relocated matches, **1,084 native bytes**, plus one complete nonmatch,
**208 bytes**. All five matches use `-g0 -O2 -mips2 -G 0 -non_shared`, with the
mandatory `-Wab,-r4300_mul`. Four matched their first natural forms; the list helper
needed two directed variants after diagnosis. All fixed O1 controls differ.
Pinned-input replay and actual-source C89/ASan/UBSan host checks pass. Paired review
and exact aggregate-head CI are still required. This is matching-source evidence,
not cartridge coverage, promotion, merge or maintainer acceptance.

## Claim and provenance

- Packet BT02-larger; branch `dot/boot-tail-bt02-larger`.
- Fresh master base `301d9e7552ad4fd7f54a38796db84671e1000d35`.
- Parent activation and central claim `b34484c9` precede source edits.
- Exact six starts: `80010628` (236 B), `80010714` (192), `80010C68` (212),
  `80011910` (208), `80012660` (208), `80013C84` (236), total **1,292 B**.
- Edit scope is these five matching files and this packet directory only.
  Central STATUS, claims and D10 belong to the aggregate lead. No queued packet.
- Packet 1 PR #59 at `21e104a22575cf4d639261d2f6d9535913074e6a` and Packet 2
  checkpoint `76780b3a1b3e26c54b86e1f153344e92dac15b20` are read-only provenance,
  not merged ancestors. Earlier BT02/BT07 packets are read-only ABI context and
  receive no duplicate credit here.
- The existing approved IDO installation is reused; every file hash equals
  Packet 2. No compiler installation or setup modification was needed.

## Reproduction

Set `IDO_DIR` to the existing pinned IDO installation, then run from repository root:

```sh
python3 cloud/work/boot_tail/BT02-larger/verify.py
python3 cloud/work/boot_tail/BT02-larger/test_host.py
```

The verifier checks protected scorer/setup/inventory/getter hashes, all installed
compiler-file digests, all three target-manifest members, all 439 inventory/extent
starts and sizes (99,120 B), and the original 12-byte getter. It strictly replays
all six actual final sources at O2 then O1, reproduces the rejected baseline
controls, and hashes the sources and harness. Every match has zero differing
words, excess nonzero words, unresolved symbols, unverified relocations and
errors. No local literal pool or table is required. `verification.json` binds the
results to source hashes; `status_delta.json` is for central application only.

## Actual ABI, behavior and types

| Function | Native-supported contract |
|---|---|
| `80010628` | Three real arguments: destination, source, unsigned byte count. Acquire via `80014594`, initialize via `800105C4`, then if volatile count `80037FA0` is below four, fill one 12-byte request, rounding the stored size up to 16 bytes, increment the count, and invalidate destination cache for the **original** size. Release via `800145DC`, block on queue `80037FE0`, then call `80010110`. Full queue only releases. No return is consumed. |
| `80010714` | Same request creation and cache invalidation, followed by release, without the wait/helper. Prior matched caller `8002506C` forwards destination, source and count in that order. |
| `80010C68` | Pointer to unsigned frequency. Set message byte `80038220`, initialize four-message queue `800381F8`, register event 6, adjust frequency with `osAiSetFrequency`, write both global and caller output, allocate a 1,024-byte stack with alignment argument 128, create `80010A40` thread at priority 122, and start it. Thread ID and argument are zero. |
| `80012660` | Return a doubly linked node. Prefer pop from free head `8003835C`, then attach to active head/tail `80038354/58`; when no free node, rotate active head to tail. One genuine unused 32-bit argument is homed natively. Actual caller `80012730` reloads its fourth word into a0 before the call and then stores that word into the returned node. This is not a fabricated extra formal. |
| `80013C84` | No arguments. Walk `8003829C` unsigned-16 records at stride 104; for each active byte, copy current unsigned word at 0x10 to previous at 0x14, then truncate double position at 0x08 to unsigned word at 0x10. Native FCSR save/rounding/exception/restore sequence is ordinary IDO unsigned conversion, not hand-written-assembly evidence. |

Both transfer bodies save all three actual incoming arguments. Caller `80025AB4`
uses a returned allocation as destination, an address-valued source, and byte
lengths, then consumes the copied bytes after the synchronous helper returns.
The lock/unlock helpers consume no incoming arguments. `800143C0` supplies the
frequency pointer to `80010C68`; native `80010A40` follows the one-pointer thread
entry ABI. Every external/indirect callee is declared, never stubbed into a match.

The queue and thread forward tags and API types follow the existing headers.
`osSetEventMesgAlt` is specifically the native call at `80006E10`, distinct from the
other symbol at `80006A30`. Global buffers remain opaque byte-array storage views.
The allocator receives exactly two words and returns the stored pointer.

`AudioState` retains the prior BT02 field offsets through 0x67 and refines the
formerly unknown 0x08/0x10/0x14 bytes to observed double/word fields. Unknown bytes
are actual record storage, not stack padding. Host assertions confirm its
104-byte size and key offsets. The node declaration exposes only its first two
links; no unknown payload is touched. Host pointer size differs from O32, so node
and request host tests validate behavior, while strict native scoring proves the
native 4-byte link and 12-byte request layout. No shared header changes are made.

## Flags and diagnosis

| Function | Final O2 diff/words | Fixed O1 diff/words | O1 excess nonzero words |
|---|---:|---:|---:|
| 80010628 | 0/59 | 57/59 | 7 |
| 80010714 | 0/48 | 46/48 | 9 |
| 80010C68 | 0/53 | 32/53 | 1 |
| 80011910 (NONMATCH) | 47/52 | 50/52 | 6 |
| 80012660 | 0/52 | 52/52 | 20 |
| 80013C84 | 0/59 | 58/59 | 13 |

Native CSE, branch-likely selection, frameless list/record loops, and scheduling
support O2. The overlong O1 controls for 10714 and 12660 also leave unpaired HI16s
at the native comparison boundary; their paired LO16s lie beyond it. They are
rejected controls, never accepted relocations or a local-data proof claim.

Initial 12660 was 33/52, with 53 generated instructions versus 52 native.
Unmodified workbench diagnosis showed alias-sensitive control-flow and scheduling.
Testing the node's just-assigned next field instead of rereading the global head
removed a load/branch difference and reached 2/52. A second diagnosis showed the
remaining two instructions were a tail-pointer load and next-link clear. Writing
the previous link before clearing next follows native dependence order and
matches. Two follow-up source variants; no fake formals, keepers or padding.

`nonmatch/func_80011910.c` is **COMPLETE-NONMATCH**, 47/52 at O2, 50/52 at O1
with six excess words. The complete body conditionally fills the audio task's
type, flags, boot/code/data addresses and sizes, command pointer/count; writes
back `count * 2576` bytes; calls the task callback; then sets the volatile active
byte. The unsigned-16 count is confirmed by caller `800119E0`. The fixed
microcode/data symbols are used only as addresses, including the boot end/start
subtraction. No bytes beyond `800277D0` were inspected or reconstructed.

Diagnosis found 52 native instructions versus 49 generated, same 24-byte frame.
The conventional whole-record source hoists one task base; native independently
materializes high halves for field pairs. Four bounded follow-up views (aligned
union, volatile record, independent scalar fields and paired region records)
did not close it. Scalar views reached 43/52 but added four excess words; they
were not retained. The archived full record is the natural complete baseline.
Next hypothesis: original task-storage declaration or independently demonstrated
IDO aggregate-lowering evidence, before another search. No literal-address
forcing, scorer change or target edit was used. `diagnosis.json` records all
measured forms; native/object diagnostics stay in temporary scratch only.

## Host verification and limits

The harness links the five actual sources as separate translation units, with
callback/API doubles only in the host test. Strict C89 O2 and ASan/UBSan O1 pass:

- 72 transfer cases: accepted/full/max counters, both variants, zero/unaligned/
  aligned/max byte counts, unsigned rounding wrap, exact call order, original
  invalidation length, blocking queue arguments, ignored queue result and
  neighboring request preservation;
- four thread-startup cases, including maximum frequency, with every queue/event/
  allocator/thread argument and output-write timing checked;
- free-pop, empty/nonnull active-head with null tail, append and nonempty active
  rotation, preserving native untouched previous links;
- inactive/active records, truncation below/above the signed boundary, maximum
  representable unsigned result, zero and 65,535 record counts, exact offsets and
  unchanged neighboring bytes.

Floating-to-unsigned host cases stay within the defined finite range; they do not
claim host equivalence for NaN, infinity, negative or overflowing input. Native
strict equality includes the full IDO conversion exception paths. Tests do not
emulate N64 hardware or concurrency and do not prove native queue scheduling,
allocation failure safety or list invariants. Rotation assumes the native-required
nonnull active head, successor and tail. Leak detection is disabled for the known
ptrace runtime restriction; the driver allocates no heap objects.

No third-party source was copied. No ROM, raw disassembly, object, credential or
unrelated private data is committed. Targets, scorer, symbols, locks, layout,
shared types, runtime images, farm configuration, D10 and production gates remain
untouched. Publication is through the central aggregate; merging and cartridge
integration remain with the independent checker.
