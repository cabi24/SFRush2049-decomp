# BT02 final small: audio initialization and tick gate

Two strict relocated O2 matches, **220 native bytes**, and two complete nonmatches,
**320 bytes**. Pinned-input replay and 2,168 actual-source host cases pass under
strict C89 and ASan/UBSan. Independent source/ABI review by the BT03 low-lookup worker passes on
`d8627b6357b52cbb3ce9b0995cede8361812f7f5`; the numeric receipt is
`independent_review.json`. Exact aggregate-head CI remains required. This packet does not claim cartridge coverage, promotion or acceptance.

## Scope and provenance

- Base: `301d9e7552ad4fd7f54a38796db84671e1000d35`.
- Central exclusive claim: `9448eb24`, before source changes.
- Branch: `dot/boot-tail-bt02-final-small`.
- Starts: `800107E0` (96 B), `80010840` (160 B), `800108E0` (160 B),
  `80013D70` (124 B), total **540 B**. No prior BT02 body is retried or credited.
- Only the two named matching sources and this packet directory are changed.
  Central claims, STATUS and D10 belong to the aggregate lead.
- Existing IDO installation and Packet 2 compiler/input hashes are reused.
  No installation or compiler/scorer/target modification was made.

## Reproduce

Set `IDO_DIR` to the existing pinned IDO 5.3 directory, then run at repo root:

```sh
python3 cloud/work/boot_tail/BT02-final-small/verify.py
python3 cloud/work/boot_tail/BT02-final-small/test_host.py
```

The verifier checks all installed compiler digests, protected scorer/setup/getter
hashes, the three target-manifest members, and all 439 inventory/extent rows
(99,120 B). The existing 12-byte getter still strictly matches. Every final
source is replayed at O2 then O1, including the two nonmatches, and all three
baseline controls are reproduced. Receipts bind actual sources and host harness
to SHA-256 digests. Matches have zero differing or excess nonzero words,
unresolved symbols, unverified relocations and errors. No local data or table
proof is needed. The native target population and shared tools remain read-only.

## ABI and behavior

`800107E0` takes one actual 32-bit frequency argument. It calls `80014E10`,
`80017108` and `800199F4` without arguments, clears the volatile byte at
`8004BE94`, forwards the saved frequency to `8001C1D8`, then calls `8001C390`
and `8001E0C0`. It finally sets `8002C630` to one and returns zero. All six
actual callees were inspected: only `8001C1D8` reads an incoming argument,
storing it in `8004F800`. The frequency is an unsigned word consistently with
the pointer passed to `80010C68`/`osAiSetFrequency`; arithmetic inside downstream
users is outside this packet. The byte's volatile qualification is a supported source-level hypothesis: it
reproduces the
native explicit address access; independent native reads/writes at `8001F13C`
and `800202C4` use the same address-materialized flag access. Volatile does not
by itself provide thread synchronization.

The complete `80010840`/`800108E0` initializers each take six actual arguments:
frequency word, voice-count byte, two setting bytes, count halfword and flags
word. The first four arrive in O32 argument registers and the final two on the
stack. They clear the enabled byte, cap voices at 32, write all three settings,
then call `800143C0` or `80014434` with a pointer to their local frequency,
the stored voice count, the count and flags. They invoke `800107E0` with the
potentially updated frequency only when setup returns zero; otherwise they
return that setup result. Actual setup callees save four arguments, receive the
frequency pointer, and load both middle arguments as halfwords. No extra or
invented formal is used. Both actual setup callees currently return zero;
the callers nevertheless contain the complete nonzero-return branch.

`80013D70` takes two unsigned halfwords: a buffer-table index and a count.
When enabled, it decrements a 32-bit countdown, and on reaching zero reloads
it from `800382A4` before calling `800198C8` and `8001B154` without arguments.
It always calls `800139D4` with the indexed buffer and count, then returns one.
Actual caller `80013DEC` supplies both halfword values; native `800139D4` reads
the pointer and masks/counts the second argument as a halfword. The countdown
is represented unsigned so the native wrapping decrement is defined in C.
`D_80038228[]` is an opaque-pointer table view supported by native scaled loads.
The older `80010D74` source's scalar pointer view accesses its first entry;
this packet does not change that file or shared declarations. The host table is
large enough for all halfword indexes, but no native bounds guarantee is claimed.

## Flags and bounded diagnosis

| Source | Final O2 differing/total words | Fixed O1 differing/total words |
|---|---:|---:|
| 800107E0 | 0/24 | 16/24 |
| 80010840 (NONMATCH) | 14/40 | 37/40 |
| 800108E0 (NONMATCH) | 14/40 | 37/40 |
| 80013D70 | 0/31 | 25/31 |

All rows have zero excess nonzero words and no relocation uncertainty. Exact
flags are `-g0 -O2 -mips2 -G 0 -non_shared`; the scorer always adds
`-Wab,-r4300_mul`. O2's scheduling, countdown/address common-subexpressions and
caller result-carrier reuse support this level. O1 controls do not match.

The first natural `107E0` scalar source was 16/24: its flag write lacked one
address-materialization instruction, shifting everything afterward. Workbench
diagnosis preceded variants. An array view did not improve it; the volatile
scalar view matches. `13D70` matched its first natural form; choosing unsigned
countdown storage preserved equality and made native wraparound explicit.

Both initializer baselines are complete 14/40 nonmatches. Their frames are the
same 24 bytes and instruction counts are both 40. Native homes the byte argument
before narrowing it into its own argument register; baseline narrows into a temp
and homes later. The resulting temp allocation shifts three subsequent byte
stores. All words from offset `0x64` onward strictly agree. Relocated workbench
diagnosis identifies schedule and register residuals, not constants or a frame
size defect. Ten directed variants (register/K&R declarations, typed local
copies, alternative clamp placement, conditional expression and volatile count)
did not improve the baseline. `diagnosis.json` records every measured result.
No fake keeper, padding, added formal, inline assembly or context helper is used.

Stop here: a new attempt needs independently supported original argument-carrier
or declaration evidence. Do not repeat these spellings or sweep compiler flags.
The `controls/` files preserve the exact initial baselines. Temporary objects,
resolved native words and detailed workbench diagnostics are not published.

## Host checks and limits

The harness compiles all four actual final sources as separate translation units;
callee doubles live only in the host test. Both configurations pass 2,168 cases:

- 12 standalone initialization cases: frequency extremes, initial flag states,
  exact reset timing, six-call order and unrelated setting preservation;
- 2,048 wrapper cases: every byte voice count for both wrappers, each with four
  setup results, setting-byte boundaries, halfword count extremes, maximum flags,
  frequency output mutation, success ordering and failure-path suppression;
- 108 countdown cases: enabled values 0/1/255, zero/one/two/max counters,
  zero/one/max reloads, indexes 0/1/65535, wraparound, reset-before-callback timing,
  pointer/count forwarding, untouched enabled/reload values and constant return.

ASan leak detection is disabled for the established runtime ptrace limitation.
These tests do not emulate N64 hardware, threads, interrupt timing or the actual
external audio operations. Native strict comparison proves only the two matching
bodies. The wrappers remain research even though their host behavior passes.

No third-party source was copied. No ROM, raw disassembly, object, credential,
unrelated private data, runtime image, layout, lock, symbol or shared-type change
is included. Publication is through the central aggregate; merging and cartridge
integration stay with the independent checker.
