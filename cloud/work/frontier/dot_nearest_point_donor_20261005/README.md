# Nearest-point donor boundary: rejected hypothesis

Research only. **No match, improved candidate, accepted bytes or ROM gain.**
The target is `func_800D2C10`, `[0x800D2C10, 0x800D2CD4)`, 196 bytes / 49 words.
Base: `e24b47d89a0c8ffade1e4c75ad76b9d390a1c232`, checked 2026-10-05.
The function is absent from the current game lock and outside open PRs #110–115.
Its sole direct JAL caller in the protected game population is
`stunt_combo_display` at `0x800D4474`; indirect callers are not excluded.

## New hypothesis and outcome

The existing B6 source uses three scalar differences. It already preserves the
native float-register lane, but has an 8-byte frame against native 32. Earlier
research rejected a padding result array and failed to suppress large unrolling
with the native signed loop counter. Those are prior failures, not new leads.

This pass asks a narrower source-boundary question: does an actually consumed
three-float vector, built by the authentic `mvecsub` macro, account for the
missing frame? Does using the authentic `dotprod` body under ordinary whole-unit
O3 supply the missing inlining context?

Donor: historicalsource/rushtherock commit
`845329d7b36f5a384c5625ed9a0aef584ab46139`:

- [game/vecmath.h, lines 17–21](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/vecmath.h#L17-L21): `mvecsub`.
- [game/vecmath.c, lines 108–110](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/vecmath.c#L108-L110): `dotprod`.

These are genuine reusable operation bodies. **An original whole-function
arcade ancestor or original N64 helper boundary is not established.**
`vector_macro.c` and `vector_dotprod.c` are controlled reconstructions, not new
matching submissions. Every vector element and source local participates in the
actual distance/selection calculation. No padding, dead reads, unused formals,
volatile shaping, invented side effects, keepers or stand-ins were introduced.

The hypothesis fails. The real consumed vector accounts for only eight extra
frame bytes, taking the candidate to 16, still short of native 32. The helper
fully inlines in the ordinary group, and does not change this outcome. Signed
induction still triggers the old unrolling failure. O1 gives larger bodies and
40-byte frames. There was no allocation, declaration-order or line-layout sweep.

## Fixed complete-body results

The verifier regenerates these eight controls, including the unchanged earlier
source. Function sizes are ELF `STT_FUNC` extents; excess counts include all
extra instructions, including zeros inside the function. Alignment outside its
extent is excluded.

| Control | ELF bytes | Frame | Different native positions | Excess ELF words |
| --- | ---: | ---: | ---: | ---: |
| Earlier unsigned source, O2 | 196 | 8 | 26/49 | 0 |
| Earlier unsigned source, O3 | 196 | 8 | 26/49 | 0 |
| Macro vector, unsigned, O3 | 216 | 16 | 43/49 | 5 |
| Macro vector + genuine dotprod, unsigned, O3 group | 216 | 16 | 43/49 | 5 |
| Macro vector, signed, O3 | 816 | 16 | 42/49 | 155 |
| Macro vector + genuine dotprod, signed, O3 group | 816 | 16 | 42/49 | 155 |
| Macro vector, signed, O1 | 304 | 40 | 49/49 | 27 |
| Earlier signed source, O1 | 264 | 40 | 48/49 | 17 |

The source-bound receipt records ordinary flags, compiler/input/source hashes,
complete body hashes, all four relocations per body, canonical comparison fields
and independently GNU-linked full-body equality for every control. The target,
scorer, accepted sources and recipes are unchanged. The canonical nonzero-excess
metric is deliberately recorded separately from the full ELF excess count.

A fresh workbench diagnosis of the historical unsigned baseline reports a
structural mismatch, a 24-byte non-save-frame deficit and identical seven-slot
float-register lane. Its relocation warnings arise because the private native
fixture has absolute words; the independent GNU-link and full-word checks in
`verify.py` resolve actual candidate relocations and are the authoritative
comparison. No raw instruction stream or binary is included here.

The initial standalone, externally visible dotprod control retained a call and
an unresolved helper reference, so it was rejected rather than scored as an
inlining result. The registered eight-control replay uses the ordinary genuine
whole-unit boundary, retaining only the real external root. This distinguishes
inlining from the unsupported assumption that plain single-file O3 must inline.

## Reproduction and limits

Source the pinned build environment, then run:

    python3 cloud/work/frontier/dot_nearest_point_donor_20261005/verify.py
    python3 -m pytest -q tests/cloud/test_nearest_point_donor.py

All five packet tests and the 764-test packet/scorer/submission/guard/source-inventory/owned-data selection pass, with no skips or failures. The full suite was not run.

A successful verifier exit means the negative experiment reproduced, not MATCH.
This is compiler-boundary research, not a behavioral proof. The reconstruction
retains the native uninitialized-result issue when no point beats the initial
threshold; no defined-host claim is made for that case. Table population,
run-time thresholds, invalid selectors, complete caller execution, full-game
shadow compilation, source-built image, compression and ROM gates are untested.

The prior 26/49 candidate is unchanged. Reopen only for concrete original scratch
layout or inlining-context evidence that predicts the additional 16 bytes and
native signed loop shape together. Do not repeat vector spelling or padding
experiments. This packet is intentionally outside submission directories and
has no claims; automatic matching discovery must schedule zero submissions.
