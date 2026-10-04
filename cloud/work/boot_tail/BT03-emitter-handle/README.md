# BT03 emitter handler: exact736-byte body, qualified semantic domain

**First natural O2 source strictly matches all184 native words /736 B.** ELF
function size and .text size are each exactly736 B, with zero nonzero excess,
masked/unresolved/unverified references or relocation errors. No refinement,
frame trick, invented argument or invented float initialization was used.

**This binary match does not establish safety on every original runtime input.**
The native/source caller conditionally writes five genuine uninitialized float
locals. Some paths can consume an unwritten value. The original global producer
and listener invariants that would exclude those paths remain unproved. This
limitation is retained alongside the match, not repaired or concealed.

Target8001DDE0 ends at8001E0C0. Central activation `42d94b52` explicitly permits
this defined-path Packet5 caller reconstruction, leaving the T050-gated
CalcEmitter body unchanged. Branch `dot/boot-tail-bt03-emitter-handle` stacks on
`01eba3fdaf662189ca1fbe4275529c0be491541e`; master anchor remains
`301d9e7552ad4fd7f54a38796db84671e1000d35`. Only the named matching submission and
this research directory change. Shared ledgers, headers, targets and tools stay
unchanged.

## Source provenance and native-specific reconstruction

The pinned CC0 lead is `s3dHandle` in
[AxioDL/musyx snd3d.c](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/snd3d.c),
revision `78d2e16e4905fc675952162d331c24d5198b2687`, under the pinned
[CC0-1.0 license](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/LICENSE).
Both were read. `provenance.json` records their exact Git blob identities.
This is a homologous source lead, not original N64 translation-unit proof.

Every retained branch and interface was checked against the complete native
body. The public call-count throttle, room/studio paths, zero-volume0x40 branch,
extra filter/parameter logic and final room/door updates are absent from this
native function and are not imported. Native FX-start has three actual inputs.
The public fade literal0.3 is **not** assumed: D_8002D90C remains an external
const float at its actual address, with original contents unknown. No local
literal/table or masked relocation stands in for that load.

## Whole caller and actual interfaces

The function is genuinely `void(void)`. It clears the start-list counters, walks
D_8004FD50 while capturing the next pointer before calls, processes shutdown,
recalculation, starting, validation, stopping, parameter updates and fading,
then calls the final continuous-emitter dispatcher. It preserves all live field
reloads and the source-level float-value lifetime across loop iterations.

Native Emitter size is68 B: next/previous pointers0/4, flags8, actual untouched
range12..51, identifier52, group56, sound ID60, counter62, fade64. The40-byte
opaque range represents real unexamined object fields, not local padding.
Native IDO assertions prove these offsets. Host pointer width is larger and the
host object is not asserted to have native sizeof or pointer offsets.

There are **ten distinct callees**, not nine:

| Callee | Actual interface / role |
| --- | --- |
| 1D928 | void(void), clear three byte counters |
| 1D084 | void(Emitter*), unlink and mask flags |
| 1C860 | void(Emitter*, five float output pointers), ordered volume/pitch/x/y/z |
| 1DA74 | int(Emitter*, volume,x,y,z,pitch), checked start-list capacity result |
| 1B1D0 | u32(u16 sound ID,u8 volume,u8 pan) |
| 201D0 | u32(u32 handle), unchanged valid handle or all-ones |
| 1D944 | void(Emitter*,float volume), unchecked running-list insertion |
| 1B8C4 | int(u32 handle), actual status ignored by this caller |
| 1CCDC | void(Emitter*,volume,x,y,z,pitch) |
| 1DC08 | void(void), final external dispatch |

All six slots for1CCDC are real even though that callee does not use y pan.
No helper implementation is added. Frozen peer ABI evidence includes
BT03-high-transitions at2c553552 (1B1D0), BT03-high-runtime-lists at86b0f12e
(1D944/1DA74), BT03-chain-dispatch at75df2d98 (1CCDC), and previously strict
1D084/1D928/1B8C4 sources. Actual canonical bodies/call sites were inspected.
`abi_proof.py` binds the ten-call set, native layout and every external address.

## Precisely what the semantic tests establish

The five float locals retain their genuine uninitialized declarations. The
required domain is: **every value actually consumed must have been written in
this invocation**, possibly during a previous emitter iteration. A skipped
recalculation preserves earlier defined values; it does not create defaults.
CalcEmitter always writes volume/pitch but can leave x/y/z untouched when there
are no listeners. The public lead and native control flow both expose this
qualification; neither proves a universal runtime invariant.

Tests use three separate views:

1. An independent state/call-trace model tracks each output as unwritten or as a
   real written value and its writer iteration. It raises a domain violation
   before a missing-value use or an unchecked helper-capacity violation.
2. A **temporary host-only instrumented copy** of the actual C replaces value
   reads with checked getters and records actual output writes by the helper.
   Only tracking metadata is initialized, never the float values. This copy is
   not compiled by IDO, scored, submitted or used as the matching source.
3. The untouched matching C runs only for the already-vetted safe fixtures.
   Its32-bit FNV-1a digest over the call trace and all normalized caller fields
   must agree with the tracked copy and independent model. This is bounded
   regression agreement, not exhaustive semantic equivalence or a collision-free
   comparison. The original file hash is checked
   before/after testing.

Across786 main fixtures:

- **612 safe cases** pass through both tracked and untouched C.
- **172 cases are rejected for consuming an unwritten output.** They never run
  through untouched C. Examples include a first waiting emitter without a
  recalculation and an empty-list calculation followed by a five-float call.
- **2 cases are rejected for unchecked helper capacities**, not turned into
  artificial failure returns. 1D944 requires valid32-entry group/small pools.
- Safe execution records **1,362 float consumptions**. **12 safe cases preserve
  57 values read from a prior iteration**, including multiple skipped recalcs.
- Two extra model probes explicitly verify unsafe-path rejection.

The fixtures also cover empty lists, removing head/middle nodes, valid/stale
handles, pending/start failure paths, successful starts, automatic stopping,
waiting/wake transitions, fade threshold crossings, large-pool failure and the
real group-creation-before-large-pool-failure side effect.

The observation boundary is **entry to final1DC08**, before its opaque internal
operations. Synthetic helpers model declared modular caller contracts; these
are not full spatial/audio-engine runs or replacements for the gated callee.
Pool/group preconditions are enforced. The real callee's nonempty-list
head-anchor insertion order is documented by peer evidence, not reimplemented
by this caller harness. The tests do not claim the original geometry
or engine state produces every chosen finite helper result.

Definedness is measured at **C value consumption**, not at every speculative
machine load. The native compiler can preload an unwritten stack slot whose
value a particular path never uses. Such an unused load is not silently turned
into a fabricated initialized local. No claim is made that arbitrary undefined
paths are safe, nor that an original producer invariant has now been proven.

Only valid allocated acyclic emitter objects, coherent bounded helper effects
and ordinary finite FP are covered. The original fade scalar, arbitrary
concurrency/reentrancy, malformed inputs, exceptional FP modes, full helper
algorithms, whole-game integration and cartridge behavior remain outside this
semantic proof. Exact native body equality is a distinct instruction-
identity result, not a universal source-language/runtime safety guarantee.

## Compiler, tests and gates

O2 flags are `-g0 -O2 -mips2 -G 0 -non_shared`, with canonical automatic
`-Wab,-r4300_mul`. O1 is the sole control:183/184 differing words plus40 nonzero
excess, and it is rejected. O2 reproduces the native112-byte frame and five real
float output slots exactly. O1 uses a different72-byte frame. There are no
source variants. `diagnosis.json` records the initial matching whole-object
workbench result without publishing native listings or objects.

Four C89 domain/actual-source/layout/receipt tests pass with pedantic-errors,
Wall/Wextra/Werror, ASan and UBSan. Only leak detection is disabled for executor
ptrace compatibility; no candidate/C-harness heap allocation is present.
Fresh setup, all manifests and439 extents /99,120 B agree; the existing12-byte
getter strictly matches. All631 existing cloud setup, guard, submission,
integrity and scorer regressions pass.

```sh
python3 cloud/work/boot_tail/BT03-emitter-handle/preflight.py
python3 cloud/work/boot_tail/BT03-emitter-handle/verify.py --check
python3 cloud/work/boot_tail/BT03-emitter-handle/abi_proof.py --check
python3 cloud/work/boot_tail/BT03-emitter-handle/test_semantics.py
```

Receipts bind the selected C hash, compiler, targets, object/function lengths,
proof fields and test-model hashes. Independent source/domain review and exact
aggregate-head CI are separate central gates. STATUS/D10 and publication belong
to central integration; merging belongs to the independent maintainer. No ROM,
raw assembly, object, credentials or unrelated data is published.

## Independent review

A separate Astra reviewer approved source commit
`754e6d0cfc35799e3cdc895c4834c37dd03a88d3`, tree
`8833df679e972e9c1e986bc5286871ccb954c550`, after checking the complete native
body, direct13DEC caller, ten helper interfaces, peer Git blobs, pinned source
and CC0 license, all packet replays and631 regressions. `REVIEW.json` binds the
exact source and qualified result. The canonical changed-submission gate,
protected guard,161 locks and whitespace pass. This metadata-only handoff
changes no C source, test or model.

The reviewer also ran4,096 deterministic seed754 random cases alongside the
786 stock cases under host GCC O2/ASan/UBSan:3,090 safe,1,790 unwritten-output
rejections and2 capacity rejections, including986 carry cases/3,363 carried
reads. Only uninitialized/maybe-uninitialized warnings were downgraded from
errors for this supplemental probe; the original source was unchanged. The
reviewer-run generator, seed and script hash are recorded in the receipt; this
is supplemental verification metadata, not an added packet test. Another
selected cloud suite passed266 tests with3 documented optional-tool skips.
These results do not broaden the defined-path domain or remove the original
producer/listener/fade-value and final opaque-call qualifications.
