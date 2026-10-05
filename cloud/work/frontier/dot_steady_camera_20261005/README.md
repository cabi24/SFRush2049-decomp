# Authentic steady-camera vector phases close E95DC

**Strict MATCH: `func_800E95DC`, 0x800E95DC–0x800E9C70, 1,684 bytes / 421 words.**
Accepted-byte gain is **zero** until independent integration. No image, compression,
whole-program shadow-unit, gameplay or ROM SHA-1 result is claimed.

Base: `e0e734babdac3c6a79d2f87f7f895e34aa170148` (2026-10-05). The current
lock and open PRs #107–109 were checked before this claim. The parent confirmed
no competing E95DC worker. Those PR targets, the active C448/C588 and E79F8
work, D11BC/D1248, and the D348C/D3B28 camera closure were not changed.

## New source evidence and the bounded change

The authentic donor is `historicalsource/rushtherock` at
`845329d7b36f5a384c5625ed9a0aef584ab46139`:

- [camera.c, steady_move_cam, lines 1400–1457](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/camera.c#L1400-L1457)
  performs `vecsub` followed by `scalmul` in all three delta calculations.
- [vecmath.h, lines 16–30](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/vecmath.h#L16-L30)
  gives the corresponding complete-vector macros.
- [vecmath.c, lines 35–47 and 74–78](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/vecmath.c#L35-L78)
  independently confirms complete subtraction before scaling in the function
  form. This is not a claim that the N64 necessarily used the macros.

Locally verified donor SHA-256 values:

- camera.c: `48a6a113b5b88908bc23c0ac8904b10af2a25dff7bc7e8aa68622aecabf3efb9`
- vecmath.h: `76526aaa1894d9cef992ea07fa524cf7348e02cbede5a646a66f04d6e317cf3f`
- vecmath.c: `7909033d4e95a86d94dc14b96539b272dbd9e8fd7f7b7e67f6b8cbbc0ca2ee31`

The prior wave-5 body combined each component's subtraction and multiply into
one expression. Wave 5 and wave 6 documented its remaining two-instruction
as1 dispatch/delay-slot difference. The current fixed controls preserve the
original declarations, all three accepted context functions, and the existing
keep list/compiler recipe. Replacing just the three fused vector calculations
with the donor's subtraction phase followed by scaling closes the residual.

The retained `candidate.c` uses plain struct-field statements. It needs no new
macro, array representation, local, guard, helper, formal, padding, volatile
qualifier or dummy read. The group's accepted E92C8 source already contains
historical shaping, including its documented buffer; this packet preserves it
byte-for-byte and does not independently establish its original-source form.
The first case's overwritten delta calculation is inherited from the existing
N64 reconstruction and native instructions, not invented pressure work.

Fixed controls in `steady_camera_experiments.py`:

| Source condition | Strict differing words | ELF function bytes |
|---|---:|---:|
| Existing fused body | 112 / 421 | 1,684 |
| Only first vector calculation split | 112 / 421 | 1,684 |
| Only second vector calculation split | 112 / 421 | 1,684 |
| Only third vector calculation split | 112 / 421 | 1,684 |
| Last two vector calculations split | 112 / 421 | 1,684 |
| All three donor calculations split | **0 / 421** | **1,684** |

The 112 positional words in the old body reflect the known one-word movement,
not 112 independent operations. Workbench's shift-aware comparison reports two
geometry edits, while the final body has zero geometry edits. Initial fixed
copy/add/subtract macro-only probes and array-type-only controls did not improve
the baseline. A full donor macro version and a plain split-statement version
both matched. The retained statement version leaves every original declaration
unchanged. No compiler-instrumentation oracle was used to produce the match.

## Complete object and context proof

`verify.py` derives a temporary group by replacing only the real E95DC body in
`src/blob/groups/func_800E92C8/group.c`. It copies that group's JSON unchanged.
The source-bound receipt records:

- All 421 words and exact 1,684-byte ELF `STT_FUNC` extent.
- All 54 target-body relocations, including both real E92C8 calls.
- The complete four-byte owned `0.1f` literal at 0x801244D0.
- Twelve zero section-alignment bytes after the function, excluded from the claim.
- Unchanged E92C8 (788 bytes), EA108 (468), and EA2DC (280): all strict MATCH and
  independently GNU-linked full-body equality for the final candidate unit.
  Every fixed source control separately strict-scores those three bodies.

For independent GNU linking, the temporary object is given equivalent named
relocations for its five section-relative calls to the real E92C8 function.
Each conversion proves `function offset + new addend == original section
addend`. Only relocation representation changes; the original object remains
untouched. GNU then applies every relocation with the actual native symbol
identities and the selected function's independently checked literal placement.
This is a per-function link proof, not placement of the whole original group
as one contiguous image region. The conversion records are included explicitly. ELF class, endianness, ABI flags, and every complete non-debug section are hashed; `.mdebug` source-path metadata is excluded from portable replay, never code or relocation bytes.

## Native, host and arithmetic verification

720 deterministic cases compare an independent scalar binary32 model, unchanged
candidate host C89+UBSan, protected native instructions, and independently
GNU-linked candidate instructions. The native runs also execute the actual
E92C8 rear-camera body and its three native matrix/interpolation callees:
`func_8009E820`, `func_800A61B0`, and `func_800CFDEC`.

The cases exercise all four slots; initialization; legs -1/0/1/2/3; both state
flag outcomes; absent/present view selection; view values 1/2/4; elapsed-time
boundaries; zero/positive/negative bounded dt; camera modes 2/4/8; zero, low,
threshold and above-threshold speeds; positive/negative rear offsets; and three
matrix families. Ordered external-call snapshots and all modeled output fields
agree. Native register preservation, stack canaries, object integrity and all
memory outside the explicitly writable regions are checked.

420/421 target instructions execute. Static CFG analysis independently finds
exactly those same reachable offsets. The sole unexecuted instruction, +0x400,
is the original case-1 move whose delay-slot copy and retargeted branch bypass
it; complete-byte equality still covers it. Native helper coverage is 195/197,
37/37, 37/37 and 34/34 instructions respectively. Five wrong-contract source
mutants (rear distance, height lift, equality transition, initial position and
view reset) are rejected with deterministic witnesses. Real temporary ELF mutations of an instruction, owned literal, and function extent are independently refused.

Four remaining external calls are explicit bounded test models: the initial
look-direction operation copies its input to the observed output; init-view
records invocation; selection returns a configured value; and camera placement
records both vectors and writes their sum. Their real internals and gameplay
are **not** proven by these tests. No concurrent mutation, invalid pointers,
NaN payload/FCSR behavior, or arbitrary out-of-range player indexing is claimed.

## Reproduce and integrate

```sh
python3 tools/cloud/score.py group cloud/work/ipa-groups/dot_steady_camera_20261005 --claims
python3 cloud/work/frontier/dot_steady_camera_20261005/verify.py
python3 -m pytest -q tests/cloud/test_dot_steady_camera.py
```

Use the pinned existing IDO and GNU MIPS tools. The verifier emits metadata only;
all object files, linked binaries and target instruction views stay temporary.
No protected source, accepted lock, compiler recipe, production keep list,
BSS/table ownership or target manifest is edited. The actual source deliverable
is `candidate.c`; its context comes from the existing accepted group. The normal
`cloud/work/ipa-groups/dot_steady_camera_20261005` submission contains exactly
that derived source and claims only E95DC. Its three previously accepted
bodies are context, with the existing compiler flags and keep list unchanged.
The verifier compiles this registered source after checking its exact derivation
and binds both submitted files in the receipt; the generic PR rescorer finds it.

Independent review and the full shadow-unit, source-built image, compression
and ROM gates remain required before acceptance. Do not splice the larger
caller closure or count the already accepted camera bodies again.

## Final local checks

- Nine packet tests plus scorer, own-data, protected-path and submission
  regressions: **757 passed**, zero failures or skips. The registration test
  checks exact derived source, unchanged compiler/keep recipe and discovery.
- Existing static locks: **402/402 intact**.
- Current master advanced to `31b2799e` during the task. E95DC remains unaccepted;
  the selected native region and accepted camera-group source/recipe are unchanged.
  Its unrelated target-manifest annotation change is retained as provenance rather
  than treated as code. Every selected runtime body and complete non-debug object
  section still has a strict source-bound digest.
- The full Conveyor/Cloud suite and remote CI were not run by this worker.
  No publication or CI watching was performed.
