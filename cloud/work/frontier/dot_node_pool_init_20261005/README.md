# 100-node free-list initialization

`func_800B2BDC`, 0x800B2BDC–0x800B2CB4, **216 bytes / 54 words**: strict
**MATCH** under ordinary IDO O3 and O2. This is new matching-candidate evidence,
with **zero accepted-byte or ROM-coverage gain**. Base: master
`e24b47d89a0c8ffade1e4c75ad76b9d390a1c232`.

## Source admission and the new evidence

The archived `tiny_A46/func_800B2BDC.c` already reconstructs the correct operation:
initialize a 100-node free chain, set every signed-halfword id to -1, terminate
node 99, and clear the active-list head and allocation count. Its pointer-bound traversal emits 80 bytes and
has 53 complete differing positions out of 54. A46's historical indexed control
was a 228-byte, 51-word nonmatch. This packet does not claim a newly recovered
algorithm or an arcade donor.

The source change is the real array's definition in its owning translation unit,
with a natural indexed loop and next-pointer-before-id order. Native peeled
stores already combine multiple accesses beneath one symbol base; the old extern
array cannot reproduce that addressing. The now-supported BSS workflow provides
a direct, independently linkable way to test the actual ownership hypothesis.
The 24-byte layout, 100-element extent and stores are grounded in the native
initializer and its independently accepted consumer, `func_80090284`.

Fixed ablations of the final source are reproduced in the receipt:

- Actual defined array, indexed loop: 216-byte complete MATCH.
- Only change the array definition to extern: 228 bytes, 51 complete differing
  positions, including all three excess words.
- Exchange the two loop stores: 216 bytes, 14 differing words.
- Put the two loop stores on one line: still MATCH. A compact exploratory source
  had an 11-word scheduling residual, but this controlled result does not support
  attributing the final match solely to that line break. No layout search follows.
- Recompile the unchanged archived pointer-bound source: 80 bytes, 53 differing
  complete-body positions, including the missing suffix.

The final Node definition is compatible with the unchanged accepted pop
allocator, including its pointer, halfword, float and flag fields. A genuine
O3 two-body unit preserves both full bodies. No stand-ins, artificial formals,
dead reads, unused locals, inline tricks, volatile shaping, padding locals,
protected compiler recipe changes, keep-list changes, or accepted-source edits.
The `padA[2]` field is the actual O32 gap at offsets 10–11, preserved by both
routines; it is not stack-shaping storage.

The active-head pointer type for D_801391F0 is supported by the real
`func_80090308` constructor: it saves the accepted allocator's returned node,
then links the old D_801391F0 into node->next and installs that node as the new
head. This corrects the archived initializer's untyped word interpretation.
The native call/save/link sites are checked; the full constructor is not replayed.

The pinned arcade checkout at `845329d7b36f5a384c5625ed9a0aef584ab46139`
was searched, but no whole-function ancestor was established. The source remains
an N64 reconstruction. Original names, original file boundaries and original
C spelling are not claimed.

## Complete object and storage proof

`verify.py` checks the exact ELF STT_FUNC extent, all 54 instruction words and
all **33 HI16/LO16 relocations** through the unmodified scorer and independent
GNU MIPS linking. Eight zero text-alignment bytes are outside the 216-byte body
and receive no credit. No literal, read-only data or initialized data section is
owned by this candidate.

The named **2,400-byte BSS object** is independently linked at
0x80138880–0x801391E0. Its complete symbol/section extent is checked and lies
within the documented boot-cleared game BSS range. Nine separately compiled
O32 assertions check all used field offsets, the node size and the array size.
They do not modify the matching translation unit. BSS storage is not ROM bytes
and earns no code coverage. This checks the candidate's storage placement, not
whole-game ownership/overlap integration or the original declaration.

The complete genuine context also matches:
- Initializer: 216 bytes
- Unchanged accepted `func_80090284` free-list pop: 132 bytes, no new credit

A direct-call census finds the initializer in `func_800BB9B0` at 0x800BC104.
That real caller is not recompiled or executed by this packet. Indirect calls
are not excluded. Legacy declarations elsewhere in the full program are not
reconciled here; complete shared-type/shadow-unit validation remains required.

## Bounded behavior

**320 deterministic cases**, **30,528 native/GNU-linked function executions**:

- All **54 initializer** and **33 consumer** instruction offsets execute; both
  outcomes of the initializer's loop branch are covered.
- Four randomized complete initial-memory arrangements, eight signed high-water
  values and ten pop counts cover no pop, intermediate consumption, complete
  exhaustion and one further empty-list call.
- Every case initializes twice, preserving all bytes that the initializer does
  not own. The linked/native write/read traces agree. Whole mapped memory,
  surrounding canaries, saved registers and stack remain checked after every
  function call.
- An independent structural oracle builds the chain and advances the free-list
  state. The original C candidate and unchanged accepted consumer compile as
  separate host C89 translation units under UBSan. The host checks full object
  bytes, including padding, after initialization and every pop, then emits a
  pointer-normalized native-layout state for exact comparison with the oracle.
- Host pointers are 64-bit; normalization does not itself prove native ABI.
  The separately compiled O32 assertions and native/GNU executions address ABI.
- Four compiled wrong contracts are rejected: short chain, wrong id, cyclic
  terminal node and omitted counter reset. Unknown native instructions and
  unmapped stores fail closed. Tests corrupt real object instructions, symbol
  extent and relocation identity and require complete verification to reject.

Scope is a mapped, single-threaded 100-node pool, initialized before consumption,
with up to 101 subsequent pops and high-water values in the documented corpus.
No arbitrary aliases, invalid pointers, concurrent modifications, complete
caller runtime, gameplay or hardware timing are modeled. The initializer reads
no data; its prior pool/head/counter values may be arbitrary mapped bytes. The
real consumer's signed-count overflow is outside this initialized, at-most-100
successful-pop domain.

## Reproduction and registration

From the repository root with IDO, GNU MIPS binutils and a host C compiler:

    python3 tools/cloud/score.py fn cloud/matches/func_800B2BDC.c func_800B2BDC --flags '-g0 -O3 -mips2 -G 0 -non_shared'
    python3 cloud/work/frontier/dot_node_pool_init_20261005/verify.py --output /tmp/node-pool-proof.json
    REQUIRE_TOOLCHAIN=1 python3 -m pytest -q tests/cloud/test_node_pool_init_match.py

The bare flags header and `cloud/matches/func_800B2BDC.c` placement register this
as one normal changed-submission check. The receipt binds candidate, accepted
consumer, baseline, harness, target and compiler hashes; no native word dump,
ROM data, compiled object or unrelated private information is included.

Eight focused packet tests and 843 selected packet/scorer/integrity/submission/
guard/layout/context tests pass with required toolchains, no failures or skips.
All 402 static locks pass. The first lock run encountered one absent sparse-
checkout storage source; materializing unchanged tracked prerequisites resolved
it. Normal changed-submission and protected-path checks pass in the final commit. Source admission,
full-game shadow unit, storage-overlap checks, source-built image, compression,
ROM SHA-1 gates and merging remain with the independent checker. No hosted CI,
full-ROM or gameplay pass is claimed, and no CI watcher is started.

## Discarded preflight

Before this target was selected, one isolated B0580 build added the actual
accepted resource-release and pool-initializer bodies to the old A28 caller.
It remained 30/38 with five excess words; all accepted context remained exact.
The parent then confirmed this closure route had been tried earlier. It was
stopped immediately, is not a fresh result, and contributes no claim or packet.
