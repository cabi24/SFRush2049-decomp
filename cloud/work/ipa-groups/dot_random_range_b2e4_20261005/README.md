# Source-backed range RNG: strict match

New candidate: `func_8008B2E4`, **[0x8008B2E4, 0x8008B32C), 72 bytes / 18 words**.
The complete function is a strict MATCH in a genuine two-body O3 unit. Its
unchanged accepted `func_8008B2B4` rand implementation also remains exact.
Only B2E4 is claimed. No accepted-byte, image, compression or ROM gain is claimed.

## Actual donor and the decisive expression

The source lead is [Rush The Rock LIB/fmath.c: Random, lines 801–808](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/LIB/fmath.c#L801-L808),
pinned at `845329d7b36f5a384c5625ed9a0aef584ab46139`; `claim.json` records its
file hash. The full upstream donor is not redistributed.

This was an old near-match, not a newly discovered function. The library hunt
and camera/path packets already identified its range scaling, inline LCG and
three-/four-word register residuals. The coordinator also reported an earlier
13-control rand-context pass whose exact spellings were not available here.
No claim of first-ever source exploration is made.

The authentic arcade wrapper computes `rand() & 0x7FFF`, despite the underlying
rand already returning 15 bits, and assigns the scaled result to a real float
`rannum`. Keeping that actual outer mask closes the old three-word temp-register
residual. A one-change reversion removing the outer mask reproduces **3/18**.
A direct-return control still matches, so the float output local is authentic
source style, not the cause of the improvement. The older complete standalone
library-hunt source also reproduces **3/18** at its exact 72-byte extent.

The N64 target performs single-precision multiplication and division by
`32768.0f`. Arcade uses a double `32767.0` denominator; that literal is not
silently carried into the N64 reconstruction. A double-32768 control is
**12/18 with three nonzero excess words**, proving it is not the native recipe.
The source uses no artificial reads, padding, unused arguments, pressure locals,
volatile changes, stand-in helpers, arbitrary keepers or compiler modifications.

## Contract and source unit

Input is one standard O32 float in f12; the return is f0. The global seed is a
32-bit word at `D_8011735C`. Each call advances the seed by the ANSI-style LCG,
extracts bits 16–30, converts that integer to float, multiplies by the requested
range, then divides by 32768 in binary32. It has no calls or frame after genuine
rand inlining. The wrapper contains the original meaningful mask, even though
its runtime arithmetic effect is redundant.

`rand.c` is byte-for-byte the existing accepted `src/blob/func_8008B2B4.c`.
The two native functions are the only kept roots and the only function definitions
in the unit. `group.json` registers B2E4 as the sole member/claim and B2B4 as
accepted context. The latter earns no duplicate matching credit. The current
production group reader independently resolves and reproduces both full bodies.
This is a real local source unit, not proof of the original complete translation
unit or of every inlined caller. No direct native `jal` caller for B2E4 was found
by the **main-game protected-target census**, which did not cover runtime images.
[PR #147](https://github.com/cabi24/SFRush2049-decomp/pull/147) establishes four
direct calls in runtime image **B**: `0x803913D8`, `0x803913F4`, `0x80391410`
and `0x8039142C`, all targeting `0x8008B2E4` from `func_8039133C`.
The [pinned caller evidence](https://github.com/cabi24/SFRush2049-decomp/blob/8636cabec85efc040f79c19108adb30d023aa88b/cloud/work/runtime_b_object_phases_20261006/README.md#complete-contract)
belongs to image B, ROM stream `0xB6FEC4`, image SHA-256
`b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd`.
Runtime image A overlaps this address space and is not interchangeable with B.

The accepted rand uses signed arithmetic whose overflow is undefined in portable
ISO C. IDO's observed MIPS code wraps modulo 2^32. Host tests compile the
unchanged source with `-fwrapv` so that wrapping is defined there too; they do not
claim portable ISO C behavior for signed overflow. No accepted source is edited.

## Complete-object and behavior evidence

`verify.py` checks actual ELF STT_FUNC extents: B2B4 is 48 bytes and B2E4 is 72.
All 30 relocated words match, with no unknown references, masks, owned data,
literals or extra function words. There are eight zero alignment bytes after
the complete pair. All four relocations are genuine HI16/LO16 seed references.
GNU readelf/nm independently confirm the extents; GNU ld places the pair at its
native addresses with `SUBALIGN(4)`, and GNU objcopy reproduces every byte.
The unmodified production group reader supplies a third relocation check.

**135,204 cases / 270,408 native executions** agree among:

- Protected native instructions
- Independently GNU-linked complete candidate
- Separately written arithmetic oracle
- Actual unchanged C pair compiled for the host with UBSan and `-fwrapv`

Every possible 15-bit random output is exercised at four scales, plus varied
finite scales, seed endpoints, signed zero and power-of-two scales. All 18 native
instruction offsets execute. Tests check the updated seed, exact result bits,
one confined seed write, unchanged stack/callee-saved registers and complete
host output. Three compiled wrong-contract controls are rejected: a 14-bit mask,
the wrong denominator, and ignoring the scale sign. Decoder tests fail closed
on unknown instructions and redirected writes.

The behavioral domain is finite normal scales/products plus signed zero under
default rounding. It excludes NaN, infinity, subnormal arithmetic, overflow and
nondefault rounding, and does not establish gameplay or floating exception flags.

## Reproduce and limits

From the repository root with IDO and GNU MIPS tools configured:

```sh
python3 tools/cloud/score.py group cloud/work/ipa-groups/dot_random_range_b2e4_20261005 --claims
python3 cloud/work/ipa-groups/dot_random_range_b2e4_20261005/verify.py --output /tmp/range-rng-replay.json
python3 -m pytest -q tests/cloud/test_random_range_contract.py
```

Base is `cd22879d40b3de443cfde047b86e75e159b6cec6`. Exact range/lock and current
assignment checks preceded source changes; unpublished work cannot be excluded.
Frozen receipts contain metadata, hashes and counts only. No native words,
assembly dumps, ROM bytes, binary objects, credentials or unrelated private data
are submitted. No protected targets, locks, contexts or build recipes change.
The final scoped regression run passes **743 tests**, including all seven new
packet checks plus scorer, submission, protected-path, target-integrity and setup
tests. Both documented scorer sanity examples pass. Independent review, full-game
shadow, image, compression, ROM gates and merging remain with the independent
checker.
