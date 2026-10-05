# Car collision force/peak helper: source-backed strict match

Base: `cc4d5fdd`, fetched 2026-10-05. Target `func_800DED78`, complete
`[0x800DED78, 0x800DEF60)`, 488 bytes / 122 words.

**Matching research, zero accepted-byte or ROM-coverage credit.** The helper
matches under the normal O3 group pipeline with its real caller, and also in the complete archived two-entry audio closure. The caller
remains nonmatching and is explicitly unclaimed. No splice, full shadow-unit,
image, compression or ROM build was performed.

## What changed

The authentic arcade `game/carsnd.c` `get_force_and_peak` body supplies the
nested force/peak state machine, direct indexed records, declaration list and
integer zero spellings. The N64 adds a player dimension, uses five 24-byte
impact records per player and measures time in float seconds. The new source
keeps a single meaningful local, `force`, and uses a typed vector parameter.

The old complete model-audio closure reproduces **35/122 differing words**.
The donor-shaped body, with the source-supported clock qualification, reaches
one difference: the saved narrow index parameter's stack home. Preserving the
arcade's index-first declaration order, with the added N64 player parameter
second, recovers the native home at entry SP+0. All formals are substantively
consumed. Real caller argument order is adapted at its two actual call sites.

There are no unused parameters, artificial locals, dead reads, dummy helpers,
stand-in callers, assembly shaping, altered compiler flags or scorer changes.
The current five accepted list/audio helper definitions are reused unchanged
only for a genuine caller-context regression.

## Clock qualification has independent source and native support

The pinned donor is historicalsource/rushtherock at
[`845329d7b36f5a384c5625ed9a0aef584ab46139`](https://github.com/historicalsource/rushtherock/tree/845329d7b36f5a384c5625ed9a0aef584ab46139):

- [`game/carsnd.c`, lines 671–712](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/carsnd.c#L671-L712)
  contains `get_force_and_peak`, including repeated IRQTIME observations.
- [`game/globals.h`, line 210](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/globals.h#L210)
  declares IRQTIME as VS32.
- [`LIB/stdtypes.h`, line 33](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/LIB/stdtypes.h#L33)
  defines VS32 as volatile long.

The N64's float-seconds counterpart is address `0x8002EB90`, independently named
`__osScElapsedTime` by the static symbol map. Its native `viTickStart` and
`viUpdateTime` writers reference that same location. In DED78 the continuing
path reads the clock at +0x1B0 and again at +0x1D0 with no intervening store or
call. The verifier checks that witness and the writer aliases. This supports
repeated observation; it does not claim to prove the original N64 header or
that a particular interrupt interleaving occurs in gameplay.

The ordinary-clock control is 20 bytes shorter and fails 94/122 words. The
player-first control has the full extent but fails the single home word. Both
are bounded causal controls, not a search over volatile qualifiers or formals.

## Verification

Run from the repository root with the pinned recovery/toolchain environment:

```sh
python3 cloud/work/frontier/dot_audio_force_20261005/verify.py
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_dot_audio_force_packet.py -q -o addopts=''
```

The verifier materializes the genuine archived caller and header, applying the index/player declaration and argument-order correction, and
normalizing only the generated D_8002EB90 declaration to volatile. The same
clock-only normalization is applied to the generated full audio core so every
translation unit agrees; archived and accepted files remain unchanged. The
archive baseline and ordinary-clock control retain consistently nonvolatile
clock declarations. The helper is internal; the real caller remains an exported entry. No production keep-list
is changed. Its ordinary O32 export shape is not claimed: native IPA carries
the threshold in f18 and permits the helper to clobber f20.

- Canonical strict comparison: all 122 words, no unresolved/unverified relocation,
  no errors and no excess words. Exact ELF extent is required separately.
- All 12 relocations are audited. Independent GNU ld resolves the complete
  helper at its native address and proves all 488 bytes without masking.
- The source-owned `0.1f` literal is separately proved against all four native
  bytes at `0x80124320`. A wrong-literal control still has zero masked instruction
  differences but is refused by the unchanged own-data gate.
- No alignment bytes are included in the helper claim. Its next ELF symbol
  begins immediately at +488. The following native eight-byte stub belongs to
  neither this body nor this claim.
- The expanded genuine caller/list context retains strict matches for all five
  unchanged accepted definitions: `func_80092278`, `entity_flags_apply`,
  `high_scores_display`, `func_8009211C`, `func_80091FBC`.
- 18,737 stable-clock cases compare unchanged host C with UBSan, an independently
  written binary32 state oracle, protected native code and GNU-linked code.
  They cover all players/slots, signed zeros, force/peak/timeout boundaries,
  narrow index conversions, no-op guards, random vectors, and selected infinities,
  underflow, overflow and quiet-NaN cases. Four additional scripted changing-clock
  cases verify repeated observations in native/linked code against the oracle.
  Total native executions: 37,482.
- Four meaningful source mutants are rejected: wrong peak comparison, wrong
  threshold comparison, wrong timeout boundary and inverted bump guard.
- 120 of 122 instruction offsets execute. The two unexecuted offsets, +0xE0
  and +0xEC, are leftover load locations bypassed after as1 copied their loads
  into incoming branch-delay slots; they are still covered by full-byte equality.
  Every memory access is checked for mapping/alignment, control flow is bounded,
  unknown opcodes fail closed and register/stack preservation is checked against
  the measured IPA contract.

The replay is bounded, not an N64 emulator or full game test. FCSR exception
flags and signaling-NaN payload identity are outside its model. Changing-clock
cases are not a host-thread race experiment. Caller table placement and caller
behavior are not covered by the helper's proof.

## Caller limits and integration

The real `mode_select_handler` caller remains a broad nonmatch: 742/744 positional
words, 3,524-byte candidate versus 2,976-byte native, plus unresolved own-table
placement evidence. The public receipt preserves counts, sites and the table
failure category without publishing raw compared instructions. No blanket
context acceptance or full-closure byte match is implied. Five accepted leaf
context bodies pass individually in this group.

Focused packet tests pass 10/10, with no skips. The unchanged `test_cloud_score.py`,
`test_cloud_guard.py` and `test_cloud_submissions.py` tooling tests pass 700/700.
These are scoped suites, not a complete game build.

The full Conveyor/Cloud suite on the pre-normalization packet commit `0cb47075`
reported 2,228 passed, 64 skipped and seven failures. All seven reproduce on
untouched base `cc4d5fdd`: three tests need the missing m2c checkout, two cleanup
subprocesses and the agent-loop fixture cannot import the local decomp-permuter,
and the smoke test lacks a Conveyor token. The final generated-clock-only
normalization is covered by the fresh ten-test packet replay; the full suite was
not rerun after that scoped change. No source-test failure is hidden as a pass.

Only source, tests, counts, hashes and research notes belong in this packet.
Objects, target streams and disassembly stay in ignored temporary/build paths.
The independent checker owns source admission, full-unit and cartridge gates,
and merging.
