# Voice/list promotion contracts: source-only repair

Base: `cf10b3392d7f00ae42d75c008b79fdc2541aab6b` (2026-10-05).
Scope is exclusively `lib_1a660.c`, `lib_1f5b0.c`, and eight new adapted
candidates under `voice_lists/sources/`. Historical candidate files stay unchanged.
No promotion, relocking, full-ROM build, coverage change, remote-builder call,
scorer/target/flags change or `lib_207b0.c` edit is performed.

## Result

All **eight candidates / 1,900 bytes** pass standalone, real-header context and
combined actual-TU compilation. All **fourteen existing accepted C bodies /
2,220 bytes** retain their normalized body hashes and pass exact ELF STT_FUNC
extents plus complete unmasked relocated word equality in baseline, current and
combined TUs. No accepted production body has been renamed or rewritten.

| TU | Pending candidates | Candidate bytes | Existing C bodies / bytes |
| --- | --- | ---: | ---: |
| lib_1a660 | 8001B29C, 8001B3A0, 8001B7C0, 8001B8C4, 8001B968 | 1,088 | 5 / 1,040 |
| lib_1f5b0 | 8001EB10, 8001ECE0, 8001F9D0 | 812 | 9 / 1,180 |

All 43 function offsets and the respective 10,544/4,608-byte text section
sizes are unchanged. Every remaining assembly slot retains identical words and
symbolic relocation identities. `8001B9F8` and `8001BE14` in `lib_1a660` reference
historical jump-table aliases absent from the protected scorer symbol manifest;
the report explicitly retains those unresolved results. Their baseline assembly
preservation is proved, but this packet does **not** claim a new fully relocated
proof of those two untouched assembly bodies. All 22 required C bodies have no
unresolved references, masks, excess words or relocation errors.

## Genuine layouts and calling contracts

The refusal was caused by separately named, incompatible partial declarations
of the same voice array and sequence heads. Merely renaming each local type did
not reconcile those objects inside a combined translation unit.

- `lib_1a660` retains its accepted `VoiceState_80019C8C` view and field names.
  The existing opaque spans expose only native byte selector +0x55 and halfword
  value +0xC2. The stride stays 0x1A0; child/parent are +0x10/+0x14, flags +0x24,
  primary channel/set +0x4A/+0x4B, identifier +0x60. The five candidates now use
  this exact declaration and its existing field names.
- `lib_1f5b0` retains its accepted `VoiceState` field names. It exposes parent
  identifier +0x14, real `SequenceNode *` at +0x18 and reset word +0x28 within
  previously opaque storage. The 0x1A0 view is placed before early list helpers;
  whole-file declaration deduplication cannot otherwise make a later typedef
  visible at an earlier insertion point.
- The genuine node is 16 bytes: next pointer +0, previous pointer +4, unsigned
  key +8, signed value +12. `D_80050C50` and `D_80050C54` are pointers to this
  same node. Existing `func_8001EAA0` and `func_8001EDF4` bodies are untouched.
- `func_8001EB10` has one real voice pointer and returns void. It calls 21844
  before reading live voice fields, then detaches a parent/child chain or moves
  its last root node from the used to free list. `func_8001ECE0` accepts that
  same TU-local voice view, returning an unsigned key or FFFFFFFF. `8001F9D0`
  invokes the same one-pointer detach contract, clears flags/reset word and
  invokes the real one-pointer 1F6EC helper.
- The accepted lookup `func_8001EDF4(u32)` returns **signed int**, with -1 for
  failure. The adapted setter declarations now match this contract. Identifiers
  retain their 32-bit representation when assigned to unsigned callers or the
  signed node value, following the documented N64/IDO two's-complement unsigned
  to signed conversion convention; this is not an implementation-independent
  conversion claim.

These are TU-local views of the same native record layout, preserving existing
accepted source spellings. Native IDO assertions cover all relevant field
positions, pointer widths, node size and both full voice strides. No synthetic
padding/keeper, fake argument, volatile trick, alias macro, assembly body, helper
stub or compiler flag change is used. Unknown spans are actual unobserved record
storage. This N64 audio runtime has no claimed arcade source equivalent; prior
ABI/behavior research is in BT03-high-larger, BT03-high-runtime-followon,
BT03-high-chains, BT03-high-runtime, BT03-high-routing and BT03-high-init.

## Reproducible verification

From the repository root, with pinned IDO and MIPS binutils available:

```sh
python3 cloud/work/boot_tail_promotion/voice_lists/verify.py --output /tmp/voice-lists.json
python3 cloud/work/boot_tail_promotion/voice_lists/semantics.py
REQUIRE_TOOLCHAIN=1 python3 -m pytest -q tests/conveyor/test_voice_list_promotion.py
```

The verifier only creates temporary source/object files. It compiles actual ROM
TUs through unchanged asm-processor and the Makefile ROM-TU flags. It runs the
existing header-context fitter and exact-statement deduplication for scratch
candidate insertion. Each C symbol has exact native ELF size; relocated words
are compared directly without masks and hashed alongside the protected target.
Receipts bind sources, inputs, compiler, assembler and objects.

Deliberate failures reproduce both original incompatible declaration errors,
a wrong voice stride (1/65 words), a wrong global relocation (1/65 words, no
unresolved reference), and a wrong node previous-pointer offset (10/116 words).
These controls distinguish full combined-TU/type/relocation checks from a
header-only or normalized-source-hash check.

The explicit adapted-source semantic invocation runs **7,634 calls in eight
C89 ASan/UBSan harnesses**. It includes live helper changes to next/parent/node
fields, valid/stale chains, both channel sources, byte values, enabled/disabled
flags, signed lookup failure, all getter slots, node insertion/collision/wrap,
empty/one/two-entry free pools, detachment topologies and reset call ordering.
Established harness field/type spellings are translated; the actual adapted
sources are included verbatim. LP64 host pointer sizes are never claimed as
native layout proof. Valid registered indices, allocated noncyclic lists and
valid post-callback records remain required; malformed-list/hardware safety is
not claimed. The focused pytest file passes **14 tests**, including complete
replay after simulated legitimate promotion of all eight candidates.

## Maintainer boundary and reopening

Only production declarations change; every original production lock remains
valid. Adapted candidate body spellings do change, so they occupy new source
paths and **have no stored locks**. All historical candidate files and locks stay
unchanged. A maintainer must lock these new adapted sources and execute the ordinary static-promotion
transaction and pass full cartridge gates before any coverage claim. No stored
lock/context/status record is rewritten here. Future legitimate promotions are
accepted by the verifier: it checks their current source/lock membership,
compiles them in place, and overlays only remaining passthrough candidates.

The bounded contract repair closes all eight assigned source conflicts. Reopen
only for an independent review defect, changed production contract or a failed
maintainer gate; unrelated master CI problems are outside this packet.
