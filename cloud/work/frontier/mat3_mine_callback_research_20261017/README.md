# Mine callback: typed behavior research

Research only, `claims: []`, **zero accepted matching bytes**. This is a large
remaining code-generation mismatch, not a local exact match or integration result.

## Result and scope

N64 `func_8010C974` at 0x8010C974 is `_AnimateMine`. The native WPR_MINE callback
table and earlier runtime work independently establish the identity. Native extent
is 2,636 bytes, frame 392. This candidate has 499/659 differing words, exact ELF
symbol extent **2,632 bytes**, frame 344. A final alignment word must not be
counted as a function instruction. There are 27 aligned opcode rows different.

The old registered-head seed does not compile: it assigns a visual pointer to a
float and has incorrect inferred call/type contracts. This independent typed body
repairs the target, mine state, visual and object-table views and reproduces bounded
behavior. There are no unused padding locals, stand-in callers, forced registers,
instruction patches, or fabricated helper calls in the new callback.

The genuine object allocator is included as context. Its complete 300-byte symbol
body has zero differing words, but the scorer observes three adjacent nonzero
compiler-generated deleted-procedure-stub words. It is **not** a new matching
claim. The signed-short return follows the original MAT3 contract and the native
sign-extended return. All inherited allocator statements and debug checks remain.
The explicit typed callbacks keep N64's `(Visual *, signed short op)` contract.

The accepted native nine-float CopyUV leaf executes in the MIPS behavior test.
Adding its complete genuine C body to compiler context was also tested separately:
Mine remained 499/659. This packet does not include or claim that contextual variant.

## Source evidence and native differences

The complete later MAT3 `_AnimateMine` donor is 2,156 bytes at 0x8004D428.
It is a reported inherited match, with a task-10 preservation audit, not a freshly
resealed standalone strict receipt. Source body SHA256:
`37d91e50634e9abcebc112ab34ffa805f0f9f0218849fde3cd695722bba63751`.
Complete donor targets.c SHA256:
`fff71e7c9e9d2c0b367b5c537841f94bc079e444a29873f367c5104811389540`.

The donor establishes the lifecycle, real color/flag accessors, MBOX allocation
wrapper, removal wrapper and list traversal. N64 instructions establish all
consumed offsets and widths, base-object flags/color writes, null matrix allocation
argument, texture bank stride, float evaluation order, and callback ABI. The
136-byte mine state, 112-byte target, 24-byte visual, 68-byte object and 20-byte
four-mine slot record are verified by IDO-emitted layout constants.

RoadPosition takes position, output position and a nine-float output orientation.
ReleaseEnviron receives the original visual and unlink flag 1. FreeStatLLE takes
the pool base, not its head member. ProximityDamage receives float radius-squared
400 and integer damage 1600, with the fifth O32 argument on the stack. StartBlast
receives float scale 0.4, not an integer reinterpretation used as a numeric value.
The remaining StartBlast dependency is unmatched.

The externally named native threshold at 0x80124954 is independently checked
against the source literal 0.0333333f. A literal-only compiler control had the same
499 residual but could not establish both own-rodata relocations under the strict
scorer; that control is excluded. No unverified relocation is accepted here.

## Bounded proof

- 206 deterministic fixtures compare every non-stack backing byte and the call
  event sequence against a separately expressed field/event model.
- Cases include initialize/release/pause, every mine-slot position, oldest-age
  ties/nonpositive ages, road hit/miss, flash phases, ring activation/reset,
  alpha boundaries, expiry and cleanup with already-removed object IDs.
- 654/659 native Mine instructions and all 19 native CopyUV instructions execute.
  Five native instructions remain uncovered. Coverage is not a runtime proof.
- Three independently compiled semantic mutations fail for specific reasons:
  wrong alpha decrement changes alpha bytes, wrong oldest-tie comparison changes
  the selected mine's age, and wrong explosion damage changes the call event.
- 28 IDO-emitted layout/type facts are checked; wider alpha, narrower slot capacity,
  and wrong object-color placement are rejected as data mismatches.
- Complete stock/traced-uopt objects are byte-identical. Trace receipts bind every
  source/proof input and the resulting object, with observation-only tracing.
- The verifier checks full symbol extents, source/model/target hashes, complete
  group roles, uncertainty-free relocations, saved GPR/FPR/SP state, and its sealed
  output receipt. Integrity refusals also run under optimized Python.

Explicit limits: allocation, RoadPosition, FreeStatLLE, display, release, blast and
radius-effect calls use deterministic boundary models. They do not execute the
actual callee bodies. Their event arguments are checked; callee side effects are
only the documented fixture effects. The model and interpreter share those
boundary models but independently express the Mine body. Fixtures use valid
pointers, owner/team indices 0..3, finite ordinary floats, stable frame time, and
sufficient backing memory. Exhaustion, asynchronous changes, invalid pointers,
NaNs, full-game C aliasing, gameplay runtime and ROM integration are not proved.

Run with the existing protected target artifacts and IDO toolchain:

    python3 cloud/work/frontier/mat3_mine_callback_research_20261017/verify.py
    python3 -O cloud/work/frontier/mat3_mine_callback_research_20261017/verify.py
    python3 -m pytest tests/cloud/test_mat3_mine_callback_20261017.py

No ROM bytes, binary objects, raw traces or assembly dumps are part of this packet.
