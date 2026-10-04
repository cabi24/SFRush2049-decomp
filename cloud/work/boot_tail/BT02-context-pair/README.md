# BT02 context pair: one complete match and one bounded nonmatch

## Result and scope

- `func_80010A40`: strict whole-body relocated O2 MATCH, **552 bytes / 138 words**.
- `func_80010E80`: COMPLETE-NONMATCH, **53/125 O2 differing words**, no excess words
  and no unresolved/unverified references. All 500 native bytes are reconstructed;
  **zero match credit** is claimed for this archive.
- The parent independently reviewed the 552-byte source and replayed all original
checks at `a5a4017d` with PASS; see `parent_review.json`. The separate
BT03 low-larger peer also reviewed the entire 500-byte archive, native callers,
ABI, layout and semantics at `853db8ea` with PASS-COMPLETE-NONMATCH and zero
matching credit; see `independent_review.json`. The added test-only
cases have also passed locally. Exact aggregate-head CI remains required. This is body
  proof and research only, with no cartridge coverage, promotion, or acceptance.

The exclusive claim was acknowledged at central commit `425bfa4d`, against master
`301d9e7552ad4fd7f54a38796db84671e1000d35`. Exact disjoint ranges are
`[0x80010A40,0x80010C68)` and `[0x80010E80,0x80011074)`. The packet changes only
its own research directory and the matching `80010A40` source. Central STATUS/D10
edits and aggregate publication remain with the coordinator. No context body,
target, scorer, shared type, lock, layout, farm, or other runtime image is edited.

## Reproducible verification

Use the already installed pinned IDO via `IDO_DIR`, without copying its directory:

```sh
IDO_DIR=/path/to/pinned/ido python3 cloud/work/boot_tail/BT02-context-pair/verify.py
python3 cloud/work/boot_tail/BT02-context-pair/test_host.py
```

`input_pins.json` preserves the original Packet 2 hashes for the installed compiler,
scorer, setup, inventory, manifest, extents, and existing getter. The verifier
checks every installed compiler file, all three target-manifest members, exactly
439 matching inventory/extent starts and sizes totaling 99,120 bytes, and a fresh
strict `80010A00` getter MATCH. All 26 recorded compiler controls are reproduced.
No local .rodata or switch-table placement is required by either function.

Both initial complete bodies were compiled O2 then O1. The native branch-likely
loops, propagated ring pointer, and preserved single state pointer favor O2.
Final `80010A40` O1 has 138/138 differences and four excess nonzero words; final
`80010E80` O1 has 121/125 differences and 17 excess nonzero words. Those failures
are explicit controls, never acceptance evidence. All compiles retain the
scorer's mandatory `-Wab,-r4300_mul` behavior.

## Actual ABI and semantics

### `80010A40`: audio-buffer ring thread

`80010C68` passes this address as the entry function to `osCreateThread`, with the
normal one-pointer thread ABI. The native entry homes its real a0 argument even
though its body does not consume it. The source therefore retains `void *argument`.
The prior `BT02-larger` packet independently establishes the same thread contract.

The native thread selects 24 or 16 buffers from byte `80038290`, halves that count
when unsigned frequency `8003828C` is at most 22050, allocates `count * 768` bytes
with alignment argument 128, and fills the actual pointer table at `80038228`.
It zeros and writes back the allocation, clears the shared byte ring index, and
submits the first 768-byte buffer. It then blocks on message queue `800381F8`.
When byte `80038291` is clear, message command 1 advances the ring modulo the live
unsigned-halfword count and submits its selected buffer; command 255 stops the
thread; other commands are ignored. While disabled, even a stop command is
ignored. Native return values from message receive and buffer submission are
ignored, and no allocation-failure guard is invented.

The ring byte is declared volatile because its native store is followed by an
actual reload, and separate callback `80013DEC` also reads it while coordinating
interrupt state. The qualifier preserves this observed shared-state access;
no local is made volatile. The consumer is read-only context, not included or
recompiled as a submission helper. `80010D74` supplies the one-byte stop command,
consistent with this receiver's byte-only load.

Declared-only static helpers resolve to `osRecvMesg` at `80007270`,
`osWritebackDCache` at `80007CA0`, `bzero` at `80008590`, and `osAiSetNextBuffer`
at `8000BE70`. Their pointer/length/message signatures agree with their canonical
static bodies and existing reviewed caller declarations. The allocator callback
uses the already established `(unsigned int bytes, unsigned int alignment)` ABI.

### `80010E80`: delay-state configuration, complete nonmatch

Do not infer this interface from the historical `__d_to_ll` census label for
excluded wrapper `80014BD8`. That wrapper forwards a real input pointer to this
function, which dereferences its unsigned halfword at offset zero. Reviewed
`80020528` likewise forwards that pointer; its earlier no-argument score-only
candidate was correctly rejected in `BT03-high-next/README.md`.

The input view contains duration at 0, eight halfword delays at 2, eight byte gains
at 18, count at 26, feedback at 27, mode at 28, and four signed filter halfwords at
30. The 72-byte allocated output view contains buffer pointer 0, length 4, count 8,
feedback 10, delay words 12, gain halfwords 44, untouched word 60, and filter
halfwords 64. These are local observed layout views, not asserted original
middleware names or a shared-header decision. IDO compile-only assertions prove
all accessed native offsets, 32-bit pointers, and the 72-byte output extent.

When `800382F0` is null, the function returns before dereferencing its argument.
Otherwise it computes the unsigned wrapped duration/frequency product, divides
by 1000, and rounds to the next 192-sample boundary, including adding 192 when
already aligned. It allocates the real 72-byte state, clears/writes back twice the
sample count in the existing buffer, fills the delay/gain arrays using the actual
byte count, and either copies four signed filter terms when mode is 1 or writes
`0,0,0,32767`. It writes back the state, enters real synchronization helper
`80014594`, clears `800382EC`, publishes its pointer at `800382E8`, and calls
`800145DC`. Both helper definitions are reviewed read-only context.

The native delay expression masks with **0xFFFC**, deliberately truncating high
bits as well as aligning; it is not changed to a full-word mask. The input count
has no native bounds check. Host tests use the structurally evidenced 0–8 range
and do not add a runtime check. Unused record space and array entries remain
untouched, matching the native stores.

## Source controls and stop decision

`80010A40` used seven natural source forms. A signed setup index recovers the native
pointer induction loop. The shared volatile ring byte restores actual reloads.
The unsigned stop flag keeps native constant-one spelling distinct from signed
SDK flags. Putting the real message local first restores its actual stack home.
Final API-type review uses an SDK-native `void *` message local and casts only
its byte dereference, avoiding a pointer-to-pointer aliasing cast while retaining
strict equality. No local, formal, operation, padding, or stub was added to
influence allocation.
The final exact 104-byte frame, every home/displacement, and all 138 code words
are included in strict equality.

`80010E80` used 17 natural source forms, below the 20-variant session bound.
Its initial rounding expression folded differently; left-associated genuine
rounding restores the native unsigned division/check form. Separating the consumed
raw-sample scalar from rounded length and reusing that scalar for byte count and
then loop index removes the extra saved register and reaches 53 differences at
the exact extent. Declaration-order, signed/unsigned/long spelling, genuine
byte-count capture, expression-CSE and branch-scope controls did not close the
remaining register/temporary-web and scheduling gap. All controls, including a
rejected excess-body variant, remain labeled and reproduce their actual results.

Workbench diagnosis ran on fully relocated private objects before refinements,
at the four-word thread near-match, and on the final configuration archive.
`diagnosis.json` records compact measurements without publishing disassembly.
The next useful hypothesis is a source-backed explanation of the original
unrounded/rounded length lifetime and loop-index web. Do not reopen by adding
padding, redundant keepers, invented formals, or fake caller/callee context.

## Tests

- Nine ring-thread scenarios cover both count modes, threshold values 0, 22050,
  22051 and UINT_MAX, two full ring wraps, disabled stop, unknown command, stop,
  and ignored SDK failure return. The additional test changes the live count
  through 1, 24 and 5 while receiving messages, and passes a null message while
  disabled to verify guard-before-dereference behavior.
- 324 configuration scenarios cover all counts 0–8, all three mode branches,
  duration 0/1/65535, frequencies including UINT_MAX with 32-bit product wrap,
  gains, signed filter extremes, untouched entries and synchronization/publication
  order. An additional disabled-buffer/null-input test proves guard order.
- Test-only SDK/callback doubles are compiled separately from submission sources.
  Host tests establish source behavior, not native layout or byte equality;
  separate IDO layout checks and strict target comparison establish those facts.

No ROM bytes, raw assembly dumps, object files, credentials, or unrelated private
material are included. No third-party implementation is vendored; reconstruction
uses only canonical repository target bodies and reviewed local ABI context.
