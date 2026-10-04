# BT07 decoder-facing caller pair

**One local strict matching caller, 604 bytes. One COMPLETE-NONMATCH caller,
840 bytes, with a two-word scheduling residual.** No decoder implementation,
cartridge coverage, promotion or production-runtime claim is made.

## Scope and recovery

Exact central activation `bd452b11`: only `8002574C–800259A8` (604 B) and
`80025F74–800262BC` (840 B), packet `BT07-decoder-facing-callers`. The coordinator
owns central status, claims, aggregate bookkeeping and publication. This packet
contains only the two sources and their local tests/research.

The recovered clean base is `496a72b0edd683bd7d46e8c21f8323ab43629494`, on
branch `dot/boot-tail-bt07-decoder-facing-callers-recovered`.
The original clean base was `eae0295b40271c4758c7a3ecb890e48192487f8c`. A filesystem
reset around 2026-10-04 04:32 UTC removed that uncommitted worktree and its receipts.
The final sources and initial controls were recreated from retained authored
command text. Fresh replay against restored PR #74 reproduced every final/initial
O2/O1 result and both exact native ELF extents. `recovery_status.md` distinguishes
historical observations from fresh evidence. The native inputs, scorer and all 24
approved IDO files remain the pinned inputs from the committed stream-pair packet.
The complete caller ABI had been closed by a separate read-only audit before any
implementation; `reconstruction.md` preserves its source-free conclusions.

## Results and exact extents

| Caller | O2 | O1 negative control | O2 ELF STT_FUNC extent |
|---|---|---|---|
| `8002574C` | strict MATCH, 0/151 differing, no extras | 150/151, 166 nonzero extras | 604 B |
| `80025F74` | COMPLETE-NONMATCH, 2/210 differing, no extras | 209/210, 54 nonzero extras | 840 B |

The `.text` sections are 608/848 bytes because of zero assembler alignment; the
actual function sizes are exactly 604/840. Padding never substitutes for a missing
function word. O2 has no masked, unresolved or unverified relocations and no errors.
The overlong 2574C O1 comparison window ends on an unpaired HI16, but independent
relocation over its complete 1,304-byte function extent is clean. The 25F74 O1
function is 1,092 bytes. Neither O1 control is a match or extent-equivalent source.

Build flags are `-g0 -O2/-O1 -mips2 -G 0 -non_shared`, plus the scorer's mandatory
`-Wab,-r4300_mul`. Native branch-likely loops, CSE and exact 96/40-byte frames support
O2; O1 expands substantially. Local strict equality still requires independent
review and exact aggregate-head CI before integrated verified-body credit.

## Complete caller contracts

`8002574C` is the registered no-input callback. It acquires/releases the existing
queue and visits two 4,648-byte records. Direct global indexing becomes the native
record-pointer induction. Every busy record invokes `80025F74`, then reloads busy
and state. Startup passes ten real inputs to `8001C580`, retaining the reviewed
byte-request/four-byte-setting prototype and genuine five-input callback. Failure,
conditional release, busy/state transitions and post-stop countdown reload are
preserved. All globals and calls resolve from the protected symbols or unchanged
address-named fallback. No local literal or switch table is invented.

`80025F74` takes exactly one stream pointer. It polls the queue, reads the initial
header, advances unsigned input counters, requests bounded four-input transfers,
invokes the external two-pointer decoder, updates the caller's counters/state,
zero-fills exhausted streams and makes the final buffering transition. The decoder
is declared `int (StreamState *, void *)`; its ignored result does not imply void.
Its first 400 state bytes stay opaque and no decoder/helper body is defined.

Both source views preserve the established 4,648-byte native layout, signed
pending/state/budget bytes, unsigned counters/halfwords, real queue storage and
actual helper reloads. Output addresses use byte-pointer arithmetic for
`8 * ((available % capacity) / 4)`, avoiding a floating-element-type assertion.
No production header, target, scorer, shared layout, lock or denominator changes.

## Bounded reconstruction

Nine directed source forms were tested for 2574C and twelve for 25F74, with O2/O1
controls. `experiments.json` preserves numerical observations recovered from the
original tool output; initial/final sources are the independently replayable forms.
The initial complete bodies differed by 15/151 and 92/210 words. Workbench diagnosis
ran before refinements and again at the 25F74 two-word plateau.

For 2574C, joining stream initialization to the loop header removed a preheader
schedule difference. Replacing the entire pointer loop with direct record indexing
removed outgoing-stack argument load/store scheduling differences and matched.
This preserved the exact actual prototype and did not introduce parameter locals,
extra formals or padding. Explicit parameter-value locals regressed. Word-width and
unprototyped external declarations were inert diagnostics, rejected and not retained.

For 25F74, explicit high-halfword masking, a one-line callback expression and the
native nonpositive halfword test left just an adjacent copy/store ordering swap at
+0x2F4/+0x2F8. Unmodified `as1 -R` trace inspection found the source-line/dependency
schedule. Natural same-line update/decrement equalized line keys without changing
ready-list order. Explicit cursors/loop operands and a split budget guard regressed;
assignment, for-loop, comma-step and byte-output-offset forms did not close the swap.
The clear byte-offset form is retained as COMPLETE-NONMATCH with zero matching credit.
A useful next step needs an authentic source/loop-form lead or measured explanation
for that exact scheduler pair, rather than more blind variants or ABI guesses.

## Verification and limits

From the packet's restored repository, with the approved compiler as `IDO_DIR`:

```sh
python3 cloud/work/boot_tail/BT07-decoder-facing-callers/verify.py
python3 cloud/work/boot_tail/BT07-decoder-facing-callers/test_semantics.py
```

The verifier checks unchanged inputs, all 439 extents/99,120 bytes, the getter
control, exact target/source hashes, full-function relocations and actual ELF
extents. It strictly replays final and initial O2/O1 controls. The semantic harness
uses actual C sources and explicit external contracts. It passes 8,964 scenarios
per mode (4,051 dispatcher and 4,913 stream) in C89 O2 and ASan/UBSan O1, plus
complete separate 32-bit layout assertions for both sources; its receipt details
coverage. Tests require correctly allocated aligned storage, valid callback/queue
lifetimes, positive sufficient capacity, valid ring/output positions and ordinary
finite positive rate. Native unsigned wrap, signed budget and post-helper reloads
are retained. Zero capacity still has the native modulus-trap domain boundary.

Host tests do not execute native targets or establish malformed-input, underflowed
occupancy, invalid-float, asynchronous-race, decoder-memory-safety or hardware claims.
No ROM, raw disassembly, objects, compiler outputs, credentials or unrelated private
data are included. Decoder reconstruction, T050, excluded destinations, bytes at or
past 800277D0, and 800D1248/helper work remain untouched.
