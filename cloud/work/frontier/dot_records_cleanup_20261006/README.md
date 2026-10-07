# records_screen: standalone 312-byte match

Complete target: `records_screen`, `[0x800D58CC, 0x800D5A04)`, 78 words.
Frozen source/tool/target baseline: `7e62ed7b3c3e6f1e788bf013419b89e132b4c65d`.
The strict standalone scorer prints **MATCH** with
`-g0 -O3 -mips2 -G 0 -non_shared`, plus the scorer's required
`-Wab,-r4300_mul`. No group, helper definition, keep list, or unit override is
needed. The source belongs in `cloud/matches/records_screen.c`.

This is a matching submission, not a claim of accepted image or ROM coverage.
Image, compressed-stream and ROM gates remain the integrator's responsibility.
No locked source, target, header, build recipe or production gate was modified.

## What closed the archived residual

`cloud/work/tiny_A121/records_screen.c` was the complete 4/78-word baseline.
A fresh O3 replay preserved that exact residual. Workbench diagnosis found
identical instruction count, frame and temporary-register lane; only two
comparison operand orders and two adjacent address-materialization words differed.

1. A named `const int sentinel = -1`, genuinely consumed in both comparisons
   and both ID stores, closed the two comparison words: 2/78 remained.
2. Initializing the status cursor before the row cursor on one physical source
   line closed the address schedule: 0/78. Reversing initialization order on
   separate lines alone did not help. The source header discloses this layout
   dependency.
3. The current accepted `sound_stop` source proves a pointer-consuming `Voice *`
   boundary. Replacing the archived integer declarations with an incomplete
   `Voice` type for both globals and the callee preserves the complete match.

The selected source has no unused local, pressure variable, synthetic volatile,
inline assembly, fabricated callee/caller, or stack filler. The 50-byte record
array fields are observed record storage, not frame padding. No arcade ancestor
is established.

## Contract and ABI

The historical name is not a semantic specification. When its first voice-list
pointer is non-null, the function clears the shared sound-handle table, releases
that voice list, and clears its global. For each live 104-byte row it clears a
separate status word, removes the scene-node IDs at row offsets 0 and 52 when
not -1, then writes -1 after each call. The signed count is read again after
every complete row. It resets the cleanup-state global, then releases and clears
the second voice-list pointer. Modes 4 and 6 invoke runtime-image-B teardown
`func_8039244C` at its exact call address.

The current accepted `entity_spawn_callback` signature is
`(s16 index, s32 freeChildren, s32 freeSiblings)`; both flags here are zero.
The explicit narrowing of the consumed `int` ID snapshot is important to the
native argument preparation. `sound_handles_clear(int)` and `sound_stop(Voice *)`
are ordinary O32 calls. The external runtime-B call takes no arguments here;
its body is documented separately in `cloud/work/runtime_b_teardown_20261006`.
That historical packet's claim that sound_stop was unlocked is superseded by
the frozen current lock. This packet claims no runtime-B bytes.

## Verification

`bindings.json` binds the final source, verifier, host harness and native body by
hash. Callee provenance is read from the frozen base commit with `git show`, so
later production locks and source edits cannot silently redefine that evidence.
The receipt does not pin evolving live target manifests or host tool binaries;
`--local-provenance` can record those identities for a local investigation. The reproducible checks establish:

- Strict 78/78 equality, no unresolved or unverified relocations, no extra words.
- Full ELF identity is checked: ELF32 big-endian SYSV ABI0, MIPS2 flags,
  relocatable input and executable linked output. Exactly one defined function
  begins at input offset zero; unexpected allocated sections are refused.
  All 20 relocation site/type/symbol tuples are bound and recorded.
- GNU readelf `STT_FUNC` extent is exactly 312 bytes. The containing 320-byte text
  section has eight trailing zero alignment bytes, outside the function.
- GNU ld independently resolves all 20 relocations (6 calls, 7 HI16/LO16 pairs),
  using portable `--hash-style=sysv`. The linked function address and complete
  extent are checked independently with GNU readelf.
  GNU objcopy's complete linked 312-byte body equals the manifest-verified target.
- No owned `.data`, `.rodata` or `.bss`; no local placement proof is missing.
- 2,400 C89/UBSan host cases verify all row bytes, independent status words,
  global state and call order, including negative/zero player counts, both
  -1 sentinels, signed-half extremes, both resource guards, modes 4/6, and
  callback-driven count reductions. Three wrong-contract controls are rejected.
  These tests supplement the exact compiler proof; they are not gameplay tests.

Reproduce from an ordinary repository checkout with the IDO/GNU toolchain:

    python3 tools/cloud/score.py fn cloud/matches/records_screen.c records_screen --flags '-g0 -O3 -mips2 -G 0 -non_shared'
    python3 cloud/work/frontier/dot_records_cleanup_20261006/verify.py --output /tmp/records-cleanup-verification.json
    python3 -m pytest -q tests/cloud/test_dot_records_cleanup.py

Focused pytest coverage checks packet bindings, ordinary and Python `-O` replay
from a foreign working directory containing spaces, source-mutation refusal,
and deliberate GNU-link misplacement refusal. Both successful replay modes
compare the entire stable result to the recorded receipt. Toolchain-dependent tests skip
explicitly before invoking IDO when required executables are unavailable.
The verifier uses explicit runtime checks, never optimization-removable asserts.

Temporary objects, linked bytes and native disassembly are private scratch only;
none are included in this deliverable.

## Integration note (2026-10-06)

Wave 13 (w13f) independently matched and spliced `records_screen`, so
`cloud/matches/records_screen.c` holds that spliced source. This packet's
byte-identical submission was moved to `records_screen.c` in this directory,
and `verify.py` plus `tests/cloud/test_dot_records_cleanup.py` now default to
it. The verifier hash in `bindings.json` and `verification.json` was updated.
