# C974 call contract prerequisite: a removed false float dependency

Research only. No candidate compile, match, source/data coverage, or integration.
Base: `0bfebc7367ebc1ddb4d6105b2012ed07fb080faf`.

## New bounded hypothesis and result

The D02 scout proposed checking whether the generated `var_f12` entry dependency
in `func_8010C974` belongs to a genuine helper contract. This packet tests that
specific hypothesis against both native bodies, rather than sweeping C variants.
The dependency is false: the call is three pointers, and `camera_trigger_check`
locally defines f12 on every path to its first use.

At caller address `0x8010CB48`, native setup is:

- a0: state pointer plus 68 (`0x44`).
- a1: caller stack plus 352 (`0x160`).
- a2: caller stack plus 316 (`0x13C`), installed in the call delay slot.

The generated seed instead uses `camera_trigger_check(var_f12, state+0x44,
&sp160)`. Its pointer-to-float entry assignment, extra float argument, and omitted
third pointer are artifacts. A future reconstruction must preserve the native
three-pointer contract. This is **not** an endorsement of compiling the rest of
that seed: it contains other unresolved types and the unknown external below.
No seed, generated header, accepted helper, or protected target is modified.

The callee at `0x800C4200` moves a0 into its input cursor; it spills a1/a2 to their
O32 argument homes at its sp+324/sp+328. It reads float coordinates through the
input cursor. The first read of f12 is at `0x800C42FC`. Exhaustive prefix path
exploration, including branch-likely annulment, proves a local f12 definition
before that read. Thus incoming f12 is not needed for this call. This proof is
narrow: it does not establish complete callee semantics, floating-point corner
cases, global ABI, or original source types.

The second pointer is used for three float output stores at offsets 0,4,8;
the third is passed to the native matrix-copy helper and later consumed by C974
as a 3x3 float matrix. These observations support vector/matrix views but do not
prove original typedefs. This packet deliberately does not edit shared prototypes.

## Hard stopping condition: unknown external contract

The native call at `0x8010D2C4` really targets `0x8038D798`; it is not a JAL decoder
or metadata error. Setup passes the same pointer in a0/a1, a signed state byte
in a2, word `0x43C80000` in a3, and integer 1600 in the fifth stack slot. The a3
word could represent float 400.0, but a literal pattern alone does not prove its
type. The caller explicitly saves/reloads its t0 pointer across this call.

No symbol at the address is present in manifest-authenticated blob symbols.
A tracked-source address search at the base finds only the existing seed's
undeclared `func_8038d798` call; no implementation or verified contract was found.
This is not proof that no runtime implementation exists. It is insufficient to
supply a reliable typed declaration or authentic compiler closure from this
checkout. Full reconstruction stops here as required by D02's prerequisite.
No speculative helper or fake caller was written.

## Reproduction, extent, and limits

`python3 cloud/work/c974_call_contract/audit.py`

`python3 -m pytest -q tests/conveyor/test_c974_call_contract.py`

The audit uses the canonical scorer's authenticated target loader, asserts the
specific argument setup and direct destinations, and explores the callee's
straight-line/conditional prefix to the first f12 read. It is intentionally not
a general MIPS interpreter. Tests include a false incoming dependency, register
precision, an annulled delay-slot trap, and rejection of calls in the proof region.

Full native extents are 2,636 bytes/659 words (caller) and 1,232 bytes/308 words
(callee), with hashes in `evidence.json`. No C candidate was compiled, so candidate
size, differing words, and unverified relocations are unavailable, not zero.
No compiler-match, sanitizer/gameplay, image, compression, or ROM gate was run.
Native binary/disassembly inspection files remain ignored under build/. No raw
instruction array, extracted object, or ROM data is published by this artifact.

This advances one concrete prerequisite beyond PR42's proposal, while retaining
its no-go outcome for full execution. Resume only when the external implementation
or trustworthy contract is available, then reconstruct the entire C974 source
with the corrected three-pointer call. No additional tuning is queued.

## Independent review

Lane A independently replayed the exact audit JSON and initial five tests, then
checked native caller setup and the callee prefix. Review approved the narrow
prerequisite claim and suggested rejecting unsupported COP1 formats explicitly.
That hardening and a sixth test were added; all six focused tests pass.
