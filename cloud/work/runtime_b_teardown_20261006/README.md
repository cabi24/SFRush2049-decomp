# Image B HUD teardown: complete, behavior-tested NONMATCH

## Result

Runtime image **B**, `func_8039244C`, `[0x8039244C, 0x803925C8)`,
**380 bytes / 95 words**. The complete typed source in `candidate.c` remains
**NONMATCH: 34/95 whole words differ** under ordinary IDO 5.3 O2.
No accepted bytes, matching submission, integration, or ROM-coverage gain.
Base: `cd22879d40b3de443cfde047b86e75e159b6cec6`.

This is the B image at ROM stream `0xB6FEC4`; address-only identification is
insufficient because runtime image A shares the same load window.
Current master, the project handoff, protected targets, accepted-source lock,
and open PRs were checked before claiming the interval. The separately
submitted B:8038CA24, B:8038A8CC and active B:8039133C work are untouched.

## Recovered behavior and actual contracts

The function first releases a non-null resource through `sound_stop`, then
clears that global pointer after the callback. When the signed player count
is positive it traverses four nonoverlapping record families:

- First 52-byte record: release handle at +2 unless it is -1, then set state
  at +0 to 8.
- Second 52-byte record: the same handle rule, then state 0.
- Third 52-byte record: release handle at +0 unless it is -1.
- Ten 72-byte records per player: release each handle at +2 unless it is -1,
  then state 11. A complete group is 720 bytes.

The player count is read again after every complete group, after all helper
calls. Side effects to the global pointer, later handles, state fields and
count are tested. The post-call clear stores must survive such changes.
Every record byte not named by these operations remains unchanged except
for explicit helper-hook writes.

`sound_call_minimal` is the real accepted 48-byte wrapper at `0x80090254`;
it sign-extends the handle and calls `entity_spawn_callback(handle, 0, 0)`.
Its current production source is recompiled unchanged and remains wholly
native-equal. No repeated matching credit is claimed. `sound_stop` at
`0x800B358C` is a 160-byte pointer-consuming linked-resource teardown whose
native implementation dereferences +0x34 and +0x3C. It has no accepted
source in the current lock. Both are external, ordinary-O32 boundaries here.
Despite their historical names, this packet does not establish an audio
subsystem interpretation.

The byte widths, offsets and strides are observed native layout. The field
and structure names are descriptive hypotheses. Unknown arrays preserve real
record storage; they are not stack padding. The available pinned Rush The
Rock game source was searched for stunt/battle/HUD leads without identifying
a whole-function donor. Original N64 type spellings and original translation
unit boundaries are not recovered.

## Compiler investigation and stopping decision

A first natural pointer-comparison loop exposed an important reconstruction
issue: the native function has an explicit signed-positive count guard before
forming its end pointer. The completed source preserves that guard, then uses
a bounded do/while traversal with the observed live count reload. Moving the
secondary record initializers into that guarded region follows their native
lifetime. The final complete function has the native 64-byte stack frame and
380-byte ELF extent, but load/store schedule and register-allocation topology
remain different. Workbench diagnosed a structural/scheduling difference,
not a relocation-only discrepancy.

The full relocated differing-word offsets are in `verification.json`. No
source permutations, artificial local/parameter pressure, inline assembly,
volatile qualifiers, invented helpers, protected-input changes or flag/keep
policy changes were used. Ordinary O1/O3 controls did not expose a standalone
matching route. Compiler experiments are frozen. Reopen only for authentic
record/alias contracts or original caller/TU evidence that explains the
remaining topology; changing declaration order to chase registers is not a
new hypothesis.

A separate earlier scout of B:8038C910 found a broad ordinary-O2 residual and
O3 spill-only behavior; it is not a second submission or matching claim in
this packet. That function needs authentic callback-record and call-boundary
context before further matching work.

## Verification and scope

`verify.py` is fail-closed even under Python optimization and writes a portable
receipt without machine-specific compiler paths or object hashes. Local tool
hashes can be emitted separately with `--tool-provenance`.

- Complete ELF STT_FUNC is 380 bytes. Complete `.text` is 384 bytes; the four
  zero alignment bytes are checked separately. No owned data or table.
- All **21** relocation records, their sites, types and symbols are checked.
  Entry/global/helper addresses are bound. GNU links the entire unmodified
  object at the native address; every relocated word agrees with the project
  reader. GNU and the project both find the same 34 native differences.
- **1,344 fixtures, 4,032 executions** compare the native body, project-linked
  candidate and independently GNU-linked candidate to an indexed oracle.
  Every one of the 95 native and candidate instruction offsets executes.
  Whole mapped nonstack memory and every helper-entry snapshot, exact helper
  order/arguments, bounded stack accesses and saved registers are checked.
  Hooks deliberately clobber all O32 caller-saved integer registers.
- **1,344 actual-source C89 fixtures** with strict aliasing and UBSan/bounds
  compare complete records and helper-entry state against a separate indexed
  oracle. The host has independently initialized opaque bytes and layout
  assertions for all four record types. Host/native fixture sequences are
  related in domain, not claimed byte-identical across endianness.
- Four compiled wrong-source mutations fail: wrong first state, omitted last
  effect, wrong handle clear, missing resource clear. Three native execution
  controls reject unknown instructions, a changed state and an unmapped base.
- Six focused tests include portable receipt replay from an unrelated working
  directory with spaces, normal and optimized Python, and fail-closed invalid
  access/helper contract checks.

The C-valid fixture domain is four accessible, nonoverlapping player groups;
initial count is -32768, -1, 0, or 1..4, and helper-modified count stays 0..4.
Negative initial counts exercise only the positive guard. No invalid pointer,
out-of-bounds player count, arithmetic wrap, concurrent access or malformed
resource chain is claimed valid. The real helper bodies do **not** execute in
behavioral tests; they are bounded side-effecting hooks. Native execution is
a deliberately narrow interpreter, not N64 hardware. No complete caller,
whole-game, broad repository suite, image/recompression or full-ROM gate was
run. Runtime integration, acceptance and merging remain checker-owned.

## Reproduction

With the documented IDO/binutils environment, from the repository root:

```
python3 cloud/work/runtime_b_teardown_20261006/verify.py
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_runtime_b_teardown_packet.py -q -o addopts=''
python3 tools/cloud/score.py fn cloud/work/runtime_b_teardown_20261006/candidate.c func_8039244C --targets asm/us/ovl_b
```

The final command intentionally returns NONMATCH. Only source, tests, and
metadata/notes belong to the packet. No ROM bytes, raw assembly dumps,
objects, credentials, unrelated private data or production inputs are added.
