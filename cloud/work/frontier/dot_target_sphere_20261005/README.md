# Donor-backed target sphere callback

**Strict MATCH: 260 bytes / 65 words. Zero accepted-byte or ROM-coverage gain.**

The historical `steering_apply` label names a sphere-overlap callback at
`[0x8010C6C8, 0x8010C7CC)`. Ordinary IDO O3 and O2 reproduce its entire ELF
function, including all four relocations. It has a 32-byte frame and twelve
zero section-alignment bytes outside the 260-byte function. No owned literal,
initialized data, BSS, or jump-table bytes are involved.

The source is in `cloud/matches/steering_apply.c`; existing changed-submission
CI automatically discovers and strictly scores it. This packet changes no
production source, shared declarations, protected target, lock, scorer, compiler
recipe, or keep list. It is matching-candidate evidence for independent review,
not integration, a full-game compilation, a source-built image or ROM acceptance.

## Selection and new evidence

The base is freshly fetched master `e24b47d8`. Current locks, fetched branches,
open PRs 110–119 and active parent-coordinated claims were checked before the
exact target was claimed. The target is disjoint from the two cylinder callbacks
in PR #111 and the other active engine, scene, frontend and audio work.

The authentic algorithmic donor is `OverlapTarget`, at pinned rushtherock
revision `845329d7b36f5a384c5625ed9a0aef584ab46139`:

- [targets.c, lines 673–693](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/targets.c#L673-L693)
- [vecmath.h, lines 20–21](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/vecmath.h#L20-L21)

The donor supplies the collidable gate, selected car position, three-axis
squared distance, inflated radius, optional gap output and boolean return.
Crucially, it declares the used `dsq, gap` scalars before the vector, computes
distance before inflating radius, and directly returns the comparison. PR #111's
neighboring cylinder reconstruction supplied new corroboration of the first two
source details. No exact original N64 source declaration or source-unit boundary
is claimed.

Native differences from the arcade are authoritative: selector and radius are
passed by pointer, the origin is snapshotted, and the radius margin is 3.5 rather
than 11.2. The enabled-byte and position-record views have strides 2,056 and 952;
the position starts at offset eight. Unknown structure storage represents
observed layout, not dummy local padding.

The historical B71 source is unchanged. Fresh fixed controls establish:

| Source | Complete ELF bytes | Scorer differing words | Complete GNU differing positions | Frame |
|---|---:|---:|---:|---:|
| Historical B71 | 252 | 47/65 | 47 | 24 |
| Final donor-backed source | 260 | 0/65 | 0 | 32 |
| Vectors declared before scalars | 260 | 2/65 | 2 | 24 |
| Radius computed before distance | 256 | 31/65 | 31 | 32 |
| Explicit if/return instead of donor boolean return | 268 | 34/65 | 36 | 32 |

The final control includes two excess words in the complete GNU count. The
scorer's target-sized differing count is not misrepresented as its full extent.
These are bounded ablations of one source combination, not claims that original
spelling is unique. Every final local is used. There are no filler locals,
unused parameters, dead reads, dummy conditions, unsupported volatile, assembly,
stand-ins, synthetic keepers or broad allocation/spelling sweeps.

## Complete verification

`verify.py` checks the unchanged final source through:

- Complete ELF `STT_FUNC` extents and GNU ld relocation independent of the
  project relocator. All 65 words and four HI16/LO16 relocations match. The twelve
  outside-function alignment bytes are separately checked to be zero.
- Explicit absence of owned literal/data sections, O32 type-size/offset asserts,
  pinned compiler-stage hashes, source hashes and protected-manifest identities.
- A genuine three-body O3 context containing the candidate and unchanged accepted
  `func_8010C02C` and `func_8010C7CC`. Every member remains a full match; each
  complete body is also GNU-linked at its native address. These are adjacent
  callback-family regression bodies with their existing independent type views,
  not a recovered contiguous original translation unit or proof of a caller.
- 18,171 protected-native/GNU-linked/independent binary32 oracle/unchanged host-C
  cases, totaling 36,342 MIPS executions. The host source is compiled as C89 with
  UBSan and floating contraction disabled. All 65 instruction offsets execute;
  both outcomes of all three conditional branches execute.
- Nine output arrangements: null, separate, any origin component, any selected
  position component, or the radius itself. Full input/record canaries, exact
  six stack writes, stack bounds, disabled-path access behavior, global/return
  and callee-saved registers, and output store shape are checked.
- Boundary neighbors, signed zeros, subnormals, large finite values, infinities,
  NaNs, three-axis equality, and deterministic arbitrary-bit/random finite cases.
  Five compiled wrong-contract mutants are rejected: excluded equality, wrong
  radius margin, wrong axis, cylinder geometry and reversed enable gate.
- 65,536 additional native cases, one for every signed-halfword selector pattern,
  check sign extension and effective addresses against separately calculated
  accessible selected-record addresses. These are native address-contract tests;
  negative/out-of-population selectors are not claimed to be valid host-C arrays
  or actual game states.

Protected code contains no direct JAL to this function. The protected opaque-data
artifact contains one aligned function pointer at `0x80117518`. This corroborates
callback registration, without proving its runtime dispatcher or caller contract.
Both the code and opaque-data manifests are validated before scanning.

All 835 selected packet/scorer/integrity/submission/guard/owned-data and storage
regressions pass, including sixteen packet tests, with no failures or skips. Both
scorer sanity examples and all 402 static-lock guards pass. Sparse checkout
initially omitted existing test/lock fixtures; these were materialized unchanged
from the pinned base before the final successful runs.

The regression file includes unchanged-host/native checks, independent boundary
examples, complete ELF/GNU checks, rejected shortened/inflated ELF extents,
rejected instruction corruption and redirected stack writes, unknown-opcode and
invalid-memory rejection, likely-branch annulment, and CI submission discovery.

## Reproduce

With the pinned IDO and GNU MIPS tools on PATH:

```sh
python3 cloud/work/frontier/dot_target_sphere_20261005/verify.py \
  --donor /path/to/rushtherock --output /tmp/sphere-verification.json
python3 -m pytest tests/conveyor/test_dot_target_sphere.py
python3 tools/cloud/score.py fn cloud/matches/steering_apply.c steering_apply \
  --flags '-g0 -O3 -mips2 -G 0 -non_shared'
```

The optional donor argument checks both HEAD and the exact working file bytes
against the pinned Git objects. The receipt contains hashes, counts and symbol
addresses, with no ROM bytes, raw assembly dumps, object files or donor checkout.
Object hashes are build provenance; portable replay does not mask any code,
relocation, complete-extent or data differences.

## Limits

The main host/native corpus assumes a readable signed selector, radius, selected
enabled record and, when enabled, a readable origin and selected position.
Eight positive host slots are test capacity, not a recovered game population.
The separate exhaustive selector proof supplies accessible native records only.
NaNs compare by class; exact NaN payload propagation, signaling-NaN behavior,
FCSR flags/exceptions, other rounding modes, concurrency, corrupt/invalid
pointers, actual gameplay, the runtime dispatcher, full-game shadow/source-image,
compression and ROM SHA-1 gates are outside this proof. No full-suite or remote
CI pass is claimed. Publication and merging remain with the parent and independent
checker respectively.
