# D9058: complete caller reconstruction, private-ABI context still open

Status: **NONMATCH**, bounded source/ABI research. No matching or coverage credit.
Claim: `func_800D9058`, `[0x800D9058, 0x800D91A0)`, 328 native bytes / 82 words.
Base: `cd22879d40b3de443cfde047b86e75e159b6cec6`.

## New result

The caller is reconstructed without a discarded slot-return keeper. Its only
selection call ignores the returned previous slot. The historical claim in
`s20261004/D/notes.md` that this particular function executes a dead return
copy is wrong; that broader census/correction belongs to the separate frontend
audit. This packet supplies the actual caller source and finite behavior proof.

The natural boolean loop in `candidate.c` reproduces the native four-way
unrolling and decision structure. It counts **13**, not 10: two fixed entries
plus twelve candidates, excluding only index 8. The native comparisons to 7,
6, and 5 are the next three unrolled iterations' rewrites of the same index-8
exclusion; they are not three additional excluded options.

No source-shape, keep-list, pressure-parameter or dead-return sweep was run.
There are two frozen diagnostic builds: standalone O2, and O3 with the complete
clean C104 selector/caller context. The latter uses the genuine existing
`object_create`, `world_trigger_check`, bank-refresh, and byte-9 routines, plus
the minimal semantically inert hook. It does not import the accepted
`slot_sound` group's explicitly non-original dead-switch inline blocker.
`verify.py` checks the context against the archived C104 source, permitting
only the documented shared queue declaration harmonization and comment.

## Native/source contract

- Read signed halfword `D_80151AD0`; load the word at field base `D_80113E8C`
  plus that index times 96, **before** the queue acquisition. The candidate
  declares a word view with a 24-word stride. This represents the accessed
  field; it does not establish the original record type, enclosing allocation,
  record count, or validity of every signed index.
- Acquire the actual `D_801461D0` queue with blocking flag 1, select slot 11,
  and release the same queue with flag 0. Ignore all three return values.
- Subtract 24 with the native 32-bit wrap, divide signed by 16 toward zero,
  and narrow to a signed halfword in `D_8014A10A`.
- Calculate the count described above. Cap at 13 when the **narrowed signed
  halfword** is larger, or when the post-callback flags at `D_801174B4` intersect
  `0x7C03FFFE`. This is not an unbounded integer clamp: quotient 32768 becomes
  -32768 before comparison. Flag bit 0 and bit 31 are outside that mask.
- Queue/selector callbacks may mutate the index, field and flags. The loaded
  field remains the original snapshot; flags are observed after unlocking.

The unsigned subtraction followed by a signed cast deliberately represents
MIPS wrap. The tested host and pinned IDO use 32-bit two's-complement `int` and
16-bit `short`. This is a behavior reconstruction, not authenticated original
N64 source or a portable guarantee for other integer representations.

The sole registered direct caller is the real 3,860-byte `func_800D91A0`, at
call site `0x800D9310`. Native D9058 writes s4/s5 without saving them; the parent
saves the encompassing register set. D9058 is therefore not an isolated O32
entry. The parent is not reconstructed in this packet. D9058 is kept as an
entry only to make the bounded O3 diagnostic observable, not to claim that
visibility setting is the original TU.

## Complete, unmasked negative measurements

- Standalone: **336 bytes**, 72/82 canonical positional differences; **74**
  differences including both words beyond the 328-byte native extent.
- Clean-context D9058: **364 bytes**, 77/82 canonical positional differences;
  **86** including every extra word.
- Selector: 248 vs 232 bytes, 34 complete-word differences.
- Bank refresh: 468 vs 488 bytes, 120 complete-word differences.
- Empty hook: 16 bytes, 3/4 words different.
- Byte-9 setter: 64 bytes, 6/16 different.
- Existing `object_create`: 112 bytes, still 1/28 different.
- Existing `world_trigger_check`: 228 vs 216 bytes, 42 complete-word differences.

The selector argument is a0 in the standalone diagnostic, s1 in the clean
context, and s2 natively. No ABI-qualified match or downstream closure is
claimed. Every emitted function's full ELF symbol size is used, including
extra nonzero instructions and missing native words; trailing object alignment
is independently verified zero. There are no masks, unresolved relocations,
unverified literal sites, or owned data/literal sections.

GNU ld links each **unmodified** object at a real contiguous `.text` placement
of `0x80000000`. Every resulting instruction must equal the project resolver
using those same placement addresses. The separate native comparison binds
internal function calls to the protected native entry addresses. This is not
a physically native-spliced GNU group or ROM-link proof; an unsplit contiguous
group cannot simultaneously occupy its noncontiguous original slots. No ELF
instruction, relocation, or symbol metadata is rewritten for the GNU witness.

## Tests and limits

`verify.py` runs 4,756 cases comparing host-compiled C, the protected native
caller, and both relocated diagnostic callers. The interpreted compiled
callers use their **own** ABI-specific selector hooks (a0/s1); they are never
passed off as calling the native s2 selector correctly. Tests cover division
sign edges, 32-bit subtraction wrap, halfword narrowing, all individual flag
bits, each mutation mode, and deterministic arbitrary 32-bit heights.

A separate ASan/UBSan executable checks 40,000 source cases. Five additional
native-only cases cover extreme signed indices. The host fixture's seven
valid starting indices are test storage only, not a source-capacity claim.
Native instruction execution reaches 77/82 words; five untaken words belong
to impossible later unrolled index-8 arms. Compiled coverage is 79/84 and
86/91. The complete branch outcome sets are retained in the receipt.

The interpreter fails closed on unknown instructions or unmapped memory,
checks call order/arguments and preserved registers under each ABI, and
clobbers declared caller scratch registers and argument homes at hook calls.
Its hooks are semantic fixtures, not executions of queues, slot selection,
loading, bank refresh, or the scheduler. This finite caller proof is not
unrestricted instruction-level equivalence, a runtime gameplay test, or proof
of valid enclosing storage for arbitrary indices.

Two corrupted-ELF drills reject truncated nonzero function tails and unsupported
relocations. The pytest tests also reject a wrong selector value, wrong selector
ABI, and an unmapped field. Fresh receipt reproduction is required; missing
tools fail under `REQUIRE_TOOLCHAIN=1`.

```
python3 cloud/work/dot_selector_d9058_20261006/verify.py
REQUIRE_TOOLCHAIN=1 python3 -m pytest -q tests/cloud/test_selector_d9058_contract.py
```

Stop here. Reopen native matching with sufficient **genuine** D91A0/callee
summary context and evidence for the retained inert-hook visibility. A fake
return consumer, switch blocker, dead read, or synthetic saved-register
pressure would not close those source questions. Production sources, accepted
contexts, flags, targets, symbols, locks and build tools are unchanged. No
image/ROM verification or coverage claim is made; publication contains no
native instruction words, raw assembly, objects or ROM bytes.

Final validation: all seven focused tests plus the existing scorer, submission,
protected-path and target-integrity tests passed: **739 passed, zero skipped**.
The initial sparse checkout lacked their archived locked-source inputs;
materializing those tracked inputs recovered the aggregate run without any
source change. `validation.json` records the command and unchanged protected
file hashes. This is a selected regression set, not the full repository suite.
