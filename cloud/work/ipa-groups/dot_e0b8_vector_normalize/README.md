# `func_8008E0B8`: exact source match through the real vector-length helper

## Result and scope

`func_8008E0B8`, `[0x8008E0B8, 0x8008E144)`, matches all **35 words / 140 bytes**
with the unchanged canonical cloud scorer and pinned stock IDO 5.3 compiler.
Its ordinary C source computes the length through the real adjacent
`func_8008E098`, returns positive zero without changing the vector when the length
is at most `D_8012394C`, otherwise normalizes the three coordinates and returns
the original length.

Both genuine public entry points are kept in the ordinary whole-program O3
pipeline. `func_8008E098` remains an exact **8-word / 32-byte** match. Its body is
identical to the existing accepted `src/blob/groups/dot_vector_length/group.c`
implementation. Only E0B8 is a new claim; the already accepted helper earns no
additional credit.

This is a **compiled-source submission**, not an image splice, promotion, ROM
build, or cartridge-coverage claim. Production source, accepted groups, locks,
protected targets, compiler, scorer and build flags are unchanged. Future
integration must consolidate the existing E098 group with this group rather than
emit two definitions of E098, and run the ordinary image and ROM gates.

## Reproduce

Use the compiler installation pinned by `tools/cloud/setup.sh` (or `IDO_DIR`
pointing to that installation):

```sh
python3 tools/cloud/score.py group cloud/work/ipa-groups/dot_e0b8_vector_normalize
python3 tools/cloud/score.py group cloud/work/ipa-groups/dot_e0b8_vector_normalize --claims
```

The first command must report `MATCH` for both functions. The claims command
judges E0B8 and reports the unchanged E098 helper as context. The literal recipe
is `-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul`, through the existing
`cc -j -> uld -> usplit -> umerge -> uopt -> ugen -> as1` group path.
No relocation waiver or masked score is used.

## Why the source matches

The earlier clean intrinsic-only candidate is faithful but compiles without a
frame and differs at all 35 target positions. An archived 13-word residual
manufactures the frame with six unused integer declarations and a dead address;
that source is not used or legitimized here.

The decisive source boundary is the real three-scalar length call **before**
direct in-place vector updates. Pre-capturing all three input components into
caller locals before the same call makes the inliner erase that boundary and
reproduces the frameless nonmatch. Adding a redundant snapshot after the call
preserves extra lifetimes and produces a 48-byte frame with excess code. Calling
the proven helper directly on the three vector elements and then updating the
vector in place reproduces the native body without any source-address exposure.

Stock retained-stage capture identifies the cause: inlining introduces actual
formal-argument lifetimes. The optimizer retains the values needed again for
the output, introduces its own third-component spill at frame offset -36, and
reuses the former argument colors for length and reciprocal. The generator
rounds that real 36-byte extent to the native 40-byte frame, placing the spill
at stack +4. See `COMPILER_CAUSE.md` for the measured pass boundary and identity
gates. No modified compiler or forced allocation is used for the match.

## Native contract and source provenance

- One ordinary pointer argument refers to three single-precision components.
- Arithmetic is `(x*x + y*y) + z*z`, intrinsic single-precision square root,
  then one reciprocal followed by three multiplies.
- The threshold is the real float at `D_8012394C`, independently verified as
  binary32 `1e-5f` against the private original image and the complete protected
  function inventory. The source retains its real external reference.
- The `<=` early return is intentional: unordered/NaN comparisons take the
  normalization path. Replacing this with a positive `>` guard changes behavior.
- All inputs and threshold are read before the first output write. Below the
  cutoff even tiny nonzero inputs and signed-zero components are preserved.
- One of the nine discovered direct callers consumes the original float length;
  a `void` or integer-return declaration is invalid.

The source uses only two caller scalar locals and the already accepted real
helper. It has no invented formal, fake helper, dummy storage, padding, volatile
access, no-op diagnostic, address-taking trick, extra arithmetic, inline assembly,
compiler patch or target/scorer change. The match establishes a legitimate
source representation and compilation boundary; it does not establish the
historical developers' exact source spelling or an exact arcade donor.

## Verification

The [independent receipt and executable regression verifier](../../e0b8_verification/README.md) accompany this
packet. They verify complete ELF function extents and relocated bodies, including
independent GNU-linker agreement, the retained helper, trailing alignment, strict
zero results, compiler/source/protected-input hashes and semantic edge cases.
The native audit, source review and compiler-cause note supply independent checks.

Raw target words, assembly dumps, compiler streams, object files, image data,
ROM bytes and credentials are not part of the submission.

### Local test results

- Final focused E0B8 and scorer tests: **662 passed**, including 18 dedicated
  checks and the complete four-way behavioral replay.
- Non-node Conveyor suite with exact pinned local dependencies: **1,779 passed,
  41 skipped, 9 deselected**.
- `make check-matched`: **all 383 locked static functions intact**.
- C89 syntax, Python byte compilation and whitespace checks passed.
- The broader cloud/scorer run had **997 passed, 18 skipped, 2 failed**. Both
  failures are stale manifest receipts in `test_dot_fresh_masked_rng.py` and
  `test_task_enqueue_research.py`; an independent clean-base replay reproduced
  precisely those failures. They are disclosed, not repaired in this task. The
  18 new E0B8 tests were collected and passed separately afterward.

See `validation.json` for exact commands, counts and the clean-base comparison,
`native_contract.json` for original-image-backed threshold/caller evidence, and
`SOURCE_REVIEW.md` for the independent source-provenance review.
