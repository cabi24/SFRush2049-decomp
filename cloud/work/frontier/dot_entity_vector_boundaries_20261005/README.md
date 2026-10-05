# Collision correction: authentic vector-boundary controls

## Result

**NONMATCH. Eight fixed builds reject the proposed source-boundary explanation.
No better matching candidate, no claims, and zero accepted-byte or ROM-coverage
gain.** The existing B76 direct-arithmetic source remains unchanged.

Target `entity_iterate`, **0x800C69C0–0x800C6AA0**, is the complete 224-byte,
56-word collision endpoint correction. The historical name is retained.
Base: `e24b47d89a0c8ffade1e4c75ad76b9d390a1c232`, fetched from master before
work. The target was unlocked; open PRs #110–121 and the parent's current
address claims were checked before the exact interval was reserved. No excluded
range, shared accepted source, symbol, target, compiler recipe or lock changed.

The new question was whether genuine vector helper boundaries, or the newly
accepted vector-normalization source context, explain the native 88-byte frame
and floating-point schedule. Historical
`cloud/work/near_miss_B76` tested direct scalar expressions and a named-vector
view; it did not test these authentic operations or this accepted context.

| Fixed control | Complete ELF bytes | Frame | Whole-body differing words |
|---|---:|---:|---:|
| Archived direct O3 | 224 | 80 | 29 |
| Authentic `vecsub` function | 224 | 96 | 29 |
| Authentic `scalmul` then `vecadd` | 300 | 128 | 64 |
| All three authentic functions | 300 | 144 | 64 |
| Authentic `SubVector` / `ScaleAddVector` macros | 224 | 80 | 29 |
| Archived direct + accepted normalization context | 224 | 80 | 29 |
| Three vector functions + accepted normalization context | 300 | 144 | 64 |
| Two macros + accepted normalization context | 224 | 80 | 29 |

The 300-byte bodies contain **19 excess ELF words**. All are counted, including
any zeros; no target-sized truncation is used. The `scalmul` donor retains its
actual three-element loop, and ordinary compilation does not produce the native
straight-line correction. The vector-subtraction boundary increases the frame
past the native size. Macros and accepted normalization context do not change
the old residual.

Both complete accepted normalization-context bodies remain independently
GNU-linked matches in all three groups: E098 is 32 bytes and E0B8 is 140 bytes.
The canonical scorer separately attributes one nonzero inter-body word to E0B8
in the three-vector-function group. Its actual ELF symbol remains exactly 140
bytes and every independently linked word agrees. The receipt retains that
canonical diagnostic alongside the full-symbol result. No context coverage is
new and no contiguous original translation-unit placement is asserted.

## Source and provenance

`donor_vectors.h` preserves the substantive `vecsub`, `scalmul` and `vecadd`
bodies from [game/vecmath.c](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/vecmath.c).
Only F32 spelling and local static linkage are adapted. The two macros come
unchanged from [LIB/fmath.h](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/LIB/fmath.h).
`provenance.json` records pinned commit, whole-file SHA-256, Git blob ID and URLs.
No explicit license notice appears in these donor files; none is invented.

Only helpers actually called by a particular control are emitted in its source.
The generated source hashes are recorded for every control. `candidate.c` is a
readable macro-based **negative control**, not a proposed match or established
whole-function arcade ancestor. The precise N64 source spelling and original
helper boundaries remain unknown.

The accepted normalization source is copied byte-for-byte from
`src/blob/groups/frontier_vector_normalize/group.c`. All three genuine public
roots are retained. The group adds no replacement callees, keepers, fake
arguments, unused locals, unsupported volatile accesses or explicit inline
annotations. Real vector and basis storage remain 12 and 36 bytes in every
control. Opaque frame filler and alternative matrix capacities were not tried.

## Native contract and bounded behavior

The actual function takes two ordinary pointers to three binary32 components.
It calls `input_deadzone_apply(second, first, &basis, 0.5f, 1, 6)`. When the
collision result is nonnull it computes second minus the callback-updated first
vector, normalizes the result through E0B8, then adds one quarter of that result
to the first vector. It returns zero on both paths. The native caller frame is
88 bytes, with the difference at +36 and nine-float basis at +48.

The verifier establishes:

- Exact ELF STT_FUNC extents, every full-body relocation, independent GNU MIPS
  linking and complete-body comparison for all eight controls and six accepted
  context-body instances. Candidate controls have two direct-call relocations,
  no owned literals/data/tables, and no unresolved or masked reference.
- The actual protected E0B8 normalizer executes during native and GNU-linked
  caller replay. Its external threshold at 0x8012394C is read from the
  manifest-verified data artifact and bound by hash; it is not invented fixture
  data. Accepted E098/E0B8 source is independently linked in the context proof.
- **1,920 cases**, **1,920 protected-target runs**, **15,360 independently
  GNU-linked control runs**, and **15,360 unchanged-caller host C89+UBSan runs**
  agree with a separately expressed binary32 oracle.
- All 56 native target instructions and both outcomes of its conditional
  branch execute. The actual normalizer executes 34 of its 35 instructions;
  no all-instruction coverage claim is made for it. Loop-bearing control bodies
  execute 74/75 instructions; the remaining instruction is still included in
  the full-extent comparison.
- Both collision results, endpoint mutation before return, exact first/second
  pointer aliasing, signed zero, small/threshold-adjacent vectors and varied
  finite values are covered. Ordered callback/vector snapshots, vector outputs,
  zero return, O32 saved registers and outer memory/stack canaries are checked.
- Three compiled wrong-source contracts (wrong correction amount, reversed
  difference and omitted normalization), an unknown opcode and a wrong call
  destination are rejected. Packet tests additionally mutate a real ELF size
  and a real relocation type and require refusal.

`input_deadzone_apply` is explicitly a bounded external callback model. Its
actual internals and full callers are not executed. The host compiles each
caller unchanged; it separately compiles the accepted normalizer body with
only its public symbol renamed so a test wrapper can capture its incoming
vector. No native/host proof of the collision engine is claimed.

The domain excludes NaNs, infinities, FCSR flags/exceptions, partial pointer
aliasing, invalid pointers, concurrency, real collision-scene selection and
whole-game behavior. Memory traces are checked for allowed accesses and
canaries; different compiler controls are not claimed to have identical
internal read/write schedules. No shadow-unit, source-built image, compression,
full-ROM, hosted CI or gameplay result is claimed.

## Reproduction and stop condition

With the pinned IDO toolchain and GNU MIPS binutils available:

```sh
python3 cloud/work/frontier/dot_entity_vector_boundaries_20261005/verify.py --write
REQUIRE_TOOLCHAIN=1 python3 -m pytest -q tests/conveyor/test_entity_vector_boundaries.py
python3 -m tools.conveyor.pipeline.lock check
```

The source-bound `verification.json` records every control, native identity,
compiler hashes, donor identities, public call census and bounded behavior.
Native words, assembly dumps, objects, ROM bytes and private credentials are
excluded from the deliverable. Claims are empty and matching-submission
scanning must schedule zero submissions.

Validation on this frozen source: **765 tests in the combined selected suite
pass without skips or failures**, including **10 packet tests**, with
`REQUIRE_TOOLCHAIN=1`. The selected suite covers packet replay,
scorer/integrity/guard/submission checks, the prior
source-boundary scout and E0B8 verification. All **402 static locks** remain
intact. This is a selected suite, not a full-project suite.

Stop here. A future attempt needs genuine original storage/interface evidence
explaining the four-byte vector/basis displacement and eight-byte frame deficit,
or a concrete different operation boundary predicting the native floating
schedule. Neither adding generic vector helpers nor exposing accepted
normalization context supplies that evidence. Do not repeat these controls or
turn the frame deficit into arbitrary source padding.
