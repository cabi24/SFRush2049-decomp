# BT02 follow-on: setup, activation and stop helpers

Six strict relocated matches, **548 native bytes**, at
`-g0 -O2 -mips2 -G 0 -non_shared` plus mandatory `-Wab,-r4300_mul`.
All fixed O1 controls differ. Four first-form sources matched; two required seven
bounded, directed follow-up variants after workbench diagnosis. Final matching
sources are self-contained natural C89. This is source matching only; paired
review and exact aggregate-head CI remain pending. No cartridge coverage,
promotion, merge, or maintainer acceptance is claimed.

## Claim and provenance

- Packet BT02-followon; branch `dot/boot-tail-p4-bt02-followon`.
- Fresh master base `301d9e7552ad4fd7f54a38796db84671e1000d35`.
- Exact central claim acknowledged at `8bdd0699` before source edits: `800105C4`
  (88 B), `80010D74` (100), `80011C84` (84), `80014374` (76), `800143C0` (116),
  `80014434` (84). These were open and disjoint from previous BT02 packets.
- Prior BT02-small, BT02-medium and BT02-remaining sources are read-only ABI
  context; none is repeated or credited. Central lead owns STATUS and claims.
- Read fresh spec015, inventory, tasks, repository guidance, central status and
  current open PRs. PRs 61 and 62 are prior green cuts; PR63 is a separate pending
  third cut. This packet is not a separate PR and does not modify their sources.
- Pinned input hashes come from Packet 2 `76780b3a`; the existing approved IDO
  installation is reused, with every compiler-file digest checked. No setup or
  tool installation is performed.

## Reproduction

Point `IDO_DIR` at the existing approved IDO installation, then run from the root:

```sh
python3 cloud/work/boot_tail/BT02-followon/verify.py
python3 cloud/work/boot_tail/BT02-followon/test_host.py
```

The verifier checks pinned scorer/setup/inventory/getter identities, every compiler
file, all target SHA256SUMS members, exact equality of 439 starts/sizes (99,120 B)
and the historical 12-byte getter. All final sources have zero differing words,
nonzero excess words, unresolved symbols, unverified relocations and errors.
The verifier separately reproduces both initial rejected controls; they receive
no credit. Final sources, controls and test harness have hash-bound receipts.

## Actual ABI, behavior and types

| Body | Observed contract |
|---|---|
| `800105C4` | No arguments, unused return. If callback pointer `80038020` is null, initialize queue `80037FE0` using message storage `80037FF8`, capacity 1; clear volatile byte `80037FA0`; assign actual callback `80010450`. The callback is only named here, not defined or investigated beyond its ABI and target address. |
| `80010D74` | No arguments, unused return. Initialize a one-byte command array with 255; pass its address to **osJamMesg** (`800075E0`), queue `800381F8`, blocking flag 1. Invoke one-pointer release callback `8003801C` first with `80038228`, then reread callback and pass `800381F0`. API and callback results are ignored; stored pointers are not cleared. |
| `80011C84` | Unsigned 16-bit record index, genuine incoming argument spill. Compute `80038294 + index * 104`, call actual pointer-taking `80011A10` on that record, then set its byte 0 to 1. Caller `800149BC` masks the index to 16 bits. Pointer is retained across the call, even if the global base changes. |
| `80014374` | One 32-bit flags argument; unsigned-byte mode result. Priority is bit16→2, bit17→3, bit18→4, bit19→0, otherwise 1. Other bits do not affect the result. |
| `800143C0` | Four consumed formals: pointer to 32-bit frequency, two unsigned 16-bit values, and 32-bit flags. Clear `80038291`, initialize queue with `80014550`, set `80038290` from bit20, call `80010C68` on frequency, classify flags, call `80014198` with original pointer/two narrowed values/mode, return 0. |
| `80014434` | Same four real formals and result as `800143C0`; initialize queue, call `80010D3C` to update frequency, classify flags, call `80014198`. It does not change `80038290/91`. |

All dependencies are declarations only in matching files. The two setup callers
`80010840/800108E0` actually pass the address of the frequency value, a byte widened
to the first 16-bit argument, a 16-bit stack argument and 32-bit flags, and consume
the result. `80014198` consumes the frequency through a word pointer and the two
halfwords and byte, confirming narrowing. No unused invented formals are used.

`AudioState` retains earlier BT02-small field offsets (initial_count 0x20, count
0x48, state 0x5C), exposes active at 0, and extends the unknown region through
0x67 to express the observed 104-byte array stride. Unknown bytes describe real
record storage, not stack padding. Host offset/size assertions verify this view;
no shared header is changed. Queue declarations use the existing
`OSMesgQueue_s` tag and real `OSMesg`/API signatures, with opaque external storage
views consistent with prior boot-tail sources. Release-callback identity beyond
observed one-pointer call contracts is not claimed.

The receiver `80010A40` reads the command's first byte and handles 255 as a stop
request. A one-byte command array is therefore real message storage, with no
invented padding. The host test consumes it during the API double; it does not
prove native thread scheduling or the receiver's access lifetime after return.
No third-party source was copied. Arcade equivalents are unknown.

## Flags and bounded diagnosis

| Body | Final O2 diff | Final O1 diff/words | O1 excess nonzero words |
|---|---:|---:|---:|
| 800105C4 | 0 | 12/22 | 0 |
| 80010D74 | 0 | 23/25 | 0 |
| 80011C84 | 0 | 20/21 | 0 |
| 80014374 | 0 | 19/19 | 12 |
| 800143C0 | 0 | 28/29 | 2 |
| 80014434 | 0 | 21/21 | 2 |

O2 evidence includes hoisted global/address formation, frameless mode selection,
native argument homes and call scheduling. Final O1 controls provide per-body
negative evidence; there is no optimization disagreement inside this packet.

Initial 10D74 was 3/25: two byte-home/address differences and a mistaken API
identity. Workbench identified the home mismatch at equal 32-byte frames; the
strict scorer identified the actual API address. One directed fix, a genuine
one-byte command array and correct API, matched. Initial 14374 was 2/19: default
constant in v0 versus v1 plus final move. Workbench reported one register and one
structural residual with unchanged temp lane. Six natural variants tested result
types/return versus shared-result flow; the shared-result if/else chain matched.
No bound extension was needed. `diagnosis.json` distinguishes diagnostic
relocation-layout cautions from strict relocated acceptance; raw objects and
native dumps were kept only in private temporary scratch and are not published.

## Host verification and scope

Actual matching files are compiled separately under strict host C89 at O2 and
ASan/UBSan at O1. Checks cover:

- null/non-null/repeated queue initialization, count, buffer and clear timing;
- 255 stop byte, exact jam queue, blocking flag, ignored error return, release order,
  second callback and pointer rereads;
- record indexes 0, 17 and 65535, 104-byte stride/offsets, call-before-activation and
  pointer retention when the dependency changes the global base;
- all 32 combinations of mode/flag bits with two patterns of unrelated bits,
  yielding 64 mode tests and 128 full setup calls;
- precedence, zero/max halfwords, real pointer forwarding, callee frequency
  mutation, flag differences, call order and zero return.

No N64 hardware, concurrency or cartridge gate is emulated. Leak detection is
turned off for the environment's ptrace limitation; this driver allocates no heap
storage. Only the six matching sources and this packet directory are changed.
Targets, scorer, symbols, locks, layout, shared types, D10, paused helper work,
runtime images, farm files and production gates remain untouched.
