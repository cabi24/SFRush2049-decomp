# Genuine runtime-B closure: bounded semantic replay

This is research evidence for one approved combined O3 diagnostic. It is not a
matching, acceptance, original-TU, original-signature, or complete-game claim.
No compiler is invoked by these scripts. No target words, ROM bytes, raw assembly
or compiled objects are retained in this directory.

## Result

The final source/ELF-bound receipt is `closure-verification.json`. The suite has
552 paired cases and 856 complete FCE0 invocations per image. Each comparison
executes genuine native/candidate children. No internal member is an external
hook. The prior standalone fixture totals are not used as a closure proof.

The native suite executes 2,815 of 2,844 instruction positions (98.98%). Exact
per-member candidate coverage comes from the parent's compiler-ECOFF/PDR-derived
`../boundaries.json`; code execution includes the entire unchanged linked text.
Unexecuted offsets and observed internal call edges remain explicit in the receipt.
The ten independent negative/fail-closed controls are in `controls.json`.

## Execution and observation

- `adapter.py` reads the pinned base's asset with `git show`, checks asset/image
  fingerprints, and compares all eight complete native bodies against the current
  `score.targets()`. Native bytes exist only in process memory.
- All private JALs enter their actual bodies; J/JR, delay slots, stack traffic and
  the native private register conventions execute directly. Natural candidate
  calling conventions are never rewritten into native conventions.
- Every root return checks SP, GP, s0..s8 and f20..f31 restoration. Unknown
  instructions, targets, unaligned/unmapped accesses and readonly writes fail closed.
- Every writable nonstack byte, including unnamed sentinel bytes, is compared at
  root returns and immediately before each ordered external helper event. Readonly
  asset/candidate constants are protected against writes. Code-layout-dependent
  temporary stack addresses are normalized; passed vectors/matrices are recorded
  by content. This does not assert external pointer-identity equivalence for stack
  temporaries.
- All four pools have actual +0x10 active and +0x14 free heads. Allocation pops
  the free list and prepends the active list; release unlinks the live node and
  pushes it onto the free list. The fixtures use singly linked pools, as required
  by these record layouts. Duplicate/invalid releases are rejected.
- D_8039A530 and D_8039AE90 are physical pool-head aliases. D_80399B54 is the
  same memory as D_80399B18+60. No parallel logical copies mask their effects.
- A78BC produces a real mapped 88-byte quad pointer; releases clear its active
  halfword. Two-frame cases exercise creation, release-node insertion and later
  FCE0 release. The external quad model does not reproduce the unrelated renderer's
  complete allocation/tail globals.
- D054 receives both genuine position arguments, returns distinct mapped attached
  effects, and performs its verified entry-SP+8 argument-home write.

## Bounded domains and helper limits

Fixtures use initialized aligned finite normal-or-zero binary32 values, default
rounding, signed player counts no greater than four, F938 owners 0..3, valid table
indices, and successful required object/release allocations. Root record/debris
failure and null-quad branches are covered. The model rejects D498 zero-distance
impulse paths rather than inventing NaN/exception behavior. FCSR, exceptional and
subnormal arithmetic, broader owner domains, pool exhaustion on unchecked paths,
and arbitrary game lifetime remain outside this result.

All original signed-byte hit-mask instructions execute. Mask values including
0x80 and 0xff are covered, but owner0..3 does not distinguish signed versus unsigned
extension in high mask bits; no claim of high-owner closure coverage is made.

Ordinary external services are deterministic bounded contracts. Matrix transforms
use binary32 ordered row/column products; pitch/yaw/scale helpers use a documented
finite deterministic effect rather than asserting N64 trigonometric equivalence.
Random and atan helpers supply bounded outputs. Damage, collision, scene effects,
sound and player updates record their observed arguments and modeled side effects.
Selected callbacks mutate caller-visible positions, matrices, counts and velocities
so later caller reads execute. Their full game dependencies are not part of the
closure proof. Every private family member is executed, not modeled this way.

## Reproduce without another compilation

From the workspace containing the existing approved diagnostic ELF:

```sh
PYTHONDONTWRITEBYTECODE=1 python3 runtime-b-closure-map/semantics/verify.py \
  --reference-root rush-sixth-baseline \
  --linked-elf closure-diagnostic-tmp-ZNENOp/closure.elf \
  --boundaries runtime-b-closure-map/boundaries.json \
  --source runtime-b-closure-map/closure.c \
  --output runtime-b-closure-map/semantics/closure-verification.json
PYTHONDONTWRITEBYTECODE=1 python3 runtime-b-closure-map/semantics/controls.py \
  --reference-root rush-sixth-baseline \
  --output runtime-b-closure-map/semantics/controls.json
```

The parent owns that unique temporary compiler directory and removes it after its
independent replay. It is not an artifact to publish. A later authorized rebuild
must pass the new ELF path and matching authenticated boundary/source receipt;
these scripts will not silently compile a replacement. Omitting `--linked-elf`
performs explicitly labeled native self-replay only, never candidate verification.

`closure-verification.json` represents the final 552-case candidate run.
The final receipt binds the complete linked ELF, source, boundary inventory,
and all four harness source files by SHA-256. The current canonical scorer is
used read-only without an integration-sensitive implementation fingerprint.
