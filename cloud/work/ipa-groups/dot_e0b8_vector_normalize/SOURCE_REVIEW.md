# E0B8 source review: genuine length-helper boundary

## Result

Source/provenance review passes for the direct-input/in-place formulation in `cloud/work/ipa-groups/dot_e0b8_vector_normalize/group.c`. An independent replay with the unchanged canonical scorer reports:

- `func_8008E098`: 0/8 differing words, no extras, unresolved references, unverified references, or errors.
- `func_8008E0B8`: 0/35 differing words, no extras, unresolved references, unverified references, or errors.

Recipe: `-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul`. Both ordinary externally linked functions are retained as public roots. Only E0B8 is a new claim; the previously accepted E098 body remains unchanged. No acceptance options or comparison masks were used.

Reviewed source SHA-256: `dae0d6c43b1d3cf25e41f0d7d625214aff3c8a3bd7303513b515441d30f7d43f`.
Reviewed `cloud/work/ipa-groups/dot_e0b8_vector_normalize/group.json` SHA-256: `138e49b581ef400b5601d240e8346a131b5a44671df7ef4d0f1b3e53618095a5`.

Rechecked the final packaged source on 2026-10-05 at approximately 17:35 UTC using both canonical commands:

```sh
python3 tools/cloud/score.py group cloud/work/ipa-groups/dot_e0b8_vector_normalize
python3 tools/cloud/score.py group cloud/work/ipa-groups/dot_e0b8_vector_normalize --claims
```

Both returned exit 0. The full-member run matched both functions; the claims run matched E0B8 and reported E098 as matching informational context. Removing comments and whitespace confirms the final source has identical code to the first independently reviewed strict-match formulation.

## Why this is a genuine source explanation

E098 is an existing, adjacent, independently strict-matched three-scalar vector-length function. The candidate reuses its unchanged body rather than introducing a fabricated helper. E0B8 passes the three input components directly to it, checks the returned length, computes one reciprocal, multiplies all three original components in place, and returns the original length. Every argument, local, arithmetic operation, memory access, branch, and return serves the native operation.

The compiler generates the native 140-byte caller, including its 40-byte frame and live z home/reloads, without padding, unused locals, volatile accesses, manually taken addresses, diagnostic conditions, hand assembly, register controls, or changes to tools/targets. The absence of a runtime call is explained by ordinary O3 inlining. The companion [stock-stage analysis](COMPILER_CAUSE.md) and [receipt](stock_stage_receipt.json), reviewed against the same final source/spec hashes, identify z's home as an optimizer-generated spill after inlining, rather than a source-exposed address. Neither function is disguised as a static stand-in.

The earlier negative helper experiment pre-captured x/y/z into caller locals. That source canonicalized to a frameless baseline. It was a negative result for that spelling only. Direct input arguments followed by in-place updates produce the match. No further variants are warranted.

## Provenance limits

The adjacent retail helper and exact compiler result support this reconstruction. No original N64 source or exact arcade donor for E0B8 has been identified. The checked [original Rush The Rock fmath.c](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/LIB/fmath.c) normalizers have different arithmetic and guards; the MB math implementation and included prototype file are absent from that donor tree. Do not describe this as recovered original source.

This replay establishes strict function matching, not accepted cartridge coverage. The final packaged-source hashes and both strict scorer modes have been checked here. Independent integration review, image/ROM identity, and acceptance gates remain the integrator's responsibility. No merge is authorized by this review.
